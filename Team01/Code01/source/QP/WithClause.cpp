#include "WithClause.h"

WithClause::WithClause(PROCESSED_SYNONYMS processed_synonyms, QueryNode with_clause, ResultList& intermediate_result_list) {
	this->processed_synonyms = processed_synonyms;
	this->intermediate_result_list = intermediate_result_list;
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

	if (!isValidWithClause()) {
		clause_bool = false;
		return;
	}

	// 1. attrRef + INTEGER 		-> [int] with a.stmt# = 12 | with 12 = a.stmt#
	if (lhs_type == QueryNodeType::attr && rhs_type == QueryNodeType::integer) {
		getAttrrefIntResult(lhs, rhs, clause_bool, clause_result_list);
		return;
	} 
	if (rhs_type == QueryNodeType::attr && lhs_type == QueryNodeType::integer) {
		getAttrrefIntResult(rhs, lhs, clause_bool, clause_result_list);
		return;
	}

	// 2. attrRef	+ IDENT			-> [string] with p.procName = "main" | with "main" = p.procName	
	if (lhs_type == QueryNodeType::attr && rhs_type == QueryNodeType::ident) {
		getAttrrefStringResult(lhs, rhs, clause_bool, clause_result_list);
		return;
	}
	if (rhs_type == QueryNodeType::attr && lhs_type == QueryNodeType::ident) {
		getAttrrefStringResult(rhs, lhs, clause_bool, clause_result_list);
		return;
	}

	// 3. attrRef + synonym		-> [int] with n = c.value | with c.value = n
	if (lhs_type == QueryNodeType::attr && rhs_type == QueryNodeType::synonym && rhs.getSynonymType() == QuerySynonymType::prog_line) {
		getAttrrefSynonymResult(lhs, rhs, clause_bool, clause_result_list);
		return;
	}
	if (rhs_type == QueryNodeType::attr && lhs_type == QueryNodeType::synonym && lhs.getSynonymType() == QuerySynonymType::prog_line) {
		getAttrrefSynonymResult(rhs, lhs, clause_bool, clause_result_list);
		return;
	}

	// 4. attrRef + attrRef		-> [int] with a.stmt# = s.stmt# | with s.stmt# = a.stmt#
	//							-> [string] with p.procName = v.varName
	if (lhs_type == QueryNodeType::attr && rhs_type == QueryNodeType::attr) {
		getAttrrefAttrrefResult(lhs, rhs, clause_bool, clause_result_list);
		return;
	}

	// 5. IDENT + INTEGER			-> INVALID
	if (lhs_type == QueryNodeType::ident && rhs_type == QueryNodeType::integer) {
		clause_bool = false;
		return;
	}
	if (rhs_type == QueryNodeType::ident && lhs_type == QueryNodeType::integer) {
		clause_bool = false;
		return;
	}

	// 6. IDENT + synonym			-> INVALID
	if (lhs_type == QueryNodeType::ident && rhs_type == QueryNodeType::synonym) {
		clause_bool - false;
		return;
	}
	if (rhs_type == QueryNodeType::ident && lhs_type == QueryNodeType::synonym) {
		clause_bool = false;
		return;
	}

	// 7. IDENT + IDENT			->[string] with "main" = "woof"
	if (lhs_type == QueryNodeType::ident && rhs_type == QueryNodeType::ident) {
		getIdentIdentResult(lhs, rhs, clause_bool, clause_result_list);
		return;
	}

	// 8. INTEGER + INTEGER		-> [int] with 5 = 3
	if (lhs_type == QueryNodeType::integer && rhs_type == QueryNodeType::integer) {
		getIntegerIntegerResult(lhs, rhs, clause_bool, clause_result_list);
		return;
	}

	// 9. INTEGER + synonym		->[int] with n = 10 | with 10 = n
	if (lhs_type == QueryNodeType::integer && rhs_type == QueryNodeType::synonym && rhs.getSynonymType() == QuerySynonymType::prog_line) {
		getIntegerSynonymResult(lhs, rhs, clause_bool, clause_result_list);
		return;
	}
	if (rhs_type == QueryNodeType::integer && lhs_type == QueryNodeType::synonym && lhs.getSynonymType() == QuerySynonymType::prog_line) {
		getIntegerSynonymResult(rhs, lhs, clause_bool, clause_result_list);
		return;
	}

	// 10. synonym + synonym		-> [int] with n1 = n2
	if (lhs_type == QueryNodeType::synonym && lhs.getSynonymType() == QuerySynonymType::prog_line && rhs_type == QueryNodeType::synonym && rhs.getSynonymType() == QuerySynonymType::prog_line) {
		getSynonymSynonymResult(lhs, rhs, clause_bool, clause_result_list);
		return;
	}

}

