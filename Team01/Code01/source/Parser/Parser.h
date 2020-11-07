#pragma once

#include <vector>
#include <string>
#include <deque>
#include <unordered_map>
#include <set>

#include "../PKB/PKB.h"
#include "../PKB/PKBBuilder.h"
#include "OperatorTypeEnum.h"
#include "RelOperatorTypeEnum.h"

typedef std::string STRING;
typedef std::vector<std::string> LIST_OF_STRINGS;
typedef std::deque<STRING>* STMT_TOKEN_QUEUE_PTR;
typedef std::deque<STRING>* PROCESS_TOKEN_QUEUE_PTR;
typedef std::deque<STRING> STMT_TOKEN_QUEUE;
typedef std::deque<STRING> PROCESS_TOKEN_QUEUE;
typedef int STMT_NUMBER;
typedef bool BOOLEAN_TYPE;
typedef int PRECEDENCE;
typedef std::istreambuf_iterator<char>* FILE_ITER_PTR;
typedef std::istreambuf_iterator<char> FILE_ITER;
typedef ExpressionTypeEnum EXPR_TYPE_ENUM;
typedef RelationTypeEnum REL_TYPE_ENUM;
typedef ConditionTypeEnum COND_TYPE_ENUM;
typedef RelOperatorTypeEnum REL_OP_TYPE_ENUM;
typedef OperatorTypeEnum OP_TYPE_ENUM;

class Parser {
public:
	PKB parseFile(STRING str);

private:
	void parseProcedure();
	void parseRead();
	void parsePrint();
	void parseAssign();

	void parseWhile();
	void parseIfThen();
	void parseElse();
	void parseCall();
	void parseStmtListClose();

	STRING getNextToken(FILE_ITER_PTR iter, FILE_ITER eos);

	PROGRAM_NODE_PTR program_node_;
	AST_NODE_PTR current_parent_node_;
	PROC_NODE_PTR current_parent_proc_node_;
	PKBBuilder pkb_builder_;

	STMT_TOKEN_QUEUE stmt_token_queue_;
	PROCESS_TOKEN_QUEUE process_token_stream_;

	STMT_NUMBER stmt_num_;

	//Helper variables & functions for parsing expressions
	PRECEDENCE takesPrecedent(OP_TYPE_ENUM l_op, OP_TYPE_ENUM r_op);
	EXPR_TYPE_ENUM getExpressionType(OP_TYPE_ENUM op);

	PRECEDENCE takesPrecedent(REL_OP_TYPE_ENUM l_op, REL_OP_TYPE_ENUM r_op);
	EXPR_TYPE_ENUM getExpressionType(REL_OP_TYPE_ENUM op);

	REL_TYPE_ENUM getArithmeticRelationType(REL_OP_TYPE_ENUM op);
	COND_TYPE_ENUM getBooleanRelationType(REL_OP_TYPE_ENUM op);

	BOOLEAN_TYPE isArithmeticOp(REL_OP_TYPE_ENUM op);
	BOOLEAN_TYPE isRelationOp(REL_OP_TYPE_ENUM op);
	BOOLEAN_TYPE isBooleanOp(REL_OP_TYPE_ENUM op);

	//Helper functions to Topologically Sort Procedure Nodes
	void topoSort(std::unordered_map<PROC_NAME, std::set<PROC_NAME>>& graph, std::deque<PROC_NAME>& sorted_procs);
	void topoSortHelper(PROC_NAME caller_proc, std::unordered_map<PROC_NAME, bool>& visited
		, std::unordered_map<PROC_NAME, std::set<PROC_NAME>> &graph, std::deque<PROC_NAME>& sorted_procs);

	//Helper functions for Adding Next R/s
	void findLastStmts(STMT_LIST_NODE_PTR stmt_list_node_ptr, std::set<STMT_NUM>& stmt_num_set);

	//Helper functions for valid Calls Checking
	bool hasNonExistentProcedureCalls();
	bool hasCyclicRecursiveCalls(std::unordered_map<PROC_NAME, std::set<PROC_NAME>>& proc_call_graph
		, std::deque<PROC_NAME>& sorted_procs);
	bool hasCyclesDFS(PROC_NAME proc_name, std::unordered_map<PROC_NAME, std::set<PROC_NAME>>& proc_call_graph
		, std::set<PROC_NAME>& visited_set, std::set<PROC_NAME>& checked_nodes_set);
};

