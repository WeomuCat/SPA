#include <typeinfo>

#include "stdafx.h"
#include "CppUnitTest.h"

#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>

#include "..\source\Parser.h"

typedef std::shared_ptr<PKB> PKB_PTR;

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
	TEST_CLASS(TestParser) {
	public:
				// For testing if file path is correct
		bool is_file_exist(const char* fileName)
		{
			std::ifstream infile(fileName);
			return infile.good();
		}

		/*
		procedure main {
			a = 1;
			print b;
			read c;
		}
		*/
		TEST_METHOD(makeSimpleAst_1) {
			Parser parser = Parser();
			PKB_PTR actual_pkb =
				std::make_shared<PKB>(parser.parseFile("../UnitTesting/Parser/TestParser-1.txt"));

			// Check program node is made
			PROGRAM_NODE_PTR actual_prog = actual_pkb->getProgramNode();
			PROGRAM_NODE_PTR prog = std::make_shared<ProgramNode>();
			Assert::IsTrue(typeid(prog) == typeid(actual_prog));

			// Check procedure list is made, with correct procedure name
			PROC_NODE_PTR_LIST actual_proc_list = actual_prog->getProcedureNodeList();
			PROC_NODE_PTR actual_proc = actual_proc_list.at(0);
			Assert::IsTrue(actual_proc->getProcedureName() == "main");

			// Check statementlist is made with 3 statements(children)
			STMT_LIST_NODE_PTR actual_stmtlist = actual_proc->getProcedureStatementListNode();
			STMT_NODE_PTR_LIST actual_stmt_node_list = actual_stmtlist->getStatementNodeList();
			Assert::IsTrue(actual_stmt_node_list.size() == 3);

			// Check assign node validity (a=1)
			AssignNode* actual_assign = static_cast<AssignNode*>(actual_stmt_node_list.at(0).get());
			VAR_NODE_PTR actual_var1 = actual_assign->getVariableNode();		// a
			EXPR_NODE_PTR actual_expr = actual_assign->getExpressionNode();		// 1
			AST_NODE_PTR temp_const = actual_expr->getLeftAstNode();			
			ConstantNode* actual_const = static_cast<ConstantNode*>(temp_const.get());
			EXPR_TYPE actual_expr_type = actual_expr->getExpressionType();
			Assert::IsTrue("a" == actual_var1->getVariableName());
			Assert::IsTrue(EXPR_TYPE::none == actual_expr_type);
			Assert::IsTrue("1" == actual_const->getValue());
			Assert::IsTrue(1 == actual_assign->getStatementNumber());

			// Check print node (print b)
			PrintNode* actual_print = static_cast<PrintNode*>(actual_stmt_node_list.at(1).get());
			VAR_NODE_PTR actual_var2 = actual_print->getVariableNode();
			Assert::IsTrue("b" == actual_var2->getVariableName());
			Assert::IsTrue(2 == actual_print->getStatementNumber());

			// Check read node (read c)
			ReadNode* actual_read = static_cast<ReadNode*>(actual_stmt_node_list.at(2).get());
			VAR_NODE_PTR actual_var3 = actual_read->getVariableNode();
			Assert::IsTrue("c" == actual_var3->getVariableName());
			Assert::IsTrue(3 == actual_read->getStatementNumber());
		}

		/*
		procedure simple {
			x = a - b;
		}
		*/
		TEST_METHOD(makeSimpleAst_2) {
			Parser parser = Parser();
			PKB_PTR actual_pkb =
				std::make_shared<PKB>(parser.parseFile("../UnitTesting/Parser/TestParser-2.txt"));

			// Check program node is made
			PROGRAM_NODE_PTR actual_prog = actual_pkb->getProgramNode();
			PROGRAM_NODE_PTR prog = std::make_shared<ProgramNode>();
			Assert::IsTrue(typeid(prog) == typeid(actual_prog));

			// Check procedure list is made, with correct procedure name
			PROC_NODE_PTR_LIST actual_proc_list = actual_prog->getProcedureNodeList();
			PROC_NODE_PTR actual_proc = actual_proc_list.at(0);
			Assert::IsTrue(actual_proc->getProcedureName() == "simple");

			// Check statementlist is made with 1 statement(children)
			STMT_LIST_NODE_PTR actual_stmtlist = actual_proc->getProcedureStatementListNode();
			STMT_NODE_PTR_LIST actual_stmt_list = actual_stmtlist->getStatementNodeList();
			Assert::IsTrue(actual_stmt_list.size() == 1);

			// Check assign node validity (x = a - b)
			AssignNode* actual_assign = static_cast<AssignNode*>(actual_stmt_list.at(0).get());
			VAR_NODE_PTR actual_var1 = actual_assign->getVariableNode();

			EXPR_NODE_PTR actual_expr = actual_assign->getExpressionNode();
			EXPR_TYPE actual_expr_type = actual_expr->getExpressionType();

			AST_NODE_PTR temp_var2 = actual_expr->getLeftAstNode();
			VariableNode* actual_var2 = static_cast<VariableNode*>(temp_var2.get());
			AST_NODE_PTR temp_var3 = actual_expr->getRightAstNode();
			VariableNode* actual_var3 = static_cast<VariableNode*>(temp_var3.get());

			Assert::IsTrue("x" == actual_var1->getVariableName());
			Assert::IsTrue(EXPR_TYPE::min == actual_expr_type);
			Assert::IsTrue("a" == actual_var2->getVariableName());
			Assert::IsTrue("b" == actual_var3->getVariableName());
			Assert::IsTrue(1 == actual_assign->getStatementNumber());
		}

		/*
		procedure minimised {
			print x; x = a * 10 / b;}
		*/
		// To ensure AST can handle slightly white space-reduced code
		TEST_METHOD(makeSimpleAst_3) {
			Parser parser = Parser();
			PKB_PTR actual_pkb =
				std::make_shared<PKB>(parser.parseFile("../UnitTesting/Parser/TestParser-3.txt"));

			// Check program node is made
			PROGRAM_NODE_PTR actual_prog = actual_pkb->getProgramNode();
			PROGRAM_NODE_PTR prog = std::make_shared<ProgramNode>();
			Assert::IsTrue(typeid(prog) == typeid(actual_prog));

			// Check procedure list is made, with correct procedure name
			PROC_NODE_PTR_LIST actual_proc_list = actual_prog->getProcedureNodeList();
			PROC_NODE_PTR actual_proc = actual_proc_list.at(0);
			Assert::IsTrue(actual_proc->getProcedureName() == "minimised");

			// Check statementlist is made with 2 statement(children)
			STMT_LIST_NODE_PTR actual_stmtlist = actual_proc->getProcedureStatementListNode();
			STMT_NODE_PTR_LIST actual_stmt_list = actual_stmtlist->getStatementNodeList();
			Assert::IsTrue(actual_stmt_list.size() == 2);

			// Check print node validity (print x)
			PrintNode* actual_print = static_cast<PrintNode*>(actual_stmt_list.at(0).get());
			VAR_NODE_PTR actual_var1 = actual_print->getVariableNode();
			Assert::IsTrue("x" == actual_var1->getVariableName());
			Assert::IsTrue(1 == actual_print->getStatementNumber());

			// Check assign node validity (x = a - 10 / b)
			AssignNode* actual_assign = static_cast<AssignNode*>(actual_stmt_list.at(1).get());
			VAR_NODE_PTR actual_var2 = actual_assign->getVariableNode();		// x

			EXPR_NODE_PTR actual_expr1 = actual_assign->getExpressionNode();	// min
			EXPR_TYPE actual_expr1_type = actual_expr1->getExpressionType();

			AST_NODE_PTR temp_var3 = actual_expr1->getLeftAstNode();			// a
			VariableNode* actual_var3 = static_cast<VariableNode*>(temp_var3.get());
			AST_NODE_PTR temp_expr2 = actual_expr1->getRightAstNode();			// div
			ExpressionNode* actual_expr2 = static_cast<ExpressionNode*>(temp_expr2.get());
			EXPR_TYPE actual_expr2_type = actual_expr2->getExpressionType();

			AST_NODE_PTR temp_const = actual_expr2->getLeftAstNode();			// 10
			ConstantNode* actual_const = static_cast<ConstantNode*>(temp_const.get());
			AST_NODE_PTR temp_var4 = actual_expr2->getRightAstNode();			// b
			VariableNode* actual_var4 = static_cast<VariableNode*>(temp_var4.get());
			
			Assert::IsTrue("x" == actual_var2->getVariableName());
			Assert::IsTrue("a" == actual_var3->getVariableName());
			Assert::IsTrue("b" == actual_var4->getVariableName());

			Assert::IsTrue(EXPR_TYPE::min == actual_expr1_type);
			Assert::IsTrue(EXPR_TYPE::div == actual_expr2_type);

			Assert::IsTrue("10" == actual_const->getValue());
			Assert::IsTrue(2 == actual_assign->getStatementNumber());
		}
		/*
		procedure proc1 {
			read a;
		}
		procedure proc2 {
			read b;
		}
		procedure proc3 {
			read b;
		}
		*/
		// To ensure parser can correctly form multiple procedure nodes
		TEST_METHOD(multipleProcedure_1) {
			Parser parser = Parser();
			PKB_PTR actual_pkb =
				std::make_shared<PKB>(parser.parseFile("../UnitTesting/Parser/TestParser-4.txt"));
			PROC_NODE_PTR_LIST actual_proc_ptr_list = actual_pkb->getProcedures();

			Assert::IsTrue(actual_proc_ptr_list.size() == 3);
			Assert::IsTrue("proc1" == actual_proc_ptr_list.at(0)->getProcedureName());
			Assert::IsTrue("proc2" == actual_proc_ptr_list.at(1)->getProcedureName());
			Assert::IsTrue("proc3" == actual_proc_ptr_list.at(2)->getProcedureName());
		}

		/*
		procedure hasWhile {
			print a;
			while (b != 3) {
				c = d - 1;
			}
		}
		*/
		// To ensure parser can correctly form while loops
		// neq condition used here
		TEST_METHOD(parsewhile_1) {
			Parser parser = Parser();
			PKB_PTR actual_pkb =
				std::make_shared<PKB>(parser.parseFile("../UnitTesting/Parser/TestParser-5.txt"));

			// Check program node is made
			PROGRAM_NODE_PTR actual_prog = actual_pkb->getProgramNode();
			PROGRAM_NODE_PTR prog = std::make_shared<ProgramNode>();
			Assert::IsTrue(typeid(prog) == typeid(actual_prog));

			// Check procedure list is made, with correct procedure name
			PROC_NODE_PTR_LIST actual_proc_list = actual_prog->getProcedureNodeList();
			PROC_NODE_PTR actual_proc = actual_proc_list.at(0);
			Assert::IsTrue(actual_proc->getProcedureName() == "hasWhile");

			// Check statementlist is made with 2 statement(children)
			STMT_LIST_NODE_PTR actual_stmtlist1 = actual_proc->getProcedureStatementListNode();
			STMT_NODE_PTR_LIST actual_stmt_list = actual_stmtlist1->getStatementNodeList();
			Assert::IsTrue(actual_stmt_list.size() == 2);

			// Check print node validity (print a)
			PrintNode* actual_print = static_cast<PrintNode*>(actual_stmt_list.at(0).get());
			VAR_NODE_PTR actual_var1 = actual_print->getVariableNode();
			Assert::IsTrue("a" == actual_var1->getVariableName());
			Assert::IsTrue(1 == actual_print->getStatementNumber());

			// Check while node validity 
			/*
				While node, condition node (b != 3), stmtlist
				While -> condition(none) + stmtlist
				condition(none) -> rel(neq) on left
				rel(neq) -> var + const
			*/
			WhileNode* actual_while = static_cast<WhileNode*>(actual_stmt_list.at(1).get());
			CONDITION_NODE_PTR actual_cond = actual_while->getConditionNode();
			CONDITION_TYPE cond_type = actual_cond->getConditionType();			// none

			AST_NODE_PTR temp_rel = actual_cond->getLeftAstNode();				// neq
			RelationNode* actual_rel = static_cast<RelationNode*>(temp_rel.get());
			RELATION_TYPE rel_type = actual_rel->getRelationType();

			AST_NODE_PTR temp_var2 = actual_rel->getLeftAstNode();				// b
			VariableNode* actual_var2 = static_cast<VariableNode*>(temp_var2.get());
			AST_NODE_PTR temp_const1 = actual_rel->getRightAstNode();			// 3
			ConstantNode* actual_const1 = static_cast<ConstantNode*>(temp_const1.get());
			
			Assert::IsTrue("b" == actual_var2->getVariableName());
			Assert::IsTrue(RelationTypeEnum::neq == rel_type);
			Assert::IsTrue("3" == actual_const1->getValue());
			Assert::IsTrue(2 == actual_while->getStatementNumber());

			// Check stmtlist validity (size 1)
			STMT_LIST_NODE_PTR actual_stmtlist2 = actual_while->getWhileStatementListNode();
			STMT_NODE_PTR_LIST actual_stmt_list2 = actual_stmtlist2->getStatementNodeList();
			Assert::IsTrue(1 == actual_stmt_list2.size());
		}

		/*
		procedure complexCond {
			while ((a<b) && (!(b==c))) || (d>e) {
				read x;
			}
		}
		*/
		// To ensure parser can correctly parse more complex conditions
		TEST_METHOD(parsewhile_2) {
			Parser parser = Parser();
			PKB_PTR actual_pkb =
				std::make_shared<PKB>(parser.parseFile("../UnitTesting/Parser/TestParser-6.txt"));

			// Check program node is made
			PROGRAM_NODE_PTR actual_prog = actual_pkb->getProgramNode();
			PROGRAM_NODE_PTR prog = std::make_shared<ProgramNode>();
			Assert::IsTrue(typeid(prog) == typeid(actual_prog));

			// Check procedure list is made, with correct procedure name
			PROC_NODE_PTR_LIST actual_proc_list = actual_prog->getProcedureNodeList();
			PROC_NODE_PTR actual_proc = actual_proc_list.at(0);
			Assert::IsTrue(actual_proc->getProcedureName() == "complexCond");

			// Check statementlist is made with 1 statement(children)
			STMT_LIST_NODE_PTR actual_stmtlist1 = actual_proc->getProcedureStatementListNode();
			STMT_NODE_PTR_LIST actual_stmt_list = actual_stmtlist1->getStatementNodeList();
			Assert::IsTrue(actual_stmt_list.size() == 1);

			// Check while node validity 
			/*
				While node, condition node ((a<b) && (!(b==c))) || (d>e), stmtlist
				While -> condition(or) + stmtlist
				cond1(or) -> cond(and) + rel(gt)
				cond2(and) -> rel(lt) + cond(not)
				rel1(gt) -> var(d) + var(e)
				rel2(lt) -> var(a) + var(b)
				cond3(not) -> rel(eq) on left
				rel3(eq) -> var(b) + var(c)
			*/
			WhileNode* actual_while = static_cast<WhileNode*>(actual_stmt_list.at(0).get());
			CONDITION_NODE_PTR actual_cond1 = actual_while->getConditionNode();
			CONDITION_TYPE cond1_type = actual_cond1->getConditionType();			// or
			Assert::IsTrue(ConditionTypeEnum::or == cond1_type);
			
			AST_NODE_PTR temp_cond2 = actual_cond1->getLeftAstNode();				// and
			ConditionNode* actual_cond2 = static_cast<ConditionNode*>(temp_cond2.get());
			CONDITION_TYPE cond2_type = actual_cond2->getConditionType();
			Assert::IsTrue(ConditionTypeEnum:: and == cond2_type);

			AST_NODE_PTR temp_rel1 = actual_cond1->getRightAstNode();				//gt
			RelationNode* actual_rel1 = static_cast<RelationNode*>(temp_rel1.get());
			RELATION_TYPE rel1_type = actual_rel1->getRelationType();
			Assert::IsTrue(RelationTypeEnum::gt == rel1_type);
			
			AST_NODE_PTR temp_rel2 = actual_cond2->getLeftAstNode();				//lt
			RelationNode* actual_rel2 = static_cast<RelationNode*>(temp_rel2.get());
			RELATION_TYPE rel2_type = actual_rel2->getRelationType();
			Assert::IsTrue(RelationTypeEnum::lt == rel2_type);

			AST_NODE_PTR temp_cond3 = actual_cond2->getRightAstNode();				// not
			ConditionNode* actual_cond3 = static_cast<ConditionNode*>(temp_cond3.get());
			CONDITION_TYPE cond3_type = actual_cond3->getConditionType();
			Assert::IsTrue(ConditionTypeEnum::not == cond3_type);

			AST_NODE_PTR temp_rel3 = actual_cond3->getLeftAstNode();				// eq
			RelationNode* actual_rel3 = static_cast<RelationNode*>(temp_rel3.get());
			RELATION_TYPE rel3_type = actual_rel3->getRelationType();
			Assert::IsTrue(RelationTypeEnum::eq == rel3_type);
		}


	};
}