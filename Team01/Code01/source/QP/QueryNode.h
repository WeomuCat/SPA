#pragma once

#include <string>
#include <vector>

#include "QueryNodeType.h"
#include "QuerySynonymType.h"
#include "QueryNodeContent.h"

class QueryNode {
private:
	QUERY_NODE_TYPE node_type;
	std::vector<QueryNode> children;
	SYNONYM_TYPE synonym_type;
	QUERY_NODE_CONTENT node_content = QueryNodeContent();

public:
	QueryNode();
	void setNodeType(QUERY_NODE_TYPE node_type);
	void setNodeType(NODE_TYPE_STRING node_type_string);

	QUERY_NODE_TYPE getNodeType();

	void setSynonymNode(SYNONYM_TYPE synonym_type, SYNONYM_NAME synonym_name); //synonym nodes MUST have snyonym name
	void setSynonymNode(SYNONYM_TYPE_STRING synonym_type_string, SYNONYM_NAME synonym_name); //synonym nodes MUST have snyonym name
	SYNONYM_TYPE getSynonymType();
	void setIdentityNode(STRING identity);
	STRING getString();

	void setIntegerNode(INTEGER integer);
	INTEGER getInteger();

	void setASTNode(AST_NODE astnode);
	AST_NODE getAstNode();

	void setAttrNode(STRING syn, ATTRIBUTE_STRING attribute);
	ATTRIBUTE getAttr();

	void setBooleanNode();

	void setChildren(QUERY_NODE_POINTERS children, int size); //need to add all children at once, in the right order
	void addChild(QueryNode child); //add a single child to node
	QUERY_NODE_LIST getChildren();

	QUERY_NODE_CONTENT getQueryNodeContent(); //generic case to get all the node content

	static bool isEquals(QueryNode first, QueryNode second);

	static bool isSameSynonymNode(QueryNode first, QueryNode second);

	static bool isSameIntegerNode(QueryNode first, QueryNode second);

	static bool isSameIdentNode(QueryNode first, QueryNode second);

	static bool isSameAttrNode(QueryNode first, QueryNode second);
};