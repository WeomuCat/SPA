#pragma once

#include <memory>
#include <vector>
#include "DesignEntityTable.h"
#include "DesignEntityTable.cpp"
#include "../ASTNode/ReadNode.h"

typedef std::shared_ptr<ReadNode> READ_NODE_PTR;
typedef std::vector<READ_NODE_PTR> READ_NODE_PTR_LIST;
typedef std::vector<STMT_NUM> STMT_NUM_LIST;
typedef std::vector<VAR_NAME> VAR_NAME_LIST;

class ReadTable : public DesignEntityTable<READ_NODE_PTR> {

public:
    /*
    Description: Returns the index of the READ_NODE_PTR added to the table.
    */
    INDEX addReadNode(READ_NODE_PTR node);

    /*
    Description: Returns a READ_NODE_PTR_LIST of the table.
    */
    READ_NODE_PTR_LIST getReadNodeList();

    /*
    Description: Returns a STMT_NUM_LIST of the table.
    */
    STMT_NUM_LIST getReadNumList();

    /*
    Description: Returns a VAR_NAME_LIST of the table.
    */
    VAR_NAME_LIST getReadVarNameList();
};
