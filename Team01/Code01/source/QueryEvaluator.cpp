#include "QueryEvaluator.h"
#include <queue>

QueryEvaluator::QueryEvaluator(PKB pkb) {
	this->pkb = pkb;
}

QUERY_RESULT QueryEvaluator::evaluateQuery(PROCESSED_SYNONYMS synonyms, PROCESSED_CLAUSES clauses) {
	
	// If root of clauses tree is unassigned, then the clause is syntactically incorrect. Return no result.
	if (clauses.getNodeType() == QueryNodeType::unassigned) {
		return no_result;
	}

	QUERY_NODE_LIST children = clauses.getChildren();
	if (children.size() == 0) {
		throw "Error: Select has no children, it should have at least 1 child (ie Select v)";
	}

	// Get the ResultClause of Select -> BOOLEAN | tuple
	this->result_clause = children[0];
	setEvaluatorReturnType();

	if (return_type == QueryEvaluatorReturnType::tuple) {
		setTupleReturnSynonyms();
	}

	// No other clauses to evaluate (ie Select <p.procName, v>) -> evaluate ResultClause
	if (children.size() == 1) {
		QUERY_RESULT final_result = evaluateResultClause();
		return final_result;
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
			Pattern pattern = Pattern(clause);
			pattern.getPatternResult(pkb, clause_bool, clause_result_list);
		}
		else if (clause_type == QueryNodeType::with) {
			WithClause with = WithClause(clause);
			with.getWithResult(pkb, clause_bool, clause_result_list);
		}
		else {
			throw "QE: Clause type is invalid. Valid clauses are such that, pattern, with.";
		}
		

		/* 
		if the clause_bool is true:
			if there are columns (means not True/False clause) => set only_true_false_clause to false
			merge result_list with clause_result_list
		if the clause_bool is false => 
			if return type is synonym => return no result list
			if return type is boolean => return false result list
		*/
		if (clause_bool) {
			if (clause_result_list.getNumColumns() != 0) {
				only_true_false_clauses = false;
			}
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

		evaluated_clauses = true;
	}

	QUERY_RESULT final_result = obtainFinalQueryResult();
	return final_result;
}

void QueryEvaluator::fillWithReturnSynonym(QueryNode result_clause, ResultList &result_list) {
	SYNONYM_NAME return_synonym_name = result_clause.getString();
	SYNONYM_TYPE return_synonym_type = result_clause.getSynonymType();

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
	else if (return_synonym_type == QuerySynonymType::prog_line) {
		result_list.addColumn(return_synonym_name, pkb.getStatementNumList());
	}
	
}

void QueryEvaluator::setEvaluatorReturnType() {
	QueryNodeType return_type = result_clause.getNodeType();

	if (return_type == QueryNodeType::boolean) {
		this->return_type = { QueryEvaluatorReturnType::boolean };
		return;
	}

	// If it is not BOOLEAN, it is Tuple -> look at the number of children
	int num_children = result_clause.getChildren().size();

	if (return_type == QueryNodeType::tuple) {
		if (num_children == 1) {
			this->return_type = { QueryEvaluatorReturnType::synonym };
		}
		else {
			this->return_type = { QueryEvaluatorReturnType::tuple };
		}
	}
	else {
		throw "QE: Select's return type is not tuple or boolean";
	}
}

void QueryEvaluator::setTupleReturnSynonyms() {
	for (QueryNode child : result_clause.getChildren()) {
		tuple_return_synonyms.push_back(child.getString());
	}
}

QUERY_RESULT QueryEvaluator::obtainFinalQueryResult() {

	// only 1 return value
	if (return_type == QueryEvaluatorReturnType::synonym) {
		
		QueryNode synonym = result_clause.getChildren()[0];
		SYNONYM_NAME return_synonym_name = synonym.getString();
		if (!result_list.containsSynonym(return_synonym_name)) {
			// if only true_false_clause, for it to reach here, it means that all clauses are true -> return synonym
			// if not true_false, then consider num_rows to see if synonym should be returned
			if (only_true_false_clauses || result_list.getNumRows() != 0) {
				ResultList final_result_list;
				QuerySynonymType return_synonym_type = synonym.getSynonymType();
				fillWithReturnSynonym(synonym, final_result_list);
				return ResultListManager::getSynonymValues(final_result_list, return_synonym_name);
			}
		}
		else {
			return ResultListManager::getSynonymValues(result_list, return_synonym_name);
		}
	}
	else if (return_type == QueryEvaluatorReturnType::boolean) {

		if (only_true_false_clauses) {		// if evaluator reaches here, means all clause_bool were True. ResultList might be empty but we return TRUE.
			return boolean_true_result;
		}
		else if (result_list.getNumRows() > 0) {	// Not all clauses evaluated are True/False, thus there should be results.
			return boolean_true_result;
		}
		else {
			return boolean_false_result;
		}
	}
	else {
		//handle tuple return result here
	}
}

QUERY_RESULT QueryEvaluator::evaluateResultClause() {
	if (return_type == QueryEvaluatorReturnType::boolean) {
		return boolean_true_result;
	}
	else if (return_type == QueryEvaluatorReturnType::synonym) {
		ResultList final_result_list;
		QueryNode return_synonym = (result_clause.getChildren())[0];
		SYNONYM_NAME return_synonym_name = return_synonym.getString();
		
		fillWithReturnSynonym(return_synonym, final_result_list);

		return ResultListManager::getSynonymValues(final_result_list, return_synonym_name);
	}
	else if (return_type == QueryEvaluatorReturnType::tuple) {
		ResultList final_result_list;
		QUERY_NODE_LIST children = result_clause.getChildren();
		
		for (QueryNode child : children) {
			ResultList child_result_list;
			fillWithReturnSynonym(child, child_result_list);

			ResultListManager::merge(final_result_list, child_result_list);
		}

		return ResultListManager::getTupleValues(final_result_list, tuple_return_synonyms);
	}
	
}
