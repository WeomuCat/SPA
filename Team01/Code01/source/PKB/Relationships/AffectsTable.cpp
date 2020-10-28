#include "AffectsTable.h"

void AffectsTable::addAffects(STMT_NUM s1, STMT_NUM s2) {
    addRelationship(s1, s2);
}

BOOLEAN_TYPE AffectsTable::isAffects(STMT_NUM s1, STMT_NUM s2) {
    return isRelationship(s1, s2);
}

BOOLEAN_TYPE AffectsTable::isAffectsTransitive(STMT_NUM s1, STMT_NUM s2) {
    return isRelationshipTransitive(s1, s2);
}
