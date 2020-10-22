#pragma once

#include "AttributeType.h"
#include "AttrRefManager.h"
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
#include <queue>

class QueryEvaluator;


typedef bool BOOLEAN;

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
	PKB pkb = (PKBBuilder().build());						// because PKB has no default constructor
	PROCESSED_SYNONYMS processed_synonyms;					// stores all the synonyms from QueryPreProcessor
	ResultList result_list;									// stores the results of evaluation
	QueryNode result_clause;								// Stores the entire QueryNode BOOLEAN | Tuple
	QueryEvaluatorReturnType return_type;					// BOOLEAN | Tuple | Synonym (tuple of 1 element)
	TUPLE_RETURN_SYNONYMS tuple_return_synonyms;			// Stores all the names of synonyms in the tuple to be returned

	BOOLEAN only_true_false_clauses = true;					// True if all clauses evaluated so far is true/false clause
	BOOLEAN evaluated_clauses = false;						// True if at least 1 clause is evaluated
	const QUERY_RESULT no_result = QUERY_RESULT();			// Empty result to be returned if short circuit
	const QUERY_RESULT boolean_true_result = { "TRUE" };	// Result representing BOOLEAN = TRUE
	const QUERY_RESULT boolean_false_result = { "FALSE" };	// Result representing BOOLEAN = FALSE


	void fillWithReturnValue(QueryNode result_clause, ResultList& result_list);
	/*
	Description: Filles the given result_list with all values of the given synonym or attrRef.
	*/

	void fillWithReturnSynonym(QueryNode result_clause, ResultList& result_list);
	/*
	Description: Fills the given result_list with all values of the given synonym_type, attached to the column name of the given synonym_name.
	*/
	
	void setEvaluatorReturnType();
	/*
	Description: Sets the return type of this Evaluator based on the result_clause. The types settable are BOOLEAN, Tuple, Synonym (Tuple of 1 element).
	*/

	void setTupleReturnSynonyms();
	/*
	Description: Sets the tuple_return_synonyms to the names of synonyms in this result_clause.
	*/

	QUERY_RESULT getSemanticallyInvalidResult();
	/*
	Description: Returns the default result for semantically invalid responses. Mainly used for invalid attrRef for synonyms.
	*/

	QUERY_RESULT getFinalQueryResultReturnValue(ResultList result_list, QueryNode synonym_node);
	/*
	Desription: Returns the value to be returned from this result_list. Handles obtaining synonyms values and atrrRef value conversion.
	*/

	SYNONYM_TYPE getSynonymType(SYNONYM_NAME synonym_name);
	/*
	Desription: Returns the synonym_type of this synonym_name.
	*/

	QUERY_RESULT evaluateResultClause();
	/*
	Description: Evaluates the Result Clause given that no other clauses are present for evaluation.
	*/

	QUERY_RESULT obtainFinalQueryResult();
	/*
	Description: Computes the final query result string from the result_list. Results depend on the return_type (BOOLEAN/Tuple/Synonym)
	*/

	void replaceSynonymsWithAttrRefValues();
	/*
	Description: Replaces the values of attrRef types in Result List to the appropriate values.
	*/

	VALUE_LIST getTupleValues();
	/*
	Description: Returns the final result of the selected tuple.
	*/

};