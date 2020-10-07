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
		Description:
		  Normal: 
		*/

	QUERY_RESULT processQuery(QUERY q);
	/*
		Description:
		  Normal: Returns the QUERY_RESULT
		  Abnormal: If 'q' is not a valid query, throw an exception
		*/
};
