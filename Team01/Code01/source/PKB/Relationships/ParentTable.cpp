#include "ParentTable.h"

void ParentTable::addParent(STMT_NUM s1, STMT_NUM s2) {
    addRelationship(s1, s2);
}

BOOLEAN_TYPE ParentTable::isParent(STMT_NUM s1, STMT_NUM s2) {
    return isRelationship(s1, s2);
}

BOOLEAN_TYPE ParentTable::isParentTransitive(STMT_NUM s1, STMT_NUM s2) {
    return isRelationshipTransitive(s1, s2);
}
