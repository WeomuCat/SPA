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

	QueryNodeContent(INTEGER integer);

	QueryNodeContent(AST_NODE astnode);

	QueryNodeContent(STRING syn, ATTRIBUTE attribute);

	STRING getString();

	INTEGER getInt();

	AST_NODE getAstNode();

	ATTRIBUTE getAttribute();
};