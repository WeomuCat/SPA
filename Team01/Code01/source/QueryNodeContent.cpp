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

QueryNodeContent::QueryNodeContent(STRING syn, ATTRIBUTE attribute) {
	this->strings = syn;
	this->attribute = attribute;
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
