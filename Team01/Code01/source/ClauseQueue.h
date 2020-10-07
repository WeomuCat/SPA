#pragma once

#include "QueryNode.h"
#include <queue>

typedef QueryNode CLAUSE;
typedef std::pair<int, CLAUSE> RANKED_CLAUSE;
typedef std::vector<CLAUSE> CLAUSE_LIST;
typedef std::priority_queue <RANKED_CLAUSE, std::vector<RANKED_CLAUSE>, std::greater<RANKED_CLAUSE>> RANKED_CLAUSE_QUEUE; //min heap

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
};