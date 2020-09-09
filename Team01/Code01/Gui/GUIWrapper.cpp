#include "GUIWrapper.h"
#include "../source/QueryNode.cpp"
#include "../source/QueryProcessor.cpp"
#include <iostream>



// a default constructor
GUIWrapper::GUIWrapper() {
  // create any objects here as instance variables of this class
  // as well as any initialization required for your spa program
}

// method for parsing the SIMPLE source
void GUIWrapper::parse(std::string filename) {
	// call your parser to do the parsing
	std::cout << "parsed " << filename << std::endl;
  // ...rest of your code...
}

// method to evaluating a query
void GUIWrapper::evaluate(std::string query, std::list<std::string>& results){
// call your evaluator to evaluate the query here
  // ...code to evaluate query...
	QueryProcessor qp = QueryProcessor();
	QueryNode qn = QueryNode();
	QUERY_RESULT query_result = qp.processQuery(query);


	std::cout << "query=  " << query << std::endl;
	std::cout << "query result= " << query_result << std::endl;
	results.push_back(query + "\nQuery result: ");
  // store the answers to the query in the results list (it is initially empty)
  // each result must be a string.
	results.push_back(query_result);
}
