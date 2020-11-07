#pragma once

#include "../../AbstractDataTypes.h"
#include "DesignEntityTable.h"
#include "DesignEntityTable.cpp"
#include "../ASTNode/PrintNode.h"

class PrintTable : public DesignEntityTable<PRINT_NODE_PTR> {

public:
    /*
    Description: Returns the index of the PRINT_NODE_PTR that was stored into the table.
    */
    INDEX addPrintNode(PRINT_NODE_PTR node);

    /*
    Description: Returns a PRINT_NODE_PTR_LIST from the table.
    */
    PRINT_NODE_PTR_LIST getPrintNodeList();

    /*
    Description: Returns a STMT_NUM_LIST of the PRINT_NODE_PTR from the table.
    */
    STMT_NUM_LIST getPrintNumList();

    /*
    Description: Returns a VAR_NAME_LIST of the VAR_NODE_PTR of each PRINT_NODE_PTR from the table.
    */
    VAR_NAME_LIST getPrintVarNameList();

    /*
    Description: Precomputes the necessary information within the table.
    */
    void preCompute();

private:
    STMT_NUM_LIST stmt_num_list_;
    VAR_NAME_LIST var_name_list_;
};
