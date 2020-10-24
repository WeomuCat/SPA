#pragma once

#include "../../AbstractDataTypes.h"
#include "NodeTypeEnum.h"

class ASTNode {

public:
	
	/*
	Description: Sets NODE_TYPE to be undefined.
	*/
	ASTNode();

	/*
	Description: Returns the parent AST_NODE_PTR of this ASTNode
				 The Parent AST_NODE_PTR is automatically set when setting this node as the child.
	*/
	AST_NODE_PTR getParentNode();

	/*
	Description: Returns the children AST_NODE_PTR_LIST of this ASTNode
				 A child AST_NODE_PTR is automatically added when setting the child nodes of a respective NODE of a particular type.
	*/
	AST_NODE_PTR_LIST getChildrenNode();

	/*
	Description: Returns the Sub Class's NODE_TYPE of this ASTNode.
	*/
	NODE_TYPE getNodeType();

private:
	friend class AssignNode;
	friend class CallNode;
	friend class ConditionNode;
	friend class ConstantNode;
	friend class ExpressionNode;
	friend class IfNode;
	friend class PrintNode;
	friend class ProcedureNode;
	friend class ProgramNode;
	friend class ReadNode;
	friend class RelationNode;
	friend class StatementListNode;
	friend class TryNode;
	friend class VariableNode;
	friend class WhileNode;

	/*
	Description: Returns True if able to set AST_NODE_PTR as a parent of this node, false otherwise
	*/
	BOOLEAN_TYPE setParentNode(AST_NODE_PTR parent_node_ptr);

	/*
	Description: Returns True if able to set AST_NODE_PTR as a child of this node, false otherwise
	*/
	BOOLEAN_TYPE addChildNode(AST_NODE_PTR child_node_ptr);

	NODE_TYPE node_type_;
	AST_NODE_PTR parent_node_ptr_;
	AST_NODE_PTR_LIST children_node_ptr_;
};
