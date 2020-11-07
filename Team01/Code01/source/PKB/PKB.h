#pragma once

#include "AbstractDataTypes.h"
#include "PKB/ASTNode/ProgramNode.h"
#include "PKB/DesignEntities/AssignTable.h"
#include "PKB/DesignEntities/CallTable.h"
#include "PKB/DesignEntities/ConstantTable.h"
#include "PKB/DesignEntities/IfTable.h"
#include "PKB/DesignEntities/PrintTable.h"
#include "PKB/DesignEntities/ProcedureTable.h"
#include "PKB/DesignEntities/ReadTable.h"
#include "PKB/DesignEntities/StatementListTable.h"
#include "PKB/DesignEntities/StatementTable.h"
#include "PKB/DesignEntities/TryTable.h"
#include "PKB/DesignEntities/VariableTable.h"
#include "PKB/DesignEntities/WhileTable.h"
#include "PKB/Relationships/FollowsTable.h"
#include "PKB/Relationships/ParentTable.h"
#include "PKB/Relationships/UsesTable.h"
#include "PKB/Relationships/ModifiesTable.h"
#include "PKB/Relationships/CallsTable.h"
#include "PKB/Relationships/NextTable.h"
#include "PKB/Relationships/AffectsTable.h"
#include "PKB/Relationships/StatementTypeTable.h"

/* 
Overview: The PKB that contains design entities and relationships of a given SIMPLE source code 
*/
class PKB {

public:

    /*==== Design Entities (Nodes) ====*/
    /*
    Description: Returns the ASSIGN_NODE_PTR_LIST stored in ASSIGN_TABLE.
                 If there are no elements in the collection, returns an empty
                 ASSIGN_NODE_PTR_LIST.
    */
    ASSIGN_NODE_PTR_LIST getAssigns();

    /*
    Description: Returns the CALL_NODE_PTR_LIST stored in CALL_TABLE.
                 If there are no elements in the collection, returns an empty
                 CALL_NODE_PTR_LIST.
    */
    CALL_NODE_PTR_LIST getCalls();

    /*
    Description: Returns the CONSTANT_NODE_PTR_LIST stored in CONSTANT_TABLE.
                 If there are no elements in the collection, returns an empty
                 CONSTANT_NODE_PTR_LIST.
    */
    CONSTANT_NODE_PTR_LIST getConstants();

    /*
    Description: Returns the IF_NODE_PTR_LIST stored in IF_TABLE.
                 If there are no elements in the collection, returns an empty
                 IF_NODE_PTR_LIST.
    */
    IF_NODE_PTR_LIST getIfs();

    /*
    Description: Returns the PRINT_NODE_PTR_LIST stored in PRINT_TABLE.
                 If there are no elements in the collection, returns an empty
                 PRINT_NODE_PTR_LIST.
    */
    PRINT_NODE_PTR_LIST getPrints();

    /*
    Description: Returns the PROC_NODE_PTR_LIST stored in PROCEDURE_TABLE.
                 If there are no elements in the collection, returns an empty
                 PROC_NODE_PTR_LIST.
    */
    PROC_NODE_PTR_LIST getProcedures();

    /*
    Description: Returns the READ_NODE_PTR_LIST stored in READ_TABLE.
                 If there are no elements in the collection, returns an empty
                 READ_NODE_PTR_LIST.
    */
    READ_NODE_PTR_LIST getReads();

    /*
    Description: Returns the STMT_LIST_NODE_PTR_LIST stored in STMT_LIST_TABLE.
                 If there are no elements in the collection, returns an empty
                 STMT_LIST_NODE_PTR_LIST
    */
    STMT_LIST_NODE_PTR_LIST getStatementLists();

    /*
    Description: Returns the TRY_NODE_PTR_LIST stored in TRY_TABLE.
                 If there are no elements in the collection, returns an empty
                 TRY_NODE_PTR_LIST
    */
    TRY_NODE_PTR_LIST getTrys();

    /*
    Description: Returns the VAR_NODE_PTR_LIST stored in VARIABLE_TABLE.
                 If there are no elements in the collection, returns an empty
                 VAR_NODE_PTR_LIST.
    */
    VAR_NODE_PTR_LIST getVariables();

