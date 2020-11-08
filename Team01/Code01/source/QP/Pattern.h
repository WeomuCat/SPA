#pragma once

#include "QueryNode.h"
#include "QueryNodeType.h"
#include "QuerySynonymType.h"
#include "ResultList.h"
#include "../PKB/PKB.h"

class Pattern {
public:
	Pattern(QueryNode pattern_node, ResultList& intermediate_result_list);
	
	static bool exactExpressionTreeMatch(EXPR_NODE_PTR haystack, EXPR_NODE_PTR needle);
	static bool partialExpressionTreeMatch(EXPR_NODE_PTR haystack, EXPR_NODE_PTR needle);

	void getPatternResult(PKB pkb, bool &clause_bool, ResultList &clause_result_list);
private:
	static bool partialTopExpressionTreeMatch(AST_NODE_PTR haystack, AST_NODE_PTR needle);
	static VAR_NAME_LIST getVarNameListFromAst(AST_NODE_PTR ast);
	static bool areExpressionNodesEqual(AST_NODE_PTR n1, AST_NODE_PTR n2);
	static AST_NODE_PTR_LIST getExpressionNodeChildren(AST_NODE_PTR n);
	static AST_NODE_PTR getExpressionNodeLeft(AST_NODE_PTR n);
	static AST_NODE_PTR getExpressionNodeRight(AST_NODE_PTR n);

	QueryNode pattern_node_;
	ResultList intermediate_result_list;
};