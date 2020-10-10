#pragma once

#include "../../AbstractDataTypes.h"

template<typename T>
class DesignEntityTable {
    /* Overview: This is a data structure that can store design entities */

protected:
    /*
    Description: Returns the index of the NODE_PTR added to the table.
    */
    INDEX addNode(T node);

    /*
    Description: Returns a NODE_PTR_LIST of the table.
    */
    std::vector<T> getNodeList();

    std::vector<T> nodes_;
};
