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

void QueryNode::setBooleanNode() {
	this->node_type = { QueryNodeType::boolean };
}

STRING QueryNode::getString() {
	return node_content.getString();
}

void QueryNode::setChildren(QUERY_NODE_POINTERS children, int size) {
	for (int i = 0; i < size; i++) {
		this->children.push_back(children[i]);
	}
}

void QueryNode::addChild(QueryNode child) {
	this->children.push_back(child);
}

QUERY_NODE_LIST QueryNode::getChildren() {
	return QUERY_NODE_LIST(children);
}

QUERY_NODE_CONTENT QueryNode::getQueryNodeContent() {
	return node_content;
}

bool QueryNode::isEquals(QueryNode first, QueryNode second) {
	QueryNodeType first_type = first.getNodeType();
	QueryNodeType second_type = second.getNodeType();

	if (first_type == second_type) {
		if (first_type == QueryNodeType::such_that) {
			QueryNode first_child = first.getChildren()[0];
			QueryNode second_child = second.getChildren()[0];

			// check if they have the same relationship type
			if (first_child.getNodeType() == second_child.getNodeType()) {
				// check if the children are the same
				QueryNode first_lhs = first_child.getChildren()[0];
				QueryNode first_rhs = first_child.getChildren()[1];
				QueryNodeType first_lhs_type = first_lhs.getNodeType();
				QueryNodeType first_rhs_type = first_rhs.getNodeType();

				QueryNode second_lhs = second_child.getChildren()[0];
				QueryNode second_rhs = second_child.getChildren()[1];
				QueryNodeType second_lhs_type = second_lhs.getNodeType();
				QueryNodeType second_rhs_type = second_rhs.getNodeType();
				
				if (first_lhs_type == second_lhs_type && first_rhs_type == second_rhs_type) {
					bool lhsIsEqual = false;
					bool rhsIsEqual = false;

					// check if lhs of both clauses are equal
					if (first_lhs_type == QueryNodeType::synonym) {
						if (isSameSynonymNode(first_lhs, second_lhs)) {
							lhsIsEqual = true;
						}
					}
					else if (first_lhs_type == QueryNodeType::integer) {
						if (isSameIntegerNode(first_lhs, second_lhs)) {
							lhsIsEqual = true;
						}
					}
					else if (first_lhs_type == QueryNodeType::ident) {
						if (isSameIdentNode(first_lhs, second_lhs)) {
							lhsIsEqual = true;
						}
					}
					else if (first_lhs_type == QueryNodeType::wild_card) {
						lhsIsEqual = true;
					}


					// check if rhs of both clauses are equal
					if (first_rhs_type == QueryNodeType::synonym) {
						if (isSameSynonymNode(first_rhs, second_rhs)) {
							rhsIsEqual = true;
						}
					}
					else if (first_rhs_type == QueryNodeType::integer) {
						if (isSameIntegerNode(first_rhs, second_rhs)) {
							rhsIsEqual = true;
						}
					}
					else if (first_rhs_type == QueryNodeType::ident) {
						if (isSameIdentNode(first_rhs, second_rhs)) {
							rhsIsEqual = true;
						}
					}
					else if (first_rhs_type == QueryNodeType::wild_card) {
						rhsIsEqual = true;
					}

					if (lhsIsEqual && rhsIsEqual) {
						return true;
					}
				}

			}
		}

		if (first_type == QueryNodeType::with) {
			QueryNode first_lhs = first.getChildren()[0];
			QueryNode first_rhs = first.getChildren()[1];
			QueryNodeType first_lhs_type = first_lhs.getNodeType();
			QueryNodeType first_rhs_type = first_rhs.getNodeType();

			QueryNode second_lhs = second.getChildren()[0];
			QueryNode second_rhs = second.getChildren()[1];
			QueryNodeType second_lhs_type = second_lhs.getNodeType();
			QueryNodeType second_rhs_type = second_rhs.getNodeType();

			if (first_lhs_type == second_lhs_type && first_rhs_type == second_rhs_type) {
				bool lhsIsEqual = false;
				bool rhsIsEqual = false;

				// check if lhs are equal
				if (first_lhs_type == QueryNodeType::attr) {
					if (isSameAttrNode(first_lhs, second_lhs)) {
						lhsIsEqual = true;
					} 
				}
				else if (first_lhs_type == QueryNodeType::integer) {
					if (isSameIntegerNode(first_lhs, second_lhs)) {
						lhsIsEqual = true;
					}
				}
				else if (first_lhs_type == QueryNodeType::ident) {
					if (isSameIdentNode(first_lhs, second_lhs)) {
						lhsIsEqual = true;
					}
				}
				else if (first_lhs_type == QueryNodeType::wild_card) {
					lhsIsEqual = true;
				}

				// check if rhs are equal
				if (first_rhs_type == QueryNodeType::attr) {
					if (isSameAttrNode(first_rhs, second_rhs)) {
						rhsIsEqual = true;
					}
				}
				else if (first_rhs_type == QueryNodeType::integer) {
					if (isSameIntegerNode(first_rhs, second_rhs)) {
						rhsIsEqual = true;
					}
				}
				else if (first_rhs_type == QueryNodeType::ident) {
					if (isSameIdentNode(first_rhs, second_rhs)) {
						rhsIsEqual = true;
					}
				}
				else if (first_rhs_type == QueryNodeType::wild_card) {
					rhsIsEqual = true;
				}


				if (lhsIsEqual && rhsIsEqual) {
					return true;
				}
			}

		}

		if (first_type == QueryNodeType::pattern) {
			QueryNode first_synonym_node = first.getChildren()[0];
			QuerySynonymType first_synonym_type = first_synonym_node.getSynonymType();
			SYNONYM_NAME first_synonym_name = first_synonym_node.getString();

			QueryNode second_synonym_node = second.getChildren()[0];
			QuerySynonymType second_synonym_type = second_synonym_node.getSynonymType();
			SYNONYM_NAME second_synonym_name = second_synonym_node.getString();

			if (first_synonym_type == second_synonym_type && first_synonym_name == second_synonym_name) {
				return true;
			}
		}
	}

	return false;
}

bool QueryNode::isSameSynonymNode(QueryNode first, QueryNode second) {
	SYNONYM_TYPE first_type = first.getSynonymType();
	SYNONYM_NAME first_name = first.getString();
	SYNONYM_TYPE second_type = second.getSynonymType();
	SYNONYM_NAME second_name = second.getString();

	if (first_type == second_type && first_name == second_name) {
		return true;
	}
	else {
		return false;
	}
}

bool QueryNode::isSameIntegerNode(QueryNode first, QueryNode second) {
	INTEGER first_int = first.getInteger();
	INTEGER second_int = second.getInteger();

	if (first_int == second_int) {
		return true;
	}
	else {
		return false;
	}
}

bool QueryNode::isSameIdentNode(QueryNode first, QueryNode second) {
	STRING first_ident = first.getString();
	STRING second_ident = second.getString();

	if (first_ident == second_ident) {
		return true;
	}
	else {
		return false;
	}
}

bool QueryNode::isSameAttrNode(QueryNode first, QueryNode second) {
	SYNONYM_NAME first_name = first.getString();
	ATTRIBUTE first_attr = first.getAttr();

	SYNONYM_NAME second_name = second.getString();
	ATTRIBUTE second_attr = second.getAttr();

	if (first_name == second_name && first_attr == second_attr) {
		return true;
	}
	else {
		return false;
	}
}
