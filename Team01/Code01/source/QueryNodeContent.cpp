#include <regex>

#include "QueryNodeContent.h"

QueryNodeContent::QueryNodeContent() {}

QueryNodeContent::QueryNodeContent(STRING content) {
	this->strings = content;
	this->modified = 1;
}

QueryNodeContent::QueryNodeContent(INTEGER integer) {
	this->integer = integer;
	this->modified = 2;
}

QueryNodeContent::QueryNodeContent(AST_NODE astnode) {
	this->astnode = astnode;
	this->modified = 3;
}

QueryNodeContent::QueryNodeContent(STRING syn, ATTRIBUTE_STRING attribute) {
	this->strings = syn;
	if (std::regex_match(attribute, std::regex("procName"))) {
		this->attribute = AttributeType::procName;
	}
	else if (std::regex_match(attribute, std::regex("varName"))) {
		this->attribute = AttributeType::varName;
	}
	else if (std::regex_match(attribute, std::regex("value"))) {
		this->attribute = AttributeType::value;
	}
	else if (std::regex_match(attribute, std::regex("stmt#"))) {
		this->attribute = AttributeType::stmtNum;
	}
	this->modified = 4;
}

STRING QueryNodeContent::getString() {
	return strings;
}

INTEGER QueryNodeContent::getInt() {
	return integer;
}

AST_NODE QueryNodeContent::getAstNode() {
	return astnode;
}

ATTRIBUTE QueryNodeContent::getAttribute() {
	return attribute;
}
