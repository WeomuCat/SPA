#pragma once

#include <string>
#include "StatementNode.h"

typedef std::string VAR_NAME;
typedef bool BOOLEAN_TYPE;

class VariableNode : public ASTNode {

public:
	VariableNode();
		/*
		Description: Sets NODE_TYPE of node to be variableNode.
		*/

	BOOLEAN_TYPE setVariableName(VAR_NAME var_name);
		/*
		Description: Returns True if able to set var_name_ as var_name, false otherwise.
		*/

	VAR_NAME getVariableName();
		/*
		Description: Returns a STRING of the variable's name
		*/

private:
	VAR_NAME var_name_;

};
