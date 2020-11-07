#include "QueryProcessor.h"

QueryProcessor::QueryProcessor(PKB pkb) {
	this->pkb = pkb;
}

QUERY_RESULT QueryProcessor::processQuery(QUERY query) {

	//Uncomment when preprocessor is done
	
	// QPP
	QueryPreProcessor pre_processor = QueryPreProcessor();
	SPLIT_QUERY splitted = pre_processor.splitQuery(query);
	PROCESSED_SYNONYMS processed_synonyms = pre_processor.preProcessSynonyms(splitted[0]);
	PROCESSED_CLAUSES processed_clauses = pre_processor.preProcessClauses(processed_synonyms, splitted[1]); //returns table of synonym nodes

	
	// QE
	QueryEvaluator query_evaluator = QueryEvaluator(this->pkb);

	QUERY_RESULT query_result = query_evaluator.evaluateQuery(processed_synonyms, processed_clauses);
	return query_result;
}

