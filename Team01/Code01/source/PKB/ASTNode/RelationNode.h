#pragma once

#include "../../AbstractDataTypes.h"
#include "ASTNode.h"
#include "RelationTypeEnum.h"

class RelationNode : public ASTNode, public std::enable_shared_from_this<RelationNode> {
/*RelationNode is ==, !=, <=, >=, <, >*/

public:
	
	/*
	Description: Sets NODE_TYPE of node to be relationNode.
	*/
	RelationNode();

	/*
	Description: Returns True if able to set the RELATION_TYPE of this relation, false otherwise.
	*/
	BOOLEAN_TYPE setRelationType(RELATION_TYPE relation_type);

	/*
	Description: Returns True if able to set AST_NODE_PTR to be the left-hand side child of this Relation, false otherwise
				 The AST_NODE_PTR must be of type: CONSTANT_NODE_PTR, VARIABLE_NODE_PTR
	*/
	BOOLEAN_TYPE setLeftAstNode(AST_NODE_PTR left_node_ptr);

	/*
	Description: Returns True if able to set AST_NODE_PTR to be the right-hand side child of this Relation, false otherwise.
				 The AST_NODE_PTR must be of type: CONSTANT_NODE_PTR, VARIABLE_NODE_PTR
	*/
	BOOLEAN_TYPE setRightAstNode(AST_NODE_PTR right_node_ptr);

	/*
	Description: Returns the RELATION_TYPE of this ExpressionNode
	*/
	RELATION_TYPE getRelationType();

	/*
	Description: Returns the left-hand side child AST_NODE_PTR of this Relation.
	*/
	AST_NODE_PTR getLeftAstNode();

	/*
	Description: Returns the right-hand side child AST_NODE_PTR of this Relation.
	*/
	AST_NODE_PTR getRightAstNode();


private:
	RELATION_TYPE relation_type_;
	AST_NODE_PTR left_node_ptr_;
	AST_NODE_PTR right_node_ptr_;

};
