#include "ClauseQueue.h"

void ClauseQueue::addAllClauses(CLAUSE_LIST all_clauses) {
    this->clause_list = all_clauses;
    sortClauses();
}

CLAUSE ClauseQueue::pop() {
    // currently return clauses in the order of the input clause vector
    RANKED_CLAUSE current_clause = clause_queue.top();
    clause_queue.pop();
    return current_clause.second;
}

bool ClauseQueue::hasNext() {
    bool hasNext = false;
    if (clause_queue.size() > 0) {
        hasNext = true;
    }
    return hasNext;
}

void ClauseQueue::sortClauses() {
    RANKED_CLAUSE_QUEUE clause_queue;

    /* 
    Currently assigns rank in increasing order
    Later on: implement getRank() and use it here
    */
    int rank = 0;
    while (listHasNext()) {
        RANKED_CLAUSE ranked_clause(rank, getNextClauseFromList());
        clause_queue.push(ranked_clause);
        rank = rank + 1;
    }

    this->clause_queue = clause_queue;
}

CLAUSE ClauseQueue::getNextClauseFromList() {
    auto first_clause_ptr = clause_list.begin();
    CLAUSE first_clause = *first_clause_ptr;
    clause_list.erase(clause_list.begin());
    return first_clause;
}

bool ClauseQueue::listHasNext() {
    bool hasNext = false;
    if (clause_list.size() > 0) {
        hasNext = true;
    }
    return hasNext;
}
