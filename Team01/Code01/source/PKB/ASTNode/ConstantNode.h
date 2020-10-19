#pragma once

#include "../../AbstractDataTypes.h"
#include "ASTNode.h"

class ConstantNode : public ASTNode {

public:

	/*
	Description: Sets NODE_TYPE of node to be constantNode.
	*/
	ConstantNode();

	/*
	Description: Returns True if able to set the VALUE of the Constant in this ConstantNode
	*/
	BOOLEAN_TYPE setValue(CONSTANT_VALUE value);

	/*
	Description: Returns the VALUE of the Constant in this ConstantNode
	*/
	CONSTANT_VALUE getValue();

private:
	CONSTANT_VALUE value_;
};