    /*
    Description: Returns the WHILE_NODE_PTR_LIST stored in WHILE_TABLE.
                 If there are no elements in the collection, returns an empty
                 WHILE_NODE_PTR_LIST.
    */
    WHILE_NODE_PTR_LIST getWhiles();

    /*
    Description: Returns the STMT_NODE_PTR_LIST stored in STATEMENT_TABLE.
                 If there are no elements in the collection, returns an empty
                 STMT_NODE_PTR_LIST.
    */
    STMT_NODE_PTR_LIST getStatements();
    

    /*==== Design Entities (Values) ====*/

    /*
    Description: Returns the STMT_NUM_LIST stored in ASSIGN_TABLE.
                 If there are no elements in the collection, returns an empty
                 STMT_NUM_LIST.
    */
    STMT_NUM_LIST getAssignNumList();

    /*
    Description: Returns the STMT_NUM_LIST stored in CALL_TABLE.
                 If there are no elements in the collection, returns an empty
                 STMT_NUM_LIST.
    */
    STMT_NUM_LIST getCallNumList();

    /*
    Description: Returns the PROC_NAME_LIST stored in CALL_TABLE.
                 If there are no elements in the collection, returns an empty
                 PROC_NAME_LIST.
    */
    PROC_NAME_LIST getCallProcNameList();

    /*
    Description: Returns the STMT_NUM_LIST stored in IF_TABLE.
                 If there are no elements in the collection, returns an empty
                 STMT_NUM_LIST.
    */
    STMT_NUM_LIST getIfNumList();

    /*
    Description: Returns the STMT_NUM_LIST stored in PRINT_TABLE.
                 If there are no elements in the collection, returns an empty
                 STMT_NUM_LIST.
    */
    STMT_NUM_LIST getPrintNumList();

    /*
    Description: Returns the VAR_NAME_LIST stored in PRINT_TABLE.
                 If there are no elements in the collection, returns an empty
                 VAR_NAME_LIST.
    */
    VAR_NAME_LIST getPrintVarNameList();

    /*
    Description: Returns the STMT_NUM_LIST stored in READ_TABLE.
                 If there are no elements in the collection, returns an empty
                 STMT_NUM_LIST.
    */
    STMT_NUM_LIST getReadNumList();

    /*
    Description: Returns the VAR_NAME_LIST stored in READ_TABLE.
                 If there are no elements in the collection, returns an empty
                 VAR_NAME_LIST.
    */
    VAR_NAME_LIST getReadVarNameList();

    /*
    Description: Returns the STMT_NUM_LIST stored in WHILE_TABLE.
                 If there are no elements in the collection, returns an empty
                 STMT_NUM_LIST.
    */
    STMT_NUM_LIST getWhileNumList();

    /*
    Description: Returns the STMT_NUM_LIST stored in STMT_TABLE.
                 If there are no elements in the collection, returns an empty
                 STMT_NUM_LIST.
    */
    STMT_NUM_LIST getStatementNumList();

    /*
    Description: Returns the STMT_NUM_LIST stored in TRY_TABLE.
                 If there are no elements in the collection, returns an empty
                 TRY_NUM_LIST.
    */
    STMT_NUM_LIST getTryNumList();

    /*
    Description: Returns the CONSTANT_VALUE_LIST stored in CONSTANT_TABLE.
                 If there are no elements in the collection, returns an empty
                 CONSTANT_VALUE_LIST.
    */
    CONSTANT_VALUE_LIST getConstantValueList();

    /*
    Description: Returns the PROC_NAME_LIST stored in PROC_TABLE.
                 If there are no elements in the collection, returns an empty
                 PROC_NAME_LIST.
    */
    PROC_NAME_LIST getProcedureNameList();

    /*
    Description: Returns the VAR_NAME_LIST stored in VAR_TABLE.
                 If there are no elements in the collection, returns an empty
                 VAR_NAME_LIST.
    */
    VAR_NAME_LIST getVariableNameList();

    /*==== Relationships ====*/
    /*
    Description: Returns a BOOLEAN_TYPE indicating whether or not
                 Follows(STMT_NUM, STMT_NUM) holds
    */
    BOOLEAN_TYPE isFollows(STMT_NUM s1, STMT_NUM s2);

