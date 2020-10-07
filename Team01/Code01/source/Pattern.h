#pragma once

#include "QueryNode.h"
#include "QueryNodeType.h"
#include "QuerySynonymType.h"
#include "ResultList.h"
#include "PKB.h"

class Pattern {
private:
	QueryNode clause_node_;
	std::unordered_set<VAR_NAME> getVarNameListFromAst(AST_NODE_PTR ast);
public:
	Pattern(QueryNode clause_node);
	void getPatternResult(PKB pkb, bool &clause_bool, ResultList &clause_result_list);
};