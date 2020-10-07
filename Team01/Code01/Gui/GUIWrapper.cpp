#include <iostream>

#include "GUIWrapper.h"
#include "../source/Parser.h"
#include "../source/PKB/DesignEntities/AssignTable.cpp"
#include "../source/PKB/DesignEntities/ConstantTable.cpp"
#include "../source/PKB/DesignEntities/IfTable.cpp"
#include "../source/PKB/DesignEntities/PrintTable.cpp"
#include "../source/PKB/DesignEntities/ProcedureTable.cpp"
#include "../source/PKB/DesignEntities/ReadTable.cpp"
#include "../source/PKB/DesignEntities/StatementTable.cpp"
#include "../source/PKB/DesignEntities/VariableTable.cpp"
#include "../source/PKB/DesignEntities/WhileTable.cpp"
#include "../source/PKB/Relationships/FollowsTable.cpp"
#include "../source/PKB/Relationships/ParentTable.cpp"
#include "../source/PKB/Relationships/UsesTable.cpp"
#include "../source/PKB/Relationships/ModifiesTable.cpp"
#include "../source/QueryNodeContent.cpp"
#include "../source/QueryNode.cpp"
#include "../source/QueryProcessor.cpp"
#include "../source/QueryEvaluator.cpp"
#include "../source/ResultList.cpp"
#include "../source/ResultListManager.h"
#include "../source/PKB.cpp"
#include "../source/PKBBuilder.cpp"
#include "../source/PKB/ASTNode/VariableNode.cpp"


// a default constructor
GUIWrapper::GUIWrapper() {
  // create any objects here as instance variables of this class
  // as well as any initialization required for your spa program
	this->pkb = PKBBuilder().build();
	this->allow_query_eval_ = false;
}

// method for parsing the SIMPLE source
void GUIWrapper::parse(std::string filename) {
	// call your parser to do the parsing
	Parser parser = Parser();
	
	try {
		
		this->pkb = parser.parseFile(filename);
		this->allow_query_eval_ = true;

	}
	catch (const char* msg) {
		std::cout << msg << "\n";
		this->allow_query_eval_ = false;
	}
	// ...rest of your code...
}

// method to evaluating a query
void GUIWrapper::evaluate(std::string query, std::list<std::string>& results){
// call your evaluator to evaluate the query here
  // ...code to evaluate query...


	QueryProcessor qp = QueryProcessor(this->pkb);
	QUERY_RESULT query_result;
	STRING_RESULT result_string;
	if (this->allow_query_eval_) {
		try {
			query_result = qp.processQuery(query);
			result_string = ResultListManager::getStringValues(query_result);
		}
		catch (const char* msg) {
			std::cout << msg << "\n";
		}

		// store the answers to the query in the results list (it is initially empty)
		// each result must be a string.
		for (std::string query : query_result) {
			results.push_back(query);
		}
	}
	else {
		results.push_back("Invalid SIMPLE source code encountered during parsing. Query Evaluation not allowed.");
	}
}
