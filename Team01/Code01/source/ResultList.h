#pragma once

#include <vector>
#include <unordered_map>
#include <string>
#include <iterator>

class ResultList;

typedef int INTEGER;
typedef std::string SYNONYM_NAME;
typedef std::string SYNONYM_VALUE;
typedef std::vector<SYNONYM_NAME> SYNONYM_NAME_LIST;
typedef std::vector<SYNONYM_VALUE> SYNONYM_VALUES_LIST;
typedef std::vector<INTEGER> SYNONYM_INT_VALUES_LIST;
typedef std::unordered_map<SYNONYM_NAME, SYNONYM_VALUE> ROW;
typedef std::vector<ROW> ROW_LIST;
typedef std::unordered_map<SYNONYM_NAME, SYNONYM_VALUES_LIST> RESULT_LIST;

class ResultList {
private:
	int num_rows = 0;
	RESULT_LIST results;
public:
	void addColumn(SYNONYM_NAME column_name);
	void addColumn(SYNONYM_NAME column_name, SYNONYM_VALUES_LIST column_values);
	void addColumn(SYNONYM_NAME column_name, SYNONYM_INT_VALUES_LIST column_values);
	void addRow(ROW row);
	void removeColumn(SYNONYM_NAME column_name);
	void replaceColumnValues(SYNONYM_NAME column_name, SYNONYM_VALUES_LIST new_values);
	SYNONYM_NAME_LIST getAllSynonyms();
	ROW_LIST getRowList();
	SYNONYM_VALUES_LIST getValuesOfSynonym(SYNONYM_NAME column_name);
	bool containsSynonym(SYNONYM_NAME column_name);
	INTEGER getNumRows();
	INTEGER getNumColumns();
};