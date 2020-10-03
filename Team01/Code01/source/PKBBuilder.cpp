#include "PKBBuilder.h"

INDEX PKBBuilder::addAssignNode(ASSIGN_NODE_PTR assign_node_ptr) {
    return assign_table_.addAssignNode(assign_node_ptr);
}

INDEX PKBBuilder::addCallNode(CALL_NODE_PTR call_node_ptr) {
    return call_table_.addCallNode(call_node_ptr);
}

INDEX PKBBuilder::addConstantNode(CONSTANT_NODE_PTR constant_node_ptr) {
    return constant_table_.addConstantNode(constant_node_ptr);
}

INDEX PKBBuilder::addIfNode(IF_NODE_PTR if_node_ptr) {
    return if_table_.addIfNode(if_node_ptr);
}

INDEX PKBBuilder::addPrintNode(PRINT_NODE_PTR print_node_ptr) {
    return print_table_.addPrintNode(print_node_ptr);
}

INDEX PKBBuilder::addProcedureNode(PROC_NODE_PTR proc_node_ptr) {
    return proc_table_.addProcedureNode(proc_node_ptr);
}

INDEX PKBBuilder::addReadNode(READ_NODE_PTR read_node_ptr) {
    return read_table_.addReadNode(read_node_ptr);
}

INDEX PKBBuilder::addStatementListNode(STMT_LIST_NODE_PTR stmt_list_node_ptr) {
    return stmt_list_table_.addStatementListNode(stmt_list_node_ptr);
}

INDEX PKBBuilder::addVariableNode(VAR_NODE_PTR var_node_ptr) {
    return var_table_.addVariableNode(var_node_ptr);
}

INDEX PKBBuilder::addWhileNode(WHILE_NODE_PTR while_node_ptr) {
    return while_table_.addWhileNode(while_node_ptr);
}

INDEX PKBBuilder::addStatementNode(STMT_NODE_PTR stmt_node_ptr) {
    return stmt_table_.addStatementNode(stmt_node_ptr);
}

/* ==== Return Nodes ==== */
ASSIGN_NODE_PTR_LIST PKBBuilder::getAssigns() {
    return assign_table_.getAssignNodeList();
}

CALL_NODE_PTR_LIST PKBBuilder::getCalls() {
    return call_table_.getCallNodeList();
}

CONSTANT_NODE_PTR_LIST PKBBuilder::getConstants() {
    return constant_table_.getConstantNodeList();
}

IF_NODE_PTR_LIST PKBBuilder::getIfs() {
    return if_table_.getIfNodeList();
}

PRINT_NODE_PTR_LIST PKBBuilder::getPrints() {
    return print_table_.getPrintNodeList();
}

PROC_NODE_PTR_LIST PKBBuilder::getProcedures() {
    return proc_table_.getProcedureNodeList();
}

READ_NODE_PTR_LIST PKBBuilder::getReads() {
    return read_table_.getReadNodeList();
}

WHILE_NODE_PTR_LIST PKBBuilder::getWhiles() {
    return while_table_.getWhileNodeList();
}

STMT_LIST_NODE_PTR_LIST PKBBuilder::getStatementLists() {
    return stmt_list_table_.getStatementListNodeList();
}

VAR_NODE_PTR_LIST PKBBuilder::getVariables() {
    return var_table_.getVariableNodeList();
}

STMT_NODE_PTR_LIST PKBBuilder::getStatements() {
    return stmt_table_.getStatementNodeList();
}


void PKBBuilder::addFollows(STMT_NUM s1, STMT_NUM s2) {
    follows_table_.addFollows(s1, s2);
}

void PKBBuilder::addParent(STMT_NUM s1, STMT_NUM s2) {
    parent_table_.addParent(s1, s2);
}

void PKBBuilder::addUses(STMT_NUM s, VAR_NAME v) {
    uses_table_.addUses(s, v);
}

void PKBBuilder::addUses(PROC_NAME p, VAR_NAME v) {
    uses_table_.addUses(p, v);
}

void PKBBuilder::addModifies(STMT_NUM s, VAR_NAME v) {
    modifies_table_.addModifies(s, v);
}

void PKBBuilder::addModifies(PROC_NAME p, VAR_NAME v) {
    modifies_table_.addModifies(p, v);
}

void PKBBuilder::addCalls(PROC_NAME p1, PROC_NAME p2) {
    calls_table_.addCalls(p1, p2);
}

void PKBBuilder::addNext(STMT_NUM s1, STMT_NUM s2) {
    next_table_.addNext(s1, s2);
}

void PKBBuilder::setProgramNode(PROGRAM_NODE_PTR program_node_ptr) {
    program_node_ptr_ = program_node_ptr;
}

PKB PKBBuilder::build() {
    return PKB(*this);
}
