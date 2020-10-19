#pragma once

#include "../../AbstractDataTypes.h"
#include "ConditionNode.h"
#include "StatementListNode.h"
#include "StatementNode.h"

class WhileNode : public StatementNode, public std::enable_shared_from_this<WhileNode> {
public:

	/*
	Description: Sets NODE_TYPE of node to be assignNode.
	*/
	WhileNode();

	/*
	Description: Returns True if able to set the CONDITION_NODE_PTR of this while statement, false otherwise.
	*/
	BOOLEAN_TYPE setConditionNode(CONDITION_NODE_PTR condition_node_ptr);

	/*
	Description: Returns True if able to set stmt_list_node_ptr_ as stmt_list_node_ptr, false otherwise.
	*/
	BOOLEAN_TYPE setWhileStatementListNode(STMT_LIST_NODE_PTR stmt_list_node_ptr);

	/*
	Description: Returns the CODITION_NODE_PTR of this while statement.
	*/
	CONDITION_NODE_PTR getConditionNode();

	/*
	Description: Returns the then child STMT_LIST_NODE_PTR of this while statement
	*/
	STMT_LIST_NODE_PTR getWhileStatementListNode();

private:
	CONDITION_NODE_PTR condition_node_ptr_;
	STMT_LIST_NODE_PTR while_stmt_list_node_ptr_;

};
