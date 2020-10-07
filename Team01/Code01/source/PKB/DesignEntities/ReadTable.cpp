#include "ReadTable.h"

INDEX ReadTable::addReadNode(READ_NODE_PTR node) {
    INDEX index = nodes_.size();
    nodes_.push_back(node);
    return index;
}

READ_NODE_PTR_LIST ReadTable::getReadNodeList() {
    return READ_NODE_PTR_LIST(nodes_);
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
