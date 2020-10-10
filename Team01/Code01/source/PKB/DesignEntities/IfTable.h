#pragma once

#include <memory>
#include <vector>
#include "../ASTNode/IfNode.h"

typedef int INDEX;
typedef std::shared_ptr<IfNode> IF_NODE_PTR;
typedef std::vector<IF_NODE_PTR> IF_NODE_PTR_LIST;
typedef std::vector<STMT_NUM> STMT_NUM_LIST;

class IfTable {

private:
    friend class PKB;
    friend class PKBBuilder;

    /*
    Description: Returns the index of the IF_NODE_PTR that was stored into the table.
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

    std::vector<IF_NODE_PTR> nodes_;

};
