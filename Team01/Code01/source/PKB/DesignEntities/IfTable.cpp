#include "IfTable.h"

INDEX IfTable::addIfNode(IF_NODE_PTR node) {
    return addNode(node);
}

IF_NODE_PTR_LIST IfTable::getIfNodeList() {
    return getNodeList();
}

STMT_NUM_LIST IfTable::getIfNumList() {
    return stmt_num_list_;
}

void IfTable::preCompute() {
    std::vector<STMT_NUM> result;
    for (IF_NODE_PTR n : nodes_) {
        STMT_NUM s = n.get()->getStatementNumber();
        result.push_back(s);
    }
    stmt_num_list_ = result;
}