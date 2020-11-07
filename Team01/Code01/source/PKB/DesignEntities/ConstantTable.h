#pragma once

#include "../../AbstractDataTypes.h"
#include "DesignEntityTable.h"
#include "DesignEntityTable.cpp"
#include "../ASTNode/ConstantNode.h"

class ConstantTable : public DesignEntityTable<CONSTANT_NODE_PTR> {

public:
    /*
    Description: Returns the index of the CONSTANT_NODE_PTR added to the table.
    */
    INDEX addConstantNode(CONSTANT_NODE_PTR node);

    /*
    Description: Returns a CONSTANT_NODE_PTR_LIST from the table.
    */
    CONSTANT_NODE_PTR_LIST getConstantNodeList();

    /*
    Description: Returns a CONSTANT_VALUE_LIST of the CONSTANT_NODE_PTR from the table.
    */
    CONSTANT_VALUE_LIST getConstantValueList();

    /*
    Description: Precomputes the necessary information within the table.
    */
    void preCompute();

private:
    CONSTANT_VALUE_LIST constant_num_list_;
};
