#pragma once

#include <queue>
#include <unordered_set>
#include "PrecomputedTransitiveRelationshipTable.h"

template<typename T>
void PrecomputedTransitiveRelationshipTable<T>::preComputeTransitive() {
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
            auto iter = children_.find(t);
            if (iter == children_.end()) {
                continue;
            }

            // Iterate children of t and add to queue
            for (T c : iter->second) {
                // Continue if c has been discovered
                if (discovered.count(c) > 0) {
                    continue;
                }

                relationship_transitive_.insert({ start, c });

                queue.push(c);
                discovered.insert(c);
            }
        }
    }
}

template<typename T>
void PrecomputedTransitiveRelationshipTable<T>::addRelationship(T t1, T t2) {
    children_[t1].push_back(t2);
    relationship_.insert({ t1, t2 });
}

template<typename T>
BOOLEAN_TYPE PrecomputedTransitiveRelationshipTable<T>::isRelationship(T t1, T t2) {
    // Check if Relationship(t1, t2) exists
    return relationship_.count({ t1, t2 }) > 0;
}


template<typename T>
BOOLEAN_TYPE PrecomputedTransitiveRelationshipTable<T>::isRelationshipTransitive(T t1, T t2) {
    // Check if RelationshipT(t1, t2) exists
    return relationship_transitive_.count({ t1, t2 }) > 0;
}
