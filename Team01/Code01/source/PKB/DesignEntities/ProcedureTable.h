#pragma once

#include <memory>
#include <vector>
#include "DesignEntityTable.h"
#include "DesignEntityTable.cpp"
#include "../ASTNode/ProcedureNode.h"

typedef std::string PROC_NAME;
typedef std::shared_ptr<ProcedureNode> PROC_NODE_PTR;
typedef std::vector<PROC_NODE_PTR> PROC_NODE_PTR_LIST;
typedef std::vector<PROC_NAME> PROC_NAME_LIST;

class ProcedureTable : public DesignEntityTable<PROC_NODE_PTR> {

public:
    /*
    Description: Returns the index of the PROC_NODE_PTR added to the table.
    */
    INDEX addProcedureNode(PROC_NODE_PTR node);

    /*
    Description: Returns an PROC_NODE_PTR_LIST from the table.
    */
    PROC_NODE_PTR_LIST getProcedureNodeList();

    /*
    Description: Returns a PROC_NAME_LIST of the PROC_NODE_PTR from the table.
    */
    PROC_NAME_LIST getProcedureNameList();
};
