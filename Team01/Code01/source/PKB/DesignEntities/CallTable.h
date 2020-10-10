#pragma once

#include "../../AbstractDataTypes.h"
#include "DesignEntityTable.h"
#include "DesignEntityTable.cpp"
#include "../ASTNode/CallNode.h"

class CallTable : public DesignEntityTable<CALL_NODE_PTR> {

public:
    /*
    Description: Returns the index of the CALL_NODE_PTR that was stored into the table.
    */
    INDEX addCallNode(CALL_NODE_PTR node);

    /*
    Description: Returns a CALL_NODE_PTR_LIST of the table.
    */
    CALL_NODE_PTR_LIST getCallNodeList();

    /*
    Description: Returns a STMT_NUM_LIST of the CALL_NODE_PTR from the table.
    */
    STMT_NUM_LIST getCallNumList();
};
