#pragma once

#include "QueryNode.h"
#include "QueryNodeType.h"
#include "QuerySynonymType.h"
#include "ResultList.h"
#include "ResultListManager.h"
#include "PKB.h"
#include "Relationship.h"

#include <string>
#include <unordered_map>
#include <vector>

class QueryEvaluator;

typedef std::vector<std::string> QUERY_RESULT;
typedef std::unordered_map<std::string, QueryNode> PROCESSED_SYNONYMS;
typedef QueryNode PROCESSED_CLAUSES;

class QueryEvaluator {
	/* Overview: Evaluates pre-processed queries and returns the corresponding result */

public:
	QueryEvaluator(PKB pkb);
	/*
	Description: Creates a QueryEvaluator for the given PKB.
	*/

	QUERY_RESULT evaluateQuery(PROCESSED_SYNONYMS synonyms, PROCESSED_CLAUSES clauses);
	/*
	Description: Returns the query result of the given processed synonyms and processed clauses.
				 Invalid queries would return an empty result list.
	*/

private:
	PKB pkb = (PKBBuilder().build()); // because PKB has no default constructor
	void QueryEvaluator::fillWithReturnSynonym(QuerySynonymType return_synonym_type, SYNONYM_NAME return_synonym_name, ResultList& result_list);
	/*
	Description: Fills the given result_list with all values of the given synonym_type, attached to the column name of the given synonym_name.
	*/

	bool QueryEvaluator::findPartialPattern(AST_NODE_PTR ast, std::string search_name);
};