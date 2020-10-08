#pragma once

#include "QueryNode.h"
#include "QueryNodeType.h"
#include "QuerySynonymType.h"
#include "ResultList.h"
#include "PKB.h"

class Pattern {
private:
	QueryNode clause_node_;
	VAR_NAME_LIST getVarNameListFromAst(AST_NODE_PTR ast);
	bool expressionTreeMatch(EXPR_NODE_PTR haystack, EXPR_NODE_PTR needle);
	bool partialExpressionTreeMatch(EXPR_NODE_PTR haystack, EXPR_NODE_PTR needle);
	bool expressionNodesEqual(AST_NODE_PTR n1, AST_NODE_PTR n2);
public:
	Pattern(QueryNode clause_node);
	void getPatternResult(PKB pkb, bool &clause_bool, ResultList &clause_result_list);
};