#include "AssignTable.h"

INDEX AssignTable::addAssignNode(ASSIGN_NODE_PTR node) {
    return addNode(node);
}

ASSIGN_NODE_PTR_LIST AssignTable::getAssignNodeList() {
    return getNodeList();
}

STMT_NUM_LIST AssignTable::getAssignNumList() {
    return stmt_num_list_;
}

void AssignTable::preCompute() {
    std::vector<STMT_NUM> result;
    for (ASSIGN_NODE_PTR n : nodes_) {
        STMT_NUM s = n.get()->getStatementNumber();
        result.push_back(s);
    }
    stmt_num_list_ = result;
}
