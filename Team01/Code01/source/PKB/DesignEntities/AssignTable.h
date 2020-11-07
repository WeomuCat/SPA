#pragma once

#include "../../AbstractDataTypes.h"
#include "DesignEntityTable.h"
#include "DesignEntityTable.cpp"
#include "../ASTNode/AssignNode.h"

class AssignTable : public DesignEntityTable<ASSIGN_NODE_PTR> {

public:
    /*
    Description: Returns the index of the ASSIGN_NODE_PTR added to the table.
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

    /*
    Description: Precomputes the necessary information within the table.
    */
    void preCompute();

private:
    STMT_NUM_LIST stmt_num_list_;
};
