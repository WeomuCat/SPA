#include "ProcedureTable.h"

INDEX ProcedureTable::addProcedureNode(PROC_NODE_PTR node) {
    return addNode(node);
}

PROC_NODE_PTR_LIST ProcedureTable::getProcedureNodeList() {
    return getNodeList();
}

PROC_NAME_LIST ProcedureTable::getProcedureNameList() {
    return proc_name_list_;
}

void ProcedureTable::preCompute() {
    std::vector<PROC_NAME> result;
    for (PROC_NODE_PTR n : nodes_) {
        PROC_NAME p = n.get()->getProcedureName();
        result.push_back(p);
    }
    proc_name_list_ = result;
}