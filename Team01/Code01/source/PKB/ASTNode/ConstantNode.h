#pragma once
#include <string>


#include "ASTNode.h"

typedef std::string VALUE;
typedef bool BOOLEAN_TYPE;

class ConstantNode : public ASTNode {

public:

	/*
	Description: Sets NODE_TYPE of node to be constantNode.
	*/
	ConstantNode();

	/*
	Description: Returns True if able to set the VALUE of the Constant in this ConstantNode
	*/
	BOOLEAN_TYPE setValue(VALUE value);

	/*
	Description: Returns the VALUE of the Constant in this ConstantNode
	*/
	VALUE getValue();

private:
	VALUE value_;
};
