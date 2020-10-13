#pragma once

#include "../../AbstractDataTypes.h"
#include "DesignEntityTable.h"
#include "DesignEntityTable.cpp"
#include "../ASTNode/WhileNode.h"

class WhileTable : public DesignEntityTable<WHILE_NODE_PTR> {

public:
    /*
    Description: Returns the index of the WHILE_NODE_PTR added to the table.
    */
    INDEX addWhileNode(WHILE_NODE_PTR node);

    /*
    Description: Returns a WHILE_NODE_PTR_LIST from the table.
    */
    WHILE_NODE_PTR_LIST getWhileNodeList();

    /*
    Description: Returns a STMT_NUM_LIST of the WHILE_NODE_PTR from the table.
    */
    STMT_NUM_LIST getWhileNumList();
};
