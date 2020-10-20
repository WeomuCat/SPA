#pragma once

#include <algorithm>
#include <string>
#include <vector>

#include "PKB.h"
#include "QuerySynonymType.h"
#include "QueryNode.h"
#include "ResultList.h"

typedef std::string ATTR_REF_VALUE;
typedef std::vector<std::string> ATTR_REF_VALUES_LIST;

class AttrRefManager {
	/*
	attrRef Types:
	1. [procName]	- string	-> procedure.procName | call.procName
	2. [varName]	- string	-> variable.varName | read.varName | print.varName
	3. [value]		- int		-> constant.value
	4. [stmt#]		- int		-> s.stmt# | read.stmt# | print.stmt# | assign.stmt# | call.stmt# | while.stmt# | if.stmt#
	*/
public:
	/*
	Description: Returns the values of the attrRef of this synonym type. Eg synonym_type.attribute -> s.stmt#
	*/
	static ATTR_REF_VALUES_LIST getAttrRefValues(PKB pkb, SYNONYM_TYPE synonym_type, ATTRIBUTE attribute) {
		ATTR_REF_VALUES_LIST values;

		switch (synonym_type) {
		case QuerySynonymType::assign:
			values = getAssignValues(pkb, attribute);
			break;
		case QuerySynonymType::call:
			values = getCallsValues(pkb, attribute);
			break;
		case QuerySynonymType::constant:
			values = getConstantValues(pkb, attribute);
			break;
		case QuerySynonymType::ifs:
			values = getIfsValues(pkb, attribute);
			break;
		case QuerySynonymType::print:
			values = getPrintValues(pkb, attribute);
			break;
		case QuerySynonymType::procedure:
			values = getProcedureValues(pkb, attribute);
			break;
		case QuerySynonymType::prog_line:
			throwsInvalidAttrRefException();
		case QuerySynonymType::read:
			values = getReadValues(pkb, attribute);
			break;
		case QuerySynonymType::stmt:
			values = getStmtValues(pkb, attribute);
			break;
		case QuerySynonymType::variable:
			values = getVariableValues(pkb, attribute);
			break;
		case QuerySynonymType::whiles:
			values = getWhilesValues(pkb, attribute);
			break;
		}

		return values;
	}

	/*
	Description: Converts stmtNum of calls to the procName of calls.
	*/
	static ATTR_REF_VALUES_LIST getCallsProcname(PKB pkb, SYNONYM_VALUES_LIST stmtnum_values) {
		ATTR_REF_VALUES_LIST call_procname_list;
		CALL_NODE_PTR_LIST call_nodes = pkb.getCalls();
		
		for (SYNONYM_VALUE stmtnum_string : stmtnum_values) {
			for (std::shared_ptr<CallNode> call_node : call_nodes) {
				STMT_NUM stmt_num = call_node->getStatementNumber();
				if (stmtnum_string == std::to_string(stmt_num)) {
					PROC_NAME procname = call_node->getCalleeProcedureName();
					call_procname_list.push_back(procname);
				}
			}
		}

		
		return call_procname_list;
	}

	/*
	Description: Converts stmtNum of reads to the varName of reads.
	*/
	static ATTR_REF_VALUES_LIST getReadVarname(PKB pkb, SYNONYM_VALUES_LIST stmtnum_values) {
		ATTR_REF_VALUES_LIST read_varname_list;
		READ_NODE_PTR_LIST read_nodes = pkb.getReads();

		for (SYNONYM_VALUE stmtnum_string : stmtnum_values) {
			for (std::shared_ptr<ReadNode> read_node : read_nodes) {
				STMT_NUM stmt_num = read_node->getStatementNumber();
				if (stmtnum_string == std::to_string(stmt_num)) {
					std::shared_ptr<VariableNode> var_node = read_node->getVariableNode();
					VAR_NAME varname = var_node->getVariableName();
					read_varname_list.push_back(varname);
				}
			}
		}
		

		return read_varname_list;
	}

