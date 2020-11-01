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
    Types of Clauses:
    1. such that clause      
    2. with clause      
    3. pattern clause   

        such that clauses
        - 2 synonyms
            -> 2 common
            -> 1 common 1 uncommon
            -> 2 uncommon
        - 1 synonym (1 synonym, 1 int/ident/wildcard        -> eg Follows(a, 5))
            -> 1 common
            -> 1 uncommon
        - 0 synonyms (combination of int/ident/wildcard)    -> eg Follows(2,3)


        with clause
        - 2 synonyms
            -> 2 common
            -> 1 common 1 uncommon
            -> 2 uncommon
        - 1 synonym
            -> 1 common
            -> 1 uncommon
        - 0 synonym (combination of int/ident/wildcard)    -> eg with INTEGER = INTEGER | with IDENT = IDENT

        Possible types: attrRef + INTEGER | attrRef + IDENT | attrRef + synonym | attrRef + attrRef | IDENT + IDENT | INTEGER + INTEGER | INTEGER + synonym | synonym + synonym


        Pattern clause
        - 1 synonym
            -> 1 common
            -> 0 common
    

    Classifying Clause Types:
    1. true/false clause     -> relationship(int, int) | relationship(ident, ident) | relationship(_, _) | relationship(_, int) | relationship(_, ident) | relationship(int, _) | relationship(ident, _) | with IDENT = IDENT | with INTEGER = INTEGER
    2. such that clause with 2 common synonyms                                                  (EXLUCDING AFFECTS*)                                                                  
    3. such that clause with 1 common synonym + 1 uncommon synonym                              (EXCLUDING AFFECTS*)
    4. such that clause with 1 common synonym + 1 int/ident                                     (INCLUDING AFFECTS*)
    5. such that clause with 1 common synonym + 1 wildcard  (larger result list size than 4)    (EXCLUDING AFFECTS*)
    6. such that clause with 0 common synonyms + 2 uncommon synonyms                            (EXCLUDING AFFECTS*)
    7. such that clause with 0 common synonyms + 1 uncommon synonym                             (EXCLUDING AFFECTS*)
    8. with clause with 2 common synonyms                               -> eg with s1.stmt# = a2.stmt# where s1,a2 are in the resultlist
    9. with clause with 1 common synonm + 1 uncommon                    -> eg with s1.stmt# = 5 where s1 is in the resultlist
    10. with clause with 1 common synonym + 1 int/ident                 -> eg with s.stmt# = 5
    11. with clause with 0 common synonyms + 2 uncommon synonyms        -> eg with s1.stmt# = a2.stmt# where s1,a2 are not in the resultlist
    12. with clause with 0 common synonyms + 1 uncommon synonym         -> eg with s1.stmt# = 5 where s1 is not in the resultlist
    13. pattern clause with 1 common synonym
    14. pattern clause with 0 common synonyms
    15. such that affects*

    Rationale:
    - Keep intermediate resultlist small
    - Prioritize true/false clauses
    - Prioritize clauses with 1 constant (int/ident)
    - Prioritize clauses with common synonyms in the result list
    - Prioritize with clauses (more restrictive than such that)
    - Push affects* to the end

    Relationship Score:
    1. Calls
    2. Follows
    3. Modifies
    4. Uses
    5. Parent
    6. Next
    7. Calls*
    8. Parent*
    9. Follows*
    10. Next*
    11. Affects
    12. Affects*    (if applicable for that clause type)


    Clause Ranking Score (by clause number):
    1       1. true/false clause
    2       10. with clause with 1 common synonym + 1 int/ident -> eg with s.stmt# = 5
    3       4. such that clause with 1 common synonym + 1 int/ident
    4       8. with clause with 2 common synonyms
    5       2. such that clause with 2 common synonyms                                                  (EXLUCDING AFFECTS*)
    6       13. pattern clause with 1 common synonym
    7       5. such that clause with 1 common synonym + 1 wildcard  (larger result list size than 4)    (EXCLUDING AFFECTS*)
    8       9. with clause with 1 common synonm + 1 uncommon
    9       3. such that clause with 1 common synonym + 1 uncommon synonym                              (EXCLUDING AFFECTS*)
    10      12. with clause with 0 common synonyms + 1 uncommon synonym         -> eg with s1.stmt# = 5 where s1 is not in the resultlist
    11      7. such that clause with 0 common synonyms + 1 uncommon synonym                             (EXCLUDING AFFECTS*)
    12      14. pattern clause with 0 common synonyms
    13      11. with clause with 0 common synonyms + 2 uncommon synonyms        -> eg with s1.stmt# = a2.stmt# where s1,a2 are not in the resultlist
    14      6. such that clause with 0 common synonyms + 2 uncommon synonyms                            (EXCLUDING AFFECTS*)
    15      15. such that affects*
                            
    */

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
