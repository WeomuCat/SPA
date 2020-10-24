#include "PKB.h"

/*==== Design Entities (Nodes) ====*/
ASSIGN_NODE_PTR_LIST PKB::getAssigns() {
    return assign_table_.getAssignNodeList();
}

CALL_NODE_PTR_LIST PKB::getCalls() {
    return call_table_.getCallNodeList();
}

CONSTANT_NODE_PTR_LIST PKB::getConstants() {
    return constant_table_.getConstantNodeList();
}

IF_NODE_PTR_LIST PKB::getIfs() {
    return if_table_.getIfNodeList();
}

PRINT_NODE_PTR_LIST PKB::getPrints() {
    return print_table_.getPrintNodeList();
}

PROC_NODE_PTR_LIST PKB::getProcedures() {
    return proc_table_.getProcedureNodeList();
}

READ_NODE_PTR_LIST PKB::getReads() {
    return read_table_.getReadNodeList();
}

WHILE_NODE_PTR_LIST PKB::getWhiles() {
    return while_table_.getWhileNodeList();
}

STMT_LIST_NODE_PTR_LIST PKB::getStatementLists() {
    return stmt_list_table_.getStatementListNodeList();
}

VAR_NODE_PTR_LIST PKB::getVariables() {
    return var_table_.getVariableNodeList();
}

STMT_NODE_PTR_LIST PKB::getStatements() {
    return stmt_table_.getStatementNodeList();
}

/*==== Design Entities (Values) ====*/
STMT_NUM_LIST PKB::getAssignNumList() {
    return assign_table_.getAssignNumList();
}

STMT_NUM_LIST PKB::getCallNumList() {
    return call_table_.getCallNumList();
}

PROC_NAME_LIST PKB::getCallProcNameList() {
    return call_table_.getCallProcNameList();
}

STMT_NUM_LIST PKB::getIfNumList() {
    return if_table_.getIfNumList();
}

STMT_NUM_LIST PKB::getPrintNumList() {
    return print_table_.getPrintNumList();
}

VAR_NAME_LIST PKB::getPrintVarNameList() {
    return print_table_.getPrintVarNameList();
}

STMT_NUM_LIST PKB::getReadNumList() {
    return read_table_.getReadNumList();
}

VAR_NAME_LIST PKB::getReadVarNameList() {
    return read_table_.getReadVarNameList();
}

STMT_NUM_LIST PKB::getWhileNumList() {
    return while_table_.getWhileNumList();
}

STMT_NUM_LIST PKB::getStatementNumList() {
    return stmt_table_.getStatementNumList();
}

CONSTANT_VALUE_LIST PKB::getConstantValueList() {
    return constant_table_.getConstantValueList();
}

PROC_NAME_LIST PKB::getProcedureNameList() {
    return proc_table_.getProcedureNameList();
}

VAR_NAME_LIST PKB::getVariableNameList() {
    return var_table_.getVariableNameList();
}

/*==== Relationships ====*/
BOOLEAN_TYPE PKB::isFollows(STMT_NUM s1, STMT_NUM s2) {
    return follows_table_.isFollows(s1, s2);
}

BOOLEAN_TYPE PKB::isFollowsTransitive(STMT_NUM s1, STMT_NUM s2) {
    return follows_table_.isFollowsTransitive(s1, s2);
}

BOOLEAN_TYPE PKB::isParent(STMT_NUM s1, STMT_NUM s2) {
    return parent_table_.isParent(s1, s2);
}

BOOLEAN_TYPE PKB::isParentTransitive(STMT_NUM s1, STMT_NUM s2) {
    return parent_table_.isParentTransitive(s1, s2);
}

BOOLEAN_TYPE PKB::isUses(STMT_NUM s, VAR_NAME v) {
    return uses_table_.isUses(s, v);
}

BOOLEAN_TYPE PKB::isUses(PROC_NAME p, VAR_NAME v) {
    return uses_table_.isUses(p, v);
}

BOOLEAN_TYPE PKB::isModifies(STMT_NUM s, VAR_NAME v) {
    return modifies_table_.isModifies(s, v);
}

BOOLEAN_TYPE PKB::isModifies(PROC_NAME p, VAR_NAME v) {
    return modifies_table_.isModifies(p, v);
}

BOOLEAN_TYPE PKB::isCalls(PROC_NAME p1, PROC_NAME p2) {
    return calls_table_.isCalls(p1, p2);
}

BOOLEAN_TYPE PKB::isCallsTransitive(PROC_NAME p1, PROC_NAME p2) {
    return calls_table_.isCallsTransitive(p1, p2);
}

BOOLEAN_TYPE PKB::isNext(STMT_NUM s1, STMT_NUM s2) {
    return next_table_.isNext(s1, s2);
}

BOOLEAN_TYPE PKB::isNextTransitive(STMT_NUM s1, STMT_NUM s2) {
    return next_table_.isNextTransitive(s1, s2);
}

BOOLEAN_TYPE PKB::isAffects(STMT_NUM s1, STMT_NUM s2) {
    return affects_table_.isAffects(s1, s2);
}

BOOLEAN_TYPE PKB::isAffectsTransitive(STMT_NUM s1, STMT_NUM s2) {
    return affects_table_.isAffectsTransitive(s1, s2);
}

/*==== AST ====*/
PROGRAM_NODE_PTR PKB::getProgramNode() {
    return program_node_ptr_;
}

/*==== Miscellaneous ====*/
void PKB::clearCache() {
    follows_table_.clearCache();
    parent_table_.clearCache();
    next_table_.clearCache();
    calls_table_.clearCache();
    affects_table_.clearCache();
}
