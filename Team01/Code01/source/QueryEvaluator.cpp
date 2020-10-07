#include "QueryEvaluator.h"
#include <queue>

QueryEvaluator::QueryEvaluator(PKB pkb) {
	this->pkb = pkb;
}

QUERY_RESULT QueryEvaluator::evaluateQuery(PROCESSED_SYNONYMS synonyms, PROCESSED_CLAUSES clauses) {
	ResultList result_list;
	const QUERY_RESULT no_result = QUERY_RESULT();

	// If no synonyms are declared, Query is INVALID. Return no result.
	if (synonyms.size() == 0) {
		return no_result;
	}

	// If root of clauses tree is unassigned, then the clause is syntactically incorrect. Return no result.
	if (clauses.getNodeType() == QueryNodeType::unassigned) {
		return no_result;
	}

	QUERY_NODE_LIST children = clauses.getChildren();
	if (children.size() == 0) {
		throw "Error: Select has no children, it should have at least 1 child (ie Select v)";
	}
	QueryNode return_synonym = children[0];
	QuerySynonymType return_synonym_type = return_synonym.getSynonymType();
	SYNONYM_NAME return_synonym_name = return_synonym.getString(); // "v"

	// fill resultList with return_synonym_name
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

			if (relationship_type == QueryNodeType::follows) {
				getFollowsResult(child1, child2, clause_bool, clause_result_list);
			}
			else if (relationship_type == QueryNodeType::followsT) {
				getFollowsTResult(child1, child2, clause_bool, clause_result_list);
			}
			else if (relationship_type == QueryNodeType::parent) {
				getParentResult(child1, child2, clause_bool, clause_result_list);
			}
			else if (relationship_type == QueryNodeType::parentT) {
				getParentTResult(child1, child2, clause_bool, clause_result_list);
			}
			else if (relationship_type == QueryNodeType::usesS) {
				//std::vector<std::pair<int, std::string>> stmt_cross;
				//std::vector<std::pair<std::string, std::string>> proc_cross;
				if (child1.getSynonymType() != QuerySynonymType::procedure) {
					getUsesSResult(child1, child2, clause_bool, clause_result_list);
				}
				else {
					getUsesPResult(child1, child2, clause_bool, clause_result_list);
				}

			}
			//else if (relationship_type == QueryNodeType::usesP) {}
			else if (relationship_type == QueryNodeType::modifiesS) {
				if (child1.getSynonymType() != QuerySynonymType::procedure) {
					getModifiesSResult(child1, child2, clause_bool, clause_result_list);
				}
				else {
					getModifiesPResult(child1, child2, clause_bool, clause_result_list);
				}
			}
			//else if (relationship_type == QueryNodeType::modifiesP) {}
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
		// if the clause_bool is false => return ""
		if (clause_bool) {
			result_list = ResultListManager::merge(result_list, clause_result_list);
		}
		else {
			return no_result;
		}
	}

	// convert result_list to string output
	return ResultListManager::getSynonymValues(result_list, return_synonym_name);
}



