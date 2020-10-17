#pragma once

#include "PKB.h"
#include "QueryNode.h"
#include "QueryNodeType.h"
#include "QuerySynonymType.h"
#include "ResultList.h"


class WithClause {
public:
	WithClause(QueryNode with_clause);
	void getWithResult(PKB pkb, bool& clause_bool, ResultList& clause_result_list);
private:
	QueryNode with_clause;
};