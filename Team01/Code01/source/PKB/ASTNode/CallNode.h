#pragma once

#include <memory>
#include <string>
#include "CallNode.h"
#include "StatementNode.h"

typedef std::shared_ptr<ProcedureNode> PROC_NODE_PTR;
typedef std::string PROC_NAME;
typedef bool BOOLEAN_TYPE;

class CallNode: public StatementNode, public std::enable_shared_from_this<CallNode> {

public:
	CallNode();
	/*
	Description: Sets NODE_TYPE of node to be callNode.
	*/

	BOOLEAN_TYPE setCallerProcedureName(PROC_NAME proc_name);
	/*
	Description: Returns True if able to set the PROC_NAME of the procedure that the call statement lies in, false otherwise.
	*/

	BOOLEAN_TYPE setCalleeProcedureName(PROC_NAME proc_name);
	/*
	Description: Returns True if able to set the PROC_NAME of the procedure that the call statement is calling, false otherwise.
	*/

	BOOLEAN_TYPE setProcedureNode(PROC_NODE_PTR proc_node_ptr);
	/*
	Description: Returns True if able to set the non-child PROC_NODE_PTR of the procedure that the call statement is calling, false otherwise.
	*/

	PROC_NAME getCallerProcedureName();
	/*
	Description: Returns the PROC_NAME of the procedure that the call statement lies in.
	*/

	PROC_NAME getCalleeProcedureName();
	/*
	Description: Returns the PROC_NAME of the procedure that the call statement is calling.
	*/

	PROC_NODE_PTR getProcedureNode();
	/*
	Description: Returns the PROC_NODE_PTR of the procedure that the call statement is calling.
	*/

private:
	PROC_NAME caller_proc_name_;
	PROC_NAME callee_proc_name_;
	PROC_NODE_PTR proc_node_ptr_;

};
