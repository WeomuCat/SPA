#include "StatementListTable.h"

INDEX StatementListTable::addStatementListNode(STMT_LIST_NODE_PTR node) {
    return addNode(node);
}

STMT_LIST_NODE_PTR_LIST StatementListTable::getStatementListNodeList() {
    return getNodeList();
}
