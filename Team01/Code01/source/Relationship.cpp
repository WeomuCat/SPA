#include <vector>
#include "Relationship.h"

Relationship::Relationship(QueryNode relationship_node, ResultList& intermediate_result_list) {
	this->relationship_node_ = relationship_node;
	this->intermediate_result_list = intermediate_result_list;
}

void Relationship::getRelationshipResult(PKB pkb, bool& clause_bool, ResultList& clause_result_list) {
	// Get type of the relationship node (Follows, Parent, etc.)
	QUERY_NODE_TYPE type = relationship_node_.getNodeType();
	// Get ref nodes of the relationship node
	QUERY_NODE_LIST refs = relationship_node_.getChildren();

	/* We assume that these semantic errors are handled by QPP */
	// (_, v)
	// UsesS
	// UsesP
	// ModifiesS
	// ModifiesP


	/* Populate valid_results from synonyms */
	std::vector<SYNONYM_VALUE> values_0;
	std::vector<SYNONYM_VALUE> values_1;


	// Populate values_0 and values_1
	switch (type) {
	case QUERY_NODE_TYPE::follows:
	case QUERY_NODE_TYPE::followsT:
	case QUERY_NODE_TYPE::parent:
	case QUERY_NODE_TYPE::parentT:
	case QUERY_NODE_TYPE::next:
	case QUERY_NODE_TYPE::nextT:
		// (s, s)
		for (SYNONYM_VALUE s : getStatementsFromRef(pkb, refs[0])) {
			values_0.push_back(s);
		}
		for (SYNONYM_VALUE s : getStatementsFromRef(pkb, refs[1])) {
			values_1.push_back(s);
		}
		break;
	case QUERY_NODE_TYPE::affects:
	case QUERY_NODE_TYPE::affectsT:
		// (a, a)
		for (SYNONYM_VALUE a : getAssignsFromRef(pkb, refs[0])) {
			values_0.push_back(a);
		}
		for (SYNONYM_VALUE a : getAssignsFromRef(pkb, refs[1])) {
			values_1.push_back(a);
		}
		break;
	case QUERY_NODE_TYPE::usesS:
	case QUERY_NODE_TYPE::modifiesS:
		// (s, v)
		for (SYNONYM_VALUE s : getStatementsFromRef(pkb, refs[0])) {
			values_0.push_back(s);
		}
		for (SYNONYM_VALUE v : getVariablesFromRef(pkb, refs[1])) {
			values_1.push_back(v);
		}
		break;
	case QUERY_NODE_TYPE::usesP:
	case QUERY_NODE_TYPE::modifiesP:
		// (p, v)
		for (SYNONYM_VALUE p : getProceduresFromRef(pkb, refs[0])) {
			values_0.push_back(p);
		}
		for (SYNONYM_VALUE v : getVariablesFromRef(pkb, refs[1])) {
			values_1.push_back(v);
		}
		break;
	case QUERY_NODE_TYPE::calls:
	case QUERY_NODE_TYPE::callsT:
		// (p, p)
		for (SYNONYM_VALUE p : getProceduresFromRef(pkb, refs[0])) {
			values_0.push_back(p);
		}
		for (SYNONYM_VALUE p : getProceduresFromRef(pkb, refs[1])) {
			values_1.push_back(p);
		}
		break;
	default:
		throw "QE: Relationship: Relationship given is not valid.";
	}

	// Populate valid results from values_0 and values_1
	std::vector<std::pair<SYNONYM_VALUE, SYNONYM_VALUE>> valid_results;

	// If both synonyms are equal, don't cross product,
	// i.e. fill valid_results with {1, 1}, {2, 2}, {3, 3}, ...

	// If they are not equal, do cross product!
	// i.e. fill valid_results with {1, 1}, {1, 2}, {1, 3}, ...
	if (areSynonymsEqual(refs[0], refs[1])) {
		for (SYNONYM_VALUE v : values_0) {
			valid_results.push_back({ v, v });
		}
	} else {
		for (SYNONYM_VALUE v0 : values_0) {
			for (SYNONYM_VALUE v1 : values_1) {
				valid_results.push_back({ v0, v1 });
			}
		}
	}

	/* Filter wrong relationships in valid_results, using PKB */
	
	// Optimization for queries with no synonyms:
	// The ResultList will always be empty because there are no synonyms.
	// Therefore if there is at least one valid result found,
	// we can skip checking the other results, as this clause will always return true!
	bool no_synonyms = refs[0].getNodeType() != QUERY_NODE_TYPE::synonym && refs[1].getNodeType() != QUERY_NODE_TYPE::synonym;

	for (auto it = valid_results.begin(); it != valid_results.end(); ) {
		std::pair<SYNONYM_VALUE, SYNONYM_VALUE> relationship = *it;
		SYNONYM_VALUE v0 = it->first;
		SYNONYM_VALUE v1 = it->second;

		bool valid;
		switch (type) {
		case QUERY_NODE_TYPE::affects:
			// (a, a)
			valid = pkb.isAffects(synValueToStmtNum(v0), synValueToStmtNum(v1));
			break;
		case QUERY_NODE_TYPE::affectsT:
			// (a, a)
			valid = pkb.isAffectsTransitive(synValueToStmtNum(v0), synValueToStmtNum(v1));
			break;
		case QUERY_NODE_TYPE::calls:
			// (p, p)
			valid = pkb.isCalls(v0, v1);
			break;
		case QUERY_NODE_TYPE::callsT:
			// (p, p)
			valid = pkb.isCallsTransitive(v0, v1);
			break;
		case QUERY_NODE_TYPE::follows:
			// (s, s)
			valid = pkb.isFollows(synValueToStmtNum(v0), synValueToStmtNum(v1));
			break;
		case QUERY_NODE_TYPE::followsT:
			// (s, s)
			valid = pkb.isFollowsTransitive(synValueToStmtNum(v0), synValueToStmtNum(v1));
			break;
		case QUERY_NODE_TYPE::modifiesP:
			// (p, v)
			valid = pkb.isModifies(v0, v1);
			break;
		case QUERY_NODE_TYPE::modifiesS:
			// (s, v)
			valid = pkb.isModifies(synValueToStmtNum(v0), v1);
			break;
		case QUERY_NODE_TYPE::next:
			// (s, s)
			valid = pkb.isNext(synValueToStmtNum(v0), synValueToStmtNum(v1));
			break;
		case QUERY_NODE_TYPE::nextT:
			// (s, s)
			valid = pkb.isNextTransitive(synValueToStmtNum(v0), synValueToStmtNum(v1));
			break;
		case QUERY_NODE_TYPE::parent:
			// (s, s)
			valid = pkb.isParent(synValueToStmtNum(v0), synValueToStmtNum(v1));
			break;
		case QUERY_NODE_TYPE::parentT:
			// (s, s)
			valid = pkb.isParentTransitive(synValueToStmtNum(v0), synValueToStmtNum(v1));
			break;
		case QUERY_NODE_TYPE::usesP:
			// (p, v)
			valid = pkb.isUses(v0, v1);
			break;
		case QUERY_NODE_TYPE::usesS:
			// (s, v)
			valid = pkb.isUses(synValueToStmtNum(v0), v1);
			break;
		default:
			throw "QE: Relationship: Relationship given is not valid.";
		}

		if (no_synonyms && valid) {
			clause_bool = true;
			return;
		}

		if (!valid) {
			it = valid_results.erase(it);
			continue;
		}
		it++;
	}

	// If valid results is empty, clause_bool is false and return
	if (valid_results.empty()) {
		clause_bool = false;
		return;
	}


	/* Populate ResultList */
	clause_bool = true;
	
	// Populate the ResultList if the ref is a synonym!
	if (refs[0].getNodeType() == QUERY_NODE_TYPE::synonym) {
		SYNONYM_NAME name = refs[0].getString();
		SYNONYM_VALUES_LIST column;

		for (std::pair<SYNONYM_VALUE, SYNONYM_VALUE> relationship : valid_results) {
			SYNONYM_VALUE v0 = relationship.first;
			column.push_back(v0);
		}

		clause_result_list.addColumn(name, column);
	}

	if (refs[1].getNodeType() == QUERY_NODE_TYPE::synonym) {
		SYNONYM_NAME name = refs[1].getString();
		SYNONYM_VALUES_LIST column;

		for (std::pair<SYNONYM_VALUE, SYNONYM_VALUE> relationship : valid_results) {
			SYNONYM_VALUE v1 = relationship.second;
			column.push_back(v1);
		}

		clause_result_list.addColumn(name, column);
	}
}

