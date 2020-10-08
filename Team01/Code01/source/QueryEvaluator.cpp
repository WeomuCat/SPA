#include "QueryEvaluator.h"
#include <queue>

QueryEvaluator::QueryEvaluator(PKB pkb) {
	this->pkb = pkb;
}

QUERY_RESULT QueryEvaluator::evaluateQuery(PROCESSED_SYNONYMS synonyms, PROCESSED_CLAUSES clauses) {
	ResultList result_list;
	BOOLEAN query_bool = true;
	const QUERY_RESULT no_result = QUERY_RESULT();
	const QUERY_RESULT boolean_true_result = { "TRUE" };
	const QUERY_RESULT boolean_false_result = { "FALSE" };


	// If root of clauses tree is unassigned, then the clause is syntactically incorrect. Return no result.
	if (clauses.getNodeType() == QueryNodeType::unassigned) {
		return no_result;
	}

	QUERY_NODE_LIST children = clauses.getChildren();
	if (children.size() == 0) {
		throw "Error: Select has no children, it should have at least 1 child (ie Select v)";
	}

	/* NOTE FOR ITER 2: NEED TO CHECK RETURN_SYNONYM FIRST, MIGHT BE BOOLEAN AND NOT SYNONYM */
	QueryNode return_value = children[0];
	setEvaluatorReturnType(return_value);
	if (return_type == QueryEvaluatorReturnType::synonym) {

		// If no synonyms are declared, Query is INVALID. Return no result.
		if (synonyms.size() == 0) {
			return no_result;
		}

		// Get return type and return synonym name
		QueryNode return_synonym = return_value;
		QuerySynonymType return_synonym_type = return_synonym.getSynonymType();
		SYNONYM_NAME return_synonym_name = return_synonym.getString(); // "v"
		this->return_synonym_names = return_synonym_name;

		// fill resultList with return_synonym_name
		fillWithReturnSynonym(return_synonym_type, return_synonym_name, result_list);
	}
	else if (return_type == QueryEvaluatorReturnType::tuple) {

		// If no synonyms are declared, Query is INVALID. Return no result.
		if (synonyms.size() == 0) {
			return no_result;
		}

		// Check with hui ming the data structure of how multiple return synonym names will be stored
		
		/*
		std::vector<std::string> all_return_synonyms = ...
		this->return_synonym_names = all_return_synonyms;
		*/
	} else if (return_type == QueryEvaluatorReturnType::boolean) {
		// if there are no clauses, automatically return true. For BOOLEAN, synonyms need not be declared.
		if (children.size() == 1) {
			return boolean_true_result;
		}

	}
	else {
		throw "QE: Return type is invalid";
	}


	// for all clauses in Select (not including the synonym)
	for (size_t i = 1; i < children.size(); i++) {
		bool clause_bool = false;
		ResultList clause_result_list;

		QueryNode clause = children[i];
		QueryNodeType clause_type = clause.getNodeType();

		if (clause_type == QueryNodeType::such_that) {
			QueryNode relationship = clause.getChildren()[0];
			QueryNodeType relationship_type = relationship.getNodeType();
			QueryNode child1 = relationship.getChildren()[0];
			QueryNode child2 = relationship.getChildren()[1];
			QueryNodeType child1_type = child1.getNodeType();
			QueryNodeType child2_type = child2.getNodeType();

			// Get the result of the relationship from pkb, fill in clause_result_list, get clause_bool
			Relationship such_that_relationship = Relationship(relationship_type, child1, child2);
			such_that_relationship.getRelationshipResult(pkb, clause_bool, clause_result_list);

		}
		else if (clause_type == QueryNodeType::pattern) {
			
			// a (_, _)
			QueryNode pattern = clause;
			QueryNode child1 = pattern.getChildren()[0];
			QueryNode child2 = pattern.getChildren()[1];
			QueryNode child3 = pattern.getChildren()[2];
			AST_NODE child3_ast = child3.getAstNode();
			
			QueryNodeType child2_type = child2.getNodeType();
			QueryNodeType child3_type = child3.getNodeType();
			
			SYNONYM_NAME assign_synonym_name = child1.getString();
			
			// ResultList contains the assign synonym + variable synonym
			// a v
			// 1 cat
			// 2 dog
			// 5 meow
			// ...
			clause_result_list.addColumn(assign_synonym_name);
		
			// pattern a (v, _)
			// add LHS search strings into lhs_set
			std::unordered_set<VAR_NAME> lhs_set;

			// if qpp_lhs is ident, add exact to lhs_set
			if (child2_type == QueryNodeType::ident) {
				STRING child2_content = child2.getString();
				lhs_set.insert(child2_content);

			// if qpp_lhs is synonym, add all synonym to lhs_set
			} else if (child2_type == QueryNodeType::synonym) {
				STRING child2_synonym_name = child2.getString();
				QuerySynonymType child2_synonym_type = child2.getSynonymType();
				if (child2_synonym_type == QuerySynonymType::variable) {
					std::vector<SYNONYM_NAME> var_names = pkb.getVariableNameList();
					for (SYNONYM_NAME var_name : var_names) {
						lhs_set.insert(var_name);
					}
					// if child2 is a synonym, then we add to ResultList
					clause_result_list.addColumn(child2_synonym_name);
				}
				else {
					// check if other synonyms are valid
					// if child2 is a synonym which is not a variable, we throw an exception 
					throw "QE: child2 synonym of pattern clause must be a variable!";
				}

			// if qpp_lhs is wildcard, add all variables to lhs_set
			} else if (child2_type == QueryNodeType::wild_card) {
				for (VAR_NAME name : pkb.getVariableNameList()) {
					lhs_set.insert(name);
				}
			}
			
			// Iterate through all assign nodes
			std::vector<ASSIGN_NODE_PTR> pkb_assigns = pkb.getAssigns();
			for (ASSIGN_NODE_PTR assign_node : pkb_assigns) {
				std::string stmt_num = std::to_string(assign_node->getStatementNumber());
				VAR_NODE_PTR lhs_node = assign_node->getVariableNode();
				VAR_NAME lhs_var_name = lhs_node->getVariableName();
				EXPR_NODE_PTR rhs_node = assign_node->getExpressionNode();

				// if lhs_var_name is not in qpp_lhs_set, skip this assign node
				if (lhs_set.count(lhs_var_name) == 0) {
					continue;
				}
				
				// if qpp_rhs is a wildcard, add to ResultList
				if (child3_type == QueryNodeType::wild_card) {
					ROW row;
					row.insert({ assign_synonym_name, stmt_num });
					// if lhs is a variable synonym, add it to the ResultList :)
					if (child2_type == QueryNodeType::synonym) {
						STRING child2_synonym_name = child2.getString();
						row.insert({ child2_synonym_name, lhs_var_name });
					}
					clause_result_list.addRow(row);
				}
				else if (child3_type == QueryNodeType::expression) {
					// if qpp_rhs is a partial match, find in AST (rhs)
					// if found, add to ResultList 
					AST_NODE_PTR child3_ast = child3.getAstNode();
					EXPR_NODE_PTR expr_node = std::static_pointer_cast<ExpressionNode>(child3_ast);
					
					// Get constant/variable name of expr_node
					std::string search_name;
					AST_NODE_PTR expr_node_lhs = expr_node->getLeftAstNode();
					NODE_TYPE expr_node_lhs_type = expr_node_lhs->getNodeType();
					if (expr_node_lhs_type == NodeTypeEnum::constantNode) {
						CONSTANT_NODE_PTR node = std::static_pointer_cast<ConstantNode>(expr_node_lhs);
						search_name = node->getValue();
					} else if (expr_node_lhs_type == NodeTypeEnum::variableNode) {
						VAR_NODE_PTR node = std::static_pointer_cast<VariableNode>(expr_node_lhs);
						search_name = node->getVariableName();
					}

					bool match = findPartialPattern(rhs_node, search_name);

					// Pattern found, add to ResultList!
					if (match) {
						ROW row;
						row.insert({ assign_synonym_name, stmt_num });
						// if lhs is a variable synonym, add it to the ResultList :)
						if (child2_type == QueryNodeType::synonym) {
							STRING child2_synonym_name = child2.getString();
							row.insert({ child2_synonym_name, lhs_var_name });
						}
						clause_result_list.addRow(row);
					}
				}
			}
			
			// if result_list is empty, clause_bool = false
			// Return a ResultList of assign synonyms
			clause_bool = (clause_result_list.getNumRows() > 0);
		}
			

		// if the clause_bool is true => merge result_list with clause_result_list
		// if the clause_bool is false => 
		//		if return type is synonym => return no result list
		//		if return type is boolean => return false result list
		if (clause_bool) {
			result_list = ResultListManager::merge(result_list, clause_result_list);
		}
		else {
			if (return_type == QueryEvaluatorReturnType::synonym) {
				return no_result;
			}
			if (return_type == QueryEvaluatorReturnType::boolean) {
				return boolean_false_result;
			}
		}
	}

	if (return_type == QueryEvaluatorReturnType::synonym) {
		// convert result_list to string output
		return ResultListManager::getSynonymValues(result_list, return_synonym_names);

	}
	else if (return_type == QueryEvaluatorReturnType::boolean) {
		
		if (result_list.getNumRows() > 0) {
			return boolean_true_result;
		} else {
			return boolean_false_result;
		}
	}
	else {
		//handle tuple return result here
	}
}

