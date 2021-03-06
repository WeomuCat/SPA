#pragma once

#include "../../AbstractDataTypes.h"
#include "StatementNode.h"
#include "VariableNode.h"

class ReadNode : public StatementNode, public std::enable_shared_from_this<ReadNode> {

public:
	
	/*
	Description: Sets NODE_TYPE of node to be readNode.
	*/
	ReadNode();

	/*
	Description: Returns True if able to set the child VAR_NODE_PTR of the read statement, false otherwise.
	*/
	BOOLEAN_TYPE setVariableNode(VAR_NODE_PTR var_node_ptr);

	/*
	Description: Returns the child VAR_NODE_PTR of the read statement.
	*/
	VAR_NODE_PTR getVariableNode();

private:
	VAR_NODE_PTR var_node_ptr_;

};
