#include "TryTable.h"

INDEX TryTable::addTryNode(TRY_NODE_PTR node) {
    return addNode(node);
}

TRY_NODE_PTR_LIST TryTable::getTryNodeList() {
    return getNodeList();
}

STMT_NUM_LIST TryTable::getTryNumList() {
    std::vector<STMT_NUM> result;
    for (TRY_NODE_PTR n : nodes_) {
        STMT_NUM s = n.get()->getStatementNumber();
        result.push_back(s);
    }
    return result;
}
