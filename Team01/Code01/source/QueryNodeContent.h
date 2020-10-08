#pragma once

#include "./PKB/ASTNode/ASTNode.h"
#include <string>

class QueryNodeContent;

typedef std::string STRING;
typedef int INTEGER;
typedef std::shared_ptr<ASTNode> AST_NODE;
typedef bool BOOLEAN;
typedef int MODIFIED_FLAG;

class QueryNodeContent {
private:
	STRING strings = "";
	INTEGER integer = -1;
	AST_NODE astnode;
	BOOLEAN boolean = false;
	MODIFIED_FLAG modified = -1;
	
public:
	QueryNodeContent();

	QueryNodeContent(STRING content);
	/*
	Description: Creates a QueryNodeContent with the given string.
				 All other values will remain their default values.
	*/

	QueryNodeContent(INTEGER integer);
	/*
	Description: Creates a QueryNodeContent with the given integer.
				 All other values will remain their default values.
	*/

	QueryNodeContent(AST_NODE astnode);
	/*
	Description: Creates a QueryNodeContent with the given astnode.
				 All other values will remain their default values.
	*/

	QueryNodeContent(BOOLEAN boolean);
	/*
	Description: Creates a QueryNodeContent with the given boolean.
				 All other values will remain their default values.
	*/

	STRING getString();
	/*
	Description: Returns this string value.
	*/

	INTEGER getInt();
	/*
	Description: Returns this integer value.
	*/

	AST_NODE getAstNode();
	/*
	Description: Returns this ASTNode.
	*/

	BOOLEAN getBool();
	/*
	Description: Returns this boolean value.
	*/
};