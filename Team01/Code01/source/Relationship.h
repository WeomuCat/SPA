#pragma once

#include "QueryNode.h"
#include "QueryNodeType.h"
#include "QuerySynonymType.h"
#include "ResultList.h"
#include "PKB.h"

#include <iostream>

class Relationship {
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

	bool isSameSynonymName(QueryNode child1, QueryNode child2);
	STMT_NUM_LIST getStmtList(PKB pkb, QueryNode child1);
	VAR_NAME_LIST getVarNameList(PKB pkb, QueryNode node);
	PROC_NAME_LIST getProcList(PKB pkb, QueryNode node);

public:
	Relationship(QueryNodeType relationship_type, QueryNode child1, QueryNode child2);
	void getRelationshipResult(PKB pkb, bool& clause_bool, ResultList& clause_result_list);
};