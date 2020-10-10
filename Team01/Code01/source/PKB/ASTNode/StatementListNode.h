#pragma once

class StatementListNode;

#include <memory>
#include <vector>
#include "ASTNode.h"
#include "StatementNode.h"

typedef std::shared_ptr<StatementNode> STMT_NODE_PTR;
typedef std::vector<std::shared_ptr<StatementNode>> STMT_NODE_PTR_LIST;
typedef bool BOOLEAN_TYPE;

class StatementListNode : public ASTNode, public std::enable_shared_from_this<StatementListNode> {

public:

	/*
	Description: Sets NODE_TYPE of node to be statementListNode.
	*/
	StatementListNode();

	/*
	Description: Returns True if able to add a child STMT_NODE_PTR of the statement list, false otherwise.
	*/
	BOOLEAN_TYPE addStatementNode(STMT_NODE_PTR stmt_node_ptr);

	/*
	Description: Returns the list of children, STMT_NODE_PTR_LIST, of the statement list.
	*/
	STMT_NODE_PTR_LIST getStatementNodeList();


private:
	STMT_NODE_PTR_LIST stmt_node_ptr_list_;

};
