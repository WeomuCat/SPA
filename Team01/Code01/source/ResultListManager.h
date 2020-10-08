#pragma once

#include "ResultList.h"

#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <string>
#include <iterator>

class ResultListManager;

typedef std::string STRING_RESULT;
typedef std::vector<std::string> VALUE_LIST;

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