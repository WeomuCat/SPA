#pragma once

#include <memory>
#include <vector>
#include "../ASTNode/CallNode.h"

typedef int INDEX;
typedef std::shared_ptr<CallNode> CALL_NODE_PTR;
typedef std::vector<CALL_NODE_PTR> CALL_NODE_PTR_LIST;
typedef std::vector<STMT_NUM> STMT_NUM_LIST;

class CallTable {

private:
    friend class PKB;
    friend class PKBBuilder;

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

    std::vector<CALL_NODE_PTR> nodes_;

};