    /*
    Description: Returns a BOOLEAN_TYPE indicating whether or not
                 Follows*(STMT_NUM, STMT_NUM) holds
    */
    BOOLEAN_TYPE isFollowsTransitive(STMT_NUM s1, STMT_NUM s2);

    /*
    Description: Returns a BOOLEAN_TYPE indicating whether or not
                 Parent(STMT_NUM, STMT_NUM) holds
    */
    BOOLEAN_TYPE isParent(STMT_NUM s1, STMT_NUM s2);

    /*
    Description: Returns a BOOLEAN_TYPE indicating whether or not
                 Parent*(STMT_NUM, STMT_NUM) holds
    */
    BOOLEAN_TYPE isParentTransitive(STMT_NUM s1, STMT_NUM s2);

    /*
    Description: Returns a BOOLEAN_TYPE indicating whether or not
                 Uses(STMT_NUM, VAR_NAME) holds
    */
    BOOLEAN_TYPE isUses(STMT_NUM s, VAR_NAME v);

    /*
    Description: Returns a BOOLEAN_TYPE indicating whether or not
                 Uses(PROC_NAME, VAR_NAME) holds
    */
    BOOLEAN_TYPE isUses(PROC_NAME p, VAR_NAME v);

    /*
    Description: Returns a BOOLEAN_TYPE indicating whether or not
                 Modifies(STMT_NUM, VAR_NAME) holds
    */
    BOOLEAN_TYPE isModifies(STMT_NUM s, VAR_NAME v);

    /*
    Description: Returns a BOOLEAN_TYPE indicating whether or not
                 Modifies(PROC_NAME, VAR_NAME) holds
    */
    BOOLEAN_TYPE isModifies(PROC_NAME p, VAR_NAME v);

    /*
    Description: Returns a BOOLEAN_TYPE indicating whether or not
                 Calls(PROC_NAME, PROC_NAME) holds
    */
    BOOLEAN_TYPE isCalls(PROC_NAME p1, PROC_NAME p2);

    /*
    Description: Returns a BOOLEAN_TYPE indicating whether or not
                 Calls*(PROC_NAME, PROC_NAME) holds
    */
    BOOLEAN_TYPE isCallsTransitive(PROC_NAME p1, PROC_NAME p2);

    /*
    Description: Returns a BOOLEAN_TYPE indicating whether or not
                 Next(STMT_NUM, STMT_NUM) holds
    */
    BOOLEAN_TYPE isNext(STMT_NUM s1, STMT_NUM s2);

    /*
    Description: Returns a BOOLEAN_TYPE indicating whether or not
                 Next*(STMT_NUM, STMT_NUM) holds
    */
    BOOLEAN_TYPE isNextTransitive(STMT_NUM s1, STMT_NUM s2);

    /*
    Description: Returns a BOOLEAN_TYPE indicating whether or not
                 Affects(STMT_NUM, STMT_NUM) holds
    */
    BOOLEAN_TYPE isAffects(STMT_NUM s1, STMT_NUM s2);
    
    /*
    Description: Returns a BOOLEAN_TYPE indicating whether or not
                 Affects*(STMT_NUM, STMT_NUM) holds
    */
    BOOLEAN_TYPE isAffectsTransitive(STMT_NUM s1, STMT_NUM s2);

    /*==== AST ====*/

    /*
    Description: Returns the source program's AST.
    */
    PROGRAM_NODE_PTR getProgramNode();

protected:
    PROGRAM_NODE_PTR program_node_ptr_;

    ASSIGN_TABLE assign_table_;
    CALL_TABLE call_table_;
    CONSTANT_TABLE constant_table_;
    IF_TABLE if_table_;
    PRINT_TABLE print_table_;
    PROC_TABLE proc_table_;
    READ_TABLE read_table_;
    STMT_LIST_TABLE stmt_list_table_;
    STMT_TABLE stmt_table_;
    TRY_TABLE try_table_;
    VAR_TABLE var_table_;
    WHILE_TABLE while_table_;

    FOLLOWS_TABLE follows_table_;
    PARENT_TABLE parent_table_;
    USES_TABLE uses_table_;
    MODIFIES_TABLE modifies_table_;
    CALLS_TABLE calls_table_;
    NEXT_TABLE next_table_;
    AFFECTS_TABLE affects_table_;
    STMT_TYPE_TABLE stmt_type_table_;
};
