#pragma once

#include <algorithm>
#include <string>
#include <vector>
#include <set>

#include "../PKB/PKB.h"
#include "QuerySynonymType.h"
#include "QueryNode.h"
#include "ResultList.h"


class AttrRefManager {
	/*
	attrRef Types:
	1. [procName]	- string	-> procedure.procName | call.procName
	2. [varName]	- string	-> variable.varName | read.varName | print.varName
	3. [value]		- int		-> constant.value
	4. [stmt#]		- int		-> s.stmt# | read.stmt# | print.stmt# | assign.stmt# | call.stmt# | while.stmt# | if.stmt#
	*/
public:
	static ATTR_REF_VALUES_LIST getAttrRefValues(PKB pkb, SYNONYM_TYPE synonym_type, ATTRIBUTE attribute);
	/*
	Description: Returns the values of the attrRef of this synonym type. Eg synonym_type.attribute -> s.stmt#
	*/
	
	static ATTR_REF_VALUES_LIST getCallsProcname(PKB pkb, SYNONYM_VALUES_LIST stmtnum_values);
	/*
	Description: Converts stmtNum of calls to the procName of calls.
	*/

	static ATTR_REF_VALUES_LIST getReadVarname(PKB pkb, SYNONYM_VALUES_LIST stmtnum_values);
	/*
	Description: Converts stmtNum of reads to the varName of reads.
	*/
	
	static ATTR_REF_VALUES_LIST getPrintVarname(PKB pkb, SYNONYM_VALUES_LIST stmtnum_values);
	/*
	Description: Converts stmtNum of prints to the varName of prints.
	*/
	
	static bool resultMatches(ResultList result_list, QueryNode synonym_node);
	/*
	Description: Returns True if the result_list's result for this synonym matches the attrRef type.
	*/
	
	static bool isValidAttrRef(SYNONYM_TYPE synonym_type, ATTRIBUTE attribute);
	/*
	Description: Checks if the attrRef is valid for the synonym type.
	*/
	
	static bool isIntegerType(ATTRIBUTE attribute);
	/*
	Description: Checks if the attrRef value is integer. (ie stmtNum, value)
	*/
	
	static bool isStringType(ATTRIBUTE attribute);
	/*
	Description: Checks if the attrRef value if string. (ie varName, procName)
	*/
	
	static bool isSameAttrValueType(ATTRIBUTE attr1, ATTRIBUTE attr2);
	/*
	Description: Checks if the value type of the ATTRIBUTES are the same. (ie String/int)
	*/
	
	static bool isSameValueType(SYNONYM_VALUE val1, SYNONYM_VALUE val2);
	/*
	Description: Checks if the string is Digit or Name
	*/
	
	static int getIndexOfDefaultValue(STRING val1, STRING val2, SYNONYM_TYPE synonym_type);
	/*
	Description: Returns 1 if val1 is the default value for this synonym type. For calls, read, print only (types with 2 attrRefs).
				 If both values are not the default value, return 1.
	*/
	
	static bool isDefaultValueTypeForSynonymType(SYNONYM_VALUE synonym_value, SYNONYM_TYPE synonym_type);
	/*
	Description: Returns true if the value given is the default value type for this synonym type.
				 Eg True if int is provided for assign, call, if, while, read, print, prog_line, stmt, constant.
					True if string is provided for procedure, variable
	*/

	static ATTR_REF_VALUES_LIST getDefaultValues(PKB pkb, SYNONYM_TYPE synonym_type, SYNONYM_VALUE value);
	/*
	Requires: synonym_type must be call, read, print only. They are the only types that have non-default values.
	Description: Returns the default values that corresponds to the value given.
	*/

private:
	static ATTR_REF_VALUES_LIST getAssignValues(PKB pkb, ATTRIBUTE attribute);

	static ATTR_REF_VALUES_LIST getCallsValues(PKB pkb, ATTRIBUTE attribute);
	
	static ATTR_REF_VALUES_LIST getConstantValues(PKB pkb, ATTRIBUTE attribute);

	static ATTR_REF_VALUES_LIST getIfsValues(PKB pkb, ATTRIBUTE attribute);
	
	static ATTR_REF_VALUES_LIST getPrintValues(PKB pkb, ATTRIBUTE attribute);
	
	static ATTR_REF_VALUES_LIST getProcedureValues(PKB pkb, ATTRIBUTE attribute);
	
	static ATTR_REF_VALUES_LIST getReadValues(PKB pkb, ATTRIBUTE attribute);
	
	static ATTR_REF_VALUES_LIST getStmtValues(PKB pkb, ATTRIBUTE attribute);
	
	static ATTR_REF_VALUES_LIST getVariableValues(PKB pkb, ATTRIBUTE attribute);
	
	static ATTR_REF_VALUES_LIST getWhilesValues(PKB pkb, ATTRIBUTE attribute);
	
	static ATTR_REF_VALUES_LIST stringifyIntList(STMT_NUM_LIST int_list);
	
	static void throwsInvalidAttrRefException();

	static bool isDigit(SYNONYM_VALUE string);

	static ATTR_REF_VALUES_LIST getUniqueValues(SYNONYM_VALUES_LIST list);

};