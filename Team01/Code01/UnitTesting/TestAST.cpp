#include "stdafx.h"
#include "CppUnitTest.h"
#include "PKB.h"
#include "PKBBuilder.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {		
	
	TEST_CLASS(ASTNodeTest) {
	public:
		VAR_NODE_PTR var = std::make_shared<VariableNode>();
		PRINT_NODE_PTR print = std::make_shared<PrintNode>();

		TEST_METHOD(getParent_withNodes_true) {
			Assert::IsTrue(var->getParentNode() == nullptr);
			print->setVariableNode(var);
			AST_NODE_PTR parent = var->getParentNode();
			Assert::IsTrue(std::addressof(*print) == std::addressof(*parent));
		}

		TEST_METHOD(getChildren_withNodes_true) {
			Assert::IsTrue(var->getChildrenNode().size() == 0);
			print->setVariableNode(var);
			AST_NODE_PTR child = print->getChildrenNode().at(0);
			Assert::IsTrue(std::addressof(*var) == std::addressof(*child));
		}
	};

	TEST_CLASS(StatementNodeTest) {
	public:
		STMT_LIST_NODE_PTR stmt_list = std::make_shared<StatementListNode>();
		READ_NODE_PTR read = std::make_shared<ReadNode>();

		TEST_METHOD(setStatementNumber_getStatementNumber_true) {
			Assert::IsTrue(read->getStatementNumber() == NULL);
			int stmt_num = 5;
			read->setStatementNumber(stmt_num);
			Assert::IsTrue(stmt_num == read->getStatementNumber());
		}

		TEST_METHOD(setStatementListNode_getStatementListNode_true) {
			Assert::IsTrue(read->getParentStatementListNode() == nullptr);
			read->setStatementListNode(stmt_list);
			STMT_LIST_NODE_PTR parent = read->getParentStatementListNode();
			Assert::IsTrue(std::addressof(*stmt_list) == std::addressof(*parent));
		}
	};

	TEST_CLASS(AssignNodeTest) {
	public:
		ASSIGN_NODE_PTR assign = std::make_shared<AssignNode>();
		EXPR_NODE_PTR expr = std::make_shared<ExpressionNode>();
		VAR_NODE_PTR var = std::make_shared<VariableNode>();

		TEST_METHOD(setVariableNode_getVariableNode_true) {
			Assert::IsTrue(assign->getVariableNode() == nullptr);
			assign->setVariableNode(var);
			VAR_NODE_PTR child = assign->getVariableNode();
			Assert::IsTrue(std::addressof(*var) == std::addressof(*child));
			
		}

		TEST_METHOD(setExpressionNode_getExpressionNode_true) {
			Assert::IsTrue(assign->getExpressionNode() == nullptr);
			assign->setExpressionNode(expr);
			EXPR_NODE_PTR child = assign->getExpressionNode();
			Assert::IsTrue(std::addressof(*expr) == std::addressof(*child));
		}
	};

	TEST_CLASS(CallNodeTest) {
	public:
		CALL_NODE_PTR call = std::make_shared<CallNode>();
		PROC_NODE_PTR callee = std::make_shared<ProcedureNode>();

		TEST_METHOD(setCalleeProcedureName_getCalleeProcedureName_true) {
			Assert::IsTrue(call->getCalleeProcedureName() == "");
			std::string value = "callee";
			call->setCalleeProcedureName(value);
			Assert::IsTrue(value == call->getCalleeProcedureName());
		}

		TEST_METHOD(setCallerProcedureName_getCallerProcedureName_true) {
			Assert::IsTrue(call->getCallerProcedureName() == "");
			std::string value = "caller";
			call->setCallerProcedureName(value);
			Assert::IsTrue(value == call->getCallerProcedureName());
		}

		TEST_METHOD(setProcedureNode_getProcedureNode_true) {
			Assert::IsTrue(call->getProcedureNode() == nullptr);
			call->setProcedureNode(callee);
			PROC_NODE_PTR callee_procedure = call->getProcedureNode();
			Assert::IsTrue(std::addressof(*callee) == std::addressof(*callee_procedure));
		}
	};

	TEST_CLASS(ConditionNodeTest) {
	public:
		CONDITION_NODE_PTR cond = std::make_shared<ConditionNode>();
		CONDITION_NODE_PTR cond_2 = std::make_shared<ConditionNode>();
		RELATION_NODE_PTR relation = std::make_shared<RelationNode>();

		TEST_METHOD(setConditionType_getConditionType_true) {
			Assert::IsTrue(CONDITION_TYPE::undefined == cond->getConditionType());
			cond->setConditionType(CONDITION_TYPE::and);
			Assert::IsTrue(CONDITION_TYPE::and == cond->getConditionType());

		}

		TEST_METHOD(setLeftstNode_getLeftAstNode_true) {
			Assert::IsTrue(cond->getLeftAstNode() == nullptr);
			cond->setLeftAstNode(relation);
			AST_NODE_PTR child = cond->getLeftAstNode();
			Assert::IsTrue(std::addressof(*relation) == std::addressof(*child));
		}


		TEST_METHOD(setRightAstNode_getRightAstNode_true) {
			auto func = [this] {cond->getRightAstNode() == nullptr;};
			Assert::ExpectException<std::exception>(func);
			cond->setRightAstNode(cond_2);
			AST_NODE_PTR child = cond->getRightAstNode();
			Assert::IsTrue(std::addressof(*cond_2) == std::addressof(*child));
		}
	};

	TEST_CLASS(ConstantNodeTest) {
	public:
		CONSTANT_NODE_PTR constant = std::make_shared<ConstantNode>();

		TEST_METHOD(setValue_getValue_true) {
			Assert::IsTrue(constant->getValue() == "");
			std::string value = "5";
			constant->setValue(value);
			Assert::IsTrue(value == constant->getValue());
		}
	};

	TEST_CLASS(ExpressionNodeTest) {
	public:
		EXPR_NODE_PTR expr = std::make_shared<ExpressionNode>();
		VAR_NODE_PTR var = std::make_shared<VariableNode>();
		CONSTANT_NODE_PTR constant = std::make_shared<ConstantNode>();

		TEST_METHOD(setExpressionType_getExpressionType_true) {
			Assert::IsTrue(EXPR_TYPE::undefined == expr->getExpressionType());
			expr->setExpressionType(EXPR_TYPE::div);
			Assert::IsTrue(EXPR_TYPE::div == expr->getExpressionType());
		}

		TEST_METHOD(setLeftAstNode_getLeftAstNode_true) {
			Assert::IsTrue(expr->getLeftAstNode() == nullptr);
			expr->setLeftAstNode(var);
			AST_NODE_PTR child = expr->getLeftAstNode();
			Assert::IsTrue(std::addressof(*var) == std::addressof(*child));
		}
		
		TEST_METHOD(setRightAstNode_getRightAstNode_true) {
			auto func = [this] {expr->getRightAstNode() == nullptr;};
			Assert::ExpectException<std::exception>(func);
			expr->setRightAstNode(constant);
			AST_NODE_PTR child = expr->getRightAstNode();
			Assert::IsTrue(std::addressof(*constant) == std::addressof(*child));
		}
	};

	TEST_CLASS(IfNodeTest) {
	public:
		IF_NODE_PTR ifs = std::make_shared<IfNode>();
		CONDITION_NODE_PTR cond = std::make_shared<ConditionNode>();
		STMT_LIST_NODE_PTR stmt_list_1 = std::make_shared<StatementListNode>();
		STMT_LIST_NODE_PTR stmt_list_2 = std::make_shared<StatementListNode>();

		TEST_METHOD(setConditionNode_getConditionNode_true) {
			Assert::IsTrue(ifs->getConditionNode() == nullptr);
			ifs->setConditionNode(cond);
			CONDITION_NODE_PTR child = ifs->getConditionNode();
			Assert::IsTrue(std::addressof(*cond) == std::addressof(*child));
		}

		TEST_METHOD(setLeftAstNode_getLeftAstNode_true) {
			Assert::IsTrue(ifs->getThenStatementListNode() == nullptr);
			ifs->setThenStatementListNode(stmt_list_1);
			STMT_LIST_NODE_PTR child = ifs->getThenStatementListNode();
			Assert::IsTrue(std::addressof(*stmt_list_1) == std::addressof(*child));
		}

		TEST_METHOD(setRightAstNode_getRightAstNode_true) {
			Assert::IsTrue(ifs->getElseStatementListNode() == nullptr);
			ifs->setElseStatementListNode(stmt_list_2);
			STMT_LIST_NODE_PTR child = ifs->getElseStatementListNode();
			Assert::IsTrue(std::addressof(*stmt_list_2) == std::addressof(*child));
		}
	};

	TEST_CLASS(PrintNodeTest) {
	public:
		PRINT_NODE_PTR print = std::make_shared<PrintNode>();
		VAR_NODE_PTR var = std::make_shared<VariableNode>();

		TEST_METHOD(setVariableName_getVariableName_true) {
			Assert::IsTrue(print->getVariableNode() == nullptr);
			print->setVariableNode(var);
			VAR_NODE_PTR child = print->getVariableNode();
			Assert::IsTrue(std::addressof(*var) == std::addressof(*child));
		}
	};

	TEST_CLASS(ProcedureNodeTest) {
	public:
		PROC_NODE_PTR proc = std::make_shared<ProcedureNode>();
		STMT_LIST_NODE_PTR stmt_list = std::make_shared<StatementListNode>();

		TEST_METHOD(setProcedureName_getVariableName_true) {
			Assert::IsTrue(proc->getProcedureName() == "");
			std::string name = "procName";
			proc->setProcedureName(name);
			Assert::IsTrue(name == proc->getProcedureName());
		}

		TEST_METHOD(setProcedureStatementListNode_getProcedureStatementListNode_true) {
			Assert::IsTrue(proc->getProcedureStatementListNode() == nullptr);
			proc->setProcedureStatementListNode(stmt_list);
			STMT_LIST_NODE_PTR child = proc->getProcedureStatementListNode();
			Assert::IsTrue(std::addressof(*stmt_list) == std::addressof(*child));
		}
	};

	TEST_CLASS(ProgramNodeTest) {
	public:
		PROGRAM_NODE_PTR program = std::make_shared<ProgramNode>();
		PROC_NODE_PTR proc = std::make_shared<ProcedureNode>();

		TEST_METHOD(AddProcedureNode_getProcedureNodeList_true) {
			Assert::IsTrue(program->getProcedureNodeList().size() == 0);
			program->addProcedureNode(proc);
			PROC_NODE_PTR child = program->getProcedureNodeList().at(0);
			Assert::IsTrue(std::addressof(*proc) == std::addressof(*child));
		}
	};

	TEST_CLASS(ReadNodeTest) {
	public:
		READ_NODE_PTR read = std::make_shared<ReadNode>();
		VAR_NODE_PTR var = std::make_shared<VariableNode>();

		TEST_METHOD(setVariableName_getVariableName_true) {
			Assert::IsTrue(read->getVariableNode() == nullptr);
			read->setVariableNode(var);
			VAR_NODE_PTR child = read->getVariableNode();
			Assert::IsTrue(std::addressof(*var) == std::addressof(*child));
		}
	};

	TEST_CLASS(RelationNodeTest) {
	public:
		RELATION_NODE_PTR relation = std::make_shared<RelationNode>();
		VAR_NODE_PTR var = std::make_shared<VariableNode>();
		CONSTANT_NODE_PTR constant = std::make_shared<ConstantNode>();

		TEST_METHOD(setRelationType_getRelationType_true) {
			Assert::IsTrue(RELATION_TYPE::undefined == relation->getRelationType());
			relation->setRelationType(RELATION_TYPE::eq);
			Assert::IsTrue(RELATION_TYPE::eq == relation->getRelationType());
		}

		TEST_METHOD(setLeftAstNode_getLeftAstNode_true) {
			Assert::IsTrue(relation->getLeftAstNode() == nullptr);
			relation->setLeftAstNode(var);
			AST_NODE_PTR child = relation->getLeftAstNode();
			Assert::IsTrue(std::addressof(*var) == std::addressof(*child));
		}

		TEST_METHOD(setRightAstNode_getRightAstNode_true) {
			Assert::IsTrue(relation->getRightAstNode() == nullptr);
			relation->setRightAstNode(constant);
			AST_NODE_PTR child = relation->getRightAstNode();
			Assert::IsTrue(std::addressof(*constant) == std::addressof(*child));
		}
	};

	TEST_CLASS(StatementListNodeTest) {
	public:
		STMT_LIST_NODE_PTR stmt_list = std::make_shared<StatementListNode>();
		ASSIGN_NODE_PTR assign = std::make_shared<AssignNode>();

		TEST_METHOD(AddStatementNode_getStatementNodeList_true) {
			Assert::IsTrue(stmt_list->getStatementNodeList().size() == 0);
			stmt_list->addStatementNode(assign);
			STMT_NODE_PTR child = stmt_list->getStatementNodeList().at(0);
			Assert::IsTrue(std::addressof(*assign) == std::addressof(*child));
		}
	};

	TEST_CLASS(VariableNodeTest) {
	public:
		VAR_NODE_PTR var = std::make_shared<VariableNode>();

		TEST_METHOD(setVariableName_getVariableName_true) {
			Assert::IsTrue(var->getVariableName() == "");
			std::string value = "x";
			var->setVariableName(value);
			Assert::IsTrue(value == var->getVariableName());
		}
	};

	TEST_CLASS(WhileNodeTest) {
	public:
		WHILE_NODE_PTR whiles = std::make_shared<WhileNode>();
		CONDITION_NODE_PTR cond = std::make_shared<ConditionNode>();
		STMT_LIST_NODE_PTR stmt_list = std::make_shared<StatementListNode>();

		TEST_METHOD(setConditionNode_getConditionNode_true) {
			Assert::IsTrue(whiles->getConditionNode() == nullptr);
			whiles->setConditionNode(cond);
			CONDITION_NODE_PTR child = whiles->getConditionNode();
			Assert::IsTrue(std::addressof(*cond) == std::addressof(*child));
		}

		TEST_METHOD(setWhileStatementListNode_getWhileStatementListNode_true) {
			Assert::IsTrue(whiles->getWhileStatementListNode() == nullptr);
			whiles->setWhileStatementListNode(stmt_list);
			STMT_LIST_NODE_PTR child = whiles->getWhileStatementListNode();
			Assert::IsTrue(std::addressof(*stmt_list) == std::addressof(*child));
		}
	};
}
