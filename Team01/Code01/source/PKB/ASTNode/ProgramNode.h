#pragma once

#include <memory>
#include <string>
#include <vector>
#include "ProcedureNode.h"
#include "ASTNode.h"

typedef bool BOOLEAN_TYPE;
typedef std::shared_ptr<ProcedureNode> PROC_NODE_PTR;
typedef std::vector<PROC_NODE_PTR> PROC_NODE_PTR_LIST;

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