	/*
	Description: Converts stmtNum of prints to the varName of prints.
	*/
	static ATTR_REF_VALUES_LIST getPrintVarname(PKB pkb, SYNONYM_VALUES_LIST stmtnum_values) {
		ATTR_REF_VALUES_LIST print_varname_list;
		PRINT_NODE_PTR_LIST print_nodes = pkb.getPrints();

		for (SYNONYM_VALUE stmtnum_string : stmtnum_values) {
			for (std::shared_ptr<PrintNode> print_node : print_nodes) {
				STMT_NUM stmt_num = print_node->getStatementNumber();
				if (stmtnum_string == std::to_string(stmt_num)) {
					std::shared_ptr<VariableNode> var_node = print_node->getVariableNode();
					VAR_NAME varname = var_node->getVariableName();
					print_varname_list.push_back(varname);
				}
			}
		}


		return print_varname_list;
	}

	/*
	Description: Returns True if the result_list's result for this synonym matches the attrRef type.
	*/
	static bool resultMatches(ResultList result_list, QueryNode synonym_node) {
		bool result_matches = false;
		SYNONYM_NAME synonym_name = synonym_node.getString();
		ATTRIBUTE attribute_type = synonym_node.getAttr();
		SYNONYM_VALUES_LIST values_list = result_list.getValuesOfSynonym(synonym_name);
		SYNONYM_VALUE one_value = values_list[0];

		if (attribute_type == AttributeType::stmtNum || attribute_type == AttributeType::value) {
			if (isDigit(one_value)) {
				result_matches = true;
			}
		}

		if (attribute_type == AttributeType::procName || attribute_type == AttributeType::varName) {
			if (!isDigit(one_value)) {
				result_matches = true;
			}
		}

		return result_matches;
	}

	/*
	Description: Checks if the attrRef is valid for the synonym type.
	*/
	static bool isValidAttrRef(SYNONYM_TYPE synonym_type, ATTRIBUTE attribute) {
		bool isValid = false;

		switch (synonym_type) {
		case QuerySynonymType::assign:
			if (attribute == AttributeType::stmtNum) {
				isValid = true;
			}
			break;
		case QuerySynonymType::call:
			if (attribute == AttributeType::stmtNum || attribute == AttributeType::procName) {
				isValid = true;
			}
			break;
		case QuerySynonymType::constant:
			if (attribute == AttributeType::value) {
				isValid = true;
			}
			break;
		case QuerySynonymType::ifs:
			if (attribute == AttributeType::stmtNum) {
				isValid = true;
			}
			break;
		case QuerySynonymType::print:
			if (attribute == AttributeType::stmtNum || attribute == AttributeType::varName) {
				isValid = true;
			}
			break;
		case QuerySynonymType::procedure:
			if (attribute == AttributeType::procName) {
				isValid = true;
			}
			break;
		case QuerySynonymType::prog_line:
			break;
		case QuerySynonymType::read:
			if (attribute == AttributeType::stmtNum || attribute == AttributeType::varName) {
				isValid = true;
			}
			break;
		case QuerySynonymType::stmt:
			if (attribute == AttributeType::stmtNum) {
				isValid = true;
			}
			break;
		case QuerySynonymType::variable:
			if (attribute == AttributeType::varName) {
				isValid = true;
			}
			break;
		case QuerySynonymType::whiles:
			if (attribute == AttributeType::stmtNum) {
				isValid = true;
			}
			break;
		}

		return isValid;
	}

	/*
	Description: Checks if the attrRef value is integer. (ie stmtNum, value)
	*/
	static bool isIntegerType(ATTRIBUTE attribute) {
		bool isInteger = false;
		if (attribute == AttributeType::stmtNum || attribute == AttributeType::value) {
			isInteger = true;
		}
		return isInteger;
	}

