#include "IfNode.h"

IfNode::IfNode() {
    node_type_ = { NODE_TYPE::ifNode };
    condition_node_ptr_ = nullptr;
    then_stmt_list_node_ptr_ = nullptr;
    else_stmt_list_node_ptr_ = nullptr;
}

BOOLEAN_TYPE IfNode::setConditionNode(CONDITION_NODE_PTR condition_node_ptr) {
    try {
        addChildNode(condition_node_ptr);
        condition_node_ptr->setParentNode(shared_from_this());
        condition_node_ptr_ = condition_node_ptr;
    } catch (int e) {
        (void)e;
        return false;
    }
    return true;
}

BOOLEAN_TYPE IfNode::setThenStatementListNode(STMT_LIST_NODE_PTR then_stmt_list_node_ptr) {
    try {
        addChildNode(then_stmt_list_node_ptr);
        then_stmt_list_node_ptr->setParentNode(shared_from_this());
        then_stmt_list_node_ptr_ = then_stmt_list_node_ptr;
    } catch (int e) {
        (void)e;
        return false;
    }
    return true;
}

BOOLEAN_TYPE IfNode::setElseStatementListNode(STMT_LIST_NODE_PTR else_stmt_list_node_ptr) {
    try {
        addChildNode(else_stmt_list_node_ptr);
        else_stmt_list_node_ptr->setParentNode(shared_from_this());
        else_stmt_list_node_ptr_ = else_stmt_list_node_ptr;
    } catch (int e) {
        (void)e;
        return false;
    }
    return true;
}

CONDITION_NODE_PTR IfNode::getConditionNode() {
    return condition_node_ptr_;
}

STMT_LIST_NODE_PTR IfNode::getThenStatementListNode() {
    return then_stmt_list_node_ptr_;
}

STMT_LIST_NODE_PTR IfNode::getElseStatementListNode() {
    return else_stmt_list_node_ptr_;
}
