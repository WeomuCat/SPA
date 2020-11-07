#include "ReadTable.h"

INDEX ReadTable::addReadNode(READ_NODE_PTR node) {
    return addNode(node);
}

READ_NODE_PTR_LIST ReadTable::getReadNodeList() {
    return getNodeList();
}

STMT_NUM_LIST ReadTable::getReadNumList() {
    return stmt_num_list_;
}

VAR_NAME_LIST ReadTable::getReadVarNameList() {
    return var_name_list_;
}

void ReadTable::preCompute() {
    STMT_NUM_LIST result;
    for (READ_NODE_PTR n : nodes_) {
        STMT_NUM s = n.get()->getStatementNumber();
        result.push_back(s);
    }
    stmt_num_list_ = result;

    VAR_NAME_LIST result_2;
    for (READ_NODE_PTR n : nodes_) {
        VAR_NAME v = n.get()->getVariableNode()->getVariableName();
        result_2.push_back(v);
    }
    var_name_list_ = result_2;
}