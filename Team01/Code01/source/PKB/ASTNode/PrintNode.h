#pragma once

#include "../../AbstractDataTypes.h"
#include "StatementNode.h"
#include "PrintNode.h"
#include "VariableNode.h"

typedef std::string STRING;
typedef bool BOOLEAN_TYPE;

class PrintNode : public StatementNode, public std::enable_shared_from_this<PrintNode> {

public:
	/*
	Description: Sets NODE_TYPE of node to be printNode.
	*/
	PrintNode();

	/*
	Description: Returns True if able to set the child VAR_NODE_PTR of the print statement, false otherwise.
	*/
	BOOLEAN_TYPE setVariableNode(VAR_NODE_PTR var_node_ptr);

	/*
	Description: Returns the child VAR_NODE_PTR of the print statement.
	*/
	VAR_NODE_PTR getVariableNode();

private:
	VAR_NODE_PTR var_node_ptr_;

};
