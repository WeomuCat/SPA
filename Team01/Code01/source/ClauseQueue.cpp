#include "ClauseQueue.h"

ClauseQueue::ClauseQueue(BOOLEAN_TYPE enable_optimization) {
    this->enable_optimization = enable_optimization;
}

void ClauseQueue::addAllClauses(CLAUSE_LIST all_clauses) {
    for (CLAUSE clause : all_clauses) {
        std::shared_ptr<CLAUSE> c_ptr = std::make_shared<CLAUSE>(clause);
        clause_list.push_back(c_ptr);
    }
}

bool ClauseQueue::hasNext() {
    bool hasNext = false;
    if (clause_list.size() > 0) {
        hasNext = true;
    }
    return hasNext;
}

CLAUSE ClauseQueue::pop() {
    // Sorts clauses to find the clause with the lowest rank to return.
    // Clear the queue after retrieving the clause to allow next round of sorting.
    if (enable_optimization) {
        sortClauses();
        RANKED_CLAUSE current_clause = clause_queue.top();
        clause_queue.pop();

        CLAUSE_PTR lowest_ranked_clause = current_clause.second;
        updateSyonymNamesInResultList(*lowest_ranked_clause); // add synonyms names of this clause to the set of result list syonym names
        removeClauseFromList(lowest_ranked_clause);
        clearClauseQueue();

        return *lowest_ranked_clause;
    }
    else {
        CLAUSE_PTR_LIST::iterator clause_ptr_ptr = clause_list.begin();
        CLAUSE* clause_ptr = clause_ptr_ptr->get();
        CLAUSE clause = *clause_ptr;
        
        clause_list.erase(clause_list.begin());

        return clause;
    }
    
}


void ClauseQueue::sortClauses() {
    /*
    CLAUSE_PTR_LIST original_list(clause_list);
    while (listHasNext()) {
        CLAUSE current_clause = getNextClauseFromList();
        RANK clause_rank = getClauseRank(current_clause);
        RANKED_CLAUSE ranked_clause(clause_rank, current_clause);

        clause_queue.push(ranked_clause);
    }
    clause_list = original_list;
    */

    for (CLAUSE_PTR clause_ptr : clause_list) {
        CLAUSE current_clause = *clause_ptr;
        RANK clause_rank = getClauseRank(current_clause);
        RANKED_CLAUSE ranked_clause = std::make_pair(clause_rank, clause_ptr);
        
        clause_queue.push(ranked_clause);
    }
}


