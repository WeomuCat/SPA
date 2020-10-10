#include "WhileTable.h"

INDEX WhileTable::addWhileNode(WHILE_NODE_PTR node) {
    return addNode(node);
}

WHILE_NODE_PTR_LIST WhileTable::getWhileNodeList() {
    return getNodeList();
}

STMT_NUM_LIST WhileTable::getWhileNumList() {
    std::vector<STMT_NUM> result;
    for (WHILE_NODE_PTR n : nodes_) {
        STMT_NUM s = n.get()->getStatementNumber();
        result.push_back(s);
    }
    return result;
}
