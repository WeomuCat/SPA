#pragma once

#include "RelationshipTransitiveTable.h"
#include "RelationshipTransitiveTable.cpp"

typedef std::string PROC_NAME;

class CallsTable : public RelationshipTransitiveTable<PROC_NAME> {
    /* Overview: This is a data structure */

public:
    void addCalls(PROC_NAME p1, PROC_NAME p2);
    /*
    Description: Stores the relationship Calls(p1, p2) in the table.
    */

    BOOLEAN_TYPE isCalls(PROC_NAME p1, PROC_NAME p2);
    /*
    Description: Returns a BOOLEAN_TYPE indicating whether or not Calls(p1, p2) holds.
    */

    BOOLEAN_TYPE isCallsTransitive(PROC_NAME p1, PROC_NAME p2);
    /*
    Description: Returns a BOOLEAN_TYPE indicating whether or not Calls*(p1, p2) holds.
    */
};
