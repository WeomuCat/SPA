#pragma once

#include "../../AbstractDataTypes.h"
#include "DesignEntityTable.h"
#include "DesignEntityTable.cpp"
#include "../ASTNode/TryNode.h"

class TryTable : public DesignEntityTable<TRY_NODE_PTR> {

public:
    /*
    Description: Returns the index of the TRY_NODE_PTR added to the table.
    */
    INDEX addTryNode(TRY_NODE_PTR node);

    /*
    Description: Returns an TRY_NODE_PTR_LIST from the table.
    */
    TRY_NODE_PTR_LIST getTryNodeList();

    /*
    Description: Returns a STMT_NUM_LIST of the TRY_NODE_PTR from the table.
    */
    STMT_NUM_LIST getTryNumList();
};
