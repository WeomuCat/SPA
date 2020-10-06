#include "CallNode.h"

CallNode::CallNode() {
	node_type_ = { NODE_TYPE::callNode };
	caller_proc_name_ = "";
	callee_proc_name_ = "";
	proc_node_ptr_ = nullptr;
}

BOOLEAN_TYPE CallNode::setCallerProcedureName(PROC_NAME proc_name) {
	try {
		caller_proc_name_ = proc_name;
	}
	catch (int e) {
		(void)e;
		return false;
	}
	return true;
}

BOOLEAN_TYPE CallNode::setCalleeProcedureName(PROC_NAME proc_name) {
	try {
		callee_proc_name_ = proc_name;
	}
	catch (int e) {
		(void)e;
		return false;
	}
	return true;
}

BOOLEAN_TYPE CallNode::setProcedureNode(PROC_NODE_PTR proc_node_ptr) {
	try {
		proc_node_ptr_ = proc_node_ptr;
	}
	catch (int e) {
		(void)e;
		return false;
	}
	return true;
}

PROC_NAME CallNode::getCallerProcedureName() {
	return caller_proc_name_;
}

PROC_NAME CallNode::getCalleeProcedureName() {
	return callee_proc_name_;
}

PROC_NODE_PTR CallNode::getProcedureNode() {
	return proc_node_ptr_;
}