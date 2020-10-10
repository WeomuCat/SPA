#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include "QueryNode.h"
#include "QueryValidator.h"

typedef std::string QUERY;
typedef std::vector<std::string> SPLIT_QUERY;
typedef std::string DECLARATIONS;
typedef std::vector<std::string> SPLIT_DECLARATIONS;
typedef std::unordered_map<std::string, QueryNode> PROCESSED_SYNONYMS;
typedef std::string CLAUSES;
typedef QueryNode PROCESSED_CLAUSES;
typedef std::string RELATIONSHIP;
typedef std::string ARGUMENT;
typedef std::string EXPRESSION;
typedef std::string STRING;
typedef int INDEX;

class QueryPreProcessor {
	/* Overview: Pre-processes queries into a suitable data structure for the evaluator */

public:
	SPLIT_QUERY splitQuery(QUERY q);
	/*
		Description:
		  Normal: Returns a SPLIT_QUERY
		*/

	PROCESSED_SYNONYMS preProcessSynonyms(DECLARATIONS d);
	/*
		Description:
		  Normal: Returns PROCESSED_SYNONYMS
		  Abnormal: If synonyms are not valid, returns an empty map
		*/

	PROCESSED_CLAUSES preProcessClauses(PROCESSED_SYNONYMS proc_s, CLAUSES c);
	/*
		Description:
		  Normal: Returns PROCESSED_CLAUSES
		  Abnormal: If clauses are not valid, returns a query node with a null NODE_TYPE
		*/

private:
	SPLIT_DECLARATIONS splitDeclarations(DECLARATIONS d);
	/*
		Description:
		  Normal: Returns SPLIT_DECLARATIONS
		*/

	INDEX getNextClauseIndex(CLAUSES c, INDEX current_index, INDEX such_that_index, INDEX pattern_index);
	/*
		Description:
		  Normal: Returns the INDEX of the next clause
		*/

	QueryNode createExpressionNode(EXPRESSION e);
	/*
		Description:
		  Normal: Returns SPLIT_DECLARATIONS
		*/

	QueryNode createArgumentNode(PROCESSED_SYNONYMS proc_s, ARGUMENT arg);
	/*
		Description:
		  Normal: Returns a QueryNode for the argument
		*/

	QueryNode createRelationNode(PROCESSED_SYNONYMS proc_s, RELATIONSHIP rel,
		ARGUMENT first_arg, ARGUMENT second_arg);
	/*
		Description:
		  Normal: Returns a QueryNode for the relationship
		*/

	QueryNode createPatternNode(PROCESSED_SYNONYMS proc_s, SYNONYM_NAME s,
		ARGUMENT first_arg, ARGUMENT second_arg);
	/*
		Description:
		  Normal: Returns a QueryNode for the pattern
		*/

	STRING trimWhitespaces(STRING s);
	/*
		Description:
		  Normal: Trims whitespaces from front and back of STRING and returns it
	*/
};