#pragma once

#include <memory>
#include <string>
#include "ASTNode.h"
#include "ExpressionTypeEnum.h"

typedef std::shared_ptr<ASTNode> AST_NODE_PTR;
typedef ExpressionTypeEnum EXPR_TYPE;
typedef bool BOOLEAN_TYPE;

class ExpressionNode : public ASTNode, public std::enable_shared_from_this<ExpressionNode> {
/*ExpressionNode is +, -, *, / or none*/

public:

	/*
	Description: Sets NODE_TYPE of node to be expressionNode.
	*/
	ExpressionNode();

	/*
	Description: Returns True if able to set the EXPR_TYPE of this ExpressionNode, false otherwise.
				 The AST_NODE_PTR must be of type: CONSTANT_NODE_PTR, VARIABLE_NODE_PTR, EXPR_NOPDE_PTR
	*/
	BOOLEAN_TYPE setExpressionType(EXPR_TYPE expr_type);

	/*
	Description: Returns True if able to set AST_NODE_PTR to be the left-hand side child of this Expression, false otherwise
				 The AST_NODE_PTR must be of type: CONSTANT_NODE_PTR, VARIABLE_NODE_PTR, EXPR_NOPDE_PTR
	*/
	BOOLEAN_TYPE setLeftAstNode(AST_NODE_PTR left_node_ptr);

	/*
	Description: Returns True if able to set AST_NODE_PTR to be the right-hand side
				 child of this Condition, false otherwise.
	*/
	BOOLEAN_TYPE setRightAstNode(AST_NODE_PTR right_node_ptr);

	/*
	Description: Returns the EXPR_TYPE of this expression.
	*/
	EXPR_TYPE getExpressionType();

	/*
	Description: Returns the left-hand side child AST_NODE_PTR of this Expression.
	*/
	AST_NODE_PTR getLeftAstNode();
	
	/*
	Description: Returns the right-hand side child AST_NODE_PTR of this Expression.
	*/
	AST_NODE_PTR getRightAstNode();

private:
	EXPR_TYPE expr_type_;
	AST_NODE_PTR left_node_ptr_;
	AST_NODE_PTR right_node_ptr_;

};
