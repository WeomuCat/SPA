#pragma once

#include "PKB.h"
#include "Parser.h"
#include "PKB/ASTNode/NodeTypeEnum.h"

class ParserPrintTree {
public:
	std::string parserPrintTree(AST_NODE_PTR);
	LIST_OF_STRINGS getNodeProperties(AST_NODE_PTR, NODE_TYPE);
};