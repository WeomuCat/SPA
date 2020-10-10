#pragma once

#include <memory>
#include <vector>
#include "../ASTNode/AssignNode.h"

typedef int INDEX;
typedef std::shared_ptr<AssignNode> ASSIGN_NODE_PTR;
typedef std::vector<ASSIGN_NODE_PTR> ASSIGN_NODE_PTR_LIST;
typedef std::vector<STMT_NUM> STMT_NUM_LIST;

class AssignTable {

private:
    friend class PKB;
    friend class PKBBuilder;

    /*
    Description: Returns the index of the ASSIGN_NODE_PTR that was stored into the table.
    */
    INDEX addAssignNode(ASSIGN_NODE_PTR node);

    /*
    Description: Returns an ASSIGN_NODE_PTR_LIST from the table.
    */
    ASSIGN_NODE_PTR_LIST getAssignNodeList();

    /*
    Description: Returns a STMT_NUM_LIST of the ASSIGN_NODE_PTR from the table.
    */
    STMT_NUM_LIST getAssignNumList();

    std::vector<ASSIGN_NODE_PTR> nodes_;

};
