#pragma once

#include "../../AbstractDataTypes.h"
#include "DesignEntityTable.h"
#include "DesignEntityTable.cpp"
#include "../ASTNode/ReadNode.h"

class ReadTable : public DesignEntityTable<READ_NODE_PTR> {

public:
    /*
    Description: Returns the index of the READ_NODE_PTR added to the table.
    */
    INDEX addReadNode(READ_NODE_PTR node);

    /*
    Description: Returns a READ_NODE_PTR_LIST from the table.
    */
    READ_NODE_PTR_LIST getReadNodeList();

    /*
    Description: Returns a STMT_NUM_LIST of the READ_NODE_PTR from the table.
    */
    STMT_NUM_LIST getReadNumList();

    /*
    Description: Returns a VAR_NAME_LIST of the VAR_NODE_PTR of each READ_NODE_PTR from the table.
    */
    VAR_NAME_LIST getReadVarNameList();
};
