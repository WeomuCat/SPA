#pragma once

#include "../../AbstractDataTypes.h"
#include "DesignEntityTable.h"
#include "DesignEntityTable.cpp"
#include "../ASTNode/IfNode.h"

class IfTable : public DesignEntityTable<IF_NODE_PTR> {

public:
    /*
    Description: Returns the index of the IF_NODE_PTR added to the table.
    */
    INDEX addIfNode(IF_NODE_PTR node);

    /*
    Description: Returns an IF_NODE_PTR_LIST from the table.
    */
    IF_NODE_PTR_LIST getIfNodeList();

    /*
    Description: Returns a STMT_NUM_LIST of the IF_NODE_PTR from the table.
    */
    STMT_NUM_LIST getIfNumList();
};
