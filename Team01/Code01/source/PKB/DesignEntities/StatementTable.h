#pragma once

#include <memory>
#include <vector>
#include "../ASTNode/StatementNode.h"

typedef int INDEX;
typedef std::shared_ptr<StatementNode> STMT_NODE_PTR;
typedef std::vector<STMT_NODE_PTR> STMT_NODE_PTR_LIST;
typedef std::vector<STMT_NUM> STMT_NUM_LIST;

class StatementTable {

private:
    friend class PKB;
    friend class PKBBuilder;

    /*
    Description: Returns the index of the STMT_NODE_PTR that was stored into the table.
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

    std::vector<STMT_NODE_PTR> nodes_;

};
