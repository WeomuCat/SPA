#pragma once

#include <queue>
#include "RelationshipTransitiveTable.h"

template<typename T>
void RelationshipTransitiveTable<T>::clearCache() {
    relationship_transitive_.clear();
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
    // Do BFS
    std::queue<T> queue;
    std::vector<T> checked;
    std::unordered_set<T> discovered;
    queue.push(t1);

    while (!queue.empty()) {
        T t = queue.front();
        queue.pop();

        // Check if Relationship*(t, t2) exists, and return it's value
        auto it1 = relationship_transitive_.find({ t, t2 });
        if (it1 != relationship_transitive_.end()) {
            return it1->second;
        }

        // Continue if t has no children statements
        auto it2 = children_.find(t);
        if (it2 == children_.end()) {
            continue;
        }

        // Iterate children of t1 and add to queue and memoize
        for (T c : it2->second) {
            // Goal was found!
            if (c == t2) {
                return true;
            }
            // Continue if c has been discovered
            if (discovered.count(c) > 0) {
                continue;
            }
            queue.push(c);
            discovered.insert(c);
            relationship_transitive_.insert({ { t1, c }, true });
        }

        checked.push_back(t);
    }

    // Goal was not found...
    // Memoize invalid transitive relationships
    for (T c : checked) {
        relationship_transitive_.insert({ { c, t2 }, false });
    }
    return false;
}