void WithClause::getAttrrefIntResult(QueryNode attrref_node, QueryNode int_node, bool& clause_bool, ResultList& clause_result_list) {
	// 1. attrRef + INTEGER 		->[int] with a.stmt#  = 12 | with 12 = a.stmt#
	SYNONYM_NAME synonym_name = attrref_node.getString();
	SYNONYM_TYPE synonym_type = processed_synonyms.find(synonym_name)->second.getSynonymType();
	ATTRIBUTE attribute = attrref_node.getAttr();
	ATTR_REF_VALUES_LIST attrref_values;

	// if the intermediate resultlist contains this synonym, use the values it in. Else get from PKB.
	if (intermediate_result_list.containsSynonym(synonym_name)) {
		SYNONYM_VALUES_LIST all_values = intermediate_result_list.getValuesOfSynonym(synonym_name);
		std::unordered_set<SYNONYM_VALUE> unique_values(all_values.begin(), all_values.end());
		SYNONYM_VALUES_LIST unique_values_list(unique_values.begin(), unique_values.end());

		attrref_values = AttrRefManager::getAttrRefValues(pkb, synonym_type, unique_values_list, attribute);
	}
	else {
		attrref_values = AttrRefManager::getAttrRefValues(pkb, synonym_type, attribute);
	}

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

void WithClause::getAttrrefStringResult(QueryNode attrref_node, QueryNode string_node, bool& clause_bool, ResultList& clause_result_list) {
	// 2. attrRef + IDENT			->[string] with p.procName = "main" | with "main" = p.procName
	SYNONYM_NAME synonym_name = attrref_node.getString();
	SYNONYM_TYPE synonym_type = processed_synonyms.find(synonym_name)->second.getSynonymType();
	ATTRIBUTE attribute = attrref_node.getAttr();
	ATTR_REF_VALUES_LIST attrref_values;

	// if the intermediate resultlist contains this synonym, use the values it in. Else get from PKB.
	if (intermediate_result_list.containsSynonym(synonym_name)) {
		SYNONYM_VALUES_LIST all_values = intermediate_result_list.getValuesOfSynonym(synonym_name);
		std::unordered_set<SYNONYM_VALUE> unique_values(all_values.begin(), all_values.end());
		SYNONYM_VALUES_LIST unique_values_list(unique_values.begin(), unique_values.end());

		attrref_values = AttrRefManager::getAttrRefValues(pkb, synonym_type, unique_values_list, attribute);
	}
	else {
		attrref_values = AttrRefManager::getAttrRefValues(pkb, synonym_type, attribute);
	}

	bool isDefaultValueType = AttrRefManager::isDefaultValueTypeForSynonymType(attrref_values[0], synonym_type);


	STRING ident_string = string_node.getString();

	clause_result_list.addColumn(synonym_name);

	for (ATTR_REF_VALUE attrref_value : attrref_values) {
		if (attrref_value.compare(ident_string) == 0) {
			if (!isDefaultValueType) {
				ATTR_REF_VALUES_LIST default_values = AttrRefManager::getDefaultValues(pkb, synonym_type, attrref_value);
				
				clause_result_list.addColumn(synonym_name);
				for (ATTR_REF_VALUE default_value : default_values) {
					ROW row;
					row.insert({ synonym_name, default_value });
					clause_result_list.addRow(row);
				}
			}
			else {
				ROW row;
				row.insert({ synonym_name, ident_string });
				clause_result_list.addRow(row);
			}
		}
	}

	if (clause_result_list.getNumRows() > 0) {
		clause_bool = true;
	}
}

void WithClause::getAttrrefSynonymResult(QueryNode attrref_node, QueryNode synonym_node, bool& clause_bool, ResultList& clause_result_list) {
	//3. attrRef + synonym		->[int] with n = c.value | with c.value = n		->the only synonym is prog_line
	
	SYNONYM_NAME synonym_name = attrref_node.getString();
	SYNONYM_TYPE synonym_type = processed_synonyms.find(synonym_name)->second.getSynonymType();
	ATTRIBUTE attribute = attrref_node.getAttr();
	ATTR_REF_VALUES_LIST attrref_values;

	// if the intermediate resultlist contains this synonym, use the values it in. Else get from PKB.
	if (intermediate_result_list.containsSynonym(synonym_name)) {
		SYNONYM_VALUES_LIST all_values = intermediate_result_list.getValuesOfSynonym(synonym_name);
		std::unordered_set<SYNONYM_VALUE> unique_values(all_values.begin(), all_values.end());
		SYNONYM_VALUES_LIST unique_values_list(unique_values.begin(), unique_values.end());

		attrref_values = AttrRefManager::getAttrRefValues(pkb, synonym_type, unique_values_list, attribute);
	}
	else {
		attrref_values = AttrRefManager::getAttrRefValues(pkb, synonym_type, attribute);
	}


	SYNONYM_NAME prog_line_synonym_name = synonym_node.getString();
	ATTR_REF_VALUES_LIST prog_line_values;

	if (intermediate_result_list.containsSynonym(prog_line_synonym_name)) {
		SYNONYM_VALUES_LIST all_values = intermediate_result_list.getValuesOfSynonym(synonym_name);
		std::unordered_set<SYNONYM_VALUE> unique_values(all_values.begin(), all_values.end());
		SYNONYM_VALUES_LIST unique_values_list(unique_values.begin(), unique_values.end());

		prog_line_values = unique_values_list;
	}
	else {
		STMT_NUM_LIST prog_line_stmtnums = pkb.getStatementNumList();
		for (int prog_line_stmtnum : prog_line_stmtnums) {
			prog_line_values.push_back(std::to_string(prog_line_stmtnum));
		}
	}

	clause_result_list.addColumn(synonym_name);
	clause_result_list.addColumn(prog_line_synonym_name);

	for (ATTR_REF_VALUE attrref_value : attrref_values) {
		for (ATTR_REF_VALUE prog_line_value : prog_line_values) {
			std::string prog_line_value_string = prog_line_value;
			if (attrref_value.compare(prog_line_value_string) == 0) {
				ROW row;
				row.insert({synonym_name, attrref_value});
				row.insert({ prog_line_synonym_name, prog_line_value_string });
				clause_result_list.addRow(row);
			}
		}
	}

	if (clause_result_list.getNumRows() > 0) {
		clause_bool = true;
	}

}

void WithClause::getAttrrefAttrrefResult(QueryNode attrref_node, QueryNode attrref_node2, bool& clause_bool, ResultList& clause_result_list) {
	// 4. attrRef + attrRef		-> [int] with a.stmt# = s.stmt# | with s.stmt# = a.stmt#
	//							-> [string] with p.procName = v.varName

	// attrref_node 1
	SYNONYM_NAME synonym_name = attrref_node.getString();
	SYNONYM_TYPE synonym_type = processed_synonyms.find(synonym_name)->second.getSynonymType();
	ATTRIBUTE attribute = attrref_node.getAttr();
	ATTR_REF_VALUES_LIST attrref_values;

	// attrref_node 2
	SYNONYM_NAME synonym_name2 = attrref_node2.getString();
	SYNONYM_TYPE synonym_type2 = processed_synonyms.find(synonym_name2)->second.getSynonymType();
	ATTRIBUTE attribute2 = attrref_node2.getAttr();

	ATTR_REF_VALUES_LIST attrref_values2;

	// if both attrref have the same synonym name and attribute (eg s1.stmt# = s1.stmt#) return true immediately and end
	if (synonym_name == synonym_name2 && attribute == attribute2) {
		clause_bool = true;
		return;
	}

	// if the intermediate resultlist contains this synonym, use the values it in. Else get from PKB.
	if (intermediate_result_list.containsSynonym(synonym_name)) {
		SYNONYM_VALUES_LIST all_values = intermediate_result_list.getValuesOfSynonym(synonym_name);
		std::unordered_set<SYNONYM_VALUE> unique_values(all_values.begin(), all_values.end());
		SYNONYM_VALUES_LIST unique_values_list(unique_values.begin(), unique_values.end());

		attrref_values = AttrRefManager::getAttrRefValues(pkb, synonym_type, unique_values_list, attribute);
	}
	else {
		attrref_values = AttrRefManager::getAttrRefValues(pkb, synonym_type, attribute);
	}

	bool isDefaultValueType1 = AttrRefManager::isDefaultValueTypeForSynonymType(attrref_values[0], synonym_type);
	

	// if the intermediate resultlist contains this synonym, use the values it in. Else get from PKB.
	if (intermediate_result_list.containsSynonym(synonym_name2)) {

		SYNONYM_VALUES_LIST all_values = intermediate_result_list.getValuesOfSynonym(synonym_name2);
		std::unordered_set<SYNONYM_VALUE> unique_values(all_values.begin(), all_values.end());
		SYNONYM_VALUES_LIST unique_values_list(unique_values.begin(), unique_values.end());

		attrref_values2 = AttrRefManager::getAttrRefValues(pkb, synonym_type2, unique_values_list, attribute2);
	}
	else {
		attrref_values2 = AttrRefManager::getAttrRefValues(pkb, synonym_type2, attribute2);
	}

	bool isDefaultValueType2 = AttrRefManager::isDefaultValueTypeForSynonymType(attrref_values2[0], synonym_type2);


	clause_result_list.addColumn(synonym_name);
	clause_result_list.addColumn(synonym_name2);

	for (ATTR_REF_VALUE attrref_value : attrref_values) {
		for (ATTR_REF_VALUE attrref_value2 : attrref_values2) {

			if (attrref_value.compare(attrref_value2) == 0) {
				if (!isDefaultValueType1 && !isDefaultValueType2) {			// with pn.varName = c.varName
					ATTR_REF_VALUES_LIST default_values1 = AttrRefManager::getDefaultValues(pkb, synonym_type, attrref_value);
					ATTR_REF_VALUES_LIST default_values2 = AttrRefManager::getDefaultValues(pkb, synonym_type2, attrref_value2);

					for (ATTR_REF_VALUE default_value1 : default_values1) {
						for (ATTR_REF_VALUE default_value2 : default_values2) {
							ROW row;
							row.insert({ synonym_name, default_value1 });
							row.insert({ synonym_name2, default_value2 });
							clause_result_list.addRow(row);
						}
					}
				}
				else if (!isDefaultValueType1) {							// with pn.varName = p.procName
					ATTR_REF_VALUES_LIST default_values1 = AttrRefManager::getDefaultValues(pkb, synonym_type, attrref_value);

					for (ATTR_REF_VALUE default_value1 : default_values1) {
						ROW row;
						row.insert({ synonym_name, default_value1 });
						row.insert({ synonym_name2, attrref_value2 });
						clause_result_list.addRow(row);
					}
				}
				else if (!isDefaultValueType2) {							// with p.procName = pn.varName
					ATTR_REF_VALUES_LIST default_values2 = AttrRefManager::getDefaultValues(pkb, synonym_type2, attrref_value2);

					for (ATTR_REF_VALUE default_value2 : default_values2) {
						ROW row;
						row.insert({ synonym_name, attrref_value });
						row.insert({ synonym_name2, default_value2 });
						clause_result_list.addRow(row);
					}
				}
				else {														// with s.stmt# = a.stmt#
					ROW row;
					row.insert({ synonym_name, attrref_value });
					row.insert({ synonym_name2, attrref_value2 });
					clause_result_list.addRow(row);
				}
			}
		}
	}

	if (clause_result_list.getNumRows() > 0) {
		clause_bool = true;
	}
}

void WithClause::getIdentIdentResult(QueryNode ident_node1, QueryNode ident_node2, bool& clause_bool, ResultList& clause_result_list) {
	// 7. IDENT + IDENT			->[string] with "main" = "woof"
	STRING ident_string1 = ident_node1.getString();
	STRING ident_string2 = ident_node2.getString();

	if (ident_string1.compare(ident_string2) == 0) {
		clause_bool = true;
	}
	else {
		clause_bool = false;
	}
}

void WithClause::getIntegerIntegerResult(QueryNode int_node1, QueryNode int_node2, bool& clause_bool, ResultList& clause_result_list) {
	// 8. INTEGER + INTEGER		-> [int] with 5 = 3
	INTEGER int_num1 = int_node1.getInteger();
	std::string int_num1_string = std::to_string(int_num1);

	INTEGER int_num2 = int_node2.getInteger();
	std::string int_num2_string = std::to_string(int_num2);

	if (int_num1 == int_num2) {
		clause_bool = true;
	}
	else {
		clause_bool = false;
	}
}

void WithClause::getIntegerSynonymResult(QueryNode int_node, QueryNode synonym_node, bool& clause_bool, ResultList& clause_result_list) {
	// 9. INTEGER + synonym		->[int] with n = 10 | with 10 = n
	INTEGER int_num = int_node.getInteger();
	std::string int_num_string = std::to_string(int_num);

	SYNONYM_NAME prog_line_synonym_name = synonym_node.getString();
	ATTR_REF_VALUES_LIST prog_line_values;

	if (intermediate_result_list.containsSynonym(prog_line_synonym_name)) {

		SYNONYM_VALUES_LIST all_values = intermediate_result_list.getValuesOfSynonym(prog_line_synonym_name);
		std::unordered_set<SYNONYM_VALUE> unique_values(all_values.begin(), all_values.end());
		SYNONYM_VALUES_LIST unique_values_list(unique_values.begin(), unique_values.end());

		prog_line_values = unique_values_list;
	}
	else {
		STMT_NUM_LIST prog_line_stmtnums = pkb.getStatementNumList();
		for (int prog_line_stmtnum : prog_line_stmtnums) {
			prog_line_values.push_back(std::to_string(prog_line_stmtnum));
		}
	}

	SYNONYM_VALUES_LIST final_values;
	for (ATTR_REF_VALUE prog_line_value : prog_line_values) {
		std::string prog_line_value_string = prog_line_value;
		if (prog_line_value_string.compare(int_num_string) == 0) {
			final_values.push_back(prog_line_value_string);
		}
	}
	clause_result_list.addColumn(prog_line_synonym_name, final_values);
	
	if (final_values.size() > 0) {
		clause_bool = true;
	}
}

void WithClause::getSynonymSynonymResult(QueryNode synonym_node1, QueryNode synonym_node2, bool& clause_bool, ResultList& clause_result_list) {
	// 10. synonym + synonym		-> [int] with n1 = n2

	STMT_NUM_LIST prog_line_stmtnums = pkb.getStatementNumList();
	ATTR_REF_VALUES_LIST filtered_list;

	SYNONYM_NAME prog_line_synonym_name1 = synonym_node1.getString();
	ATTR_REF_VALUES_LIST prog_line_values1;

	SYNONYM_NAME prog_line_synonym_name2 = synonym_node1.getString();
	ATTR_REF_VALUES_LIST prog_line_values2;

	if (prog_line_synonym_name1 == prog_line_synonym_name2) {
		clause_bool = true;
		return;
	}

	if (intermediate_result_list.containsSynonym(prog_line_synonym_name1)) {
		SYNONYM_VALUES_LIST all_values = intermediate_result_list.getValuesOfSynonym(prog_line_synonym_name1);
		std::unordered_set<SYNONYM_VALUE> unique_values(all_values.begin(), all_values.end());
		SYNONYM_VALUES_LIST unique_values_list(unique_values.begin(), unique_values.end());

		prog_line_values1 = unique_values_list;
	}
	else {
		for (int prog_line_stmtnum : prog_line_stmtnums) {
			prog_line_values1.push_back(std::to_string(prog_line_stmtnum));
		}
	}


	if (intermediate_result_list.containsSynonym(prog_line_synonym_name2)) {
		SYNONYM_VALUES_LIST all_values = intermediate_result_list.getValuesOfSynonym(prog_line_synonym_name2);
		std::unordered_set<SYNONYM_VALUE> unique_values(all_values.begin(), all_values.end());
		SYNONYM_VALUES_LIST unique_values_list(unique_values.begin(), unique_values.end());

		prog_line_values2 = unique_values_list;
	}
	else {
		for (int prog_line_stmtnum : prog_line_stmtnums) {
			prog_line_values2.push_back(std::to_string(prog_line_stmtnum));
		}
	}
	

	// check if both prog_line values are the same
	for (ATTR_REF_VALUE progline1_value : prog_line_values1) {
		for (ATTR_REF_VALUE progline2_value : prog_line_values2) {
			if (progline1_value == progline2_value) {
				filtered_list.push_back(progline1_value);
			}
		}
	}

	clause_result_list.addColumn(prog_line_synonym_name1, filtered_list);
	clause_result_list.addColumn(prog_line_synonym_name2, filtered_list);

	if (clause_result_list.getNumRows() > 0) {
		clause_bool = true;
	}
}


bool WithClause::isValidWithClause() {
	/* For the WithClause to be valid:
	- Both lhs and rhs needs to be the same type (int or string)
	- Each side needs to be a valid value (ie int | ident | synonym| attrRef)
	*/
	bool isValid = false;

	if (!bothAreValidTypes()) {
		isValid = false;
		return isValid;
	}
	if (bothAreIntegerTypes()) {
		isValid = true;
		return isValid;
	}
	if (bothAreStringTypes()) {
		isValid = true;
		return isValid;
	}

	return isValid;
}

bool WithClause::bothAreValidTypes() {
	bool areValidTypes = false;

	if (isValidType(lhs) && isValidType(rhs)) {
		areValidTypes = true;
	}

	return areValidTypes;
}

bool WithClause::isValidType(QueryNode node) {
	bool isValidType = false;

	QueryNodeType node_type = node.getNodeType();
	if (node_type == QueryNodeType::integer || node_type == QueryNodeType::ident || node_type == QueryNodeType::synonym) {
		isValidType = true;
	}
	if (node_type == QueryNodeType::attr) {
		SYNONYM_TYPE synonym_type = processed_synonyms.find(node.getString())->second.getSynonymType();
		ATTRIBUTE attribute = node.getAttr();
		isValidType = AttrRefManager::isValidAttrRef(synonym_type, attribute);
	}

	return isValidType;
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

