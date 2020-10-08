#pragma once

class StatementNode;

#include <memory>
#include "ASTNode.h"
#include "StatementListNode.h"

typedef std::shared_ptr<StatementListNode> STMT_LIST_NODE_PTR;
typedef bool BOOLEAN_TYPE;
typedef int STMT_NUM;

class StatementNode : public ASTNode {

public:

	/*
	Description: Constructs a StatementNode with NULL value for its STMT_NUM.
	*/
	StatementNode();

	/*
	Description: Returns True if able to set a STMT_NUM of this StatementNode.
	*/
	BOOLEAN_TYPE setStatementNumber(STMT_NUM stmt_num);

	/*
	Description: Returns True if able to set the STMT_LIST_NODE_PTR that contains this StatementNode.
	*/
	BOOLEAN_TYPE setStatementListNode(STMT_LIST_NODE_PTR stmt_list_node_ptr);

	/*
	Description: Returns the STMT_NUM of this StatementNode.
	*/
	STMT_NUM getStatementNumber();

	/*
	Description: Returns the STMT_LIST_NODE_PTR that contains this StatementNode.
	*/
	STMT_LIST_NODE_PTR getParentStatementListNode();

private:
	STMT_NUM stmt_num_;
	STMT_LIST_NODE_PTR stmt_list_node_ptr_;

};