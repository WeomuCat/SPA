#pragma once

#include "../../AbstractDataTypes.h"
#include "ASTNode.h"
#include "ProcedureNode.h"

/*This class is the root node of the AST, also known as the program node*/
class ProgramNode: public ASTNode, public std::enable_shared_from_this<ProgramNode> {


public:

	/*
	Description: Constructs an empty ProgramNode.
	*/
	ProgramNode();

	/*
	Description: Returns True if able to add the PROC_NODE_PTR to the PROC_NODE_PTR_LIST, false otherwise.
	*/
	BOOLEAN_TYPE addProcedureNode(PROC_NODE_PTR proc_node_ptr);

	/*
	Description: Returns the PROC_NODE_PTR_LIST containing the list of PROC_NODE_PTR of the program.
	*/
	PROC_NODE_PTR_LIST getProcedureNodeList();

private:
	PROC_NODE_PTR_LIST proc_node_ptr_list_;

};
