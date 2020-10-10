#pragma once

#include "PKB.h"
#include "PKBBuilder.h"

#include <string>

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
	Description: Returns the QUERY_RESULT.
				 If 'q' is not a valid query, throws an exception.
	*/
	QUERY_RESULT processQuery(QUERY q);
};
