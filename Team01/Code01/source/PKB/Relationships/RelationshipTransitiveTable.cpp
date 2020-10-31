#pragma once

#include <queue>
#include <unordered_set>
#include "RelationshipTransitiveTable.h"

template<typename T>
void RelationshipTransitiveTable<T>::preComputeTransitive() {
    // For each node, do a BFS to all reachable nodes
    // Add transitive relationship from start to all reachable nodes
    for (auto it1 : children_) {
        T start = it1.first;

        // Do BFS
        std::queue<T> queue;
        std::unordered_set<T> discovered;
        queue.push(start);

        while (!queue.empty()) {
            T t = queue.front();
            queue.pop();

            // Continue if t has no children statements
            auto it2 = children_.find(t);
            if (it2 == children_.end()) {
                continue;
            }

            // Iterate children of t1 and add to queue and memoize
            for (T c : it2->second) {
                // Continue if c has been discovered
                if (discovered.count(c) > 0) {
                    continue;
                }
                queue.push(c);
                discovered.insert(c);
                relationship_transitive_.insert({ start, c });
            }
        }
    }
}

template<typename T>
void RelationshipTransitiveTable<T>::addRelationship(T t1, T t2) {
    children_[t1].push_back(t2);
    relationship_.insert({ t1, t2 });
}

template<typename T>
BOOLEAN_TYPE RelationshipTransitiveTable<T>::isRelationship(T t1, T t2) {
    // Check if Relationship(t1, t2) exists
    return relationship_.count({ t1, t2 }) > 0;
}


template<typename T>
BOOLEAN_TYPE RelationshipTransitiveTable<T>::isRelationshipTransitive(T t1, T t2) {
    // Check if RelationshipT(t1, t2) exists
    return relationship_transitive_.count({ t1, t2 }) > 0;
}
