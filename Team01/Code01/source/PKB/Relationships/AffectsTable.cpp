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

BOOLEAN_TYPE AffectsTable::isAffects(STMT_NUM s1, STMT_NUM s2) {
    // TODO: Add logic for Affects.
    return false;
}

BOOLEAN_TYPE AffectsTable::isAffectsTransitive(STMT_NUM s1, STMT_NUM s2) {
    // TODO: Add logic for Affects.
    return false;
}