#pragma once

#include "ResultList.h"

#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <string>
#include <iterator>

class ResultListManager;

typedef std::string FINAL_RESULT;

class ResultListManager {
// Note to self: for methods that change result_list, return ResultList!
public:
	static std::string filter(std::string list1, std::string list2);
	
	static ResultList addSynonymAndValues(ResultList *result_list_ptr, SYNONYM_NAME synonym_name, SYNONYM_VALUES_LIST synonym_values) {
		ResultList result_list = *result_list_ptr;
		result_list.addColumn(synonym_name, synonym_values);
		return result_list;
	};

	static FINAL_RESULT getValues(ResultList result_list, SYNONYM_NAME synonym_name) {
		// Return all the values of this synonym_name
		SYNONYM_VALUES_LIST raw_result = result_list.getValuesOfSynonym(synonym_name);
		return processReturnResult(raw_result);
	}
	
	static bool containsSynonym(ResultList result_list, SYNONYM_NAME synonym_name) {
		return result_list.containsSynonym(synonym_name);
	}

private:
	static FINAL_RESULT processReturnResult(SYNONYM_VALUES_LIST raw_results) {
		std::string processed_results = "";
		for (std::string result : raw_results) {
			processed_results = processed_results + result + ", ";
		}
		processed_results = processed_results.substr(0, processed_results.size() - 2);
		return processed_results;
	};
};