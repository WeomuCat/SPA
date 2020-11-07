#pragma once

#include <queue>
#include <regex>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "../PKB/ASTNode/ConstantNode.h"
#include "../PKB/ASTNode/VariableNode.h"
#include "../PKB/ASTNode/ExpressionTypeEnum.h"
#include "../PKB/ASTNode/ExpressionNode.h"
#include "QueryNode.h"
#include "ClauseQueue.h"
#include "QueryNodeType.h"
#include "QueryValidator.h"

typedef std::vector<std::string> SPLIT_QUERY;
typedef std::string DECLARATIONS;
typedef std::vector<std::string> SPLIT_DECLARATIONS;
typedef QueryNode PROCESSED_CLAUSES;
typedef std::string EXPRESSION;
typedef std::vector<TOKEN> INFIX_EXPR;
typedef std::string STRING;
typedef int INDEX;
typedef std::vector<INDEX> NEXT_INDICES;

/*
Overview: Pre-processes queries into a suitable data structure for the evaluator.
*/
class QueryPreProcessor {

public:
	/*
	Description: Returns a SPLIT_QUERY.
	*/
	SPLIT_QUERY splitQuery(INPUT_QUERY q);

	/*
	Description: Returns PROCESSED_SYNONYMS.
				 If synonyms are not valid, returns an empty map.
	*/
	PROCESSED_SYNONYMS preProcessSynonyms(DECLARATIONS d);

	/*
	Description: Returns PROCESSED_CLAUSES.
				 If clauses are not valid, returns a query node with an unassigned NODE_TYPE.
	*/
	PROCESSED_CLAUSES preProcessClauses(PROCESSED_SYNONYMS proc_s, CLAUSES c);

private:
	/*
	Description: Returns SPLIT_DECLARATIONS.
	*/
	SPLIT_DECLARATIONS splitDeclarations(DECLARATIONS d);

	/*
	Description: Returns a QueryNode for the element.
	*/
	QueryNode createElemNode(PROCESSED_SYNONYMS proc_s, ELEMENT e);

	/*
	Description: Returns a QueryNode for the result clause.
				 If result clause is not valid, returns a query node with an unassigned NODE_TYPE.
	*/
	QueryNode createResultNode(PROCESSED_SYNONYMS proc_s, RESULT r);

	/*
	Description: Returns NEXT_INDICES, the starting and ending indices of the next clause.
	*/
	NEXT_INDICES getNextClauseIndex(CLAUSES c, INDEX current_index);

	/*
	Description: Returns a vector of arguments for a clause.
	*/
	ARGUMENTS getArguments(SINGLE_CLAUSE c);

	/*
	Description: Returns a tokenized INFIX_EXPR.
	*/
	INFIX_EXPR tokenizeExpression(EXPRESSION e);

	/*
	Description: Returns priotity of TOKEN.
				 The larger the number, the higher the priority.
	*/
	INTEGER getTokenPriority(TOKEN t);

	/*
	Description: Returns a POSTFIX_EXPR.
	*/
	POSTFIX_EXPR infixToPostfix(INFIX_EXPR e);

	/*
	Description: Returns a QueryNode for the expression.
	*/
	QueryNode createExpressionNode(EXPRESSION e);

	/*
	Description: Returns a QueryNode for the argument.
	*/
	QueryNode createArgumentNode(PROCESSED_SYNONYMS proc_s, SINGLE_ARGUMENT arg);

	/*
	Description: Returns a QueryNode for the relationship.
				 If relationship clause is not valid syntatically, returns a query node with an unassigned NODE_TYPE.
				 If result clause is not valid semantically, returns a query node with an follows NODE_TYPE, without children.
	*/
	QueryNode createRelationNode(PROCESSED_SYNONYMS proc_s, SINGLE_CLAUSE c);

	/*
	Description: Returns a QueryNode for the pattern.
				 If pattern clause is not valid syntatically, returns a query node with an unassigned NODE_TYPE.
				 If result clause is not valid semantically, returns a query node with an pattern NODE_TYPE, without children.
	*/
	QueryNode createPatternNode(PROCESSED_SYNONYMS proc_s, SINGLE_CLAUSE c);

	/*
	Description: Returns a QueryNode for the pattern.
				 If with clause is not valid syntatically, returns a query node with an unassigned NODE_TYPE.
				 If with clause is not valid semantically, returns a query node with a with NODE_TYPE, without children.
	*/
	QueryNode createWithNode(PROCESSED_SYNONYMS proc_s, SINGLE_CLAUSE c);

	/*
	Description: Trims whitespaces from front and back of STRING and returns it.
	*/
	STRING trimWhitespaces(STRING s);
};