#pragma once

#include <queue>
#include <unordered_set>
#include "NonPrecomputedTransitiveRelationshipTable.h"

template<typename T>
void NonPrecomputedTransitiveRelationshipTable<T>::addRelationship(T t1, T t2) {
    relationship_.insert({ t1, t2 });
}

template<typename T>
BOOLEAN_TYPE NonPrecomputedTransitiveRelationshipTable<T>::isRelationship(T t1, T t2) {
    // Check if Relationship(t1, t2) exists
    return relationship_.count({ t1, t2 }) > 0;
}

template<typename T>
BOOLEAN_TYPE NonPrecomputedTransitiveRelationshipTable<T>::isRelationshipTransitive(T t1, T t2) {
    // Do BFS
    std::queue<T> queue;
    std::unordered_set<T> discovered;
    queue.push(t1);

    while (!queue.empty()) {
        T t = queue.front();
        queue.pop();

        // Continue if t has no children statements
        auto children = getChildren(t);
        if (children.empty()) {
            continue;
        }

        // Iterate children of t and add to queue
        for (T c : children) {
            // Continue if c has been discovered
            if (discovered.count(c) > 0) {
                continue;
            }

            // Goal Test
            if (c == t2) {
                return true;
            }

            queue.push(c);
            discovered.insert(c);
        }
    }

    return false;
}
