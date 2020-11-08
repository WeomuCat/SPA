#pragma once

#include <unordered_map>
#include <unordered_set>
#include <queue>

#include "../../AbstractDataTypes.h"
#include "NonPrecomputedTransitiveRelationshipTable.h"
#include "NonPrecomputedTransitiveRelationshipTable.cpp"

class NextTable : public NonPrecomputedTransitiveRelationshipTable<STMT_NUM> {

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
    Description: Gets a STMT_NUM_LIST of the STMT_NUM that are next of s.
    */
    STMT_NUM_LIST getChildren(STMT_NUM s) override;

    /*
    Description: Returns the CFG of the given source that is based off next relation.
    */
    CFG getControlFlowGraph();

private:
    CFG cfg_;
};
