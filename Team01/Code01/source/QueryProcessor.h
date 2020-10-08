#pragma once

#include "PKB.h"
#include "PKBBuilder.h"

#include <string>

class QueryProcessor;

typedef std::vector<std::string> QUERY_RESULT;
typedef std::string QUERY;

class QueryProcessor {
	/* Overview: Oversees and drives the pre-processor and evaluator*/

private:
	PKB pkb = (PKBBuilder().build()); // because PKB has no default constructor
public:

	QueryProcessor(PKB pkb);
	/*
	Description: Creates a QueryProcessor for the given PKB.
	*/

	QUERY_RESULT processQuery(QUERY q);
	/*
	Description: Processes the given query and returns the query result.
	*/
};
