#include "VariableNode.h"

VariableNode::VariableNode() {
	node_type_ = { NODE_TYPE::variableNode };
	var_name_ = "";
}

BOOLEAN_TYPE VariableNode::setVariableName(VAR_NAME var_name) {
	try {
		var_name_ = var_name;
	} catch (int e) {
		(void)e;
		return false;
	}
	return true;
}

VAR_NAME VariableNode::getVariableName() {
	return var_name_;
}

