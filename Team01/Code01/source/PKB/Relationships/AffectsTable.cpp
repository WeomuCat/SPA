#include "AffectsTable.h"

void AffectsTable::setModifiesTable(MODIFIES_TABLE modifies_table) {
    modifies_table_ = modifies_table;
}

void AffectsTable::setUsesTable(USES_TABLE uses_table) {
    uses_table_ = uses_table;
}

void AffectsTable::setControlFlowGraph(CFG cfg) {
    cfg_ = cfg;
}

void AffectsTable::setStatementTypeTable(STMT_TYPE_TABLE stmt_type_table) {
    stmt_type_table_ = stmt_type_table;
}

BOOLEAN_TYPE AffectsTable::isAffects(STMT_NUM s1, STMT_NUM s2) {
    // Ensure s1 is an assignment statement
    bool is_type = stmt_type_table_.isAssignStatement(s1);
    if (!is_type) {
        return false;
    }

    // Get LHS of s1
    VAR_NAME var_name = modifies_table_.getModifiedVariables(s1).at(0);

    // Ensure s2 is an assignment statement
    is_type = stmt_type_table_.isAssignStatement(s2);
    if (!is_type) {
        return false;
    }

    // Ensure s2 uses s1's variable
    if (!uses_table_.isUses(s2, var_name)) {
        return false;
    }

    // Do BFS
    std::queue<STMT_NUM> queue;
    std::unordered_set<STMT_NUM> discovered;
    queue.push(s1);

    while (!queue.empty()) {
        STMT_NUM s = queue.front();
        queue.pop();

        // Continue if t has no children statements
        auto iter = cfg_.find(s);
        if (iter == cfg_.end()) {
            continue;
        }

        // Iterate children of t and add to queue
        for (STMT_NUM c : iter->second) {
            // Continue if c has been discovered
            if (discovered.count(c) > 0) {
                continue;
            }
            
            // Goal Test
            if (c == s2) {
                return true;
            }

            // If c is an assign, read or call, and it Modifies var_name
            // skip c
            bool is_type = stmt_type_table_.isAssignStatement(c) || stmt_type_table_.isReadStatement(c) || stmt_type_table_.isCallStatement(c);
            if (is_type && modifies_table_.isModifies(c, var_name)) {
                discovered.insert(c);
                continue;
            }

            queue.push(c);
            discovered.insert(c);
        }
    }

    return false;
}

BOOLEAN_TYPE AffectsTable::isAffectsTransitive(STMT_NUM s1, STMT_NUM s2) {
    // Do BFS
    std::queue<STMT_NUM> queue;
    std::unordered_set<STMT_NUM> discovered;
    queue.push(s1);

    while (!queue.empty()) {
        STMT_NUM s = queue.front();
        queue.pop();

        // Continue if t has no children statements
        auto affected = getAffectedList(s);
        if (affected.empty()) {
            continue;
        }

        // Iterate children of t and add to queue
        for (STMT_NUM c : affected) {
            // Continue if c has been discovered
            if (discovered.count(c) > 0) {
                continue;
            }
            
            // Goal Test
            if (c == s2) {
                return true;
            }

            queue.push(c);
            discovered.insert(c);
        }
    }

    return false;
}

STMT_NUM_LIST AffectsTable::getAffectedList(STMT_NUM start) {
    STMT_NUM_LIST result;

    // Ensure start is an assignment statement
    bool is_type = stmt_type_table_.isAssignStatement(start);
    if (!is_type) {
        return result;
    }

    // Get LHS of assign statement
    VAR_NAME var_name = modifies_table_.getModifiedVariables(start).at(0);

    // Do BFS
    std::queue<STMT_NUM> queue;
    std::unordered_set<STMT_NUM> discovered;
    queue.push(start);

    while (!queue.empty()) {
        STMT_NUM s = queue.front();
        queue.pop();

        // Continue if t has no children statements
        auto iter = cfg_.find(s);
        if (iter == cfg_.end()) {
            continue;
        }

        // Iterate children of t and add to queue
        for (STMT_NUM c : iter->second) {
            // Continue if c has been discovered
            if (discovered.count(c) > 0) {
                continue;
            }

            // If c is an assign stmt, and it Uses var_name
            // Add c to results!
            bool is_type = stmt_type_table_.isAssignStatement(c);
            if (is_type && uses_table_.isUses(c, var_name)) {
                result.push_back(c);
            }

            // If c is an assign, read or call, and it Modifies var_name
            // skip c
            is_type = stmt_type_table_.isAssignStatement(c) || stmt_type_table_.isReadStatement(c) || stmt_type_table_.isCallStatement(c);
            if (is_type && modifies_table_.isModifies(c, var_name)) {
                discovered.insert(c);
                continue;
            }

            queue.push(c);
            discovered.insert(c);
        }
    }

    return result;
}