bool Relationship::areSynonymsEqual(QueryNode r1, QueryNode r2) {
	// If either are not synonyms, return false!
	if (r1.getNodeType() != QUERY_NODE_TYPE::synonym || r2.getNodeType() != QUERY_NODE_TYPE::synonym) {
		return false;
	}

	// If the synonym types are different, return false!
	if (r1.getSynonymType() != r2.getSynonymType()) {
		return false;
	}

	// If the synonym names are equal, return true!
	return r1.getString().compare(r2.getString()) == 0;
}

SYNONYM_VALUES_LIST Relationship::getStatementsFromRef(PKB pkb, QueryNode ref) {
	if (ref.getNodeType() == QueryNodeType::synonym) {
		SYNONYM_NAME ref_synonym_name = ref.getString();
		if (intermediate_result_list.containsSynonym(ref_synonym_name)) {
			SYNONYM_VALUES_LIST all_values = intermediate_result_list.getValuesOfSynonym(ref_synonym_name);
			std::unordered_set<SYNONYM_VALUE> unique_values(all_values.begin(), all_values.end());
			SYNONYM_VALUES_LIST unique_values_list(unique_values.begin(), unique_values.end());
			return unique_values_list;
		}
		else {
			return stmtNumsToSynValues(getStatementNumsFromRef(pkb, ref));
		}
	}
	else {
		return stmtNumsToSynValues(getStatementNumsFromRef(pkb, ref));
	}
}

