#pragma once

#include <memory>
#include <vector>
#include "DesignEntityTable.h"
#include "DesignEntityTable.cpp"
#include "../ASTNode/AssignNode.h"

typedef std::shared_ptr<AssignNode> ASSIGN_NODE_PTR;
typedef std::vector<ASSIGN_NODE_PTR> ASSIGN_NODE_PTR_LIST;
typedef std::vector<STMT_NUM> STMT_NUM_LIST;

class AssignTable : public DesignEntityTable<ASSIGN_NODE_PTR> {

public:
    /*
    Description: Returns the index of the ASSIGN_NODE_PTR added to the table.
    */
    INDEX addAssignNode(ASSIGN_NODE_PTR node);

    /*
    Description: Returns a ASSIGN_NODE_PTR_LIST of the table.
    */
    ASSIGN_NODE_PTR_LIST getAssignNodeList();

    /*
    Description: Returns a STMT_NUM_LIST of the table.
    */
    STMT_NUM_LIST getAssignNumList();
};
