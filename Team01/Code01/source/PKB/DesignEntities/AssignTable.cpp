#include "AssignTable.h"

INDEX AssignTable::addAssignNode(ASSIGN_NODE_PTR node) {
    STMT_NUM s = node->getStatementNumber();
    return addNode(node);
}

ASSIGN_NODE_PTR_LIST AssignTable::getAssignNodeList() {
    return getNodeList();
}

STMT_NUM_LIST AssignTable::getAssignNumList() {
    std::vector<STMT_NUM> result;
    for (ASSIGN_NODE_PTR n : nodes_) {
        STMT_NUM s = n.get()->getStatementNumber();
        result.push_back(s);
    }
    return result;
}
