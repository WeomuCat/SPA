#include "TryNode.h"

TryNode::TryNode() {
    node_type_ = { NODE_TYPE::tryNode };
    try_stmt_list_node_ptr_ = nullptr;
    catch_stmt_list_node_ptr_ = nullptr;
    finally_stmt_list_node_ptr_ = nullptr;
}

BOOLEAN_TYPE TryNode::setTryStatementListNode(STMT_LIST_NODE_PTR try_stmt_list_node_ptr) {
    try {
        addChildNode(try_stmt_list_node_ptr);
        try_stmt_list_node_ptr->setParentNode(shared_from_this());
        try_stmt_list_node_ptr_ = try_stmt_list_node_ptr;
    }
    catch (int e) {
        (void)e;
        return false;
    }
    return true;
}

BOOLEAN_TYPE TryNode::setCatchStatementListNode(STMT_LIST_NODE_PTR catch_stmt_list_node_ptr) {
    try {
        addChildNode(catch_stmt_list_node_ptr);
        catch_stmt_list_node_ptr->setParentNode(shared_from_this());
        catch_stmt_list_node_ptr_ = catch_stmt_list_node_ptr;
    }
    catch (int e) {
        (void)e;
        return false;
    }
    return true;
}

BOOLEAN_TYPE TryNode::setFinallyStatementListNode(STMT_LIST_NODE_PTR finally_stmt_list_node_ptr) {
    try {
        addChildNode(finally_stmt_list_node_ptr);
        finally_stmt_list_node_ptr->setParentNode(shared_from_this());
        finally_stmt_list_node_ptr_ = finally_stmt_list_node_ptr;
    }
    catch (int e) {
        (void)e;
        return false;
    }
    return true;
}

STMT_LIST_NODE_PTR TryNode::getTryStatementListNode() {
    return try_stmt_list_node_ptr_;
}

STMT_LIST_NODE_PTR TryNode::getCatchStatementListNode() {
    return catch_stmt_list_node_ptr_;
}

STMT_LIST_NODE_PTR TryNode::getFinallyStatementListNode() {
    return finally_stmt_list_node_ptr_;
}
