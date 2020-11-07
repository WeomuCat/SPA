#include "PrintTable.h"

INDEX PrintTable::addPrintNode(PRINT_NODE_PTR node) {
    return addNode(node);
}

PRINT_NODE_PTR_LIST PrintTable::getPrintNodeList() {
    return getNodeList();
}

STMT_NUM_LIST PrintTable::getPrintNumList() {
    return stmt_num_list_;
}

VAR_NAME_LIST PrintTable::getPrintVarNameList() {
    return var_name_list_;
}

void PrintTable::preCompute() {
    STMT_NUM_LIST result;
    for (PRINT_NODE_PTR n : nodes_) {
        STMT_NUM s = n.get()->getStatementNumber();
        result.push_back(s);
    }
    stmt_num_list_ = result;

    VAR_NAME_LIST result_2;
    for (PRINT_NODE_PTR n : nodes_) {
        VAR_NAME v = n.get()->getVariableNode()->getVariableName();
        result_2.push_back(v);
    }
    var_name_list_ = result_2;
}
