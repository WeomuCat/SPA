#pragma once

#include "../../AbstractDataTypes.h"
#include "DesignEntityTable.h"
#include "DesignEntityTable.cpp"
#include "../ASTNode/StatementNode.h"

class StatementTable : public DesignEntityTable<STMT_NODE_PTR> {

public:
    /*
    Description: Returns the index of the STMT_NODE_PTR added to the table.
    */
    INDEX addStatementNode(STMT_NODE_PTR node);

    /*
    Description: Returns an STMT_NODE_PTR_LIST from the table.
    */
    STMT_NODE_PTR_LIST getStatementNodeList();
    
    /*
    Description: Returns a STMT_NUM_LIST of the STMT_NODE_PTR from the table.
    */
    STMT_NUM_LIST getStatementNumList();

    /*
    Description: Precomputes the necessary information within the table.
    */
        void preCompute();

private:
    STMT_NUM_LIST stmt_num_list_;
};
