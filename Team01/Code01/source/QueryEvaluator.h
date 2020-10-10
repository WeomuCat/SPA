#pragma once

#include "PKB.h"
#include "Pattern.h"
#include "QueryEvaluatorReturnType.h"
#include "QueryNode.h"
#include "QueryNodeType.h"
#include "QuerySynonymType.h"
#include "Relationship.h"
#include "ResultList.h"
#include "ResultListManager.h"
#include "WithClause.h"

#include <string>
#include <unordered_map>
#include <vector>

class QueryEvaluator;

typedef std::vector<std::string> QUERY_RESULT;
typedef std::unordered_map<std::string, QueryNode> PROCESSED_SYNONYMS;
typedef QueryNode PROCESSED_CLAUSES;
typedef std::string RETURN_SYNONYMS; //ITER 2: change this to VECTOR

class QueryEvaluator {
	/* Overview: Evaluates pre-processed queries and returns the corresponding result */

public:
	QueryEvaluator(PKB pkb);
	/*
	Description: Creates a QueryEvaluator for the given PKB.
	*/

	QUERY_RESULT evaluateQuery(PROCESSED_SYNONYMS synonyms, PROCESSED_CLAUSES clauses);
	/*
	Description: Returns the query result of the given processed synonyms and processed clauses.
				 Invalid queries would return an empty result list.
	*/

private:
	PKB pkb = (PKBBuilder().build()); // because PKB has no default constructor
	RETURN_SYNONYMS return_synonym_names;
	QueryEvaluatorReturnType return_type;

	void QueryEvaluator::fillWithReturnSynonym(QuerySynonymType return_synonym_type, SYNONYM_NAME return_synonym_name, ResultList& result_list);
	/*
	Description: Fills the given result_list with all values of the given synonym_type, attached to the column name of the given synonym_name.
	*/
	
	void setEvaluatorReturnType(QueryNode select_return);
};