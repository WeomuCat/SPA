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

	QueryNodeContent(INTEGER integer);

	QueryNodeContent(AST_NODE astnode);

	QueryNodeContent(BOOLEAN boolean);

	STRING getString();

	INTEGER getInt();

	AST_NODE getAstNode();

	BOOLEAN getBool();
};