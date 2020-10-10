#pragma once

#include <string>
#include <unordered_map>

#include "QueryNode.h"

typedef bool VALIDATION_RESULT;
typedef std::string QUERY;
typedef std::string CLAUSES;
typedef std::string SINGLE_DECLARATION;
typedef std::string SINGLE_CLAUSE;
typedef std::unordered_map<std::string, QueryNode> PROCESSED_SYNONYMS;
typedef std::string ARGUMENT;
typedef std::string RELATIONSHIP;

/*
Overview: Checks if queries adheres to grammar rules
*/
class QueryValidator {

public:
	/*
	Description: Returns a VALIDATION_RESULT based on general structure of input query.
	*/
	static VALIDATION_RESULT isValidStructure(QUERY q);

	/*
	Description: Returns a VALIDATION_RESULT based on grammer for declarations.
	*/
	static VALIDATION_RESULT isValidDeclaration(SINGLE_DECLARATION single_d);

	/*
	Description: Returns a VALIDATION_RESULT based on grammar for clauses.
	*/
	static VALIDATION_RESULT isValidClause(CLAUSES c);

	/*
	Description: Returns a VALIDATION_RESULT based on whether synonym has been declared.
	*/
	static VALIDATION_RESULT isSynonymDeclared(PROCESSED_SYNONYMS proc_s, SYNONYM_NAME s);

	/*
	Description: Returns a VALIDATION_RESULT based on relationship format.
	*/
	static VALIDATION_RESULT isValidRelationFormat(SINGLE_CLAUSE single_c);

	/*
	Description: Returns a VALIDATION_RESULT based on whether argument returns a statemenr number.
	*/
	static VALIDATION_RESULT isStatementArgument(PROCESSED_SYNONYMS proc_s, ARGUMENT a);

	/*
	Description: Returns a VALIDATION_RESULT based on relationship argument validity.
	*/
	static VALIDATION_RESULT isValidRelationArguments(PROCESSED_SYNONYMS proc_s, RELATIONSHIP rel,
		ARGUMENT first_arg, ARGUMENT second_arg);

	/*
	Description: Returns a VALIDATION_RESULT based on pattern format.
	*/
	static VALIDATION_RESULT isValidPatternFormat(SINGLE_CLAUSE single_c);

	/*
	Description: Returns a VALIDATION_RESULT based on pattern argument validity.
	*/
	static VALIDATION_RESULT isValidPatternArguments(PROCESSED_SYNONYMS proc_s, SYNONYM_NAME s,
		ARGUMENT first_arg, ARGUMENT second_arg);
};