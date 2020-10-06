#pragma once

#include <unordered_map>
#include <unordered_set>
#include "../Hashing.h"

typedef bool BOOLEAN_TYPE;
typedef int STMT_NUM;

// TODO: Might need to change STMT_NUM to PROG_NUM
class NextTable {
    /* Overview: This is a data structure*/

private:
    friend class PKB;
    friend class PKBBuilder;

    void addNext(STMT_NUM s1, STMT_NUM s2);
    /*
    Description: Stores the relationship Next(s1, s2) in the table.
    */

    BOOLEAN_TYPE isNext(STMT_NUM s1, STMT_NUM s2);
    /*
    Description: Returns a BOOLEAN_TYPE indicating whether or not Next(s1, s2) holds.
    */

    BOOLEAN_TYPE isNextTransitive(STMT_NUM s1, STMT_NUM s2);
    /*
    Description: Returns a BOOLEAN_TYPE indicating whether or not Next*(s1, s2) holds.
    */

    std::unordered_map<STMT_NUM, STMT_NUM> statements_;
    std::unordered_set<std::pair<STMT_NUM, STMT_NUM>> next_;
    std::unordered_map<std::pair<STMT_NUM, STMT_NUM>, BOOLEAN_TYPE> next_transitive_;

};
