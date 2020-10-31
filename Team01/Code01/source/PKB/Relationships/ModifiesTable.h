#pragma once

#include <unordered_set>

#include "../../AbstractDataTypes.h"
#include "../Hashing.h"

class ModifiesTable {

public:
    /*
    Description: Adds to a collection that maps s to v.
    */
    void addModifies(STMT_NUM s, VAR_NAME v);

    /*
    Description: Adds to a collection that maps p to v.
    */
    void addModifies(PROC_NAME p, VAR_NAME v);

    /*
    Description: Returns a BOOLEAN_TYPE indicating whether or not Modifies(s, v) holds.
    */
    BOOLEAN_TYPE isModifies(STMT_NUM s, VAR_NAME v);

    /*
    Description: Returns a BOOLEAN_TYPE indicating whether or not Modifies(p, v) holds.
    */
    BOOLEAN_TYPE isModifies(PROC_NAME p, VAR_NAME v);

    /*
    Description: Returns a list of modified VAR_NAME of a STMT_NUM.
    */
    VAR_NAME_LIST getModifiedVariables(STMT_NUM s);

private:
    std::unordered_set<std::pair<STMT_NUM, VAR_NAME>> modifies_s_;
    std::unordered_set<std::pair<PROC_NAME, VAR_NAME>> modifies_p_;

    std::unordered_map<STMT_NUM, std::vector<VAR_NAME>> modifies_s_children_;
};
