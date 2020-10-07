#pragma once

#include <string>
#include <unordered_set>
#include "../Hashing.h"

typedef bool BOOLEAN_TYPE;
typedef int STMT_NUM;
typedef std::string PROC_NAME;
typedef std::string VAR_NAME;

class UsesTable {

private:
    friend class PKB;
    friend class PKBBuilder;

    /*
    Description: Adds to a collection that maps s to v.
    */
    void addUses(STMT_NUM s, VAR_NAME v);

    /*
    Description: Adds to a collection that maps p to v.
    */
    void addUses(PROC_NAME p, VAR_NAME v);

    /*
    Description: Returns a BOOLEAN_TYPE indicating whether or not Uses(s, v) holds.
    */
    BOOLEAN_TYPE isUses(STMT_NUM s, VAR_NAME v);

    /*
    Description: Returns a BOOLEAN_TYPE indicating whether or not Uses(p, v) holds.
    */
    BOOLEAN_TYPE isUses(PROC_NAME p, VAR_NAME v);

    std::unordered_set<std::pair<STMT_NUM, VAR_NAME>> uses_s_;
    std::unordered_set<std::pair<PROC_NAME, VAR_NAME>> uses_p_;

};
