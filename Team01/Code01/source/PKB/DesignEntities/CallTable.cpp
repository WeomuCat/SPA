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

PROC_NAME_LIST CallTable::getCallProcNameList() {
    PROC_NAME_LIST result;
    for (CALL_NODE_PTR n : nodes_) {
        VAR_NAME v = n.get()->getCalleeProcedureName();
        result.push_back(v);
    }
    return result;
}
