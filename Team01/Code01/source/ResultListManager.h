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
	static ResultList merge(ResultList list1, ResultList list2, PROCESSED_SYNONYMS processed_synonyms, PKB pkb) {
		replaceAllAttrRefWithDefaultValue(list1, processed_synonyms, pkb);
		replaceAllAttrRefWithDefaultValue(list2, processed_synonyms, pkb);

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

		if (list1.hasNoCols()) {
			return list2;
		}
		if (list2.hasNoCols()) {
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

	/*
	Description: Replace all values of attrRef columns with their default value type.
	Input: ResultList - { call, {main, woof }}
	Output: ResultList - { call, {2, 3}}	where 2, 3 corresponds to the stmtNum of main, woof
	*/
	static void replaceAllAttrRefWithDefaultValue(ResultList& list, PROCESSED_SYNONYMS processed_synonyms, PKB pkb) {
		// check if there a column in the list that is not the default value of that synonym type (ie calls column storing string)
		if (!list.hasNoRows()) {
			SYNONYM_NAME_LIST all_synonym_names = list.getAllSynonyms();
			for (SYNONYM_NAME synonym_name : all_synonym_names) {
				QueryNode synonym_node = processed_synonyms.find(synonym_name)->second;

				SYNONYM_VALUES_LIST synonym_values = list.getValuesOfSynonym(synonym_name);
				SYNONYM_VALUE one_synonym_value = synonym_values[0];
				SYNONYM_TYPE synonym_type = synonym_node.getSynonymType();

				// If the synonym value is not the default type (string/int) of this synonym type, replace the result list value with the default value
				if (!AttrRefManager::isDefaultValueTypeForSynonymType(one_synonym_value, synonym_type)) {
					SYNONYM_VALUES_LIST new_values;

					// If the synonym type is call, replace varName with stmtNum
					if (synonym_type == QuerySynonymType::call) {
						CALL_NODE_PTR_LIST call_nodes = pkb.getCalls();
						for (SYNONYM_VALUE synonym_value : synonym_values) {
							for (CALL_NODE_PTR call_node : call_nodes) {
								if (call_node->getCalleeProcedureName().compare(synonym_value) == 0) {
									STMT_NUM new_value = call_node->getStatementNumber();
									new_values.push_back(std::to_string(new_value));
								}
							}
						}
					}

					// If the synonym type is read, replace varName with stmtNum
					if (synonym_type == QuerySynonymType::read) {
						READ_NODE_PTR_LIST read_nodes = pkb.getReads();
						for (SYNONYM_VALUE synonym_value : synonym_values) {
							for (READ_NODE_PTR read_node : read_nodes) {
								VAR_NODE_PTR var_node = read_node->getVariableNode();
								VAR_NAME variable_name = var_node->getVariableName();
								if (variable_name.compare(synonym_value) == 0) {
									STMT_NUM new_value = read_node->getStatementNumber();
									new_values.push_back(std::to_string(new_value));
								}
							}
						}
					}

					// If the synonym type is print, replace varName with stmtNum
					if (synonym_type == QuerySynonymType::print) {
						PRINT_NODE_PTR_LIST print_nodes = pkb.getPrints();
						for (SYNONYM_VALUE synonym_value : synonym_values) {
							for (PRINT_NODE_PTR print_node : print_nodes) {
								if (print_node->getVariableNode()->getVariableName().compare(synonym_value) == 0) {
									STMT_NUM new_value = print_node->getStatementNumber();
									new_values.push_back(std::to_string(new_value));
								}
							}
						}
					}
					
					
					list.replaceColumnValues(synonym_name, new_values);
				}
				
			}
		}
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
			SYNONYM_VALUE r1_value = r1[n];
			SYNONYM_VALUE r2_value = r2[n];

			
			if (r1[n] != r2[n]) {
				return false;
			}
			
		}
		return true;
	}

	/*
	Description: Returns the final result string from this list of values.
	*/
	static STRING_RESULT processReturnResult(SYNONYM_VALUES_LIST raw_results) {
		std::string processed_results = "";
		for (std::string result : raw_results) {
			processed_results = processed_results + result + ", ";
		}
		processed_results = processed_results.substr(0, processed_results.size() - 2);
		return processed_results;
	};

	/*
	Description: Converts an int list to a string list.
	*/
	static SYNONYM_VALUES_LIST convertIntToStringValues(STMT_NUM_LIST int_list) {
		SYNONYM_VALUES_LIST string_list;
		for (int i : int_list) {
			string_list.push_back(std::to_string(i));
		}
		return string_list;
	}
};