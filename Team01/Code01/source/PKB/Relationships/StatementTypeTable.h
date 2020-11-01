#pragma once

#include <unordered_map>

#include "../../AbstractDataTypes.h"
#include "StatementTypeEnum.h"

class StatementTypeTable {

public:
    /*
    Description: Adds a statement
    */
    void addStatement(STMT_NUM s, STMT_TYPE stmt_type);

    /*
    Description: Returns a BOOLEAN_TYPE of whether the STMT_NUM is an assignment statment or not.
    */
    BOOLEAN_TYPE isAssignStatement(STMT_NUM s);

    /*
    Description: Returns a BOOLEAN_TYPE of whether the STMT_NUM is an call statment or not.
    */
    BOOLEAN_TYPE isCallStatement(STMT_NUM s);

    /*
    Description: Returns a BOOLEAN_TYPE of whether the STMT_NUM is an if statment or not.
    */
    BOOLEAN_TYPE isIfStatement(STMT_NUM s);

    /*
    Description: Returns a BOOLEAN_TYPE of whether the STMT_NUM is an print statment or not.
    */
    BOOLEAN_TYPE isPrintStatement(STMT_NUM s);

    /*
    Description: Returns a BOOLEAN_TYPE of whether the STMT_NUM is an read statment or not.
    */
    BOOLEAN_TYPE isReadStatement(STMT_NUM s);

    /*
    Description: Returns a BOOLEAN_TYPE of whether the STMT_NUM is an try statment or not.
    */
    BOOLEAN_TYPE isTryStatement(STMT_NUM s);

    /*
    Description: Returns a BOOLEAN_TYPE of whether the STMT_NUM is an while statment or not.
    */
    BOOLEAN_TYPE isWhileStatement(STMT_NUM s);

private:
    std::unordered_map<STMT_NUM, STMT_TYPE> stmt_type_table_;
};
