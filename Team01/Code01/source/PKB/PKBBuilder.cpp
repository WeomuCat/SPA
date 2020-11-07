#include "PKBBuilder.h"

INDEX PKBBuilder::addAssignNode(ASSIGN_NODE_PTR assign_node_ptr) {
    stmt_type_table_.addStatement(assign_node_ptr->getStatementNumber(), STMT_TYPE::assignStmt);
    return assign_table_.addAssignNode(assign_node_ptr);
}

INDEX PKBBuilder::addCallNode(CALL_NODE_PTR call_node_ptr) {
    stmt_type_table_.addStatement(call_node_ptr->getStatementNumber(), STMT_TYPE::callStmt);
    return call_table_.addCallNode(call_node_ptr);
}

INDEX PKBBuilder::addConstantNode(CONSTANT_NODE_PTR constant_node_ptr) {
    return constant_table_.addConstantNode(constant_node_ptr);
}

INDEX PKBBuilder::addIfNode(IF_NODE_PTR if_node_ptr) {
    stmt_type_table_.addStatement(if_node_ptr->getStatementNumber(), STMT_TYPE::ifStmt);
    return if_table_.addIfNode(if_node_ptr);
}

INDEX PKBBuilder::addPrintNode(PRINT_NODE_PTR print_node_ptr) {
    stmt_type_table_.addStatement(print_node_ptr->getStatementNumber(), STMT_TYPE::printStmt);
    return print_table_.addPrintNode(print_node_ptr);
}

INDEX PKBBuilder::addProcedureNode(PROC_NODE_PTR proc_node_ptr) {
    return proc_table_.addProcedureNode(proc_node_ptr);
}

INDEX PKBBuilder::addReadNode(READ_NODE_PTR read_node_ptr) {
    stmt_type_table_.addStatement(read_node_ptr->getStatementNumber(), STMT_TYPE::readStmt);
    return read_table_.addReadNode(read_node_ptr);
}

INDEX PKBBuilder::addStatementListNode(STMT_LIST_NODE_PTR stmt_list_node_ptr) {
    return stmt_list_table_.addStatementListNode(stmt_list_node_ptr);
}

INDEX PKBBuilder::addTryNode(TRY_NODE_PTR try_node_ptr) {
    stmt_type_table_.addStatement(try_node_ptr->getStatementNumber(), STMT_TYPE::tryStmt);
    return try_table_.addTryNode(try_node_ptr);
}

INDEX PKBBuilder::addVariableNode(VAR_NODE_PTR var_node_ptr) {
    return var_table_.addVariableNode(var_node_ptr);
}

INDEX PKBBuilder::addWhileNode(WHILE_NODE_PTR while_node_ptr) {
    stmt_type_table_.addStatement(while_node_ptr->getStatementNumber(), STMT_TYPE::whileStmt);
    return while_table_.addWhileNode(while_node_ptr);
}

INDEX PKBBuilder::addStatementNode(STMT_NODE_PTR stmt_node_ptr) {
    return stmt_table_.addStatementNode(stmt_node_ptr);
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
    assign_table_.preCompute();
    call_table_.preCompute();
    constant_table_.preCompute();
    if_table_.preCompute();
    print_table_.preCompute();
    proc_table_.preCompute();
    read_table_.preCompute();
    stmt_table_.preCompute();
    try_table_.preCompute();
    var_table_.preCompute();
    while_table_.preCompute();

    follows_table_.preComputeTransitive();
    parent_table_.preComputeTransitive();
    calls_table_.preComputeTransitive();
    affects_table_.setModifiesTable(modifies_table_);
    affects_table_.setUsesTable(uses_table_);
    affects_table_.setStatementTypeTable(stmt_type_table_);
    affects_table_.setControlFlowGraph(next_table_.getControlFlowGraph());
    return (PKB)*this;
}
