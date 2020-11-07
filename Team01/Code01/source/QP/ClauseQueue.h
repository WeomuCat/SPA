#pragma once

#include "QueryNode.h"
#include <queue>
#include <set>
#include <utility>
#include <algorithm>

typedef QueryNode CLAUSE;
typedef std::shared_ptr<QueryNode> CLAUSE_PTR;
typedef int RANK;
typedef std::pair<int, CLAUSE_PTR> RANKED_CLAUSE;
typedef std::vector<CLAUSE> CLAUSE_LIST;
typedef std::vector<CLAUSE_PTR> CLAUSE_PTR_LIST;
typedef std::unordered_set<SYNONYM_NAME> SYNONYM_NAMES;

// if c1's rank is larger, return true. Arrange from largest to smallest, min heap returns smallest. 
struct CompareRankedClauses {
	bool operator()(RANKED_CLAUSE const& c1, RANKED_CLAUSE const& c2) {
		return c1.first > c2.first;
	}
};

typedef std::priority_queue <RANKED_CLAUSE, std::vector<RANKED_CLAUSE>, CompareRankedClauses> RANKED_CLAUSE_QUEUE; //min heap

class ClauseQueue {
	/*Overview: Data structure that ranks clauses by order of evaluation*/
	/*
	Usage:
	INIT:
		std::vector<QueryNode> all_clauses;
		ClauseQueue clause_queue(true);
		clause_queue.addAllClauses(all_clauses);
	
	ACCESS:
		if (clause_queue.hasNext()) {
			CLAUSE next_clause = clause_queue.pop();
		}
	*/

public:
	ClauseQueue(BOOLEAN_TYPE enable_optimization);
	/*
	Description: Creates a ClauseQueue. If enable_optimization is true, clauses will be sorted. Else the order will be in the order of addition.
	*/

	void addAllClauses(CLAUSE_LIST all_clauses);
	/*
	Description: Adds all clauses of the query to this queue.
	*/

	CLAUSE pop();
	/*
	Description: Returns the highest priority clause by order of evaluation.
	*/
	
	bool hasNext();
	/*
	Description: Returns True if this ClauseQueue is non-empty.
	*/

private:
	BOOLEAN_TYPE enable_optimization;
	CLAUSE_PTR_LIST clause_list;
	RANKED_CLAUSE_QUEUE clause_queue;
	SYNONYM_NAMES synonyms_in_resultlist;

	void sortClauses();
	/*
	Description: Sort clauses by their rank, with lowest rank being the highest priority
	*/

	void removeClauseFromList(CLAUSE_PTR clause_ptr);
	/*
	Description: Removes this clause from clause_list.
	*/

	void updateSyonymNamesInResultList(CLAUSE new_clause);
	/*
	Description: Adds the synonym names in the new_clause to the synonyms_in_resultlist.
	*/

	RANK getClauseRank(CLAUSE clause);
	/*
	Description: Gets the rank of this clause.
	*/

	bool isTrueFalseClause(CLAUSE clause);
	/*
	Description: Returns True if the clause is true/false type.
	*/

	void clearClauseQueue();
	/*
	Description: Clears the clause_queue in this ClauseQueue.
	*/

	bool isInResultList(SYNONYM_NAME synonym_name);
	/*
	Description: Returns true if this synonym_name exists in synonyms_in_resultlist.
	*/

	RANK calculateFinalScore(RANK clause_rank_score, int relationship_score);
	/*
	Description: Calculates the final clause score based on its clause rank and relationship score.
	*/

	RANK calculateFinalScore(RANK clause_rank_score);
	/*
	Description: Calculates the final clause score based on its clause rank. Treats the relationship score as 0.
	*/

	RANK getRelationshipScore(QueryNodeType relationship_type);
	/*
	Description: Returns the score of the given relationship type.
	*/
};
