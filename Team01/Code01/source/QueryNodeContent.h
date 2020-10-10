#pragma once

#include "./PKB/ASTNode/ASTNode.h"
#include <string>

class QueryNodeContent;

typedef std::string STRING;
typedef int INTEGER;
typedef std::shared_ptr<ASTNode> AST_NODE;
typedef int MODIFIED_FLAG;

class QueryNodeContent {
private:
	STRING strings = "";
	INTEGER integer = -1;
	AST_NODE astnode;
	MODIFIED_FLAG modified = -1;
	
public:
	QueryNodeContent();

	QueryNodeContent(STRING content);

	QueryNodeContent(INTEGER integer);

	QueryNodeContent(AST_NODE astnode);

	STRING getString();

	INTEGER getInt();

	AST_NODE getAstNode();
};