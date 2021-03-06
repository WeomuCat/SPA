#pragma once

#include <unordered_map>
#include <unordered_set>

#include "../../AbstractDataTypes.h"
#include "../Hashing.h"

template<typename T>
class PrecomputedTransitiveRelationshipTable {
    /* Overview: This is a data structure that precomputes transitive relationships */

public:
    /*
    Description: Precompute the transitive relation of this table and store it.
    */
    void preComputeTransitive();

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

private:
    std::unordered_map<T, std::vector<T>> children_;
    std::unordered_set<std::pair<T, T>> relationship_;
    std::unordered_set<std::pair<T, T>> relationship_transitive_;
};
