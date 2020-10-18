#pragma once

#include <string>
#include <unordered_map>

#include "QueryNode.h"

typedef bool VALIDATION_RESULT;
typedef std::string QUERY;
typedef std::string CLAUSES;
typedef std::string ELEMENT;
typedef std::string RESULT;
typedef std::string SINGLE_DECLARATION;
typedef std::string SINGLE_CLAUSE;
typedef std::unordered_map<std::string, QueryNode> PROCESSED_SYNONYMS;
typedef std::string SINGLE_ARGUMENT;
typedef std::vector<SINGLE_ARGUMENT> ARGUMENTS;
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
	Description: Returns a VALIDATION_RESULT based on grammar for elements.
	*/
	static VALIDATION_RESULT isValidElem(PROCESSED_SYNONYMS proc_s, ELEMENT elem);

	/*
	Description: Returns a VALIDATION_RESULT based on format for result clause.
	*/
	static VALIDATION_RESULT isValidResultFormat(RESULT res);

	/*
	Description: Returns a VALIDATION_RESULT based on relationship format.
	*/
	static VALIDATION_RESULT isValidRelationFormat(SINGLE_CLAUSE single_c);

	/*
	Description: Returns a VALIDATION_RESULT based on whether argument returns a statemenr number.
	*/
	static VALIDATION_RESULT isStatementRef(PROCESSED_SYNONYMS proc_s, SINGLE_ARGUMENT a);

	/*
	Description: Returns a VALIDATION_RESULT based on whether argument returns an entity reference.
	*/
	static VALIDATION_RESULT isEntityRef(PROCESSED_SYNONYMS proc_s, SINGLE_ARGUMENT a);

	/*
	Description: Returns a VALIDATION_RESULT based on whether argument returns a line reference.
	*/
	static VALIDATION_RESULT isLineRef(PROCESSED_SYNONYMS proc_s, SINGLE_ARGUMENT a);

	/*
	Description: Returns a VALIDATION_RESULT based on relationship argument validity.
	*/
	static VALIDATION_RESULT isValidRelationArguments(PROCESSED_SYNONYMS proc_s, RELATIONSHIP rel,
		ARGUMENTS args);

	/*
	Description: Returns a VALIDATION_RESULT based on pattern format.
	*/
	static VALIDATION_RESULT isValidPatternFormat(SINGLE_CLAUSE single_c);

	/*
	Description: Returns a VALIDATION_RESULT based on pattern argument validity.
	*/
	static VALIDATION_RESULT isValidPatternArguments(PROCESSED_SYNONYMS proc_s, SYNONYM_NAME s,
		ARGUMENTS args);

	/*
	Description: Returns a VALIDATION_RESULT based on with format.
	*/
	static VALIDATION_RESULT isValidWithFormat(SINGLE_CLAUSE single_c);

	/*
	Description: Returns a VALIDATION_RESULT based on whether argument is a valid reference.
	*/
	static VALIDATION_RESULT isValidRef(PROCESSED_SYNONYMS proc_s, SINGLE_ARGUMENT a);
};