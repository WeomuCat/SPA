#include <algorithm>
#include "NextTable.h"

// TODO: Check this

void NextTable::addNext(STMT_NUM s1, STMT_NUM s2) {
    statements_[s1] = s2;
    next_.insert({ s1, s2 });
    next_transitive_.insert({ { s1, s2 }, true });
}

BOOLEAN_TYPE NextTable::isNext(STMT_NUM s1, STMT_NUM s2) {
    // Check if Next(s1, s2) exists
    return next_.count({ s1, s2 }) > 0;
}

BOOLEAN_TYPE NextTable::isNextTransitive(STMT_NUM s1, STMT_NUM s2) {
    // TODO: Algorithm is wrong. Please edit to make it right. Or do unit test case.
    // Check if Next*(s1, s2) exists, and return it's value
    auto it1 = next_transitive_.find({ s1, s2 });
    if (it1 != next_transitive_.end()) {
        return it1->second;
    }

    // Check if s1 has a follower
    auto it2 = statements_.find(s1);
    if (it2 == statements_.end()) {
        return false;
    }

    // Check if follower is transitive
    // Memoize transitive relationships :)
    if (isNextTransitive(it2->second, s2)) {
        next_transitive_.insert({ { s1, s2 }, true });
        return true;
    }
    next_transitive_.insert({ { s1, s2 }, false });
    return false;
}
