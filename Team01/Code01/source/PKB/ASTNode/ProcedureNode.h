#pragma once

#include <memory>
#include <string>
#include "StatementListNode.h"
#include "ASTNode.h"

typedef std::shared_ptr<StatementListNode> STMT_LIST_NODE_PTR;
typedef std::string STRING;
typedef bool BOOLEAN_TYPE;

class ProcedureNode : public ASTNode, public std::enable_shared_from_this<ProcedureNode> {

public:

	/*
	Description: Sets NODE_TYPE of node to be procedureNode.
	*/
	ProcedureNode();

	/*
	Description: Returns True if able to set STRING of the procedure's name, false otherwise.
	*/
	BOOLEAN_TYPE setProcedureName(STRING proc_name);

	/*
	Returns True if able to set the child STMT_LIST_NODE_PTR of the procedure, false otherwise.
	*/
	BOOLEAN_TYPE setProcedureStatementListNode(STMT_LIST_NODE_PTR stmt_list_node_ptr);

	/*
	Description: Returns the STRING of the procedure's name
	*/
	STRING getProcedureName();

	/*
	Description: Returns the child STMT_LIST_NODE_POINTER of the procedure.
	*/
	STMT_LIST_NODE_PTR getProcedureStatementListNode();

private:
	STRING proc_name_;
	STMT_LIST_NODE_PTR proc_stmt_list_node_ptr_;

};
