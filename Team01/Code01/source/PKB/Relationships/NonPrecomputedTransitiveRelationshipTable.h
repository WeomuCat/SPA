#pragma once

#include <unordered_map>
#include <unordered_set>

#include "../../AbstractDataTypes.h"
#include "../Hashing.h"

template<typename T>
class NonPrecomputedTransitiveRelationshipTable {
    /* Overview: This is a data structure that does not precompute transitive relationships */

protected:
    /*
    Description: Stores the relationship Relationship(t1, t2) in the table.
    */
    void addRelationship(T t1, T t2);

    /*
    Description: Returns a BOOLEAN_TYPE indicating whether or not Relationship(t1, t2) holds.
    */
    BOOLEAN_TYPE isRelationship(T t1, T t2);

    /*
    Description: Returns a BOOLEAN_TYPE indicating whether or not Relationship*(t1, t2) holds.
    */
    BOOLEAN_TYPE isRelationshipTransitive(T t1, T t2);

    /*
    Description: Get the children of a specific node of this relationship.
    */
    virtual std::vector<T> getChildren(T t) = 0;

private:
    std::unordered_set<std::pair<T, T>> relationship_;
};
