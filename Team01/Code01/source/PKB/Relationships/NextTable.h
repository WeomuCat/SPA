#pragma once

#include "../../AbstractDataTypes.h"
#include "RelationshipTransitiveTable.h"
#include "RelationshipTransitiveTable.cpp"

class NextTable : public RelationshipTransitiveTable<STMT_NUM> {

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
};
