#pragma once

#include<memory>
#include<vector>

// AST NODES
#ifdef AST_NODES
#include "PKB/ASTNode/ProgramNode.h"
#include "PKB/ASTNode/ConditionTypeEnum.h"
#include "PKB/ASTNode/ExpressionTypeEnum.h"
#include "PKB/ASTNode/RelationTypeEnum.h"
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
typedef NodeTypeEnum NODE_TYPE;
typedef ConditionTypeEnum CONDITON_TYPE;
typedef ExpressionTypeEnum EXPR_TYPE;
typedef RelationTypeEnum RELATION_TYPE;
#endif

// Design Entity Tables
#ifdef DESIGN_ENTITY_TABLES
#include "PKB/DesignEntities/AssignTable.h"
#include "PKB/DesignEntities/CallTable.h"
#include "PKB/DesignEntities/ConstantTable.h"
#include "PKB/DesignEntities/IfTable.h"
#include "PKB/DesignEntities/PrintTable.h"
#include "PKB/DesignEntities/ProcedureTable.h"
#include "PKB/DesignEntities/ReadTable.h"
#include "PKB/DesignEntities/StatementListTable.h"
#include "PKB/DesignEntities/StatementTable.h"
#include "PKB/DesignEntities/VariableTable.h"
#include "PKB/DesignEntities/WhileTable.h"
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
#endif

// Relationship Tables
#ifdef RELATIONSHIP_TABLES
#include "PKB/Relationships/CallsTable.h"
#include "PKB/Relationships/FollowsTable.h"
#include "PKB/Relationships/ModifiesTable.h"
#include "PKB/Relationships/NextTable.h"
#include "PKB/Relationships/ParentTable.h"
#include "PKB/Relationships/UsesTable.h"
typedef FollowsTable FOLLOWS_TABLE;
typedef ParentTable PARENT_TABLE;
typedef UsesTable USES_TABLE;
typedef ModifiesTable MODIFIES_TABLE;
typedef CallsTable CALLS_TABLE;
typedef NextTable NEXT_TABLE;
#endif

#ifdef GENERAL
typedef bool BOOLEAN_TYPE
typedef int INDEX;
typedef int STMT_NUM;
typedef std::vector<int> STMT_NUM_LIST;
typedef std::string CONSTANT_VALUE;
typedef std::vector<std::string> CONSTANT_VALUE_LIST;
typedef std::string VAR_NAME;
typedef std::vector<std::string> VAR_NAME_LIST;
typedef std::string PROC_NAME;
typedef std::vector<std::string> PROC_NAME_LIST;
#endif
