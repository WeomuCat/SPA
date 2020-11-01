#pragma once

#include "QueryNode.h"
#include <queue>
#include <utility>

typedef QueryNode CLAUSE;
typedef int RANK;
typedef std::pair<int, CLAUSE> RANKED_CLAUSE;
typedef std::vector<CLAUSE> CLAUSE_LIST;
typedef std::vector<SYNONYM_NAME> SYNONYM_NAMES;

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
		ClauseQueue clause_queue;
		clause_queue.addAllClauses(all_clauses);
	
	ACCESS:
		if (clause_queue.hasNext()) {
			CLAUSE next_clause = clause_queue.pop();
		}
	*/

public:
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
	CLAUSE_LIST clause_list;
	RANKED_CLAUSE_QUEUE clause_queue;

	void sortClauses();
	/*
	Description: Sort clauses by their rank, with lowest rank being the highest priority
	*/

	CLAUSE getNextClauseFromList();
	/*
	Description: Get the next clause from the unsorted input clause_list.
	*/

	bool listHasNext();
	/*
	Description: Returns True if the unsorted input clause_list is non-empty.
	*/

	RANK getClauseRank(CLAUSE clause, SYNONYM_NAMES& current_synonyms);
	/*
	Description: Gets the rank of this clause.
	*/

	bool isTrueFalseClause(CLAUSE clause);

	void clearClauseQueue();
};
