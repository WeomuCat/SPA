#pragma once

#include "RelationshipTransitiveTable.h"
#include "RelationshipTransitiveTable.cpp"

// TODO: Might need to change STMT_NUM to PROG_NUM
typedef int STMT_NUM;

class NextTable : public RelationshipTransitiveTable<STMT_NUM> {
    /* Overview: This is a data structure */

public:
    void addNext(STMT_NUM s1, STMT_NUM s2);
    /*
    Description: Stores the relationship Next(s1, s2) in the table.
    */

    BOOLEAN_TYPE isNext(STMT_NUM s1, STMT_NUM s2);
    /*
    Description: Returns a BOOLEAN_TYPE indicating whether or not Next(s1, s2) holds.
    */

    BOOLEAN_TYPE isNextTransitive(STMT_NUM s1, STMT_NUM s2);
    /*
    Description: Returns a BOOLEAN_TYPE indicating whether or not Next*(s1, s2) holds.
    */
};
