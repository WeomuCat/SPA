#pragma once

#include <typeinfo>
#include <exception>

#include "../../AbstractDataTypes.h"
#include "RelationNode.h"
#include "StatementNode.h"
#include "ConditionTypeEnum.h"


class ConditionNode : public ASTNode, public std::enable_shared_from_this<ConditionNode> {

public:
	/*
	Description: Sets NODE_TYPE of node to be conditionNode.
	*/
	ConditionNode();

	/*
	Description: Returns True if able to set the CONDITION_TYPE of this ConditionNode, false otherwise.
	*/
	BOOLEAN_TYPE setConditionType(CONDITION_TYPE condition_type);

	/*
	Description: Returns True if able to set AST_NODE_PTR to be the left-hand side child of this Condition, false otherwise
				 The AST_NODE_PTR must be of type: CONDITION_NODE_PTR, RELATION_NODE_PTR
	*/
	BOOLEAN_TYPE setLeftAstNode(AST_NODE_PTR left_node_ptr);

	/*
	Description: Returns True if able to set AST_NODE_PTR to be the right-hand side child of this Condition, false otherwise.
				 The AST_NODE_PTR must be of type: CONDITION_NODE_PTR, RELATION_NODE_PTR
	*/
	BOOLEAN_TYPE setRightAstNode(AST_NODE_PTR right_node_ptr);

	/*
	Description: Returns the CONDITION_TYPE of this ConditionNode
	*/
	CONDITION_TYPE getConditionType();

	/*
	Description: Returns the left-hand side child AST_NODE_PTR of this Condition.
	*/
	AST_NODE_PTR getLeftAstNode();

	/*
	Description: Returns the right-hand side child AST_NODE_PTR of this Condition.
	*/
	AST_NODE_PTR getRightAstNode();

private:
	CONDITION_TYPE condition_type_;
	AST_NODE_PTR left_node_ptr_;
	AST_NODE_PTR right_node_ptr_;

};
