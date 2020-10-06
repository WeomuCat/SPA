#include "AssignNode.h"

AssignNode::AssignNode() {
	node_type_ = { NODE_TYPE::assignNode };
	var_node_ptr_ = nullptr;
	expr_node_ptr_ = nullptr;
}

BOOLEAN_TYPE AssignNode::setVariableNode(VAR_NODE_PTR var_node_ptr) {
	try {
		addChildNode(var_node_ptr);
		var_node_ptr->setParentNode(shared_from_this());
		var_node_ptr_ = var_node_ptr;
	} catch (int e) {
		(void)e;
		return false;
	}
	return true;
}

BOOLEAN_TYPE AssignNode::setExpressionNode(EXPR_NODE_PTR expr_node_ptr) {
    try {
		expr_node_ptr_ = expr_node_ptr;
		addChildNode(expr_node_ptr_);
		expr_node_ptr_->setParentNode(shared_from_this());
	} catch (int e) {
		(void)e;
		return false;
	}
	return true;
}

VAR_NODE_PTR AssignNode::getVariableNode() {
	return var_node_ptr_;
}


EXPR_NODE_PTR AssignNode::getExpressionNode() {
	return expr_node_ptr_;
}
