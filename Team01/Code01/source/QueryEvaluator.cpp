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

	// Get return type and return synonym name
	QueryNode return_synonym = children[0];
	QuerySynonymType return_synonym_type = return_synonym.getSynonymType();
	SYNONYM_NAME return_synonym_name = return_synonym.getString(); // "v"

	// fill resultList with return_synonym_name
	fillWithReturnSynonym(return_synonym_type, return_synonym_name, result_list);


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
			Pattern pattern = Pattern(clause);
			pattern.getPatternResult(pkb, clause_bool, clause_result_list);
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
			CONSTANT_VALUE node_name = n->getValue();
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
