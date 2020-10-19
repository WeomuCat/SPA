#pragma once

#include <algorithm>
#include <string>
#include <vector>

#include "PKB.h"
#include "QuerySynonymType.h"
#include "QueryNode.h"
#include "ResultList.h"

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
	Description: Returns the values of the attrRef of this synonym type.
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
	Description: Returns the procName of calls, given the stmtNum of calls.
	*/
	static ATTR_REF_VALUES_LIST getCallsProcname(PKB pkb, SYNONYM_VALUES_LIST stmtnum_values) {
		ATTR_REF_VALUES_LIST call_procname_list;
		CALL_NODE_PTR_LIST call_nodes = pkb.getCalls();
		
		for (std::shared_ptr<CallNode> call_node : call_nodes) {
			STMT_NUM stmt_num = call_node->getStatementNumber();
			if (std::find(stmtnum_values.begin(), stmtnum_values.end(), std::to_string(stmt_num)) != stmtnum_values.end()) {
				PROC_NAME procname = call_node->getCalleeProcedureName();
				call_procname_list.push_back(procname);
			}
		}
		
		return call_procname_list;
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
};