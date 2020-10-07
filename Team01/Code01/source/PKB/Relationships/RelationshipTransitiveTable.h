#pragma once

#include <unordered_map>
#include <unordered_set>
#include "../Hashing.h"

typedef bool BOOLEAN_TYPE;

template<typename T>
class RelationshipTransitiveTable {
    /* Overview: This is a data structure that can store transitive relationships */

public:
    void clearCache();
    /*
    Description: Clears the cache of this table.
    */

protected:
    void addRelationship(T t1, T t2);
    /*
    Description: Stores the relationship Relationship(t1, t2) in the table.
    */

    BOOLEAN_TYPE isRelationship(T t1, T t2);
    /*
    Description: Returns a BOOLEAN_TYPE indicating whether or not Relationship(t1, t2) holds.
    */

    BOOLEAN_TYPE isRelationshipTransitive(T t1, T t2);
    /*
    Description: Returns a BOOLEAN_TYPE indicating whether or not Relationship*(t1, t2) holds.
    */

private:
    std::unordered_map<T, std::vector<T>> children_;
    std::unordered_set<std::pair<T, T>> relationship_;
    std::unordered_map<std::pair<T, T>, BOOLEAN_TYPE> relationship_transitive_;
};
