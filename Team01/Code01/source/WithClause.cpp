#include "WithClause.h"

WithClause::WithClause(PROCESSED_SYNONYMS processed_synonyms, QueryNode with_clause) {
	this->processed_synonyms = processed_synonyms;
	this->with_clause = with_clause;
	this->lhs = with_clause.getChildren()[0];
	this->rhs = with_clause.getChildren()[1];
	this->lhs_type = lhs.getNodeType();
	this->rhs_type = rhs.getNodeType();
}

void WithClause::getWithResult(PKB pkb, bool& clause_bool, ResultList& clause_result_list) {
	/*
	Format: with ref = ref
	ref: IDENT | INTEGER | attrRef | synonym
	attrRef: synonym.attrName
	attrName: procName | varName | value | stmt#

	Both refs must be of the same type (ie both strings or both integers)

	IDENT			-> string
	INTEGER			-> int
	synonym			-> int [prog_line ONLY]
	attrRef			-> int/string

	attrRef Types:
	1. [procName]	- string	-> procedure.procName | call.procName
	2. [varName]	- string	-> variable.varName | read.varName | print.varName
	3. [value]		- int		-> constant.value
	4. [stmt#]		- int		-> s.stmt# | read.stmt# | print.stmt# | assign.stmt# | call.stmt# | while.stmt# | if.stmt#


	synonyms:
	1. stmt
	2. read
	3. print
	4. call
	5. while
	6. if
	7. assign
	8. variable
	9. constant
	10. procedure
	11. prog_line		-> int

	Types:
	1. attrRef + INTEGER 		-> [int] with a.stmt# = 12 | with 12 = a.stmt#						-> INTEGER is int
	2. attrRef	+ IDENT			-> [string] with p.procName = "main" | with "main" = p.procName		-> IDENT is string
	3. attrRef + synonym		-> [int] with n = c.value | with c.value = n						-> the only synonym is prog_line
	4. attrRef + attrRef		-> [int] with a.stmt# = s.stmt# | with s.stmt# = a.stmt#
								-> [string] with p.procName = v.varName
	5. IDENT + INTEGER			-> INVALID															-> must compare same type				
	6. IDENT + synonym			-> INVALID															-> the only synonym is prog_line which is int
	7. IDENT + IDENT			-> [string] with "main" = "woof"
	8. INTEGER + INTEGER		-> [int] with 5 = 3
	9. INTEGER + synonym		-> [int] with n = 10 | with 10 = n									-> the only synonym is prog_line
	10. synonym + synonym		-> [int] with n1 = n2
	*/
	this->pkb = pkb;
	this->clause_bool = clause_bool;
	this->clause_result_list = clause_result_list;

	if (lhs_type == QueryNodeType::attr && rhs_type == QueryNodeType::integer) {
		getAttrrefIntResult(lhs, rhs, clause_bool, clause_result_list);
		return;
	} 
	if (rhs_type == QueryNodeType::attr && lhs_type == QueryNodeType::integer) {
		getAttrrefIntResult(rhs, lhs, clause_bool, clause_result_list);
		return;
	}
}

void WithClause::getAttrrefIntResult(QueryNode attrref_node, QueryNode int_node, bool& clause_bool, ResultList& clause_result_list) {
	//1. attrRef + INTEGER 		->[int] with a.stmt#  = 12 | with 12 = a.stmt#
	SYNONYM_NAME synonym_name = attrref_node.getString();
	SYNONYM_TYPE synonym_type = processed_synonyms.find(synonym_name)->second.getSynonymType();
	ATTRIBUTE attribute = attrref_node.getAttr();
	ATTR_REF_VALUES_LIST attrref_values = AttrRefManager::getAttrRefValues(pkb, synonym_type, attribute);

	INTEGER int_num = int_node.getInteger();
	std::string int_num_string = std::to_string(int_num);

	SYNONYM_VALUES_LIST filtered_list;
	for (ATTR_REF_VALUE attrref_value : attrref_values) {
		if (attrref_value.compare(int_num_string) == 0) {
			filtered_list.push_back(int_num_string);
		}
	}

	if (filtered_list.size() > 0) {
		clause_bool = true;
	}

	clause_result_list.addColumn(synonym_name, filtered_list);
}

bool WithClause::isValidWithClause() {
	// For the WithClause to be valid, both needs to be the same type (int or string)
	bool isValid = false;

	if (bothAreIntegerTypes()) {
		isValid = true;
	}
	if (bothAreStringTypes()) {
		isValid = true;
	}

	return isValid;
}

bool WithClause::bothAreIntegerTypes() {
	bool areIntegers = false;
	if (isIntegerType(lhs) && isIntegerType(rhs)) {
		areIntegers = true;
	}
	return areIntegers;
}

bool WithClause::isIntegerType(QueryNode node) {
	bool isInteger = false;
	QueryNodeType node_type = node.getNodeType();
	
	// int values: INTEGER | attrRef int | prog_line
	if (node_type == QueryNodeType::attr) {
		ATTRIBUTE node_attr = node.getAttr();
		if (AttrRefManager::isIntegerType(node_attr)) {
			isInteger = true;
		}
	}
	if (node_type == QueryNodeType::integer) {
		isInteger = true;
	}
	if (node_type == QueryNodeType::synonym) {
		if (node.getSynonymType() == QuerySynonymType::prog_line) {
			isInteger = true;
		}
	}

	return isInteger;
}

bool WithClause::bothAreStringTypes() {
	bool areStrings = false;
	if (isStringType(lhs) && isStringType(rhs)) {
		areStrings = true;
	}
	return areStrings;
}

bool WithClause::isStringType(QueryNode node) {
	bool isString = false;
	QueryNodeType node_type = node.getNodeType();

	// string type: IDENT | attrRef string
	if (node_type == QueryNodeType::ident) {
		isString = true;
	}
	if (node_type == QueryNodeType::attr) {
		ATTRIBUTE node_attr = node.getAttr();
		if (AttrRefManager::isStringType(node_attr)) {
			isString = true;
		}
	}

	return isString;
}

