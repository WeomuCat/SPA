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
	if (std::regex_match(node_type_string, std::regex("select"))) {
		this->node_type = QueryNodeType::select;
	}
	else if (std::regex_match(node_type_string, std::regex("synonym"))) {
		this->node_type = QueryNodeType::synonym;
	}
	else if (std::regex_match(node_type_string, std::regex("such that"))) {
		this->node_type = QueryNodeType::such_that;
	}
	else if (std::regex_match(node_type_string, std::regex("pattern"))) {
		this->node_type = QueryNodeType::pattern;
	}
	else if (std::regex_match(node_type_string, std::regex("Follows"))) {
		this->node_type = QueryNodeType::follows;
	}
	else if (std::regex_match(node_type_string, std::regex("Follows\\*"))) {
		this->node_type = QueryNodeType::followsT;
	}
	else if (std::regex_match(node_type_string, std::regex("Parent"))) {
		this->node_type = QueryNodeType::parent;
	}
	else if (std::regex_match(node_type_string, std::regex("Parent\\*"))) {
		this->node_type = QueryNodeType::parentT;
	}
	else if (std::regex_match(node_type_string, std::regex("UsesS"))) {
		this->node_type = QueryNodeType::usesS;
	}
	else if (std::regex_match(node_type_string, std::regex("UsesP"))) {
		this->node_type = QueryNodeType::usesP;
	}
	else if (std::regex_match(node_type_string, std::regex("ModifiesS"))) {
		this->node_type = QueryNodeType::modifiesS;
	}
	else if (std::regex_match(node_type_string, std::regex("ModifiesP"))) {
		this->node_type = QueryNodeType::modifiesP;
	}
	else if (std::regex_match(node_type_string, std::regex("Calls"))) {
		this->node_type = QueryNodeType::calls;
	}
	else if (std::regex_match(node_type_string, std::regex("Calls\\*"))) {
		this->node_type = QueryNodeType::callsT;
	}
	else if (std::regex_match(node_type_string, std::regex("Next"))) {
		this->node_type = QueryNodeType::next;
	}
	else if (std::regex_match(node_type_string, std::regex("Next\\*"))) {
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

	if (std::regex_match(synonym_type_string, std::regex("stmt"))) {
		this->synonym_type = QuerySynonymType::stmt;
	}
	else if (std::regex_match(synonym_type_string, std::regex("read"))) {
		this->synonym_type = QuerySynonymType::read;
	}
	else if (std::regex_match(synonym_type_string, std::regex("print"))) {
		this->synonym_type = QuerySynonymType::print;
	}
	else if (std::regex_match(synonym_type_string, std::regex("calll"))) {
		this->synonym_type = QuerySynonymType::call;
	}
	else if (std::regex_match(synonym_type_string, std::regex("while"))) {
		this->synonym_type = QuerySynonymType::whiles;
	}
	else if (std::regex_match(synonym_type_string, std::regex("if"))) {
		this->synonym_type = QuerySynonymType::ifs;
	}
	else if (std::regex_match(synonym_type_string, std::regex("assign"))) {
		this->synonym_type = QuerySynonymType::assign;
	}
	else if (std::regex_match(synonym_type_string, std::regex("variable"))) {
		this->synonym_type = QuerySynonymType::variable;
	}
	else if (std::regex_match(synonym_type_string, std::regex("constant"))) {
		this->synonym_type = QuerySynonymType::constant;
	}
	else if (std::regex_match(synonym_type_string, std::regex("prog_line"))) {
		this->synonym_type = QuerySynonymType::prog_line;
	}
	else if (std::regex_match(synonym_type_string, std::regex("procedure"))) {
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
