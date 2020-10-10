#pragma once

#include "../../AbstractDataTypes.h"
#include "RelationshipTransitiveTable.h"
#include "RelationshipTransitiveTable.cpp"

class ParentTable : public RelationshipTransitiveTable<STMT_NUM> {
    /* Overview: This is a data structure */

public:
    /*
    Description: Stores the relationship Parent(s1, s2) in the table.
    */
    void addParent(STMT_NUM s1, STMT_NUM s2);

    /*
    Description: Returns a BOOLEAN_TYPE indicating whether or not Parent(s1, s2) holds.
    */
    BOOLEAN_TYPE isParent(STMT_NUM s1, STMT_NUM s2);

    /*
    Description: Returns a BOOLEAN_TYPE indicating whether or not Parent*(s1, s2) holds.
    */
    BOOLEAN_TYPE isParentTransitive(STMT_NUM s1, STMT_NUM s2);
};
