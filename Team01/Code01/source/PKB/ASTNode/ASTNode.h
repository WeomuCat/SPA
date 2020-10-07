#pragma once

#include <memory>
#include <vector>
#include "NodeTypeEnum.h"

class ASTNode;

typedef std::shared_ptr<ASTNode> AST_NODE_PTR;
typedef std::vector<std::shared_ptr<ASTNode>> AST_NODE_PTR_LIST;
typedef bool BOOLEAN_TYPE;
typedef NodeTypeEnum NODE_TYPE;

class ASTNode {

public:
	ASTNode();
		/*
		Description: Sets NODE_TYPE to be undefined.
		*/

	AST_NODE_PTR getParentNode();
		/*
		Description: Returns the parent AST_NODE_PTR of this ASTNode
					 The Parent AST_NODE_PTR is automatically set when setting this node as the child.
		*/

	AST_NODE_PTR_LIST getChildrenNode();
		/*
		Description: Returns the children AST_NODE_PTR_LIST of this ASTNode
					 A child AST_NODE_PTR is automatically added when setting the child nodes of a respective NODE of a particular type.

		*/

	NODE_TYPE getNodeType();
		/*
		Description: Returns the Sub Class's NODE_TYPE of this ASTNode.
		*/

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
	friend class VariableNode;
	friend class WhileNode;

	BOOLEAN_TYPE setParentNode(AST_NODE_PTR parent_node_ptr);
		/*
		Description: Returns True if able to set AST_NODE_PTR as a parent of this node, false otherwise
		*/

	BOOLEAN_TYPE addChildNode(AST_NODE_PTR child_node_ptr);
		/*
		Description: Returns True if able to set AST_NODE_PTR as a child of this node, false otherwise
		*/

	NODE_TYPE node_type_;
	AST_NODE_PTR parent_node_ptr_;
	AST_NODE_PTR_LIST children_node_ptr_;
};
