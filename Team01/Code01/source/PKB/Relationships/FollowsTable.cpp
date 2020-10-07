#include "FollowsTable.h"

void FollowsTable::addFollows(STMT_NUM s1, STMT_NUM s2) {
    addRelationship(s1, s2);
}

BOOLEAN_TYPE FollowsTable::isFollows(STMT_NUM s1, STMT_NUM s2) {
    return isRelationship(s1, s2);
}

BOOLEAN_TYPE FollowsTable::isFollowsTransitive(STMT_NUM s1, STMT_NUM s2) {
    return isRelationshipTransitive(s1, s2);
}
