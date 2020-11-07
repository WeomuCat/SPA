#ifndef TESTWRAPPER_H
#define TESTWRAPPER_H

#include <string>
#include <iostream>
#include <list>
#include "../../Code01/source/PKB/PKB.h"
#include "../../Code01/source/PKB/PKBBuilder.h"


class GUIWrapper {
 public:
  // default constructor
  GUIWrapper();
  
  // destructor
  ~GUIWrapper();
  
  // method for parsing the SIMPLE source
  virtual void parse(std::string filename);
  
  // method for evaluating a query
  virtual void evaluate(std::string query, std::list<std::string>& results);

private:
	PKB pkb = PKBBuilder().build();
	bool allow_query_eval_;
};

#endif
