#pragma once

#include <memory>
#include <vector>
#include "DesignEntityTable.h"
#include "DesignEntityTable.cpp"
#include "../ASTNode/CallNode.h"

typedef std::shared_ptr<CallNode> CALL_NODE_PTR;
typedef std::vector<CALL_NODE_PTR> CALL_NODE_PTR_LIST;
typedef std::vector<STMT_NUM> STMT_NUM_LIST;

class CallTable : public DesignEntityTable<CALL_NODE_PTR> {

public:
    /*
    Description: Returns the index of the CALL_NODE_PTR added to the table.
    */
    INDEX addCallNode(CALL_NODE_PTR node);

    /*
    Description: Returns a CALL_NODE_PTR_LIST of the table.
    */
    CALL_NODE_PTR_LIST getCallNodeList();

    /*
    Description: Returns a STMT_NUM_LIST of the table.
    */
    STMT_NUM_LIST getCallNumList();
};
