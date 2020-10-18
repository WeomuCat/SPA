#include <regex>

#include "QueryNode.h"
#include "QueryNodeType.h"
#include <iterator>

QueryNode::QueryNode() {
	this->node_type = QueryNodeType::unassigned;
	this->synonym_type = QuerySynonymType::unassigned;
}

void QueryNode::setNodeType(QUERY_NODE_TYPE node_type) {
	this->node_type = node_type;
}

void QueryNode::setNodeType(NODE_TYPE_STRING node_type_string) {
	if (node_type_string.compare("select") == 0) {
		this->node_type = QueryNodeType::select;
	}
	else if (node_type_string.compare("synonym") == 0) {
		this->node_type = QueryNodeType::synonym;
	}
	else if (node_type_string.compare("such that") == 0) {
		this->node_type = QueryNodeType::such_that;
	}
	else if (node_type_string.compare("pattern") == 0) {
		this->node_type = QueryNodeType::pattern;
	}
	else if (node_type_string.compare("Follows") == 0) {
		this->node_type = QueryNodeType::follows;
	}
	else if (node_type_string.compare("Follows*") == 0) {
		this->node_type = QueryNodeType::followsT;
	}
	else if (node_type_string.compare("Parent") == 0) {
		this->node_type = QueryNodeType::parent;
	}
	else if (node_type_string.compare("Parent*") == 0) {
		this->node_type = QueryNodeType::parentT;
	}
	else if (node_type_string.compare("UsesS") == 0) {
		this->node_type = QueryNodeType::usesS;
	}
	else if (node_type_string.compare("UsesP") == 0) {
		this->node_type = QueryNodeType::usesP;
	}
	else if (node_type_string.compare("ModifiesS") == 0) {
		this->node_type = QueryNodeType::modifiesS;
	}
	else if (node_type_string.compare("ModifiesP") == 0) {
		this->node_type = QueryNodeType::modifiesP;
	}
	else if (node_type_string.compare("Calls") == 0) {
		this->node_type = QueryNodeType::calls;
	}
	else if (node_type_string.compare("Calls*") == 0) {
		this->node_type = QueryNodeType::callsT;
	}
	else if (node_type_string.compare("Next") == 0) {
		this->node_type = QueryNodeType::next;
	}
	else if (node_type_string.compare("Next*") == 0) {
		this->node_type = QueryNodeType::nextT;
	}
}

QUERY_NODE_TYPE QueryNode::getNodeType() {
	return node_type;
}


void QueryNode::setIntegerNode(INTEGER integer) {
	this->node_type = { QueryNodeType::integer };
	QueryNodeContent content = QueryNodeContent(integer);
	this->node_content = content;
}

INTEGER QueryNode::getInteger() {
	return node_content.getInt();
}


void QueryNode::setASTNode(AST_NODE astnode) {
	this->node_type = { QueryNodeType::expression };

	QueryNodeContent content = QueryNodeContent(astnode);
	this->node_content = content;
}

AST_NODE QueryNode::getAstNode() {
	return node_content.getAstNode();
}

void QueryNode::setSynonymNode(SYNONYM_TYPE synonym_type, SYNONYM_NAME synonym_name) {
	this->node_type = { QueryNodeType::synonym };
	this->synonym_type = synonym_type;
	QueryNodeContent content = QueryNodeContent(synonym_name);
	this->node_content = content;
}

void QueryNode::setSynonymNode(SYNONYM_TYPE_STRING synonym_type_string, SYNONYM_NAME synonym_name) {
	this->node_type = {QueryNodeType::synonym};

	if (synonym_type_string.compare("stmt") == 0) {
		this->synonym_type = QuerySynonymType::stmt;
	}
	else if (synonym_type_string.compare("read") == 0) {
		this->synonym_type = QuerySynonymType::read;
	}
	else if (synonym_type_string.compare("print") == 0) {
		this->synonym_type = QuerySynonymType::print;
	}
	else if (synonym_type_string.compare("call") == 0) {
		this->synonym_type = QuerySynonymType::call;
	}
	else if (synonym_type_string.compare("while") == 0) {
		this->synonym_type = QuerySynonymType::whiles;
	}
	else if (synonym_type_string.compare("if") == 0) {
		this->synonym_type = QuerySynonymType::ifs;
	}
	else if (synonym_type_string.compare("assign") == 0) {
		this->synonym_type = QuerySynonymType::assign;
	}
	else if (synonym_type_string.compare("variable") == 0) {
		this->synonym_type = QuerySynonymType::variable;
	}
	else if (synonym_type_string.compare("constant") == 0) {
		this->synonym_type = QuerySynonymType::constant;
	}
	else if (synonym_type_string.compare("prog_line") == 0) {
		this->synonym_type = QuerySynonymType::prog_line;
	}
	else if (synonym_type_string.compare("procedure") == 0) {
		this->synonym_type = QuerySynonymType::procedure;
	}

	QueryNodeContent content = QueryNodeContent(synonym_name);
	this->node_content = content;
}

SYNONYM_TYPE QueryNode::getSynonymType() {
	return synonym_type;
}

void QueryNode::setIdentityNode(STRING identity) {
	this->node_type = { QueryNodeType::ident };
	QueryNodeContent content = QueryNodeContent(identity);
	this->node_content = content;
}

void QueryNode::setAttrNode(STRING syn, ATTRIBUTE_STRING attribute) {
	this->node_type = { QueryNodeType::attr };
	QueryNodeContent content = QueryNodeContent(syn, attribute);
	this->node_content = content;
}

ATTRIBUTE QueryNode::getAttr() {
	return node_content.getAttribute();
}

STRING QueryNode::getString() {
	return node_content.getString();
}

void QueryNode::setChildren(QUERY_NODE_POINTERS children, int size) {
	for (int i = 0; i < size; i++) {
		this->children.push_back(children[i]);
	}
}

QUERY_NODE_LIST QueryNode::getChildren() {
	return QUERY_NODE_LIST(children);
}

QUERY_NODE_CONTENT QueryNode::getQueryNodeContent() {
	return node_content;
}
