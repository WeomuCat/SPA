#pragma once

#include "AttrRefManager.h"
#include "ResultList.h"
#include "QueryNode.h"

#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <string>
#include <iterator>

class ResultListManager;

typedef std::string STRING_RESULT;
typedef std::vector<std::string> VALUE_LIST;
typedef std::vector<SYNONYM_NAME> TUPLE_RETURN_SYNONYMS;
typedef std::unordered_map<std::string, QueryNode> PROCESSED_SYNONYMS;

class ResultListManager {
// Note to self: for methods that change result_list, return ResultList!
public:
	
	static ResultList addSynonymAndValues(ResultList *result_list_ptr, SYNONYM_NAME synonym_name, SYNONYM_VALUES_LIST synonym_values) {
		ResultList result_list = *result_list_ptr;
		result_list.addColumn(synonym_name, synonym_values);
		return result_list;
	};

	static VALUE_LIST getSynonymValues(ResultList result_list, SYNONYM_NAME target_synonym) {
		return result_list.getValuesOfSynonym(target_synonym);
	}
	
	static VALUE_LIST getTupleValues(ResultList result_list, TUPLE_RETURN_SYNONYMS target_synonyms) {
		VALUE_LIST final_results;
		std::vector<SYNONYM_VALUES_LIST> target_synonym_list;

		for (SYNONYM_NAME target_synonym : target_synonyms) {
			target_synonym_list.push_back(result_list.getValuesOfSynonym(target_synonym));
		}

		for (int i = 0; i < result_list.getNumRows(); i++) {
			std::string row_result = "";
			for (int j = 0; j < target_synonym_list.size(); j++) {
				std::string synonym_value = target_synonym_list[j][i];
				row_result = row_result + synonym_value + " ";
			}
			if (row_result.size() > 0) {
				row_result.pop_back(); // remove extra whitespace behind
			}
			final_results.push_back(row_result);
		}

		return final_results;
	}

	static STRING_RESULT getStringValues(VALUE_LIST result_list) {
		// Return all the values of this synonym_name
		return processReturnResult(result_list);
	}
	
	static bool containsSynonym(ResultList result_list, SYNONYM_NAME synonym_name) {
		return result_list.containsSynonym(synonym_name);
	}

	// More for debugging
	static void printResultListValues(RESULT_LIST result_list) {
		auto iter = result_list.begin();

		std::cout << "Result list length: " << result_list.size();

		while (iter != result_list.end()) {
			std::pair<std::string, std::vector<std::string>> iter_value = *iter;
			std::cout << "\nkey: " << iter_value.first;
			std::vector<std::string> vector_value = iter_value.second;
			std::vector<std::string>::iterator vector_iter = vector_value.begin();
			while (vector_iter != vector_value.end()) {
				std::string synonym_string = *vector_iter;
				std::cout << "\nvalue: " << synonym_string;

				++vector_iter;
			}

			++iter;
		}
	}
	
	/*
	Description: Safe Merge for QueryEvaluator. Accounts for attrRef conflict in result lists.
				 Eg Handles { call, {1, 2, 3}} and { call, {main, woof }}
	*/
	static ResultList merge(ResultList list1, ResultList list2, PROCESSED_SYNONYMS processed_synonyms) {
		dropSameSynonymDifferentValueColumn(list1, list2, processed_synonyms);
		return merge(list1, list2);
	}

	/*
	Description: Unsafe Merge, do not use in QueryEvaluator. Does not account for attrRef conflict in result lists.
	*/
	static ResultList merge(ResultList list1, ResultList list2) {
		// If either list is empty (no columns), return the other list
		// Note: if there is at least 1 column, but no rows for the column, it is not considered empty.
		ROW_LIST rows1 = list1.getRowList();
		ROW_LIST rows2 = list2.getRowList();
		if (rows1.size() == 0) {
			return list2;
		}
		if (rows2.size() == 0) {
			return list1;
		}

		// Add columns of result
		ResultList result;
		for (SYNONYM_NAME n1 : list1.getAllSynonyms()) {
			result.addColumn(n1);
		}
		for (SYNONYM_NAME n2 : list2.getAllSynonyms()) {
			result.addColumn(n2);
		}


		// Get Common Synonyms
		SYNONYM_NAME_LIST common_synonyms = getCommonSynonyms(list1, list2);

		// Permutate all common values
		for (auto r1 : rows1) {
			for (auto r2 : rows2) {
				// If the two rows do not share the same values for all the same common synonyms,
				// filter out these two rows from the final result! :)
				if (common_synonyms.size() > 0 && !sameValuesForTheSameCommonSynonyms(r1, r2, common_synonyms)) {
					continue;
				}

				result.addRow(joinRows(r1, r2));
			}
		}
		return result;
	}


private:

	static void dropSameSynonymDifferentValueColumn(ResultList& list1, ResultList& list2, PROCESSED_SYNONYMS processed_synonyms) {
		for (SYNONYM_NAME n1 : list1.getAllSynonyms()) {
			for (SYNONYM_NAME n2 : list2.getAllSynonyms()) {
				SYNONYM_VALUE n1_val = list1.getValuesOfSynonym(n1)[0];
				SYNONYM_VALUE n2_val = list2.getValuesOfSynonym(n2)[0];

				// if same synonym name but different value type	-> ie { call, {1, 2, 3}} and { call, {main, woof }}
				if (n1 == n2 && !AttrRefManager::isSameValueType(n1_val, n2_val)) {
					SYNONYM_TYPE synonym_type = processed_synonyms.find(n1)->second.getSynonymType();

					int index = AttrRefManager::getIndexOfDefaultValue(n1_val, n2_val, synonym_type);

					if (index == 1) {
						list2.removeColumn(n2);
					}
					else {
						list1.removeColumn(n1);
					}
				}
			}
		}
	}

	static SYNONYM_NAME_LIST getCommonSynonyms(ResultList list1, ResultList list2) {
		SYNONYM_NAME_LIST common_synonyms;
		for (SYNONYM_NAME n1 : list1.getAllSynonyms()) {
			for (SYNONYM_NAME n2 : list2.getAllSynonyms()) {

				if (n1 == n2) {
					common_synonyms.push_back(n1);
				}

			}
		}
		return common_synonyms;
	}

	static ROW joinRows(ROW r1, ROW r2) {
		ROW joined_row;
		for (ROW::iterator it = r1.begin(); it != r1.end(); it++) {
			joined_row.insert(*it);
		}
		for (ROW::iterator it2 = r2.begin(); it2 != r2.end(); it2++) {
			joined_row.insert(*it2);
		}
		return joined_row;
	}

	static bool sameValuesForTheSameCommonSynonyms(ROW r1, ROW r2, SYNONYM_NAME_LIST common_synonyms) {
		for (SYNONYM_NAME n : common_synonyms) {
			SYNONYM_VALUE r1_value = r1[n];
			SYNONYM_VALUE r2_value = r2[n];

			/*
			if (AttrRefManager::isSameValueType(r1_value, r2_value) && r1[n] != r2[n]) {
				return false;
			}
			*/
			
			if (r1[n] != r2[n]) {
				return false;
			}
			
		}
		return true;
	}

	static STRING_RESULT processReturnResult(SYNONYM_VALUES_LIST raw_results) {
		std::string processed_results = "";
		for (std::string result : raw_results) {
			processed_results = processed_results + result + ", ";
		}
		processed_results = processed_results.substr(0, processed_results.size() - 2);
		return processed_results;
	};
};