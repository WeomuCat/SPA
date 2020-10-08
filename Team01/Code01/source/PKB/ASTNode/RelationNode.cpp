#include "RelationNode.h"

RelationNode::RelationNode() {
    relation_type_ = { RELATION_TYPE::undefined };
    node_type_ = { NODE_TYPE::relationNode };
    left_node_ptr_ = nullptr;
    right_node_ptr_ = nullptr;
}

BOOLEAN_TYPE RelationNode::setRelationType(RELATION_TYPE relation_type) {
    try {
        relation_type_ = relation_type;
    } catch (int e) {
        (void)e;
        return false;
    }
    return true;
}

BOOLEAN_TYPE RelationNode::setLeftAstNode(AST_NODE_PTR left_node_ptr) {
    try {
        if (left_node_ptr->getNodeType() != NODE_TYPE::variableNode &&
            left_node_ptr->getNodeType() != NODE_TYPE::constantNode &&
            left_node_ptr->getNodeType() != NODE_TYPE::expressionNode) {
            return false;
        }
        addChildNode(left_node_ptr);
        left_node_ptr->setParentNode(shared_from_this());
        left_node_ptr_ = left_node_ptr;
    } catch (int e) {
        (void)e;
        return false;
    }
    return true;
}

BOOLEAN_TYPE RelationNode::setRightAstNode(AST_NODE_PTR right_node_ptr) {
    try {
        if (right_node_ptr->getNodeType() != NODE_TYPE::variableNode &&
            right_node_ptr->getNodeType() != NODE_TYPE::constantNode &&
            right_node_ptr->getNodeType() != NODE_TYPE::expressionNode) {
            return false;
        }
        addChildNode(right_node_ptr);
        right_node_ptr->setParentNode(shared_from_this());
        right_node_ptr_ = right_node_ptr;
    } catch (int e) {
        (void)e;
        return false;
    }
    return true;
}

RELATION_TYPE RelationNode::getRelationType() {
    return relation_type_;
}

AST_NODE_PTR RelationNode::getLeftAstNode() {
    return left_node_ptr_;
}

AST_NODE_PTR RelationNode::getRightAstNode() {
    return right_node_ptr_;
}
