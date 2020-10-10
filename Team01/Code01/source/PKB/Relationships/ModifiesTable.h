#pragma once

#include <string>
#include <unordered_set>
#include "../Hashing.h"

typedef bool BOOLEAN_TYPE;
typedef int STMT_NUM;
typedef std::string PROC_NAME;
typedef std::string VAR_NAME;

class ModifiesTable {

private:
    friend class PKB;
    friend class PKBBuilder;

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


    std::unordered_set<std::pair<STMT_NUM, VAR_NAME>> modifies_s_;
    std::unordered_set<std::pair<PROC_NAME, VAR_NAME>> modifies_p_;

};
