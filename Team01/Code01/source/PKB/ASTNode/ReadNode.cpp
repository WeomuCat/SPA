#include "ReadNode.h"

ReadNode::ReadNode() {
	node_type_ = { NODE_TYPE::readNode };
	var_node_ptr_ = nullptr;
}

BOOLEAN_TYPE ReadNode::setVariableNode(VAR_NODE_PTR var_node_ptr) {
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

VAR_NODE_PTR ReadNode::getVariableNode() {
	return var_node_ptr_;
}

