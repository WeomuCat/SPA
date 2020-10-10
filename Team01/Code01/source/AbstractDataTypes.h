#pragma once

#include<memory>
#include<vector>

#include "PKBBuilder.h"

typedef std::shared_ptr<ASTNode> AST_NODE_PTR;
typedef std::vector<std::shared_ptr<ASTNode>> AST_NODE_PTR_LIST;
typedef std::shared_ptr<AssignNode> ASSIGN_NODE_PTR;
typedef std::vector<std::shared_ptr<AssignNode>> ASSIGN_NODE_PTR_LIST;
typedef std::shared_ptr<CallNode> CALL_NODE_PTR;
typedef std::vector<std::shared_ptr<CallNode>> CALL_NODE_PTR_LIST;
typedef std::shared_ptr<ConditionNode> CONDITION_NODE_PTR;
typedef std::vector<std::shared_ptr<ConditionNode>> CONDITION_NODE_PTR_LIST;
typedef std::shared_ptr<ConstantNode> CONSTANT_NODE_PTR;
typedef std::vector<std::shared_ptr<ConstantNode>> CONSTANT_NODE_PTR_LIST;
typedef std::shared_ptr<ExpressionNode> EXPR_NODE_PTR;
typedef std::vector<std::shared_ptr<ExpressionNode>> EXPR_NODE_PTR_LIST;
typedef std::shared_ptr<IfNode> IF_NODE_PTR;
typedef std::vector<std::shared_ptr<IfNode>> IF_NODE_PTR_LIST;
typedef std::shared_ptr<PrintNode> PRINT_NODE_PTR;
typedef std::vector<std::shared_ptr<PrintNode>> PRINT_NODE_PTR_LIST;
typedef std::shared_ptr<ProcedureNode> PROC_NODE_PTR;
typedef std::vector<std::shared_ptr<ProcedureNode>> PROC_NODE_PTR_LIST;
typedef std::shared_ptr<ProgramNode> PROGRAM_NODE_PTR;
typedef std::shared_ptr<ReadNode> READ_NODE_PTR;
typedef std::vector<std::shared_ptr<ReadNode>> READ_NODE_PTR_LIST;
typedef std::shared_ptr<RelationNode> RELATION_NODE_PTR;
typedef std::vector<std::shared_ptr<RelationNode>> RELATION_NODE_PTR_LIST;
typedef std::shared_ptr<StatementListNode> STMT_LIST_NODE;
typedef std::vector<std::shared_ptr<StatementListNode>> STMT_LIST_NODE_LIST;
typedef std::shared_ptr<StatementNode> STMT_NODE_PTR;
typedef std::vector<std::shared_ptr<StatementNode>>  STMT_NODE_PTR_LIST;
typedef std::shared_ptr<VariableNode> VAR_NODE_PTR;
typedef std::vector<std::shared_ptr<VariableNode>> VAR_NODE_PTR_LIST;
typedef std::shared_ptr<WhileNode> WHILE_NODE_PTR;
typedef std::vector<std::shared_ptr<WhileNode>> WHILE_NODE_PTR_LIST;
typedef AssignTable ASSIGN_TABLE;
typedef CallTable CALL_TABLE;;
typedef ConstantTable CONSTANT_TABLE;
typedef IfTable IF_TABLE;
typedef PrintTable PRINT_TABLE;
typedef ProcedureTable PROC_TABLE;
typedef ReadTable READ_TABLE;
typedef StatementListTable STMT_LIST_TABLE;
typedef StatementTable STMT_TABLE;
typedef VariableTable VAR_TABLE;
typedef WhileTable WHILE_TABLE;
typedef FollowsTable FOLLOWS_TABLE;
typedef ParentTable PARENT_TABLE;
typedef UsesTable USES_TABLE;
typedef ModifiesTable MODIFIES_TABLE;
typedef CallsTable CALLS_TABLE;
typedef NextTable NEXT_TABLE;
typedef int STMT_NUM;
typedef std::vector<int> STMT_NUM_LIST;
typedef std::string CONSTANT_VALUE;
typedef std::vector<std::string> CONSTANT_VALUE_LIST;
typedef std::string VAR_NAME;
typedef std::vector<std::string> VAR_NAME_LIST;
typedef std::string PROC_NAME;
typedef std::vector<std::string> PROC_NAME_LIST;
typedef PKBBuilder PKB_BUILDER;
typedef bool BOOLEAN_TYPE;
typedef int INDEX;
typedef NodeTypeEnum NODE_TYPE;
typedef ConditionTypeEnum CONDITON_TYPE;
typedef ExpressionTypeEnum EXPR_TYPE;
typedef RelationTypeEnum RELATION_TYPE;
