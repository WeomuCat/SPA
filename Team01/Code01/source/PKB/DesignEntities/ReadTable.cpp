#include "ReadTable.h"

INDEX ReadTable::addReadNode(READ_NODE_PTR node) {
    return addNode(node);
}

READ_NODE_PTR_LIST ReadTable::getReadNodeList() {
    return getNodeList();
}

STMT_NUM_LIST ReadTable::getReadNumList() {
    STMT_NUM_LIST result;
    for (READ_NODE_PTR n : nodes_) {
        STMT_NUM s = n.get()->getStatementNumber();
        result.push_back(s);
    }
    return result;
}

VAR_NAME_LIST ReadTable::getReadVarNameList() {
    VAR_NAME_LIST result;
    for (READ_NODE_PTR n : nodes_) {
        VAR_NAME v = n.get()->getVariableNode()->getVariableName();
        result.push_back(v);
    }
    return result;
}