RANK ClauseQueue::getClauseRank(CLAUSE clause) {
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
    1. true/false clause     -> relationship(int, int) | relationship(ident, ident) | UsesP(int, ident) | ModifiesP(int, ident) | relationship(_, _) | relationship(_, int) | relationship(_, ident) | relationship(int, _) | relationship(ident, _) | with IDENT = IDENT | with INTEGER = INTEGER
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
            
            Order for affects*: 
                1. 1 common + 1 wildcard
                2. 2 common
                3. 1 common + 1 uncommon
                4. 0 common + 1 uncommon 
                5. 0 common + 2 uncommon

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


    Clause Ranking Score BEFORE accounting for Relationship Score (by clause number):
    0       1. true/false clause
    1       10. with clause with 1 common synonym + 1 int/ident -> eg with s.stmt# = 5
    2       4. such that clause with 1 common synonym + 1 int/ident
    3       8. with clause with 2 common synonyms
    4       2. such that clause with 2 common synonyms                                                  (EXLUCDING AFFECTS*)
    5       13. pattern clause with 1 common synonym
    6       5. such that clause with 1 common synonym + 1 wildcard  (larger result list size than 4)    (EXCLUDING AFFECTS*)
    7       9. with clause with 1 common synonm + 1 uncommon
    8       3. such that clause with 1 common synonym + 1 uncommon synonym                              (EXCLUDING AFFECTS*)
    9       12. with clause with 0 common synonyms + 1 uncommon synonym         -> eg with s1.stmt# = 5 where s1 is not in the resultlist
    10      7. such that clause with 0 common synonyms + 1 uncommon synonym                             (EXCLUDING AFFECTS*)
    11      14. pattern clause with 0 common synonyms
    12      11. with clause with 0 common synonyms + 2 uncommon synonyms        -> eg with s1.stmt# = a2.stmt# where s1,a2 are not in the resultlist
    13      6. such that clause with 0 common synonyms + 2 uncommon synonyms                            (EXCLUDING AFFECTS*)
    14      15. such that affects*
    
    FINAL SCORE:  (Clause Ranking Score * 13) + (Relationship Score | 0 if clause is not such that)
    
    Notes:
    - we need clause ranking score * 13 to account for the 12 relationships in such that (eg 2*13+12 = 38 < 3*13 = 39)
    - we do this for every clause type for convenience, since the exact score does not matter but the order matters more.
    - for clauses of the exact same score (eg both true/false clause), tie break arbitrarily

    */
    
    RANK rank = 195;    // init to max score possible
    QueryNodeType clause_type = clause.getNodeType();

    if (isTrueFalseClause(clause)) {
        rank = calculateFinalScore(0, 0);
        return rank;
    }

    if (clause_type == QueryNodeType::such_that) {
        QueryNode relationship = clause.getChildren()[0];
        QueryNodeType relationship_type = relationship.getNodeType();
        RANK relationship_score = getRelationshipScore(relationship_type);

        QueryNode child1 = relationship.getChildren()[0];
        QueryNode child2 = relationship.getChildren()[1];
        QueryNodeType child1_type = child1.getNodeType();
        QueryNodeType child2_type = child2.getNodeType();

        if (child1_type == QueryNodeType::synonym) {
            SYNONYM_NAME child1_name = child1.getString();
            
            if (isInResultList(child1_name)) {
                if (child2_type == QueryNodeType::synonym) {
                    SYNONYM_NAME child2_name = child2.getString();

                    if (isInResultList(child2_name)) {
                        
                        // 2 common synonyms
                        if (relationship_type == QueryNodeType::affectsT) {
                            // 15. affects* with 2 common synonyms
                            rank = calculateFinalScore(14, 2);
                        }
                        else {
                            // 2. such that clause with 2 common synonyms (EXLUCDING AFFECTS*)
                            rank = calculateFinalScore(4, relationship_score);
                        }
                    }
                    else {

                        // 1 common + 1 uncommon
                        if (relationship_type == QueryNodeType::affectsT) {
                            // 15. affects* with 1 common + 1 uncommon
                            rank = calculateFinalScore(14, 3);
                        }
                        else {
                            // 3. such that clause with 1 common synonym + 1 uncommon synonym (EXCLUDING AFFECTS*)
                            rank = calculateFinalScore(8, relationship_score);
                        }
                    }
                }

                else if (child2_type == QueryNodeType::integer || child2_type == QueryNodeType::ident) {
                    
                    // 4. such that clause with 1 common synonym + 1 int / ident (INCLUDING AFFECTS*)
                    rank = calculateFinalScore(2, relationship_score);
                }

                else if (child2_type == QueryNodeType::wild_card) {
                    
                    // 1 common synonym + 1 wildcard
                    if (relationship_type == QueryNodeType::affectsT) {
                        // 15. affects* with 1 common + 1 wildcard
                        rank = calculateFinalScore(14, 1);
                    }
                    else {
                        // 5. such that clause with 1 common synonym + 1 wildcard(larger result list size than 4)   (EXCLUDING AFFECTS*)
                        rank = calculateFinalScore(6, relationship_score);
                    }
                }
                else {
                    throw "ClauseQueue: Clause type is not valid. Should be of the type: Relationship(synonym, synonym/int/ident/wildcard)";
                }
            }

            else {
                // child1 is synonym not in resultlist
                if (child2_type == QueryNodeType::synonym) {
                    SYNONYM_NAME child2_name = child2.getString();
                    
                    if (isInResultList(child2_name)) {
                        
                        // 1 uncommon + 1 common
                        if (relationship_type == QueryNodeType::affectsT) {
                            // 15. affects* with 1 common + 1 uncommon
                            rank = calculateFinalScore(14, 3);
                        }
                        else {
                            // 3. such that clause with 1 common synonym + 1 uncommon synonym (EXCLUDING AFFECTS*)
                            rank = calculateFinalScore(8, relationship_score);
                        }
                        
                    }
                    else {

                        // 0 common + 2 uncommon
                        if (relationship_type == QueryNodeType::affectsT) {
                            // 15. affects* with 0 common + 2 uncommon
                            rank = calculateFinalScore(14, 5);
                        }
                        else {
                            // 6. such that clause with 0 common synonyms + 2 uncommon synonyms(EXCLUDING AFFECTS*)
                            rank = calculateFinalScore(13, relationship_score);
                        }
                    }
                }

                else if (child2_type == QueryNodeType::integer || child2_type == QueryNodeType::ident || child2_type == QueryNodeType::wild_card) {
                    // 0 common + 1 uncommon
                    if (relationship_type == QueryNodeType::affectsT) {
                        // 15. affects* with 0 common + 1 uncommon
                        rank = calculateFinalScore(14, 4);
                    }
                    else {
                        // 7. such that clause with 0 common synonyms + 1 uncommon synonym (EXCLUDING AFFECTS*)
                        rank = calculateFinalScore(10, relationship_score);
                    }
                }
                else {
                    throw "ClauseQueue: Clause type is invalid. Should be of the type: Relationship(synonym, synonym/int/ident/wildcard";
                }
            }
        }

        if (child1_type == QueryNodeType::integer || child1_type == QueryNodeType::ident) {
            if (child2_type == QueryNodeType::synonym) {
                SYNONYM_NAME child2_name = child2.getString();

                if (isInResultList(child2_name)) {
                    // 4. such that clause with 1 common synonym + 1 int / ident(INCLUDING AFFECTS*)
                    rank = calculateFinalScore(2, relationship_score);
                }
                else {
                    // 0 common + 1 uncommon
                    if (relationship_type == QueryNodeType::affectsT) {
                        // 15. affects* with 0 common + 1 uncommon
                        rank = calculateFinalScore(14, 4);
                    }
                    else {
                        // 7. such that clause with 0 common synonyms + 1 uncommon synonym (EXCLUDING AFFECTS*)
                        rank = calculateFinalScore(10, relationship_score);
                    }
                }
            }
            // Note that if child2_type is int/ident/wildcard, it would be a true/false clause that is already tabulated at the start
        }
        
        if (child1_type == QueryNodeType::wild_card) {
            if (child2_type == QueryNodeType::synonym) {
                SYNONYM_NAME child2_name = child2.getString();

                if (isInResultList(child2_name)) {
                    // 1 common + 1 wildcard
                    if (relationship_type == QueryNodeType::affectsT) {
                        // 15. affects* with 1 common + 1 wildcard
                        rank = calculateFinalScore(14, 1);
                    }
                    else {
                        // 5. such that clause with 1 common synonym + 1 wildcard (EXCLUDING AFFECTS*)
                        rank = calculateFinalScore(6, relationship_score);
                    }
                }
                else {
                    // 0 common + 1 uncommon
                    if (relationship_type == QueryNodeType::affectsT) {
                        // 15. affects* with 0 common + 1 uncommon
                        rank = calculateFinalScore(14, 4);
                    }
                    else {
                        // 7. such that clause with 0 common synonyms + 1 uncommon synonym (EXCLUDING AFFECTS*)
                        rank = calculateFinalScore(10, relationship_score);
                    }
                }
            }
            // Note that if child2_type is int/ident/wildcard, it would be a true/false clause that is already tabulated at the start
        }
    }

    if (clause_type == QueryNodeType::with) {
        QueryNode lhs = clause.getChildren()[0];
        QueryNode rhs = clause.getChildren()[1];
        QueryNodeType lhs_type = lhs.getNodeType();
        QueryNodeType rhs_type = rhs.getNodeType();

        // Possible types : attrRef + INTEGER | attrRef + IDENT | attrRef + synonym | attrRef + attrRef | IDENT + IDENT (true/false) | INTEGER + INTEGER (true/false) | INTEGER + synonym | synonym + synonym

        if (lhs_type == QueryNodeType::attr || lhs_type == QueryNodeType::synonym) {
            SYNONYM_NAME lhs_name = lhs.getString();
            
            if (isInResultList(lhs_name)) {
                if (rhs_type == QueryNodeType::attr || rhs_type == QueryNodeType::synonym) {
                    SYNONYM_NAME rhs_name = rhs.getString();

                    if (isInResultList(rhs_name)) {
                        // 8. with clause with 2 common synonyms
                        rank = calculateFinalScore(3);
                    }
                    else {
                        // 9. with clause with 1 common synonm + 1 uncommon
                        rank = calculateFinalScore(7);
                    }
                }
                
                else if (rhs_type == QueryNodeType::integer || rhs_type == QueryNodeType::ident) {
                    // 10. with clause with 1 common synonym + 1 int/ident
                    rank = calculateFinalScore(1);
                }
                
                else {
                    throw "ClauseQueue: Rhs type is not attr | integer | ident | synonym.";
                }
            }
            else {
                // lhs synonym name is not in result list
                if (rhs_type == QueryNodeType::attr || rhs_type == QueryNodeType::synonym) {
                    SYNONYM_NAME rhs_name = rhs.getString();

                    if (isInResultList(rhs_name)) {
                        // 9. with clause with 1 common synonm + 1 uncommon
                        rank = calculateFinalScore(7);
                    }
                    else {
                        // 11. with clause with 0 common synonyms + 2 uncommon synonyms
                        rank = calculateFinalScore(12);
                    }
                }
                
                else if (rhs_type == QueryNodeType::integer || rhs_type == QueryNodeType::ident) {
                    // 12. with clause with 0 common synonyms + 1 uncommon synonym
                    rank = calculateFinalScore(9);
                }
                
                else {
                    throw "ClauseQueue: Rhs type is not attr | integer | ident | synonym.";
                }
            }
        }
        
        if (lhs_type == QueryNodeType::ident || lhs_type == QueryNodeType::integer) {
            if (rhs_type == QueryNodeType::attr) {
                SYNONYM_NAME rhs_name = rhs.getString();
                if (isInResultList(rhs_name)) {
                    // 10. with clause with 1 common synonym + 1 int/ident
                    rank = calculateFinalScore(1);
                }
                else {
                    // 12. with clause with 0 common synonyms + 1 uncommon synonym
                    rank = calculateFinalScore(9);
                }
            }
            // Note: if rhs_type is integer/ident, it would be a true/false clause that is tabulated at the start.
        }
    }

    if (clause_type == QueryNodeType::pattern) {
        QueryNode synonym_node = clause.getChildren()[0];
        SYNONYM_NAME synonym_name = synonym_node.getString();
        
        if (isInResultList(synonym_name)) {
            // 13. pattern clause with 1 common synonym
            rank = calculateFinalScore(5);
        }
        else {
            // 14. pattern clause with 0 common synonyms
            rank = calculateFinalScore(11);
        }
    }

    return rank;
}