void QueryEvaluator::fillWithReturnSynonym(QuerySynonymType return_synonym_type, SYNONYM_NAME return_synonym_name, ResultList &result_list) {
	if (return_synonym_type == QuerySynonymType::assign) {
		result_list.addColumn(return_synonym_name, pkb.getAssignNumList());
	}
	else if (return_synonym_type == QuerySynonymType::constant) {
		result_list.addColumn(return_synonym_name, pkb.getConstantValueList());
	}
	else if (return_synonym_type == QuerySynonymType::ifs) {
		result_list.addColumn(return_synonym_name, pkb.getIfNumList());
	}
	else if (return_synonym_type == QuerySynonymType::print) {
		result_list.addColumn(return_synonym_name, pkb.getPrintNumList());
	}
	else if (return_synonym_type == QuerySynonymType::procedure) {
		result_list.addColumn(return_synonym_name, pkb.getProcedureNameList());
	}
	else if (return_synonym_type == QuerySynonymType::read) {
		result_list.addColumn(return_synonym_name, pkb.getReadNumList());
	}
	else if (return_synonym_type == QuerySynonymType::stmt) {
		result_list.addColumn(return_synonym_name, pkb.getStatementNumList());
	}
	else if (return_synonym_type == QuerySynonymType::variable) {
		result_list.addColumn(return_synonym_name, pkb.getVariableNameList());
	}
	else if (return_synonym_type == QuerySynonymType::whiles) {
		result_list.addColumn(return_synonym_name, pkb.getWhileNumList());
	}
}

