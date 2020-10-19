#pragma once

#include "../../AbstractDataTypes.h"
#include "ExpressionNode.h"
#include "StatementNode.h"
#include "VariableNode.h"

class AssignNode : public StatementNode, public std::enable_shared_from_this<AssignNode> {

public:
	/*
	Description: Sets NODE_TYPE of node to be assignNode.
	*/
	AssignNode();

	/*
	Description: Returns True if able to set VAR_NODE_PTR as a child of this AssignNode, false otherwise
	*/
	BOOLEAN_TYPE setVariableNode(VAR_NODE_PTR var_node_ptr);

	/*
	Description: Returns True if able to set EXPR_NODE_PTR as a child of this AssignNode, false otherwise
	*/
	BOOLEAN_TYPE setExpressionNode(EXPR_NODE_PTR expr_node_ptr);

	/*
	Description: Returns the child VAR_NODE_PTR of this AssignNode
	*/
	VAR_NODE_PTR getVariableNode();

	/*
	Description: Returns the child EXPR_NODE_PTR of this AssignNode
	*/
	EXPR_NODE_PTR getExpressionNode();

private:
	VAR_NODE_PTR var_node_ptr_;
	EXPR_NODE_PTR expr_node_ptr_;

};
