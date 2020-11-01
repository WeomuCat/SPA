#include "ModifiesTable.h"

void ModifiesTable::addModifies(STMT_NUM s, VAR_NAME v) {
    modifies_s_.insert({ s, v });
    modifies_s_children_[s].push_back(v);
}

void ModifiesTable::addModifies(PROC_NAME p, VAR_NAME v) {
    modifies_p_.insert({ p, v });
}

BOOLEAN_TYPE ModifiesTable::isModifies(STMT_NUM s, VAR_NAME v) {
    return modifies_s_.count({ s, v }) > 0;
}

BOOLEAN_TYPE ModifiesTable::isModifies(PROC_NAME p, VAR_NAME v) {
    return modifies_p_.count({ p, v }) > 0;
}

VAR_NAME_LIST ModifiesTable::getModifiedVariables(STMT_NUM s) {
    auto iter = modifies_s_children_.find(s);
    if (iter == modifies_s_children_.end()) {
        return {};
    }
    return iter->second;
}
