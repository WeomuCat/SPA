#pragma once

#include <memory>
#include <vector>
#include "DesignEntityTable.h"
#include "DesignEntityTable.cpp"
#include "../ASTNode/StatementNode.h"

typedef std::shared_ptr<StatementNode> STMT_NODE_PTR;
typedef std::vector<STMT_NODE_PTR> STMT_NODE_PTR_LIST;
typedef std::vector<STMT_NUM> STMT_NUM_LIST;

class StatementTable : public DesignEntityTable<STMT_NODE_PTR> {

public:
    /*
    Description: Returns the index of the STMT_NODE_PTR added to the table.
    */
    INDEX addStatementNode(STMT_NODE_PTR node);

    /*
    Description: Returns a STMT_NODE_PTR_LIST of the table.
    */
    STMT_NODE_PTR_LIST getStatementNodeList();
    
    /*
    Description: Returns a STMT_NUM_LIST of the table.
    */
    STMT_NUM_LIST getStatementNumList();
};