	/*
	Description: Checks if the attrRef value if string. (ie varName, procName)
	*/
	static bool isStringType(ATTRIBUTE attribute) {
		bool isString = false;
		if (attribute == AttributeType::varName || attribute == AttributeType::procName) {
			isString = true;
		}
		return isString;
	}


private:
	static ATTR_REF_VALUES_LIST getAssignValues(PKB pkb, ATTRIBUTE attribute) {
		if (attribute == AttributeType::stmtNum) {
			return stringifyIntList(pkb.getAssignNumList());
		}
		else {
			throwsInvalidAttrRefException();
		}
	}

	static ATTR_REF_VALUES_LIST getCallsValues(PKB pkb, ATTRIBUTE attribute) {
		if (attribute == AttributeType::stmtNum) {
			return stringifyIntList(pkb.getCallNumList());
		}
		else if (attribute == AttributeType::procName) {
			return pkb.getCallProcNameList();
		}
		else {
			throwsInvalidAttrRefException();
		}
	}

	static ATTR_REF_VALUES_LIST getConstantValues(PKB pkb, ATTRIBUTE attribute) {
		if (attribute == AttributeType::value) {
			return pkb.getConstantValueList();
		}
		else {
			throwsInvalidAttrRefException();
		}
	}

	static ATTR_REF_VALUES_LIST getIfsValues(PKB pkb, ATTRIBUTE attribute) {
		if (attribute == AttributeType::stmtNum) {
			return stringifyIntList(pkb.getIfNumList());
		}
		else {
			throwsInvalidAttrRefException();
		}
	}

	static ATTR_REF_VALUES_LIST getPrintValues(PKB pkb, ATTRIBUTE attribute) {
		if (attribute == AttributeType::stmtNum) {
			return stringifyIntList(pkb.getPrintNumList());
		}
		else if (attribute == AttributeType::varName) {
			return pkb.getPrintVarNameList();
		}
		else {
			throwsInvalidAttrRefException();
		}
	}

	static ATTR_REF_VALUES_LIST getProcedureValues(PKB pkb, ATTRIBUTE attribute) {
		if (attribute == AttributeType::procName) {
			return pkb.getProcedureNameList();
		}
		else {
			throwsInvalidAttrRefException();
		}
	}

	static ATTR_REF_VALUES_LIST getReadValues(PKB pkb, ATTRIBUTE attribute) {
		if (attribute == AttributeType::stmtNum) {
			return stringifyIntList(pkb.getReadNumList());
		}
		else if (attribute == AttributeType::varName) {
			return pkb.getReadVarNameList();
		}
		else {
			throwsInvalidAttrRefException();
		}
	}

	static ATTR_REF_VALUES_LIST getStmtValues(PKB pkb, ATTRIBUTE attribute) {
		if (attribute == AttributeType::stmtNum) {
			return stringifyIntList(pkb.getStatementNumList());
		}
		else {
			throwsInvalidAttrRefException();
		}
	}

	static ATTR_REF_VALUES_LIST getVariableValues(PKB pkb, ATTRIBUTE attribute) {
		if (attribute == AttributeType::varName) {
			return pkb.getVariableNameList();
		}
		else {
			throwsInvalidAttrRefException();
		}
	}

	static ATTR_REF_VALUES_LIST getWhilesValues(PKB pkb, ATTRIBUTE attribute) {
		if (attribute == AttributeType::stmtNum) {
			return stringifyIntList(pkb.getWhileNumList());
		}
		else {
			throwsInvalidAttrRefException();
		}
	}



	static ATTR_REF_VALUES_LIST stringifyIntList(STMT_NUM_LIST int_list) {
		ATTR_REF_VALUES_LIST string_values;
		for (int i : int_list) {
			string_values.push_back(std::to_string(i));
		}

		return string_values;
	}

	static void throwsInvalidAttrRefException() {
		throw "AttrRefManager: Attribute is invalid for this given synonym type.";
	}

	static bool isDigit(SYNONYM_VALUE string) {
		return std::isdigit(*string.begin());
	}
};