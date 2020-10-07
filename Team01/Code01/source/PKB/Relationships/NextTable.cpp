#include "NextTable.h"

void NextTable::addNext(STMT_NUM s1, STMT_NUM s2) {
    addRelationship(s1, s2);
}

BOOLEAN_TYPE NextTable::isNext(STMT_NUM s1, STMT_NUM s2) {
    return isRelationship(s1, s2);
}

BOOLEAN_TYPE NextTable::isNextTransitive(STMT_NUM s1, STMT_NUM s2) {
    return isRelationshipTransitive(s1, s2);
}
