#pragma once

#include "../../AbstractDataTypes.h"
#include "RelationshipTransitiveTable.h"
#include "RelationshipTransitiveTable.cpp"

class FollowsTable : public RelationshipTransitiveTable<STMT_NUM> {

public:
    /*
    Description: Stores the relationship Follows(s1, s2) in the table.
    */
    void addFollows(STMT_NUM s1, STMT_NUM s2);

    /*
    Description: Returns a BOOLEAN_TYPE indicating whether or not Follows(s1, s2) holds.
    */
    BOOLEAN_TYPE isFollows(STMT_NUM s1, STMT_NUM s2);

    /*
    Description: Returns a BOOLEAN_TYPE indicating whether or not Follows*(s1, s2) holds.
    */
    BOOLEAN_TYPE isFollowsTransitive(STMT_NUM s1, STMT_NUM s2);
};