void QueryEvaluator::getFollowsResult(QueryNode child1, QueryNode child2, bool &clause_bool, ResultList &clause_result_list) {
	/* Follows(s1, s2)
		s1 = [1, 2, 3] if assign
		s2 = [1, 2, 3]
		cross = [[1,1], [1,2], [1,3]...]

		filter the cross with PKB

		then add to ResultList
	*/

	// Initialize with list of all values for the given synonym
	QueryNodeType child1_type = child1.getNodeType();
	QueryNodeType child2_type = child2.getNodeType();

	if (isSameSynonymName(child1, child2)) {
		return;
	}

	std::vector<int> list1;
	std::vector<int> list2;

	try {
		list1 = getStmtList(child1);

		list2 = getStmtList(child2);
	}
	catch (const char* msg) {
		std::cout << msg << "\n";
	}


	// create all possible pairs of list1 and list2 values
	std::vector<std::pair<int, int>> cross;
	for (int s1 : list1) {
		for (int s2 : list2) {
			cross.push_back({ s1, s2 });
		}
	}

	// filter the cross with PKB.isFollows
	std::vector<std::pair<int, int>> filter;
	for (std::pair<int, int> p : cross) {
		if (pkb.isFollows(p.first, p.second)) {
			filter.push_back(p);
		}
	}
	
	// after filtering:
	// if the filtered list is empty, then we say that this clause is FALSE
	// else, this clause is TRUE
	clause_bool = (filter.size() > 0);

	// Add the filtered to ResultList!
	// 1. Add the synonym names to as column headers
	if (child1_type == QueryNodeType::synonym) {
		SYNONYM_NAME synonym_name = child1.getString();
		clause_result_list.addColumn(synonym_name);
	}
	if (child2_type == QueryNodeType::synonym) {
		SYNONYM_NAME synonym_name = child2.getString();
		clause_result_list.addColumn(synonym_name);
	}

	// 2. Add synonym values to the Resultlist row wise
	for (std::pair<int, int> p : filter) {
		ROW row;
		if (child1_type == QueryNodeType::synonym) {
			SYNONYM_NAME child1_synonym_name = child1.getString();
			SYNONYM_VALUE child1_synonym_value = std::to_string(p.first);
			row.insert({ child1_synonym_name, child1_synonym_value });
		}
		if (child2_type == QueryNodeType::synonym) {
			SYNONYM_NAME child2_synonym_name = child2.getString();
			SYNONYM_VALUE child2_synonym_value = std::to_string(p.second);
			row.insert({ child2_synonym_name, child2_synonym_value });
		}
		clause_result_list.addRow(row);
	}
}

void QueryEvaluator::getFollowsTResult(QueryNode child1, QueryNode child2, bool& clause_bool, ResultList& clause_result_list) {
	QueryNodeType child1_type = child1.getNodeType();
	QueryNodeType child2_type = child2.getNodeType();

	if (isSameSynonymName(child1, child2)) {
		return;
	}

	std::vector<int> list1 = getStmtList(child1);
	std::vector<int> list2 = getStmtList(child2);

	// create all possible pairs of list1 and list2 values
	std::vector<std::pair<int, int>> cross;
	for (int s1 : list1) {
		for (int s2 : list2) {
			cross.push_back({ s1, s2 });
		}
	}

	// filter the cross with PKB.isFollowsTransitive
	std::vector<std::pair<int, int>> filter;
	for (std::pair<int, int> p : cross) {
		if (pkb.isFollowsTransitive(p.first, p.second)) {
			filter.push_back(p);
		}
	}

	// after filtering:
	// if the filtered list is empty, then we say that this clause is FALSE
	// else, this clause is TRUE
	clause_bool = (filter.size() > 0);

	// Add the filtered to ResultList!
	// 1. Add the synonym names to as column headers
	if (child1_type == QueryNodeType::synonym) {
		SYNONYM_NAME synonym_name = child1.getString();
		clause_result_list.addColumn(synonym_name);
	}
	if (child2_type == QueryNodeType::synonym) {
		SYNONYM_NAME synonym_name = child2.getString();
		clause_result_list.addColumn(synonym_name);
	}

	// 2. Add synonym values to the Resultlist row wise
	for (std::pair<int, int> p : filter) {
		ROW row;
		if (child1_type == QueryNodeType::synonym) {
			SYNONYM_NAME child1_synonym_name = child1.getString();
			SYNONYM_VALUE child1_synonym_value = std::to_string(p.first);
			row.insert({ child1_synonym_name, child1_synonym_value });
		}
		if (child2_type == QueryNodeType::synonym) {
			SYNONYM_NAME child2_synonym_name = child2.getString();
			SYNONYM_VALUE child2_synonym_value = std::to_string(p.second);
			row.insert({ child2_synonym_name, child2_synonym_value });
		}
		clause_result_list.addRow(row);
	}
}

