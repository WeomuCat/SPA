#include "StatementTypeTable.h";

void StatementTypeTable::addStatement(STMT_NUM s, STMT_TYPE stmt_type) {
    stmt_type_table_.insert({ s, stmt_type });
}

BOOLEAN_TYPE StatementTypeTable::isAssignStatement(STMT_NUM s) {
    auto iter = stmt_type_table_.find(s);
    if (iter == stmt_type_table_.end()) {
        return false;
    }
    return iter->second == STMT_TYPE::assignStmt;
}

BOOLEAN_TYPE StatementTypeTable::isCallStatement(STMT_NUM s) {
    auto iter = stmt_type_table_.find(s);
    if (iter == stmt_type_table_.end()) {
        return false;
    }
    return iter->second == STMT_TYPE::callStmt;
}

BOOLEAN_TYPE StatementTypeTable::isIfStatement(STMT_NUM s) {
    auto iter = stmt_type_table_.find(s);
    if (iter == stmt_type_table_.end()) {
        return false;
    }
    return iter->second == STMT_TYPE::ifStmt;
}

BOOLEAN_TYPE StatementTypeTable::isPrintStatement(STMT_NUM s) {
    auto iter = stmt_type_table_.find(s);
    if (iter == stmt_type_table_.end()) {
        return false;
    }
    return iter->second == STMT_TYPE::printStmt;
}

BOOLEAN_TYPE StatementTypeTable::isReadStatement(STMT_NUM s) {
    auto iter = stmt_type_table_.find(s);
    if (iter == stmt_type_table_.end()) {
        return false;
    }
    return iter->second == STMT_TYPE::readStmt;
}

BOOLEAN_TYPE StatementTypeTable::isTryStatement(STMT_NUM s) {
    auto iter = stmt_type_table_.find(s);
    if (iter == stmt_type_table_.end()) {
        return false;
    }
    return iter->second == STMT_TYPE::tryStmt;
}

BOOLEAN_TYPE StatementTypeTable::isWhileStatement(STMT_NUM s) {
    auto iter = stmt_type_table_.find(s);
    if (iter == stmt_type_table_.end()) {
        return false;
    }
    return iter->second == STMT_TYPE::whileStmt;
}
