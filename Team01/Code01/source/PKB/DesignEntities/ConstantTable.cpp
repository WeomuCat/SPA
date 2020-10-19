#include <unordered_set>
#include "ConstantTable.h"

INDEX ConstantTable::addConstantNode(CONSTANT_NODE_PTR node) {
    return addNode(node);
}

CONSTANT_NODE_PTR_LIST ConstantTable::getConstantNodeList() {
    return getNodeList();
}

CONSTANT_VALUE_LIST ConstantTable::getConstantValueList() {
    std::unordered_set<CONSTANT_VALUE> set;
    for (CONSTANT_NODE_PTR n : nodes_) {
        CONSTANT_VALUE v = n.get()->getValue();
        set.insert(v);
    }
    return CONSTANT_VALUE_LIST(set.begin(), set.end());
}
