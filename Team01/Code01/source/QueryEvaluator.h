#pragma once

#include "QueryNode.h"
#include "QueryNodeType.h"
#include "QuerySynonymType.h"
#include "ResultList.h"
#include "ResultListManager.h"
#include "PKB.h"

#include <string>
#include <unordered_map>
#include <vector>

class QueryEvaluator;

typedef std::vector<std::string> QUERY_RESULT;
/*may need to typedef processed_synonyms? check with huiming on the type from preprocessor*/
typedef std::unordered_map<std::string, QueryNode> PROCESSED_SYNONYMS;
typedef QueryNode PROCESSED_CLAUSES;

class QueryEvaluator {
	/* Overview: Evaluates pre-processed queries and returns the result */

private:
	PKB pkb = (PKBBuilder().build()); // because PKB has no default constructor
	STMT_NUM_LIST QueryEvaluator::getStmtList(QueryNode node);
	VAR_NAME_LIST QueryEvaluator::getVarNameList(QueryNode node);
	PROC_NAME_LIST QueryEvaluator::getProcList(QueryNode node);
	void QueryEvaluator::getFollowsResult(QueryNode child1, QueryNode child2, bool& clause_bool, ResultList& clause_result_list);
	void QueryEvaluator::getFollowsTResult(QueryNode child1, QueryNode child2, bool& clause_bool, ResultList& clause_result_list);
	void QueryEvaluator::getParentResult(QueryNode child1, QueryNode child2, bool& clause_bool, ResultList& clause_result_list);
	void QueryEvaluator::getParentTResult(QueryNode child1, QueryNode child2, bool& clause_bool, ResultList& clause_result_list);
	void QueryEvaluator::getUsesSResult(QueryNode child1, QueryNode child2, bool& clause_bool, ResultList& clause_result_list);
	void QueryEvaluator::getUsesPResult(QueryNode child1, QueryNode child2, bool& clause_bool, ResultList& clause_result_list);
	void QueryEvaluator::getModifiesSResult(QueryNode child1, QueryNode child2, bool& clause_bool, ResultList& clause_result_list);
	void QueryEvaluator::getModifiesPResult(QueryNode child1, QueryNode child2, bool& clause_bool, ResultList& clause_result_list);
	//ResultList QueryEvaluator::getFollowsResult(QueryNode child1, QueryNode child2);
	//ResultList QueryEvaluator::getFollowsTResult(QueryNode child1, QueryNode child2);
	//ResultList QueryEvaluator::getParentResult(QueryNode child1, QueryNode child2);
	//ResultList QueryEvaluator::getParentTResult(QueryNode child1, QueryNode child2);
	//ResultList QueryEvaluator::getUsesSResult(QueryNode child1, QueryNode child2);
	//ResultList QueryEvaluator::getUsesPResult(QueryNode child1, QueryNode child2);
	//ResultList QueryEvaluator::getModifiesSResult(QueryNode child1, QueryNode child2);
	//ResultList QueryEvaluator::getModifiesPResult(QueryNode child1, QueryNode child2);
	bool findPartialPattern(AST_NODE_PTR ast, std::string search_name);
	bool isSameSynonymName(QueryNode child1, QueryNode child2);

public:
	QueryEvaluator(PKB pkb);
	/*
		Description:
		  Normal:
		*/

	QUERY_RESULT evaluateQuery(PROCESSED_SYNONYMS synonyms, PROCESSED_CLAUSES clauses);
	/*
		Requires: A query in the form of PROCESSED_SYNONYMS and PROCESSED_CLAUSES
		Description:
		  Normal: Returns the QUERY_RESULT
		  Abnormal: If 's' and 'c' is not a valid processed query, throw an exception
		*/
};