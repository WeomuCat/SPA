#pragma once

#include "../AbstractDataTypes.h"
#include "PKB.h"

class PKBBuilder : public PKB {
/* 
Overview: A builder to aid in the construction of the PKB 
*/
public:
    /*==== Design Entities ====*/
    
    /*
    Description: Adds an ASSIGN_NODE_PTR to the ASSIGN_TABLE when building the PKB.
                 Returns the INDEX of the ASSIGN_NODE_PTR in the ASSIGN_TABLE.
    */
    INDEX addAssignNode(ASSIGN_NODE_PTR assign_node_ptr);
       
    /*
    Description: Adds a CALL_NODE_PTR to the CALL_TABLE when building the PKB.
                 Returns the INDEX of the CALL_NODE_PTR in the CALL_TABLE.
    */
    INDEX addCallNode(CALL_NODE_PTR call_node_ptr);

    /*
    Description: Adds a CONSTANT_NODE_PTR to the CONSTANT_TABLE when building the PKB.
                 Returns the INDEX of the CONSTANT_NODE_PTR in the CONSTANT_TABLE.
    */
    INDEX addConstantNode(CONSTANT_NODE_PTR constant_node_ptr);

    /*
    Description: Adds a IF_NODE_PTR to the IF_TABLE when building the PKB.
                 Returns the INDEX of the IF_NODE_PTR in the IF_TABLE.
    */
    INDEX addIfNode(IF_NODE_PTR if_node_ptr);

    /*
    Description: Adds a PRINT_NODE_PTR to the PRINT_TABLE when building the PKB.
                    Returns the INDEX of the PRINT_NODE_PTR in the PRINT_TABLE.
    */
    INDEX addPrintNode(PRINT_NODE_PTR print_node_ptr);

    /*
    Description: Adds a PROC_NAME to the PROC_TABLE when building the PKB.
                 Returns the INDEX of the PROC_NAME in the PROC_TABLE.
    */
    INDEX addProcedureNode(PROC_NODE_PTR proc_node_ptr);

    /*
    Description: Adds a READ_NODE_PTR to the READ_TABLE when building the PKB.
                 Returns the INDEX of the READ_NODE_PTR in the READ_TABLE.
    */
    INDEX addReadNode(READ_NODE_PTR read_node_ptr);

    /*
    Description: Adds a STMT_LIST_NODE_PTR to the STMT_LIST_TABLE when building the PKB.
                 Returns the INDEX of the STMT_LIST_NODE_PTR in the STMT_LIST_TABLE.
    */
    INDEX addStatementListNode(STMT_LIST_NODE_PTR stmt_list_node_ptr);

    /*
    Description: Adds a TRY_NODE_PTR to the TRY_TABLE when building the PKB.
                 Returns the INDEX of the TRY_NODE_PTR in the TRY_TABLE.
    */
    INDEX addTryNode(TRY_NODE_PTR try_node_ptr);

    /*
    Description: Adds a WHILE_NODE to the WHILE_TABLE when building the PKB.
                 Returns the INDEX of the STMT_NUM in the WHILE_TABLE.
    */
    INDEX addWhileNode(WHILE_NODE_PTR while_node_ptr);

    /*
    Description: Adds a VAR_NODE_PTR to the VAR_TABLE when building the PKB.
                 Returns the INDEX of the VAR_NODE_PTR in the VAR_TABLE.
    */
    INDEX addVariableNode(VAR_NODE_PTR var_node_ptr);

    /*
    Description: Adds a STMT_NODE_PTR to the STMT_TABLE when building the PKB.
                 Returns the INDEX of the STMT_NODE_PTR in the STMT_TABLE.
    */
    INDEX addStatementNode(STMT_NODE_PTR stmt_node_ptr);


    /*==== Relationships ====*/

    /*
    Description: Stores a Follows(STMT_NUM, STMT_NUM) relationship to
                 the FOLLOWS_TABLE when building the PKB.
    */
    void addFollows(STMT_NUM s1, STMT_NUM s2);

    /*
    Description: Stores a Parent(STMT_NUM, STMT_NUM) relationship to
                 the PARENT_TABLE when building the PKB.
    */
    void addParent(STMT_NUM s1, STMT_NUM s2);

    /*
    Description: Stores a Uses(STATEMENT_NAME, VAR_NAME) relationship to
                 the USES_TABLE when building the PKB.
    */
    void addUses(STMT_NUM s, VAR_NAME v);

    /*
    Description: Stores a Uses(PROC_NAME, VAR_NAME) relationship to
                 the USES_TABLE when building the PKB.
    */
    void addUses(PROC_NAME p, VAR_NAME v);

    /*
    Description: Stores a Modifies(STMT_NUM, VAR_NAME) relationship to
                 the MODIFIES_TABLE when building the PKB.
    */
    void addModifies(STMT_NUM s, VAR_NAME v);

    /*
    Description: Stores a Modifies(PROC_NAME, VAR_NAME) relationship to
                 the MODIFIES_TABLE when building the PKB.
    */
    void addModifies(PROC_NAME p, VAR_NAME v);

    /*
    Description: Stores a Calls(PROC_NAME, PROC_NAME) relationship to
                 the CALL_TABLE when building the PKB.
    */
    void addCalls(PROC_NAME p1, PROC_NAME p2);

    /*
    Description: Stores a Next(STMT_NUM, STMT_NUM) relationship to
                 the NEXT_TABLE when building the PKB.
    */
    void addNext(STMT_NUM s1, STMT_NUM s2);

    /*
    Description: Stores the root/program node pointer into the PKB.
    */
    void setProgramNode(PROGRAM_NODE_PTR program_node_ptr);

    PKB build();
};
