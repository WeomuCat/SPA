#include "CallTable.h"

INDEX CallTable::addCallNode(CALL_NODE_PTR node) {
    INDEX index = nodes_.size();
    nodes_.push_back(node);
    return index;
}

CALL_NODE_PTR_LIST CallTable::getCallNodeList() {
    return CALL_NODE_PTR_LIST(nodes_);
}

STMT_NUM_LIST CallTable::getCallNumList() {
    std::vector<STMT_NUM> result;
    for (CALL_NODE_PTR n : nodes_) {
        STMT_NUM s = n.get()->getStatementNumber();
        result.push_back(s);
    }
    return result;
}
