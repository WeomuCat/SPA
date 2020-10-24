#pragma once

#include "QueryNode.h"
#include "QueryNodeType.h"
#include "QuerySynonymType.h"
#include "ResultList.h"
#include "PKB.h"

#include <iostream>

class Relationship {
	/*Overview: This class handles the logic of such that relationships of 2 arguments. */
public:
	Relationship(QueryNodeType relationship_type, QueryNode child1, QueryNode child2);
	/*
	Description: Creates a relationship with a given relationship type, argument 1 and argument 2 of the relationship.
	*/

	void getRelationshipResult(PKB pkb, bool& clause_bool, ResultList& clause_result_list);
	/*
	Description: Updates the given clause_result_list with the result of this Relationship, consulting from the given pkb.
				 Updates the give clause_bool to True if this clause result is Non-Empty. 
	*/

private:
	QueryNode child1;
	QueryNode child2;
	QueryNodeType relationship_type;

	void getFollowsResult(PKB pkb, bool& clause_bool, ResultList& clause_result_list);
	void getFollowsTResult(PKB pkb, bool& clause_bool, ResultList& clause_result_list);
	void getParentResult(PKB pkb, bool& clause_bool, ResultList& clause_result_list);
	void getParentTResult(PKB pkb, bool& clause_bool, ResultList& clause_result_list);
	void getUsesSResult(PKB pkb, bool& clause_bool, ResultList& clause_result_list);
	void getUsesPResult(PKB pkb, bool& clause_bool, ResultList& clause_result_list);
	void getModifiesSResult(PKB pkb, bool& clause_bool, ResultList& clause_result_list);
	void getModifiesPResult(PKB pkb, bool& clause_bool, ResultList& clause_result_list);
	void getCallsResult(PKB pkb, bool& clause_bool, ResultList& clause_result_list);
	void getCallsTResult(PKB pkb, bool& clause_bool, ResultList& clause_result_list);
	void getNextResult(PKB pkb, bool& clause_bool, ResultList& clause_result_list);
	void getNextTResult(PKB pkb, bool& clause_bool, ResultList& clause_result_list);
	void getAffectsResult(PKB pkb, bool& clause_bool, ResultList& clause_result_list);
	void getAffectsTResult(PKB pkb, bool& clause_bool, ResultList& clause_result_list);


	bool isSameSynonymName(QueryNode child1, QueryNode child2);
	STMT_NUM_LIST getStmtList(PKB pkb, QueryNode child1);
	VAR_NAME_LIST getVarNameList(PKB pkb, QueryNode node);
	PROC_NAME_LIST getProcList(PKB pkb, QueryNode node);

};