SYNONYM_VALUES_LIST Relationship::getAssignsFromRef(PKB pkb, QueryNode ref) {
	if (ref.getNodeType() == QueryNodeType::synonym) {
		SYNONYM_NAME ref_synonym_name = ref.getString();
		if (intermediate_result_list.containsSynonym(ref_synonym_name)) {
			SYNONYM_VALUES_LIST all_values = intermediate_result_list.getValuesOfSynonym(ref_synonym_name);
			std::unordered_set<SYNONYM_VALUE> unique_values(all_values.begin(), all_values.end());
			SYNONYM_VALUES_LIST unique_values_list(unique_values.begin(), unique_values.end());
			return unique_values_list;
		}
		else {
			return stmtNumsToSynValues(getAssignNumsFromRef(pkb, ref));
		}
	}
	else {
		return stmtNumsToSynValues(getAssignNumsFromRef(pkb, ref));
	}
}

SYNONYM_VALUES_LIST Relationship::getVariablesFromRef(PKB pkb, QueryNode ref) {
	QueryNodeType type = ref.getNodeType();

	switch (type) {
	case QUERY_NODE_TYPE::ident:
		return { ref.getString() };
	case QUERY_NODE_TYPE::synonym:
	{
		SYNONYM_NAME ref_synonym_name = ref.getString();
		if (intermediate_result_list.containsSynonym(ref_synonym_name)) {
			SYNONYM_VALUES_LIST all_values = intermediate_result_list.getValuesOfSynonym(ref_synonym_name);
			std::unordered_set<SYNONYM_VALUE> unique_values(all_values.begin(), all_values.end());
			SYNONYM_VALUES_LIST unique_values_list(unique_values.begin(), unique_values.end());
			return unique_values_list;
		}
		
		QuerySynonymType syn_type = ref.getSynonymType();
		switch (syn_type) {
		case QuerySynonymType::variable:
			return pkb.getVariableNameList();
		default:
			throw "Relationship: Invalid entRef (variable) synonym!";
		}
	}
	case QUERY_NODE_TYPE::wild_card:
		return pkb.getVariableNameList();
	default:
		throw "Relationship: entRef (variable) is not a ident, synonym or wildcard!";
	}
}