bool ClauseQueue::isTrueFalseClause(CLAUSE clause) {
    /*
    Types of True/False Clauses:
        relationship(int, int)
        relationship(ident, ident) 
        UsesP(int, ident) 
        ModifiesP(int, ident) 
        relationship(_, _) 
        relationship(_, int) 
        relationship(_, ident) 
        relationship(int, _) 
        relationship(ident, _) 
        with IDENT = IDENT 
        with INTEGER = INTEGER
    */
    bool isTrueFalseClause = false;
    QueryNodeType clause_type = clause.getNodeType();

    if (clause_type == QueryNodeType::such_that) {
        QueryNode relationship = clause.getChildren()[0];
        QueryNodeType relationship_type = relationship.getNodeType();

        QueryNode child1 = relationship.getChildren()[0];
        QueryNode child2 = relationship.getChildren()[1];
        QueryNodeType child1_type = child1.getNodeType();
        QueryNodeType child2_type = child2.getNodeType();

        if (child1_type == QueryNodeType::integer && child2_type == QueryNodeType::integer) {
            isTrueFalseClause = true;
        }

        if (child1_type == QueryNodeType::ident && child2_type == QueryNodeType::ident) {
            isTrueFalseClause = true;
        }
        
        if (relationship_type == QueryNodeType::usesP && child1_type == QueryNodeType::integer && child2_type == QueryNodeType::ident) {
            isTrueFalseClause = true;
        }

        if (relationship_type == QueryNodeType::modifiesP && child1_type == QueryNodeType::integer && child2_type == QueryNodeType::ident) {
            isTrueFalseClause = true;
        }

        if (child1_type == QueryNodeType::wild_card) {
            if (child2_type == QueryNodeType::wild_card || child2_type == QueryNodeType::integer || child2_type == QueryNodeType::ident) {
                isTrueFalseClause = true;
            }
        }

        if (child2_type == QueryNodeType::wild_card) {
            if (child1_type == QueryNodeType::integer || child1_type == QueryNodeType::ident) {
                isTrueFalseClause = true;
            }
        }
    }

    if (clause_type == QueryNodeType::with) {
        QueryNode lhs = clause.getChildren()[0];
        QueryNode rhs = clause.getChildren()[1];
        QueryNodeType lhs_type = lhs.getNodeType();
        QueryNodeType rhs_type = rhs.getNodeType();

        if (lhs_type == QueryNodeType::ident && rhs_type == QueryNodeType::ident) {
            isTrueFalseClause = true;
        }

        if (lhs_type == QueryNodeType::integer && rhs_type == QueryNodeType::integer) {
            isTrueFalseClause = true;
        }
    }

    return isTrueFalseClause;
}

