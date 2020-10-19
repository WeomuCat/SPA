#pragma once

#include <unordered_map>

#include "PKB.h"
#include "QueryNode.h"
#include "QueryNodeType.h"
#include "QuerySynonymType.h"
#include "ResultList.h"
#include "AttrRefManager.h"

typedef std::unordered_map<std::string, QueryNode> PROCESSED_SYNONYMS;

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
	bool isValidWithClause();
	bool bothAreIntegerTypes();
	bool bothAreStringTypes();
	bool isIntegerType(QueryNode node);
	bool isStringType(QueryNode node);

};