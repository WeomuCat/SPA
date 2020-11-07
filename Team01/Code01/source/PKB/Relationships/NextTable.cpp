#include "NextTable.h"

void NextTable::addNext(STMT_NUM s1, STMT_NUM s2) {
    cfg_[s1].push_back(s2);
    addRelationship(s1, s2);
}

BOOLEAN_TYPE NextTable::isNext(STMT_NUM s1, STMT_NUM s2) {
    return isRelationship(s1, s2);
}

BOOLEAN_TYPE NextTable::isNextTransitive(STMT_NUM s1, STMT_NUM s2) {
    return isRelationshipTransitive(s1, s2);
}

STMT_NUM_LIST NextTable::getChildren(STMT_NUM s) {
    auto iter = cfg_.find(s);
    if (iter == cfg_.end()) {
        return {};
    }
    return iter->second;
}

CFG NextTable::getControlFlowGraph() {
    return cfg_;
}
