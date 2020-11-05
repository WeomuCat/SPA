#pragma once

#include "QueryNode.h"
#include "QueryNodeType.h"
#include "QuerySynonymType.h"
#include "ResultList.h"
#include "PKB.h"

#include <iostream>
#include <unordered_map>

class Relationship {
	/*Overview: This class handles the logic of such that relationships of 2 arguments. */
public:
	/*
	Description: Creates a relationship from a clause node.
	*/
	Relationship(QueryNode relationship_node, ResultList& intermediate_result_list);

	/*
	Description: Updates the given clause_result_list with the result of this Relationship, consulting from the given pkb.
				 Updates the give clause_bool to True if this clause result is Non-Empty.
	*/
	void getRelationshipResult(PKB pkb, bool& clause_bool, ResultList& clause_result_list);

private:
	QueryNode relationship_node_;
	ResultList intermediate_result_list;
	std::unordered_map<SYNONYM_VALUE, STMT_NUM> stmt_nums_;

	bool areSynonymsEqual(QueryNode r1, QueryNode r2);
	SYNONYM_VALUES_LIST getStatementsFromRef(PKB pkb, QueryNode ref);
	SYNONYM_VALUES_LIST getAssignsFromRef(PKB pkb, QueryNode ref);
	SYNONYM_VALUES_LIST getVariablesFromRef(PKB pkb, QueryNode ref);
	SYNONYM_VALUES_LIST getProceduresFromRef(PKB pkb, QueryNode ref);

	STMT_NUM_LIST getStatementNumsFromRef(PKB pkb, QueryNode ref);
	STMT_NUM_LIST getAssignNumsFromRef(PKB pkb, QueryNode ref);

	SYNONYM_VALUES_LIST stmtNumsToSynValues(STMT_NUM_LIST stmt_nums);
	STMT_NUM synValueToStmtNum(SYNONYM_VALUE stmt_string);
};