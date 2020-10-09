#include "StatementTable.h"

INDEX StatementTable::addStatementNode(STMT_NODE_PTR node) {
    return addNode(node);
}

STMT_NODE_PTR_LIST StatementTable::getStatementNodeList() {
    return getNodeList();
}

STMT_NUM_LIST StatementTable::getStatementNumList() {
    std::vector<STMT_NUM> result;
    for (STMT_NODE_PTR n : nodes_) {
        STMT_NUM s = n.get()->getStatementNumber();
        result.push_back(s);
    }
    return result;
}
