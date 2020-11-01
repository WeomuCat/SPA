#pragma once

#include <unordered_map>
#include <unordered_set>
#include <queue>

#include "../../AbstractDataTypes.h"
#include "../Hashing.h"

class NextTable {

public:
    /*
    Description: Stores the relationship Next(s1, s2) in the table.
    */
    void addNext(STMT_NUM s1, STMT_NUM s2);

    /*
    Description: Returns a BOOLEAN_TYPE indicating whether or not Next(s1, s2) holds.
    */
    BOOLEAN_TYPE isNext(STMT_NUM s1, STMT_NUM s2);

    /*
    Description: Returns a BOOLEAN_TYPE indicating whether or not Next*(s1, s2) holds.
    */
    BOOLEAN_TYPE isNextTransitive(STMT_NUM s1, STMT_NUM s2);

    /*
    Description: Returns the CFG of the given source that is based off next relation.
    */
    CFG getControlFlowGraph();

private:
    CFG cfg_;
    std::unordered_set<std::pair<STMT_NUM, STMT_NUM>> relationship_;
};
