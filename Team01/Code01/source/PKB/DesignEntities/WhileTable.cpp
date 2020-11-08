#include "WhileTable.h"

INDEX WhileTable::addWhileNode(WHILE_NODE_PTR node) {
    return addNode(node);
}

WHILE_NODE_PTR_LIST WhileTable::getWhileNodeList() {
    return getNodeList();
}

STMT_NUM_LIST WhileTable::getWhileNumList() {
    return stmt_num_list_;
}

void WhileTable::preCompute() {
    std::vector<STMT_NUM> result;
    for (WHILE_NODE_PTR n : nodes_) {
        STMT_NUM s = n.get()->getStatementNumber();
        result.push_back(s);
    }
    stmt_num_list_ = result;
}
