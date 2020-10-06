#include "ASTNode.h"

ASTNode::ASTNode() {
	node_type_ = { NODE_TYPE::undefined };
	parent_node_ptr_ = nullptr;
}

NODE_TYPE ASTNode::getNodeType() {
	return node_type_;
}

BOOLEAN_TYPE ASTNode::setParentNode(AST_NODE_PTR parent_node_ptr) {
	try {
		parent_node_ptr_ = parent_node_ptr;
	} catch (int e) {
		(void)e;
		return false;
	}
	return true;
}

BOOLEAN_TYPE ASTNode::addChildNode(AST_NODE_PTR child_node_ptr) {
	try {
		children_node_ptr_.push_back(child_node_ptr);
	} catch (int e) {
		(void)e;
		return false;
	}
	return true;
}

AST_NODE_PTR ASTNode::getParentNode() {
	return parent_node_ptr_;
}

AST_NODE_PTR_LIST ASTNode::getChildrenNode() {
	return children_node_ptr_;
}

