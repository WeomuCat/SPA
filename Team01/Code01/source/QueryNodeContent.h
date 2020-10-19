#pragma once

#include "./PKB/ASTNode/ASTNode.h"
#include "./AttributeType.h"
#include <string>

class QueryNodeContent;

typedef std::string STRING;
typedef int INTEGER;
typedef std::shared_ptr<ASTNode> AST_NODE;
typedef AttributeType ATTRIBUTE;
typedef std::string ATTRIBUTE_STRING;
typedef int MODIFIED_FLAG;

class QueryNodeContent {
private:
	STRING strings = "";
	INTEGER integer = -1;
	AST_NODE astnode;
	ATTRIBUTE attribute;
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


	QueryNodeContent(STRING syn, ATTRIBUTE_STRING attribute);
	/*
	Description: Creates a QueryNodeContent with the given attribute and string.
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

	ATTRIBUTE getAttribute();
	/*
	Description: Returns this attribute value.
	*/

};