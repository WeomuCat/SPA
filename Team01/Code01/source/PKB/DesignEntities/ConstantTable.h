#pragma once

#include <memory>
#include <string>
#include <vector>
#include "../ASTNode/ConstantNode.h"

typedef int INDEX;
typedef std::string CONSTANT_VALUE;
typedef std::shared_ptr<ConstantNode> CONSTANT_NODE_PTR;
typedef std::vector<CONSTANT_NODE_PTR> CONSTANT_NODE_PTR_LIST;
typedef std::vector<CONSTANT_VALUE> CONSTANT_VALUE_LIST;

class ConstantTable {

private:
    friend class PKB;
    friend class PKBBuilder;

    /*
    Description: Returns the index of the CONSTANT_NODE_PTR that was stored into the table.
    */
    INDEX addConstantNode(CONSTANT_NODE_PTR node);

    /*
    Description: Returns a CONSTANT_NODE_PTR_LIST from the table.
    */
    CONSTANT_NODE_PTR_LIST getConstantNodeList();

    /*
    Description: Returns a CONSTANT_VALUE_LIST of the CONSTANT_NODE_PTR from the table.
    */
    CONSTANT_VALUE_LIST getConstantValueList();

    std::vector<CONSTANT_NODE_PTR> nodes_;

};