void QueryEvaluator::getParentResult(QueryNode child1, QueryNode child2, bool& clause_bool, ResultList& clause_result_list) {
	QueryNodeType child1_type = child1.getNodeType();
	QueryNodeType child2_type = child2.getNodeType();

	if (isSameSynonymName(child1, child2)) {
		return;
	}

	std::vector<int> list1 = getStmtList(child1);
	std::vector<int> list2 = getStmtList(child2);

	// create all possible pairs of list1 and list2 values
	std::vector<std::pair<int, int>> cross;
	for (int s1 : list1) {
		for (int s2 : list2) {
			cross.push_back({ s1, s2 });
		}
	}

	// filter the cross with PKB.isParent
	std::vector<std::pair<int, int>> filter;
	for (std::pair<int, int> p : cross) {
		if (pkb.isParent(p.first, p.second)) {
			filter.push_back(p);
		}
	}

	// after filtering:
	// if the filtered list is empty, then we say that this clause is FALSE
	// else, this clause is TRUE
	clause_bool = (filter.size() > 0);

	// Add the filtered to ResultList!
	// 1. Add the synonym names to as column headers
	if (child1_type == QueryNodeType::synonym) {
		SYNONYM_NAME synonym_name = child1.getString();
		clause_result_list.addColumn(synonym_name);
	}
	if (child2_type == QueryNodeType::synonym) {
		SYNONYM_NAME synonym_name = child2.getString();
		clause_result_list.addColumn(synonym_name);
	}

	// 2. Add synonym values to the Resultlist row wise
	for (std::pair<int, int> p : filter) {
		ROW row;
		if (child1_type == QueryNodeType::synonym) {
			SYNONYM_NAME child1_synonym_name = child1.getString();
			SYNONYM_VALUE child1_synonym_value = std::to_string(p.first);
			row.insert({ child1_synonym_name, child1_synonym_value });
		}
		if (child2_type == QueryNodeType::synonym) {
			SYNONYM_NAME child2_synonym_name = child2.getString();
			SYNONYM_VALUE child2_synonym_value = std::to_string(p.second);
			row.insert({ child2_synonym_name, child2_synonym_value });
		}
		clause_result_list.addRow(row);
	}
}

void QueryEvaluator::getParentTResult(QueryNode child1, QueryNode child2, bool& clause_bool, ResultList& clause_result_list) {
	QueryNodeType child1_type = child1.getNodeType();
	QueryNodeType child2_type = child2.getNodeType();

	if (isSameSynonymName(child1, child2)) {
		return;
	}

	std::vector<int> list1 = getStmtList(child1);
	std::vector<int> list2 = getStmtList(child2);

	// create all possible pairs of list1 and list2 values
	std::vector<std::pair<int, int>> cross;
	for (int s1 : list1) {
		for (int s2 : list2) {
			cross.push_back({ s1, s2 });
		}
	}

	// filter the cross with PKB.isParentTransitive
	std::vector<std::pair<int, int>> filter;
	for (std::pair<int, int> p : cross) {
		if (pkb.isParentTransitive(p.first, p.second)) {
			filter.push_back(p);
		}
	}

	// after filtering:
	// if the filtered list is empty, then we say that this clause is FALSE
	// else, this clause is TRUE
	clause_bool = (filter.size() > 0);

	// Add the filtered to ResultList!
	// 1. Add the synonym names to as column headers
	if (child1_type == QueryNodeType::synonym) {
		SYNONYM_NAME synonym_name = child1.getString();
		clause_result_list.addColumn(synonym_name);
	}
	if (child2_type == QueryNodeType::synonym) {
		SYNONYM_NAME synonym_name = child2.getString();
		clause_result_list.addColumn(synonym_name);
	}

	// 2. Add synonym values to the Resultlist row wise
	for (std::pair<int, int> p : filter) {
		ROW row;
		if (child1_type == QueryNodeType::synonym) {
			SYNONYM_NAME child1_synonym_name = child1.getString();
			SYNONYM_VALUE child1_synonym_value = std::to_string(p.first);
			row.insert({ child1_synonym_name, child1_synonym_value });
		}
		if (child2_type == QueryNodeType::synonym) {
			SYNONYM_NAME child2_synonym_name = child2.getString();
			SYNONYM_VALUE child2_synonym_value = std::to_string(p.second);
			row.insert({ child2_synonym_name, child2_synonym_value });
		}
		clause_result_list.addRow(row);
	}
}

