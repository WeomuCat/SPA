#pragma once

#include <unordered_map>
#include <string>

#include "../PKB/PKB.h"
#include "../PKB/PKBBuilder.h"
#include "QueryPreProcessor.h"
#include "QueryEvaluator.h"
#include "QueryNode.h"
#include "QueryNodeType.h"
#include "QuerySynonymType.h"

class QueryProcessor;

typedef std::vector<std::string> QUERY_RESULT;
typedef std::string QUERY;

/*
Overview: Oversees and drives the pre-processor and evaluator.
*/
class QueryProcessor {

private:
	PKB pkb = (PKBBuilder().build()); // because PKB has no default constructor
public:
	/*==== Constructor ====*/
	QueryProcessor(PKB pkb);
	/*
	Description: Creates a QueryProcessor for the given PKB.
	*/


	QUERY_RESULT processQuery(QUERY q);
	/*
	Description: Returns the QUERY_RESULT.
				 If 'q' is not a valid query, throws an exception.
	*/
};
