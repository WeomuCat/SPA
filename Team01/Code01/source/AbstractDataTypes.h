#pragma once

#include<memory>
#include<vector>
#include<string>
#include<unordered_map>

// Forward Declaration

// PKB
class PKB;
class PKBBuilder;

// AST Nodes
class ASTNode;
class AssignNode;
class CallNode;
class ConditionNode;
class ConstantNode;
class ExpressionNode;
class IfNode;
class PrintNode;
class ProcedureNode;
class ProgramNode;
class ReadNode;
class RelationNode;
class StatementListNode;
class StatementNode;
class VariableNode;
class WhileNode;
enum class NodeTypeEnum;
enum class ConditionTypeEnum;
enum class ExpressionTypeEnum;
enum class RelationTypeEnum;

// Design Entity Tables
class AssignTable;
class CallTable;
class ConstantTable;
class IfTable;
class PrintTable;
class ProcedureTable;
class ReadTable;
class StatementListTable;
class StatementTable;
class VariableTable;
class WhileTable;

// Relationship Tables
class FollowsTable;
class ParentTable;
class UsesTable;
class ModifiesTable;
class CallsTable;
class NextTable;
class AffectsTable;

// QP
class QueryNode;
enum class QueryNodeType;
enum class QuerySynonymType;
class QueryNodeContent;

// PKB
typedef PKBBuilder PKB_BUILDER;

// AST_NODES
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
typedef std::shared_ptr<StatementListNode> STMT_LIST_NODE_PTR;
typedef std::vector<std::shared_ptr<StatementListNode>> STMT_LIST_NODE_PTR_LIST;
typedef std::shared_ptr<StatementNode> STMT_NODE_PTR;
typedef std::vector<std::shared_ptr<StatementNode>>  STMT_NODE_PTR_LIST;
typedef std::shared_ptr<VariableNode> VAR_NODE_PTR;
typedef std::vector<std::shared_ptr<VariableNode>> VAR_NODE_PTR_LIST;
typedef std::shared_ptr<WhileNode> WHILE_NODE_PTR;
typedef std::vector<std::shared_ptr<WhileNode>> WHILE_NODE_PTR_LIST;
typedef NodeTypeEnum NODE_TYPE;
typedef ConditionTypeEnum CONDITION_TYPE;
typedef ExpressionTypeEnum EXPR_TYPE;
typedef RelationTypeEnum RELATION_TYPE;

// Design Entity Tables
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

// Relationship Tables
typedef FollowsTable FOLLOWS_TABLE;
typedef ParentTable PARENT_TABLE;
typedef UsesTable USES_TABLE;
typedef ModifiesTable MODIFIES_TABLE;
typedef CallsTable CALLS_TABLE;
typedef NextTable NEXT_TABLE;
typedef AffectsTable AFFECTS_TABLE;

// QP
typedef std::vector<std::string> QUERY_RESULT;
typedef QueryNode PROCESSED_CLAUSES;
typedef std::unordered_map<std::string, QueryNode> PROCESSED_SYNONYMS;

// AttrRef
typedef std::string ATTR_REF_VALUE;
typedef std::vector<std::string> ATTR_REF_VALUES_LIST;

// QueryNode
typedef QueryNodeType QUERY_NODE_TYPE;
typedef std::string NODE_TYPE_STRING;
typedef QueryNode* QUERY_NODE_POINTERS;
typedef QuerySynonymType SYNONYM_TYPE;
typedef std::string SYNONYM_TYPE_STRING;
typedef std::string SYNONYM_NAME;
typedef QueryNodeContent QUERY_NODE_CONTENT;
typedef std::vector<QueryNode> QUERY_NODE_LIST;

// ResultListManager
typedef std::string STRING_RESULT;
typedef std::vector<std::string> VALUE_LIST;
typedef std::vector<SYNONYM_NAME> TUPLE_RETURN_SYNONYMS;
typedef std::unordered_map<std::string, QueryNode> PROCESSED_SYNONYMS;

// General
typedef bool BOOLEAN_TYPE;
typedef int INDEX;
typedef int STMT_NUM;
typedef std::vector<int> STMT_NUM_LIST;
typedef std::string CONSTANT_VALUE;
typedef std::vector<std::string> CONSTANT_VALUE_LIST;
typedef std::string VAR_NAME;
typedef std::vector<std::string> VAR_NAME_LIST;
typedef std::string PROC_NAME;
typedef std::vector<std::string> PROC_NAME_LIST;
