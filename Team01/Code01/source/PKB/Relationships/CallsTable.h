#pragma once

#include <unordered_map>
#include <unordered_set>
#include "../Hashing.h"

typedef bool BOOLEAN_TYPE;
typedef std::string PROC_NAME;

class CallsTable {
    /* Overview: This is a data structure*/

private:
    friend class PKB;
    friend class PKBBuilder;

    void addCalls(PROC_NAME p1, PROC_NAME p2);
    /*
    Description: Stores the relationship Calls(s1, s2) in the table.
    */

    BOOLEAN_TYPE isCalls(PROC_NAME p1, PROC_NAME p2);
    /*
    Description: Returns a BOOLEAN_TYPE indicating whether or not Calls(s1, s2) holds.
    */

    BOOLEAN_TYPE isCallsTransitive(PROC_NAME p1, PROC_NAME p2);
    /*
    Description: Returns a BOOLEAN_TYPE indicating whether or not Calls*(p1, p2) holds.
    */

    std::unordered_map<PROC_NAME, PROC_NAME> procedures_;
    std::unordered_set<std::pair<PROC_NAME, PROC_NAME>> calls_;
    std::unordered_map<std::pair<PROC_NAME, PROC_NAME>, BOOLEAN_TYPE> calls_transitive_;

};
