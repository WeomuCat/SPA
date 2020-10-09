#include "PrintTable.h"

INDEX PrintTable::addPrintNode(PRINT_NODE_PTR node) {
    return addNode(node);
}

PRINT_NODE_PTR_LIST PrintTable::getPrintNodeList() {
    return getNodeList();
}

STMT_NUM_LIST PrintTable::getPrintNumList() {
    STMT_NUM_LIST result;
    for (PRINT_NODE_PTR n : nodes_) {
        STMT_NUM s = n.get()->getStatementNumber();
        result.push_back(s);
    }
    return result;
}

VAR_NAME_LIST PrintTable::getPrintVarNameList() {
    VAR_NAME_LIST result;
    for (PRINT_NODE_PTR n : nodes_) {
        VAR_NAME v = n.get()->getVariableNode()->getVariableName();
        result.push_back(v);
    }
    return result;
}
