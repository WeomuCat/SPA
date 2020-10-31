#pragma once

#include "../../AbstractDataTypes.h"
#include "ModifiesTable.h"
#include "UsesTable.h"

class AffectsTable {

public:
    /*
    Description: Sets a copy of the MODIFIES_TABLE in the PKB as reference when calculating Affects relation.
    */
    void setModifiesTable(MODIFIES_TABLE modifies_table);

    /*
    Description: Sets a copy of the USES_TABLE in the PKB as reference when calculating Affects relation.
    */
    void setUsesTable(USES_TABLE uses_table);

    /*
    Description: Sets a reference to the CFG of the given source.
    */
    void setControlFlowGraph(CFG cfg);

    /*
    Description: Returns a BOOLEAN_TYPE indicating whether or not Affects(s1, s2) holds.
    */
    BOOLEAN_TYPE isAffects(STMT_NUM s1, STMT_NUM s2);

    /*
    Description: Returns a BOOLEAN_TYPE indicating whether or not Affects*(s1, s2) holds.
    */
    BOOLEAN_TYPE isAffectsTransitive(STMT_NUM s1, STMT_NUM s2);

private:
    MODIFIES_TABLE modifies_table_;
    USES_TABLE uses_table_;
    CFG cfg_;

};
