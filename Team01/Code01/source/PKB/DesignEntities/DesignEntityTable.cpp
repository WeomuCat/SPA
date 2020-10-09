#pragma once

#include "DesignEntityTable.h"

template<typename T>
INDEX DesignEntityTable<T>::addNode(T node) {
    INDEX index = nodes_.size();
    nodes_.push_back(node);
    return index;
}

template<typename T>
std::vector<T> DesignEntityTable<T>::getNodeList() {
    return std::vector<T>(nodes_);
}
