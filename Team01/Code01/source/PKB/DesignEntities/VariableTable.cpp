#include <unordered_set>
#include "VariableTable.h"

INDEX VariableTable::addVariableNode(VAR_NODE_PTR node) {
    return addNode(node);
}

VAR_NODE_PTR_LIST VariableTable::getVariableNodeList() {
    return getNodeList();
}

VAR_NAME_LIST VariableTable::getVariableNameList() {
    return var_name_list_;
}

void VariableTable::preCompute() {
    std::unordered_set<VAR_NAME> set;
    for (VAR_NODE_PTR n : nodes_) {
        VAR_NAME v = n.get()->getVariableName();
        set.insert(v);
    }
    var_name_list_ = VAR_NAME_LIST(set.begin(), set.end());
}