void QueryEvaluator::getUsesSResult(QueryNode child1, QueryNode child2, bool& clause_bool, ResultList& clause_result_list) {
	/*
	Format: Uses(stmtRef, entRef)
	
	stmtRef: synonym | INTEGER
	entRef: synonym | _ | IDENT (synonym can only be v, wildcard is just all v)

	Possible Combinations:
	1. Uses(synonym, synonym)
	2. Uses(INTEGER, synonym)
	3. Uses(synonym, IDENT)
	4. Uses(synonym, _)
	5. Uses(INTEGER, IDENT)
	6. Uses(INTEGER, _)
	*/

	QueryNodeType child1_type = child1.getNodeType();
	QueryNodeType child2_type = child2.getNodeType();

	// Populate list1 with stmtRef values
	std::vector<int> list1;
	if (child1_type == QueryNodeType::wild_card) {
		throw "QE: First argument of Uses cannot be a wildcard!";
	} else {
		 list1 = getStmtList(child1);
	}

	// Populate list2 with entRef values
	/*
		if IDENT: add IDENT string to the list
		if SYNONYM: add all values of SYNONYM v to the list
		if WILDCARD: add all v to the list
	*/
	std::vector<VAR_NAME> list2;
	if (child2_type == QueryNodeType::ident) {
		list2.push_back(child2.getString());
	}
	else if (child2_type == QueryNodeType::synonym) {
		list2 = getVarNameList(child2);
	}
	else if (child2_type == QueryNodeType::wild_card) {
		list2 = pkb.getVariableNameList();
	}
	else {
		throw "QE: Second argument of Uses should be SYNONYM or IDENT";
	}

	// create all possible pairs of list1 and list2 values
	std::vector<std::pair<int, VAR_NAME>> cross;
	for (int s1 : list1) {
		for (VAR_NAME s2 : list2) {
			cross.push_back({ s1, s2 });
		}
	}

	// filter the cross with PKB.isUses
	std::vector<std::pair<int, VAR_NAME>> filter;
	for (std::pair<int, VAR_NAME> p : cross) {
		if (pkb.isUses(p.first, p.second)) {
			filter.push_back(p);
		}
	}

	// after filtering:
	// if the filtered list is empty, then we say that this clause is FALSE
	// else, this clause is TRUE
	clause_bool = (filter.size() > 0);

	// Add the filtered to ResultList!
	// 1. Add the synonym names to as column headers
	if (child1_type == QueryNodeType::synonym) {
		SYNONYM_NAME synonym_name = child1.getString();
		clause_result_list.addColumn(synonym_name);
	}
	if (child2_type == QueryNodeType::synonym) {
		SYNONYM_NAME synonym_name = child2.getString();
		clause_result_list.addColumn(synonym_name);
	}

	// 2. Add synonym values to the Resultlist row wise
	for (std::pair<int, VAR_NAME> p : filter) {
		ROW row;
		if (child1_type == QueryNodeType::synonym) {
			SYNONYM_NAME child1_synonym_name = child1.getString();
			SYNONYM_VALUE child1_synonym_value = std::to_string(p.first);
			row.insert({ child1_synonym_name, child1_synonym_value });
		}
		if (child2_type == QueryNodeType::synonym) {
			SYNONYM_NAME child2_synonym_name = child2.getString();
			SYNONYM_VALUE child2_synonym_value = p.second;
			row.insert({ child2_synonym_name, child2_synonym_value });
		}
		clause_result_list.addRow(row);
	}
}

