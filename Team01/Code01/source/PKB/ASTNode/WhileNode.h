#pragma once

#include <memory>
#include "ConditionNode.h"
#include "StatementListNode.h"
#include "StatementNode.h"

typedef std::shared_ptr<ConditionNode> CONDITION_NODE_PTR;
typedef std::shared_ptr<StatementListNode> STMT_LIST_NODE_PTR;
typedef bool BOOLEAN_TYPE;

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
	Description: Returns the CONDITION_NODE_PTR of WhileNode condition.
	*/
	CONDITION_NODE_PTR getConditionNode();

	/*
	Description: Returns a STATEMENT_LIST_NODE_PTR of the while statement body.
	*/
	STMT_LIST_NODE_PTR getWhileStatementListNode();

private:
	CONDITION_NODE_PTR condition_node_ptr_;
	STMT_LIST_NODE_PTR while_stmt_list_node_ptr_;

};
