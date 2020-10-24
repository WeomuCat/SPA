#include "stdafx.h"
#include "CppUnitTest.h"
#include "PKB.h"
#include "PKBBuilder.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {		
	
	TEST_CLASS(ASTNodeTest) {
	public:
		PRINT_NODE_PTR print = std::make_shared<PrintNode>();
		VAR_NODE_PTR var = std::make_shared<VariableNode>();
		STMT_LIST_NODE_PTR stmt_list = std::make_shared<StatementListNode>();
		READ_NODE_PTR read = std::make_shared<ReadNode>();
		ASSIGN_NODE_PTR assign = std::make_shared<AssignNode>();
		EXPR_NODE_PTR expr = std::make_shared<ExpressionNode>();
		CALL_NODE_PTR call = std::make_shared<CallNode>();
		PROC_NODE_PTR proc = std::make_shared<ProcedureNode>();
		CONDITION_NODE_PTR cond = std::make_shared<ConditionNode>();
		RELATION_NODE_PTR relation = std::make_shared<RelationNode>();
		CONSTANT_NODE_PTR constant = std::make_shared<ConstantNode>();
		WHILE_NODE_PTR whiles = std::make_shared<WhileNode>();
		TRY_NODE_PTR trys = std::make_shared<TryNode>();
		PROGRAM_NODE_PTR program = std::make_shared<ProgramNode>();
		IF_NODE_PTR ifs = std::make_shared<IfNode>();

		TEST_METHOD(ASTNode_setgetParent_true) {
			Assert::IsTrue(var->getParentNode() == nullptr);
			print->setVariableNode(var);
			AST_NODE_PTR parent = var->getParentNode();
			Assert::IsTrue(std::addressof(*print) == std::addressof(*parent));
		}

		TEST_METHOD(ASTNode_setgetChildren_true) {
			Assert::IsTrue(var->getChildrenNode().size() == 0);
			print->setVariableNode(var);
			AST_NODE_PTR child = print->getChildrenNode().at(0);
			Assert::IsTrue(std::addressof(*var) == std::addressof(*child));
		}

		TEST_METHOD(StatementNode_setgetStatementNumber_true) {
			Assert::IsTrue(read->getStatementNumber() == NULL);
			int stmt_num = 5;
			read->setStatementNumber(stmt_num);
			Assert::IsTrue(stmt_num == read->getStatementNumber());
		}

		TEST_METHOD(StatementNode_setgetStatementListNode_true) {
			Assert::IsTrue(read->getParentStatementListNode() == nullptr);
			read->setStatementListNode(stmt_list);
			STMT_LIST_NODE_PTR parent = read->getParentStatementListNode();
			Assert::IsTrue(std::addressof(*stmt_list) == std::addressof(*parent));
		}

		TEST_METHOD(AssignNode_setgetVariableNode_true) {
			Assert::IsTrue(assign->getVariableNode() == nullptr);
			assign->setVariableNode(var);
			VAR_NODE_PTR child = assign->getVariableNode();
			Assert::IsTrue(std::addressof(*var) == std::addressof(*child));
		}

		TEST_METHOD(AssignNode_setgetExpressionNode_true) {
			Assert::IsTrue(assign->getExpressionNode() == nullptr);
			assign->setExpressionNode(expr);
			EXPR_NODE_PTR child = assign->getExpressionNode();
			Assert::IsTrue(std::addressof(*expr) == std::addressof(*child));
		}

		TEST_METHOD(CallNode_setgetCalleeProcedureName_true) {
			Assert::IsTrue(call->getCalleeProcedureName() == "");
			std::string value = "callee";
			call->setCalleeProcedureName(value);
			Assert::IsTrue(value == call->getCalleeProcedureName());
		}

		TEST_METHOD(CallNode_setgetCallerProcedureName_true) {
			Assert::IsTrue(call->getCallerProcedureName() == "");
			std::string value = "caller";
			call->setCallerProcedureName(value);
			Assert::IsTrue(value == call->getCallerProcedureName());
		}

		TEST_METHOD(CallNode_setgetProcedureNode_true) {
			Assert::IsTrue(call->getProcedureNode() == nullptr);
			call->setProcedureNode(proc);
			PROC_NODE_PTR callee_procedure = call->getProcedureNode();
			Assert::IsTrue(std::addressof(*proc) == std::addressof(*callee_procedure));
		}

		TEST_METHOD(ConditionNode_setgetConditionType_true) {
			Assert::IsTrue(CONDITION_TYPE::undefined == cond->getConditionType());
			cond->setConditionType(CONDITION_TYPE::and);
			Assert::IsTrue(CONDITION_TYPE::and == cond->getConditionType());
		}

		TEST_METHOD(ConditionNode_setgetLeftAstNode_true) {
			Assert::IsTrue(cond->getLeftAstNode() == nullptr);
			cond->setLeftAstNode(relation);
			AST_NODE_PTR child = cond->getLeftAstNode();
			Assert::IsTrue(std::addressof(*relation) == std::addressof(*child));
		}

		TEST_METHOD(ConditionNode_setgetRightAstNode_true) {
			auto func = [this] {cond->getRightAstNode() == nullptr;};
			Assert::ExpectException<std::exception>(func);
			cond->setRightAstNode(cond);
			AST_NODE_PTR child = cond->getRightAstNode();
			Assert::IsTrue(std::addressof(*cond) == std::addressof(*child));
		}

		TEST_METHOD(ConstantNode_setgetValue_true) {
			Assert::IsTrue(constant->getValue() == "");
			std::string value = "5";
			constant->setValue(value);
			Assert::IsTrue(value == constant->getValue());
		}

		TEST_METHOD(ExpressionNode_setgetExpressionType_true) {
			Assert::IsTrue(EXPR_TYPE::undefined == expr->getExpressionType());
			expr->setExpressionType(EXPR_TYPE::div);
			Assert::IsTrue(EXPR_TYPE::div == expr->getExpressionType());
		}

		TEST_METHOD(ExpressionNode_setgetLeftAstNode_true) {
			Assert::IsTrue(expr->getLeftAstNode() == nullptr);
			expr->setLeftAstNode(var);
			AST_NODE_PTR child = expr->getLeftAstNode();
			Assert::IsTrue(std::addressof(*var) == std::addressof(*child));
		}
		
		TEST_METHOD(ExpressionNode_setgetRightAstNode_true) {
			auto func = [this] {expr->getRightAstNode() == nullptr;};
			Assert::ExpectException<std::exception>(func);
			expr->setRightAstNode(constant);
			AST_NODE_PTR child = expr->getRightAstNode();
			Assert::IsTrue(std::addressof(*constant) == std::addressof(*child));
		}

		TEST_METHOD(IfNode_setgetConditionNode_true) {
			Assert::IsTrue(ifs->getConditionNode() == nullptr);
			ifs->setConditionNode(cond);
			CONDITION_NODE_PTR child = ifs->getConditionNode();
			Assert::IsTrue(std::addressof(*cond) == std::addressof(*child));
		}

		TEST_METHOD(IfNode_setgetThenStatementListNode_true) {
			Assert::IsTrue(ifs->getThenStatementListNode() == nullptr);
			ifs->setThenStatementListNode(stmt_list);
			STMT_LIST_NODE_PTR child = ifs->getThenStatementListNode();
			Assert::IsTrue(std::addressof(*stmt_list) == std::addressof(*child));
		}

		TEST_METHOD(IfNode_setgetElseStatementListNode_true) {
			Assert::IsTrue(ifs->getElseStatementListNode() == nullptr);
			ifs->setElseStatementListNode(stmt_list);
			STMT_LIST_NODE_PTR child = ifs->getElseStatementListNode();
			Assert::IsTrue(std::addressof(*stmt_list) == std::addressof(*child));
		}

		TEST_METHOD(PrintNode_setgetVariableName_true) {
			Assert::IsTrue(print->getVariableNode() == nullptr);
			print->setVariableNode(var);
			VAR_NODE_PTR child = print->getVariableNode();
			Assert::IsTrue(std::addressof(*var) == std::addressof(*child));
		}

		TEST_METHOD(ProcedureNode_setgetVariableName_true) {
			Assert::IsTrue(proc->getProcedureName() == "");
			std::string name = "procName";
			proc->setProcedureName(name);
			Assert::IsTrue(name == proc->getProcedureName());
		}

		TEST_METHOD(ProcedureNode_setgetProcedureStatementListNode_true) {
			Assert::IsTrue(proc->getProcedureStatementListNode() == nullptr);
			proc->setProcedureStatementListNode(stmt_list);
			STMT_LIST_NODE_PTR child = proc->getProcedureStatementListNode();
			Assert::IsTrue(std::addressof(*stmt_list) == std::addressof(*child));
		}

		TEST_METHOD(ProgramNode_setgetProcedureNodeList_true) {
			Assert::IsTrue(program->getProcedureNodeList().size() == 0);
			program->addProcedureNode(proc);
			PROC_NODE_PTR child = program->getProcedureNodeList().at(0);
			Assert::IsTrue(std::addressof(*proc) == std::addressof(*child));
		}

		TEST_METHOD(ReadNode_setgetVariableName_true) {
			Assert::IsTrue(read->getVariableNode() == nullptr);
			read->setVariableNode(var);
			VAR_NODE_PTR child = read->getVariableNode();
			Assert::IsTrue(std::addressof(*var) == std::addressof(*child));
		}

		TEST_METHOD(RelationNode_setgetRelationType_true) {
			Assert::IsTrue(RELATION_TYPE::undefined == relation->getRelationType());
			relation->setRelationType(RELATION_TYPE::eq);
			Assert::IsTrue(RELATION_TYPE::eq == relation->getRelationType());
		}

		TEST_METHOD(RelationNode_setgetLeftAstNode_true) {
			Assert::IsTrue(relation->getLeftAstNode() == nullptr);
			relation->setLeftAstNode(var);
			AST_NODE_PTR child = relation->getLeftAstNode();
			Assert::IsTrue(std::addressof(*var) == std::addressof(*child));
		}

		TEST_METHOD(RelationNode_setgetRightAstNode_true) {
			Assert::IsTrue(relation->getRightAstNode() == nullptr);
			relation->setRightAstNode(constant);
			AST_NODE_PTR child = relation->getRightAstNode();
			Assert::IsTrue(std::addressof(*constant) == std::addressof(*child));
		}

		TEST_METHOD(StatementListNode_setgetStatementNodeList_true) {
			Assert::IsTrue(stmt_list->getStatementNodeList().size() == 0);
			stmt_list->addStatementNode(assign);
			STMT_NODE_PTR child = stmt_list->getStatementNodeList().at(0);
			Assert::IsTrue(std::addressof(*assign) == std::addressof(*child));
		}

		TEST_METHOD(TryNode_setgetTryStatementListNode_true) {
			Assert::IsTrue(trys->getTryStatementListNode() == nullptr);
			trys->setTryStatementListNode(stmt_list);
			STMT_LIST_NODE_PTR child = trys->getTryStatementListNode();
			Assert::IsTrue(std::addressof(*stmt_list) == std::addressof(*child));
		}

		TEST_METHOD(TryNode_setgetCatchStatementListNode_true) {
			Assert::IsTrue(trys->getCatchStatementListNode() == nullptr);
			trys->setCatchStatementListNode(stmt_list);
			STMT_LIST_NODE_PTR child = trys->getCatchStatementListNode();
			Assert::IsTrue(std::addressof(*stmt_list) == std::addressof(*child));
		}

		TEST_METHOD(TryNode_setgetFinallyStatementListNode_true) {
			Assert::IsTrue(trys->getFinallyStatementListNode() == nullptr);
			trys->setFinallyStatementListNode(stmt_list);
			STMT_LIST_NODE_PTR child = trys->getFinallyStatementListNode();
			Assert::IsTrue(std::addressof(*stmt_list) == std::addressof(*child));
		}

		TEST_METHOD(VariableNode_setgetVariableName_true) {
			Assert::IsTrue(var->getVariableName() == "");
			std::string value = "x";
			var->setVariableName(value);
			Assert::IsTrue(value == var->getVariableName());
		}

		TEST_METHOD(WhileNode_setgetConditionNode_true) {
			Assert::IsTrue(whiles->getConditionNode() == nullptr);
			whiles->setConditionNode(cond);
			CONDITION_NODE_PTR child = whiles->getConditionNode();
			Assert::IsTrue(std::addressof(*cond) == std::addressof(*child));
		}

		TEST_METHOD(WhileNode_setgetWhileStatementListNode_true) {
			Assert::IsTrue(whiles->getWhileStatementListNode() == nullptr);
			whiles->setWhileStatementListNode(stmt_list);
			STMT_LIST_NODE_PTR child = whiles->getWhileStatementListNode();
			Assert::IsTrue(std::addressof(*stmt_list) == std::addressof(*child));
		}
	};
}
