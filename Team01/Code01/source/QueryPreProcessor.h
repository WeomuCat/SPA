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

/*
Overview: Pre-processes queries into a suitable data structure for the evaluator.
*/
class QueryPreProcessor {

public:
	/*
	Description: Returns a SPLIT_QUERY.
	*/
	SPLIT_QUERY splitQuery(QUERY q);

	/*
	Description: Returns PROCESSED_SYNONYMS.
				 If synonyms are not valid, returns an empty map.
	*/
	PROCESSED_SYNONYMS preProcessSynonyms(DECLARATIONS d);

	/*
	Description: Returns PROCESSED_CLAUSES.
				 If clauses are not valid, returns a query node with a null NODE_TYPE.
	*/
	PROCESSED_CLAUSES preProcessClauses(PROCESSED_SYNONYMS proc_s, CLAUSES c);

private:
	/*
	Description: Returns SPLIT_DECLARATIONS.
	*/
	SPLIT_DECLARATIONS splitDeclarations(DECLARATIONS d);

	/*
	Description: Returns the INDEX of the next clause.
	*/
	INDEX getNextClauseIndex(CLAUSES c, INDEX current_index, INDEX such_that_index, INDEX pattern_index);

	/*
	Description: Returns SPLIT_DECLARATIONS.
	*/
	QueryNode createExpressionNode(EXPRESSION e);

	/*
	Description: Returns a QueryNode for the argument.
	*/
	QueryNode createArgumentNode(PROCESSED_SYNONYMS proc_s, ARGUMENT arg);

	/*
	Description: Returns a QueryNode for the relationship.
	*/
	QueryNode createRelationNode(PROCESSED_SYNONYMS proc_s, RELATIONSHIP rel,
		ARGUMENT first_arg, ARGUMENT second_arg);

	/*
	Description: Returns a QueryNode for the pattern.
	*/
	QueryNode createPatternNode(PROCESSED_SYNONYMS proc_s, SYNONYM_NAME s,
		ARGUMENT first_arg, ARGUMENT second_arg);

	/*
	Description: Trims whitespaces from front and back of STRING and returns it.
	*/
	STRING trimWhitespaces(STRING s);
};