bool QueryEvaluator::findPartialPattern(AST_NODE_PTR ast, std::string search_name) {
	// BFS
	std::queue<AST_NODE_PTR> queue;
	queue.push(ast);

	while (!queue.empty()) {
		AST_NODE_PTR node = queue.front();
		queue.pop();
						
		NODE_TYPE node_type = node->getNodeType();
		if (node_type == NodeTypeEnum::constantNode) {
			CONSTANT_NODE_PTR n = std::static_pointer_cast<ConstantNode>(node);
			VALUE node_name = n->getValue();
			if (node_name.compare(search_name) == 0) {
				return true;
			}
		} else if (node_type == NodeTypeEnum::expressionNode) {
			EXPR_NODE_PTR n = std::static_pointer_cast<ExpressionNode>(node);
			if (n->getExpressionType() == ExpressionTypeEnum::none) {
				queue.push(n->getLeftAstNode());
			} else {
				queue.push(n->getLeftAstNode());
				queue.push(n->getRightAstNode());
			}
		} else if (node_type == NodeTypeEnum::variableNode) {
			VAR_NODE_PTR n = std::static_pointer_cast<VariableNode>(node);
			VAR_NAME node_name = n->getVariableName();
			if (node_name.compare(search_name) == 0) {
				return true;
			}
		}
	}

	return false;
}

void QueryEvaluator::setEvaluatorReturnType(QueryNode select_return) {
	QueryNodeType return_type = select_return.getNodeType();

	if (return_type == QueryNodeType::synonym) {
		this->return_type = { QueryEvaluatorReturnType::synonym };
	}
	else if (return_type == QueryNodeType::tuple) {
		this->return_type = { QueryEvaluatorReturnType::tuple };
	}
	else if(return_type == QueryNodeType::boolean) {
		this->return_type = { QueryEvaluatorReturnType::boolean };
	}
	else {
		throw "QE: Select's return type is not synonym, tuple, boolean";
	}
}
