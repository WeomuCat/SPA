#include <algorithm>
#include "CallsTable.h"

// TODO: Check if this works

void CallsTable::addCalls(PROC_NAME p1, PROC_NAME p2) {
    procedures_[p1] = p2;
    calls_.insert({ p1, p2 });
    calls_transitive_.insert({ { p1, p2 }, true });
}

BOOLEAN_TYPE CallsTable::isCalls(PROC_NAME p1, PROC_NAME p2) {
    // Check if Calls(p1, p2) exists
    return calls_.count({ p1, p2 }) > 0;
}

BOOLEAN_TYPE CallsTable::isCallsTransitive(PROC_NAME p1, PROC_NAME p2) {
    // TODO: Algorithm is wrong. Please edit to make it right. Or do unit test case.
    // Check if Calls*(s1, s2) exists, and return it's value
    auto it1 = calls_transitive_.find({ p1, p2 });
    if (it1 != calls_transitive_.end()) {
        return it1->second;
    }

    // Check if s1 has a follower
    auto it2 = procedures_.find(p1);
    if (it2 == procedures_.end()) {
        return false;
    }

    // Check if caller is transitive
    // Memoize transitive relationships :)
    if (isCallsTransitive(it2->second, p2)) {
        calls_transitive_.insert({ { p1, p2 }, true });
        return true;
    }
    calls_transitive_.insert({ { p1, p2 }, false });
    return false;
}