CLAUSE ClauseQueue::getNextClauseFromList() {
    auto first_clause_ptr = clause_list.begin();
    CLAUSE first_clause = **first_clause_ptr;
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

void ClauseQueue::clearClauseQueue() {
    // reset since priority queue doesn't have a clear method
    RANKED_CLAUSE_QUEUE empty_queue;
    clause_queue = empty_queue;
}

bool ClauseQueue::isInResultList(SYNONYM_NAME synonym_name) {
    return synonyms_in_resultlist.find(synonym_name) != synonyms_in_resultlist.end();
}

void ClauseQueue::removeClauseFromList(CLAUSE_PTR clause_ptr) {
    auto iterend = clause_list.end();
    auto remove_iter = std::remove_if(clause_list.begin(), clause_list.end(), [clause_ptr](CLAUSE_PTR i) {return i == clause_ptr; });
    clause_list.erase(
        remove_iter,
        clause_list.end());
}

void ClauseQueue::updateSyonymNamesInResultList(CLAUSE clause) {
    QueryNodeType clause_type = clause.getNodeType();

    if (clause_type == QueryNodeType::such_that) {
        QueryNode relationship = clause.getChildren()[0];
        QueryNode child1 = relationship.getChildren()[0];
        QueryNode child2 = relationship.getChildren()[1];
        QueryNodeType child1_type = child1.getNodeType();
        QueryNodeType child2_type = child2.getNodeType();

        if (child1_type == QueryNodeType::synonym) {
            SYNONYM_NAME child1_name = child1.getString();
            synonyms_in_resultlist.insert(child1_name);
        }

        if (child2_type == QueryNodeType::synonym) {
            SYNONYM_NAME child2_name = child2.getString();
            synonyms_in_resultlist.insert(child2_name);
        }
    }
    else if (clause_type == QueryNodeType::with) {
        QueryNode lhs = clause.getChildren()[0];
        QueryNode rhs = clause.getChildren()[1];
        QueryNodeType lhs_type = lhs.getNodeType();
        QueryNodeType rhs_type = rhs.getNodeType();

        if (lhs_type == QueryNodeType::attr || lhs_type == QueryNodeType::synonym) {
            SYNONYM_NAME lhs_name = lhs.getString();
            synonyms_in_resultlist.insert(lhs_name);
        }

        if (rhs_type == QueryNodeType::attr || rhs_type == QueryNodeType::synonym) {
            SYNONYM_NAME rhs_name = rhs.getString();
            synonyms_in_resultlist.insert(rhs_name);
        }
        
    }
    else if (clause_type == QueryNodeType::pattern) {
        QueryNode synonym_node = clause.getChildren()[0];
        SYNONYM_NAME synonym_name = synonym_node.getString();

        synonyms_in_resultlist.insert(synonym_name);
    }
    else {
        throw "ClauseQueue: updateSynonymNamesInResultList: clause type is not such that | with | pattern.";
    }

}

RANK ClauseQueue::getRelationshipScore(QueryNodeType relationship_type) {
    /*
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
    12. Affects*
    */
    RANK score = 0;

    switch (relationship_type) {
    case(QueryNodeType::calls):
        score = 1;
        break;
    case(QueryNodeType::follows):
        score = 2;
        break;
    case(QueryNodeType::modifiesP):
        score = 3;
        break;
    case(QueryNodeType::modifiesS):
        score = 3;
        break;
    case(QueryNodeType::usesP):
        score = 4;
        break;
    case(QueryNodeType::usesS):
        score = 4;
        break;
    case(QueryNodeType::parent):
        score = 5;
        break;
    case(QueryNodeType::next):
        score = 6;
        break;
    case(QueryNodeType::callsT):
        score = 7;
        break;
    case(QueryNodeType::parentT):
        score = 8;
        break;
    case(QueryNodeType::followsT):
        score = 9;
        break;
    case(QueryNodeType::nextT):
        score = 10;
        break;
    case(QueryNodeType::affects):
        score = 11;
        break;
    case(QueryNodeType::affectsT):
        score = 12;
        break;
    }

    return score;
}


RANK ClauseQueue::calculateFinalScore(RANK clause_rank_score) {
    return calculateFinalScore(clause_rank_score, 0);
}

RANK ClauseQueue::calculateFinalScore(RANK clause_rank_score, int relationship_score) {
    // FINAL SCORE : (Clause Ranking Score * 13) + (Relationship Score | 0 if clause is not such that)
    RANK final_score = clause_rank_score * 13 + relationship_score;

    return final_score;
}

