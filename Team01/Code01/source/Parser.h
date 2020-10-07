#pragma once

#include <vector>
#include <string>
#include <deque>

#include "../source/PKB.h"
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
	//int parseCall(STRING str);
	void parseStmtListClose();

	STRING getNextToken(FILE_ITER_PTR iter, FILE_ITER eos);

	PROGRAM_NODE_PTR program_node_;
	AST_NODE_PTR current_parent_node_;
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
};

