#pragma once

#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <string>
#include <iterator>

#include "AttrRefManager.h"
#include "ResultList.h"
#include "QueryNode.h"

class ResultListManager {
public:
	
	static ResultList addSynonymAndValues(ResultList* result_list_ptr, SYNONYM_NAME synonym_name, SYNONYM_VALUES_LIST synonym_values);
	/*
	Description: Adds the given synonym name and synonym values to this result list.
				 ie add {synonym_name, synonym_value}
	*/

	static VALUE_LIST getSynonymValues(ResultList result_list, SYNONYM_NAME target_synonym);
	/*
	Description: Returns all values of this synonym name in this result list.
	*/

	static VALUE_LIST getTupleValues(ResultList result_list, TUPLE_RETURN_SYNONYMS target_synonyms);
	/*
	Description: Returns a list of tuple values computed from the given resul list.
	*/

	static STRING_RESULT getStringValues(VALUE_LIST result_list);
	/*
	Description: Returns the final string result from the given list of values.
	*/

	static bool containsSynonym(ResultList result_list, SYNONYM_NAME synonym_name);
	/*
	Description: Returns True if the result list contains this synonym name.
	*/

	// More for debugging
	static void printResultListValues(RESULT_LIST result_list);
	
	static ResultList merge(ResultList list1, ResultList list2, PROCESSED_SYNONYMS processed_synonyms, PKB pkb);
	/*
	Description: Safe Merge for QueryEvaluator. Accounts for attrRef conflict in result lists.
				 Eg Handles { call, {1, 2, 3}} and { call, {main, woof }}
	*/
	
	static ResultList merge(ResultList list1, ResultList list2);
	/*
	Description: Unsafe Merge, do not use in QueryEvaluator. Does not account for attrRef conflict in result lists.
	*/
	
	static void replaceAllAttrRefWithDefaultValue(ResultList& list, PROCESSED_SYNONYMS processed_synonyms, PKB pkb);
	/*
	Description: Replace all values of attrRef columns with their default value type.
	Input: ResultList - { call, {main, woof }}
	Output: ResultList - { call, {2, 3}}	where 2, 3 corresponds to the stmtNum of main, woof
	*/



private:
	static SYNONYM_NAME_LIST getCommonSynonyms(ResultList list1, ResultList list2);

	static ROW joinRows(ROW r1, ROW r2);

	static bool sameValuesForTheSameCommonSynonyms(ROW r1, ROW r2, SYNONYM_NAME_LIST common_synonyms);

	static STRING_RESULT processReturnResult(SYNONYM_VALUES_LIST raw_results);
	/*
	Description: Returns the final result string from this list of values.
	*/

	static SYNONYM_VALUES_LIST convertIntToStringValues(STMT_NUM_LIST int_list);
	/*
	Description: Converts an int list to a string list.
	*/
	
};