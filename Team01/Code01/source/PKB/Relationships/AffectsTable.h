#pragma once

#include "../../AbstractDataTypes.h"
#include "RelationshipTransitiveTable.h"
#include "RelationshipTransitiveTable.cpp"

class AffectsTable : public RelationshipTransitiveTable<STMT_NUM> {

public:
    /*
    Description: Stores the relationship Affects(s1, s2) in the table.
    */
    void addAffects(STMT_NUM s1, STMT_NUM s2);

    /*
    Description: Returns a BOOLEAN_TYPE indicating whether or not Affects(s1, s2) holds.
    */
    BOOLEAN_TYPE isAffects(STMT_NUM s1, STMT_NUM s2);

    /*
    Description: Returns a BOOLEAN_TYPE indicating whether or not Affects*(s1, s2) holds.
    */
    BOOLEAN_TYPE isAffectsTransitive(STMT_NUM s1, STMT_NUM s2);
};
