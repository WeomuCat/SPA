#pragma once

#include <memory>
#include <vector>
#include "DesignEntityTable.h"
#include "DesignEntityTable.cpp"
#include "../ASTNode/StatementListNode.h"

typedef std::shared_ptr<StatementListNode> STMT_LIST_NODE_PTR;
typedef std::vector<STMT_LIST_NODE_PTR> STMT_LIST_NODE_PTR_LIST;

class StatementListTable : public DesignEntityTable<STMT_LIST_NODE_PTR> {

public:
    /*
    Description: Returns the index of the STMT_LIST_NODE_PTR added to the table.
    */
    INDEX addStatementListNode(STMT_LIST_NODE_PTR node);

    /*
    Description: Returns a STMT_LIST_NODE_PTR_LIST of the table.
    */
    STMT_LIST_NODE_PTR_LIST getStatementListNodeList();
};
