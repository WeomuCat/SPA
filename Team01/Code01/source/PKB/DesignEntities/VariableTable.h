#pragma once

#include "../../AbstractDataTypes.h"
#include "DesignEntityTable.h"
#include "DesignEntityTable.cpp"
#include "../ASTNode/VariableNode.h"

class VariableTable : public DesignEntityTable<VAR_NODE_PTR> {

public:
    /*
    Description: Returns the index of the VAR_NODE_PTR added to the table.
    */
    INDEX addVariableNode(VAR_NODE_PTR node);

    /*
    Description: Returns an VAR_NODE_PTR_LIST from the table.
    */
    VAR_NODE_PTR_LIST getVariableNodeList();

    /*
    Description: Returns a VAR_NAME_LIST of the VAR_NODE_PTR from the table.
    */
    VAR_NAME_LIST getVariableNameList();
};