void QueryEvaluator::getUsesPResult(QueryNode child1, QueryNode child2, bool& clause_bool, ResultList& clause_result_list) {
	QueryNodeType child1_type = child1.getNodeType();
	QueryNodeType child2_type = child2.getNodeType();

	// Iteration 1: procedure list only have 1 procedure
	// Approach: get all the stmts of that procedure, ignore all other procedures
	// Check isUses(stmt, v) as per normal

	// Populate list1 with child1 values
	std::vector<PROC_NAME> list1 = getProcList(child1);
	
	// Populate list2 with child2 values
	std::vector<VAR_NAME> list2;
	if (child2_type == QueryNodeType::ident) {
		list2.push_back(child2.getString());
	}
	else if (child2_type == QueryNodeType::synonym) {
		list2 = getVarNameList(child2);
	}
	else if (child2_type == QueryNodeType::wild_card) {
		list2 = pkb.getVariableNameList();
	}
	else {
		throw "QE: Second argument of Uses should be SYNONYM or IDENT";
	}

	// create all possible pairs of list1 and list2 values
	std::vector<std::pair<PROC_NAME, VAR_NAME>> cross;
	for (PROC_NAME s1 : list1) {
		for (VAR_NAME s2 : list2) {
			cross.push_back({ s1, s2 });
		}
	}

	// filter the cross with PKB.isFollows
	std::vector<std::pair<PROC_NAME, VAR_NAME>> filter;
	for (std::pair<PROC_NAME, VAR_NAME> p : cross) {
		if (pkb.isUses(p.first, p.second)) {
			filter.push_back(p);
		}
	}

	// after filtering:
	// if the filtered list is empty, then we say that this clause is FALSE
	// else, this clause is TRUE
	clause_bool = (filter.size() > 0);

	// Add the filtered to ResultList!
	// 1. Add the synonym names to as column headers
	if (child1_type == QueryNodeType::synonym) {
		SYNONYM_NAME synonym_name = child1.getString();
		clause_result_list.addColumn(synonym_name);
	}
	if (child2_type == QueryNodeType::synonym) {
		SYNONYM_NAME synonym_name = child2.getString();
		clause_result_list.addColumn(synonym_name);
	}

	// 2. Add synonym values to the Resultlist row wise
	for (std::pair<std::string, VAR_NAME> p : filter) {
		ROW row;
		if (child1_type == QueryNodeType::synonym) {
			SYNONYM_NAME child1_synonym_name = child1.getString();
			SYNONYM_VALUE child1_synonym_value = p.first;
			row.insert({ child1_synonym_name, child1_synonym_value });
		}
		if (child2_type == QueryNodeType::synonym) {
			SYNONYM_NAME child2_synonym_name = child2.getString();
			SYNONYM_VALUE child2_synonym_value = p.second;
			row.insert({ child2_synonym_name, child2_synonym_value });
		}
		clause_result_list.addRow(row);
	}
}

