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
    SYNONYM_NAMES current_synonyms;


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

RANK ClauseQueue::getClauseRank(CLAUSE clause, SYNONYM_NAMES& current_synonyms) {
    /*
    Possible Clauses:
    - such that clause      -> 2 common synonyms | 1 common synonym + 1 uncommon synonym | 1 common synonym + int/ident | 2 uncommon synonyms | 2 int/ident
    - with clause           -> attrRef + INTEGER | attrRef + IDENT | attrRef + synonym | attrRef + attrRef | IDENT + IDENT | INTEGER + INTEGER | INTEGER + synonym | synonym + synonym
    - pattern clause        -> 1 common synonym | 1 uncommon synonym

    - true/false clause     -> relationship(int, int) | relationship(ident, ident) | relationship(_, _) | relationship(_, int) | relationship(_, ident) | relationship(int, _) | relationship(ident, _) | with IDENT = IDENT | with INT = INT
    */

    // True/False clause                                                    -> rank = 
    // Such that clause with 2 common synonyms                              -> rank = 
    // Such that clause with 1 common syn, 1 integer                        -> rank = 
    // With clause that is not both attrRef and not both synonym            -> rank = 
    // Pattern clause with 1 common synonym                                 -> rank = 
    // With clause (remaining: both attrRef or both synonym)                -> rank = 
    // Such that clause with 1 common synonym, other is synonym not common  -> rank = 
    // Pattern with no common synonym                                       -> rank = 
    // Such that clause with no common synonyms (cross product)             -> rank = 
    RANK rank;

    if (isTrueFalseClause(clause)) {
        rank = 1;
        return rank;
    }

    if (clause.getNodeType() == QueryNodeType::such_that) {
        QueryNode first_child = clause.getChildren()[0];
        QueryNode second_child = clause.getChildren()[1];
        SYNONYM_NAME first_child_name = first_child.getString();
        SYNONYM_NAME second_child_name = second_child.getString();

        // if current_synonyms contains first_child_name and second_child_name -> rank = 2
        // if current_synonyms contains one child and other child is int -> rank = 3
        // With clause that is not both attrRef and not both synonym -> rank = 4
        // Pattern clause with 1 common synonym -> rank = 5
        // With clause (remaining: both attrRef or both synonym) -> rank = 6
        // Such that clause with 1 common synonym, other is synonym not common -> rank = 7
        // Pattern with no common synonym -> rank = 8
        // Such that clause with no common synonyms (cross product) -> rank = 9
    }

    return rank;
}

bool ClauseQueue::isTrueFalseClause(CLAUSE clause) {
    // unimplemented
    // is True if such_that with 2 ident
    // is True if with with 2 integer/ident
    bool isTrueFalseClause = false;

    return isTrueFalseClause;
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
