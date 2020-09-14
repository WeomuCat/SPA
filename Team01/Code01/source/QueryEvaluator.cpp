#include "QueryEvaluator.h"

#include <string>
#include <vector>

QueryEvaluator::QueryEvaluator(PKB pkb) {
	this->pkb = pkb;
}

QUERY_RESULT QueryEvaluator::evaluateQuery(PROCESSED_SYNONYMS synonyms, PROCESSED_CLAUSES clauses) {
	std::vector<std::string> result_list;
	result_list.push_back("yo yo im here"); //initialise to empty_result
	QueryNode chosen_return_type;
	
	QUERY_NODE_POINTERS children = clauses.getChildren();
	if (clauses.getNodeType() == QueryNodeType::select) { 
		result_list[0] = result_list[0] + "\n" + "Select Worked";
		chosen_return_type = children[0];
		result_list[0] = result_list[0] + "\n" + "chosen return synonym name: " + chosen_return_type.getSynonymName();
	}

	// Hard coded result from PKBStub
	VAR_NODE_PTR_LIST all_variables = pkb.getVariables();
	std::string variable_string = "";
	for (VAR_NODE_PTR variable : all_variables) {
		VariableNode variable_node = *variable;
		variable_string += variable_node.getVariableName();
	}
	result_list[0] = result_list[0] + "\n" + variable_string;

	
	//intermediate result_list can have many synonyms
	//but final result should return just 1 string/empty
	
	return result_list[0]; 
}

// to Iterate through Map of string:vector<string> pair
/*
int main() {
  std::vector<int> v1 = {1,2,3};
  std::vector<int> v2 = {5,4,3};
  //std::cout << v1.size() << "\n";

  std::map<int, std::vector<int>> m1;
  m1.insert(std::pair(0,v1));
  m1.insert(std::pair(1,v2));
  m1.insert(std::pair(2,v1));
  m1.insert(std::pair(3,v2));

  std::map<int, std::vector<int>>::iterator iter = m1.begin();
  std::cout << "initial map: \n";
  std::cout << "initial size: " << m1.size() << "";
  //print out values in map
  iter = m1.begin();
  while (iter != m1.end()) {
	std::pair<int, std::vector<int>> iter_value = *iter;
	std::cout << "\nkey: " << iter_value.first;
	std::cout << "\nvalue: " << iter_value.second[0];
	++iter;
  }

  iter = m1.begin();
  while(iter != m1.end()) {
	std::pair<int, std::vector<int>> iter_value = *iter;
	//std::cout << "key: " << iter_value.first;
	//std::cout << "value: " << iter_value.second;
	if (ShouldDelete(iter_value)) {
	  iter = m1.erase(iter);
	} else {
	  ++iter;
	}
  }

  std::cout << "\nend map";
  //print out values in map
  iter = m1.begin();
  while (iter != m1.end()) {
	std::pair<int, std::vector<int>> iter_value = *iter;
	std::cout << "\nkey: " << iter_value.first;
	std::cout << "\nvalue: " << iter_value.second[0];
	++iter;
  }

  std::string output = ResultListManager::Filter("h", "i");
  std::cout << "ResultListManager: " << output;
}

bool ShouldDelete(std::pair<int, std::vector<int>> iter_value) {
  std::vector<int> values = iter_value.second;
  if (std::find(values.begin(), values.end(), 2) != values.end()) {
	return true;
  } else {
	return false;
  }
}
*/