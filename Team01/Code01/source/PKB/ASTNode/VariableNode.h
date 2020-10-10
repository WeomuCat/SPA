#pragma once

#include <string>
#include "StatementNode.h"

typedef std::string VAR_NAME;
typedef bool BOOLEAN_TYPE;

class VariableNode : public ASTNode {

public:

	/*
	Description: Sets NODE_TYPE of node to be variableNode.
	*/
	VariableNode();

	/*
	Description: Returns True if able to set the STRING of the variable name.
	*/
	BOOLEAN_TYPE setVariableName(VAR_NAME var_name);

	/*
	Description: Returns the VALUE of the STRING of the variable's name
	*/
	VAR_NAME getVariableName();

private:
	VAR_NAME var_name_;

};
