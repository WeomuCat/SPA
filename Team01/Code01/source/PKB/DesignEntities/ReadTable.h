#pragma once

#include <memory>
#include <vector>
#include "../ASTNode/ReadNode.h"

typedef int INDEX;
typedef std::shared_ptr<ReadNode> READ_NODE_PTR;
typedef std::vector<READ_NODE_PTR> READ_NODE_PTR_LIST;
typedef std::vector<STMT_NUM> STMT_NUM_LIST;
typedef std::vector<VAR_NAME> VAR_NAME_LIST;

class ReadTable {

private:
    friend class PKB;
    friend class PKBBuilder;

    /*
    Description: Returns the index of the PRINT_NODE_PTR that was stored into the table.
    */
    INDEX addReadNode(READ_NODE_PTR node);

    /*
    Description: Returns a READ_NODE_PTR_LIST from the table.
    */
    READ_NODE_PTR_LIST getReadNodeList();

    /*
    Description: Returns a STMT_NUM_LIST of the READ_NODE_PTR from the table.
    */
    STMT_NUM_LIST getReadNumList();

    /*
    Description: Returns a VAR_NAME_LIST of the VAR_NODE_PTR of each READ_NODE_PTR from the table.
    */
    VAR_NAME_LIST getReadVarNameList();

    std::vector<READ_NODE_PTR> nodes_;

};
