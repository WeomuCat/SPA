#pragma once

#include <memory>
#include "ConditionNode.h"
#include "StatementListNode.h"
#include "StatementNode.h"

typedef std::shared_ptr<ConditionNode> CONDITION_NODE_PTR;
typedef std::shared_ptr<StatementListNode> STMT_LIST_NODE_PTR;
typedef bool BOOLEAN_TYPE;

class IfNode : public StatementNode, public std::enable_shared_from_this<StatementNode> {
public:

	/*
	Description: Sets NODE_TYPE of node to be ifNode.
	*/
	IfNode();

	/*
	Description: Returns True if able to set the
				 CONDITION_NODE_PTR of this if statement, false otherwise.
	*/
	BOOLEAN_TYPE setConditionNode(CONDITION_NODE_PTR condition_node_ptr);

	/*
	Description: Returns True if able to set the then child
				 STMT_LIST_NODE_PTR of this if statement, false otherwise.
	*/
	BOOLEAN_TYPE setThenStatementListNode(STMT_LIST_NODE_PTR then_stmt_list_node_ptr);

	/*
	Description: Returns True if able to set the else child
				 STMT_LIST_NODE_PTR of this if statement, false otherwise.
	*/
	BOOLEAN_TYPE setElseStatementListNode(STMT_LIST_NODE_PTR else_stmt_list_node_ptr);

	/*
	Description: Returns the CODITION_NODE_PTR of this if statement.
	*/
	CONDITION_NODE_PTR getConditionNode();

	/*
	Description: Returns the then child STMT_LIST_NODE_PTR of this if statement
	*/
	STMT_LIST_NODE_PTR getThenStatementListNode();

	/*
	Description: Returns the else child STMT_LIST_NODE_PTR of this if statement
	*/
	STMT_LIST_NODE_PTR getElseStatementListNode();

private:
	CONDITION_NODE_PTR condition_node_ptr_;
	STMT_LIST_NODE_PTR then_stmt_list_node_ptr_;
	STMT_LIST_NODE_PTR else_stmt_list_node_ptr_;

};
