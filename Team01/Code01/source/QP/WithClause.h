#pragma once

#include <unordered_map>

#include "../PKB/PKB.h"
#include "../PKB/PKBBuilder.h"
#include "QueryNode.h"
#include "QueryNodeType.h"
#include "QuerySynonymType.h"
#include "ResultList.h"
#include "AttrRefManager.h"


class WithClause {
public:
	WithClause(PROCESSED_SYNONYMS processed_synonyms, QueryNode with_clause);
	void getWithResult(PKB pkb, bool& clause_bool, ResultList& clause_result_list);

private:
	PROCESSED_SYNONYMS processed_synonyms;
	QueryNode with_clause;
	QueryNode lhs;
	QueryNode rhs;
	QUERY_NODE_TYPE lhs_type;
	QUERY_NODE_TYPE rhs_type;

	PKB pkb = (PKBBuilder().build());
	bool clause_bool;
	ResultList clause_result_list;

	void getAttrrefIntResult(QueryNode attrref_node, QueryNode int_node, bool& clause_bool, ResultList& clause_result_list);
	void getAttrrefStringResult(QueryNode attrref_node, QueryNode string_node, bool& clause_bool, ResultList& clause_result_list);
	void getAttrrefSynonymResult(QueryNode attrref_node, QueryNode synonym_node, bool& clause_bool, ResultList& clause_result_list);
	void getAttrrefAttrrefResult(QueryNode attrref_node, QueryNode int_node, bool& clause_bool, ResultList& clause_result_list);
	void getIdentIdentResult(QueryNode attrref_node, QueryNode string_node, bool& clause_bool, ResultList& clause_result_list);
	void getIntegerIntegerResult(QueryNode ident_node1, QueryNode ident_node2, bool& clause_bool, ResultList& clause_result_list);
	void getIntegerSynonymResult(QueryNode int_node1, QueryNode int_node2, bool& clause_bool, ResultList& clause_result_list);
	void getSynonymSynonymResult(QueryNode synonym_node1, QueryNode synonym_node2, bool& clause_bool, ResultList& clause_result_list);

	bool isValidWithClause();
	bool bothAreValidTypes();
	bool bothAreIntegerTypes();
	bool bothAreStringTypes();
	bool isValidType(QueryNode node);
	bool isIntegerType(QueryNode node);
	bool isStringType(QueryNode node);

};