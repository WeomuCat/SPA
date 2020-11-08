#include "CallTable.h"

INDEX CallTable::addCallNode(CALL_NODE_PTR node) {
    return addNode(node);
}

CALL_NODE_PTR_LIST CallTable::getCallNodeList() {
    return getNodeList();
}

STMT_NUM_LIST CallTable::getCallNumList() {
    return stmt_num_list_;
}

PROC_NAME_LIST CallTable::getCallProcNameList() {
    return proc_name_list_;
}

void CallTable::preCompute() {
    std::vector<STMT_NUM> result;
    for (CALL_NODE_PTR n : nodes_) {
        STMT_NUM s = n.get()->getStatementNumber();
        result.push_back(s);
    }
    stmt_num_list_ = result;

    PROC_NAME_LIST result_2;
    for (CALL_NODE_PTR n : nodes_) {
        PROC_NAME p = n.get()->getCalleeProcedureName();
        result_2.push_back(p);
    }
    proc_name_list_ = result_2;
}

