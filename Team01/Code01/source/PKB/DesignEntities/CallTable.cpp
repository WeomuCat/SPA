#include "CallTable.h"

INDEX CallTable::addCallNode(CALL_NODE_PTR node) {
    return addNode(node);
}

CALL_NODE_PTR_LIST CallTable::getCallNodeList() {
    return getNodeList();
}

STMT_NUM_LIST CallTable::getCallNumList() {
    std::vector<STMT_NUM> result;
    for (CALL_NODE_PTR n : nodes_) {
        STMT_NUM s = n.get()->getStatementNumber();
        result.push_back(s);
    }
    return result;
}
