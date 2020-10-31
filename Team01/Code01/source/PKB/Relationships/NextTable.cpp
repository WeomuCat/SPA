#include "NextTable.h"

void NextTable::addNext(STMT_NUM s1, STMT_NUM s2) {
    cfg_[s1].push_back(s2);
    relationship_.insert({ s1, s2 });
}


BOOLEAN_TYPE NextTable::isNext(STMT_NUM s1, STMT_NUM s2) {
    return relationship_.count({ s1, s2 }) > 0;
}

BOOLEAN_TYPE NextTable::isNextTransitive(STMT_NUM s1, STMT_NUM s2) {
    std::queue<STMT_NUM> queue;
    std::unordered_set<STMT_NUM> discovered;
    queue.push(s1);

    while (!queue.empty()) {
        STMT_NUM s = queue.front();
        queue.pop();

        // Continue if t has no children statements
        auto it2 = cfg_.find(s);
        if (it2 == cfg_.end()) {
            continue;
        }

        // Iterate children of t1 and add to queue
        for (STMT_NUM c : it2->second) {
            // Goal was found!
            if (c == s2) {
                return true;
            }
            // Continue if c has been discovered
            if (discovered.count(c) > 0) {
                continue;
            }
            queue.push(c);
            discovered.insert(c);
        }
    }

    return false;
}

CFG NextTable::getControlFlowGraph() {
    return cfg_;
}