#include "ConstantNode.h"

ConstantNode::ConstantNode() {
    node_type_ = { NODE_TYPE::constantNode };
    value_ = "";
}

BOOLEAN_TYPE ConstantNode::setValue(CONSTANT_VALUE value) {
    try {
        value_ = value;
    }
    catch (int e) {
        (void)e;
        return false;
    }
    return true;
}

CONSTANT_VALUE ConstantNode::getValue() {
    return value_;
}