SYNONYM_VALUES_LIST Relationship::getProceduresFromRef(PKB pkb, QueryNode ref) {
	QueryNodeType type = ref.getNodeType();

	switch (type) {
	case QUERY_NODE_TYPE::ident:
		return { ref.getString() };
	case QUERY_NODE_TYPE::synonym:
	{
		SYNONYM_NAME ref_synonym_name = ref.getString();
		if (intermediate_result_list.containsSynonym(ref_synonym_name)) {
			SYNONYM_VALUES_LIST all_values = intermediate_result_list.getValuesOfSynonym(ref_synonym_name);
			std::unordered_set<SYNONYM_VALUE> unique_values(all_values.begin(), all_values.end());
			SYNONYM_VALUES_LIST unique_values_list(unique_values.begin(), unique_values.end());
			return unique_values_list;
		}
		
		QuerySynonymType syn_type = ref.getSynonymType();
		switch (syn_type) {
		case QuerySynonymType::procedure:
			return pkb.getProcedureNameList();
		default:
			throw "Relationship: Invalid entRef (procedure) synonym!";
		}
	}
	case QUERY_NODE_TYPE::wild_card:
		return pkb.getProcedureNameList();
	default:
		throw "Relationship: entRef (procedure) is not a ident, synonym or wildcard!";
	}
}

STMT_NUM_LIST Relationship::getStatementNumsFromRef(PKB pkb, QueryNode ref) {
	QueryNodeType type = ref.getNodeType();

	switch (type) {
	case QUERY_NODE_TYPE::integer:
		return { ref.getInteger() };
	case QUERY_NODE_TYPE::synonym:
	{	
		QuerySynonymType syn_type = ref.getSynonymType();
		switch (syn_type) {
		case QuerySynonymType::assign:
			return pkb.getAssignNumList();
		case QuerySynonymType::call:
			return pkb.getCallNumList();
		case QuerySynonymType::ifs:
			return pkb.getIfNumList();
		case QuerySynonymType::print:
			return pkb.getPrintNumList();
		case QuerySynonymType::prog_line:
			return pkb.getStatementNumList();
		case QuerySynonymType::read:
			return pkb.getReadNumList();
		case QuerySynonymType::stmt:
			return pkb.getStatementNumList();
		case QuerySynonymType::whiles:
			return pkb.getWhileNumList();
		default:
			throw "Relationship: Invalid stmtRef (statement) synonym!";
		}
	}
	case QUERY_NODE_TYPE::wild_card:
		return pkb.getStatementNumList();
	default:
		throw "Relationship: stmtRef (statement) is not a integer, synonym or wildcard!";
	}
}

STMT_NUM_LIST Relationship::getAssignNumsFromRef(PKB pkb, QueryNode ref) {
	QueryNodeType type = ref.getNodeType();

	switch (type) {
	case QUERY_NODE_TYPE::integer:
		return { ref.getInteger() };
	case QUERY_NODE_TYPE::synonym:
	{
		QuerySynonymType syn_type = ref.getSynonymType();
		switch (syn_type) {
		case QuerySynonymType::assign:
		case QuerySynonymType::prog_line:
		case QuerySynonymType::stmt:
			return pkb.getAssignNumList();
		case QuerySynonymType::call:
		case QuerySynonymType::ifs:
		case QuerySynonymType::print:
		case QuerySynonymType::read:
		case QuerySynonymType::whiles:
			return {};
		default:
			throw "Relationship: Invalid stmtRef (assign) synonym!";
		}
	}
	case QUERY_NODE_TYPE::wild_card:
		return pkb.getAssignNumList();
	default:
		throw "Relationship: stmtRef (assign) is not a integer, synonym or wildcard!";
	}
}


/* STMT_NUM <=> SYNONYM_VALUE helper functions */
SYNONYM_VALUES_LIST Relationship::stmtNumsToSynValues(STMT_NUM_LIST stmt_nums) {
	SYNONYM_VALUES_LIST result;
	for (int s : stmt_nums) {
		SYNONYM_VALUE str = std::to_string(s);
		stmt_nums_.insert({ str, s });
		result.push_back(str);
	}
	return result;
}

STMT_NUM Relationship::synValueToStmtNum(SYNONYM_VALUE stmt_string) {
	auto it = stmt_nums_.find(stmt_string);
	if (it == stmt_nums_.end()) {
		return std::stoi(stmt_string);
	}
	return it->second;
}
