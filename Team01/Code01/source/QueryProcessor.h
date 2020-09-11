#pragma once

#include "PKB.h"

#include <string>

class QueryProcessor;

typedef std::string QUERY_RESULT;
typedef std::string QUERY;

class QueryProcessor {
public:
	QUERY_RESULT processQuery(QUERY q);
};
