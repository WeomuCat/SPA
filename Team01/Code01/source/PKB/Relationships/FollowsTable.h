#pragma once

#include "../../AbstractDataTypes.h"
#include "PrecomputedTransitiveRelationshipTable.h"
#include "PrecomputedTransitiveRelationshipTable.cpp"

class FollowsTable : public PrecomputedTransitiveRelationshipTable<STMT_NUM> {

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
