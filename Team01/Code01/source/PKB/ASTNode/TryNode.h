#pragma once

#include "../../AbstractDataTypes.h"
#include "StatementListNode.h"
#include "StatementNode.h"

class TryNode : public StatementNode, public std::enable_shared_from_this<StatementNode> {
public:

	/*
	Description: Sets NODE_TYPE of node to be TryNode.
	*/
	TryNode();

	/*
	Description: Returns True if able to set the try child
				 STMT_LIST_NODE_PTR of this try statement, false otherwise.
	*/
	BOOLEAN_TYPE setTryStatementListNode(STMT_LIST_NODE_PTR try_stmt_list_node_ptr);
	
	/*
	Description: Returns True if able to set the catch child
				 STMT_LIST_NODE_PTR of this try statement, false otherwise.
	*/
	BOOLEAN_TYPE setCatchStatementListNode(STMT_LIST_NODE_PTR catch_stmt_list_node_ptr);

	/*
	Description: Returns True if able to set the finally child
				 STMT_LIST_NODE_PTR of this try statement, false otherwise.
	*/
	BOOLEAN_TYPE setFinallyStatementListNode(STMT_LIST_NODE_PTR finally_stmt_list_node_ptr);

	/*
	Description: Returns the try child STMT_LIST_NODE_PTR of this try statement
	*/
	STMT_LIST_NODE_PTR getTryStatementListNode();

	/*
	Description: Returns the catch child STMT_LIST_NODE_PTR of this try statement
	*/
	STMT_LIST_NODE_PTR getCatchStatementListNode();

	/*
	Description: Returns the finally child STMT_LIST_NODE_PTR of this try statement
	*/
	STMT_LIST_NODE_PTR getFinallyStatementListNode();

private:
	STMT_LIST_NODE_PTR try_stmt_list_node_ptr_;
	STMT_LIST_NODE_PTR catch_stmt_list_node_ptr_;
	STMT_LIST_NODE_PTR finally_stmt_list_node_ptr_;

};
