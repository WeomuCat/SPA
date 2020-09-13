#pragma once

#include <memory>
#include <vector>
#include "../ASTNode/StatementListNode.h"

typedef int INDEX;
typedef std::shared_ptr<StatementListNode> STMT_LIST_NODE_PTR;
typedef std::vector<STMT_LIST_NODE_PTR> STMT_LIST_NODE_PTR_LIST;

class StatementListTable {
    /* Overview: This is a data structure*/

public:
    INDEX addStatementListNode(STMT_LIST_NODE_PTR node);
    /*
    Description: Returns the index of the STMT_LIST_NODE_PTR added to the table.
    */

    STMT_LIST_NODE_PTR_LIST getStatementListNodeList();
    /*
    Description: Returns a STMT_LIST_NODE_PTR_LIST of the table.
    */

private:
    STMT_LIST_NODE_PTR_LIST table_;

};