void QueryEvaluator::getModifiesSResult(QueryNode child1, QueryNode child2, bool& clause_bool, ResultList& clause_result_list) {
	/*
	Format: Modifies(stmtRef, entRef)

	stmtRef: synonym | INTEGER
	entRef: synonym | _ | IDENT (synonym can only be v, wildcard is just all v)

	Possible Combinations:
	1. Modifies(synonym, synonym)
	2. Modifies(INTEGER, synonym)
	3. Modifies(synonym, IDENT)
	4. Modifies(synonym, _)
	5. Modifies(INTEGER, IDENT)
	6. Modifies(INTEGER, _)
	*/

	QueryNodeType child1_type = child1.getNodeType();
	QueryNodeType child2_type = child2.getNodeType();

	// Populate list1 with child1 values
	std::vector<int> list1;
	if (child1_type == QueryNodeType::wild_card) {
		throw "QE: First argument of Uses cannot be a wildcard!";
	} else {
		list1 = getStmtList(child1);
	}
	
	// Populate list2 with child2 values
	/*
		if IDENT: add IDENT string to the list
		if SYNONYM: add all values of SYNONYM v to the list
		if WILDCARD: add all v to the list
	*/
	std::vector<VAR_NAME> list2;
	if (child2_type == QueryNodeType::ident) {
		list2.push_back(child2.getString());
	}
	else if (child2_type == QueryNodeType::synonym) {
		list2 = getVarNameList(child2);
	}
	else if (child2_type == QueryNodeType::wild_card) {
		list2 = pkb.getVariableNameList();
	}
	else {
		throw "QE: Second argument of Uses should be SYNONYM or IDENT";
	}

	// create all possible pairs of list1 and list2 values
	std::vector<std::pair<int, VAR_NAME>> cross;
	for (int s1 : list1) {
		for (VAR_NAME s2 : list2) {
			cross.push_back({ s1, s2 });
		}
	}

	// filter the cross with PKB.isFollows
	std::vector<std::pair<int, VAR_NAME>> filter;
	for (std::pair<int, VAR_NAME> p : cross) {
		if (pkb.isModifies(p.first, p.second)) {
			filter.push_back(p);
		}
	}

	// after filtering:
	// if the filtered list is empty, then we say that this clause is FALSE
	// else, this clause is TRUE
	clause_bool = (filter.size() > 0);

	// Add the filtered to ResultList!
	// 1. Add the synonym names to as column headers
	if (child1_type == QueryNodeType::synonym) {
		SYNONYM_NAME synonym_name = child1.getString();
		clause_result_list.addColumn(synonym_name);
	}
	if (child2_type == QueryNodeType::synonym) {
		SYNONYM_NAME synonym_name = child2.getString();
		clause_result_list.addColumn(synonym_name);
	}

	// 2. Add synonym values to the Resultlist row wise
	for (std::pair<int, VAR_NAME> p : filter) {
		ROW row;
		if (child1_type == QueryNodeType::synonym) {
			SYNONYM_NAME child1_synonym_name = child1.getString();
			SYNONYM_VALUE child1_synonym_value = std::to_string(p.first);
			row.insert({ child1_synonym_name, child1_synonym_value });
		}
		if (child2_type == QueryNodeType::synonym) {
			SYNONYM_NAME child2_synonym_name = child2.getString();
			SYNONYM_VALUE child2_synonym_value = p.second;
			row.insert({ child2_synonym_name, child2_synonym_value });
		}
		clause_result_list.addRow(row);
	}
}

void QueryEvaluator::getModifiesPResult(QueryNode child1, QueryNode child2, bool& clause_bool, ResultList& clause_result_list) {
	QueryNodeType child1_type = child1.getNodeType();
	QueryNodeType child2_type = child2.getNodeType();

	// Populate list1 with child1 values
	std::vector<PROC_NAME> list1 = getProcList(child1);
	
	// Populate list2 with child2 values
	std::vector<VAR_NAME> list2;
	if (child2_type == QueryNodeType::ident) {
		list2.push_back(child2.getString());
	}
	else if (child2_type == QueryNodeType::synonym) {
		list2 = getVarNameList(child2);
	}
	else if (child2_type == QueryNodeType::wild_card) {
		list2 = pkb.getVariableNameList();
	}
	else {
		throw "QE: Second argument of Uses should be SYNONYM or IDENT";
	}

	// create all possible pairs of list1 and list2 values
	std::vector<std::pair<PROC_NAME, VAR_NAME>> cross;
	for (PROC_NAME s1 : list1) {
		for (VAR_NAME s2 : list2) {
			cross.push_back({ s1, s2 });
		}
	}

	// filter the cross with PKB.isFollows
	std::vector<std::pair<PROC_NAME, VAR_NAME>> filter;
	for (std::pair<PROC_NAME, VAR_NAME> p : cross) {
		if (pkb.isModifies(p.first, p.second)) {
			filter.push_back(p);
		}
	}

	// after filtering:
	// if the filtered list is empty, then we say that this clause is FALSE
	// else, this clause is TRUE
	clause_bool = (filter.size() > 0);

	// Add the filtered to ResultList!
	// 1. Add the synonym names to as column headers
	if (child1_type == QueryNodeType::synonym) {
		SYNONYM_NAME synonym_name = child1.getString();
		clause_result_list.addColumn(synonym_name);
	}
	if (child2_type == QueryNodeType::synonym) {
		SYNONYM_NAME synonym_name = child2.getString();
		clause_result_list.addColumn(synonym_name);
	}

	// 2. Add synonym values to the Resultlist row wise
	for (std::pair<std::string, VAR_NAME> p : filter) {
		ROW row;
		if (child1_type == QueryNodeType::synonym) {
			SYNONYM_NAME child1_synonym_name = child1.getString();
			SYNONYM_VALUE child1_synonym_value = p.first;
			row.insert({ child1_synonym_name, child1_synonym_value });
		}
		if (child2_type == QueryNodeType::synonym) {
			SYNONYM_NAME child2_synonym_name = child2.getString();
			SYNONYM_VALUE child2_synonym_value = p.second;
			row.insert({ child2_synonym_name, child2_synonym_value });
		}
		clause_result_list.addRow(row);
	}
}

