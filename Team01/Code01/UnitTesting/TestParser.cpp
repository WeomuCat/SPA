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

		/*
		procedure simpleIf {
			if (a < b) then {
				read c;
			} else {
				print d;
			}
		}
		*/
		TEST_METHOD(parseIf_1) {
			Parser parser = Parser();
			PKB_PTR actual_pkb =
				std::make_shared<PKB>(parser.parseFile("../UnitTesting/Parser/TestParser-7.txt"));

			// Check program node is made
			PROGRAM_NODE_PTR actual_prog = actual_pkb->getProgramNode();
			PROGRAM_NODE_PTR prog = std::make_shared<ProgramNode>();
			Assert::IsTrue(typeid(prog) == typeid(actual_prog));

			// Check procedure list is made, with correct procedure name
			PROC_NODE_PTR_LIST actual_proc_list = actual_prog->getProcedureNodeList();
			PROC_NODE_PTR actual_proc = actual_proc_list.at(0);
			Assert::IsTrue(actual_proc->getProcedureName() == "simpleIf");

			// Check statementlist is made with 1 statement(children)
			STMT_LIST_NODE_PTR actual_stmtlist1 = actual_proc->getProcedureStatementListNode();
			STMT_NODE_PTR_LIST actual_stmt_list = actual_stmtlist1->getStatementNodeList();
			Assert::IsTrue(actual_stmt_list.size() == 1);

			// Check if node validity
			/*
				If node, condition node (a < b), stmtlist
				If -> condition(none) + stmtlist
				condition(none) -> rel(lt) on left
				rel(lt) -> var + var
			*/
			IfNode* actual_if = static_cast<IfNode*>(actual_stmt_list.at(0).get());
			CONDITION_NODE_PTR actual_cond = actual_if->getConditionNode();
			CONDITION_TYPE cond_type = actual_cond->getConditionType();			// none

			AST_NODE_PTR temp_rel = actual_cond->getLeftAstNode();				// lt
			RelationNode* actual_rel = static_cast<RelationNode*>(temp_rel.get());
			RELATION_TYPE rel_type = actual_rel->getRelationType();

			AST_NODE_PTR temp_var1 = actual_rel->getLeftAstNode();				// a
			VariableNode* actual_var1 = static_cast<VariableNode*>(temp_var1.get());
			AST_NODE_PTR temp_var2 = actual_rel->getRightAstNode();				// b
			VariableNode* actual_var2 = static_cast<VariableNode*>(temp_var2.get());

			Assert::IsTrue("a" == actual_var1->getVariableName());
			Assert::IsTrue(RelationTypeEnum::lt == rel_type);
			Assert::IsTrue("b" == actual_var2->getVariableName());
			Assert::IsTrue(1 == actual_if->getStatementNumber());

			// Check 2 stmtList nodes made
			STMT_LIST_NODE_PTR actual_then = actual_if->getThenStatementListNode();
			STMT_NODE_PTR_LIST actual_then_stmt_list = actual_then->getStatementNodeList();
			ReadNode* actual_read = static_cast<ReadNode*>(actual_then_stmt_list.at(0).get());
			AST_NODE_PTR temp_var3 = actual_read->getVariableNode();				// c
			VariableNode* actual_var3 = static_cast<VariableNode*>(temp_var3.get());

			STMT_LIST_NODE_PTR actual_else = actual_if->getElseStatementListNode();
			STMT_NODE_PTR_LIST actual_else_stmt_list = actual_else->getStatementNodeList();
			PrintNode* actual_print = static_cast<PrintNode*>(actual_else_stmt_list.at(0).get());
			AST_NODE_PTR temp_var4 = actual_print->getVariableNode();				// d
			VariableNode* actual_var4 = static_cast<VariableNode*>(temp_var4.get());

			Assert::IsTrue("c" == actual_var3->getVariableName());
			Assert::IsTrue("d" == actual_var4->getVariableName());
		}

		/*
		procedure nestedWhile {
			while (a < b) {
				print x;
				
				while (c > d) {
					read y;
				}
			}
		}
		*/
		TEST_METHOD(parseNestedWhile_1) {
			Parser parser = Parser();
			PKB_PTR actual_pkb =
				std::make_shared<PKB>(parser.parseFile("../UnitTesting/Parser/TestParser-8.txt"));

			// Check program node is made
			PROGRAM_NODE_PTR actual_prog = actual_pkb->getProgramNode();
			PROGRAM_NODE_PTR prog = std::make_shared<ProgramNode>();
			Assert::IsTrue(typeid(prog) == typeid(actual_prog));

			// Check procedure list is made, with correct procedure name
			PROC_NODE_PTR_LIST actual_proc_list = actual_prog->getProcedureNodeList();
			PROC_NODE_PTR actual_proc = actual_proc_list.at(0);
			Assert::IsTrue(actual_proc->getProcedureName() == "nestedWhile");

			// Check statementlist is made with 1 statement(children)
			STMT_LIST_NODE_PTR actual_stmtlist1 = actual_proc->getProcedureStatementListNode();
			STMT_NODE_PTR_LIST actual_stmt_list1 = actual_stmtlist1->getStatementNodeList();
			Assert::IsTrue(actual_stmt_list1.size() == 1);

			// Check first while node made
			WhileNode* actual_while1 = static_cast<WhileNode*>(actual_stmt_list1.at(0).get());
			CONDITION_NODE_PTR actual_cond1 = actual_while1->getConditionNode();
			CONDITION_TYPE cond_type = actual_cond1->getConditionType();			// none

			AST_NODE_PTR temp_rel1 = actual_cond1->getLeftAstNode();				// lt
			RelationNode* actual_rel1 = static_cast<RelationNode*>(temp_rel1.get());
			RELATION_TYPE rel_type1 = actual_rel1->getRelationType();

			AST_NODE_PTR temp_var1 = actual_rel1->getLeftAstNode();					// a
			VariableNode* actual_var1 = static_cast<VariableNode*>(temp_var1.get());
			AST_NODE_PTR temp_var2 = actual_rel1->getRightAstNode();				// b
			VariableNode* actual_var2 = static_cast<VariableNode*>(temp_var2.get());

			Assert::IsTrue("a" == actual_var1->getVariableName());
			Assert::IsTrue(RelationTypeEnum::lt == rel_type1);
			Assert::IsTrue("b" == actual_var2->getVariableName());
			Assert::IsTrue(1 == actual_while1->getStatementNumber());

			// Check print node
			STMT_LIST_NODE_PTR actual_stmtlist2 = actual_while1->getWhileStatementListNode();
			STMT_NODE_PTR_LIST actual_stmt_list2 = actual_stmtlist2->getStatementNodeList();
			PrintNode* actual_print = static_cast<PrintNode*>(actual_stmt_list2.at(0).get());
			AST_NODE_PTR temp_var3 = actual_print->getVariableNode();				// x
			VariableNode* actual_var3 = static_cast<VariableNode*>(temp_var3.get());

			Assert::IsTrue("x" == actual_var3->getVariableName());
			Assert::IsTrue(2 == actual_print->getStatementNumber());

			// Check second while node
			WhileNode* actual_while2 = static_cast<WhileNode*>(actual_stmt_list2.at(1).get());
			CONDITION_NODE_PTR actual_cond2 = actual_while2->getConditionNode();
			CONDITION_TYPE cond_type2 = actual_cond2->getConditionType();			// none

			AST_NODE_PTR temp_rel2 = actual_cond2->getLeftAstNode();				// gt
			RelationNode* actual_rel2 = static_cast<RelationNode*>(temp_rel2.get());
			RELATION_TYPE rel_type2 = actual_rel2->getRelationType();

			AST_NODE_PTR temp_var4 = actual_rel2->getLeftAstNode();					// c
			VariableNode* actual_var4 = static_cast<VariableNode*>(temp_var4.get());
			AST_NODE_PTR temp_var5 = actual_rel2->getRightAstNode();				// d
			VariableNode* actual_var5 = static_cast<VariableNode*>(temp_var5.get());

			Assert::IsTrue("c" == actual_var4->getVariableName());
			Assert::IsTrue(RelationTypeEnum::gt == rel_type2);
			Assert::IsTrue("d" == actual_var5->getVariableName());
			Assert::IsTrue(3 == actual_while2->getStatementNumber());

			// Check read node
			STMT_LIST_NODE_PTR actual_stmtlist3 = actual_while2->getWhileStatementListNode();
			STMT_NODE_PTR_LIST actual_stmt_list3 = actual_stmtlist3->getStatementNodeList();
			ReadNode* actual_read = static_cast<ReadNode*>(actual_stmt_list3.at(0).get());
			AST_NODE_PTR temp_var6 = actual_read->getVariableNode();				// y
			VariableNode* actual_var6 = static_cast<VariableNode*>(temp_var6.get());

			Assert::IsTrue("y" == actual_var6->getVariableName());
			Assert::IsTrue(4 == actual_read->getStatementNumber());
		}

		/*
		procedure proc1 {
			print a;
			call proc2;
			print a;
		}

		procedure proc2 {
			a = 5;
		}
		*/
		TEST_METHOD(parseCall_1) {
			Parser parser = Parser();
			PKB_PTR actual_pkb =
				std::make_shared<PKB>(parser.parseFile("../UnitTesting/Parser/TestParser-10.txt"));
			
			// Check program node is made
			PROGRAM_NODE_PTR actual_prog = actual_pkb->getProgramNode();
			PROGRAM_NODE_PTR prog = std::make_shared<ProgramNode>();
			Assert::IsTrue(typeid(prog) == typeid(actual_prog));

			// Check procedure lists are made, with correct procedure name
			PROC_NODE_PTR_LIST actual_proc_list = actual_prog->getProcedureNodeList();
			PROC_NODE_PTR actual_proc1 = actual_proc_list.at(0);
			Assert::IsTrue(actual_proc1->getProcedureName() == "proc1");

			PROC_NODE_PTR actual_proc2 = actual_proc_list.at(1);
			Assert::IsTrue(actual_proc2->getProcedureName() == "proc2");

			// Check statementlist for proc1 is made with 3 statement(children)
			STMT_LIST_NODE_PTR actual_stmtlist1 = actual_proc1->getProcedureStatementListNode();
			STMT_NODE_PTR_LIST actual_stmt_list1 = actual_stmtlist1->getStatementNodeList();
			Assert::IsTrue(actual_stmt_list1.size() == 3);

			// Check correctly formed CallNode
			CallNode* actual_call = static_cast<CallNode*>(actual_stmt_list1.at(1).get());
			
			Assert::IsTrue("proc1" == actual_call->getCallerProcedureName());
			Assert::IsTrue("proc2" == actual_call->getCalleeProcedureName());
		}

		/*
		procedure hasWhile {
			print a;
			while (b != 3) {
				c = d - 1;
			}
		}
		*/
		TEST_METHOD(parseParent_1) {
			Parser parser = Parser();
			PKB_PTR actual_pkb =
				std::make_shared<PKB>(parser.parseFile("../UnitTesting/Parser/TestParser-5.txt"));

			Assert::IsTrue(actual_pkb->isParent(2, 3));
			Assert::IsFalse(actual_pkb->isParent(1, 2));
			Assert::IsFalse(actual_pkb->isParent(1, 3));
		}

		TEST_METHOD(parseParent_2) {
			Parser parser = Parser();
			PKB_PTR actual_pkb =
				std::make_shared<PKB>(parser.parseFile("../UnitTesting/Parser/TestParser-9.txt"));

			Assert::IsTrue(actual_pkb->isParent(3, 4));
			Assert::IsTrue(actual_pkb->isParent(3, 5));
			Assert::IsTrue(actual_pkb->isParent(3, 6));
			Assert::IsTrue(actual_pkb->isParent(3, 7));
			Assert::IsTrue(actual_pkb->isParent(3, 8));
			Assert::IsTrue(actual_pkb->isParent(3, 9));
			Assert::IsTrue(actual_pkb->isParent(3, 10));

			Assert::IsFalse(actual_pkb->isParent(3, 11));

			Assert::IsTrue(actual_pkb->isParentTransitive(3, 10));
			Assert::IsTrue(actual_pkb->isParentTransitive(3, 11));
			Assert::IsTrue(actual_pkb->isParentTransitive(3, 12));
			Assert::IsTrue(actual_pkb->isParentTransitive(3, 13));
			Assert::IsTrue(actual_pkb->isParentTransitive(3, 14));
			Assert::IsTrue(actual_pkb->isParentTransitive(3, 15));
			Assert::IsTrue(actual_pkb->isParentTransitive(3, 16));
			Assert::IsTrue(actual_pkb->isParentTransitive(3, 17));
			Assert::IsTrue(actual_pkb->isParentTransitive(3, 18));
			Assert::IsTrue(actual_pkb->isParentTransitive(3, 19));
			Assert::IsTrue(actual_pkb->isParentTransitive(3, 20));

			Assert::IsFalse(actual_pkb->isParent(9, 10));
			Assert::IsFalse(actual_pkb->isParent(9, 11));
			Assert::IsFalse(actual_pkb->isParent(9, 12));

			Assert::IsFalse(actual_pkb->isParent(14, 15));
			Assert::IsFalse(actual_pkb->isParent(14, 17));
			Assert::IsFalse(actual_pkb->isParent(14, 18));
			Assert::IsFalse(actual_pkb->isParent(14, 19));
			Assert::IsFalse(actual_pkb->isParent(14, 20));
		}

		TEST_METHOD(parseCalls_1) {
			Parser parser = Parser();
			PKB_PTR actual_pkb =
				std::make_shared<PKB>(parser.parseFile("../UnitTesting/Parser/TestParser-11.txt"));
			Assert::IsTrue(actual_pkb->isCalls("proc1", "proc2"));
			Assert::IsFalse(actual_pkb->isCalls("proc2", "proc1"));
		}

		/*
		The slightly complex source
		1 calls 2, calls 3
		3 calls 4
		4 calls 2
		*/
		TEST_METHOD(parseCalls_2) {
			Parser parser = Parser();
			PKB_PTR actual_pkb =
				std::make_shared<PKB>(parser.parseFile("../UnitTesting/Parser/TestParser-9.txt"));
			
			Assert::IsTrue(actual_pkb->isCalls("proc1", "proc2"));
			Assert::IsTrue(actual_pkb->isCalls("proc1", "proc3"));

			Assert::IsFalse(actual_pkb->isCalls("proc1", "proc1"));
			Assert::IsFalse(actual_pkb->isCalls("proc1", "proc4"));

			Assert::IsFalse(actual_pkb->isCalls("proc2", "proc1"));
			Assert::IsFalse(actual_pkb->isCalls("proc2", "proc3"));
			Assert::IsFalse(actual_pkb->isCalls("proc2", "proc4"));

			Assert::IsTrue(actual_pkb->isCalls("proc3", "proc4"));
			
			Assert::IsFalse(actual_pkb->isCalls("proc3", "proc1"));
			Assert::IsFalse(actual_pkb->isCalls("proc3", "proc2"));
			Assert::IsFalse(actual_pkb->isCalls("proc3", "proc3"));

			Assert::IsTrue(actual_pkb->isCalls("proc4", "proc2"));

			Assert::IsFalse(actual_pkb->isCalls("proc4", "proc1"));
			Assert::IsFalse(actual_pkb->isCalls("proc4", "proc3"));
			Assert::IsFalse(actual_pkb->isCalls("proc4", "proc4"));
		}
		

		/*
		procedure hasWhile {
			print a;
			while (b != 3) {
				c = d - 1;
			}
		}
		*/
		TEST_METHOD(parseNext_1) {
			Parser parser = Parser();
			PKB_PTR actual_pkb =
				std::make_shared<PKB>(parser.parseFile("../UnitTesting/Parser/TestParser-5.txt"));
			
			Assert::IsTrue(actual_pkb->isNext(1, 2));
			Assert::IsTrue(actual_pkb->isNext(2, 3));

			Assert::IsFalse(actual_pkb->isNext(1, 3));

			Assert::IsTrue(actual_pkb->isNextTransitive(1, 3));
		}
		
		/*
		procedure nestedWhile {
			while (a < b) {
				print x;

				while (c > d) {
					read y;
				}
			}
		}
		*/
		TEST_METHOD(parseNext_2) {
			Parser parser = Parser();
			PKB_PTR actual_pkb =
				std::make_shared<PKB>(parser.parseFile("../UnitTesting/Parser/TestParser-8.txt"));
			
			Assert::IsTrue(actual_pkb->isNext(1, 2));
			Assert::IsTrue(actual_pkb->isNext(2, 3));
			Assert::IsTrue(actual_pkb->isNext(3, 4));
			
			Assert::IsTrue(actual_pkb->isNextTransitive(1, 2));
			Assert::IsTrue(actual_pkb->isNextTransitive(1, 3));
			Assert::IsTrue(actual_pkb->isNextTransitive(1, 4));
			
			Assert::IsTrue(actual_pkb->isNextTransitive(2, 3));
			Assert::IsTrue(actual_pkb->isNextTransitive(2, 4));
			
			Assert::IsTrue(actual_pkb->isNextTransitive(3, 4));
		}

		/*
			procedure proc1 {
		1		a = b + 1;
		2		print a;
		3		call proc2;

		4		while(a > 2) {
		5			print c;
		6			call proc2;
				}
			}

			procedure proc2 {
		7		read x;
		8		y = z + 2;
			}
		*/
		TEST_METHOD(parseUses_1) {
			Parser parser = Parser();
			PKB_PTR actual_pkb =
				std::make_shared<PKB>(parser.parseFile("../UnitTesting/Parser/TestParser-12.txt"));

			// Test Procedures (including transcending Uses due to call stmt)
			Assert::IsTrue(actual_pkb->isUses("proc1", "a"));
			Assert::IsTrue(actual_pkb->isUses("proc1", "b"));
			Assert::IsTrue(actual_pkb->isUses("proc1", "c"));
			Assert::IsTrue(actual_pkb->isUses("proc1", "z"));
			
			Assert::IsTrue(actual_pkb->isUses("proc2", "z"));

			// Test assign stmts
			Assert::IsTrue(actual_pkb->isUses(1, "b"));
			Assert::IsTrue(actual_pkb->isUses(6, "z"));
			
			Assert::IsFalse(actual_pkb->isUses(1, "a"));
			Assert::IsFalse(actual_pkb->isUses(6, "y"));

			// Test print stmt
			Assert::IsTrue(actual_pkb->isUses(2, "a"));
			Assert::IsTrue(actual_pkb->isUses(5, "c"));

			// Test container stmt (condition and stmt list and due to call stmt)
			Assert::IsTrue(actual_pkb->isUses(4, "a"));
			Assert::IsTrue(actual_pkb->isUses(4, "c"));
			Assert::IsTrue(actual_pkb->isUses(4, "z"));

			// Test call stmt
			Assert::IsTrue(actual_pkb->isUses(6, "z"));
		}

		/*
			procedure proc1 {
		1		a = b + 1;
		2		read a;
		3		call proc2;

		4		while(a > 2) {
		5			read c;
		6			call proc2;
				}
			}

			procedure proc2 {
		7		print x;
		8		y = z + 2;
			}
		*/
		TEST_METHOD(parseModifies_1) {
			Parser parser = Parser();
			PKB_PTR actual_pkb =
				std::make_shared<PKB>(parser.parseFile("../UnitTesting/Parser/TestParser-13.txt"));

			// Test Procedures (including transcending Uses due to call stmt)
			Assert::IsTrue(actual_pkb->isModifies("proc1", "a"));
			Assert::IsTrue(actual_pkb->isModifies("proc1", "c"));
			Assert::IsTrue(actual_pkb->isModifies("proc1", "y"));

			Assert::IsTrue(actual_pkb->isModifies("proc2", "y"));

			// Test assign stmts
			Assert::IsTrue(actual_pkb->isModifies(1, "a"));
			Assert::IsTrue(actual_pkb->isModifies(6, "y"));

			Assert::IsFalse(actual_pkb->isModifies(1, "b"));
			Assert::IsFalse(actual_pkb->isModifies(6, "z"));

			// Test print stmt
			Assert::IsTrue(actual_pkb->isModifies(2, "a"));
			Assert::IsTrue(actual_pkb->isModifies(5, "c"));

			// Test container stmt (condition and stmt list and due to call stmt)
			Assert::IsTrue(actual_pkb->isModifies(4, "c"));
			Assert::IsTrue(actual_pkb->isModifies(4, "y"));

			// Test call stmt
			Assert::IsTrue(actual_pkb->isModifies(6, "y"));
		}

		/*
			procedure main {
		1		call sub1;
		2		call sub2;
			}

			procedure sub1 {
		3		print x;
			}

			procedure sub2 {
		4		call subsub3;
		5		call subsub4;
			}

			procedure subsub3 {
		6		read y;
			}

			procedure subsub4 {
		7		call subsubsub5;
			}

			procedure subsubsub5 {
		8		print z;
			}
		*/
		// Commented out sections are asserts to do with variables used/modified in called procedure, and currently fail.
		TEST_METHOD(parseTopoSort_1) {
			Parser parser = Parser();
			PKB_PTR actual_pkb =
				std::make_shared<PKB>(parser.parseFile("../UnitTesting/Parser/BryanTestParserTopoSort.txt"));

			// Test procedure, call stmts in main
			Assert::IsTrue(actual_pkb->isCalls("main", "sub1"));
			Assert::IsTrue(actual_pkb->isCalls("main", "sub2"));
			
			Assert::IsTrue(actual_pkb->isCallsTransitive("main", "subsub3"));
			Assert::IsTrue(actual_pkb->isCallsTransitive("main", "subsub4"));
			Assert::IsTrue(actual_pkb->isCallsTransitive("main", "subsubsub5"));

			Assert::IsTrue(actual_pkb->isUses("main", "x"));
			Assert::IsTrue(actual_pkb->isModifies("main", "y"));
			Assert::IsTrue(actual_pkb->isUses("main", "z"));
			
			Assert::IsTrue(actual_pkb->isUses(1, "x"));
			Assert::IsTrue(actual_pkb->isModifies(2, "y"));
			Assert::IsTrue(actual_pkb->isUses(2, "z"));

			// Test procedure in sub1.
			Assert::IsTrue(actual_pkb->isUses("sub1", "x"));
			Assert::IsTrue(actual_pkb->isUses(3, "x"));

			// Test procedure, call stmts in sub2
			Assert::IsTrue(actual_pkb->isCalls("sub2", "subsub3"));
			Assert::IsTrue(actual_pkb->isCalls("sub2", "subsub4"));
			
			Assert::IsTrue(actual_pkb->isCallsTransitive("sub2", "subsubsub5"));

			Assert::IsTrue(actual_pkb->isModifies("sub2", "y"));
			Assert::IsTrue(actual_pkb->isUses("sub2", "z"));
			
			Assert::IsTrue(actual_pkb->isModifies(4, "y"));
			Assert::IsTrue(actual_pkb->isUses(5, "z"));

			// Test procedure in subsub3.
			Assert::IsTrue(actual_pkb->isModifies("subsub3", "y"));
			Assert::IsTrue(actual_pkb->isModifies(6, "y"));

			// Test procedure, call stmt in subsub4
			Assert::IsTrue(actual_pkb->isCalls("subsub4", "subsubsub5"));

			Assert::IsTrue(actual_pkb->isUses("subsub4", "z"));

			Assert::IsTrue(actual_pkb->isUses(7, "z"));

			// Test procedure in subsubsub5.
			Assert::IsTrue(actual_pkb->isUses("subsubsub5", "z"));
			Assert::IsTrue(actual_pkb->isUses(8, "z"));
		}

		//*************START TEST NEXT R/S*****************
		TEST_METHOD(parseNextIfEvenBranching) {
			Parser parser = Parser();
			PKB_PTR actual_pkb =
				std::make_shared<PKB>(parser.parseFile("../UnitTesting/Parser/TestParser-If Even Branching.txt"));

			std::set<std::pair<int, int>> valid_next;
			valid_next.insert(std::make_pair(1,2));
			valid_next.insert(std::make_pair(2, 3));
			valid_next.insert(std::make_pair(2, 4));
			valid_next.insert(std::make_pair(3, 5));
			valid_next.insert(std::make_pair(4, 5));

			for (int i = 1; i <= 6; i++) {
				for (int j = 1; j <= 6; j++) {
					if (valid_next.count(std::pair<int, int>(i, j)) > 0) {
						Assert::IsTrue(actual_pkb->isNext(i, j));
					} else {
						Assert::IsFalse(actual_pkb->isNext(i, j));
					}
				}
			}
		}

		TEST_METHOD(parseNextIfEvenBranchingMulti) {
			Parser parser = Parser();
			PKB_PTR actual_pkb =
				std::make_shared<PKB>(parser.parseFile("../UnitTesting/Parser/TestParser-If Even Branching Multi Line.txt"));

			std::set<std::pair<int, int>> valid_next;
			valid_next.insert(std::make_pair(1, 2));

			valid_next.insert(std::make_pair(2, 3));
			valid_next.insert(std::make_pair(3, 4));
			valid_next.insert(std::make_pair(2, 5));
			valid_next.insert(std::make_pair(5, 6));
			
			valid_next.insert(std::make_pair(4, 7));
			valid_next.insert(std::make_pair(6, 7));

			for (int i = 1; i <= 7; i++) {
				for (int j = 1; j <= 7; j++) {
					if (valid_next.count(std::pair<int, int>(i, j)) > 0) {
						Assert::IsTrue(actual_pkb->isNext(i, j));
					}
					else {
						Assert::IsFalse(actual_pkb->isNext(i, j));
					}
				}
			}
		}

		TEST_METHOD(parseNextIfUnevenLHSBranching) {
			Parser parser = Parser();
			PKB_PTR actual_pkb =
				std::make_shared<PKB>(parser.parseFile("../UnitTesting/Parser/TestParser-If Uneven LHS Branching.txt"));

			std::set<std::pair<int, int>> valid_next;
			valid_next.insert(std::make_pair(1, 2));

			
			valid_next.insert(std::make_pair(2, 3));
			valid_next.insert(std::make_pair(3, 4));
			
			valid_next.insert(std::make_pair(2, 5));
			

			valid_next.insert(std::make_pair(4, 6));
			valid_next.insert(std::make_pair(5, 6));

			for (int i = 1; i <= 6; i++) {
				for (int j = 1; j <= 6; j++) {
					
					if (valid_next.count(std::pair<int, int>(i, j)) > 0) {
						Logger::WriteMessage("Assert True");
						Logger::WriteMessage(":");
						Assert::IsTrue(actual_pkb->isNext(i, j));
						
					}
					else {
						Logger::WriteMessage("Assert False");
						Logger::WriteMessage(":");
						Assert::IsFalse(actual_pkb->isNext(i, j));
						
					}
					Logger::WriteMessage(std::to_string(i).c_str());
					Logger::WriteMessage(":");
					Logger::WriteMessage(std::to_string(j).c_str());
					Logger::WriteMessage("\n");
				}
			}
		}

		TEST_METHOD(parseNextIfUnevenRHSBranching) {
			Parser parser = Parser();
			PKB_PTR actual_pkb =
				std::make_shared<PKB>(parser.parseFile("../UnitTesting/Parser/TestParser-If Uneven RHS Branching.txt"));

			std::set<std::pair<int, int>> valid_next;
			valid_next.insert(std::make_pair(1, 2));

			valid_next.insert(std::make_pair(2, 3));
			valid_next.insert(std::make_pair(3, 4));

			valid_next.insert(std::make_pair(2, 5));
			valid_next.insert(std::make_pair(5, 6));
			valid_next.insert(std::make_pair(6, 7));

			valid_next.insert(std::make_pair(4, 8));
			valid_next.insert(std::make_pair(7, 8));

			for (int i = 1; i <= 8; i++) {
				for (int j = 1; j <= 8; j++) {
					if (valid_next.count(std::pair<int, int>(i, j)) > 0) {
						Assert::IsTrue(actual_pkb->isNext(i, j));
					}
					else {
						Assert::IsFalse(actual_pkb->isNext(i, j));
					}
				}
			}
		}

		TEST_METHOD(parseNextIfNested) {
			Parser parser = Parser();
			PKB_PTR actual_pkb =
				std::make_shared<PKB>(parser.parseFile("../UnitTesting/Parser/TestParser-If Nested.txt"));

			std::set<std::pair<int, int>> valid_next;
			valid_next.insert(std::make_pair(1, 2));

			//If 
			valid_next.insert(std::make_pair(2, 3));

			//Nested If
			valid_next.insert(std::make_pair(3, 4));
			
			//Nested Else
			valid_next.insert(std::make_pair(3, 5));
			valid_next.insert(std::make_pair(5, 6));
			
			//Else
			valid_next.insert(std::make_pair(2, 7));
			
			//Nested if
			valid_next.insert(std::make_pair(7, 8));
			valid_next.insert(std::make_pair(8, 9));
			valid_next.insert(std::make_pair(9, 10));

			//Nested Else
			valid_next.insert(std::make_pair(7, 11));
			valid_next.insert(std::make_pair(11, 12));
			valid_next.insert(std::make_pair(12, 13));
			valid_next.insert(std::make_pair(13, 14));

			//Out of Nested
			valid_next.insert(std::make_pair(4, 15));
			valid_next.insert(std::make_pair(6, 15));
			valid_next.insert(std::make_pair(10, 15));
			valid_next.insert(std::make_pair(14, 15));
			

			for (int i = 1; i <= 15; i++) {
				for (int j = 1; j <= 15; j++) {
					if (valid_next.count(std::pair<int, int>(i, j)) > 0) {
						Assert::IsTrue(actual_pkb->isNext(i, j));
					}
					else {
						Assert::IsFalse(actual_pkb->isNext(i, j));
					}
				}
			}
		}

		TEST_METHOD(parseNextWhile) {
			Parser parser = Parser();
			PKB_PTR actual_pkb =
				std::make_shared<PKB>(parser.parseFile("../UnitTesting/Parser/TestParser-While.txt"));

			std::set<std::pair<int, int>> valid_next;
			valid_next.insert(std::make_pair(1, 2));

			valid_next.insert(std::make_pair(2, 3));
			valid_next.insert(std::make_pair(3, 1));

			valid_next.insert(std::make_pair(1, 4));
			
			for (int i = 1; i <= 4; i++) {
				for (int j = 1; j <= 4; j++) {
					if (valid_next.count(std::pair<int, int>(i, j)) > 0) {
						Assert::IsTrue(actual_pkb->isNext(i, j));
					}
					else {
						Assert::IsFalse(actual_pkb->isNext(i, j));
					}
				}
			}
		}

		TEST_METHOD(parseNextWhileIf) {
			Parser parser = Parser();
			PKB_PTR actual_pkb =
				std::make_shared<PKB>(parser.parseFile("../UnitTesting/Parser/TestParser-While If.txt"));

			std::set<std::pair<int, int>> valid_next;
			valid_next.insert(std::make_pair(1, 2));

			//If
			valid_next.insert(std::make_pair(2, 3));
			valid_next.insert(std::make_pair(3, 4));

			//Else
			valid_next.insert(std::make_pair(2, 5));
			valid_next.insert(std::make_pair(5, 6));

			//Exit If
			valid_next.insert(std::make_pair(4, 1));
			valid_next.insert(std::make_pair(6, 1));

			//Exit While
			valid_next.insert(std::make_pair(1, 7));

			for (int i = 1; i <= 7; i++) {
				for (int j = 1; j <= 7; j++) {
					if (valid_next.count(std::pair<int, int>(i, j)) > 0) {
						Assert::IsTrue(actual_pkb->isNext(i, j));
					}
					else {
						Assert::IsFalse(actual_pkb->isNext(i, j));
					}
				}
			}
		}

		TEST_METHOD(parseNextWhileWhileIf) {
			Parser parser = Parser();
			PKB_PTR actual_pkb =
				std::make_shared<PKB>(parser.parseFile("../UnitTesting/Parser/TestParser-While While If.txt"));

			std::set<std::pair<int, int>> valid_next;
			valid_next.insert(std::make_pair(1, 2));
			valid_next.insert(std::make_pair(2, 1));
			
			valid_next.insert(std::make_pair(2, 3));

			//If
			valid_next.insert(std::make_pair(3, 4));

			//Else
			valid_next.insert(std::make_pair(3, 5));
			valid_next.insert(std::make_pair(5, 6));

			//Exit If
			valid_next.insert(std::make_pair(4, 2));
			valid_next.insert(std::make_pair(6, 2));

			//Exit While
			valid_next.insert(std::make_pair(1, 7));

			for (int i = 1; i <= 7; i++) {
				for (int j = 1; j <= 7; j++) {
					if (valid_next.count(std::pair<int, int>(i, j)) > 0) {
						Assert::IsTrue(actual_pkb->isNext(i, j));
					}
					else {
						Assert::IsFalse(actual_pkb->isNext(i, j));
					}
				}
			}
		}

		TEST_METHOD(parseNextWhileIfWhile) {
			Parser parser = Parser();
			PKB_PTR actual_pkb =
				std::make_shared<PKB>(parser.parseFile("../UnitTesting/Parser/TestParser-While If While.txt"));

			std::set<std::pair<int, int>> valid_next;
			valid_next.insert(std::make_pair(1, 2));
			
			//If
			valid_next.insert(std::make_pair(2, 3));
			valid_next.insert(std::make_pair(3, 4));

			//Nested While
			valid_next.insert(std::make_pair(4, 5));
			valid_next.insert(std::make_pair(5, 4));
			//Exit
			valid_next.insert(std::make_pair(4, 1));

			//Else
			valid_next.insert(std::make_pair(2, 6));
			valid_next.insert(std::make_pair(6, 7));
			//Nested While
			valid_next.insert(std::make_pair(7, 8));
			valid_next.insert(std::make_pair(8, 7));
			//Exit
			valid_next.insert(std::make_pair(7, 1));

			//Exit While
			valid_next.insert(std::make_pair(1, 9));

			for (int i = 1; i <= 9; i++) {
				for (int j = 1; j <= 9; j++) {
					if (valid_next.count(std::pair<int, int>(i, j)) > 0) {
						Assert::IsTrue(actual_pkb->isNext(i, j));
					}
					else {
						Assert::IsFalse(actual_pkb->isNext(i, j));
					}
				}
			}
		}

		TEST_METHOD(parseNextWhileNestedIf) {
			Parser parser = Parser();
			PKB_PTR actual_pkb =
				std::make_shared<PKB>(parser.parseFile("../UnitTesting/Parser/TestParser-While Nested If.txt"));

			std::set<std::pair<int, int>> valid_next;
			valid_next.insert(std::make_pair(1, 2));

			//If
			valid_next.insert(std::make_pair(2, 3));
			
			//Nested If
			valid_next.insert(std::make_pair(3, 4));
			
			//Nested Else
			valid_next.insert(std::make_pair(3, 5));

			//Else
			valid_next.insert(std::make_pair(2, 6));
			
			//Nested If
			valid_next.insert(std::make_pair(6, 7));
			//Nested Else
			valid_next.insert(std::make_pair(6, 8));

			//Exit from Nested
			valid_next.insert(std::make_pair(4, 1));
			valid_next.insert(std::make_pair(5, 1));
			valid_next.insert(std::make_pair(7, 1));
			valid_next.insert(std::make_pair(8, 1));

			//Exit While
			valid_next.insert(std::make_pair(1, 9));

			for (int i = 1; i <= 9; i++) {
				for (int j = 1; j <= 9; j++) {
					if (valid_next.count(std::pair<int, int>(i, j)) > 0) {
						Assert::IsTrue(actual_pkb->isNext(i, j));
					}
					else {
						Assert::IsFalse(actual_pkb->isNext(i, j));
					}
				}
			}
		}
		//*************END TEST NEXT R/S*****************
	};
}