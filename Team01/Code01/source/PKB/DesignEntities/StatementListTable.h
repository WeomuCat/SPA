#pragma once

#include <memory>
#include <vector>
#include "../ASTNode/StatementListNode.h"

typedef int INDEX;
typedef std::shared_ptr<StatementListNode> STMT_LIST_NODE_PTR;
typedef std::vector<STMT_LIST_NODE_PTR> STMT_LIST_NODE_PTR_LIST;

class StatementListTable {

private:
    friend class PKB;
    friend class PKBBuilder;

    /*
    Description: Returns the index of the PRINT_NODE_PTR that was stored into the table.
    */
    INDEX addStatementListNode(STMT_LIST_NODE_PTR node);

    /*
    Description: Returns a STMT_LIST_NODE_PTR_LIST from the table.
    */
    STMT_LIST_NODE_PTR_LIST getStatementListNodeList();

    STMT_LIST_NODE_PTR_LIST nodes_;

};
