#include "CallsTable.h"

void CallsTable::addCalls(PROC_NAME p1, PROC_NAME p2) {
    addRelationship(p1, p2);
}

BOOLEAN_TYPE CallsTable::isCalls(PROC_NAME p1, PROC_NAME p2) {
    return isRelationship(p1, p2);
}

BOOLEAN_TYPE CallsTable::isCallsTransitive(PROC_NAME p1, PROC_NAME p2) {
    return isRelationshipTransitive(p1, p2);
}