STMT_NUM_LIST QueryEvaluator::getStmtList(QueryNode child1) {
	QueryNodeType child1_type = child1.getNodeType();

	if (child1_type == QueryNodeType::integer) {
		STMT_NUM_LIST oneIntList = { child1.getInteger() };
		return oneIntList;
		//return STMT_NUM_LIST(chilld1.getInteger())
	}
	else if (child1_type == QueryNodeType::synonym) {
		QuerySynonymType child1_syn_type = child1.getSynonymType();
		if (child1_syn_type == QuerySynonymType::assign) {
			return pkb.getAssignNumList();
		}
		// else if (child1_syn_type == QuerySynonymType::call) { for iteration 2
		else if (child1_syn_type == QuerySynonymType::ifs) {
			return pkb.getIfNumList();
		}
		else if (child1_syn_type == QuerySynonymType::print) {
			return pkb.getPrintNumList();
		}
		else if (child1_syn_type == QuerySynonymType::read) {
			return pkb.getReadNumList();
		}
		else if (child1_syn_type == QuerySynonymType::stmt) {
			return pkb.getStatementNumList();
		}
		else if (child1_syn_type == QuerySynonymType::whiles) {
			return pkb.getWhileNumList();
		}
		else {
			throw "QE: Synonym is not a statement or an integer!";
		}
	}
	else if (child1_type == QueryNodeType::wild_card) {
		return pkb.getStatementNumList();
	}
	else {
		throw "QE: stmtRef is not a integer, synonym or wildcard!";
	}
}

VAR_NAME_LIST QueryEvaluator::getVarNameList(QueryNode node) {
	if (node.getSynonymType() == QuerySynonymType::variable) {
		return pkb.getVariableNameList();
	}
	else {
		throw "QE: node is not a variable, getVarNameList requires a variable node";
	}
}

PROC_NAME_LIST QueryEvaluator::getProcList(QueryNode node) {
	if (node.getSynonymType() == QuerySynonymType::procedure) {
		return pkb.getProcedureNameList();
	}
	else {
		throw "QE: node is not a procedure, getProcList requires a procedure node";
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

bool QueryEvaluator::isSameSynonymName(QueryNode child1, QueryNode child2) {
	bool same = false;
	STRING child1_synonym_name = child1.getString();
	STRING child2_synonym_name = child2.getString();
	bool child1_is_synonym = (child1.getNodeType() == QueryNodeType::synonym);
	bool child2_is_synonym = (child2.getNodeType() == QueryNodeType::synonym);

	if (child1_is_synonym && child2_is_synonym && (child1_synonym_name.compare(child2_synonym_name) == 0)) {
		same = true;
	}

	return same;
}