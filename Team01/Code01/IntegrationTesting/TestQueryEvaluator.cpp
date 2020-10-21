#include "stdafx.h"
#include "CppUnitTest.h"

#include "../source/QueryEvaluator.h"
#include "../source/QueryNode.h"
#include "../source/QueryNodeType.h"
#include "../source/PKB/ASTNode/PrintNode.h"
#include "../source/PKB/ASTNode/StatementNode.h"
#include "../source/PKB/ASTNode/VariableNode.h"
#include "../source/PKBBuilder.h"
#include "../source/PKB.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace IntegrationTesting
{
	TEST_CLASS(TestQueryEvaluator)
	{
	public:
		/* PKB 1:
		procedure main {
			print a;
			print b;
			print c;
			print d;
			print e;
		}
		*/
		std::shared_ptr<PKB> pkb;

		/* PKB 2:
		procedure meow {
			a = woof;
			b = a + b + c;
			c = 2 / (b + 3) % d + o * g;
		}
		*/
		std::shared_ptr<PKB> pkb2;

		/* PKB 3: For pattern only!!
		procedure woof {
			while (meow > 0)
			while (!(a + b == c + d))
			if (a <= 3)
			if ((c - a / t != 1) && (d + o * g != 2))
		}
		*/
		std::shared_ptr<PKB> pkb3;

		/* PKB 4: 
		procedure main {
			calls morning;			1
			calls afternoon;		2
			calls evening;			3
			calls night;			4
		}
		procedure morning {
			calls breakfast;		5
		}
		procedure afternoon {
			calls lunch;			6
		}
		procedure evening {
			calls dinner;			7
		}
		procedure night {
			calls diarrhoea;		8
		}
		procedure breakfast {
			read newspaper;			9
		}
		procedure lunch {
			read menu;				10
		}
		procedure dinner {
			read menu;				11	
		}
		procedure diarrhoea {
			pain = 100;				12
		}
		*/
		std::shared_ptr<PKB> pkb4;


		// MULTIPLE CURSOR INSTRUCTIONS :)
		// 1. hold ALT
		// 2. click and drag mouse
		// 3. release mousey and then release ALT
		// 4. then there should be multiple cursors!

		TEST_METHOD_INITIALIZE(PKBInitialize) {
			{
				// PKB 1
				PKBBuilder builder;

				std::shared_ptr<ProcedureNode> proc = std::make_shared<ProcedureNode>();
				proc->setProcedureName("main");
				builder.addProcedureNode(proc);

				std::shared_ptr<VariableNode> v1 = std::make_shared<VariableNode>();
				std::shared_ptr<VariableNode> v2 = std::make_shared<VariableNode>();
				std::shared_ptr<VariableNode> v3 = std::make_shared<VariableNode>();
				std::shared_ptr<VariableNode> v4 = std::make_shared<VariableNode>();
				std::shared_ptr<VariableNode> v5 = std::make_shared<VariableNode>();
				v1->setVariableName("a");
				v2->setVariableName("b");
				v3->setVariableName("c");
				v4->setVariableName("d");
				v5->setVariableName("e");
				builder.addVariableNode(v1);
				builder.addVariableNode(v2);
				builder.addVariableNode(v3);
				builder.addVariableNode(v4);
				builder.addVariableNode(v5);

				std::shared_ptr<PrintNode> p1 = std::make_shared<PrintNode>();
				std::shared_ptr<PrintNode> p2 = std::make_shared<PrintNode>();
				std::shared_ptr<PrintNode> p3 = std::make_shared<PrintNode>();
				std::shared_ptr<PrintNode> p4 = std::make_shared<PrintNode>();
				std::shared_ptr<PrintNode> p5 = std::make_shared<PrintNode>();
				p1->setStatementNumber(1);
				p1->setVariableNode(v1);
				p2->setStatementNumber(2);
				p2->setVariableNode(v2);
				p3->setStatementNumber(3);
				p3->setVariableNode(v3);
				p4->setStatementNumber(4);
				p4->setVariableNode(v4);
				p5->setStatementNumber(5);
				p5->setVariableNode(v5);
				builder.addPrintNode(p1);
				builder.addPrintNode(p2);
				builder.addPrintNode(p3);
				builder.addPrintNode(p4);
				builder.addPrintNode(p5);

				std::shared_ptr<StatementNode> s1 = std::make_shared<StatementNode>();
				std::shared_ptr<StatementNode> s2 = std::make_shared<StatementNode>();
				std::shared_ptr<StatementNode> s3 = std::make_shared<StatementNode>();
				std::shared_ptr<StatementNode> s4 = std::make_shared<StatementNode>();
				std::shared_ptr<StatementNode> s5 = std::make_shared<StatementNode>();
				s1->setStatementNumber(1);
				s2->setStatementNumber(2);
				s3->setStatementNumber(3);
				s4->setStatementNumber(4);
				s5->setStatementNumber(5);
				builder.addStatementNode(s1);
				builder.addStatementNode(s2);
				builder.addStatementNode(s3);
				builder.addStatementNode(s4);
				builder.addStatementNode(s5);

				builder.addFollows(1, 2);
				builder.addFollows(2, 3);
				builder.addFollows(3, 4);
				builder.addFollows(4, 5);

				builder.addUses("main", "a");
				builder.addUses("main", "b");
				builder.addUses("main", "c");
				builder.addUses("main", "d");
				builder.addUses("main", "e");

				builder.addUses(1, "a");
				builder.addUses(2, "b");
				builder.addUses(3, "c");
				builder.addUses(4, "d");
				builder.addUses(5, "e");

				pkb = std::make_shared<PKB>(builder.build());
			}


			{
				// PKB 2
				PKBBuilder builder;

				std::shared_ptr<ProcedureNode> proc = std::make_shared<ProcedureNode>();
				proc->setProcedureName("meow");
				builder.addProcedureNode(proc);

				std::shared_ptr<VariableNode> v1 = std::make_shared<VariableNode>();
				std::shared_ptr<VariableNode> v2 = std::make_shared<VariableNode>();
				std::shared_ptr<VariableNode> v3 = std::make_shared<VariableNode>();
				std::shared_ptr<VariableNode> v4 = std::make_shared<VariableNode>();
				std::shared_ptr<VariableNode> v5 = std::make_shared<VariableNode>();
				std::shared_ptr<VariableNode> v6 = std::make_shared<VariableNode>();
				std::shared_ptr<VariableNode> v7 = std::make_shared<VariableNode>();
				std::shared_ptr<VariableNode> v8 = std::make_shared<VariableNode>();
				std::shared_ptr<VariableNode> v9 = std::make_shared<VariableNode>();
				std::shared_ptr<VariableNode> v10 = std::make_shared<VariableNode>();
				std::shared_ptr<VariableNode> v11 = std::make_shared<VariableNode>();
				// a = woof
				v1->setVariableName("a");
				v2->setVariableName("woof");
				// b = a + b + c
				v3->setVariableName("b");
				v4->setVariableName("a");
				v5->setVariableName("b");
				v6->setVariableName("c");
				// c = 2 / (b + 3) % d + o * g;
				v7->setVariableName("c");
				v8->setVariableName("b");
				v9->setVariableName("d");
				v10->setVariableName("o");
				v11->setVariableName("g");
				builder.addVariableNode(v1);
				builder.addVariableNode(v2);
				builder.addVariableNode(v3);
				builder.addVariableNode(v4);
				builder.addVariableNode(v5);
				builder.addVariableNode(v6);
				builder.addVariableNode(v7);
				builder.addVariableNode(v8);
				builder.addVariableNode(v9);
				builder.addVariableNode(v10);
				builder.addVariableNode(v11);

				std::shared_ptr<ConstantNode> c1 = std::make_shared<ConstantNode>();
				std::shared_ptr<ConstantNode> c2 = std::make_shared<ConstantNode>();
				c1->setValue("2");
				c2->setValue("3");
				builder.addConstantNode(c1);
				builder.addConstantNode(c2);

				std::shared_ptr<ExpressionNode> e1 = std::make_shared<ExpressionNode>();
				std::shared_ptr<ExpressionNode> e2 = std::make_shared<ExpressionNode>();
				std::shared_ptr<ExpressionNode> e3 = std::make_shared<ExpressionNode>();
				std::shared_ptr<ExpressionNode> e4 = std::make_shared<ExpressionNode>();
				std::shared_ptr<ExpressionNode> e5 = std::make_shared<ExpressionNode>();
				std::shared_ptr<ExpressionNode> e6 = std::make_shared<ExpressionNode>();
				std::shared_ptr<ExpressionNode> e7 = std::make_shared<ExpressionNode>();
				std::shared_ptr<ExpressionNode> e8 = std::make_shared<ExpressionNode>();

				//Expression node of no expression, only 1 child (left child) ASTNode woof
				e1->setExpressionType(ExpressionTypeEnum::none);
				e1->setLeftAstNode(v2);

				// a + b
				e2->setExpressionType(ExpressionTypeEnum::plus);
				e2->setLeftAstNode(v4);
				e2->setRightAstNode(v5);

				// e2 + c
				e3->setExpressionType(ExpressionTypeEnum::plus);
				e3->setLeftAstNode(e2);
				e3->setRightAstNode(v6);

				// o * g
				e4->setExpressionType(ExpressionTypeEnum::times);
				e4->setLeftAstNode(v10);
				e4->setRightAstNode(v11);

				// b + 3
				e5->setExpressionType(ExpressionTypeEnum::plus);
				e5->setLeftAstNode(v8);
				e5->setRightAstNode(c2);

				// 2 / e5
				e6->setExpressionType(ExpressionTypeEnum::div);
				e6->setLeftAstNode(c1);
				e6->setRightAstNode(e5);

				// e6 % d
				e7->setExpressionType(ExpressionTypeEnum::mod);
				e7->setLeftAstNode(e6);
				e7->setRightAstNode(v9);

				// e7 + e4
				e8->setExpressionType(ExpressionTypeEnum::plus);
				e8->setLeftAstNode(e7);
				e8->setRightAstNode(e4);

				std::shared_ptr<AssignNode> a1 = std::make_shared<AssignNode>();
				std::shared_ptr<AssignNode> a2 = std::make_shared<AssignNode>();
				std::shared_ptr<AssignNode> a3 = std::make_shared<AssignNode>();
				a1->setVariableNode(v1);
				a1->setExpressionNode(e1);
				a1->setStatementNumber(1);
				a2->setVariableNode(v3);
				a2->setExpressionNode(e3);
				a2->setStatementNumber(2);
				a3->setVariableNode(v7);
				a3->setExpressionNode(e8);
				a3->setStatementNumber(3);
				builder.addAssignNode(a1);
				builder.addAssignNode(a2);
				builder.addAssignNode(a3);

				std::shared_ptr<StatementNode> s1 = std::make_shared<StatementNode>();
				std::shared_ptr<StatementNode> s2 = std::make_shared<StatementNode>();
				std::shared_ptr<StatementNode> s3 = std::make_shared<StatementNode>();
				s1->setStatementNumber(1);
				s2->setStatementNumber(2);
				s3->setStatementNumber(3);
				builder.addStatementNode(s1);
				builder.addStatementNode(s2);
				builder.addStatementNode(s3);

				builder.addFollows(1, 2);
				builder.addFollows(2, 3);

				builder.addUses(1, "woof");
				builder.addUses(2, "a");
				builder.addUses(2, "b");
				builder.addUses(2, "c");
				builder.addUses(3, "b");
				builder.addUses(3, "d");
				builder.addUses(3, "o");
				builder.addUses(3, "g");

				builder.addUses("meow", "woof");
				builder.addUses("meow", "a");
				builder.addUses("meow", "b");
				builder.addUses("meow", "c");
				builder.addUses("meow", "d");
				builder.addUses("meow", "o");
				builder.addUses("meow", "g");

				builder.addModifies(1, "a");
				builder.addModifies(2, "b");
				builder.addModifies(3, "c");

				builder.addModifies("meow", "a");
				builder.addModifies("meow", "b");
				builder.addModifies("meow", "c");

				pkb2 = std::make_shared<PKB>(builder.build());
			}

			{
				// PKB 3
				PKBBuilder builder;

				std::shared_ptr<ProcedureNode> proc = std::make_shared<ProcedureNode>();
				proc->setProcedureName("woof");
				builder.addProcedureNode(proc);

				std::shared_ptr<VariableNode> v1 = std::make_shared<VariableNode>();
				std::shared_ptr<VariableNode> v2 = std::make_shared<VariableNode>();
				std::shared_ptr<VariableNode> v3 = std::make_shared<VariableNode>();
				std::shared_ptr<VariableNode> v4 = std::make_shared<VariableNode>();
				std::shared_ptr<VariableNode> v5 = std::make_shared<VariableNode>();
				std::shared_ptr<VariableNode> v6 = std::make_shared<VariableNode>();
				std::shared_ptr<VariableNode> v7 = std::make_shared<VariableNode>();
				std::shared_ptr<VariableNode> v8 = std::make_shared<VariableNode>();
				std::shared_ptr<VariableNode> v9 = std::make_shared<VariableNode>();
				std::shared_ptr<VariableNode> v10 = std::make_shared<VariableNode>();
				std::shared_ptr<VariableNode> v11 = std::make_shared<VariableNode>();
				std::shared_ptr<VariableNode> v12 = std::make_shared<VariableNode>();
				v1->setVariableName("meow");
				v2->setVariableName("a");
				v3->setVariableName("b");
				v4->setVariableName("c");
				v5->setVariableName("d");
				v6->setVariableName("a");
				v7->setVariableName("c");
				v8->setVariableName("a");
				v9->setVariableName("t");
				v10->setVariableName("d");
				v11->setVariableName("o");
				v12->setVariableName("g");
				builder.addVariableNode(v1);
				builder.addVariableNode(v2);
				builder.addVariableNode(v3);
				builder.addVariableNode(v4);
				builder.addVariableNode(v5);
				builder.addVariableNode(v6);
				builder.addVariableNode(v7);
				builder.addVariableNode(v8);
				builder.addVariableNode(v9);
				builder.addVariableNode(v10);
				builder.addVariableNode(v11);
				builder.addVariableNode(v12);

				std::shared_ptr<ConstantNode> c1 = std::make_shared<ConstantNode>();
				std::shared_ptr<ConstantNode> c2 = std::make_shared<ConstantNode>();
				std::shared_ptr<ConstantNode> c3 = std::make_shared<ConstantNode>();
				std::shared_ptr<ConstantNode> c4 = std::make_shared<ConstantNode>();
				c1->setValue("0");
				c2->setValue("3");
				c3->setValue("1");
				c4->setValue("2");

				std::shared_ptr<ExpressionNode> e1 = std::make_shared<ExpressionNode>();
				std::shared_ptr<ExpressionNode> e2 = std::make_shared<ExpressionNode>();
				std::shared_ptr<ExpressionNode> e3 = std::make_shared<ExpressionNode>();
				std::shared_ptr<ExpressionNode> e4 = std::make_shared<ExpressionNode>();
				std::shared_ptr<ExpressionNode> e5 = std::make_shared<ExpressionNode>();
				std::shared_ptr<ExpressionNode> e6 = std::make_shared<ExpressionNode>();
				std::shared_ptr<ExpressionNode> e7 = std::make_shared<ExpressionNode>();
				std::shared_ptr<ExpressionNode> e8 = std::make_shared<ExpressionNode>();
				std::shared_ptr<ExpressionNode> e9 = std::make_shared<ExpressionNode>();
				std::shared_ptr<ExpressionNode> e10 = std::make_shared<ExpressionNode>();
				std::shared_ptr<ExpressionNode> e11 = std::make_shared<ExpressionNode>();
				std::shared_ptr<ExpressionNode> e12 = std::make_shared<ExpressionNode>();
				// meow
				e1->setExpressionType(ExpressionTypeEnum::none);
				e1->setLeftAstNode(v1);
				// 0
				e2->setExpressionType(ExpressionTypeEnum::none);
				e2->setLeftAstNode(c1);
				// a + b
				e3->setExpressionType(ExpressionTypeEnum::plus);
				e3->setLeftAstNode(v2);
				e3->setRightAstNode(v3);
				// c + d
				e4->setExpressionType(ExpressionTypeEnum::plus);
				e4->setLeftAstNode(v4);
				e4->setRightAstNode(v5);
				// a
				e5->setExpressionType(ExpressionTypeEnum::none);
				e5->setLeftAstNode(v6);
				// 3
				e6->setExpressionType(ExpressionTypeEnum::none);
				e6->setLeftAstNode(c2);
				// a / t
				e7->setExpressionType(ExpressionTypeEnum::div);
				e7->setLeftAstNode(v8);
				e7->setRightAstNode(v9);
				// c - e7
				e8->setExpressionType(ExpressionTypeEnum::min);
				e8->setLeftAstNode(v7);
				e8->setRightAstNode(e7);
				// 1
				e9->setExpressionType(ExpressionTypeEnum::none);
				e9->setLeftAstNode(c3);
				// o * g
				e10->setExpressionType(ExpressionTypeEnum::times);
				e10->setLeftAstNode(v11);
				e10->setRightAstNode(v12);
				// d + e10
				e11->setExpressionType(ExpressionTypeEnum::plus);
				e11->setLeftAstNode(v10);
				e11->setRightAstNode(e10);
				// 2
				e12->setExpressionType(ExpressionTypeEnum::none);
				e12->setLeftAstNode(c4);

				std::shared_ptr<RelationNode> r1 = std::make_shared<RelationNode>();
				std::shared_ptr<RelationNode> r2 = std::make_shared<RelationNode>();
				std::shared_ptr<RelationNode> r3 = std::make_shared<RelationNode>();
				std::shared_ptr<RelationNode> r4 = std::make_shared<RelationNode>();
				std::shared_ptr<RelationNode> r5 = std::make_shared<RelationNode>();
				// e1 > e2
				r1->setRelationType(RelationTypeEnum::gt);
				r1->setLeftAstNode(e1);
				r1->setRightAstNode(e2);
				// e3 == e4
				r2->setRelationType(RelationTypeEnum::eq);
				r2->setLeftAstNode(e3);
				r2->setRightAstNode(e4);
				// e5 <= e6
				r3->setRelationType(RelationTypeEnum::lte);
				r3->setLeftAstNode(e5);
				r3->setRightAstNode(e6);
				// e8 != e9
				r4->setRelationType(RelationTypeEnum::neq);
				r4->setLeftAstNode(e8);
				r4->setRightAstNode(e9);
				// e11 != e12
				r5->setRelationType(RelationTypeEnum::neq);
				r5->setLeftAstNode(e11);
				r5->setRightAstNode(e12);

				std::shared_ptr<ConditionNode> cd1 = std::make_shared<ConditionNode>();
				std::shared_ptr<ConditionNode> cd2 = std::make_shared<ConditionNode>();
				std::shared_ptr<ConditionNode> cd3 = std::make_shared<ConditionNode>();
				std::shared_ptr<ConditionNode> cd4 = std::make_shared<ConditionNode>();
				// r1
				cd1->setConditionType(ConditionTypeEnum::none);
				cd1->setLeftAstNode(r1);
				// !(r2)
				cd2->setConditionType(ConditionTypeEnum::not);
				cd2->setLeftAstNode(r2);
				// r3
				cd3->setConditionType(ConditionTypeEnum::none);
				cd3->setLeftAstNode(r3);
				// (r4) && (r5)
				cd4->setConditionType(ConditionTypeEnum::and);
				cd4->setLeftAstNode(r4);
				cd4->setRightAstNode(r5);

				std::shared_ptr<WhileNode> w1 = std::make_shared<WhileNode>();
				std::shared_ptr<WhileNode> w2 = std::make_shared<WhileNode>();
				w1->setConditionNode(cd1);
				w1->setStatementNumber(1);
				w2->setConditionNode(cd2);
				w2->setStatementNumber(2);
				builder.addWhileNode(w1);
				builder.addWhileNode(w2);

				std::shared_ptr<IfNode> i1 = std::make_shared<IfNode>();
				std::shared_ptr<IfNode> i2 = std::make_shared<IfNode>();
				i1->setConditionNode(cd3);
				i1->setStatementNumber(3);
				i2->setConditionNode(cd4);
				i2->setStatementNumber(4);
				builder.addIfNode(i1);
				builder.addIfNode(i2);

				pkb3 = std::make_shared<PKB>(builder.build());
			}

			{
				// PKB 4
				PKBBuilder builder;

				std::shared_ptr<VariableNode> v1 = std::make_shared<VariableNode>();
				std::shared_ptr<VariableNode> v2 = std::make_shared<VariableNode>();
				std::shared_ptr<VariableNode> v3 = std::make_shared<VariableNode>();
				v1->setVariableName("newspaper");
				v2->setVariableName("menu");
				v3->setVariableName("pain");
				builder.addVariableNode(v1);
				builder.addVariableNode(v2);
				builder.addVariableNode(v3);

				std::shared_ptr<ReadNode> r1 = std::make_shared<ReadNode>();
				std::shared_ptr<ReadNode> r2 = std::make_shared<ReadNode>();
				std::shared_ptr<ReadNode> r3 = std::make_shared<ReadNode>();
				r1->setStatementNumber(9);
				r1->setVariableNode(v1);
				r2->setStatementNumber(10);
				r2->setVariableNode(v2);
				r3->setStatementNumber(11);
				r3->setVariableNode(v2);
				builder.addReadNode(r1);
				builder.addReadNode(r2);
				builder.addReadNode(r3);

				// pain = 100
				std::shared_ptr<ConstantNode> const1 = std::make_shared<ConstantNode>();
				const1->setValue("100");
				builder.addConstantNode(const1);
				std::shared_ptr<ExpressionNode> e1 = std::make_shared<ExpressionNode>();
				e1->setExpressionType(ExpressionTypeEnum::none);
				e1->setLeftAstNode(const1);
				std::shared_ptr<AssignNode> a1 = std::make_shared<AssignNode>();
				a1->setVariableNode(v3);
				a1->setExpressionNode(e1);
				a1->setStatementNumber(12);


				std::shared_ptr<StatementNode> s1 = std::make_shared<StatementNode>();
				std::shared_ptr<StatementNode> s2 = std::make_shared<StatementNode>();
				std::shared_ptr<StatementNode> s3 = std::make_shared<StatementNode>();
				std::shared_ptr<StatementNode> s4 = std::make_shared<StatementNode>();
				std::shared_ptr<StatementNode> s5 = std::make_shared<StatementNode>();
				std::shared_ptr<StatementNode> s6 = std::make_shared<StatementNode>();
				std::shared_ptr<StatementNode> s7 = std::make_shared<StatementNode>();
				std::shared_ptr<StatementNode> s8 = std::make_shared<StatementNode>();
				std::shared_ptr<StatementNode> s9 = std::make_shared<StatementNode>();
				std::shared_ptr<StatementNode> s10 = std::make_shared<StatementNode>();
				std::shared_ptr<StatementNode> s11 = std::make_shared<StatementNode>();
				std::shared_ptr<StatementNode> s12 = std::make_shared<StatementNode>();

				s1->setStatementNumber(1);
				s2->setStatementNumber(2);
				s3->setStatementNumber(3);
				s4->setStatementNumber(4);
				s5->setStatementNumber(5);
				s6->setStatementNumber(6);
				s7->setStatementNumber(7);
				s8->setStatementNumber(8);
				s9->setStatementNumber(9);
				s10->setStatementNumber(10);
				s11->setStatementNumber(11);
				s12->setStatementNumber(12);

				builder.addStatementNode(s1);
				builder.addStatementNode(s2);
				builder.addStatementNode(s3);
				builder.addStatementNode(s4);
				builder.addStatementNode(s5);
				builder.addStatementNode(s6);
				builder.addStatementNode(s7);
				builder.addStatementNode(s8);
				builder.addStatementNode(s9);
				builder.addStatementNode(s10);
				builder.addStatementNode(s11);
				builder.addStatementNode(s12);

				// stmtList of main
				std::shared_ptr<StatementListNode> sl1 = std::make_shared<StatementListNode>();
				sl1->addStatementNode(s1);
				sl1->addStatementNode(s2);
				sl1->addStatementNode(s3);
				sl1->addStatementNode(s4);

				//stmtList of morning
				std::shared_ptr<StatementListNode> sl2 = std::make_shared<StatementListNode>();
				sl2->addStatementNode(s5);

				//stmtList of afternoon
				std::shared_ptr<StatementListNode> sl3 = std::make_shared<StatementListNode>();
				sl3->addStatementNode(s6);

				//stmtList of evening
				std::shared_ptr<StatementListNode> sl4 = std::make_shared<StatementListNode>();
				sl4->addStatementNode(s7);

				//stmtList of night
				std::shared_ptr<StatementListNode> sl5 = std::make_shared<StatementListNode>();
				sl5->addStatementNode(s8);

				//stmtList of breakfast
				std::shared_ptr<StatementListNode> sl6 = std::make_shared<StatementListNode>();
				sl6->addStatementNode(s9);

				//stmtList of lunch
				std::shared_ptr<StatementListNode> sl7 = std::make_shared<StatementListNode>();
				sl7->addStatementNode(s10);

				//stmtList of dinner
				std::shared_ptr<StatementListNode> sl8 = std::make_shared<StatementListNode>();
				sl8->addStatementNode(s11);

				//stmtList of diarrhoea
				std::shared_ptr<StatementListNode> sl9 = std::make_shared<StatementListNode>();
				sl9->addStatementNode(s12);
				
				builder.addStatementListNode(sl1);
				builder.addStatementListNode(sl2);
				builder.addStatementListNode(sl3);
				builder.addStatementListNode(sl4);
				builder.addStatementListNode(sl5);
				builder.addStatementListNode(sl6);
				builder.addStatementListNode(sl7);
				builder.addStatementListNode(sl8);
				builder.addStatementListNode(sl9);


				std::shared_ptr<ProcedureNode> proc1 = std::make_shared<ProcedureNode>();
				proc1->setProcedureName("main");
				std::shared_ptr<ProcedureNode> proc2 = std::make_shared<ProcedureNode>();
				proc2->setProcedureName("morning");
				std::shared_ptr<ProcedureNode> proc3 = std::make_shared<ProcedureNode>();
				proc3->setProcedureName("afternoon");
				std::shared_ptr<ProcedureNode> proc4 = std::make_shared<ProcedureNode>();
				proc4->setProcedureName("evening");
				std::shared_ptr<ProcedureNode> proc5 = std::make_shared<ProcedureNode>();
				proc5->setProcedureName("night");
				std::shared_ptr<ProcedureNode> proc6 = std::make_shared<ProcedureNode>();
				proc6->setProcedureName("breakfast");
				std::shared_ptr<ProcedureNode> proc7 = std::make_shared<ProcedureNode>();
				proc7->setProcedureName("lunch");
				std::shared_ptr<ProcedureNode> proc8 = std::make_shared<ProcedureNode>();
				proc8->setProcedureName("dinner");
				std::shared_ptr<ProcedureNode> proc9 = std::make_shared<ProcedureNode>();
				proc9->setProcedureName("diarrhoea");

				builder.addProcedureNode(proc1);
				builder.addProcedureNode(proc2);
				builder.addProcedureNode(proc3);
				builder.addProcedureNode(proc4);
				builder.addProcedureNode(proc5);
				builder.addProcedureNode(proc6);
				builder.addProcedureNode(proc7);
				builder.addProcedureNode(proc8);
				builder.addProcedureNode(proc9);

				// calls morning
				std::shared_ptr<CallNode> c1 = std::make_shared<CallNode>();
				c1->setCallerProcedureName("main");
				c1->setCalleeProcedureName("morning");
				c1->setProcedureNode(proc2);
				c1->setStatementListNode(sl2);
				c1->setStatementNumber(1);

				// calls afternoon
				std::shared_ptr<CallNode> c2 = std::make_shared<CallNode>();
				c2->setCallerProcedureName("main");
				c2->setCalleeProcedureName("afternoon");
				c2->setProcedureNode(proc3);
				c2->setStatementListNode(sl3);
				c2->setStatementNumber(2);

				// calls evening
				std::shared_ptr<CallNode> c3 = std::make_shared<CallNode>();
				c3->setCallerProcedureName("main");
				c3->setCalleeProcedureName("evening");
				c3->setProcedureNode(proc4);
				c3->setStatementListNode(sl4);
				c3->setStatementNumber(3);

				// calls night
				std::shared_ptr<CallNode> c4 = std::make_shared<CallNode>();
				c4->setCallerProcedureName("main");
				c4->setCalleeProcedureName("night");
				c4->setProcedureNode(proc5);
				c4->setStatementListNode(sl5);
				c4->setStatementNumber(4);

				// calls breakfast
				std::shared_ptr<CallNode> c5 = std::make_shared<CallNode>();
				c5->setCallerProcedureName("morning");
				c5->setCalleeProcedureName("breakfast");
				c5->setProcedureNode(proc6);
				c5->setStatementListNode(sl6);
				c5->setStatementNumber(5);

				// calls lunch
				std::shared_ptr<CallNode> c6 = std::make_shared<CallNode>();
				c6->setCallerProcedureName("afternoon");
				c6->setCalleeProcedureName("lunch");
				c6->setProcedureNode(proc7);
				c6->setStatementListNode(sl7);
				c6->setStatementNumber(6);

				// calls dinner
				std::shared_ptr<CallNode> c7 = std::make_shared<CallNode>();
				c7->setCallerProcedureName("evening");
				c7->setCalleeProcedureName("dinner");
				c7->setProcedureNode(proc8);
				c7->setStatementListNode(sl8);
				c7->setStatementNumber(7);

				// calls diarrhoea
				std::shared_ptr<CallNode> c8 = std::make_shared<CallNode>();
				c8->setCallerProcedureName("night");
				c8->setCalleeProcedureName("diarrhoea");
				c8->setProcedureNode(proc9);
				c8->setStatementListNode(sl9);
				c8->setStatementNumber(8);

				builder.addCallNode(c1);
				builder.addCallNode(c2);
				builder.addCallNode(c3);
				builder.addCallNode(c4);
				builder.addCallNode(c5);
				builder.addCallNode(c6);
				builder.addCallNode(c7);
				builder.addCallNode(c8);

				builder.addFollows(1, 2);
				builder.addFollows(2, 3);
				builder.addFollows(3, 4);

				builder.addModifies("breakfast", "newspaper");
				builder.addModifies("morning", "newspaper");
				builder.addModifies("main", "newspaper");
				builder.addModifies("lunch", "menu");
				builder.addModifies("afternoon", "menu");
				builder.addModifies("main", "menu");
				builder.addModifies("dinner", "menu");
				builder.addModifies("evening", "menu");
				builder.addModifies("main", "menu");
				builder.addModifies("diarrhoea", "pain");
				builder.addModifies("night", "pain");
				builder.addModifies("main", "pain");

				builder.addModifies(9, "newspaper");
				builder.addModifies(10, "menu");
				builder.addModifies(11, "menu");
				builder.addModifies(12, "pain");
				
				builder.addCalls("main", "morning");
				builder.addCalls("main", "afternoon");
				builder.addCalls("main", "evening");
				builder.addCalls("main", "night");
				builder.addCalls("morning", "breakfast");
				builder.addCalls("afternoon", "lunch");
				builder.addCalls("evening", "dinner");
				builder.addCalls("night", "diarrhoea");

				builder.addNext(1, 2);
				builder.addNext(2, 3);
				builder.addNext(3, 4);


				pkb4 = std::make_shared<PKB>(builder.build());
			}

		}

		TEST_METHOD(evaluateQuery_Select_Success)
		{
			// Query: "variable v; Select v"
			// Get processed_synonyms and processed clauses
			QueryNode variable_node = QueryNode();
			variable_node.setSynonymNode({ QuerySynonymType::variable }, "v");
			std::unordered_map<std::string, QueryNode> processed_synonyms = { {"v", variable_node} };

			QueryNode child1 = QueryNode();
			child1.setNodeType({ QueryNodeType::select });
			QueryNode tuple = QueryNode();
			tuple.setNodeType({ QueryNodeType::tuple });
			QueryNode child2 = QueryNode();
			child2.setSynonymNode({ QuerySynonymType::variable }, "v");
			QueryNode tuple_children[] = { child2 };
			tuple.setChildren(tuple_children, 1);
			QueryNode child1_children[] = { tuple };
			child1.setChildren(child1_children, 1);

			QueryNode processed_clauses = child1; //stores root node of the tree

			QueryEvaluator qe = QueryEvaluator(*pkb);
			QUERY_RESULT result = qe.evaluateQuery(processed_synonyms, processed_clauses);
			STRING_RESULT result_string = ResultListManager::getStringValues(result);
			STRING_RESULT correct_result = "a, b, c, d, e";

			Logger::WriteMessage(result_string.c_str());
			Assert::IsTrue(result_string.compare(correct_result) == 0);
		}

		TEST_METHOD(evaluateQuery_SelectPn_Success) {
			// Query: "print pn; Select pn"
			// Get processed_synonyms and processed clauses
			QueryNode variable_node = QueryNode();
			variable_node.setSynonymNode({ QuerySynonymType::print }, "pn");
			std::unordered_map<std::string, QueryNode> processed_synonyms = { {"pn", variable_node} };

			QueryNode child1 = QueryNode();
			child1.setNodeType({ QueryNodeType::select });
			QueryNode tuple = QueryNode();
			tuple.setNodeType({ QueryNodeType::tuple });
			QueryNode child2 = QueryNode();
			child2.setSynonymNode({ QuerySynonymType::print }, "pn");
			QueryNode tuple_children[] = { child2 };
			tuple.setChildren(tuple_children, 1);
			QueryNode child1_children[] = { tuple };
			child1.setChildren(child1_children, 1);

			QueryNode processed_clauses = child1; //stores root node of the tree

			QueryEvaluator qe = QueryEvaluator(*pkb);
			QUERY_RESULT result = qe.evaluateQuery(processed_synonyms, processed_clauses);
			STRING_RESULT result_string = ResultListManager::getStringValues(result);
			STRING_RESULT correct_result = "1, 2, 3, 4, 5";

			Logger::WriteMessage(result_string.c_str());
			Assert::IsTrue(result_string.compare(correct_result) == 0);
		}

		TEST_METHOD(evaluateQuery_SelectPn_varname_Success) {
			// Query: "print pn; Select pn.varName"
			// Get processed_synonyms and processed clauses
			QueryNode variable_node = QueryNode();
			variable_node.setSynonymNode({ QuerySynonymType::print }, "pn");
			std::unordered_map<std::string, QueryNode> processed_synonyms = { {"pn", variable_node} };

			QueryNode child1 = QueryNode();
			child1.setNodeType({ QueryNodeType::select });
			QueryNode tuple = QueryNode();
			tuple.setNodeType({ QueryNodeType::tuple });
			QueryNode child2 = QueryNode();
			child2.setAttrNode("pn", "varName");
			QueryNode tuple_children[] = { child2 };
			tuple.setChildren(tuple_children, 1);
			QueryNode child1_children[] = { tuple };
			child1.setChildren(child1_children, 1);

			QueryNode processed_clauses = child1; //stores root node of the tree

			QueryEvaluator qe = QueryEvaluator(*pkb);
			QUERY_RESULT result = qe.evaluateQuery(processed_synonyms, processed_clauses);
			STRING_RESULT result_string = ResultListManager::getStringValues(result);
			STRING_RESULT correct_result = "a, b, c, d, e";

			Logger::WriteMessage(result_string.c_str());
			Assert::IsTrue(result_string.compare(correct_result) == 0);
		}

		TEST_METHOD(evaluateQuery_SelectBOOLEAN_PKB1_ReturnsTrue)
		{
			// Query: "Select BOOLEAN"
			std::unordered_map<std::string, QueryNode> processed_synonyms;

			QueryNode root = QueryNode();
			root.setNodeType({ QueryNodeType::select });
			QueryNode child1 = QueryNode();
			child1.setBooleanNode();
			QueryNode root_children[] = { child1 };
			root.setChildren(root_children, 1);

			QueryNode processed_clauses = root; //stores root node of the tree

			// Evaluate
			QueryEvaluator qe = QueryEvaluator(*pkb);
			QUERY_RESULT result = qe.evaluateQuery(processed_synonyms, processed_clauses);
			STRING_RESULT result_string = ResultListManager::getStringValues(result);
			STRING_RESULT correct_result = "TRUE";

			Logger::WriteMessage("Result: ");
			Logger::WriteMessage(result_string.c_str());
			Assert::IsTrue(result_string.compare(correct_result) == 0);
		}

		/*=================================  PKB 1: Follows =========================================*/

		TEST_METHOD(evaluateQuery_SelectS1FollowsS1S2_ReturnsS1)
		{
			// Query: "stmt s1, s2; variable v; Select s1 such that Follows(s1,s2)"
			// Get processed_synonyms and processed clauses
			QueryNode variable_node = QueryNode();
			variable_node.setSynonymNode({ QuerySynonymType::variable }, "v");
			QueryNode s1_node = QueryNode();
			s1_node.setSynonymNode({ QuerySynonymType::stmt }, "s1");
			QueryNode s2_node = QueryNode();
			s2_node.setSynonymNode({ QuerySynonymType::stmt }, "s2");
			std::unordered_map<std::string, QueryNode> processed_synonyms = { {"v", variable_node}, {"s1", s1_node}, {"s2", s2_node} };

			QueryNode child1 = QueryNode();
			child1.setSynonymNode({ QuerySynonymType::stmt }, "s1");
			QueryNode child2 = QueryNode();
			child2.setNodeType({ QueryNodeType::such_that });
			QueryNode child_child1 = QueryNode();
			child_child1.setNodeType({ QueryNodeType::follows });
			QueryNode child_child_child1 = QueryNode();
			child_child_child1.setSynonymNode({ QuerySynonymType::stmt }, "s1");
			QueryNode child_child_child2 = QueryNode();
			child_child_child2.setSynonymNode({ QuerySynonymType::stmt }, "s2");
			QueryNode child_child1_children[] = { child_child_child1, child_child_child2 };
			child_child1.setChildren(child_child1_children, 2);
			QueryNode child2_children[] = { child_child1 };
			child2.setChildren(child2_children, 1);

			QueryNode root = QueryNode();
			root.setNodeType({ QueryNodeType::select });
			QueryNode tuple = QueryNode();
			tuple.setNodeType({ QueryNodeType::tuple });
			QueryNode tuple_children[] = { child1 };
			tuple.setChildren(tuple_children, 1);
			QueryNode root_children[] = { tuple, child2 };
			root.setChildren(root_children, 2);

			QueryNode processed_clauses = root; //stores root node of the tree

			// Evaluate
			QueryEvaluator qe = QueryEvaluator(*pkb);
			QUERY_RESULT result = qe.evaluateQuery(processed_synonyms, processed_clauses);
			STRING_RESULT result_string = ResultListManager::getStringValues(result);
			STRING_RESULT correct_result = "1, 2, 3, 4";

			Logger::WriteMessage(result_string.c_str());
			Assert::IsTrue(result_string.compare(correct_result) == 0);
		}

		TEST_METHOD(evaluateQuery_SelectBOOLEANFollowsS1S2_ReturnsTrue)
		{
			// Query: "stmt s1, s2; Select BOOLEAN such that Follows(s1, s2)"
			// Get processed_synonyms and processed clauses
			QueryNode s1_node = QueryNode();
			s1_node.setSynonymNode({ QuerySynonymType::stmt }, "s1");
			QueryNode s2_node = QueryNode();
			s2_node.setSynonymNode({ QuerySynonymType::stmt }, "s2");
			std::unordered_map<std::string, QueryNode> processed_synonyms = { {"s1", s1_node}, {"s2", s2_node} };

			// Select: BOOLEAN
			QueryNode child1 = QueryNode();
			child1.setBooleanNode();

			// such that
			QueryNode child2 = QueryNode();
			child2.setNodeType({ QueryNodeType::such_that });

			// follows
			QueryNode child_child1 = QueryNode();
			child_child1.setNodeType({ QueryNodeType::follows });

			// arg1: s1
			QueryNode child_child_child1 = QueryNode();
			child_child_child1.setSynonymNode({ QuerySynonymType::stmt }, "s1");

			// arg2: s2
			QueryNode child_child_child2 = QueryNode();
			child_child_child2.setSynonymNode({ QuerySynonymType::stmt }, "s2");

			// set children
			QueryNode child_child1_children[] = { child_child_child1, child_child_child2 };
			child_child1.setChildren(child_child1_children, 2);
			QueryNode child2_children[] = { child_child1 };
			child2.setChildren(child2_children, 1);

			QueryNode root = QueryNode();
			root.setNodeType({ QueryNodeType::select });
			QueryNode root_children[] = { child1, child2 };
			root.setChildren(root_children, 2);

			QueryNode processed_clauses = root; //stores root node of the tree

			// Evaluate
			QueryEvaluator qe = QueryEvaluator(*pkb);
			QUERY_RESULT result = qe.evaluateQuery(processed_synonyms, processed_clauses);
			STRING_RESULT result_string = ResultListManager::getStringValues(result);
			STRING_RESULT correct_result = "TRUE";

			Logger::WriteMessage("Result: ");
			Logger::WriteMessage(result_string.c_str());
			Assert::IsTrue(result_string.compare(correct_result) == 0);
		}

		TEST_METHOD(evaluateQuery_SelectAFollows1A_ReturnsEmpty)
		{
			// Query: "assign a; Select a such that Follows(1,a)"
			// Get processed_synonyms and processed clauses
			QueryNode assign_node = QueryNode();
			assign_node.setSynonymNode({ QuerySynonymType::assign }, "a");
			std::unordered_map<std::string, QueryNode> processed_synonyms = { {"a", assign_node} };

			QueryNode child1 = QueryNode();
			child1.setSynonymNode({ QuerySynonymType::assign }, "a");
			QueryNode child2 = QueryNode();
			child2.setNodeType({ QueryNodeType::such_that });
			QueryNode child_child1 = QueryNode();
			child_child1.setNodeType({ QueryNodeType::follows });
			QueryNode child_child_child1 = QueryNode();
			child_child_child1.setIntegerNode(1);
			QueryNode child_child_child2 = QueryNode();
			child_child_child2.setSynonymNode({ QuerySynonymType::assign }, "a");
			QueryNode child_child1_children[] = { child_child_child1, child_child_child2 };
			child_child1.setChildren(child_child1_children, 2);
			QueryNode child2_children[] = { child_child1 };
			child2.setChildren(child2_children, 1);

			QueryNode root = QueryNode();
			root.setNodeType({ QueryNodeType::select });
			QueryNode tuple = QueryNode();
			tuple.setNodeType({ QueryNodeType::tuple });
			QueryNode tuple_children[] = { child1 };
			tuple.setChildren(tuple_children, 1);
			QueryNode root_children[] = { tuple, child2 };
			root.setChildren(root_children, 2);

			QueryNode processed_clauses = root; //stores root node of the tree

			// Evaluate
			QueryEvaluator qe = QueryEvaluator(*pkb);
			QUERY_RESULT result = qe.evaluateQuery(processed_synonyms, processed_clauses);
			STRING_RESULT result_string = ResultListManager::getStringValues(result);
			STRING_RESULT correct_result = "";

			Logger::WriteMessage(result_string.c_str());
			Assert::IsTrue(result_string.compare(correct_result) == 0);
		}

		TEST_METHOD(evaluateQuery_SelectBOOLEANFollows1A_ReturnsFalse)
		{
			// Query: "assign a; Select BOOLEAN such that Follows(1,a)"
			// Get processed_synonyms and processed clauses
			QueryNode assign_node = QueryNode();
			assign_node.setSynonymNode({ QuerySynonymType::assign }, "a");
			std::unordered_map<std::string, QueryNode> processed_synonyms = { {"a", assign_node} };

			// Select: BOOLEAN
			QueryNode child1 = QueryNode();
			child1.setBooleanNode();

			// such that
			QueryNode child2 = QueryNode();
			child2.setNodeType({ QueryNodeType::such_that });

			// follows
			QueryNode child_child1 = QueryNode();
			child_child1.setNodeType({ QueryNodeType::follows });

			// arg1: 1
			QueryNode child_child_child1 = QueryNode();
			child_child_child1.setIntegerNode(1);

			// arg2: a
			QueryNode child_child_child2 = QueryNode();
			child_child_child2.setSynonymNode({ QuerySynonymType::assign }, "a");

			// set children
			QueryNode child_child1_children[] = { child_child_child1, child_child_child2 };
			child_child1.setChildren(child_child1_children, 2);
			QueryNode child2_children[] = { child_child1 };
			child2.setChildren(child2_children, 1);

			QueryNode root = QueryNode();
			root.setNodeType({ QueryNodeType::select });
			QueryNode root_children[] = { child1, child2 };
			root.setChildren(root_children, 2);

			QueryNode processed_clauses = root; //stores root node of the tree

			// Evaluate
			QueryEvaluator qe = QueryEvaluator(*pkb);
			QUERY_RESULT result = qe.evaluateQuery(processed_synonyms, processed_clauses);
			STRING_RESULT result_string = ResultListManager::getStringValues(result);
			STRING_RESULT correct_result = "FALSE";

			Logger::WriteMessage("Result: ");
			Logger::WriteMessage(result_string.c_str());
			Assert::IsTrue(result_string.compare(correct_result) == 0);
		}

		TEST_METHOD(evaluateQuery_SelectPnFollows2_3_ReturnsPn)
		{
			// Query: "print pn; Select pn such that Follows(2,3)"
			// Get processed_synonyms and processed clauses
			QueryNode print_node = QueryNode();
			print_node.setSynonymNode({ QuerySynonymType::print }, "pn");
			std::unordered_map<std::string, QueryNode> processed_synonyms = { {"pn", print_node} };

			QueryNode child1 = QueryNode();
			child1.setSynonymNode({ QuerySynonymType::print }, "pn");
			QueryNode child2 = QueryNode();
			child2.setNodeType({ QueryNodeType::such_that });
			QueryNode child_child1 = QueryNode();
			child_child1.setNodeType({ QueryNodeType::follows });
			QueryNode child_child_child1 = QueryNode();
			child_child_child1.setIntegerNode(2);
			QueryNode child_child_child2 = QueryNode();
			child_child_child2.setIntegerNode(3);
			QueryNode child_child1_children[] = { child_child_child1, child_child_child2 };
			child_child1.setChildren(child_child1_children, 2);
			QueryNode child2_children[] = { child_child1 };
			child2.setChildren(child2_children, 1);

			QueryNode root = QueryNode();
			root.setNodeType({ QueryNodeType::select });
			QueryNode tuple = QueryNode();
			tuple.setNodeType({ QueryNodeType::tuple });
			QueryNode tuple_children[] = { child1 };
			tuple.setChildren(tuple_children, 1);
			QueryNode root_children[] = { tuple, child2 };
			root.setChildren(root_children, 2);

			QueryNode processed_clauses = root; //stores root node of the tree

			// Evaluate
			QueryEvaluator qe = QueryEvaluator(*pkb);
			QUERY_RESULT result = qe.evaluateQuery(processed_synonyms, processed_clauses);
			STRING_RESULT result_string = ResultListManager::getStringValues(result);
			STRING_RESULT correct_result = "1, 2, 3, 4, 5";

			Logger::WriteMessage(result_string.c_str());
			Assert::IsTrue(result_string.compare(correct_result) == 0);
		}

		TEST_METHOD(evaluateQuery_SelectPnFollows2_4_ReturnsEmpty)
		{
			// Query: "print pn; Select pn such that Follows(2,4)"
			// Get processed_synonyms and processed clauses
			QueryNode print_node = QueryNode();
			print_node.setSynonymNode({ QuerySynonymType::print }, "pn");
			std::unordered_map<std::string, QueryNode> processed_synonyms = { {"pn", print_node} };

			QueryNode child1 = QueryNode();
			child1.setSynonymNode({ QuerySynonymType::print }, "pn");
			QueryNode child2 = QueryNode();
			child2.setNodeType({ QueryNodeType::such_that });
			QueryNode child_child1 = QueryNode();
			child_child1.setNodeType({ QueryNodeType::follows });
			QueryNode child_child_child1 = QueryNode();
			child_child_child1.setIntegerNode(2);
			QueryNode child_child_child2 = QueryNode();
			child_child_child2.setIntegerNode(4);
			QueryNode child_child1_children[] = { child_child_child1, child_child_child2 };
			child_child1.setChildren(child_child1_children, 2);
			QueryNode child2_children[] = { child_child1 };
			child2.setChildren(child2_children, 1);

			QueryNode root = QueryNode();
			root.setNodeType({ QueryNodeType::select });
			QueryNode tuple = QueryNode();
			tuple.setNodeType({ QueryNodeType::tuple });
			QueryNode tuple_children[] = { child1 };
			tuple.setChildren(tuple_children, 1);
			QueryNode root_children[] = { tuple, child2 };
			root.setChildren(root_children, 2);

			QueryNode processed_clauses = root; //stores root node of the tree

			// Evaluate
			QueryEvaluator qe = QueryEvaluator(*pkb);
			QUERY_RESULT result = qe.evaluateQuery(processed_synonyms, processed_clauses);
			STRING_RESULT result_string = ResultListManager::getStringValues(result);
			STRING_RESULT correct_result = "";

			Logger::WriteMessage(result_string.c_str());
			Assert::IsTrue(result_string.compare(correct_result) == 0);
		}

		TEST_METHOD(evaluateQuery_SelectSFollowsS_S_ReturnsEmpty)
		{
			// Query: "stmt s; Select s such that Follows(s,s)"
			// Get processed_synonyms and processed clauses
			QueryNode stmt_node = QueryNode();
			stmt_node.setSynonymNode({ QuerySynonymType::stmt }, "s");
			std::unordered_map<std::string, QueryNode> processed_synonyms = { {"s", stmt_node} };

			// Select: s
			QueryNode child1 = QueryNode();
			child1.setSynonymNode({ QuerySynonymType::stmt }, "s");

			// such that 
			QueryNode child2 = QueryNode();
			child2.setNodeType({ QueryNodeType::such_that });

			// follows
			QueryNode child_child1 = QueryNode();
			child_child1.setNodeType({ QueryNodeType::follows });

			// arg 1: s
			QueryNode child_child_child1 = QueryNode();
			child_child_child1.setSynonymNode({ QuerySynonymType::stmt }, "s");

			// arg 2: s
			QueryNode child_child_child2 = QueryNode();
			child_child_child2.setSynonymNode({ QuerySynonymType::stmt }, "s");
			QueryNode child_child1_children[] = { child_child_child1, child_child_child2 };

			// set children, make tree
			child_child1.setChildren(child_child1_children, 2);
			QueryNode child2_children[] = { child_child1 };
			child2.setChildren(child2_children, 1);

			QueryNode root = QueryNode();
			root.setNodeType({ QueryNodeType::select });
			QueryNode tuple = QueryNode();
			tuple.setNodeType({ QueryNodeType::tuple });
			QueryNode tuple_children[] = { child1 };
			tuple.setChildren(tuple_children, 1);
			QueryNode root_children[] = { tuple, child2 };
			root.setChildren(root_children, 2);

			QueryNode processed_clauses = root; //stores root node of the tree

			// Evaluate
			QueryEvaluator qe = QueryEvaluator(*pkb);
			QUERY_RESULT result;
			STRING_RESULT result_string;

			try {
				result = qe.evaluateQuery(processed_synonyms, processed_clauses);
				result_string = ResultListManager::getStringValues(result);
			}
			catch (const char* msg) {
				Logger::WriteMessage("Evaluate exception caught");
			}
			STRING_RESULT correct_result = "";

			Logger::WriteMessage(result_string.c_str());
			Assert::IsTrue(result_string.compare(correct_result) == 0);
		}


		/*=================================  PKB 1: FollowsT =========================================*/

		TEST_METHOD(evaluateQuery_SelectSFollowsTS_S_ReturnsEmpty)
		{
			// Query: "stmt s; Select s such that FollowsT(s,s)"
			// Get processed_synonyms and processed clauses
			QueryNode stmt_node = QueryNode();
			stmt_node.setSynonymNode({ QuerySynonymType::stmt }, "s");
			std::unordered_map<std::string, QueryNode> processed_synonyms = { {"s", stmt_node} };

			// Select: s
			QueryNode child1 = QueryNode();
			child1.setSynonymNode({ QuerySynonymType::stmt }, "s");

			// such that 
			QueryNode child2 = QueryNode();
			child2.setNodeType({ QueryNodeType::such_that });

			// followsT
			QueryNode child_child1 = QueryNode();
			child_child1.setNodeType({ QueryNodeType::followsT });

			// arg 1: s
			QueryNode child_child_child1 = QueryNode();
			child_child_child1.setSynonymNode({ QuerySynonymType::stmt }, "s");

			// arg 2: s
			QueryNode child_child_child2 = QueryNode();
			child_child_child2.setSynonymNode({ QuerySynonymType::stmt }, "s");
			QueryNode child_child1_children[] = { child_child_child1, child_child_child2 };

			// set children, make tree
			child_child1.setChildren(child_child1_children, 2);
			QueryNode child2_children[] = { child_child1 };
			child2.setChildren(child2_children, 1);

			QueryNode root = QueryNode();
			root.setNodeType({ QueryNodeType::select });
			QueryNode tuple = QueryNode();
			tuple.setNodeType({ QueryNodeType::tuple });
			QueryNode tuple_children[] = { child1 };
			tuple.setChildren(tuple_children, 1);
			QueryNode root_children[] = { tuple, child2 };
			root.setChildren(root_children, 2);

			QueryNode processed_clauses = root; //stores root node of the tree

			// Evaluate
			QueryEvaluator qe = QueryEvaluator(*pkb);
			QUERY_RESULT result;
			STRING_RESULT result_string;

			try {
				result = qe.evaluateQuery(processed_synonyms, processed_clauses);
				result_string = ResultListManager::getStringValues(result);
			}
			catch (const char* msg) {
				Logger::WriteMessage("Evaluate exception caught");
			}
			STRING_RESULT correct_result = "";

			Logger::WriteMessage(result_string.c_str());
			Assert::IsTrue(result_string.compare(correct_result) == 0);
		}

		TEST_METHOD(evaluateQuery_SelectVFollowsT1_S_ReturnsV)
		{
			// Query: "variable v; stmt s; Select v such that FollowsT(1,s)"
			// Get processed_synonyms and processed clauses
			QueryNode variable_node = QueryNode();
			variable_node.setSynonymNode({ QuerySynonymType::variable }, "v");
			QueryNode stmt_node = QueryNode();
			stmt_node.setSynonymNode({ QuerySynonymType::stmt }, "s");
			std::unordered_map<std::string, QueryNode> processed_synonyms = { {"v", variable_node}, {"s", stmt_node} };

			QueryNode child1 = QueryNode();
			child1.setSynonymNode({ QuerySynonymType::variable }, "v");
			QueryNode child2 = QueryNode();
			child2.setNodeType({ QueryNodeType::such_that });
			QueryNode child_child1 = QueryNode();
			child_child1.setNodeType({ QueryNodeType::followsT });
			QueryNode child_child_child1 = QueryNode();
			child_child_child1.setIntegerNode(1);
			QueryNode child_child_child2 = QueryNode();
			child_child_child2.setSynonymNode({ QuerySynonymType::stmt }, "s");
			QueryNode child_child1_children[] = { child_child_child1, child_child_child2 };
			child_child1.setChildren(child_child1_children, 2);
			QueryNode child2_children[] = { child_child1 };
			child2.setChildren(child2_children, 1);

			QueryNode root = QueryNode();
			root.setNodeType({ QueryNodeType::select });
			QueryNode tuple = QueryNode();
			tuple.setNodeType({ QueryNodeType::tuple });
			QueryNode tuple_children[] = { child1 };
			tuple.setChildren(tuple_children, 1);
			QueryNode root_children[] = { tuple, child2 };
			root.setChildren(root_children, 2);

			QueryNode processed_clauses = root; //stores root node of the tree

			// Evaluate
			QueryEvaluator qe = QueryEvaluator(*pkb);
			QUERY_RESULT result;
			STRING_RESULT result_string;

			try {
				result = qe.evaluateQuery(processed_synonyms, processed_clauses);
				result_string = ResultListManager::getStringValues(result);
			}
			catch (const char* msg) {
				Logger::WriteMessage("Evaluate exception caught");
			}
			STRING_RESULT correct_result = "a, b, c, d, e";

			Logger::WriteMessage(result_string.c_str());
			Assert::IsTrue(result_string.compare(correct_result) == 0);
		}

		TEST_METHOD(evaluateQuery_SelectVPn_varname_FollowsT1_S_ReturnsVPn)
		{
			// Query: "variable v; stmt s; print pn; Select <v.varName, pn.varName> such that FollowsT(1,s)"
			// Get processed_synonyms and processed clauses
			QueryNode variable_node = QueryNode();
			variable_node.setSynonymNode({ QuerySynonymType::variable }, "v");
			QueryNode stmt_node = QueryNode();
			stmt_node.setSynonymNode({ QuerySynonymType::stmt }, "s");
			QueryNode print_node = QueryNode();
			print_node.setSynonymNode({ QuerySynonymType::print }, "pn");
			std::unordered_map<std::string, QueryNode> processed_synonyms = { {"v", variable_node}, {"s", stmt_node}, {"pn", print_node} };

			QueryNode child1 = QueryNode();
			child1.setAttrNode("v", "varName");
			QueryNode child1_2 = QueryNode();
			child1_2.setAttrNode("pn", "varName");

			QueryNode child2 = QueryNode();
			child2.setNodeType({ QueryNodeType::such_that });
			QueryNode child_child1 = QueryNode();
			child_child1.setNodeType({ QueryNodeType::followsT });
			QueryNode child_child_child1 = QueryNode();
			child_child_child1.setIntegerNode(1);
			QueryNode child_child_child2 = QueryNode();
			child_child_child2.setSynonymNode({ QuerySynonymType::stmt }, "s");
			QueryNode child_child1_children[] = { child_child_child1, child_child_child2 };
			child_child1.setChildren(child_child1_children, 2);
			QueryNode child2_children[] = { child_child1 };
			child2.setChildren(child2_children, 1);

			QueryNode root = QueryNode();
			root.setNodeType({ QueryNodeType::select });
			QueryNode tuple = QueryNode();
			tuple.setNodeType({ QueryNodeType::tuple });
			QueryNode tuple_children[] = { child1, child1_2};
			tuple.setChildren(tuple_children, 2);
			QueryNode root_children[] = { tuple, child2 };
			root.setChildren(root_children, 2);

			QueryNode processed_clauses = root; //stores root node of the tree

			// Evaluate
			QueryEvaluator qe = QueryEvaluator(*pkb);
			QUERY_RESULT result;
			STRING_RESULT result_string;

			try {
				result = qe.evaluateQuery(processed_synonyms, processed_clauses);
				result_string = ResultListManager::getStringValues(result);
			}
			catch (const char* msg) {
				Logger::WriteMessage("Evaluate exception caught");
			}
			STRING_RESULT correct_result = "a a, a b, a c, a d, a e, b a, b b, b c, b d, b e, c a, c b, c c, c d, "
				"c e, d a, d b, d c, d d, d e, e a, e b, e c, e d, e e, a a, a b, a c, a d, a e, b a, b b, b c, "
				"b d, b e, c a, c b, c c, c d, c e, d a, d b, d c, d d, d e, e a, e b, e c, e d, e e, a a, a b, a c, "
				"a d, a e, b a, b b, b c, b d, b e, c a, c b, c c, c d, c e, d a, d b, d c, d d, d e, e a, e b, e c, "
				"e d, e e, a a, a b, a c, a d, a e, b a, b b, b c, b d, b e, c a, c b, c c, c d, c e, d a, d b, d c, "
				"d d, d e, e a, e b, e c, e d, e e";

			Logger::WriteMessage(result_string.c_str());
			Assert::IsTrue(result_string.compare(correct_result) == 0);
		}

		TEST_METHOD(evaluateQuery_SelectPnFollowsT1_Pn_ReturnsPn)
		{
			// Query: "print pn; Select pn such that FollowsT(1,pn)"
			// Get processed_synonyms and processed clauses
			QueryNode variable_node = QueryNode();
			variable_node.setSynonymNode({ QuerySynonymType::print }, "pn");
			std::unordered_map<std::string, QueryNode> processed_synonyms = { {"pn", variable_node} };

			QueryNode child1 = QueryNode();
			child1.setSynonymNode({ QuerySynonymType::print }, "pn");
			QueryNode child2 = QueryNode();
			child2.setNodeType({ QueryNodeType::such_that });
			QueryNode child_child1 = QueryNode();
			child_child1.setNodeType({ QueryNodeType::followsT });
			QueryNode child_child_child1 = QueryNode();
			child_child_child1.setIntegerNode(1);
			QueryNode child_child_child2 = QueryNode();
			child_child_child2.setSynonymNode({ QuerySynonymType::print }, "pn");
			QueryNode child_child1_children[] = { child_child_child1, child_child_child2 };
			child_child1.setChildren(child_child1_children, 2);
			QueryNode child2_children[] = { child_child1 };
			child2.setChildren(child2_children, 1);

			QueryNode root = QueryNode();
			root.setNodeType({ QueryNodeType::select });
			QueryNode tuple = QueryNode();
			tuple.setNodeType({ QueryNodeType::tuple });
			QueryNode tuple_children[] = { child1 };
			tuple.setChildren(tuple_children, 1);
			QueryNode root_children[] = { tuple, child2 };
			root.setChildren(root_children, 2);

			QueryNode processed_clauses = root; //stores root node of the tree

			// Evaluate
			QueryEvaluator qe = QueryEvaluator(*pkb);
			QUERY_RESULT result;
			STRING_RESULT result_string;

			try {
				result = qe.evaluateQuery(processed_synonyms, processed_clauses);
				result_string = ResultListManager::getStringValues(result);
			}
			catch (const char* msg) {
				Logger::WriteMessage("Evaluate exception caught");
			}
			STRING_RESULT correct_result = "2, 3, 4, 5";

			Logger::WriteMessage(result_string.c_str());
			Assert::IsTrue(result_string.compare(correct_result) == 0);
		}

		TEST_METHOD(evaluateQuery_SelectPn_varname_FollowsT1_Pn_ReturnsPn)
		{
			// Query: "print pn; Select pn.varName such that FollowsT(1,pn)"
			// Get processed_synonyms and processed clauses
			QueryNode variable_node = QueryNode();
			variable_node.setSynonymNode({ QuerySynonymType::print }, "pn");
			std::unordered_map<std::string, QueryNode> processed_synonyms = { {"pn", variable_node} };

			QueryNode child1 = QueryNode();
			child1.setAttrNode("pn", "varName");
			QueryNode child2 = QueryNode();
			child2.setNodeType({ QueryNodeType::such_that });
			QueryNode child_child1 = QueryNode();
			child_child1.setNodeType({ QueryNodeType::followsT });
			QueryNode child_child_child1 = QueryNode();
			child_child_child1.setIntegerNode(1);
			QueryNode child_child_child2 = QueryNode();
			child_child_child2.setSynonymNode({ QuerySynonymType::print }, "pn");
			QueryNode child_child1_children[] = { child_child_child1, child_child_child2 };
			child_child1.setChildren(child_child1_children, 2);
			QueryNode child2_children[] = { child_child1 };
			child2.setChildren(child2_children, 1);

			QueryNode root = QueryNode();
			root.setNodeType({ QueryNodeType::select });
			QueryNode tuple = QueryNode();
			tuple.setNodeType({ QueryNodeType::tuple });
			QueryNode tuple_children[] = { child1 };
			tuple.setChildren(tuple_children, 1);
			QueryNode root_children[] = { tuple, child2 };
			root.setChildren(root_children, 2);

			QueryNode processed_clauses = root; //stores root node of the tree

			// Evaluate
			QueryEvaluator qe = QueryEvaluator(*pkb);
			QUERY_RESULT result;
			STRING_RESULT result_string;

			try {
				result = qe.evaluateQuery(processed_synonyms, processed_clauses);
				result_string = ResultListManager::getStringValues(result);
			}
			catch (const char* msg) {
				Logger::WriteMessage("Evaluate exception caught");
			}
			STRING_RESULT correct_result = "b, c, d, e";

			Logger::WriteMessage(result_string.c_str());
			Assert::IsTrue(result_string.compare(correct_result) == 0);
		}

		TEST_METHOD(evaluateQuery_SelectS2FollowsTS1_S2_Returns2)
		{
			// Query: "stmt s1, s2; Select s2 such that FollowsT(s1,s2)"
			// Get processed_synonyms and processed clauses
			QueryNode s1_node = QueryNode();
			s1_node.setSynonymNode({ QuerySynonymType::stmt }, "s1");
			QueryNode s2_node = QueryNode();
			s2_node.setSynonymNode({ QuerySynonymType::stmt }, "s2");
			std::unordered_map<std::string, QueryNode> processed_synonyms = { {"s1", s1_node}, {"s2", s2_node} };

			QueryNode child1 = QueryNode();
			child1.setSynonymNode({ QuerySynonymType::print }, "s2");
			QueryNode child2 = QueryNode();
			child2.setNodeType({ QueryNodeType::such_that });
			QueryNode child_child1 = QueryNode();
			child_child1.setNodeType({ QueryNodeType::followsT });
			QueryNode child_child_child1 = QueryNode();
			child_child_child1.setSynonymNode({ QuerySynonymType::stmt }, "s1");
			QueryNode child_child_child2 = QueryNode();
			child_child_child2.setSynonymNode({ QuerySynonymType::stmt }, "s2");
			QueryNode child_child1_children[] = { child_child_child1, child_child_child2 };
			child_child1.setChildren(child_child1_children, 2);
			QueryNode child2_children[] = { child_child1 };
			child2.setChildren(child2_children, 1);

			QueryNode root = QueryNode();
			root.setNodeType({ QueryNodeType::select });
			QueryNode tuple = QueryNode();
			tuple.setNodeType({ QueryNodeType::tuple });
			QueryNode tuple_children[] = { child1 };
			tuple.setChildren(tuple_children, 1);
			QueryNode root_children[] = { tuple, child2 };
			root.setChildren(root_children, 2);

			QueryNode processed_clauses = root; //stores root node of the tree

			// Evaluate
			QueryEvaluator qe = QueryEvaluator(*pkb);
			QUERY_RESULT result;
			STRING_RESULT result_string;

			try {
				result = qe.evaluateQuery(processed_synonyms, processed_clauses);
				result_string = ResultListManager::getStringValues(result);
			}
			catch (const char* msg) {
				Logger::WriteMessage("Evaluate exception caught");
			}
			STRING_RESULT correct_result = "2, 3, 4, 5, 3, 4, 5, 4, 5, 5";

			Logger::WriteMessage(result_string.c_str());
			Assert::IsTrue(result_string.compare(correct_result) == 0);
		}

		TEST_METHOD(evaluateQuery_SelectVFollowsTPn_3_ReturnsV)
		{
			// Query: "print pn; variable v; Select v such that FollowsT(pn,3)"
			// Get processed_synonyms and processed clauses
			QueryNode print_node = QueryNode();
			print_node.setSynonymNode({ QuerySynonymType::print }, "pn");
			QueryNode variable_node = QueryNode();
			variable_node.setSynonymNode({ QuerySynonymType::variable }, "v");
			std::unordered_map<std::string, QueryNode> processed_synonyms = { {"pn", print_node}, {"v", variable_node} };

			QueryNode child1 = QueryNode();
			child1.setSynonymNode({ QuerySynonymType::variable }, "v");
			QueryNode child2 = QueryNode();
			child2.setNodeType({ QueryNodeType::such_that });
			QueryNode child_child1 = QueryNode();
			child_child1.setNodeType({ QueryNodeType::followsT });
			QueryNode child_child_child1 = QueryNode();
			child_child_child1.setSynonymNode({ QuerySynonymType::print }, "pn");
			QueryNode child_child_child2 = QueryNode();
			child_child_child2.setIntegerNode(3);
			QueryNode child_child1_children[] = { child_child_child1, child_child_child2 };
			child_child1.setChildren(child_child1_children, 2);
			QueryNode child2_children[] = { child_child1 };
			child2.setChildren(child2_children, 1);

			QueryNode root = QueryNode();
			root.setNodeType({ QueryNodeType::select });
			QueryNode tuple = QueryNode();
			tuple.setNodeType({ QueryNodeType::tuple });
			QueryNode tuple_children[] = { child1 };
			tuple.setChildren(tuple_children, 1);
			QueryNode root_children[] = { tuple, child2 };
			root.setChildren(root_children, 2);

			QueryNode processed_clauses = root; //stores root node of the tree

			// Evaluate
			QueryEvaluator qe = QueryEvaluator(*pkb);
			QUERY_RESULT result;
			STRING_RESULT result_string;

			try {
				result = qe.evaluateQuery(processed_synonyms, processed_clauses);
				result_string = ResultListManager::getStringValues(result);
			}
			catch (const char* msg) {
				Logger::WriteMessage("Evaluate exception caught");
			}
			STRING_RESULT correct_result = "a, b, c, d, e";

			Logger::WriteMessage(result_string.c_str());
			Assert::IsTrue(result_string.compare(correct_result) == 0);
		}

		/*=================================  PKB 1: Parent =========================================*/
		TEST_METHOD(evaluateQuery_SelectSParentS_S_ReturnsEmpty)
		{
			// Query: "stmt s; Select s such that Parent(s,s)"
			// Get processed_synonyms and processed clauses
			QueryNode stmt_node = QueryNode();
			stmt_node.setSynonymNode({ QuerySynonymType::stmt }, "s");
			std::unordered_map<std::string, QueryNode> processed_synonyms = { {"s", stmt_node} };

			// Select: s
			QueryNode child1 = QueryNode();
			child1.setSynonymNode({ QuerySynonymType::stmt }, "s");

			// such that 
			QueryNode child2 = QueryNode();
			child2.setNodeType({ QueryNodeType::such_that });

			// parent
			QueryNode child_child1 = QueryNode();
			child_child1.setNodeType({ QueryNodeType::parent });

			// arg 1: s
			QueryNode child_child_child1 = QueryNode();
			child_child_child1.setSynonymNode({ QuerySynonymType::stmt }, "s");

			// arg 2: s
			QueryNode child_child_child2 = QueryNode();
			child_child_child2.setSynonymNode({ QuerySynonymType::stmt }, "s");
			QueryNode child_child1_children[] = { child_child_child1, child_child_child2 };

			// set children, make tree
			child_child1.setChildren(child_child1_children, 2);
			QueryNode child2_children[] = { child_child1 };
			child2.setChildren(child2_children, 1);

			QueryNode root = QueryNode();
			root.setNodeType({ QueryNodeType::select });
			QueryNode tuple = QueryNode();
			tuple.setNodeType({ QueryNodeType::tuple });
			QueryNode tuple_children[] = { child1 };
			tuple.setChildren(tuple_children, 1);
			QueryNode root_children[] = { tuple, child2 };
			root.setChildren(root_children, 2);

			QueryNode processed_clauses = root; //stores root node of the tree

			// Evaluate
			QueryEvaluator qe = QueryEvaluator(*pkb);
			QUERY_RESULT result;
			STRING_RESULT result_string;

			try {
				result = qe.evaluateQuery(processed_synonyms, processed_clauses);
				result_string = ResultListManager::getStringValues(result);
			}
			catch (const char* msg) {
				Logger::WriteMessage("Evaluate exception caught");
			}
			STRING_RESULT correct_result = "";

			Logger::WriteMessage(result_string.c_str());
			Assert::IsTrue(result_string.compare(correct_result) == 0);
		}

		TEST_METHOD(evaluateQuery_SelectSParent1_2_ReturnsEmpty)
		{
			// Query: "stmt s; Select s such that Parent(1,2)"
			// Get processed_synonyms and processed clauses
			QueryNode variable_node = QueryNode();
			variable_node.setSynonymNode({ QuerySynonymType::stmt }, "s");
			std::unordered_map<std::string, QueryNode> processed_synonyms = { {"s", variable_node} };

			QueryNode child1 = QueryNode();
			child1.setSynonymNode({ QuerySynonymType::stmt }, "ss");
			QueryNode child2 = QueryNode();
			child2.setNodeType({ QueryNodeType::such_that });
			QueryNode child_child1 = QueryNode();
			child_child1.setNodeType({ QueryNodeType::parent });
			QueryNode child_child_child1 = QueryNode();
			child_child_child1.setIntegerNode(1);
			QueryNode child_child_child2 = QueryNode();
			child_child_child2.setIntegerNode(2);
			QueryNode child_child1_children[] = { child_child_child1, child_child_child2 };
			child_child1.setChildren(child_child1_children, 2);
			QueryNode child2_children[] = { child_child1 };
			child2.setChildren(child2_children, 1);

			QueryNode root = QueryNode();
			root.setNodeType({ QueryNodeType::select });
			QueryNode tuple = QueryNode();
			tuple.setNodeType({ QueryNodeType::tuple });
			QueryNode tuple_children[] = { child1 };
			tuple.setChildren(tuple_children, 1);
			QueryNode root_children[] = { tuple, child2 };
			root.setChildren(root_children, 2);

			QueryNode processed_clauses = root; //stores root node of the tree

			// Evaluate
			QueryEvaluator qe = QueryEvaluator(*pkb);
			QUERY_RESULT result = qe.evaluateQuery(processed_synonyms, processed_clauses);
			STRING_RESULT result_string = ResultListManager::getStringValues(result);
			STRING_RESULT correct_result = "";

			Logger::WriteMessage(result_string.c_str());
			Assert::IsTrue(result_string.compare(correct_result) == 0);
		}

		/*=================================  PKB 1: ParentT =========================================*/

		TEST_METHOD(evaluateQuery_SelectSParentTS_S_ReturnsEmpty)
		{
			// Query: "stmt s; Select s such that ParentT(s,s)"
			// Get processed_synonyms and processed clauses
			QueryNode stmt_node = QueryNode();
			stmt_node.setSynonymNode({ QuerySynonymType::stmt }, "s");
			std::unordered_map<std::string, QueryNode> processed_synonyms = { {"s", stmt_node} };

			// Select: s
			QueryNode child1 = QueryNode();
			child1.setSynonymNode({ QuerySynonymType::stmt }, "s");

			// such that 
			QueryNode child2 = QueryNode();
			child2.setNodeType({ QueryNodeType::such_that });

			// parentT
			QueryNode child_child1 = QueryNode();
			child_child1.setNodeType({ QueryNodeType::parentT });

			// arg 1: s
			QueryNode child_child_child1 = QueryNode();
			child_child_child1.setSynonymNode({ QuerySynonymType::stmt }, "s");

			// arg 2: s
			QueryNode child_child_child2 = QueryNode();
			child_child_child2.setSynonymNode({ QuerySynonymType::stmt }, "s");
			QueryNode child_child1_children[] = { child_child_child1, child_child_child2 };

			// set children, make tree
			child_child1.setChildren(child_child1_children, 2);
			QueryNode child2_children[] = { child_child1 };
			child2.setChildren(child2_children, 1);

			QueryNode root = QueryNode();
			root.setNodeType({ QueryNodeType::select });
			QueryNode tuple = QueryNode();
			tuple.setNodeType({ QueryNodeType::tuple });
			QueryNode tuple_children[] = { child1 };
			tuple.setChildren(tuple_children, 1);
			QueryNode root_children[] = { tuple, child2 };
			root.setChildren(root_children, 2);

			QueryNode processed_clauses = root; //stores root node of the tree

			// Evaluate
			QueryEvaluator qe = QueryEvaluator(*pkb);
			QUERY_RESULT result;
			STRING_RESULT result_string;

			try {
				result = qe.evaluateQuery(processed_synonyms, processed_clauses);
				result_string = ResultListManager::getStringValues(result);
			}
			catch (const char* msg) {
				Logger::WriteMessage("Evaluate exception caught");
			}
			STRING_RESULT correct_result = "";

			Logger::WriteMessage(result_string.c_str());
			Assert::IsTrue(result_string.compare(correct_result) == 0);
		}

		TEST_METHOD(evaluateQuery_SelectPnParentT4_Pn_ReturnsEmpty)
		{
			// Query: "print pn; Select pn such that ParentT(4,pn)"
			// Get processed_synonyms and processed clauses
			QueryNode variable_node = QueryNode();
			variable_node.setSynonymNode({ QuerySynonymType::print }, "pn");
			std::unordered_map<std::string, QueryNode> processed_synonyms = { {"pn", variable_node} };

			QueryNode child1 = QueryNode();
			child1.setSynonymNode({ QuerySynonymType::print }, "pn");
			QueryNode child2 = QueryNode();
			child2.setNodeType({ QueryNodeType::such_that });
			QueryNode child_child1 = QueryNode();
			child_child1.setNodeType({ QueryNodeType::parentT });
			QueryNode child_child_child1 = QueryNode();
			child_child_child1.setIntegerNode(4);
			QueryNode child_child_child2 = QueryNode();
			child_child_child2.setSynonymNode({ QuerySynonymType::print }, "pn");
			QueryNode child_child1_children[] = { child_child_child1, child_child_child2 };
			child_child1.setChildren(child_child1_children, 2);
			QueryNode child2_children[] = { child_child1 };
			child2.setChildren(child2_children, 1);

			QueryNode root = QueryNode();
			root.setNodeType({ QueryNodeType::select });
			QueryNode tuple = QueryNode();
			tuple.setNodeType({ QueryNodeType::tuple });
			QueryNode tuple_children[] = { child1 };
			tuple.setChildren(tuple_children, 1);
			QueryNode root_children[] = { tuple, child2 };
			root.setChildren(root_children, 2);

			QueryNode processed_clauses = root; //stores root node of the tree

			// Evaluate
			QueryEvaluator qe = QueryEvaluator(*pkb);
			QUERY_RESULT result = qe.evaluateQuery(processed_synonyms, processed_clauses);
			STRING_RESULT result_string = ResultListManager::getStringValues(result);
			STRING_RESULT correct_result = "";

			Logger::WriteMessage(result_string.c_str());
			Assert::IsTrue(result_string.compare(correct_result) == 0);
		}

		/*=================================  PKB 1: UsesS =========================================*/

		TEST_METHOD(evaluateQuery_SelectPnUsesPn_v_ReturnsPn)
		{
			// Query: "print pn; variable v; Select pn such that Uses(pn,v)"
			// Get processed_synonyms and processed clauses
			QueryNode print_node = QueryNode();
			print_node.setSynonymNode({ QuerySynonymType::print }, "pn");
			QueryNode variable_node = QueryNode();
			variable_node.setSynonymNode({ QuerySynonymType::variable }, "v");
			std::unordered_map<std::string, QueryNode> processed_synonyms = { {"pn", print_node}, {"v", variable_node} };

			QueryNode child1 = QueryNode();
			child1.setSynonymNode({ QuerySynonymType::print }, "pn");
			QueryNode child2 = QueryNode();
			child2.setNodeType({ QueryNodeType::such_that });
			QueryNode child_child1 = QueryNode();
			child_child1.setNodeType({ QueryNodeType::usesS });
			QueryNode child_child_child1 = QueryNode();
			child_child_child1.setSynonymNode({ QuerySynonymType::print }, "pn");
			QueryNode child_child_child2 = QueryNode();
			child_child_child2.setSynonymNode({ QuerySynonymType::variable }, "v");
			QueryNode child_child1_children[] = { child_child_child1, child_child_child2 };
			child_child1.setChildren(child_child1_children, 2);
			QueryNode child2_children[] = { child_child1 };
			child2.setChildren(child2_children, 1);

			QueryNode root = QueryNode();
			root.setNodeType({ QueryNodeType::select });
			QueryNode tuple = QueryNode();
			tuple.setNodeType({ QueryNodeType::tuple });
			QueryNode tuple_children[] = { child1 };
			tuple.setChildren(tuple_children, 1);
			QueryNode root_children[] = { tuple, child2 };
			root.setChildren(root_children, 2);

			QueryNode processed_clauses = root; //stores root node of the tree

			// Evaluate
			QueryEvaluator qe = QueryEvaluator(*pkb);
			QUERY_RESULT result = qe.evaluateQuery(processed_synonyms, processed_clauses);
			STRING_RESULT result_string = ResultListManager::getStringValues(result);
			STRING_RESULT correct_result = "1, 2, 3, 4, 5";

			Logger::WriteMessage(result_string.c_str());
			Assert::IsTrue(result_string.compare(correct_result) == 0);
		}

		TEST_METHOD(evaluateQuery_SelectVUsesPn_v_ReturnsV)
		{
			// Query: "print pn; variable v; Select pn such that Uses(pn,v)"
			// Get processed_synonyms and processed clauses
			QueryNode print_node = QueryNode();
			print_node.setSynonymNode({ QuerySynonymType::print }, "pn");
			QueryNode variable_node = QueryNode();
			variable_node.setSynonymNode({ QuerySynonymType::variable }, "v");
			std::unordered_map<std::string, QueryNode> processed_synonyms = { {"pn", print_node}, {"v", variable_node} };

			QueryNode child1 = QueryNode();
			child1.setSynonymNode({ QuerySynonymType::variable }, "v");
			QueryNode child2 = QueryNode();
			child2.setNodeType({ QueryNodeType::such_that });
			QueryNode child_child1 = QueryNode();
			child_child1.setNodeType({ QueryNodeType::usesS });
			QueryNode child_child_child1 = QueryNode();
			child_child_child1.setSynonymNode({ QuerySynonymType::print }, "pn");
			QueryNode child_child_child2 = QueryNode();
			child_child_child2.setSynonymNode({ QuerySynonymType::variable }, "v");
			QueryNode child_child1_children[] = { child_child_child1, child_child_child2 };
			child_child1.setChildren(child_child1_children, 2);
			QueryNode child2_children[] = { child_child1 };
			child2.setChildren(child2_children, 1);

			QueryNode root = QueryNode();
			root.setNodeType({ QueryNodeType::select });
			QueryNode tuple = QueryNode();
			tuple.setNodeType({ QueryNodeType::tuple });
			QueryNode tuple_children[] = { child1 };
			tuple.setChildren(tuple_children, 1);
			QueryNode root_children[] = { tuple, child2 };
			root.setChildren(root_children, 2);

			QueryNode processed_clauses = root; //stores root node of the tree

			// Evaluate
			QueryEvaluator qe = QueryEvaluator(*pkb);
			QUERY_RESULT result = qe.evaluateQuery(processed_synonyms, processed_clauses);
			STRING_RESULT result_string = ResultListManager::getStringValues(result);
			STRING_RESULT correct_result = "a, b, c, d, e";

			Logger::WriteMessage(result_string.c_str());
			Assert::IsTrue(result_string.compare(correct_result) == 0);
		}

		TEST_METHOD(evaluateQuery_SelectSUses1_v_ReturnsS)
		{
			// Query: "stmt s; variable v; Select s such that Uses(1,v)"
			// Get processed_synonyms and processed clauses
			QueryNode print_node = QueryNode();
			print_node.setSynonymNode({ QuerySynonymType::stmt }, "s");
			QueryNode variable_node = QueryNode();
			variable_node.setSynonymNode({ QuerySynonymType::variable }, "v");
			std::unordered_map<std::string, QueryNode> processed_synonyms = { {"s", print_node}, {"v", variable_node} };

			QueryNode child1 = QueryNode();
			child1.setSynonymNode({ QuerySynonymType::stmt }, "s");
			QueryNode child2 = QueryNode();
			child2.setNodeType({ QueryNodeType::such_that });
			QueryNode child_child1 = QueryNode();
			child_child1.setNodeType({ QueryNodeType::usesS });
			QueryNode child_child_child1 = QueryNode();
			child_child_child1.setIntegerNode(1);
			QueryNode child_child_child2 = QueryNode();
			child_child_child2.setSynonymNode({ QuerySynonymType::variable }, "v");
			QueryNode child_child1_children[] = { child_child_child1, child_child_child2 };
			child_child1.setChildren(child_child1_children, 2);
			QueryNode child2_children[] = { child_child1 };
			child2.setChildren(child2_children, 1);

			QueryNode root = QueryNode();
			root.setNodeType({ QueryNodeType::select });
			QueryNode tuple = QueryNode();
			tuple.setNodeType({ QueryNodeType::tuple });
			QueryNode tuple_children[] = { child1 };
			tuple.setChildren(tuple_children, 1);
			QueryNode root_children[] = { tuple, child2 };
			root.setChildren(root_children, 2);

			QueryNode processed_clauses = root; //stores root node of the tree

			// Evaluate
			QueryEvaluator qe = QueryEvaluator(*pkb);
			QUERY_RESULT result = qe.evaluateQuery(processed_synonyms, processed_clauses);
			STRING_RESULT result_string = ResultListManager::getStringValues(result);
			STRING_RESULT correct_result = "1, 2, 3, 4, 5";

			Logger::WriteMessage(result_string.c_str());
			Assert::IsTrue(result_string.compare(correct_result) == 0);
		}

		TEST_METHOD(evaluateQuery_SelectBOOLEAN_Uses1_v_ReturnsTrue)
		{
			// Query: "stmt s; variable v; Select BOOLEAN such that Uses(1,v)"
			// Get processed_synonyms and processed clauses
			QueryNode print_node = QueryNode();
			print_node.setSynonymNode({ QuerySynonymType::stmt }, "s");
			QueryNode variable_node = QueryNode();
			variable_node.setSynonymNode({ QuerySynonymType::variable }, "v");
			std::unordered_map<std::string, QueryNode> processed_synonyms = { {"s", print_node}, {"v", variable_node} };

			// Select: BOOLEAN
			QueryNode child1 = QueryNode();
			child1.setBooleanNode();

			// such that
			QueryNode child2 = QueryNode();
			child2.setNodeType({ QueryNodeType::such_that });

			// usesS
			QueryNode child_child1 = QueryNode();
			child_child1.setNodeType({ QueryNodeType::usesS });

			// arg1: 1
			QueryNode child_child_child1 = QueryNode();
			child_child_child1.setIntegerNode(1);

			// arg2: v
			QueryNode child_child_child2 = QueryNode();
			child_child_child2.setSynonymNode({ QuerySynonymType::variable }, "v");

			// set children
			QueryNode child_child1_children[] = { child_child_child1, child_child_child2 };
			child_child1.setChildren(child_child1_children, 2);
			QueryNode child2_children[] = { child_child1 };
			child2.setChildren(child2_children, 1);

			QueryNode root = QueryNode();
			root.setNodeType({ QueryNodeType::select });
			QueryNode root_children[] = { child1, child2 };
			root.setChildren(root_children, 2);

			QueryNode processed_clauses = root; //stores root node of the tree

			// Evaluate
			QueryEvaluator qe = QueryEvaluator(*pkb);
			QUERY_RESULT result = qe.evaluateQuery(processed_synonyms, processed_clauses);
			STRING_RESULT result_string = ResultListManager::getStringValues(result);
			STRING_RESULT correct_result = "TRUE";

			Logger::WriteMessage("Result: ");
			Logger::WriteMessage(result_string.c_str());
			Assert::IsTrue(result_string.compare(correct_result) == 0);
		}

		TEST_METHOD(evaluateQuery_SelectV_UsesIfs_v_ReturnsEmpty)
		{
			// Query: "if ifs; variable v; Select v such that Uses(ifs,v)"
			// Get processed_synonyms and processed clauses
			QueryNode print_node = QueryNode();
			print_node.setSynonymNode({ QuerySynonymType::ifs }, "ifs");
			QueryNode variable_node = QueryNode();
			variable_node.setSynonymNode({ QuerySynonymType::variable }, "v");
			std::unordered_map<std::string, QueryNode> processed_synonyms = { {"ifs", print_node}, {"v", variable_node} };

			QueryNode child1 = QueryNode();
			child1.setSynonymNode({ QuerySynonymType::variable }, "v");
			QueryNode child2 = QueryNode();
			child2.setNodeType({ QueryNodeType::such_that });
			QueryNode child_child1 = QueryNode();
			child_child1.setNodeType({ QueryNodeType::usesS });
			QueryNode child_child_child1 = QueryNode();
			child_child_child1.setSynonymNode({ QuerySynonymType::ifs }, "ifs");
			QueryNode child_child_child2 = QueryNode();
			child_child_child2.setSynonymNode({ QuerySynonymType::variable }, "v");
			QueryNode child_child1_children[] = { child_child_child1, child_child_child2 };
			child_child1.setChildren(child_child1_children, 2);
			QueryNode child2_children[] = { child_child1 };
			child2.setChildren(child2_children, 1);

			QueryNode root = QueryNode();
			root.setNodeType({ QueryNodeType::select });
			QueryNode tuple = QueryNode();
			tuple.setNodeType({ QueryNodeType::tuple });
			QueryNode tuple_children[] = { child1 };
			tuple.setChildren(tuple_children, 1);
			QueryNode root_children[] = { tuple, child2 };
			root.setChildren(root_children, 2);

			QueryNode processed_clauses = root; //stores root node of the tree

			// Evaluate
			QueryEvaluator qe = QueryEvaluator(*pkb);
			QUERY_RESULT result = qe.evaluateQuery(processed_synonyms, processed_clauses);
			STRING_RESULT result_string = ResultListManager::getStringValues(result);
			STRING_RESULT correct_result = "";

			Logger::WriteMessage(result_string.c_str());
			Assert::IsTrue(result_string.compare(correct_result) == 0);
		}

		TEST_METHOD(evaluateQuery_SelectBOOLEAN_UsesIfs_v_ReturnsFalse)
		{
			// Query: "if ifs; variable v; Select BOOLEAN such that Uses(ifs,v)"
			// Get processed_synonyms and processed clauses
			QueryNode print_node = QueryNode();
			print_node.setSynonymNode({ QuerySynonymType::ifs }, "ifs");
			QueryNode variable_node = QueryNode();
			variable_node.setSynonymNode({ QuerySynonymType::variable }, "v");
			std::unordered_map<std::string, QueryNode> processed_synonyms = { {"ifs", print_node}, {"v", variable_node} };

			// Select: BOOLEAN
			QueryNode child1 = QueryNode();
			child1.setBooleanNode();

			// such that
			QueryNode child2 = QueryNode();
			child2.setNodeType({ QueryNodeType::such_that });

			// follows
			QueryNode child_child1 = QueryNode();
			child_child1.setNodeType({ QueryNodeType::usesS });

			// arg1: ifs
			QueryNode child_child_child1 = QueryNode();
			child_child_child1.setSynonymNode({ QuerySynonymType::ifs }, "ifs");

			// arg2: v
			QueryNode child_child_child2 = QueryNode();
			child_child_child2.setSynonymNode({ QuerySynonymType::variable }, "v");

			// set children
			QueryNode child_child1_children[] = { child_child_child1, child_child_child2 };
			child_child1.setChildren(child_child1_children, 2);
			QueryNode child2_children[] = { child_child1 };
			child2.setChildren(child2_children, 1);

			QueryNode root = QueryNode();
			root.setNodeType({ QueryNodeType::select });
			QueryNode root_children[] = { child1, child2 };
			root.setChildren(root_children, 2);

			QueryNode processed_clauses = root; //stores root node of the tree

			// Evaluate
			QueryEvaluator qe = QueryEvaluator(*pkb);
			QUERY_RESULT result = qe.evaluateQuery(processed_synonyms, processed_clauses);
			STRING_RESULT result_string = ResultListManager::getStringValues(result);
			STRING_RESULT correct_result = "FALSE";

			Logger::WriteMessage("Result: ");
			Logger::WriteMessage(result_string.c_str());
			Assert::IsTrue(result_string.compare(correct_result) == 0);
		}

		/*=================================  PKB 1: UsesP =========================================*/

		TEST_METHOD(evaluateQuery_SelectBOOLEAN_UsesPIDENT_IDENT_ReturnsTrue)
		{
			// Query: "BOOLEAN such that UsesP("main",a)"
			// Get processed_synonyms and processed clauses
			std::unordered_map<std::string, QueryNode> processed_synonyms = {};

			// Select: p1
			QueryNode child1 = QueryNode();
			child1.setBooleanNode();

			// such that 
			QueryNode child2 = QueryNode();
			child2.setNodeType({ QueryNodeType::such_that });

			// Calls
			QueryNode child_child1 = QueryNode();
			child_child1.setNodeType({ QueryNodeType::usesP });

			// arg 1: "main"
			QueryNode child_child_child1 = QueryNode();
			child_child_child1.setIdentityNode("main");

			// arg 2: "afternoon"
			QueryNode child_child_child2 = QueryNode();
			child_child_child2.setIdentityNode("a");

			// set children, make tree
			QueryNode child_child1_children[] = { child_child_child1, child_child_child2 };
			child_child1.setChildren(child_child1_children, 2);
			QueryNode child2_children[] = { child_child1 };
			child2.setChildren(child2_children, 1);

			QueryNode root = QueryNode();
			root.setNodeType({ QueryNodeType::select });
			QueryNode root_children[] = { child1, child2 };
			root.setChildren(root_children, 2);

			QueryNode processed_clauses = root; //stores root node of the tree

			// Evaluate
			QueryEvaluator qe = QueryEvaluator(*pkb);
			QUERY_RESULT result;
			STRING_RESULT result_string;

			try {
				result = qe.evaluateQuery(processed_synonyms, processed_clauses);
				result_string = ResultListManager::getStringValues(result);
			}
			catch (const char* msg) {
				Logger::WriteMessage(msg);
			}
			STRING_RESULT correct_result = "TRUE";

			Logger::WriteMessage(result_string.c_str());
			Assert::IsTrue(result_string.compare(correct_result) == 0);
		}

		TEST_METHOD(evaluateQuery_SelectVUsesSP_v_ReturnsV)
		{
			// Query: "variable v; procedure p; Select v such that UsesP(p,v)"
			// Get processed_synonyms and processed clauses
			QueryNode variable_node = QueryNode();
			variable_node.setSynonymNode({ QuerySynonymType::variable }, "v");
			QueryNode proc_node = QueryNode();
			proc_node.setSynonymNode({ QuerySynonymType::procedure }, "p");
			std::unordered_map<std::string, QueryNode> processed_synonyms = { {"v", variable_node}, {"p", proc_node} };

			QueryNode child1 = QueryNode();
			child1.setSynonymNode({ QuerySynonymType::variable }, "v");
			QueryNode child2 = QueryNode();
			child2.setNodeType({ QueryNodeType::such_that });
			QueryNode child_child1 = QueryNode();
			child_child1.setNodeType({ QueryNodeType::usesP });
			QueryNode child_child_child1 = QueryNode();
			child_child_child1.setSynonymNode({ QuerySynonymType::procedure }, "p");
			QueryNode child_child_child2 = QueryNode();
			child_child_child2.setSynonymNode({ QuerySynonymType::variable }, "v");
			QueryNode child_child1_children[] = { child_child_child1, child_child_child2 };
			child_child1.setChildren(child_child1_children, 2);
			QueryNode child2_children[] = { child_child1 };
			child2.setChildren(child2_children, 1);

			QueryNode root = QueryNode();
			root.setNodeType({ QueryNodeType::select });
			QueryNode tuple = QueryNode();
			tuple.setNodeType({ QueryNodeType::tuple });
			QueryNode tuple_children[] = { child1 };
			tuple.setChildren(tuple_children, 1);
			QueryNode root_children[] = { tuple, child2 };
			root.setChildren(root_children, 2);

			QueryNode processed_clauses = root; //stores root node of the tree

			// Evaluate
			QueryEvaluator qe = QueryEvaluator(*pkb);
			QUERY_RESULT result = qe.evaluateQuery(processed_synonyms, processed_clauses);
			STRING_RESULT result_string = ResultListManager::getStringValues(result);
			STRING_RESULT correct_result = "a, b, c, d, e";

			Logger::WriteMessage(result_string.c_str());
			Assert::IsTrue(result_string.compare(correct_result) == 0);
		}

		TEST_METHOD(evaluateQuery_SelectPUsesSP_v_ReturnsP)
		{
			// Query: "variable v; procedure p; Select v such that UsesP(p,v)"
			// Get processed_synonyms and processed clauses
			QueryNode variable_node = QueryNode();
			variable_node.setSynonymNode({ QuerySynonymType::variable }, "v");
			QueryNode proc_node = QueryNode();
			proc_node.setSynonymNode({ QuerySynonymType::procedure }, "p");
			std::unordered_map<std::string, QueryNode> processed_synonyms = { {"v", variable_node}, {"p", proc_node} };

			QueryNode child1 = QueryNode();
			child1.setSynonymNode({ QuerySynonymType::procedure }, "p");
			QueryNode child2 = QueryNode();
			child2.setNodeType({ QueryNodeType::such_that });
			QueryNode child_child1 = QueryNode();
			child_child1.setNodeType({ QueryNodeType::usesP });
			QueryNode child_child_child1 = QueryNode();
			child_child_child1.setSynonymNode({ QuerySynonymType::procedure }, "p");
			QueryNode child_child_child2 = QueryNode();
			child_child_child2.setSynonymNode({ QuerySynonymType::variable }, "v");
			QueryNode child_child1_children[] = { child_child_child1, child_child_child2 };
			child_child1.setChildren(child_child1_children, 2);
			QueryNode child2_children[] = { child_child1 };
			child2.setChildren(child2_children, 1);

			QueryNode root = QueryNode();
			root.setNodeType({ QueryNodeType::select });
			QueryNode tuple = QueryNode();
			tuple.setNodeType({ QueryNodeType::tuple });
			QueryNode tuple_children[] = { child1 };
			tuple.setChildren(tuple_children, 1);
			QueryNode root_children[] = { tuple, child2 };
			root.setChildren(root_children, 2);

			QueryNode processed_clauses = root; //stores root node of the tree

			// Evaluate
			QueryEvaluator qe = QueryEvaluator(*pkb);
			QUERY_RESULT result = qe.evaluateQuery(processed_synonyms, processed_clauses);
			STRING_RESULT result_string = ResultListManager::getStringValues(result);
			STRING_RESULT correct_result = "main, main, main, main, main";

			Logger::WriteMessage(result_string.c_str());
			Assert::IsTrue(result_string.compare(correct_result) == 0);
		}

		/*=================================  PKB 1: Modifies =========================================*/

		TEST_METHOD(evaluateQuery_SelectVModifies2_v_ReturnsEmpty)
		{
			// Query: "variable v; Select v such that ModifiesS(2,v)"
			// Get processed_synonyms and processed clauses
			QueryNode variable_node = QueryNode();
			variable_node.setSynonymNode({ QuerySynonymType::variable }, "v");
			std::unordered_map<std::string, QueryNode> processed_synonyms = { {"v", variable_node} };

			QueryNode child1 = QueryNode();
			child1.setSynonymNode({ QuerySynonymType::variable }, "v");
			QueryNode child2 = QueryNode();
			child2.setNodeType({ QueryNodeType::such_that });
			QueryNode child_child1 = QueryNode();
			child_child1.setNodeType({ QueryNodeType::modifiesS });
			QueryNode child_child_child1 = QueryNode();
			child_child_child1.setIntegerNode(2);
			QueryNode child_child_child2 = QueryNode();
			child_child_child2.setSynonymNode({ QuerySynonymType::variable }, "v");
			QueryNode child_child1_children[] = { child_child_child1, child_child_child2 };
			child_child1.setChildren(child_child1_children, 2);
			QueryNode child2_children[] = { child_child1 };
			child2.setChildren(child2_children, 1);

			QueryNode root = QueryNode();
			root.setNodeType({ QueryNodeType::select });
			QueryNode tuple = QueryNode();
			tuple.setNodeType({ QueryNodeType::tuple });
			QueryNode tuple_children[] = { child1 };
			tuple.setChildren(tuple_children, 1);
			QueryNode root_children[] = { tuple, child2 };
			root.setChildren(root_children, 2);

			QueryNode processed_clauses = root; //stores root node of the tree

			// Evaluate
			QueryEvaluator qe = QueryEvaluator(*pkb);
			QUERY_RESULT result = qe.evaluateQuery(processed_synonyms, processed_clauses);
			STRING_RESULT result_string = ResultListManager::getStringValues(result);
			STRING_RESULT correct_result = "";

			Logger::WriteMessage(result_string.c_str());
			Assert::IsTrue(result_string.compare(correct_result) == 0);
		}

		TEST_METHOD(evaluateQuery_SelectVModifiesP_v_ReturnsEmpty)
		{
			// Query: "variable v; procedure p; Select v such that ModifiesP(p,v)"
			// Get processed_synonyms and processed clauses
			QueryNode variable_node = QueryNode();
			variable_node.setSynonymNode({ QuerySynonymType::variable }, "v");
			QueryNode proc_node = QueryNode();
			proc_node.setSynonymNode({ QuerySynonymType::procedure }, "p");
			std::unordered_map<std::string, QueryNode> processed_synonyms = { {"v", variable_node}, {"p", proc_node} };

			QueryNode child1 = QueryNode();
			child1.setSynonymNode({ QuerySynonymType::variable }, "v");
			QueryNode child2 = QueryNode();
			child2.setNodeType({ QueryNodeType::such_that });
			QueryNode child_child1 = QueryNode();
			child_child1.setNodeType({ QueryNodeType::modifiesP });
			QueryNode child_child_child1 = QueryNode();
			child_child_child1.setSynonymNode({ QuerySynonymType::procedure }, "p");
			QueryNode child_child_child2 = QueryNode();
			child_child_child2.setSynonymNode({ QuerySynonymType::variable }, "v");
			QueryNode child_child1_children[] = { child_child_child1, child_child_child2 };
			child_child1.setChildren(child_child1_children, 2);
			QueryNode child2_children[] = { child_child1 };
			child2.setChildren(child2_children, 1);

			QueryNode root = QueryNode();
			root.setNodeType({ QueryNodeType::select });
			QueryNode tuple = QueryNode();
			tuple.setNodeType({ QueryNodeType::tuple });
			QueryNode tuple_children[] = { child1 };
			tuple.setChildren(tuple_children, 1);
			QueryNode root_children[] = { tuple, child2 };
			root.setChildren(root_children, 2);

			QueryNode processed_clauses = root; //stores root node of the tree

			// Evaluate
			QueryEvaluator qe = QueryEvaluator(*pkb);
			QUERY_RESULT result = qe.evaluateQuery(processed_synonyms, processed_clauses);
			STRING_RESULT result_string = ResultListManager::getStringValues(result);
			STRING_RESULT correct_result = "";

			Logger::WriteMessage(result_string.c_str());
			Assert::IsTrue(result_string.compare(correct_result) == 0);
		}

		/*=================================  PKB 2: Pattern =========================================*/

		// PATTERN TEST CASES

		// 1. (_, _)
		// 2. (_, _"x"_)
		// 3. (v, _)
		// 4. (v, _"2"_)
		// 5. ("x", _)
		// 6. ("x", _"x"_)

		// 1. assign a; variable v; Select a pattern a(_, _) //Return1_2_3
		// 2. assign a; variable v; Select a pattern a(_, _"woof"_) //Return1
		// 3. assign a; variable v; Select a pattern a(v, _) //return1_2_3
		// 4. assign a; variable v; Select a pattern a(v, _"b"_) //return2_3
		// 5. assign a; variable v; Select a pattern a("a", _)  // Return1
		// 6. assign a; variable v; Select a pattern a("c", _"2"_) //Return3

		// Extra tests
		// assign a; variable v; Select a pattern a(v, _"a"_) //return2								  
		// assign a; variable v; Select a pattern a(v, _"b"_) //return2_3								  
		// assign a; variable v; Select a pattern a(_, _"b"_) //Return2_3
		// assign a; variable v; Select a pattern a(_, _"2"_) //Return3
		// assign a; variable v; Select a pattern a("a", _)  // ReturnEmpty
		// assign a; variable v; Select a pattern a("a", _"woof"_) //Return1
		// assign a; variable v; Select a pattern a("c", _"d"_)
		// assign a; variable v; Select a pattern a(v, "3")

		TEST_METHOD(evaluateQuery_SelectBOOLEAN_PKB2_ReturnsTrue)
		{
			// Query: "Select BOOLEAN"
			std::unordered_map<std::string, QueryNode> processed_synonyms;

			QueryNode root = QueryNode();
			root.setNodeType({ QueryNodeType::select });
			QueryNode child1 = QueryNode();
			child1.setBooleanNode();
			QueryNode root_children[] = { child1 };
			root.setChildren(root_children, 1);

			QueryNode processed_clauses = root; //stores root node of the tree

			// Evaluate
			QueryEvaluator qe = QueryEvaluator(*pkb2);
			QUERY_RESULT result = qe.evaluateQuery(processed_synonyms, processed_clauses);
			STRING_RESULT result_string = ResultListManager::getStringValues(result);
			STRING_RESULT correct_result = "TRUE";

			Logger::WriteMessage("Result: ");
			Logger::WriteMessage(result_string.c_str());
			Assert::IsTrue(result_string.compare(correct_result) == 0);
		}
		TEST_METHOD(evaluateQuery_SelectA_PatternWildCardWildCard_Returns123)
		{
			// Query: "assign a; Select a pattern a(_, _)"
			// Get processed_synonyms and processed clauses
			QueryNode assign_node = QueryNode();
			assign_node.setSynonymNode({ QuerySynonymType::assign }, "a");
			std::unordered_map<std::string, QueryNode> processed_synonyms = { {"a", assign_node} };

			QueryNode child1 = QueryNode();
			child1.setSynonymNode({ QuerySynonymType::assign }, "a");
			QueryNode child2 = QueryNode();
			child2.setNodeType({ QueryNodeType::pattern });
			QueryNode child_child_child0 = QueryNode();
			child_child_child0.setSynonymNode({ QuerySynonymType::assign }, "a");
			QueryNode child_child_child1 = QueryNode();
			child_child_child1.setNodeType({ QueryNodeType::wild_card });
			QueryNode child_child_child2 = QueryNode();
			child_child_child2.setNodeType({ QueryNodeType::wild_card });
			QueryNode child2_children[] = { child_child_child0, child_child_child1, child_child_child2 };
			child2.setChildren(child2_children, 3);

			QueryNode root = QueryNode();
			root.setNodeType({ QueryNodeType::select });
			QueryNode tuple = QueryNode();
			tuple.setNodeType({ QueryNodeType::tuple });
			QueryNode tuple_children[] = { child1 };
			tuple.setChildren(tuple_children, 1);
			QueryNode root_children[] = { tuple, child2 };
			root.setChildren(root_children, 2);

			QueryNode processed_clauses = root; //stores root node of the tree

			// Evaluate
			QueryEvaluator qe = QueryEvaluator(*pkb2);
			QUERY_RESULT result = qe.evaluateQuery(processed_synonyms, processed_clauses);
			STRING_RESULT result_string = ResultListManager::getStringValues(result);
			STRING_RESULT correct_result = "1, 2, 3";

			Logger::WriteMessage("Result: ");
			Logger::WriteMessage(result_string.c_str());
			Assert::IsTrue(result_string.compare(correct_result) == 0);
		}

		TEST_METHOD(evaluateQuery_SelectA_PatternAWildCardPartial_Returns1)
		{
			// Query: "assign a; Select a pattern a(_, _"woof"_)"
			// Get processed_synonyms and processed clauses
			QueryNode assign_node = QueryNode();
			assign_node.setSynonymNode({ QuerySynonymType::assign }, "a");
			std::unordered_map<std::string, QueryNode> processed_synonyms = { {"a", assign_node} };

			QueryNode child1 = QueryNode();
			child1.setSynonymNode({ QuerySynonymType::assign }, "a");
			QueryNode child2 = QueryNode();
			child2.setNodeType({ QueryNodeType::pattern });
			QueryNode child_child_child0 = QueryNode();
			child_child_child0.setSynonymNode({ QuerySynonymType::assign }, "a");
			QueryNode child_child_child1 = QueryNode();
			child_child_child1.setNodeType({ QueryNodeType::wild_card });
			QueryNode child_child_child2 = QueryNode();

			std::shared_ptr<VariableNode> woof = std::make_shared<VariableNode>();
			woof->setVariableName("woof");
			std::shared_ptr<ExpressionNode> ewoof = std::make_shared<ExpressionNode>();
			ewoof->setExpressionType({ ExpressionTypeEnum::none });
			ewoof->setLeftAstNode(woof);

			child_child_child2.setASTNode(ewoof);
			child_child_child2.setNodeType({ QueryNodeType::partial_expression });

			QueryNode child2_children[] = { child_child_child0, child_child_child1, child_child_child2 };
			child2.setChildren(child2_children, 3);

			QueryNode root = QueryNode();
			root.setNodeType({ QueryNodeType::select });
			QueryNode tuple = QueryNode();
			tuple.setNodeType({ QueryNodeType::tuple });
			QueryNode tuple_children[] = { child1 };
			tuple.setChildren(tuple_children, 1);
			QueryNode root_children[] = { tuple, child2 };
			root.setChildren(root_children, 2);

			QueryNode processed_clauses = root; //stores root node of the tree

			// Evaluate
			QueryEvaluator qe = QueryEvaluator(*pkb2);
			QUERY_RESULT result = qe.evaluateQuery(processed_synonyms, processed_clauses);
			STRING_RESULT result_string = ResultListManager::getStringValues(result);
			STRING_RESULT correct_result = "1";

			Logger::WriteMessage("Result: ");
			Logger::WriteMessage(result_string.c_str());
			Assert::IsTrue(result_string.compare(correct_result) == 0);
		}

		TEST_METHOD(evaluateQuery_SelectBOOLEAN_PatternWildCardWildCard_ReturnsTrue)
		{
			// Query: assign a; Select BOOLEAN pattern a(_, _"woof"_)"
			// Get processed_synonyms and processed clauses
			QueryNode assign_node = QueryNode();
			assign_node.setSynonymNode({ QuerySynonymType::assign }, "a");
			std::unordered_map<std::string, QueryNode> processed_synonyms = { {"a", assign_node} };

			// Select: BOOLEAN
			QueryNode child1 = QueryNode();
			child1.setBooleanNode();

			// pattern
			QueryNode child2 = QueryNode();
			child2.setNodeType({ QueryNodeType::pattern });

			// pattern a
			QueryNode child_child_child0 = QueryNode();
			child_child_child0.setSynonymNode({ QuerySynonymType::assign }, "a");

			// arg1: _
			QueryNode child_child_child1 = QueryNode();
			child_child_child1.setNodeType({ QueryNodeType::wild_card });

			// arg2: _"woof"_
			QueryNode child_child_child2 = QueryNode();
			std::shared_ptr<VariableNode> woof = std::make_shared<VariableNode>();
			woof->setVariableName("woof");
			std::shared_ptr<ExpressionNode> ewoof = std::make_shared<ExpressionNode>();
			ewoof->setExpressionType({ ExpressionTypeEnum::none });
			ewoof->setLeftAstNode(woof);

			child_child_child2.setNodeType({ QueryNodeType::partial_expression });
			child_child_child2.setASTNode(ewoof);

			// Set pattern's children
			QueryNode child2_children[] = { child_child_child0, child_child_child1, child_child_child2 };
			child2.setChildren(child2_children, 3);

			// Set root node
			QueryNode root = QueryNode();
			root.setNodeType({ QueryNodeType::select });
			QueryNode root_children[] = { child1, child2 };
			root.setChildren(root_children, 2);

			QueryNode processed_clauses = root; //stores root node of the tree

			// Evaluate
			QueryEvaluator qe = QueryEvaluator(*pkb2);
			QUERY_RESULT result = qe.evaluateQuery(processed_synonyms, processed_clauses);
			STRING_RESULT result_string = ResultListManager::getStringValues(result);
			STRING_RESULT correct_result = "TRUE";

			Logger::WriteMessage("Result: ");
			Logger::WriteMessage(result_string.c_str());
			Assert::IsTrue(result_string.compare(correct_result) == 0);
		}
		TEST_METHOD(evaluateQuery_SelectA_PatternAVWildCard_Returns123)
		{
			// Query: "assign a; variable v; Select a pattern a(v, _)"
			// Get processed_synonyms and processed clauses
			QueryNode assign_node = QueryNode();
			assign_node.setSynonymNode({ QuerySynonymType::assign }, "a");
			QueryNode var_node = QueryNode();
			var_node.setSynonymNode({ QuerySynonymType::variable }, "v");
			std::unordered_map<std::string, QueryNode> processed_synonyms = { {"a", assign_node}, {"v", var_node} };

			QueryNode child1 = QueryNode();
			child1.setSynonymNode({ QuerySynonymType::assign }, "a");
			QueryNode child2 = QueryNode();
			child2.setNodeType({ QueryNodeType::pattern });
			QueryNode child_child_child0 = QueryNode();
			child_child_child0.setSynonymNode({ QuerySynonymType::assign }, "a");

			QueryNode child_child_child1 = QueryNode();
			child_child_child1.setSynonymNode({ QuerySynonymType::variable }, "v");

			QueryNode child_child_child2 = QueryNode();
			child_child_child2.setNodeType({ QueryNodeType::wild_card });

			QueryNode child2_children[] = { child_child_child0, child_child_child1, child_child_child2 };
			child2.setChildren(child2_children, 3);

			QueryNode root = QueryNode();
			root.setNodeType({ QueryNodeType::select });
			QueryNode tuple = QueryNode();
			tuple.setNodeType({ QueryNodeType::tuple });
			QueryNode tuple_children[] = { child1 };
			tuple.setChildren(tuple_children, 1);
			QueryNode root_children[] = { tuple, child2 };
			root.setChildren(root_children, 2);

			QueryNode processed_clauses = root; //stores root node of the tree

			// Evaluate
			QueryEvaluator qe = QueryEvaluator(*pkb2);
			QUERY_RESULT result = qe.evaluateQuery(processed_synonyms, processed_clauses);
			STRING_RESULT result_string = ResultListManager::getStringValues(result);
			STRING_RESULT correct_result = "1, 2, 3";

			Logger::WriteMessage("Result: ");
			Logger::WriteMessage(result_string.c_str());
			Assert::IsTrue(result_string.compare(correct_result) == 0);
		}

		TEST_METHOD(evaluateQuery_SelectA_PatternAVPartial_Returns23)
		{
			// Query: "assign a; variable v; Select a pattern a(v, _"b"_)"
			// Get processed_synonyms and processed clauses
			QueryNode assign_node = QueryNode();
			assign_node.setSynonymNode({ QuerySynonymType::assign }, "a");
			QueryNode var_node = QueryNode();
			var_node.setSynonymNode({ QuerySynonymType::variable }, "v");
			std::unordered_map<std::string, QueryNode> processed_synonyms = { {"a", assign_node}, {"v", var_node} };

			QueryNode child1 = QueryNode();
			child1.setSynonymNode({ QuerySynonymType::assign }, "a");
			QueryNode child2 = QueryNode();
			child2.setNodeType({ QueryNodeType::pattern });
			QueryNode child_child_child0 = QueryNode();
			child_child_child0.setSynonymNode({ QuerySynonymType::assign }, "a");

			QueryNode child_child_child1 = QueryNode();
			child_child_child1.setSynonymNode({ QuerySynonymType::variable }, "v");

			QueryNode child_child_child2 = QueryNode();
			std::shared_ptr<VariableNode> b = std::make_shared<VariableNode>();
			b->setVariableName("b");
			std::shared_ptr<ExpressionNode> eb = std::make_shared<ExpressionNode>();
			eb->setExpressionType({ ExpressionTypeEnum::none });
			eb->setLeftAstNode(b);

			child_child_child2.setASTNode(eb);
			child_child_child2.setNodeType({ QueryNodeType::partial_expression });

			QueryNode child2_children[] = { child_child_child0, child_child_child1, child_child_child2 };
			child2.setChildren(child2_children, 3);

			QueryNode root = QueryNode();
			root.setNodeType({ QueryNodeType::select });
			QueryNode tuple = QueryNode();
			tuple.setNodeType({ QueryNodeType::tuple });
			QueryNode tuple_children[] = { child1 };
			tuple.setChildren(tuple_children, 1);
			QueryNode root_children[] = { tuple, child2 };
			root.setChildren(root_children, 2);

			QueryNode processed_clauses = root; //stores root node of the tree

			// Evaluate
			QueryEvaluator qe = QueryEvaluator(*pkb2);
			QUERY_RESULT result = qe.evaluateQuery(processed_synonyms, processed_clauses);
			STRING_RESULT result_string = ResultListManager::getStringValues(result);
			STRING_RESULT correct_result = "2, 3";

			Logger::WriteMessage("Result: ");
			Logger::WriteMessage(result_string.c_str());
			Assert::IsTrue(result_string.compare(correct_result) == 0);
		}

		TEST_METHOD(evaluateQuery_SelectA_PatternAIdentWildCard_Returns1)
		{
			// Query: "assign a; Select a pattern a("a", _)"
			// Get processed_synonyms and processed clauses
			QueryNode assign_node = QueryNode();
			assign_node.setSynonymNode({ QuerySynonymType::assign }, "a");
			std::unordered_map<std::string, QueryNode> processed_synonyms = { {"a", assign_node} };

			QueryNode child1 = QueryNode();
			child1.setSynonymNode({ QuerySynonymType::assign }, "a");
			QueryNode child2 = QueryNode();
			child2.setNodeType({ QueryNodeType::pattern });
			QueryNode child_child_child0 = QueryNode();
			child_child_child0.setSynonymNode({ QuerySynonymType::assign }, "a");

			QueryNode child_child_child1 = QueryNode();
			child_child_child1.setIdentityNode("a");

			QueryNode child_child_child2 = QueryNode();
			child_child_child2.setNodeType({ QueryNodeType::wild_card });

			QueryNode child2_children[] = { child_child_child0, child_child_child1, child_child_child2 };
			child2.setChildren(child2_children, 3);

			QueryNode root = QueryNode();
			root.setNodeType({ QueryNodeType::select });
			QueryNode tuple = QueryNode();
			tuple.setNodeType({ QueryNodeType::tuple });
			QueryNode tuple_children[] = { child1 };
			tuple.setChildren(tuple_children, 1);
			QueryNode root_children[] = { tuple, child2 };
			root.setChildren(root_children, 2);

			QueryNode processed_clauses = root; //stores root node of the tree

			// Evaluate
			QueryEvaluator qe = QueryEvaluator(*pkb2);
			QUERY_RESULT result = qe.evaluateQuery(processed_synonyms, processed_clauses);
			STRING_RESULT result_string = ResultListManager::getStringValues(result);
			STRING_RESULT correct_result = "1";

			Logger::WriteMessage("Result: ");
			Logger::WriteMessage(result_string.c_str());
			Assert::IsTrue(result_string.compare(correct_result) == 0);
		}

		TEST_METHOD(evaluateQuery_SelectA_PatternAIdentPartial_Returns3)
		{
			// Query: "assign a; Select a pattern a("c", _"2"_)"
			// Get processed_synonyms and processed clauses
			QueryNode assign_node = QueryNode();
			assign_node.setSynonymNode({ QuerySynonymType::assign }, "a");
			std::unordered_map<std::string, QueryNode> processed_synonyms = { {"a", assign_node} };

			QueryNode child1 = QueryNode();
			child1.setSynonymNode({ QuerySynonymType::assign }, "a");
			QueryNode child2 = QueryNode();
			child2.setNodeType({ QueryNodeType::pattern });
			QueryNode child_child_child0 = QueryNode();
			child_child_child0.setSynonymNode({ QuerySynonymType::assign }, "a");

			QueryNode child_child_child1 = QueryNode();
			child_child_child1.setIdentityNode("c");

			QueryNode child_child_child2 = QueryNode();
			std::shared_ptr<ConstantNode> const_node = std::make_shared<ConstantNode>();
			const_node->setValue("2");
			std::shared_ptr<ExpressionNode> expr_node = std::make_shared<ExpressionNode>();
			expr_node->setExpressionType({ ExpressionTypeEnum::none });
			expr_node->setLeftAstNode(const_node);

			child_child_child2.setASTNode(expr_node);
			child_child_child2.setNodeType({ QueryNodeType::partial_expression });

			QueryNode child2_children[] = { child_child_child0, child_child_child1, child_child_child2 };
			child2.setChildren(child2_children, 3);

			QueryNode root = QueryNode();
			root.setNodeType({ QueryNodeType::select });
			QueryNode tuple = QueryNode();
			tuple.setNodeType({ QueryNodeType::tuple });
			QueryNode tuple_children[] = { child1 };
			tuple.setChildren(tuple_children, 1);
			QueryNode root_children[] = { tuple, child2 };
			root.setChildren(root_children, 2);

			QueryNode processed_clauses = root; //stores root node of the tree

			// Evaluate
			QueryEvaluator qe = QueryEvaluator(*pkb2);
			QUERY_RESULT result = qe.evaluateQuery(processed_synonyms, processed_clauses);
			STRING_RESULT result_string = ResultListManager::getStringValues(result);
			STRING_RESULT correct_result = "3";

			Logger::WriteMessage("Result: ");
			Logger::WriteMessage(result_string.c_str());
			Assert::IsTrue(result_string.compare(correct_result) == 0);
		}

		TEST_METHOD(evaluateQuery_SelectV_PatternAIdentPartial_Returns3)
		{
			// Query: "variable v; assign a; Select v pattern a("c", _"2"_)"
			// Get processed_synonyms and processed clauses
			QueryNode assign_node = QueryNode();
			assign_node.setSynonymNode({ QuerySynonymType::assign }, "a");
			QueryNode var_node = QueryNode();
			var_node.setSynonymNode({ QuerySynonymType::variable }, "v");
			std::unordered_map<std::string, QueryNode> processed_synonyms = { {"a", assign_node}, {"v", var_node} };

			QueryNode child1 = QueryNode();
			child1.setSynonymNode({ QuerySynonymType::variable }, "v");
			QueryNode child2 = QueryNode();
			child2.setNodeType({ QueryNodeType::pattern });
			QueryNode child_child_child0 = QueryNode();
			child_child_child0.setSynonymNode({ QuerySynonymType::assign }, "a");

			QueryNode child_child_child1 = QueryNode();
			child_child_child1.setIdentityNode("c");

			QueryNode child_child_child2 = QueryNode();
			std::shared_ptr<ConstantNode> const_node = std::make_shared<ConstantNode>();
			const_node->setValue("2");
			std::shared_ptr<ExpressionNode> expr_node = std::make_shared<ExpressionNode>();
			expr_node->setExpressionType({ ExpressionTypeEnum::none });
			expr_node->setLeftAstNode(const_node);

			child_child_child2.setASTNode(expr_node);
			child_child_child2.setNodeType({ QueryNodeType::partial_expression });

			QueryNode child2_children[] = { child_child_child0, child_child_child1, child_child_child2 };
			child2.setChildren(child2_children, 3);

			QueryNode root = QueryNode();
			root.setNodeType({ QueryNodeType::select });
			QueryNode tuple = QueryNode();
			tuple.setNodeType({ QueryNodeType::tuple });
			QueryNode tuple_children[] = { child1 };
			tuple.setChildren(tuple_children, 1);
			QueryNode root_children[] = { tuple, child2 };
			root.setChildren(root_children, 2);

			QueryNode processed_clauses = root; //stores root node of the tree

			// Evaluate
			QueryEvaluator qe = QueryEvaluator(*pkb2);
			QUERY_RESULT result = qe.evaluateQuery(processed_synonyms, processed_clauses);
			STRING_RESULT result_string = ResultListManager::getStringValues(result);
			STRING_RESULT correct_result = "a, g, o, woof, b, c, d";

			Logger::WriteMessage("Result: ");
			Logger::WriteMessage(result_string.c_str());
			Assert::IsTrue(result_string.compare(correct_result) == 0);
		}

		TEST_METHOD(evaluateQuery_SelectBOOLEAN_PatternAIdentPartial_ReturnsTrue)
		{
			// Query: "assign a; Select BOOLEAN pattern a("c", _"2"_)"
			// Get processed_synonyms and processed clauses
			QueryNode assign_node = QueryNode();
			assign_node.setSynonymNode({ QuerySynonymType::assign }, "a");
			std::unordered_map<std::string, QueryNode> processed_synonyms = { {"a", assign_node} };

			QueryNode child1 = QueryNode();
			child1.setBooleanNode();

			QueryNode child2 = QueryNode();
			child2.setNodeType({ QueryNodeType::pattern });

			QueryNode child_child_child0 = QueryNode();
			child_child_child0.setSynonymNode({ QuerySynonymType::assign }, "a");

			QueryNode child_child_child1 = QueryNode();
			child_child_child1.setIdentityNode("c");

			QueryNode child_child_child2 = QueryNode();
			std::shared_ptr<ConstantNode> const_node = std::make_shared<ConstantNode>();
			const_node->setValue("2");
			std::shared_ptr<ExpressionNode> expr_node = std::make_shared<ExpressionNode>();
			expr_node->setExpressionType({ ExpressionTypeEnum::none });
			expr_node->setLeftAstNode(const_node);

			child_child_child2.setASTNode(expr_node);
			child_child_child2.setNodeType({ QueryNodeType::partial_expression });

			QueryNode child2_children[] = { child_child_child0, child_child_child1, child_child_child2 };
			child2.setChildren(child2_children, 3);

			QueryNode root = QueryNode();
			root.setNodeType({ QueryNodeType::select });
			QueryNode root_children[] = { child1, child2 };
			root.setChildren(root_children, 2);

			QueryNode processed_clauses = root; //stores root node of the tree

			// Evaluate
			QueryEvaluator qe = QueryEvaluator(*pkb2);
			QUERY_RESULT result = qe.evaluateQuery(processed_synonyms, processed_clauses);
			STRING_RESULT result_string = ResultListManager::getStringValues(result);
			STRING_RESULT correct_result = "TRUE";

			Logger::WriteMessage("Result: ");
			Logger::WriteMessage(result_string.c_str());
			Assert::IsTrue(result_string.compare(correct_result) == 0);
		}

		TEST_METHOD(evaluateQuery_SelectA_PatternAIdentPartial_ReturnEmpty)
		{
			// Query: "assign a; Select a pattern a("c", _"10"_)"
			// Get processed_synonyms and processed clauses
			QueryNode assign_node = QueryNode();
			assign_node.setSynonymNode({ QuerySynonymType::assign }, "a");
			std::unordered_map<std::string, QueryNode> processed_synonyms = { {"a", assign_node} };

			QueryNode child1 = QueryNode();
			child1.setSynonymNode({ QuerySynonymType::assign }, "a");
			QueryNode child2 = QueryNode();
			child2.setNodeType({ QueryNodeType::pattern });
			QueryNode child_child_child0 = QueryNode();
			child_child_child0.setSynonymNode({ QuerySynonymType::assign }, "a");

			QueryNode child_child_child1 = QueryNode();
			child_child_child1.setIdentityNode("c");

			QueryNode child_child_child2 = QueryNode();
			std::shared_ptr<ConstantNode> const_node = std::make_shared<ConstantNode>();
			const_node->setValue("10");
			std::shared_ptr<ExpressionNode> expr_node = std::make_shared<ExpressionNode>();
			expr_node->setExpressionType({ ExpressionTypeEnum::none });
			expr_node->setLeftAstNode(const_node);

			child_child_child2.setNodeType({ QueryNodeType::partial_expression });
			child_child_child2.setASTNode(expr_node);

			QueryNode child2_children[] = { child_child_child0, child_child_child1, child_child_child2 };
			child2.setChildren(child2_children, 3);

			QueryNode root = QueryNode();
			root.setNodeType({ QueryNodeType::select });
			QueryNode tuple = QueryNode();
			tuple.setNodeType({ QueryNodeType::tuple });
			QueryNode tuple_children[] = { child1 };
			tuple.setChildren(tuple_children, 1);
			QueryNode root_children[] = { tuple, child2 };
			root.setChildren(root_children, 2);

			QueryNode processed_clauses = root; //stores root node of the tree

			// Evaluate
			QueryEvaluator qe = QueryEvaluator(*pkb2);
			QUERY_RESULT result = qe.evaluateQuery(processed_synonyms, processed_clauses);
			STRING_RESULT result_string = ResultListManager::getStringValues(result);
			STRING_RESULT correct_result = "";

			Logger::WriteMessage("Result: ");
			Logger::WriteMessage(result_string.c_str());
			Assert::IsTrue(result_string.compare(correct_result) == 0);
		}

		TEST_METHOD(evaluateQuery_SelectBOOLEAN_PatternAIdentPartial_ReturnsFalse)
		{
			// Query: "assign a; Select BOOLEAN pattern a("c", _"10"_)"
			// Get processed_synonyms and processed clauses
			QueryNode assign_node = QueryNode();
			assign_node.setSynonymNode({ QuerySynonymType::assign }, "a");
			std::unordered_map<std::string, QueryNode> processed_synonyms = { {"a", assign_node} };

			// Select: BOOLEAN
			QueryNode child1 = QueryNode();
			child1.setBooleanNode();

			// pattern
			QueryNode child2 = QueryNode();
			child2.setNodeType({ QueryNodeType::pattern });

			// pattern a
			QueryNode child_child_child0 = QueryNode();
			child_child_child0.setSynonymNode({ QuerySynonymType::assign }, "a");

			// arg1: "c"
			QueryNode child_child_child1 = QueryNode();
			child_child_child1.setIdentityNode("c");

			// arg2: _"10"_
			QueryNode child_child_child2 = QueryNode();
			std::shared_ptr<ConstantNode> const_node = std::make_shared<ConstantNode>();
			const_node->setValue("10");
			std::shared_ptr<ExpressionNode> expr_node = std::make_shared<ExpressionNode>();
			expr_node->setExpressionType({ ExpressionTypeEnum::none });
			expr_node->setLeftAstNode(const_node);

			child_child_child2.setNodeType({ QueryNodeType::partial_expression });
			child_child_child2.setASTNode(expr_node);

			// Set pattern's children
			QueryNode child2_children[] = { child_child_child0, child_child_child1, child_child_child2 };
			child2.setChildren(child2_children, 3);

			// Set root node
			QueryNode root = QueryNode();
			root.setNodeType({ QueryNodeType::select });
			QueryNode root_children[] = { child1, child2 };
			root.setChildren(root_children, 2);

			QueryNode processed_clauses = root; //stores root node of the tree

			// Evaluate
			QueryEvaluator qe = QueryEvaluator(*pkb2);
			QUERY_RESULT result = qe.evaluateQuery(processed_synonyms, processed_clauses);
			STRING_RESULT result_string = ResultListManager::getStringValues(result);
			STRING_RESULT correct_result = "FALSE";

			Logger::WriteMessage("Result: ");
			Logger::WriteMessage(result_string.c_str());
			Assert::IsTrue(result_string.compare(correct_result) == 0);
		}

		TEST_METHOD(evaluateQuery_SelectA_PatternAWildCardPartial_Returns3)
		{
			// Query: "assign a; Select a pattern a (_, _"b + 3"_)"
			// Get processed_synonyms and processed clauses
			QueryNode assign_node = QueryNode();
			assign_node.setSynonymNode({ QuerySynonymType::assign }, "a");
			std::unordered_map<std::string, QueryNode> processed_synonyms = { {"a", assign_node} };

			QueryNode child1 = QueryNode();
			child1.setSynonymNode({ QuerySynonymType::assign }, "a");
			QueryNode child2 = QueryNode();
			child2.setNodeType({ QueryNodeType::pattern });
			QueryNode child_child_child0 = QueryNode();
			child_child_child0.setSynonymNode({ QuerySynonymType::assign }, "a");

			QueryNode child_child_child1 = QueryNode();
			child_child_child1.setNodeType({ QueryNodeType::wild_card });

			QueryNode child_child_child2 = QueryNode();
			std::shared_ptr<VariableNode> left_node = std::make_shared<VariableNode>();
			left_node->setVariableName("b");
			std::shared_ptr<ConstantNode> right_node = std::make_shared<ConstantNode>();
			right_node->setValue("3");
			std::shared_ptr<ExpressionNode> expr_node = std::make_shared<ExpressionNode>();
			expr_node->setExpressionType({ ExpressionTypeEnum::plus });
			expr_node->setLeftAstNode(left_node);
			expr_node->setRightAstNode(right_node);

			child_child_child2.setASTNode(expr_node);
			child_child_child2.setNodeType({ QueryNodeType::partial_expression });

			QueryNode child2_children[] = { child_child_child0, child_child_child1, child_child_child2 };
			child2.setChildren(child2_children, 3);

			QueryNode root = QueryNode();
			root.setNodeType({ QueryNodeType::select });
			QueryNode tuple = QueryNode();
			tuple.setNodeType({ QueryNodeType::tuple });
			QueryNode tuple_children[] = { child1 };
			tuple.setChildren(tuple_children, 1);
			QueryNode root_children[] = { tuple, child2 };
			root.setChildren(root_children, 2);

			QueryNode processed_clauses = root; //stores root node of the tree

			// Evaluate
			QueryEvaluator qe = QueryEvaluator(*pkb2);
			QUERY_RESULT result = qe.evaluateQuery(processed_synonyms, processed_clauses);
			STRING_RESULT result_string = ResultListManager::getStringValues(result);
			Logger::WriteMessage("Result: ");
			Logger::WriteMessage(result_string.c_str());

			std::unordered_set<CONSTANT_VALUE> set_result(result.begin(), result.end());
			std::unordered_set<CONSTANT_VALUE> correct_result = { "3" };
			Assert::IsTrue(set_result == correct_result);
		}

		TEST_METHOD(evaluateQuery_SelectA_PatternAWildCardExact_ReturnsEmpty)
		{
			// Query: "assign a; Select a pattern a (_, "b + 3")"
			// Get processed_synonyms and processed clauses
			QueryNode assign_node = QueryNode();
			assign_node.setSynonymNode({ QuerySynonymType::assign }, "a");
			std::unordered_map<std::string, QueryNode> processed_synonyms = { {"a", assign_node} };

			QueryNode child1 = QueryNode();
			child1.setSynonymNode({ QuerySynonymType::assign }, "a");
			QueryNode child2 = QueryNode();
			child2.setNodeType({ QueryNodeType::pattern });
			QueryNode child_child_child0 = QueryNode();
			child_child_child0.setSynonymNode({ QuerySynonymType::assign }, "a");

			QueryNode child_child_child1 = QueryNode();
			child_child_child1.setNodeType({ QueryNodeType::wild_card });

			QueryNode child_child_child2 = QueryNode();
			std::shared_ptr<VariableNode> left_node = std::make_shared<VariableNode>();
			left_node->setVariableName("b");
			std::shared_ptr<ConstantNode> right_node = std::make_shared<ConstantNode>();
			right_node->setValue("3");
			std::shared_ptr<ExpressionNode> expr_node = std::make_shared<ExpressionNode>();
			expr_node->setExpressionType({ ExpressionTypeEnum::plus });
			expr_node->setLeftAstNode(left_node);
			expr_node->setRightAstNode(right_node);

			child_child_child2.setASTNode(expr_node);
			child_child_child2.setNodeType({ QueryNodeType::expression });

			QueryNode child2_children[] = { child_child_child0, child_child_child1, child_child_child2 };
			child2.setChildren(child2_children, 3);

			QueryNode root = QueryNode();
			root.setNodeType({ QueryNodeType::select });
			QueryNode tuple = QueryNode();
			tuple.setNodeType({ QueryNodeType::tuple });
			QueryNode tuple_children[] = { child1 };
			tuple.setChildren(tuple_children, 1);
			QueryNode root_children[] = { tuple, child2 };
			root.setChildren(root_children, 2);

			QueryNode processed_clauses = root; //stores root node of the tree

			// Evaluate
			QueryEvaluator qe = QueryEvaluator(*pkb2);
			QUERY_RESULT result = qe.evaluateQuery(processed_synonyms, processed_clauses);
			STRING_RESULT result_string = ResultListManager::getStringValues(result);
			Logger::WriteMessage("Result: ");
			Logger::WriteMessage(result_string.c_str());

			std::unordered_set<CONSTANT_VALUE> set_result(result.begin(), result.end());
			std::unordered_set<CONSTANT_VALUE> correct_result = {};
			Assert::IsTrue(set_result == correct_result);
		}

		TEST_METHOD(evaluateQuery_SelectA_PatternAWildCardExact_Returns3)
		{
			// Query: "assign a; Select a pattern a (_, "2 / (b + 3) % d + o * g")"
			// Get processed_synonyms and processed clauses
			QueryNode assign_node = QueryNode();
			assign_node.setSynonymNode({ QuerySynonymType::assign }, "a");
			std::unordered_map<std::string, QueryNode> processed_synonyms = { {"a", assign_node} };

			QueryNode child1 = QueryNode();
			child1.setSynonymNode({ QuerySynonymType::assign }, "a");
			QueryNode child2 = QueryNode();
			child2.setNodeType({ QueryNodeType::pattern });
			QueryNode child_child_child0 = QueryNode();
			child_child_child0.setSynonymNode({ QuerySynonymType::assign }, "a");

			QueryNode child_child_child1 = QueryNode();
			child_child_child1.setNodeType({ QueryNodeType::wild_card });

			QueryNode child_child_child2 = QueryNode();
			std::shared_ptr<VariableNode> v1 = std::make_shared<VariableNode>();
			std::shared_ptr<VariableNode> v2 = std::make_shared<VariableNode>();
			std::shared_ptr<VariableNode> v3 = std::make_shared<VariableNode>();
			std::shared_ptr<VariableNode> v4 = std::make_shared<VariableNode>();
			v1->setVariableName("b");
			v2->setVariableName("d");
			v3->setVariableName("o");
			v4->setVariableName("g");

			std::shared_ptr<ConstantNode> c1 = std::make_shared<ConstantNode>();
			std::shared_ptr<ConstantNode> c2 = std::make_shared<ConstantNode>();
			c1->setValue("2");
			c2->setValue("3");

			std::shared_ptr<ExpressionNode> e1 = std::make_shared<ExpressionNode>();
			std::shared_ptr<ExpressionNode> e2 = std::make_shared<ExpressionNode>();
			std::shared_ptr<ExpressionNode> e3 = std::make_shared<ExpressionNode>();
			std::shared_ptr<ExpressionNode> e4 = std::make_shared<ExpressionNode>();
			std::shared_ptr<ExpressionNode> e5 = std::make_shared<ExpressionNode>();
			// o * g
			e1->setExpressionType(ExpressionTypeEnum::times);
			e1->setLeftAstNode(v3);
			e1->setRightAstNode(v4);
			// b + 3
			e2->setExpressionType(ExpressionTypeEnum::plus);
			e2->setLeftAstNode(v1);
			e2->setRightAstNode(c2);
			// 2 / e2
			e3->setExpressionType(ExpressionTypeEnum::div);
			e3->setLeftAstNode(c1);
			e3->setRightAstNode(e2);
			// e3 % d
			e4->setExpressionType(ExpressionTypeEnum::mod);
			e4->setLeftAstNode(e3);
			e4->setRightAstNode(v2);
			// e4 + e1
			e5->setExpressionType(ExpressionTypeEnum::plus);
			e5->setLeftAstNode(e4);
			e5->setRightAstNode(e1);

			child_child_child2.setASTNode(e5);
			child_child_child2.setNodeType({ QueryNodeType::expression });

			QueryNode child2_children[] = { child_child_child0, child_child_child1, child_child_child2 };
			child2.setChildren(child2_children, 3);

			QueryNode root = QueryNode();
			root.setNodeType({ QueryNodeType::select });
			QueryNode tuple = QueryNode();
			tuple.setNodeType({ QueryNodeType::tuple });
			QueryNode tuple_children[] = { child1 };
			tuple.setChildren(tuple_children, 1);
			QueryNode root_children[] = { tuple, child2 };
			root.setChildren(root_children, 2);

			QueryNode processed_clauses = root; //stores root node of the tree

			// Evaluate
			QueryEvaluator qe = QueryEvaluator(*pkb2);
			QUERY_RESULT result = qe.evaluateQuery(processed_synonyms, processed_clauses);
			STRING_RESULT result_string = ResultListManager::getStringValues(result);
			Logger::WriteMessage("Result: ");
			Logger::WriteMessage(result_string.c_str());

			std::unordered_set<CONSTANT_VALUE> set_result(result.begin(), result.end());
			std::unordered_set<CONSTANT_VALUE> correct_result = { "3" };
			Assert::IsTrue(set_result == correct_result);
		}

		/*=================================  PKB 3: Pattern =========================================*/
		TEST_METHOD(evaluateQuery_SelectI_PatternIIdent_Returns4)
		{
			// Query: "if i; Select i pattern i ("g", _)"
			// Get processed_synonyms and processed clauses
			QueryNode if_node;
			if_node.setSynonymNode({ QuerySynonymType::ifs }, "i");
			std::unordered_map<std::string, QueryNode> processed_synonyms = { {"i", if_node} };

			QueryNode child1 = QueryNode();
			child1.setSynonymNode({ QuerySynonymType::ifs }, "i");
			QueryNode child2 = QueryNode();
			child2.setNodeType({ QueryNodeType::pattern });
			QueryNode child_child_child0 = QueryNode();
			child_child_child0.setSynonymNode({ QuerySynonymType::ifs }, "i");

			QueryNode child_child_child1 = QueryNode();
			child_child_child1.setIdentityNode("g");

			QueryNode child2_children[] = { child_child_child0, child_child_child1 };
			child2.setChildren(child2_children, 2);

			QueryNode root = QueryNode();
			root.setNodeType({ QueryNodeType::select });
			QueryNode tuple = QueryNode();
			tuple.setNodeType({ QueryNodeType::tuple });
			QueryNode tuple_children[] = { child1 };
			tuple.setChildren(tuple_children, 1);
			QueryNode root_children[] = { tuple, child2 };
			root.setChildren(root_children, 2);

			QueryNode processed_clauses = root; //stores root node of the tree

			// Evaluate
			QueryEvaluator qe = QueryEvaluator(*pkb3);
			QUERY_RESULT result = qe.evaluateQuery(processed_synonyms, processed_clauses);
			STRING_RESULT result_string = ResultListManager::getStringValues(result);
			Logger::WriteMessage("Result: ");
			Logger::WriteMessage(result_string.c_str());

			std::unordered_set<CONSTANT_VALUE> set_result(result.begin(), result.end());
			std::unordered_set<CONSTANT_VALUE> correct_result = { "4" };
			Assert::IsTrue(set_result == correct_result);
		}

		TEST_METHOD(evaluateQuery_SelectW_PatternWIdent_Returns2)
		{
			// Query: "while w; Select w pattern w ("a", _)"
			// Get processed_synonyms and processed clauses
			QueryNode while_node;
			while_node.setSynonymNode({ QuerySynonymType::whiles }, "w");
			std::unordered_map<std::string, QueryNode> processed_synonyms = { {"w", while_node} };

			QueryNode child1 = QueryNode();
			child1.setSynonymNode({ QuerySynonymType::whiles }, "w");
			QueryNode child2 = QueryNode();
			child2.setNodeType({ QueryNodeType::pattern });
			QueryNode child_child_child0 = QueryNode();
			child_child_child0.setSynonymNode({ QuerySynonymType::whiles }, "w");

			QueryNode child_child_child1 = QueryNode();
			child_child_child1.setIdentityNode("a");

			QueryNode child2_children[] = { child_child_child0, child_child_child1 };
			child2.setChildren(child2_children, 2);

			QueryNode root = QueryNode();
			root.setNodeType({ QueryNodeType::select });
			QueryNode tuple = QueryNode();
			tuple.setNodeType({ QueryNodeType::tuple });
			QueryNode tuple_children[] = { child1 };
			tuple.setChildren(tuple_children, 1);
			QueryNode root_children[] = { tuple, child2 };
			root.setChildren(root_children, 2);

			QueryNode processed_clauses = root; //stores root node of the tree

			// Evaluate
			QueryEvaluator qe = QueryEvaluator(*pkb3);
			QUERY_RESULT result = qe.evaluateQuery(processed_synonyms, processed_clauses);
			STRING_RESULT result_string = ResultListManager::getStringValues(result);
			Logger::WriteMessage("Result: ");
			Logger::WriteMessage(result_string.c_str());

			std::unordered_set<CONSTANT_VALUE> set_result(result.begin(), result.end());
			std::unordered_set<CONSTANT_VALUE> correct_result = { "2" };
			Assert::IsTrue(set_result == correct_result);
		}

		TEST_METHOD(evaluateQuery_SelectI_PatternIWildCard_Returns34)
		{
			// Query: "if i; Select i pattern i (_, _)"
			// Get processed_synonyms and processed clauses
			QueryNode if_node;
			if_node.setSynonymNode({ QuerySynonymType::ifs }, "i");
			std::unordered_map<std::string, QueryNode> processed_synonyms = { {"i", if_node} };

			QueryNode child1 = QueryNode();
			child1.setSynonymNode({ QuerySynonymType::ifs }, "i");
			QueryNode child2 = QueryNode();
			child2.setNodeType({ QueryNodeType::pattern });
			QueryNode child_child_child0 = QueryNode();
			child_child_child0.setSynonymNode({ QuerySynonymType::ifs }, "i");

			QueryNode child_child_child1 = QueryNode();
			child_child_child1.setNodeType(QueryNodeType::wild_card);

			QueryNode child2_children[] = { child_child_child0, child_child_child1 };
			child2.setChildren(child2_children, 2);

			QueryNode root = QueryNode();
			root.setNodeType({ QueryNodeType::select });
			QueryNode tuple = QueryNode();
			tuple.setNodeType({ QueryNodeType::tuple });
			QueryNode tuple_children[] = { child1 };
			tuple.setChildren(tuple_children, 1);
			QueryNode root_children[] = { tuple, child2 };
			root.setChildren(root_children, 2);

			QueryNode processed_clauses = root; //stores root node of the tree

			// Evaluate
			QueryEvaluator qe = QueryEvaluator(*pkb3);
			QUERY_RESULT result = qe.evaluateQuery(processed_synonyms, processed_clauses);
			STRING_RESULT result_string = ResultListManager::getStringValues(result);
			Logger::WriteMessage("Result: ");
			Logger::WriteMessage(result_string.c_str());

			std::unordered_set<CONSTANT_VALUE> set_result(result.begin(), result.end());
			std::unordered_set<CONSTANT_VALUE> correct_result = { "3", "4" };
			Assert::IsTrue(set_result == correct_result);
		}

		TEST_METHOD(evaluateQuery_SelectW_PatternWWildCard_Returns12)
		{
			// Query: "while w; Select w pattern w (_, _)"
			// Get processed_synonyms and processed clauses
			QueryNode while_node;
			while_node.setSynonymNode({ QuerySynonymType::whiles }, "w");
			std::unordered_map<std::string, QueryNode> processed_synonyms = { {"w", while_node} };

			QueryNode child1 = QueryNode();
			child1.setSynonymNode({ QuerySynonymType::whiles }, "w");
			QueryNode child2 = QueryNode();
			child2.setNodeType({ QueryNodeType::pattern });
			QueryNode child_child_child0 = QueryNode();
			child_child_child0.setSynonymNode({ QuerySynonymType::whiles }, "w");

			QueryNode child_child_child1 = QueryNode();
			child_child_child1.setNodeType(QueryNodeType::wild_card);

			QueryNode child2_children[] = { child_child_child0, child_child_child1 };
			child2.setChildren(child2_children, 2);

			QueryNode root = QueryNode();
			root.setNodeType({ QueryNodeType::select });
			QueryNode tuple = QueryNode();
			tuple.setNodeType({ QueryNodeType::tuple });
			QueryNode tuple_children[] = { child1 };
			tuple.setChildren(tuple_children, 1);
			QueryNode root_children[] = { tuple, child2 };
			root.setChildren(root_children, 2);

			QueryNode processed_clauses = root; //stores root node of the tree

			// Evaluate
			QueryEvaluator qe = QueryEvaluator(*pkb3);
			QUERY_RESULT result = qe.evaluateQuery(processed_synonyms, processed_clauses);
			STRING_RESULT result_string = ResultListManager::getStringValues(result);
			Logger::WriteMessage("Result: ");
			Logger::WriteMessage(result_string.c_str());

			std::unordered_set<CONSTANT_VALUE> set_result(result.begin(), result.end());
			std::unordered_set<CONSTANT_VALUE> correct_result = { "1", "2" };
			Assert::IsTrue(set_result == correct_result);
		}

		TEST_METHOD(evaluateQuery_SelectW_stmtnum_PatternWWildCard_Returns12)
		{
			// Query: "while w; Select w.stmt# pattern w (_, _)"
			// Get processed_synonyms and processed clauses
			QueryNode while_node;
			while_node.setSynonymNode({ QuerySynonymType::whiles }, "w");
			std::unordered_map<std::string, QueryNode> processed_synonyms = { {"w", while_node} };

			QueryNode child1 = QueryNode();
			child1.setAttrNode("w", "stmt#");
			QueryNode child2 = QueryNode();
			child2.setNodeType({ QueryNodeType::pattern });
			QueryNode child_child_child0 = QueryNode();
			child_child_child0.setSynonymNode({ QuerySynonymType::whiles }, "w");

			QueryNode child_child_child1 = QueryNode();
			child_child_child1.setNodeType(QueryNodeType::wild_card);

			QueryNode child2_children[] = { child_child_child0, child_child_child1 };
			child2.setChildren(child2_children, 2);

			QueryNode root = QueryNode();
			root.setNodeType({ QueryNodeType::select });
			QueryNode tuple = QueryNode();
			tuple.setNodeType({ QueryNodeType::tuple });
			QueryNode tuple_children[] = { child1 };
			tuple.setChildren(tuple_children, 1);
			QueryNode root_children[] = { tuple, child2 };
			root.setChildren(root_children, 2);

			QueryNode processed_clauses = root; //stores root node of the tree

			// Evaluate
			QueryEvaluator qe = QueryEvaluator(*pkb3);
			QUERY_RESULT result = qe.evaluateQuery(processed_synonyms, processed_clauses);
			STRING_RESULT result_string = ResultListManager::getStringValues(result);
			Logger::WriteMessage("Result: ");
			Logger::WriteMessage(result_string.c_str());

			std::unordered_set<CONSTANT_VALUE> set_result(result.begin(), result.end());
			std::unordered_set<CONSTANT_VALUE> correct_result = { "1", "2" };
			Assert::IsTrue(set_result == correct_result);
		}

		TEST_METHOD(evaluateQuery_SelectV_PatternAVWildCard_ReturnsABC)
		{
			// Query: "assign a; variable v; Select v pattern a (v, _)"
			// Get processed_synonyms and processed clauses
			QueryNode assign_node;
			assign_node.setSynonymNode({ QuerySynonymType::assign }, "a");
			QueryNode variable_node;
			variable_node.setSynonymNode({ QuerySynonymType::variable }, "v");
			std::unordered_map<std::string, QueryNode> processed_synonyms = { {"a", assign_node}, {"v", variable_node} };

			QueryNode child1 = QueryNode();
			child1.setSynonymNode({ QuerySynonymType::variable }, "v");
			QueryNode child2 = QueryNode();
			child2.setNodeType({ QueryNodeType::pattern });
			QueryNode child_child_child0 = QueryNode();
			child_child_child0.setSynonymNode({ QuerySynonymType::assign }, "a");

			QueryNode child_child_child1 = QueryNode();
			child_child_child1.setSynonymNode({ QuerySynonymType::variable }, "v");

			QueryNode child_child_child2 = QueryNode();
			child_child_child2.setNodeType({ QueryNodeType::wild_card });

			QueryNode child2_children[] = { child_child_child0, child_child_child1, child_child_child2 };
			child2.setChildren(child2_children, 3);

			QueryNode root = QueryNode();
			root.setNodeType({ QueryNodeType::select });
			QueryNode tuple = QueryNode();
			tuple.setNodeType({ QueryNodeType::tuple });
			QueryNode tuple_children[] = { child1 };
			tuple.setChildren(tuple_children, 1);
			QueryNode root_children[] = { tuple, child2 };
			root.setChildren(root_children, 2);

			QueryNode processed_clauses = root; //stores root node of the tree

			// Evaluate
			QueryEvaluator qe = QueryEvaluator(*pkb2);
			QUERY_RESULT result = qe.evaluateQuery(processed_synonyms, processed_clauses);
			STRING_RESULT result_string = ResultListManager::getStringValues(result);
			Logger::WriteMessage("Result: ");
			Logger::WriteMessage(result_string.c_str());

			std::unordered_set<CONSTANT_VALUE> set_result(result.begin(), result.end());
			std::unordered_set<CONSTANT_VALUE> correct_result = { "a", "b", "c" };
			Assert::IsTrue(set_result == correct_result);
		}

		TEST_METHOD(evaluateQuery_SelectV_PatternIV_ReturnsVariables)
		{
			// Query: "if i; variable v; Select v pattern i (v, _, _)"
			// Get processed_synonyms and processed clauses
			QueryNode if_node;
			if_node.setSynonymNode({ QuerySynonymType::ifs }, "i");
			QueryNode variable_node;
			variable_node.setSynonymNode({ QuerySynonymType::variable }, "v");
			std::unordered_map<std::string, QueryNode> processed_synonyms = { {"i", if_node}, {"v", variable_node} };

			QueryNode child1 = QueryNode();
			child1.setSynonymNode({ QuerySynonymType::variable }, "v");
			QueryNode child2 = QueryNode();
			child2.setNodeType({ QueryNodeType::pattern });
			QueryNode child_child_child0 = QueryNode();
			child_child_child0.setSynonymNode({ QuerySynonymType::ifs }, "i");

			QueryNode child_child_child1 = QueryNode();
			child_child_child1.setSynonymNode({ QuerySynonymType::variable }, "v");

			QueryNode child2_children[] = { child_child_child0, child_child_child1 };
			child2.setChildren(child2_children, 2);

			QueryNode root = QueryNode();
			root.setNodeType({ QueryNodeType::select });
			QueryNode tuple = QueryNode();
			tuple.setNodeType({ QueryNodeType::tuple });
			QueryNode tuple_children[] = { child1 };
			tuple.setChildren(tuple_children, 1);
			QueryNode root_children[] = { tuple, child2 };
			root.setChildren(root_children, 2);

			QueryNode processed_clauses = root; //stores root node of the tree

			// Evaluate
			QueryEvaluator qe = QueryEvaluator(*pkb3);
			QUERY_RESULT result = qe.evaluateQuery(processed_synonyms, processed_clauses);
			STRING_RESULT result_string = ResultListManager::getStringValues(result);
			Logger::WriteMessage("Result: ");
			Logger::WriteMessage(result_string.c_str());

			std::unordered_set<CONSTANT_VALUE> set_result(result.begin(), result.end());
			std::unordered_set<CONSTANT_VALUE> correct_result = { "a", "c", "t", "d", "o", "g" };
			Assert::IsTrue(set_result == correct_result);
		}

		TEST_METHOD(evaluateQuery_SelectV_PatternWV_ReturnsVariables)
		{
			// Query: "while w; variable v; Select v pattern w (v, _)"
			// Get processed_synonyms and processed clauses
			QueryNode while_node;
			while_node.setSynonymNode({ QuerySynonymType::whiles }, "w");
			QueryNode variable_node;
			variable_node.setSynonymNode({ QuerySynonymType::variable }, "v");
			std::unordered_map<std::string, QueryNode> processed_synonyms = { {"w", while_node}, {"v", variable_node} };

			QueryNode child1 = QueryNode();
			child1.setSynonymNode({ QuerySynonymType::variable }, "v");
			QueryNode child2 = QueryNode();
			child2.setNodeType({ QueryNodeType::pattern });
			QueryNode child_child_child0 = QueryNode();
			child_child_child0.setSynonymNode({ QuerySynonymType::whiles }, "w");

			QueryNode child_child_child1 = QueryNode();
			child_child_child1.setSynonymNode({ QuerySynonymType::variable }, "v");

			QueryNode child2_children[] = { child_child_child0, child_child_child1 };
			child2.setChildren(child2_children, 2);

			QueryNode root = QueryNode();
			root.setNodeType({ QueryNodeType::select });
			QueryNode tuple = QueryNode();
			tuple.setNodeType({ QueryNodeType::tuple });
			QueryNode tuple_children[] = { child1 };
			tuple.setChildren(tuple_children, 1);
			QueryNode root_children[] = { tuple, child2 };
			root.setChildren(root_children, 2);

			QueryNode processed_clauses = root; //stores root node of the tree

			// Evaluate
			QueryEvaluator qe = QueryEvaluator(*pkb3);
			QUERY_RESULT result = qe.evaluateQuery(processed_synonyms, processed_clauses);
			STRING_RESULT result_string = ResultListManager::getStringValues(result);
			Logger::WriteMessage("Result: ");
			Logger::WriteMessage(result_string.c_str());

			std::unordered_set<CONSTANT_VALUE> set_result(result.begin(), result.end());
			std::unordered_set<CONSTANT_VALUE> correct_result = { "meow", "a", "b", "c", "d" };
			Assert::IsTrue(set_result == correct_result);
		}

		TEST_METHOD(evaluateQuery_SelectV_varname_PatternWV_ReturnsVariables)
		{
			// Query: "while w; variable v; Select v.varName pattern w (v, _)"
			// Get processed_synonyms and processed clauses
			QueryNode while_node;
			while_node.setSynonymNode({ QuerySynonymType::whiles }, "w");
			QueryNode variable_node;
			variable_node.setSynonymNode({ QuerySynonymType::variable }, "v");
			std::unordered_map<std::string, QueryNode> processed_synonyms = { {"w", while_node}, {"v", variable_node} };

			QueryNode child1 = QueryNode();
			child1.setAttrNode("v", "varName");
			QueryNode child2 = QueryNode();
			child2.setNodeType({ QueryNodeType::pattern });
			QueryNode child_child_child0 = QueryNode();
			child_child_child0.setSynonymNode({ QuerySynonymType::whiles }, "w");

			QueryNode child_child_child1 = QueryNode();
			child_child_child1.setSynonymNode({ QuerySynonymType::variable }, "v");

			QueryNode child2_children[] = { child_child_child0, child_child_child1 };
			child2.setChildren(child2_children, 2);

			QueryNode root = QueryNode();
			root.setNodeType({ QueryNodeType::select });
			QueryNode tuple = QueryNode();
			tuple.setNodeType({ QueryNodeType::tuple });
			QueryNode tuple_children[] = { child1 };
			tuple.setChildren(tuple_children, 1);
			QueryNode root_children[] = { tuple, child2 };
			root.setChildren(root_children, 2);

			QueryNode processed_clauses = root; //stores root node of the tree

			// Evaluate
			QueryEvaluator qe = QueryEvaluator(*pkb3);
			QUERY_RESULT result = qe.evaluateQuery(processed_synonyms, processed_clauses);
			STRING_RESULT result_string = ResultListManager::getStringValues(result);
			Logger::WriteMessage("Result: ");
			Logger::WriteMessage(result_string.c_str());

			std::unordered_set<CONSTANT_VALUE> set_result(result.begin(), result.end());
			std::unordered_set<CONSTANT_VALUE> correct_result = { "meow", "a", "b", "c", "d" };
			Assert::IsTrue(set_result == correct_result);
		}

		/*=================================  PKB 4: Calls =========================================*/
		TEST_METHOD(evaluateQuery_SelectP1_CallsP1P2_ReturnsP1)
		{
			// Query: "procedure p1, p2; Select p1 such that Calls(p1, p2)"
			// Get processed_synonyms and processed clauses
			QueryNode proc1_node = QueryNode();
			proc1_node.setSynonymNode({ QuerySynonymType::procedure }, "p1");
			QueryNode proc2_node = QueryNode();
			proc2_node.setSynonymNode({ QuerySynonymType::procedure }, "p2");
			std::unordered_map<std::string, QueryNode> processed_synonyms = { {"p1", proc1_node}, {"p2", proc2_node} };

			// Select: p1
			QueryNode child1 = QueryNode();
			child1.setSynonymNode({ QuerySynonymType::procedure }, "p1");

			// such that 
			QueryNode child2 = QueryNode();
			child2.setNodeType({ QueryNodeType::such_that });

			// Calls
			QueryNode child_child1 = QueryNode();
			child_child1.setNodeType({ QueryNodeType::calls });

			// arg 1: p1
			QueryNode child_child_child1 = QueryNode();
			child_child_child1.setSynonymNode({ QuerySynonymType::procedure }, "p1");

			// arg 2: p2
			QueryNode child_child_child2 = QueryNode();
			child_child_child2.setSynonymNode({ QuerySynonymType::procedure }, "p2");
			
			// set children, make tree
			QueryNode child_child1_children[] = { child_child_child1, child_child_child2 };
			child_child1.setChildren(child_child1_children, 2);
			QueryNode child2_children[] = { child_child1 };
			child2.setChildren(child2_children, 1);

			QueryNode root = QueryNode();
			root.setNodeType({ QueryNodeType::select });
			QueryNode tuple = QueryNode();
			tuple.setNodeType({ QueryNodeType::tuple });
			QueryNode tuple_children[] = { child1 };
			tuple.setChildren(tuple_children, 1);
			QueryNode root_children[] = { tuple, child2 };
			root.setChildren(root_children, 2);

			QueryNode processed_clauses = root; //stores root node of the tree

			// Evaluate
			QueryEvaluator qe = QueryEvaluator(*pkb4);
			QUERY_RESULT result;
			STRING_RESULT result_string;

			try {
				result = qe.evaluateQuery(processed_synonyms, processed_clauses);
				result_string = ResultListManager::getStringValues(result);
			}
			catch (const char* msg) {
				Logger::WriteMessage(msg);
			}
			STRING_RESULT correct_result = "main, main, main, main, morning, afternoon, evening, night";

			Logger::WriteMessage(result_string.c_str());
			Assert::IsTrue(result_string.compare(correct_result) == 0);
		}

		TEST_METHOD(evaluateQuery_SelectP2_CallsP1P2_ReturnsP2)
		{
			// Query: "procedure p1, p2; Select p2 such that Calls(p1, p2)"
			// Get processed_synonyms and processed clauses
			QueryNode proc1_node = QueryNode();
			proc1_node.setSynonymNode({ QuerySynonymType::procedure }, "p1");
			QueryNode proc2_node = QueryNode();
			proc2_node.setSynonymNode({ QuerySynonymType::procedure }, "p2");
			std::unordered_map<std::string, QueryNode> processed_synonyms = { {"p1", proc1_node}, {"p2", proc2_node} };

			// Select: p1
			QueryNode child1 = QueryNode();
			child1.setSynonymNode({ QuerySynonymType::procedure }, "p2");

			// such that 
			QueryNode child2 = QueryNode();
			child2.setNodeType({ QueryNodeType::such_that });

			// Calls
			QueryNode child_child1 = QueryNode();
			child_child1.setNodeType({ QueryNodeType::calls });

			// arg 1: p1
			QueryNode child_child_child1 = QueryNode();
			child_child_child1.setSynonymNode({ QuerySynonymType::procedure }, "p1");

			// arg 2: p2
			QueryNode child_child_child2 = QueryNode();
			child_child_child2.setSynonymNode({ QuerySynonymType::procedure }, "p2");

			// set children, make tree
			QueryNode child_child1_children[] = { child_child_child1, child_child_child2 };
			child_child1.setChildren(child_child1_children, 2);
			QueryNode child2_children[] = { child_child1 };
			child2.setChildren(child2_children, 1);

			QueryNode root = QueryNode();
			root.setNodeType({ QueryNodeType::select });
			QueryNode tuple = QueryNode();
			tuple.setNodeType({ QueryNodeType::tuple });
			QueryNode tuple_children[] = { child1 };
			tuple.setChildren(tuple_children, 1);
			QueryNode root_children[] = { tuple, child2 };
			root.setChildren(root_children, 2);

			QueryNode processed_clauses = root; //stores root node of the tree

			// Evaluate
			QueryEvaluator qe = QueryEvaluator(*pkb4);
			QUERY_RESULT result;
			STRING_RESULT result_string;

			try {
				result = qe.evaluateQuery(processed_synonyms, processed_clauses);
				result_string = ResultListManager::getStringValues(result);
			}
			catch (const char* msg) {
				Logger::WriteMessage(msg);
			}
			STRING_RESULT correct_result = "morning, afternoon, evening, night, breakfast, lunch, dinner, diarrhoea";

			Logger::WriteMessage(result_string.c_str());
			Assert::IsTrue(result_string.compare(correct_result) == 0);
		}

		TEST_METHOD(evaluateQuery_SelectP2R_varname_CallsP1P2_ReturnsP2R)
		{
			// Query: "procedure p1, p2; read r; Select <p2, r.varName> such that Calls(p1, p2)"
			// Get processed_synonyms and processed clauses
			QueryNode proc1_node = QueryNode();
			proc1_node.setSynonymNode({ QuerySynonymType::procedure }, "p1");
			QueryNode proc2_node = QueryNode();
			proc2_node.setSynonymNode({ QuerySynonymType::procedure }, "p2");
			QueryNode read_node = QueryNode();
			read_node.setSynonymNode({ QuerySynonymType::read }, "r");
			std::unordered_map<std::string, QueryNode> processed_synonyms = { {"p1", proc1_node}, {"p2", proc2_node}, {"r", read_node} };

			// Select: <p1, r.varName>
			QueryNode child1 = QueryNode();
			child1.setSynonymNode({ QuerySynonymType::procedure }, "p2");
			QueryNode child1_2 = QueryNode();
			child1_2.setAttrNode("r", "varName");

			// such that 
			QueryNode child2 = QueryNode();
			child2.setNodeType({ QueryNodeType::such_that });

			// Calls
			QueryNode child_child1 = QueryNode();
			child_child1.setNodeType({ QueryNodeType::calls });

			// arg 1: p1
			QueryNode child_child_child1 = QueryNode();
			child_child_child1.setSynonymNode({ QuerySynonymType::procedure }, "p1");

			// arg 2: p2
			QueryNode child_child_child2 = QueryNode();
			child_child_child2.setSynonymNode({ QuerySynonymType::procedure }, "p2");

			// set children, make tree
			QueryNode child_child1_children[] = { child_child_child1, child_child_child2 };
			child_child1.setChildren(child_child1_children, 2);
			QueryNode child2_children[] = { child_child1 };
			child2.setChildren(child2_children, 1);

			QueryNode root = QueryNode();
			root.setNodeType({ QueryNodeType::select });
			QueryNode tuple = QueryNode();
			tuple.setNodeType({ QueryNodeType::tuple });
			QueryNode tuple_children[] = { child1, child1_2 };
			tuple.setChildren(tuple_children, 2);
			QueryNode root_children[] = { tuple, child2 };
			root.setChildren(root_children, 2);

			QueryNode processed_clauses = root; //stores root node of the tree

			// Evaluate
			QueryEvaluator qe = QueryEvaluator(*pkb4);
			QUERY_RESULT result;
			STRING_RESULT result_string;

			try {
				result = qe.evaluateQuery(processed_synonyms, processed_clauses);
				result_string = ResultListManager::getStringValues(result);
			}
			catch (const char* msg) {
				Logger::WriteMessage(msg);
			}
			STRING_RESULT correct_result = "morning newspaper, morning menu, morning menu, afternoon newspaper, afternoon menu, afternoon menu, evening newspaper, evening menu, evening menu, night newspaper, night menu, night menu, breakfast newspaper, breakfast menu, breakfast menu, lunch newspaper, lunch menu, lunch menu, dinner newspaper, dinner menu, dinner menu, diarrhoea newspaper, diarrhoea menu, diarrhoea menu";


			Logger::WriteMessage(result_string.c_str());
			Assert::IsTrue(result_string.compare(correct_result) == 0);
		}

		TEST_METHOD(evaluateQuery_SelectP1_CallsP1Wildcard_ReturnsP1)
		{
			// Query: "procedure p1; Select p1 such that Calls(p1, _)"
			// Get processed_synonyms and processed clauses
			QueryNode proc1_node = QueryNode();
			proc1_node.setSynonymNode({ QuerySynonymType::procedure }, "p1");
			std::unordered_map<std::string, QueryNode> processed_synonyms = { {"p1", proc1_node} };

			// Select: p1
			QueryNode child1 = QueryNode();
			child1.setSynonymNode({ QuerySynonymType::procedure }, "p1");

			// such that 
			QueryNode child2 = QueryNode();
			child2.setNodeType({ QueryNodeType::such_that });

			// Calls
			QueryNode child_child1 = QueryNode();
			child_child1.setNodeType({ QueryNodeType::calls });

			// arg 1: p1
			QueryNode child_child_child1 = QueryNode();
			child_child_child1.setSynonymNode({ QuerySynonymType::procedure }, "p1");

			// arg 2: _
			QueryNode child_child_child2 = QueryNode();
			child_child_child2.setNodeType({ QueryNodeType::wild_card });

			// set children, make tree
			QueryNode child_child1_children[] = { child_child_child1, child_child_child2 };
			child_child1.setChildren(child_child1_children, 2);
			QueryNode child2_children[] = { child_child1 };
			child2.setChildren(child2_children, 1);

			QueryNode root = QueryNode();
			root.setNodeType({ QueryNodeType::select });
			QueryNode tuple = QueryNode();
			tuple.setNodeType({ QueryNodeType::tuple });
			QueryNode tuple_children[] = { child1 };
			tuple.setChildren(tuple_children, 1);
			QueryNode root_children[] = { tuple, child2 };
			root.setChildren(root_children, 2);

			QueryNode processed_clauses = root; //stores root node of the tree

			// Evaluate
			QueryEvaluator qe = QueryEvaluator(*pkb4);
			QUERY_RESULT result;
			STRING_RESULT result_string;

			try {
				result = qe.evaluateQuery(processed_synonyms, processed_clauses);
				result_string = ResultListManager::getStringValues(result);
			}
			catch (const char* msg) {
				Logger::WriteMessage(msg);
			}
			STRING_RESULT correct_result = "main, main, main, main, morning, afternoon, evening, night";

			Logger::WriteMessage(result_string.c_str());
			Assert::IsTrue(result_string.compare(correct_result) == 0);
		}

		TEST_METHOD(evaluateQuery_SelectP1_CallsIdentP1_ReturnsP1)
		{
			// Query: "procedure p1; Select p1 such that Calls("main", p1)"
			// Get processed_synonyms and processed clauses
			QueryNode proc1_node = QueryNode();
			proc1_node.setSynonymNode({ QuerySynonymType::procedure }, "p1");
			std::unordered_map<std::string, QueryNode> processed_synonyms = { {"p1", proc1_node} };

			// Select: p1
			QueryNode child1 = QueryNode();
			child1.setSynonymNode({ QuerySynonymType::procedure }, "p1");

			// such that 
			QueryNode child2 = QueryNode();
			child2.setNodeType({ QueryNodeType::such_that });

			// Calls
			QueryNode child_child1 = QueryNode();
			child_child1.setNodeType({ QueryNodeType::calls });

			// arg 1: "main"
			QueryNode child_child_child1 = QueryNode();
			child_child_child1.setIdentityNode("main");

			// arg 2: p1
			QueryNode child_child_child2 = QueryNode();
			child_child_child2.setSynonymNode({ QuerySynonymType::procedure }, "p1");

			// set children, make tree
			QueryNode child_child1_children[] = { child_child_child1, child_child_child2 };
			child_child1.setChildren(child_child1_children, 2);
			QueryNode child2_children[] = { child_child1 };
			child2.setChildren(child2_children, 1);

			QueryNode root = QueryNode();
			root.setNodeType({ QueryNodeType::select });
			QueryNode tuple = QueryNode();
			tuple.setNodeType({ QueryNodeType::tuple });
			QueryNode tuple_children[] = { child1 };
			tuple.setChildren(tuple_children, 1);
			QueryNode root_children[] = { tuple, child2 };
			root.setChildren(root_children, 2);

			QueryNode processed_clauses = root; //stores root node of the tree

			// Evaluate
			QueryEvaluator qe = QueryEvaluator(*pkb4);
			QUERY_RESULT result;
			STRING_RESULT result_string;

			try {
				result = qe.evaluateQuery(processed_synonyms, processed_clauses);
				result_string = ResultListManager::getStringValues(result);
			}
			catch (const char* msg) {
				Logger::WriteMessage(msg);
			}
			STRING_RESULT correct_result = "morning, afternoon, evening, night";

			Logger::WriteMessage(result_string.c_str());
			Assert::IsTrue(result_string.compare(correct_result) == 0);
		}

		TEST_METHOD(evaluateQuery_SelectBOOLEAN_CallsIdentIdent_ReturnsTrue)
		{
			// Query: "Select BOOLEAN such that Calls("main", "afternoon")"
			// Get processed_synonyms and processed clauses
			QueryNode proc1_node = QueryNode();
			proc1_node.setSynonymNode({ QuerySynonymType::procedure }, "p1");
			std::unordered_map<std::string, QueryNode> processed_synonyms = { {"p1", proc1_node} };

			// Select: p1
			QueryNode child1 = QueryNode();
			child1.setBooleanNode();

			// such that 
			QueryNode child2 = QueryNode();
			child2.setNodeType({ QueryNodeType::such_that });

			// Calls
			QueryNode child_child1 = QueryNode();
			child_child1.setNodeType({ QueryNodeType::calls });

			// arg 1: "main"
			QueryNode child_child_child1 = QueryNode();
			child_child_child1.setIdentityNode("main");

			// arg 2: "afternoon"
			QueryNode child_child_child2 = QueryNode();
			child_child_child2.setIdentityNode("afternoon");

			// set children, make tree
			QueryNode child_child1_children[] = { child_child_child1, child_child_child2 };
			child_child1.setChildren(child_child1_children, 2);
			QueryNode child2_children[] = { child_child1 };
			child2.setChildren(child2_children, 1);

			QueryNode root = QueryNode();
			root.setNodeType({ QueryNodeType::select });
			QueryNode root_children[] = { child1, child2 };
			root.setChildren(root_children, 2);

			QueryNode processed_clauses = root; //stores root node of the tree

			// Evaluate
			QueryEvaluator qe = QueryEvaluator(*pkb4);
			QUERY_RESULT result;
			STRING_RESULT result_string;

			try {
				result = qe.evaluateQuery(processed_synonyms, processed_clauses);
				result_string = ResultListManager::getStringValues(result);
			}
			catch (const char* msg) {
				Logger::WriteMessage(msg);
			}
			STRING_RESULT correct_result = "TRUE";

			Logger::WriteMessage(result_string.c_str());
			Assert::IsTrue(result_string.compare(correct_result) == 0);
		}

		TEST_METHOD(evaluateQuery_SelectBOOLEAN_CallsIdentIdent_ReturnsFalse)
		{
			// Query: "Select BOOLEAN such that Calls("main", "breakfast")"
			// Get processed_synonyms and processed clauses
			QueryNode proc1_node = QueryNode();
			proc1_node.setSynonymNode({ QuerySynonymType::procedure }, "p1");
			std::unordered_map<std::string, QueryNode> processed_synonyms = { {"p1", proc1_node} };

			// Select: p1
			QueryNode child1 = QueryNode();
			child1.setBooleanNode();

			// such that 
			QueryNode child2 = QueryNode();
			child2.setNodeType({ QueryNodeType::such_that });

			// Calls
			QueryNode child_child1 = QueryNode();
			child_child1.setNodeType({ QueryNodeType::calls });

			// arg 1: "main"
			QueryNode child_child_child1 = QueryNode();
			child_child_child1.setIdentityNode("main");

			// arg 2: "afternoon"
			QueryNode child_child_child2 = QueryNode();
			child_child_child2.setIdentityNode("breakfast");

			// set children, make tree
			QueryNode child_child1_children[] = { child_child_child1, child_child_child2 };
			child_child1.setChildren(child_child1_children, 2);
			QueryNode child2_children[] = { child_child1 };
			child2.setChildren(child2_children, 1);

			QueryNode root = QueryNode();
			root.setNodeType({ QueryNodeType::select });
			QueryNode root_children[] = { child1, child2 };
			root.setChildren(root_children, 2);

			QueryNode processed_clauses = root; //stores root node of the tree

			// Evaluate
			QueryEvaluator qe = QueryEvaluator(*pkb4);
			QUERY_RESULT result;
			STRING_RESULT result_string;

			try {
				result = qe.evaluateQuery(processed_synonyms, processed_clauses);
				result_string = ResultListManager::getStringValues(result);
			}
			catch (const char* msg) {
				Logger::WriteMessage(msg);
			}
			STRING_RESULT correct_result = "FALSE";

			Logger::WriteMessage(result_string.c_str());
			Assert::IsTrue(result_string.compare(correct_result) == 0);
		}

		TEST_METHOD(evaluateQuery_SelectP1_CallsWildcardWildcard_ReturnsP1)
		{
			// Query: "procedure p1; Select p1 such that Calls(_, _)"
			// Get processed_synonyms and processed clauses
			QueryNode proc1_node = QueryNode();
			proc1_node.setSynonymNode({ QuerySynonymType::procedure }, "p1");
			std::unordered_map<std::string, QueryNode> processed_synonyms = { {"p1", proc1_node} };

			// Select: p1
			QueryNode child1 = QueryNode();
			child1.setSynonymNode({ QuerySynonymType::procedure }, "p1");

			// such that 
			QueryNode child2 = QueryNode();
			child2.setNodeType({ QueryNodeType::such_that });

			// Calls
			QueryNode child_child1 = QueryNode();
			child_child1.setNodeType({ QueryNodeType::calls });

			// arg 1: _
			QueryNode child_child_child1 = QueryNode();
			child_child_child1.setNodeType({ QueryNodeType::wild_card });

			// arg 2: _
			QueryNode child_child_child2 = QueryNode();
			child_child_child2.setNodeType({ QueryNodeType::wild_card });

			// set children, make tree
			QueryNode child_child1_children[] = { child_child_child1, child_child_child2 };
			child_child1.setChildren(child_child1_children, 2);
			QueryNode child2_children[] = { child_child1 };
			child2.setChildren(child2_children, 1);

			QueryNode root = QueryNode();
			root.setNodeType({ QueryNodeType::select });
			QueryNode tuple = QueryNode();
			tuple.setNodeType({ QueryNodeType::tuple });
			QueryNode tuple_children[] = { child1 };
			tuple.setChildren(tuple_children, 1);
			QueryNode root_children[] = { tuple, child2 };
			root.setChildren(root_children, 2);

			QueryNode processed_clauses = root; //stores root node of the tree

			// Evaluate
			QueryEvaluator qe = QueryEvaluator(*pkb4);
			QUERY_RESULT result;
			STRING_RESULT result_string;

			try {
				result = qe.evaluateQuery(processed_synonyms, processed_clauses);
				result_string = ResultListManager::getStringValues(result);
			}
			catch (const char* msg) {
				Logger::WriteMessage(msg);
			}
			STRING_RESULT correct_result = "main, morning, afternoon, evening, night, breakfast, lunch, dinner, diarrhoea";

			Logger::WriteMessage(result_string.c_str());
			Assert::IsTrue(result_string.compare(correct_result) == 0);
		}

		/*=================================  PKB 4: CallsT =========================================*/
		
		TEST_METHOD(evaluateQuery_SelectP1_CallsTP1P2_ReturnsP1)
		{
			// Query: "procedure p1, p2; Select p1 such that CallsT(p1, p2)"
			// Get processed_synonyms and processed clauses
			QueryNode proc1_node = QueryNode();
			proc1_node.setSynonymNode({ QuerySynonymType::procedure }, "p1");
			QueryNode proc2_node = QueryNode();
			proc2_node.setSynonymNode({ QuerySynonymType::procedure }, "p2");
			std::unordered_map<std::string, QueryNode> processed_synonyms = { {"p1", proc1_node}, {"p2", proc2_node} };

			// Select: p1
			QueryNode child1 = QueryNode();
			child1.setSynonymNode({ QuerySynonymType::procedure }, "p1");

			// such that 
			QueryNode child2 = QueryNode();
			child2.setNodeType({ QueryNodeType::such_that });

			// CallsT
			QueryNode child_child1 = QueryNode();
			child_child1.setNodeType({ QueryNodeType::callsT });

			// arg 1: p1
			QueryNode child_child_child1 = QueryNode();
			child_child_child1.setSynonymNode({ QuerySynonymType::procedure }, "p1");

			// arg 2: p2
			QueryNode child_child_child2 = QueryNode();
			child_child_child2.setSynonymNode({ QuerySynonymType::procedure }, "p2");

			// set children, make tree
			QueryNode child_child1_children[] = { child_child_child1, child_child_child2 };
			child_child1.setChildren(child_child1_children, 2);
			QueryNode child2_children[] = { child_child1 };
			child2.setChildren(child2_children, 1);

			QueryNode root = QueryNode();
			root.setNodeType({ QueryNodeType::select });
			QueryNode tuple = QueryNode();
			tuple.setNodeType({ QueryNodeType::tuple });
			QueryNode tuple_children[] = { child1 };
			tuple.setChildren(tuple_children, 1);
			QueryNode root_children[] = { tuple, child2 };
			root.setChildren(root_children, 2);

			QueryNode processed_clauses = root; //stores root node of the tree

			// Evaluate
			QueryEvaluator qe = QueryEvaluator(*pkb4);
			QUERY_RESULT result;
			STRING_RESULT result_string;

			try {
				result = qe.evaluateQuery(processed_synonyms, processed_clauses);
				result_string = ResultListManager::getStringValues(result);
			}
			catch (const char* msg) {
				Logger::WriteMessage(msg);
			}
			STRING_RESULT correct_result = "main, main, main, main, main, main, main, main, morning, afternoon, evening, night";

			Logger::WriteMessage(result_string.c_str());
			Assert::IsTrue(result_string.compare(correct_result) == 0);
		}

		TEST_METHOD(evaluateQuery_SelectP1_CallsTP1Ident_ReturnsP1)
		{
			// Query: "procedure p1; Select p1 such that CallsT(p1, "diarrhoea")"
			// Get processed_synonyms and processed clauses
			QueryNode proc1_node = QueryNode();
			proc1_node.setSynonymNode({ QuerySynonymType::procedure }, "p1");
			std::unordered_map<std::string, QueryNode> processed_synonyms = { {"p1", proc1_node} };

			// Select: p1
			QueryNode child1 = QueryNode();
			child1.setSynonymNode({ QuerySynonymType::procedure }, "p1");

			// such that 
			QueryNode child2 = QueryNode();
			child2.setNodeType({ QueryNodeType::such_that });

			// CallsT
			QueryNode child_child1 = QueryNode();
			child_child1.setNodeType({ QueryNodeType::callsT });

			// arg 1: p1
			QueryNode child_child_child1 = QueryNode();
			child_child_child1.setSynonymNode({ QuerySynonymType::procedure }, "p1");

			// arg 2: diarrhoea
			QueryNode child_child_child2 = QueryNode();
			child_child_child2.setIdentityNode("diarrhoea");

			// set children, make tree
			QueryNode child_child1_children[] = { child_child_child1, child_child_child2 };
			child_child1.setChildren(child_child1_children, 2);
			QueryNode child2_children[] = { child_child1 };
			child2.setChildren(child2_children, 1);

			QueryNode root = QueryNode();
			root.setNodeType({ QueryNodeType::select });
			QueryNode tuple = QueryNode();
			tuple.setNodeType({ QueryNodeType::tuple });
			QueryNode tuple_children[] = { child1 };
			tuple.setChildren(tuple_children, 1);
			QueryNode root_children[] = { tuple, child2 };
			root.setChildren(root_children, 2);

			QueryNode processed_clauses = root; //stores root node of the tree

			// Evaluate
			QueryEvaluator qe = QueryEvaluator(*pkb4);
			QUERY_RESULT result;
			STRING_RESULT result_string;

			try {
				result = qe.evaluateQuery(processed_synonyms, processed_clauses);
				result_string = ResultListManager::getStringValues(result);
			}
			catch (const char* msg) {
				Logger::WriteMessage(msg);
			}
			STRING_RESULT correct_result = "main, night";

			Logger::WriteMessage(result_string.c_str());
			Assert::IsTrue(result_string.compare(correct_result) == 0);
		}

		TEST_METHOD(evaluateQuery_SelectP1_CallsTIdentP1_ReturnsP1)
		{
			// Query: "procedure p1; Select p1 such that CallsT("main", p1)"
			// Get processed_synonyms and processed clauses
			QueryNode proc1_node = QueryNode();
			proc1_node.setSynonymNode({ QuerySynonymType::procedure }, "p1");
			std::unordered_map<std::string, QueryNode> processed_synonyms = { {"p1", proc1_node} };

			// Select: p1
			QueryNode child1 = QueryNode();
			child1.setSynonymNode({ QuerySynonymType::procedure }, "p1");

			// such that 
			QueryNode child2 = QueryNode();
			child2.setNodeType({ QueryNodeType::such_that });

			// Calls
			QueryNode child_child1 = QueryNode();
			child_child1.setNodeType({ QueryNodeType::callsT });

			// arg 1: "main"
			QueryNode child_child_child1 = QueryNode();
			child_child_child1.setIdentityNode("main");

			// arg 2: p1
			QueryNode child_child_child2 = QueryNode();
			child_child_child2.setSynonymNode({ QuerySynonymType::procedure }, "p1");

			// set children, make tree
			QueryNode child_child1_children[] = { child_child_child1, child_child_child2 };
			child_child1.setChildren(child_child1_children, 2);
			QueryNode child2_children[] = { child_child1 };
			child2.setChildren(child2_children, 1);

			QueryNode root = QueryNode();
			root.setNodeType({ QueryNodeType::select });
			QueryNode tuple = QueryNode();
			tuple.setNodeType({ QueryNodeType::tuple });
			QueryNode tuple_children[] = { child1 };
			tuple.setChildren(tuple_children, 1);
			QueryNode root_children[] = { tuple, child2 };
			root.setChildren(root_children, 2);

			QueryNode processed_clauses = root; //stores root node of the tree

			// Evaluate
			QueryEvaluator qe = QueryEvaluator(*pkb4);
			QUERY_RESULT result;
			STRING_RESULT result_string;

			try {
				result = qe.evaluateQuery(processed_synonyms, processed_clauses);
				result_string = ResultListManager::getStringValues(result);
			}
			catch (const char* msg) {
				Logger::WriteMessage(msg);
			}
			STRING_RESULT correct_result = "morning, afternoon, evening, night, breakfast, lunch, dinner, diarrhoea";

			Logger::WriteMessage(result_string.c_str());
			Assert::IsTrue(result_string.compare(correct_result) == 0);
		}

		TEST_METHOD(evaluateQuery_Select_P1VC_CallsTIdentP1_ReturnsP1)
		{
			// Query: "procedure p1; variable v; constant c; Select <p1, v, c> such that CallsT("main", p1)"
			// Get processed_synonyms and processed clauses
			QueryNode proc1_node = QueryNode();
			proc1_node.setSynonymNode({ QuerySynonymType::procedure }, "p1");
			QueryNode var_node = QueryNode();
			var_node.setSynonymNode({ QuerySynonymType::variable }, "v");
			QueryNode const_node = QueryNode();
			const_node.setSynonymNode({ QuerySynonymType::constant}, "c");
			std::unordered_map<std::string, QueryNode> processed_synonyms = { {"p1", proc1_node}, {"v", var_node}, {"c", const_node} };

			// Select: <p1, v, c>
			QueryNode child1 = QueryNode();
			child1.setSynonymNode({ QuerySynonymType::procedure }, "p1");
			QueryNode child1_2 = QueryNode();
			child1_2.setSynonymNode({ QuerySynonymType::variable }, "v");
			QueryNode child1_3 = QueryNode();
			child1_3.setSynonymNode({ QuerySynonymType::constant }, "c");

			// such that 
			QueryNode child2 = QueryNode();
			child2.setNodeType({ QueryNodeType::such_that });

			// Calls
			QueryNode child_child1 = QueryNode();
			child_child1.setNodeType({ QueryNodeType::callsT });

			// arg 1: "main"
			QueryNode child_child_child1 = QueryNode();
			child_child_child1.setIdentityNode("main");

			// arg 2: p1
			QueryNode child_child_child2 = QueryNode();
			child_child_child2.setSynonymNode({ QuerySynonymType::procedure }, "p1");

			// set children, make tree
			QueryNode child_child1_children[] = { child_child_child1, child_child_child2 };
			child_child1.setChildren(child_child1_children, 2);
			QueryNode child2_children[] = { child_child1 };
			child2.setChildren(child2_children, 1);

			QueryNode root = QueryNode();
			root.setNodeType({ QueryNodeType::select });
			QueryNode tuple = QueryNode();
			tuple.setNodeType({ QueryNodeType::tuple });
			QueryNode tuple_children[] = { child1, child1_2, child1_3 };
			tuple.setChildren(tuple_children, 3);
			QueryNode root_children[] = { tuple, child2 };
			root.setChildren(root_children, 2);

			QueryNode processed_clauses = root; //stores root node of the tree

			// Evaluate
			QueryEvaluator qe = QueryEvaluator(*pkb4);
			QUERY_RESULT result;
			STRING_RESULT result_string;

			try {
				result = qe.evaluateQuery(processed_synonyms, processed_clauses);
				result_string = ResultListManager::getStringValues(result);
			}
			catch (const char* msg) {
				Logger::WriteMessage(msg);
			}
			STRING_RESULT correct_result = "morning newspaper 100, morning menu 100, morning pain 100, afternoon newspaper 100, afternoon menu 100, afternoon pain 100, evening newspaper 100, evening menu 100, evening pain 100, night newspaper 100, night menu 100, night pain 100, breakfast newspaper 100, breakfast menu 100, breakfast pain 100, lunch newspaper 100, lunch menu 100, lunch pain 100, dinner newspaper 100, dinner menu 100, dinner pain 100, diarrhoea newspaper 100, diarrhoea menu 100, diarrhoea pain 100";

			Logger::WriteMessage(result_string.c_str());
			Assert::IsTrue(result_string.compare(correct_result) == 0);
		}

		TEST_METHOD(evaluateQuery_Select_P1VC_procName_CallsTIdentP1_ReturnsEmpty)
		{
			// Query: "procedure p1; variable v; constant c; Select <p1, v, c.procName> such that CallsT("main", p1)"
			// Get processed_synonyms and processed clauses
			QueryNode proc1_node = QueryNode();
			proc1_node.setSynonymNode({ QuerySynonymType::procedure }, "p1");
			QueryNode var_node = QueryNode();
			var_node.setSynonymNode({ QuerySynonymType::variable }, "v");
			QueryNode const_node = QueryNode();
			const_node.setSynonymNode({ QuerySynonymType::constant }, "c");
			std::unordered_map<std::string, QueryNode> processed_synonyms = { {"p1", proc1_node}, {"v", var_node}, {"c", const_node} };

			// Select: <p1, v, c.procName>
			QueryNode child1 = QueryNode();
			child1.setSynonymNode({ QuerySynonymType::procedure }, "p1");
			QueryNode child1_2 = QueryNode();
			child1_2.setSynonymNode({ QuerySynonymType::variable }, "v");
			QueryNode child1_3 = QueryNode();
			child1_3.setAttrNode("c", "procName");

			// such that 
			QueryNode child2 = QueryNode();
			child2.setNodeType({ QueryNodeType::such_that });

			// Calls
			QueryNode child_child1 = QueryNode();
			child_child1.setNodeType({ QueryNodeType::callsT });

			// arg 1: "main"
			QueryNode child_child_child1 = QueryNode();
			child_child_child1.setIdentityNode("main");

			// arg 2: p1
			QueryNode child_child_child2 = QueryNode();
			child_child_child2.setSynonymNode({ QuerySynonymType::procedure }, "p1");

			// set children, make tree
			QueryNode child_child1_children[] = { child_child_child1, child_child_child2 };
			child_child1.setChildren(child_child1_children, 2);
			QueryNode child2_children[] = { child_child1 };
			child2.setChildren(child2_children, 1);

			QueryNode root = QueryNode();
			root.setNodeType({ QueryNodeType::select });
			QueryNode tuple = QueryNode();
			tuple.setNodeType({ QueryNodeType::tuple });
			QueryNode tuple_children[] = { child1, child1_2, child1_3 };
			tuple.setChildren(tuple_children, 3);
			QueryNode root_children[] = { tuple, child2 };
			root.setChildren(root_children, 2);

			QueryNode processed_clauses = root; //stores root node of the tree

			// Evaluate
			QueryEvaluator qe = QueryEvaluator(*pkb4);
			QUERY_RESULT result;
			STRING_RESULT result_string;

			try {
				result = qe.evaluateQuery(processed_synonyms, processed_clauses);
				result_string = ResultListManager::getStringValues(result);
			}
			catch (const char* msg) {
				Logger::WriteMessage(msg);
			}
			STRING_RESULT correct_result = "";

			Logger::WriteMessage(result_string.c_str());
			Assert::IsTrue(result_string.compare(correct_result) == 0);
		}

		TEST_METHOD(evaluateQuery_Select_P1VC_procName_CallsTIdentP1_ReturnsP1VC)
		{
			// Query: "procedure p1; read r; calls c; Select <p1, r, c.procName> such that CallsT("main", p1)"
			// Get processed_synonyms and processed clauses
			QueryNode proc1_node = QueryNode();
			proc1_node.setSynonymNode({ QuerySynonymType::procedure }, "p1");
			QueryNode var_node = QueryNode();
			var_node.setSynonymNode({ QuerySynonymType::read }, "r");
			QueryNode call_node = QueryNode();
			call_node.setSynonymNode({QuerySynonymType::call}, "c");
			std::unordered_map<std::string, QueryNode> processed_synonyms = { {"p1", proc1_node}, {"r", var_node}, {"c", call_node} };

			// Select: <p1, r, c.procName>
			QueryNode child1 = QueryNode();
			child1.setSynonymNode({ QuerySynonymType::procedure }, "p1");
			QueryNode child1_2 = QueryNode();
			child1_2.setSynonymNode({ QuerySynonymType::read }, "r");
			QueryNode child1_3 = QueryNode();
			child1_3.setAttrNode("c", "procName");

			// such that 
			QueryNode child2 = QueryNode();
			child2.setNodeType({ QueryNodeType::such_that });

			// Calls
			QueryNode child_child1 = QueryNode();
			child_child1.setNodeType({ QueryNodeType::callsT });

			// arg 1: "main"
			QueryNode child_child_child1 = QueryNode();
			child_child_child1.setIdentityNode("main");

			// arg 2: p1
			QueryNode child_child_child2 = QueryNode();
			child_child_child2.setSynonymNode({ QuerySynonymType::procedure }, "p1");

			// set children, make tree
			QueryNode child_child1_children[] = { child_child_child1, child_child_child2 };
			child_child1.setChildren(child_child1_children, 2);
			QueryNode child2_children[] = { child_child1 };
			child2.setChildren(child2_children, 1);

			QueryNode root = QueryNode();
			root.setNodeType({ QueryNodeType::select });
			QueryNode tuple = QueryNode();
			tuple.setNodeType({ QueryNodeType::tuple });
			QueryNode tuple_children[] = { child1, child1_2, child1_3 };
			tuple.setChildren(tuple_children, 3);
			QueryNode root_children[] = { tuple, child2 };
			root.setChildren(root_children, 2);

			QueryNode processed_clauses = root; //stores root node of the tree

			// Evaluate
			QueryEvaluator qe = QueryEvaluator(*pkb4);
			QUERY_RESULT result;
			STRING_RESULT result_string;

			try {
				result = qe.evaluateQuery(processed_synonyms, processed_clauses);
				result_string = ResultListManager::getStringValues(result);
			}
			catch (const char* msg) {
				Logger::WriteMessage(msg);
			}
			STRING_RESULT correct_result = "morning 9 morning, morning 9 afternoon, morning 9 evening, morning 9 night, "
				"morning 9 breakfast, morning 9 lunch, morning 9 dinner, morning 9 diarrhoea, morning 10 morning, morning 10 afternoon, "
				"morning 10 evening, morning 10 night, morning 10 breakfast, morning 10 lunch, morning 10 dinner, morning 10 diarrhoea, "
				"morning 11 morning, morning 11 afternoon, morning 11 evening, morning 11 night, morning 11 breakfast, morning 11 lunch, "
				"morning 11 dinner, morning 11 diarrhoea, afternoon 9 morning, afternoon 9 afternoon, afternoon 9 evening, afternoon 9 night, "
				"afternoon 9 breakfast, afternoon 9 lunch, afternoon 9 dinner, afternoon 9 diarrhoea, afternoon 10 morning, "
				"afternoon 10 afternoon, afternoon 10 evening, afternoon 10 night, afternoon 10 breakfast, afternoon 10 lunch, "
				"afternoon 10 dinner, afternoon 10 diarrhoea, afternoon 11 morning, afternoon 11 afternoon, afternoon 11 evening, "
				"afternoon 11 night, afternoon 11 breakfast, afternoon 11 lunch, afternoon 11 dinner, afternoon 11 diarrhoea, "
				"evening 9 morning, evening 9 afternoon, evening 9 evening, evening 9 night, evening 9 breakfast, evening 9 lunch, "
				"evening 9 dinner, evening 9 diarrhoea, evening 10 morning, evening 10 afternoon, evening 10 evening, evening 10 night, "
				"evening 10 breakfast, evening 10 lunch, evening 10 dinner, evening 10 diarrhoea, evening 11 morning, evening 11 afternoon, "
				"evening 11 evening, evening 11 night, evening 11 breakfast, evening 11 lunch, evening 11 dinner, evening 11 diarrhoea, "
				"night 9 morning, night 9 afternoon, night 9 evening, night 9 night, night 9 breakfast, night 9 lunch, night 9 dinner, "
				"night 9 diarrhoea, night 10 morning, night 10 afternoon, night 10 evening, night 10 night, night 10 breakfast, night 10 lunch, "
				"night 10 dinner, night 10 diarrhoea, night 11 morning, night 11 afternoon, night 11 evening, night 11 night, night 11 breakfast, "
				"night 11 lunch, night 11 dinner, night 11 diarrhoea, breakfast 9 morning, breakfast 9 afternoon, breakfast 9 evening, "
				"breakfast 9 night, breakfast 9 breakfast, breakfast 9 lunch, breakfast 9 dinner, breakfast 9 diarrhoea, breakfast 10 morning, "
				"breakfast 10 afternoon, breakfast 10 evening, breakfast 10 night, breakfast 10 breakfast, breakfast 10 lunch, breakfast 10 dinner, "
				"breakfast 10 diarrhoea, breakfast 11 morning, breakfast 11 afternoon, breakfast 11 evening, breakfast 11 night, breakfast 11 breakfast, "
				"breakfast 11 lunch, breakfast 11 dinner, breakfast 11 diarrhoea, lunch 9 morning, lunch 9 afternoon, lunch 9 evening, lunch 9 night, "
				"lunch 9 breakfast, lunch 9 lunch, lunch 9 dinner, lunch 9 diarrhoea, lunch 10 morning, lunch 10 afternoon, lunch 10 evening, "
				"lunch 10 night, lunch 10 breakfast, lunch 10 lunch, lunch 10 dinner, lunch 10 diarrhoea, lunch 11 morning, lunch 11 afternoon, "
				"lunch 11 evening, lunch 11 night, lunch 11 breakfast, lunch 11 lunch, lunch 11 dinner, lunch 11 diarrhoea, dinner 9 morning, "
				"dinner 9 afternoon, dinner 9 evening, dinner 9 night, dinner 9 breakfast, dinner 9 lunch, dinner 9 dinner, dinner 9 diarrhoea, "
				"dinner 10 morning, dinner 10 afternoon, dinner 10 evening, dinner 10 night, dinner 10 breakfast, dinner 10 lunch, dinner 10 dinner, "
				"dinner 10 diarrhoea, dinner 11 morning, dinner 11 afternoon, dinner 11 evening, dinner 11 night, dinner 11 breakfast, dinner 11 lunch, "
				"dinner 11 dinner, dinner 11 diarrhoea, diarrhoea 9 morning, diarrhoea 9 afternoon, diarrhoea 9 evening, diarrhoea 9 night, diarrhoea 9 breakfast, "
				"diarrhoea 9 lunch, diarrhoea 9 dinner, diarrhoea 9 diarrhoea, diarrhoea 10 morning, diarrhoea 10 afternoon, diarrhoea 10 evening, diarrhoea 10 night, "
				"diarrhoea 10 breakfast, diarrhoea 10 lunch, diarrhoea 10 dinner, diarrhoea 10 diarrhoea, diarrhoea 11 morning, diarrhoea 11 afternoon, "
				"diarrhoea 11 evening, diarrhoea 11 night, diarrhoea 11 breakfast, diarrhoea 11 lunch, diarrhoea 11 dinner, diarrhoea 11 diarrhoea";

			Logger::WriteMessage(result_string.c_str());
			Assert::IsTrue(result_string.compare(correct_result) == 0);
		}

		TEST_METHOD(evaluateQuery_Select_P1VC_value_CallsTIdentP1_ReturnsP1VC)
		{
			// Query: "procedure p1; variable v; constant c; Select <p1, v, c.value> such that CallsT("main", p1)"
			// Get processed_synonyms and processed clauses
			QueryNode proc1_node = QueryNode();
			proc1_node.setSynonymNode({ QuerySynonymType::procedure }, "p1");
			QueryNode var_node = QueryNode();
			var_node.setSynonymNode({ QuerySynonymType::variable }, "v");
			QueryNode const_node = QueryNode();
			const_node.setSynonymNode({ QuerySynonymType::constant }, "c");
			std::unordered_map<std::string, QueryNode> processed_synonyms = { {"p1", proc1_node}, {"v", var_node}, {"c", const_node} };

			// Select: <p1, v, c.value>
			QueryNode child1 = QueryNode();
			child1.setSynonymNode({ QuerySynonymType::procedure }, "p1");
			QueryNode child1_2 = QueryNode();
			child1_2.setSynonymNode({ QuerySynonymType::variable }, "v");
			QueryNode child1_3 = QueryNode();
			child1_3.setAttrNode("c", "value");

			// such that 
			QueryNode child2 = QueryNode();
			child2.setNodeType({ QueryNodeType::such_that });

			// Calls
			QueryNode child_child1 = QueryNode();
			child_child1.setNodeType({ QueryNodeType::callsT });

			// arg 1: "main"
			QueryNode child_child_child1 = QueryNode();
			child_child_child1.setIdentityNode("main");

			// arg 2: p1
			QueryNode child_child_child2 = QueryNode();
			child_child_child2.setSynonymNode({ QuerySynonymType::procedure }, "p1");

			// set children, make tree
			QueryNode child_child1_children[] = { child_child_child1, child_child_child2 };
			child_child1.setChildren(child_child1_children, 2);
			QueryNode child2_children[] = { child_child1 };
			child2.setChildren(child2_children, 1);

			QueryNode root = QueryNode();
			root.setNodeType({ QueryNodeType::select });
			QueryNode tuple = QueryNode();
			tuple.setNodeType({ QueryNodeType::tuple });
			QueryNode tuple_children[] = { child1, child1_2, child1_3 };
			tuple.setChildren(tuple_children, 3);
			QueryNode root_children[] = { tuple, child2 };
			root.setChildren(root_children, 2);

			QueryNode processed_clauses = root; //stores root node of the tree

			// Evaluate
			QueryEvaluator qe = QueryEvaluator(*pkb4);
			QUERY_RESULT result;
			STRING_RESULT result_string;

			try {
				result = qe.evaluateQuery(processed_synonyms, processed_clauses);
				result_string = ResultListManager::getStringValues(result);
			}
			catch (const char* msg) {
				Logger::WriteMessage(msg);
			}
			STRING_RESULT correct_result = "morning newspaper 100, morning menu 100, morning pain 100, afternoon newspaper 100, afternoon menu 100, "
				"afternoon pain 100, evening newspaper 100, evening menu 100, evening pain 100, night newspaper 100, night menu 100, night pain 100, breakfast newspaper 100, "
				"breakfast menu 100, breakfast pain 100, lunch newspaper 100, lunch menu 100, lunch pain 100, dinner newspaper 100, dinner menu 100, dinner pain 100, diarrhoea newspaper 100, "
				"diarrhoea menu 100, diarrhoea pain 100";

			Logger::WriteMessage(result_string.c_str());
			Assert::IsTrue(result_string.compare(correct_result) == 0);
		}

		TEST_METHOD(evaluateQuery_Select_VC_CallsTIdentP1_ReturnsP1)
		{
			// Query: "procedure p1; variable v; constant c; Select <v, c> such that CallsT("main", p1)"
			// Get processed_synonyms and processed clauses
			QueryNode var_node = QueryNode();
			var_node.setSynonymNode({ QuerySynonymType::variable }, "v");
			QueryNode const_node = QueryNode();
			const_node.setSynonymNode({ QuerySynonymType::constant }, "c");
			QueryNode proc_node = QueryNode();
			proc_node.setSynonymNode({ QuerySynonymType::procedure }, "p1");
			std::unordered_map<std::string, QueryNode> processed_synonyms = { {"v", var_node}, {"c", const_node}, {"p1", proc_node} };

			// Select: <v, c>
			QueryNode child1_2 = QueryNode();
			child1_2.setSynonymNode({ QuerySynonymType::variable }, "v");
			QueryNode child1_3 = QueryNode();
			child1_3.setSynonymNode({ QuerySynonymType::constant }, "c");

			// such that 
			QueryNode child2 = QueryNode();
			child2.setNodeType({ QueryNodeType::such_that });

			// Calls
			QueryNode child_child1 = QueryNode();
			child_child1.setNodeType({ QueryNodeType::callsT });

			// arg 1: "main"
			QueryNode child_child_child1 = QueryNode();
			child_child_child1.setIdentityNode("main");

			// arg 2: p1
			QueryNode child_child_child2 = QueryNode();
			child_child_child2.setSynonymNode({ QuerySynonymType::procedure }, "p1");

			// set children, make tree
			QueryNode child_child1_children[] = { child_child_child1, child_child_child2 };
			child_child1.setChildren(child_child1_children, 2);
			QueryNode child2_children[] = { child_child1 };
			child2.setChildren(child2_children, 1);

			QueryNode root = QueryNode();
			root.setNodeType({ QueryNodeType::select });
			QueryNode tuple = QueryNode();
			tuple.setNodeType({ QueryNodeType::tuple });
			QueryNode tuple_children[] = {child1_2, child1_3 };
			tuple.setChildren(tuple_children, 2);
			QueryNode root_children[] = { tuple, child2 };
			root.setChildren(root_children, 2);

			QueryNode processed_clauses = root; //stores root node of the tree

			// Evaluate
			QueryEvaluator qe = QueryEvaluator(*pkb4);
			QUERY_RESULT result;
			STRING_RESULT result_string;

			try {
				result = qe.evaluateQuery(processed_synonyms, processed_clauses);
				result_string = ResultListManager::getStringValues(result);
			}
			catch (const char* msg) {
				Logger::WriteMessage(msg);
			}
			STRING_RESULT correct_result = "newspaper 100, menu 100, pain 100, newspaper 100, menu 100, pain 100, newspaper 100, menu 100, pain 100, newspaper 100, menu 100, pain 100, newspaper 100, menu 100, pain 100, newspaper 100, menu 100, pain 100, newspaper 100, menu 100, pain 100, newspaper 100, menu 100, pain 100";

			Logger::WriteMessage(result_string.c_str());
			Assert::IsTrue(result_string.compare(correct_result) == 0);
		}

		TEST_METHOD(evaluateQuery_SelectBOOLEAN_CallsTIdentIdent_ReturnsTrue)
		{
			// Query: "Select BOOLEAN such that CallsT("main", "breakfast")"
			// Get processed_synonyms and processed clauses
			QueryNode proc1_node = QueryNode();
			proc1_node.setSynonymNode({ QuerySynonymType::procedure }, "p1");
			std::unordered_map<std::string, QueryNode> processed_synonyms = { {"p1", proc1_node} };

			// Select: p1
			QueryNode child1 = QueryNode();
			child1.setBooleanNode();

			// such that 
			QueryNode child2 = QueryNode();
			child2.setNodeType({ QueryNodeType::such_that });

			// CallsT
			QueryNode child_child1 = QueryNode();
			child_child1.setNodeType({ QueryNodeType::callsT });

			// arg 1: "main"
			QueryNode child_child_child1 = QueryNode();
			child_child_child1.setIdentityNode("main");

			// arg 2: "afternoon"
			QueryNode child_child_child2 = QueryNode();
			child_child_child2.setIdentityNode("breakfast");

			// set children, make tree
			QueryNode child_child1_children[] = { child_child_child1, child_child_child2 };
			child_child1.setChildren(child_child1_children, 2);
			QueryNode child2_children[] = { child_child1 };
			child2.setChildren(child2_children, 1);

			QueryNode root = QueryNode();
			root.setNodeType({ QueryNodeType::select });
			QueryNode root_children[] = { child1, child2 };
			root.setChildren(root_children, 2);

			QueryNode processed_clauses = root; //stores root node of the tree

			// Evaluate
			QueryEvaluator qe = QueryEvaluator(*pkb4);
			QUERY_RESULT result;
			STRING_RESULT result_string;

			try {
				result = qe.evaluateQuery(processed_synonyms, processed_clauses);
				result_string = ResultListManager::getStringValues(result);
			}
			catch (const char* msg) {
				Logger::WriteMessage(msg);
			}
			STRING_RESULT correct_result = "TRUE";

			Logger::WriteMessage(result_string.c_str());
			Assert::IsTrue(result_string.compare(correct_result) == 0);
		}

		TEST_METHOD(evaluateQuery_SelectBOOLEAN_CallsTIdentIdent_ReturnsFalse)
		{
			// Query: "Select BOOLEAN such that CallsT("morning", "diarrhoea")"
			// Get processed_synonyms and processed clauses
			QueryNode proc1_node = QueryNode();
			proc1_node.setSynonymNode({ QuerySynonymType::procedure }, "p1");
			std::unordered_map<std::string, QueryNode> processed_synonyms = { {"p1", proc1_node} };

			// Select: p1
			QueryNode child1 = QueryNode();
			child1.setBooleanNode();

			// such that 
			QueryNode child2 = QueryNode();
			child2.setNodeType({ QueryNodeType::such_that });

			// CallsT
			QueryNode child_child1 = QueryNode();
			child_child1.setNodeType({ QueryNodeType::callsT });

			// arg 1: "moarning"
			QueryNode child_child_child1 = QueryNode();
			child_child_child1.setIdentityNode("morning");

			// arg 2: "diarrhoea"
			QueryNode child_child_child2 = QueryNode();
			child_child_child2.setIdentityNode("diarrhoea");

			// set children, make tree
			QueryNode child_child1_children[] = { child_child_child1, child_child_child2 };
			child_child1.setChildren(child_child1_children, 2);
			QueryNode child2_children[] = { child_child1 };
			child2.setChildren(child2_children, 1);

			QueryNode root = QueryNode();
			root.setNodeType({ QueryNodeType::select });
			QueryNode root_children[] = { child1, child2 };
			root.setChildren(root_children, 2);

			QueryNode processed_clauses = root; //stores root node of the tree

			// Evaluate
			QueryEvaluator qe = QueryEvaluator(*pkb4);
			QUERY_RESULT result;
			STRING_RESULT result_string;

			try {
				result = qe.evaluateQuery(processed_synonyms, processed_clauses);
				result_string = ResultListManager::getStringValues(result);
			}
			catch (const char* msg) {
				Logger::WriteMessage(msg);
			}
			STRING_RESULT correct_result = "FALSE";

			Logger::WriteMessage(result_string.c_str());
			Assert::IsTrue(result_string.compare(correct_result) == 0);
		}

		/*=================================  PKB 4: Next =========================================*/
		
		TEST_METHOD(evaluateQuery_SelectS_NextSC_ReturnsS)
		{
			// Query: "call c; stmt s; Select s such that Next(s, c)"
			// Get processed_synonyms and processed clauses
			QueryNode call_node = QueryNode();
			call_node.setSynonymNode({ QuerySynonymType::call }, "c");
			QueryNode stmt_node = QueryNode();
			stmt_node.setSynonymNode({ QuerySynonymType::stmt }, "s");
			std::unordered_map<std::string, QueryNode> processed_synonyms = { {"c", call_node}, {"s", stmt_node} };

			// Select: s
			QueryNode child1 = QueryNode();
			child1.setSynonymNode({ QuerySynonymType::stmt }, "s");

			// such that 
			QueryNode child2 = QueryNode();
			child2.setNodeType({ QueryNodeType::such_that });

			// Next
			QueryNode child_child1 = QueryNode();
			child_child1.setNodeType({ QueryNodeType::next });

			// arg 1: s
			QueryNode child_child_child1 = QueryNode();
			child_child_child1.setSynonymNode({ QuerySynonymType::stmt }, "s");

			// arg 2: c
			QueryNode child_child_child2 = QueryNode();
			child_child_child2.setSynonymNode({ QuerySynonymType::call }, "c");

			// set children, make tree
			QueryNode child_child1_children[] = { child_child_child1, child_child_child2 };
			child_child1.setChildren(child_child1_children, 2);
			QueryNode child2_children[] = { child_child1 };
			child2.setChildren(child2_children, 1);

			QueryNode root = QueryNode();
			root.setNodeType({ QueryNodeType::select });
			QueryNode tuple = QueryNode();
			tuple.setNodeType({ QueryNodeType::tuple });
			QueryNode tuple_children[] = { child1 };
			tuple.setChildren(tuple_children, 1);
			QueryNode root_children[] = { tuple, child2 };
			root.setChildren(root_children, 2);

			QueryNode processed_clauses = root; //stores root node of the tree

			// Evaluate
			QueryEvaluator qe = QueryEvaluator(*pkb4);
			QUERY_RESULT result;
			STRING_RESULT result_string;

			try {
				result = qe.evaluateQuery(processed_synonyms, processed_clauses);
				result_string = ResultListManager::getStringValues(result);
			}
			catch (const char* msg) {
				Logger::WriteMessage(msg);
			}
			STRING_RESULT correct_result = "1, 2, 3";

			Logger::WriteMessage(result_string.c_str());
			Assert::IsTrue(result_string.compare(correct_result) == 0);
		}

		TEST_METHOD(evaluateQuery_SelectC_procName_NextSC_ReturnsS)
		{
			// Query: "call c; stmt s; Select c.procName such that Next(s, c)"
			// Get processed_synonyms and processed clauses
			QueryNode call_node = QueryNode();
			call_node.setSynonymNode({ QuerySynonymType::call }, "c");
			QueryNode stmt_node = QueryNode();
			stmt_node.setSynonymNode({ QuerySynonymType::stmt }, "s");
			std::unordered_map<std::string, QueryNode> processed_synonyms = { {"c", call_node}, {"s", stmt_node} };

			// Select: c.procName
			QueryNode child1 = QueryNode();
			child1.setAttrNode("c", "procName");

			// such that 
			QueryNode child2 = QueryNode();
			child2.setNodeType({ QueryNodeType::such_that });

			// Next
			QueryNode child_child1 = QueryNode();
			child_child1.setNodeType({ QueryNodeType::next });

			// arg 1: s
			QueryNode child_child_child1 = QueryNode();
			child_child_child1.setSynonymNode({ QuerySynonymType::stmt }, "s");

			// arg 2: c
			QueryNode child_child_child2 = QueryNode();
			child_child_child2.setSynonymNode({ QuerySynonymType::call }, "c");

			// set children, make tree
			QueryNode child_child1_children[] = { child_child_child1, child_child_child2 };
			child_child1.setChildren(child_child1_children, 2);
			QueryNode child2_children[] = { child_child1 };
			child2.setChildren(child2_children, 1);

			QueryNode root = QueryNode();
			root.setNodeType({ QueryNodeType::select });
			QueryNode tuple = QueryNode();
			tuple.setNodeType({ QueryNodeType::tuple });
			QueryNode tuple_children[] = { child1 };
			tuple.setChildren(tuple_children, 1);
			QueryNode root_children[] = { tuple, child2 };
			root.setChildren(root_children, 2);

			QueryNode processed_clauses = root; //stores root node of the tree

			// Evaluate
			QueryEvaluator qe = QueryEvaluator(*pkb4);
			QUERY_RESULT result;
			STRING_RESULT result_string;

			try {
				result = qe.evaluateQuery(processed_synonyms, processed_clauses);
				result_string = ResultListManager::getStringValues(result);
			}
			catch (const char* msg) {
				Logger::WriteMessage(msg);
			}
			STRING_RESULT correct_result = "afternoon, evening, night";

			Logger::WriteMessage(result_string.c_str());
			Assert::IsTrue(result_string.compare(correct_result) == 0);
		}
		
		TEST_METHOD(evaluateQuery_Select_SC_NextSC_ReturnsS)
		{
			// Query: "call c; stmt s; Select <s, c> such that Next(s, c)"
			// Get processed_synonyms and processed clauses
			QueryNode call_node = QueryNode();
			call_node.setSynonymNode({ QuerySynonymType::call }, "c");
			QueryNode stmt_node = QueryNode();
			stmt_node.setSynonymNode({ QuerySynonymType::stmt }, "s");
			std::unordered_map<std::string, QueryNode> processed_synonyms = { {"c", call_node}, {"s", stmt_node} };

			// Select: <s, c>
			QueryNode child1 = QueryNode();
			child1.setSynonymNode({ QuerySynonymType::stmt }, "s");
			QueryNode child1_2 = QueryNode();
			child1_2.setSynonymNode({ QuerySynonymType::call }, "c");

			// such that 
			QueryNode child2 = QueryNode();
			child2.setNodeType({ QueryNodeType::such_that });

			// Next
			QueryNode child_child1 = QueryNode();
			child_child1.setNodeType({ QueryNodeType::next });

			// arg 1: s
			QueryNode child_child_child1 = QueryNode();
			child_child_child1.setSynonymNode({ QuerySynonymType::stmt }, "s");

			// arg 2: c
			QueryNode child_child_child2 = QueryNode();
			child_child_child2.setSynonymNode({ QuerySynonymType::call }, "c");

			// set children, make tree
			QueryNode child_child1_children[] = { child_child_child1, child_child_child2 };
			child_child1.setChildren(child_child1_children, 2);
			QueryNode child2_children[] = { child_child1 };
			child2.setChildren(child2_children, 1);

			QueryNode root = QueryNode();
			root.setNodeType({ QueryNodeType::select });
			QueryNode tuple = QueryNode();
			tuple.setNodeType({ QueryNodeType::tuple });
			QueryNode tuple_children[] = { child1, child1_2 };
			tuple.setChildren(tuple_children, 2);
			QueryNode root_children[] = { tuple, child2 };
			root.setChildren(root_children, 2);

			QueryNode processed_clauses = root; //stores root node of the tree

			// Evaluate
			QueryEvaluator qe = QueryEvaluator(*pkb4);
			QUERY_RESULT result;
			STRING_RESULT result_string;

			try {
				result = qe.evaluateQuery(processed_synonyms, processed_clauses);
				result_string = ResultListManager::getStringValues(result);
			}
			catch (const char* msg) {
				Logger::WriteMessage(msg);
			}
			STRING_RESULT correct_result = "1 2, 2 3, 3 4";

			Logger::WriteMessage(result_string.c_str());
			Assert::IsTrue(result_string.compare(correct_result) == 0);
		}

		TEST_METHOD(evaluateQuery_Select_CS_NextSC_ReturnsCS)
		{
			// Query: "call c; stmt s; Select <c, s> such that Next(s, c)"
			// Get processed_synonyms and processed clauses
			QueryNode call_node = QueryNode();
			call_node.setSynonymNode({ QuerySynonymType::call }, "c");
			QueryNode stmt_node = QueryNode();
			stmt_node.setSynonymNode({ QuerySynonymType::stmt }, "s");
			std::unordered_map<std::string, QueryNode> processed_synonyms = { {"c", call_node}, {"s", stmt_node} };

			// Select: <s, c>
			QueryNode child1 = QueryNode();
			child1.setSynonymNode({ QuerySynonymType::call }, "c");
			QueryNode child1_2 = QueryNode();
			child1_2.setSynonymNode({ QuerySynonymType::stmt }, "s");

			// such that 
			QueryNode child2 = QueryNode();
			child2.setNodeType({ QueryNodeType::such_that });

			// Next
			QueryNode child_child1 = QueryNode();
			child_child1.setNodeType({ QueryNodeType::next });

			// arg 1: s
			QueryNode child_child_child1 = QueryNode();
			child_child_child1.setSynonymNode({ QuerySynonymType::stmt }, "s");

			// arg 2: c
			QueryNode child_child_child2 = QueryNode();
			child_child_child2.setSynonymNode({ QuerySynonymType::call }, "c");

			// set children, make tree
			QueryNode child_child1_children[] = { child_child_child1, child_child_child2 };
			child_child1.setChildren(child_child1_children, 2);
			QueryNode child2_children[] = { child_child1 };
			child2.setChildren(child2_children, 1);

			QueryNode root = QueryNode();
			root.setNodeType({ QueryNodeType::select });
			QueryNode tuple = QueryNode();
			tuple.setNodeType({ QueryNodeType::tuple });
			QueryNode tuple_children[] = { child1, child1_2 };
			tuple.setChildren(tuple_children, 2);
			QueryNode root_children[] = { tuple, child2 };
			root.setChildren(root_children, 2);

			QueryNode processed_clauses = root; //stores root node of the tree

			// Evaluate
			QueryEvaluator qe = QueryEvaluator(*pkb4);
			QUERY_RESULT result;
			STRING_RESULT result_string;

			try {
				result = qe.evaluateQuery(processed_synonyms, processed_clauses);
				result_string = ResultListManager::getStringValues(result);
			}
			catch (const char* msg) {
				Logger::WriteMessage(msg);
			}
			STRING_RESULT correct_result = "2 1, 3 2, 4 3";

			Logger::WriteMessage(result_string.c_str());
			Assert::IsTrue(result_string.compare(correct_result) == 0);
		}

		TEST_METHOD(evaluateQuery_Select_CProcnameS_NextSC_ReturnsCS)
		{
			// Query: "call c; stmt s; Select <c.procName, s> such that Next(s, c)"
			// Get processed_synonyms and processed clauses
			QueryNode call_node = QueryNode();
			call_node.setSynonymNode({ QuerySynonymType::call }, "c");
			QueryNode stmt_node = QueryNode();
			stmt_node.setSynonymNode({ QuerySynonymType::stmt }, "s");
			std::unordered_map<std::string, QueryNode> processed_synonyms = { {"c", call_node}, {"s", stmt_node} };

			// Select: <s, c>
			QueryNode child1 = QueryNode();
			child1.setAttrNode("c", "procName");
			QueryNode child1_2 = QueryNode();
			child1_2.setSynonymNode({ QuerySynonymType::stmt }, "s");

			// such that 
			QueryNode child2 = QueryNode();
			child2.setNodeType({ QueryNodeType::such_that });

			// Next
			QueryNode child_child1 = QueryNode();
			child_child1.setNodeType({ QueryNodeType::next });

			// arg 1: s
			QueryNode child_child_child1 = QueryNode();
			child_child_child1.setSynonymNode({ QuerySynonymType::stmt }, "s");

			// arg 2: c
			QueryNode child_child_child2 = QueryNode();
			child_child_child2.setSynonymNode({ QuerySynonymType::call }, "c");

			// set children, make tree
			QueryNode child_child1_children[] = { child_child_child1, child_child_child2 };
			child_child1.setChildren(child_child1_children, 2);
			QueryNode child2_children[] = { child_child1 };
			child2.setChildren(child2_children, 1);

			QueryNode root = QueryNode();
			root.setNodeType({ QueryNodeType::select });
			QueryNode tuple = QueryNode();
			tuple.setNodeType({ QueryNodeType::tuple });
			QueryNode tuple_children[] = { child1, child1_2 };
			tuple.setChildren(tuple_children, 2);
			QueryNode root_children[] = { tuple, child2 };
			root.setChildren(root_children, 2);

			QueryNode processed_clauses = root; //stores root node of the tree

			// Evaluate
			QueryEvaluator qe = QueryEvaluator(*pkb4);
			QUERY_RESULT result;
			STRING_RESULT result_string;

			try {
				result = qe.evaluateQuery(processed_synonyms, processed_clauses);
				result_string = ResultListManager::getStringValues(result);
			}
			catch (const char* msg) {
				Logger::WriteMessage(msg);
			}
			STRING_RESULT correct_result = "afternoon 1, evening 2, night 3";

			Logger::WriteMessage(result_string.c_str());
			Assert::IsTrue(result_string.compare(correct_result) == 0);
		}

		TEST_METHOD(evaluateQuery_Select_SCV_NextSC_ReturnsSCV)
		{
			// Query: "call c; stmt s; variable v; Select <s, c, v> such that Next(s, c)"
			// Get processed_synonyms and processed clauses
			QueryNode call_node = QueryNode();
			call_node.setSynonymNode({ QuerySynonymType::call }, "c");
			QueryNode stmt_node = QueryNode();
			stmt_node.setSynonymNode({ QuerySynonymType::stmt }, "s");
			QueryNode var_node = QueryNode();
			var_node.setSynonymNode({ QuerySynonymType::variable }, "v");
			std::unordered_map<std::string, QueryNode> processed_synonyms = { {"c", call_node}, {"s", stmt_node}, {"v", var_node} };

			// Select: <s, c, v>
			QueryNode child1 = QueryNode();
			child1.setSynonymNode({ QuerySynonymType::stmt }, "s");
			QueryNode child1_2 = QueryNode();
			child1_2.setSynonymNode({ QuerySynonymType::call }, "c");
			QueryNode child1_3 = QueryNode();
			child1_3.setSynonymNode({ QuerySynonymType::variable }, "v");

			// such that 
			QueryNode child2 = QueryNode();
			child2.setNodeType({ QueryNodeType::such_that });

			// Next
			QueryNode child_child1 = QueryNode();
			child_child1.setNodeType({ QueryNodeType::next });

			// arg 1: s
			QueryNode child_child_child1 = QueryNode();
			child_child_child1.setSynonymNode({ QuerySynonymType::stmt }, "s");

			// arg 2: c
			QueryNode child_child_child2 = QueryNode();
			child_child_child2.setSynonymNode({ QuerySynonymType::call }, "c");

			// set children, make tree
			QueryNode child_child1_children[] = { child_child_child1, child_child_child2 };
			child_child1.setChildren(child_child1_children, 2);
			QueryNode child2_children[] = { child_child1 };
			child2.setChildren(child2_children, 1);

			QueryNode root = QueryNode();
			root.setNodeType({ QueryNodeType::select });
			QueryNode tuple = QueryNode();
			tuple.setNodeType({ QueryNodeType::tuple });
			QueryNode tuple_children[] = { child1, child1_2, child1_3 };
			tuple.setChildren(tuple_children, 3);
			QueryNode root_children[] = { tuple, child2 };
			root.setChildren(root_children, 2);

			QueryNode processed_clauses = root; //stores root node of the tree

			// Evaluate
			QueryEvaluator qe = QueryEvaluator(*pkb4);
			QUERY_RESULT result;
			STRING_RESULT result_string;

			try {
				result = qe.evaluateQuery(processed_synonyms, processed_clauses);
				result_string = ResultListManager::getStringValues(result);
			}
			catch (const char* msg) {
				Logger::WriteMessage(msg);
			}
			STRING_RESULT correct_result = "1 2 newspaper, 1 2 menu, 1 2 pain, 2 3 newspaper, 2 3 menu, 2 3 pain, 3 4 newspaper, 3 4 menu, 3 4 pain";

			Logger::WriteMessage(result_string.c_str());
			Assert::IsTrue(result_string.compare(correct_result) == 0);
		}

		TEST_METHOD(evaluateQuery_Select_SCVvarname_NextSC_ReturnsSCV)
		{
			// Query: "call c; stmt s; variable v; Select <s, c, v.varName> such that Next(s, c)"
			// Get processed_synonyms and processed clauses
			QueryNode call_node = QueryNode();
			call_node.setSynonymNode({ QuerySynonymType::call }, "c");
			QueryNode stmt_node = QueryNode();
			stmt_node.setSynonymNode({ QuerySynonymType::stmt }, "s");
			QueryNode var_node = QueryNode();
			var_node.setSynonymNode({ QuerySynonymType::variable }, "v");
			std::unordered_map<std::string, QueryNode> processed_synonyms = { {"c", call_node}, {"s", stmt_node}, {"v", var_node} };

			// Select: <s, c, v.varName>
			QueryNode child1 = QueryNode();
			child1.setSynonymNode({ QuerySynonymType::stmt }, "s");
			QueryNode child1_2 = QueryNode();
			child1_2.setSynonymNode({ QuerySynonymType::call }, "c");
			QueryNode child1_3 = QueryNode();
			child1_3.setAttrNode("v", "varName");

			// such that 
			QueryNode child2 = QueryNode();
			child2.setNodeType({ QueryNodeType::such_that });

			// Next
			QueryNode child_child1 = QueryNode();
			child_child1.setNodeType({ QueryNodeType::next });

			// arg 1: s
			QueryNode child_child_child1 = QueryNode();
			child_child_child1.setSynonymNode({ QuerySynonymType::stmt }, "s");

			// arg 2: c
			QueryNode child_child_child2 = QueryNode();
			child_child_child2.setSynonymNode({ QuerySynonymType::call }, "c");

			// set children, make tree
			QueryNode child_child1_children[] = { child_child_child1, child_child_child2 };
			child_child1.setChildren(child_child1_children, 2);
			QueryNode child2_children[] = { child_child1 };
			child2.setChildren(child2_children, 1);

			QueryNode root = QueryNode();
			root.setNodeType({ QueryNodeType::select });
			QueryNode tuple = QueryNode();
			tuple.setNodeType({ QueryNodeType::tuple });
			QueryNode tuple_children[] = { child1, child1_2, child1_3 };
			tuple.setChildren(tuple_children, 3);
			QueryNode root_children[] = { tuple, child2 };
			root.setChildren(root_children, 2);

			QueryNode processed_clauses = root; //stores root node of the tree

			// Evaluate
			QueryEvaluator qe = QueryEvaluator(*pkb4);
			QUERY_RESULT result;
			STRING_RESULT result_string;

			try {
				result = qe.evaluateQuery(processed_synonyms, processed_clauses);
				result_string = ResultListManager::getStringValues(result);
			}
			catch (const char* msg) {
				Logger::WriteMessage(msg);
			}
			STRING_RESULT correct_result = "1 2 newspaper, 1 2 menu, 1 2 pain, 2 3 newspaper, 2 3 menu, 2 3 pain, 3 4 newspaper, 3 4 menu, 3 4 pain";

			Logger::WriteMessage(result_string.c_str());
			Assert::IsTrue(result_string.compare(correct_result) == 0);
		}

		TEST_METHOD(evaluateQuery_Selectn_NextNInteger_ReturnsN)
		{
			// Query: "prog_line n; Select n such that Next(n, 3)"
			// Get processed_synonyms and processed clauses
			QueryNode progline_node = QueryNode();
			progline_node.setSynonymNode({ QuerySynonymType::prog_line }, "n");
			std::unordered_map<std::string, QueryNode> processed_synonyms = { {"n", progline_node} };

			// Select: s
			QueryNode child1 = QueryNode();
			child1.setSynonymNode({ QuerySynonymType::prog_line }, "n");

			// such that 
			QueryNode child2 = QueryNode();
			child2.setNodeType({ QueryNodeType::such_that });

			// Next
			QueryNode child_child1 = QueryNode();
			child_child1.setNodeType({ QueryNodeType::next });

			// arg 1: n
			QueryNode child_child_child1 = QueryNode();
			child_child_child1.setSynonymNode({ QuerySynonymType::prog_line }, "n");

			// arg 2: 3
			QueryNode child_child_child2 = QueryNode();
			child_child_child2.setIntegerNode(3);

			// set children, make tree
			QueryNode child_child1_children[] = { child_child_child1, child_child_child2 };
			child_child1.setChildren(child_child1_children, 2);
			QueryNode child2_children[] = { child_child1 };
			child2.setChildren(child2_children, 1);

			QueryNode root = QueryNode();
			root.setNodeType({ QueryNodeType::select });
			QueryNode tuple = QueryNode();
			tuple.setNodeType({ QueryNodeType::tuple });
			QueryNode tuple_children[] = { child1 };
			tuple.setChildren(tuple_children, 1);
			QueryNode root_children[] = { tuple, child2 };
			root.setChildren(root_children, 2);

			QueryNode processed_clauses = root; //stores root node of the tree

			// Evaluate
			QueryEvaluator qe = QueryEvaluator(*pkb4);
			QUERY_RESULT result;
			STRING_RESULT result_string;

			try {
				result = qe.evaluateQuery(processed_synonyms, processed_clauses);
				result_string = ResultListManager::getStringValues(result);
			}
			catch (const char* msg) {
				Logger::WriteMessage(msg);
			}
			STRING_RESULT correct_result = "2";

			Logger::WriteMessage(result_string.c_str());
			Assert::IsTrue(result_string.compare(correct_result) == 0);
		}

		TEST_METHOD(evaluateQuery_Selectn_NextWildcardN_ReturnsN)
		{
			// Query: "prog_line n; Select n such that Next(_, n)"
			// Get processed_synonyms and processed clauses
			QueryNode progline_node = QueryNode();
			progline_node.setSynonymNode({ QuerySynonymType::prog_line }, "n");
			std::unordered_map<std::string, QueryNode> processed_synonyms = { {"n", progline_node} };

			// Select: s
			QueryNode child1 = QueryNode();
			child1.setSynonymNode({ QuerySynonymType::prog_line }, "n");

			// such that 
			QueryNode child2 = QueryNode();
			child2.setNodeType({ QueryNodeType::such_that });

			// Next
			QueryNode child_child1 = QueryNode();
			child_child1.setNodeType({ QueryNodeType::next });

			// arg 1: _
			QueryNode child_child_child1 = QueryNode();
			child_child_child1.setNodeType({ QueryNodeType::wild_card });

			// arg 2: n
			QueryNode child_child_child2 = QueryNode();
			child_child_child2.setSynonymNode({ QuerySynonymType::prog_line }, "n");

			// set children, make tree
			QueryNode child_child1_children[] = { child_child_child1, child_child_child2 };
			child_child1.setChildren(child_child1_children, 2);
			QueryNode child2_children[] = { child_child1 };
			child2.setChildren(child2_children, 1);

			QueryNode root = QueryNode();
			root.setNodeType({ QueryNodeType::select });
			QueryNode tuple = QueryNode();
			tuple.setNodeType({ QueryNodeType::tuple });
			QueryNode tuple_children[] = { child1 };
			tuple.setChildren(tuple_children, 1);
			QueryNode root_children[] = { tuple, child2 };
			root.setChildren(root_children, 2);

			QueryNode processed_clauses = root; //stores root node of the tree

			// Evaluate
			QueryEvaluator qe = QueryEvaluator(*pkb4);
			QUERY_RESULT result;
			STRING_RESULT result_string;

			try {
				result = qe.evaluateQuery(processed_synonyms, processed_clauses);
				result_string = ResultListManager::getStringValues(result);
			}
			catch (const char* msg) {
				Logger::WriteMessage(msg);
			}
			STRING_RESULT correct_result = "2, 3, 4";

			Logger::WriteMessage(result_string.c_str());
			Assert::IsTrue(result_string.compare(correct_result) == 0);
		}

		TEST_METHOD(evaluateQuery_SelectBOOLEAN_NextIntegerInteger_ReturnsTrue)
		{
			// Query: "Select BOOLEAN such that Next(2, 3)"
			// Get processed_synonyms and processed clauses
			std::unordered_map<std::string, QueryNode> processed_synonyms = {};

			// Select: BOOLEAN
			QueryNode child1 = QueryNode();
			child1.setBooleanNode();

			// such that 
			QueryNode child2 = QueryNode();
			child2.setNodeType({ QueryNodeType::such_that });

			// Next
			QueryNode child_child1 = QueryNode();
			child_child1.setNodeType({ QueryNodeType::next });

			// arg 1: n
			QueryNode child_child_child1 = QueryNode();
			child_child_child1.setIntegerNode(2);

			// arg 2: 3
			QueryNode child_child_child2 = QueryNode();
			child_child_child2.setIntegerNode(3);

			// set children, make tree
			QueryNode child_child1_children[] = { child_child_child1, child_child_child2 };
			child_child1.setChildren(child_child1_children, 2);
			QueryNode child2_children[] = { child_child1 };
			child2.setChildren(child2_children, 1);

			QueryNode root = QueryNode();
			root.setNodeType({ QueryNodeType::select });
			QueryNode root_children[] = { child1, child2 };
			root.setChildren(root_children, 2);

			QueryNode processed_clauses = root; //stores root node of the tree

			// Evaluate
			QueryEvaluator qe = QueryEvaluator(*pkb4);
			QUERY_RESULT result;
			STRING_RESULT result_string;

			try {
				result = qe.evaluateQuery(processed_synonyms, processed_clauses);
				result_string = ResultListManager::getStringValues(result);
			}
			catch (const char* msg) {
				Logger::WriteMessage(msg);
			}
			STRING_RESULT correct_result = "TRUE";

			Logger::WriteMessage(result_string.c_str());
			Assert::IsTrue(result_string.compare(correct_result) == 0);
		}

		TEST_METHOD(evaluateQuery_SelectBOOLEAN_NextIntegerInteger_ReturnsFalse)
		{
			// Query: "Select BOOLEAN such that Next(1, 3)"
			// Get processed_synonyms and processed clauses
			std::unordered_map<std::string, QueryNode> processed_synonyms = {};

			// Select: BOOLEAN
			QueryNode child1 = QueryNode();
			child1.setBooleanNode();

			// such that 
			QueryNode child2 = QueryNode();
			child2.setNodeType({ QueryNodeType::such_that });

			// Next
			QueryNode child_child1 = QueryNode();
			child_child1.setNodeType({ QueryNodeType::next });

			// arg 1: n
			QueryNode child_child_child1 = QueryNode();
			child_child_child1.setIntegerNode(1);

			// arg 2: 3
			QueryNode child_child_child2 = QueryNode();
			child_child_child2.setIntegerNode(3);

			// set children, make tree
			QueryNode child_child1_children[] = { child_child_child1, child_child_child2 };
			child_child1.setChildren(child_child1_children, 2);
			QueryNode child2_children[] = { child_child1 };
			child2.setChildren(child2_children, 1);

			QueryNode root = QueryNode();
			root.setNodeType({ QueryNodeType::select });
			QueryNode root_children[] = { child1, child2 };
			root.setChildren(root_children, 2);

			QueryNode processed_clauses = root; //stores root node of the tree

			// Evaluate
			QueryEvaluator qe = QueryEvaluator(*pkb4);
			QUERY_RESULT result;
			STRING_RESULT result_string;

			try {
				result = qe.evaluateQuery(processed_synonyms, processed_clauses);
				result_string = ResultListManager::getStringValues(result);
			}
			catch (const char* msg) {
				Logger::WriteMessage(msg);
			}
			STRING_RESULT correct_result = "FALSE";

			Logger::WriteMessage(result_string.c_str());
			Assert::IsTrue(result_string.compare(correct_result) == 0);
		}

		/*=================================  PKB 4: NextT =========================================*/
		TEST_METHOD(evaluateQuery_SelectS_NextTNS_ReturnsS)
		{
			// Query: "prog_line n; stmt s; Select s such that NextT(n, s)"
			// Get processed_synonyms and processed clauses
			QueryNode progline_node = QueryNode();
			progline_node.setSynonymNode({ QuerySynonymType::prog_line }, "n");
			QueryNode stmt_node = QueryNode();
			stmt_node.setSynonymNode({ QuerySynonymType::stmt }, "s");
			std::unordered_map<std::string, QueryNode> processed_synonyms = { {"n", progline_node}, {"s", stmt_node} };

			// Select: s
			QueryNode child1 = QueryNode();
			child1.setSynonymNode({ QuerySynonymType::stmt }, "s");

			// such that 
			QueryNode child2 = QueryNode();
			child2.setNodeType({ QueryNodeType::such_that });

			// Next
			QueryNode child_child1 = QueryNode();
			child_child1.setNodeType({ QueryNodeType::nextT });

			// arg 1: n
			QueryNode child_child_child1 = QueryNode();
			child_child_child1.setSynonymNode({ QuerySynonymType::prog_line }, "n");

			// arg 2: s
			QueryNode child_child_child2 = QueryNode();
			child_child_child2.setSynonymNode({ QuerySynonymType::stmt }, "s");

			// set children, make tree
			QueryNode child_child1_children[] = { child_child_child1, child_child_child2 };
			child_child1.setChildren(child_child1_children, 2);
			QueryNode child2_children[] = { child_child1 };
			child2.setChildren(child2_children, 1);

			QueryNode root = QueryNode();
			root.setNodeType({ QueryNodeType::select });
			QueryNode tuple = QueryNode();
			tuple.setNodeType({ QueryNodeType::tuple });
			QueryNode tuple_children[] = { child1 };
			tuple.setChildren(tuple_children, 1);
			QueryNode root_children[] = { tuple, child2 };
			root.setChildren(root_children, 2);

			QueryNode processed_clauses = root; //stores root node of the tree

			// Evaluate
			QueryEvaluator qe = QueryEvaluator(*pkb4);
			QUERY_RESULT result;
			STRING_RESULT result_string;

			try {
				result = qe.evaluateQuery(processed_synonyms, processed_clauses);
				result_string = ResultListManager::getStringValues(result);
			}
			catch (const char* msg) {
				Logger::WriteMessage(msg);
			}
			STRING_RESULT correct_result = "2, 3, 4, 3, 4, 4";

			Logger::WriteMessage(result_string.c_str());
			Assert::IsTrue(result_string.compare(correct_result) == 0);
		}

		TEST_METHOD(evaluateQuery_SelectS_stmtNum_NextTNS_ReturnsS)
		{
			// Query: "prog_line n; stmt s; Select s.stmt# such that NextT(n, s)"
			// Get processed_synonyms and processed clauses
			QueryNode progline_node = QueryNode();
			progline_node.setSynonymNode({ QuerySynonymType::prog_line }, "n");
			QueryNode stmt_node = QueryNode();
			stmt_node.setSynonymNode({ QuerySynonymType::stmt }, "s");
			std::unordered_map<std::string, QueryNode> processed_synonyms = { {"n", progline_node}, {"s", stmt_node} };

			// Select: s
			QueryNode child1 = QueryNode();
			child1.setAttrNode("s", "stmt#");

			// such that 
			QueryNode child2 = QueryNode();
			child2.setNodeType({ QueryNodeType::such_that });

			// Next
			QueryNode child_child1 = QueryNode();
			child_child1.setNodeType({ QueryNodeType::nextT });

			// arg 1: n
			QueryNode child_child_child1 = QueryNode();
			child_child_child1.setSynonymNode({ QuerySynonymType::prog_line }, "n");

			// arg 2: s
			QueryNode child_child_child2 = QueryNode();
			child_child_child2.setSynonymNode({ QuerySynonymType::stmt }, "s");

			// set children, make tree
			QueryNode child_child1_children[] = { child_child_child1, child_child_child2 };
			child_child1.setChildren(child_child1_children, 2);
			QueryNode child2_children[] = { child_child1 };
			child2.setChildren(child2_children, 1);

			QueryNode root = QueryNode();
			root.setNodeType({ QueryNodeType::select });
			QueryNode tuple = QueryNode();
			tuple.setNodeType({ QueryNodeType::tuple });
			QueryNode tuple_children[] = { child1 };
			tuple.setChildren(tuple_children, 1);
			QueryNode root_children[] = { tuple, child2 };
			root.setChildren(root_children, 2);

			QueryNode processed_clauses = root; //stores root node of the tree

			// Evaluate
			QueryEvaluator qe = QueryEvaluator(*pkb4);
			QUERY_RESULT result;
			STRING_RESULT result_string;

			try {
				result = qe.evaluateQuery(processed_synonyms, processed_clauses);
				result_string = ResultListManager::getStringValues(result);
			}
			catch (const char* msg) {
				Logger::WriteMessage(msg);
			}
			STRING_RESULT correct_result = "2, 3, 4, 3, 4, 4";

			Logger::WriteMessage(result_string.c_str());
			Assert::IsTrue(result_string.compare(correct_result) == 0);
		}

		TEST_METHOD(evaluateQuery_SelectS_stmtNum_NextTNS_ReturnsEmpty)
		{
			// Query: "prog_line n; stmt s; Select s.procName such that NextT(n, s)"
			// Get processed_synonyms and processed clauses
			QueryNode progline_node = QueryNode();
			progline_node.setSynonymNode({ QuerySynonymType::prog_line }, "n");
			QueryNode stmt_node = QueryNode();
			stmt_node.setSynonymNode({ QuerySynonymType::stmt }, "s");
			std::unordered_map<std::string, QueryNode> processed_synonyms = { {"n", progline_node}, {"s", stmt_node} };

			// Select: s
			QueryNode child1 = QueryNode();
			child1.setAttrNode("s", "procName");

			// such that 
			QueryNode child2 = QueryNode();
			child2.setNodeType({ QueryNodeType::such_that });

			// Next
			QueryNode child_child1 = QueryNode();
			child_child1.setNodeType({ QueryNodeType::nextT });

			// arg 1: n
			QueryNode child_child_child1 = QueryNode();
			child_child_child1.setSynonymNode({ QuerySynonymType::prog_line }, "n");

			// arg 2: s
			QueryNode child_child_child2 = QueryNode();
			child_child_child2.setSynonymNode({ QuerySynonymType::stmt }, "s");

			// set children, make tree
			QueryNode child_child1_children[] = { child_child_child1, child_child_child2 };
			child_child1.setChildren(child_child1_children, 2);
			QueryNode child2_children[] = { child_child1 };
			child2.setChildren(child2_children, 1);

			QueryNode root = QueryNode();
			root.setNodeType({ QueryNodeType::select });
			QueryNode tuple = QueryNode();
			tuple.setNodeType({ QueryNodeType::tuple });
			QueryNode tuple_children[] = { child1 };
			tuple.setChildren(tuple_children, 1);
			QueryNode root_children[] = { tuple, child2 };
			root.setChildren(root_children, 2);

			QueryNode processed_clauses = root; //stores root node of the tree

			// Evaluate
			QueryEvaluator qe = QueryEvaluator(*pkb4);
			QUERY_RESULT result;
			STRING_RESULT result_string;

			try {
				result = qe.evaluateQuery(processed_synonyms, processed_clauses);
				result_string = ResultListManager::getStringValues(result);
			}
			catch (const char* msg) {
				Logger::WriteMessage(msg);
			}
			STRING_RESULT correct_result = "";

			Logger::WriteMessage(result_string.c_str());
			Assert::IsTrue(result_string.compare(correct_result) == 0);
		}

		TEST_METHOD(evaluateQuery_SelectN_NextTNInteger_ReturnsN)
		{
			// Query: "prog_line n; Select n such that NextT(n, 4)"
			// Get processed_synonyms and processed clauses
			QueryNode progline_node = QueryNode();
			progline_node.setSynonymNode({ QuerySynonymType::prog_line }, "n");
			std::unordered_map<std::string, QueryNode> processed_synonyms = { {"n", progline_node} };

			// Select: n
			QueryNode child1 = QueryNode();
			child1.setSynonymNode({ QuerySynonymType::prog_line }, "n");

			// such that 
			QueryNode child2 = QueryNode();
			child2.setNodeType({ QueryNodeType::such_that });

			// Next
			QueryNode child_child1 = QueryNode();
			child_child1.setNodeType({ QueryNodeType::nextT });

			// arg 1: n
			QueryNode child_child_child1 = QueryNode();
			child_child_child1.setSynonymNode({ QuerySynonymType::prog_line }, "n");

			// arg 2: 4
			QueryNode child_child_child2 = QueryNode();
			child_child_child2.setIntegerNode(4);

			// set children, make tree
			QueryNode child_child1_children[] = { child_child_child1, child_child_child2 };
			child_child1.setChildren(child_child1_children, 2);
			QueryNode child2_children[] = { child_child1 };
			child2.setChildren(child2_children, 1);

			QueryNode root = QueryNode();
			root.setNodeType({ QueryNodeType::select });
			QueryNode tuple = QueryNode();
			tuple.setNodeType({ QueryNodeType::tuple });
			QueryNode tuple_children[] = { child1 };
			tuple.setChildren(tuple_children, 1);
			QueryNode root_children[] = { tuple, child2 };
			root.setChildren(root_children, 2);

			QueryNode processed_clauses = root; //stores root node of the tree

			// Evaluate
			QueryEvaluator qe = QueryEvaluator(*pkb4);
			QUERY_RESULT result;
			STRING_RESULT result_string;

			try {
				result = qe.evaluateQuery(processed_synonyms, processed_clauses);
				result_string = ResultListManager::getStringValues(result);
			}
			catch (const char* msg) {
				Logger::WriteMessage(msg);
			}
			STRING_RESULT correct_result = "1, 2, 3";

			Logger::WriteMessage(result_string.c_str());
			Assert::IsTrue(result_string.compare(correct_result) == 0);
		}

		TEST_METHOD(evaluateQuery_SelectBOOLEAN_NextTIntegerInteger_ReturnsTrue)
		{
			// Query: "Select BOOLEAN such that Next(1, 3)"
			// Get processed_synonyms and processed clauses
			std::unordered_map<std::string, QueryNode> processed_synonyms = {};

			// Select: BOOLEAN
			QueryNode child1 = QueryNode();
			child1.setBooleanNode();

			// such that 
			QueryNode child2 = QueryNode();
			child2.setNodeType({ QueryNodeType::such_that });

			// NextT
			QueryNode child_child1 = QueryNode();
			child_child1.setNodeType({ QueryNodeType::nextT });

			// arg 1: 1
			QueryNode child_child_child1 = QueryNode();
			child_child_child1.setIntegerNode(1);

			// arg 2: 3
			QueryNode child_child_child2 = QueryNode();
			child_child_child2.setIntegerNode(3);

			// set children, make tree
			QueryNode child_child1_children[] = { child_child_child1, child_child_child2 };
			child_child1.setChildren(child_child1_children, 2);
			QueryNode child2_children[] = { child_child1 };
			child2.setChildren(child2_children, 1);

			QueryNode root = QueryNode();
			root.setNodeType({ QueryNodeType::select });
			QueryNode root_children[] = { child1, child2 };
			root.setChildren(root_children, 2);

			QueryNode processed_clauses = root; //stores root node of the tree

			// Evaluate
			QueryEvaluator qe = QueryEvaluator(*pkb4);
			QUERY_RESULT result;
			STRING_RESULT result_string;

			try {
				result = qe.evaluateQuery(processed_synonyms, processed_clauses);
				result_string = ResultListManager::getStringValues(result);
			}
			catch (const char* msg) {
				Logger::WriteMessage(msg);
			}
			STRING_RESULT correct_result = "TRUE";

			Logger::WriteMessage(result_string.c_str());
			Assert::IsTrue(result_string.compare(correct_result) == 0);
		}

		TEST_METHOD(evaluateQuery_SelectBOOLEAN_NextTIntegerInteger_ReturnsFalse)
		{
			// Query: "Select BOOLEAN such that Next(3, 1)"
			// Get processed_synonyms and processed clauses
			std::unordered_map<std::string, QueryNode> processed_synonyms = {};

			// Select: BOOLEAN
			QueryNode child1 = QueryNode();
			child1.setBooleanNode();

			// such that 
			QueryNode child2 = QueryNode();
			child2.setNodeType({ QueryNodeType::such_that });

			// NextT
			QueryNode child_child1 = QueryNode();
			child_child1.setNodeType({ QueryNodeType::nextT });

			// arg 1: 3
			QueryNode child_child_child1 = QueryNode();
			child_child_child1.setIntegerNode(3);

			// arg 2: 1
			QueryNode child_child_child2 = QueryNode();
			child_child_child2.setIntegerNode(1);

			// set children, make tree
			QueryNode child_child1_children[] = { child_child_child1, child_child_child2 };
			child_child1.setChildren(child_child1_children, 2);
			QueryNode child2_children[] = { child_child1 };
			child2.setChildren(child2_children, 1);

			QueryNode root = QueryNode();
			root.setNodeType({ QueryNodeType::select });
			QueryNode root_children[] = { child1, child2 };
			root.setChildren(root_children, 2);

			QueryNode processed_clauses = root; //stores root node of the tree

			// Evaluate
			QueryEvaluator qe = QueryEvaluator(*pkb4);
			QUERY_RESULT result;
			STRING_RESULT result_string;

			try {
				result = qe.evaluateQuery(processed_synonyms, processed_clauses);
				result_string = ResultListManager::getStringValues(result);
			}
			catch (const char* msg) {
				Logger::WriteMessage(msg);
			}
			STRING_RESULT correct_result = "FALSE";

			Logger::WriteMessage(result_string.c_str());
			Assert::IsTrue(result_string.compare(correct_result) == 0);
		}

		TEST_METHOD(evaluateQuery_Select_SN_NextTNS_ReturnsS)
		{
			// Query: "prog_line n; stmt s; Select <n, s> such that NextT(n, s)" 
			// Get processed_synonyms and processed clauses
			QueryNode progline_node = QueryNode();
			progline_node.setSynonymNode({ QuerySynonymType::prog_line }, "n");
			QueryNode stmt_node = QueryNode();
			stmt_node.setSynonymNode({ QuerySynonymType::stmt }, "s");
			std::unordered_map<std::string, QueryNode> processed_synonyms = { {"n", progline_node}, {"s", stmt_node} };

			// Select: <n, s>
			QueryNode child1 = QueryNode();
			child1.setSynonymNode({ QuerySynonymType::prog_line }, "n");
			QueryNode child1_2 = QueryNode();
			child1_2.setSynonymNode({ QuerySynonymType::stmt }, "s");

			// such that 
			QueryNode child2 = QueryNode();
			child2.setNodeType({ QueryNodeType::such_that });

			// Next
			QueryNode child_child1 = QueryNode();
			child_child1.setNodeType({ QueryNodeType::nextT });

			// arg 1: n
			QueryNode child_child_child1 = QueryNode();
			child_child_child1.setSynonymNode({ QuerySynonymType::prog_line }, "n");

			// arg 2: s
			QueryNode child_child_child2 = QueryNode();
			child_child_child2.setSynonymNode({ QuerySynonymType::stmt }, "s");

			// set children, make tree
			QueryNode child_child1_children[] = { child_child_child1, child_child_child2 };
			child_child1.setChildren(child_child1_children, 2);
			QueryNode child2_children[] = { child_child1 };
			child2.setChildren(child2_children, 1);

			QueryNode root = QueryNode();
			root.setNodeType({ QueryNodeType::select });
			QueryNode tuple = QueryNode();
			tuple.setNodeType({ QueryNodeType::tuple });
			QueryNode tuple_children[] = { child1, child1_2};
			tuple.setChildren(tuple_children, 2);
			QueryNode root_children[] = { tuple, child2 };
			root.setChildren(root_children, 2);

			QueryNode processed_clauses = root; //stores root node of the tree

			// Evaluate
			QueryEvaluator qe = QueryEvaluator(*pkb4);
			QUERY_RESULT result;
			STRING_RESULT result_string;

			try {
				result = qe.evaluateQuery(processed_synonyms, processed_clauses);
				result_string = ResultListManager::getStringValues(result);
			}
			catch (const char* msg) {
				Logger::WriteMessage(msg);
			}
			STRING_RESULT correct_result = "1 2, 1 3, 1 4, 2 3, 2 4, 3 4";

			Logger::WriteMessage(result_string.c_str());
			Assert::IsTrue(result_string.compare(correct_result) == 0);
		}

		/*=================================  PKB 4: With =========================================*/
		TEST_METHOD(evaluateQuery_SelectS_WithN3_ReturnsN)
		{
			// Query: "prog_line n; stmt s; Select s such that with s.stmt# = 3"
			// Get processed_synonyms and processed clauses
			QueryNode progline_node = QueryNode();
			progline_node.setSynonymNode({ QuerySynonymType::prog_line }, "n");
			QueryNode stmt_node = QueryNode();
			stmt_node.setSynonymNode({ QuerySynonymType::stmt }, "s");
			std::unordered_map<std::string, QueryNode> processed_synonyms = { {"n", progline_node}, {"s", stmt_node} };

			// Select: s.stmt#
			QueryNode child1 = QueryNode();
			child1.setAttrNode("s", "stmt#");

			// with
			QueryNode child2 = QueryNode();
			child2.setNodeType({ QueryNodeType::with });

			// arg 1: s.stmt#
			QueryNode child_child1 = QueryNode();
			child_child1.setAttrNode("s", "stmt#");

			// arg 2: 3
			QueryNode child_child2 = QueryNode();
			child_child2.setIntegerNode(3);

			// set children, make tree
			QueryNode child2_children[] = { child_child1, child_child2 };
			child2.setChildren(child2_children, 2);

			QueryNode root = QueryNode();
			root.setNodeType({ QueryNodeType::select });
			QueryNode tuple = QueryNode();
			tuple.setNodeType({ QueryNodeType::tuple });
			QueryNode tuple_children[] = { child1 };
			tuple.setChildren(tuple_children, 1);
			QueryNode root_children[] = { tuple, child2 };
			root.setChildren(root_children, 2);

			QueryNode processed_clauses = root; //stores root node of the tree

			// Evaluate
			QueryEvaluator qe = QueryEvaluator(*pkb4);
			QUERY_RESULT result;
			STRING_RESULT result_string;

			try {
				result = qe.evaluateQuery(processed_synonyms, processed_clauses);
				result_string = ResultListManager::getStringValues(result);
			}
			catch (const char* msg) {
				Logger::WriteMessage(msg);
			}
			STRING_RESULT correct_result = "3";

			Logger::WriteMessage(result_string.c_str());
			Assert::IsTrue(result_string.compare(correct_result) == 0);
		}

		TEST_METHOD(evaluateQuery_SelectS_With3N_ReturnsN)
		{
			// Query: "prog_line n; stmt s; Select s such that with 3 = s.stmt#"
			// Get processed_synonyms and processed clauses
			QueryNode progline_node = QueryNode();
			progline_node.setSynonymNode({ QuerySynonymType::prog_line }, "n");
			QueryNode stmt_node = QueryNode();
			stmt_node.setSynonymNode({ QuerySynonymType::stmt }, "s");
			std::unordered_map<std::string, QueryNode> processed_synonyms = { {"n", progline_node}, {"s", stmt_node} };

			// Select: s.stmt#
			QueryNode child1 = QueryNode();
			child1.setAttrNode("s", "stmt#");

			// with
			QueryNode child2 = QueryNode();
			child2.setNodeType({ QueryNodeType::with });

			// arg 1: 3
			QueryNode child_child1 = QueryNode();
			child_child1.setIntegerNode(3);

			// arg 2: s.stmt#
			QueryNode child_child2 = QueryNode();
			child_child2.setAttrNode("s", "stmt#");

			// set children, make tree
			QueryNode child2_children[] = { child_child1, child_child2 };
			child2.setChildren(child2_children, 2);

			QueryNode root = QueryNode();
			root.setNodeType({ QueryNodeType::select });
			QueryNode tuple = QueryNode();
			tuple.setNodeType({ QueryNodeType::tuple });
			QueryNode tuple_children[] = { child1 };
			tuple.setChildren(tuple_children, 1);
			QueryNode root_children[] = { tuple, child2 };
			root.setChildren(root_children, 2);

			QueryNode processed_clauses = root; //stores root node of the tree

			// Evaluate
			QueryEvaluator qe = QueryEvaluator(*pkb4);
			QUERY_RESULT result;
			STRING_RESULT result_string;

			try {
				result = qe.evaluateQuery(processed_synonyms, processed_clauses);
				result_string = ResultListManager::getStringValues(result);
			}
			catch (const char* msg) {
				Logger::WriteMessage(msg);
			}
			STRING_RESULT correct_result = "3";

			Logger::WriteMessage(result_string.c_str());
			Assert::IsTrue(result_string.compare(correct_result) == 0);
		}

		TEST_METHOD(evaluateQuery_SelectP_WithPIdent_ReturnsP)
		{
			// Query: "procedure p; stmt s; Select p such that with p.procName = night"
			// Get processed_synonyms and processed clauses
			QueryNode progline_node = QueryNode();
			progline_node.setSynonymNode({ QuerySynonymType::procedure }, "p");
			QueryNode stmt_node = QueryNode();
			stmt_node.setSynonymNode({ QuerySynonymType::stmt }, "s");
			std::unordered_map<std::string, QueryNode> processed_synonyms = { {"p", progline_node}, {"s", stmt_node} };

			// Select: p
			QueryNode child1 = QueryNode();
			child1.setSynonymNode({ QuerySynonymType::procedure }, "p");

			// with
			QueryNode child2 = QueryNode();
			child2.setNodeType({ QueryNodeType::with });

			// arg 1: p.procName
			QueryNode child_child1 = QueryNode();
			child_child1.setAttrNode("p", "procName");

			// arg 2: night
			QueryNode child_child2 = QueryNode();
			child_child2.setIdentityNode("night");

			// set children, make tree
			QueryNode child2_children[] = { child_child1, child_child2 };
			child2.setChildren(child2_children, 2);

			QueryNode root = QueryNode();
			root.setNodeType({ QueryNodeType::select });
			QueryNode tuple = QueryNode();
			tuple.setNodeType({ QueryNodeType::tuple });
			QueryNode tuple_children[] = { child1 };
			tuple.setChildren(tuple_children, 1);
			QueryNode root_children[] = { tuple, child2 };
			root.setChildren(root_children, 2);

			QueryNode processed_clauses = root; //stores root node of the tree

			// Evaluate
			QueryEvaluator qe = QueryEvaluator(*pkb4);
			QUERY_RESULT result;
			STRING_RESULT result_string;

			try {
				result = qe.evaluateQuery(processed_synonyms, processed_clauses);
				result_string = ResultListManager::getStringValues(result);
			}
			catch (const char* msg) {
				Logger::WriteMessage(msg);
			}
			STRING_RESULT correct_result = "night";

			Logger::WriteMessage(result_string.c_str());
			Assert::IsTrue(result_string.compare(correct_result) == 0);
		}

		TEST_METHOD(evaluateQuery_SelectN_WithCN_ReturnsEmpty)
		{
			// Query: "constant c; prog_line n; Select n such that with c.value = n"
			// Get processed_synonyms and processed clauses
			QueryNode progline_node = QueryNode();
			progline_node.setSynonymNode({ QuerySynonymType::prog_line }, "n");
			QueryNode stmt_node = QueryNode();
			stmt_node.setSynonymNode({ QuerySynonymType::constant }, "c");
			std::unordered_map<std::string, QueryNode> processed_synonyms = { {"n", progline_node}, {"c", stmt_node} };

			// Select: n
			QueryNode child1 = QueryNode();
			child1.setSynonymNode({ QuerySynonymType::prog_line }, "n");

			// with
			QueryNode child2 = QueryNode();
			child2.setNodeType({ QueryNodeType::with });

			// arg 1: p.procName
			QueryNode child_child1 = QueryNode();
			child_child1.setAttrNode("c", "value");

			// arg 2: night
			QueryNode child_child2 = QueryNode();
			child_child2.setSynonymNode({ QuerySynonymType::prog_line }, "n");

			// set children, make tree
			QueryNode child2_children[] = { child_child1, child_child2 };
			child2.setChildren(child2_children, 2);

			QueryNode root = QueryNode();
			root.setNodeType({ QueryNodeType::select });
			QueryNode tuple = QueryNode();
			tuple.setNodeType({ QueryNodeType::tuple });
			QueryNode tuple_children[] = { child1 };
			tuple.setChildren(tuple_children, 1);
			QueryNode root_children[] = { tuple, child2 };
			root.setChildren(root_children, 2);

			QueryNode processed_clauses = root; //stores root node of the tree

			// Evaluate
			QueryEvaluator qe = QueryEvaluator(*pkb4);
			QUERY_RESULT result;
			STRING_RESULT result_string;

			try {
				result = qe.evaluateQuery(processed_synonyms, processed_clauses);
				result_string = ResultListManager::getStringValues(result);
			}
			catch (const char* msg) {
				Logger::WriteMessage(msg);
			}
			STRING_RESULT correct_result = "";

			Logger::WriteMessage(result_string.c_str());
			Assert::IsTrue(result_string.compare(correct_result) == 0);
		}

		TEST_METHOD(evaluateQuery_SelectP_WithPV_ReturnsP)
		{
			// Query: "procedure p; variable v; Select p such that with p.procName = v.varName"
			// Get processed_synonyms and processed clauses
			QueryNode progline_node = QueryNode();
			progline_node.setSynonymNode({ QuerySynonymType::procedure }, "p");
			QueryNode stmt_node = QueryNode();
			stmt_node.setSynonymNode({ QuerySynonymType::variable }, "v");
			std::unordered_map<std::string, QueryNode> processed_synonyms = { {"p", progline_node}, {"v", stmt_node} };

			// Select: p
			QueryNode child1 = QueryNode();
			child1.setSynonymNode({ QuerySynonymType::procedure }, "p");

			// with
			QueryNode child2 = QueryNode();
			child2.setNodeType({ QueryNodeType::with });

			// arg 1: p.procName
			QueryNode child_child1 = QueryNode();
			child_child1.setAttrNode("p", "procName");

			// arg 2: night
			QueryNode child_child2 = QueryNode();
			child_child2.setAttrNode("v", "varName");

			// set children, make tree
			QueryNode child2_children[] = { child_child1, child_child2 };
			child2.setChildren(child2_children, 2);

			QueryNode root = QueryNode();
			root.setNodeType({ QueryNodeType::select });
			QueryNode tuple = QueryNode();
			tuple.setNodeType({ QueryNodeType::tuple });
			QueryNode tuple_children[] = { child1 };
			tuple.setChildren(tuple_children, 1);
			QueryNode root_children[] = { tuple, child2 };
			root.setChildren(root_children, 2);

			QueryNode processed_clauses = root; //stores root node of the tree

			// Evaluate
			QueryEvaluator qe = QueryEvaluator(*pkb4);
			QUERY_RESULT result;
			STRING_RESULT result_string;

			try {
				result = qe.evaluateQuery(processed_synonyms, processed_clauses);
				result_string = ResultListManager::getStringValues(result);
			}
			catch (const char* msg) {
				Logger::WriteMessage(msg);
			}
			STRING_RESULT correct_result = "";

			Logger::WriteMessage(result_string.c_str());
			Assert::IsTrue(result_string.compare(correct_result) == 0);
		}

		TEST_METHOD(evaluateQuery_SelectS_WithSC_ReturnsS)
		{
			// Query: "stmt s; call c; Select s such that with s.stmt# = c.stmt#"
			// Get processed_synonyms and processed clauses
			QueryNode progline_node = QueryNode();
			progline_node.setSynonymNode({ QuerySynonymType::stmt }, "s");
			QueryNode stmt_node = QueryNode();
			stmt_node.setSynonymNode({ QuerySynonymType::call }, "c");
			std::unordered_map<std::string, QueryNode> processed_synonyms = { {"s", progline_node}, {"c", stmt_node} };

			// Select: p
			QueryNode child1 = QueryNode();
			child1.setSynonymNode({ QuerySynonymType::stmt }, "s");

			// with
			QueryNode child2 = QueryNode();
			child2.setNodeType({ QueryNodeType::with });

			// arg 1: s.stmt#
			QueryNode child_child1 = QueryNode();
			child_child1.setAttrNode("s", "stmt#");

			// arg 2: c.stmt#
			QueryNode child_child2 = QueryNode();
			child_child2.setAttrNode("c", "stmt#");

			// set children, make tree
			QueryNode child2_children[] = { child_child1, child_child2 };
			child2.setChildren(child2_children, 2);

			QueryNode root = QueryNode();
			root.setNodeType({ QueryNodeType::select });
			QueryNode tuple = QueryNode();
			tuple.setNodeType({ QueryNodeType::tuple });
			QueryNode tuple_children[] = { child1 };
			tuple.setChildren(tuple_children, 1);
			QueryNode root_children[] = { tuple, child2 };
			root.setChildren(root_children, 2);

			QueryNode processed_clauses = root; //stores root node of the tree

			// Evaluate
			QueryEvaluator qe = QueryEvaluator(*pkb4);
			QUERY_RESULT result;
			STRING_RESULT result_string;

			try {
				result = qe.evaluateQuery(processed_synonyms, processed_clauses);
				result_string = ResultListManager::getStringValues(result);
			}
			catch (const char* msg) {
				Logger::WriteMessage(msg);
			}
			STRING_RESULT correct_result = "1, 2, 3, 4, 5, 6, 7, 8";

			Logger::WriteMessage(result_string.c_str());
			Assert::IsTrue(result_string.compare(correct_result) == 0);
		}

		TEST_METHOD(evaluateQuery_SelectBoolean_WithIdentIdent_ReturnsTrue)
		{
			// Query: "procedure p; stmt s; Select boolean such that with night = night"
			// Get processed_synonyms and processed clauses
			QueryNode progline_node = QueryNode();
			progline_node.setSynonymNode({ QuerySynonymType::procedure }, "p");
			QueryNode stmt_node = QueryNode();
			stmt_node.setSynonymNode({ QuerySynonymType::stmt }, "s");
			std::unordered_map<std::string, QueryNode> processed_synonyms = { {"p", progline_node}, {"s", stmt_node} };

			// Select: p
			QueryNode child1 = QueryNode();
			child1.setBooleanNode();

			// with
			QueryNode child2 = QueryNode();
			child2.setNodeType({ QueryNodeType::with });

			// arg 1: night
			QueryNode child_child1 = QueryNode();
			child_child1.setIdentityNode("night");

			// arg 2: night
			QueryNode child_child2 = QueryNode();
			child_child2.setIdentityNode("night");

			// set children, make tree
			QueryNode child2_children[] = { child_child1, child_child2 };
			child2.setChildren(child2_children, 2);

			QueryNode root = QueryNode();
			root.setNodeType({ QueryNodeType::select });
			QueryNode root_children[] = { child1, child2 };
			root.setChildren(root_children, 2);

			QueryNode processed_clauses = root; //stores root node of the tree

			// Evaluate
			QueryEvaluator qe = QueryEvaluator(*pkb4);
			QUERY_RESULT result;
			STRING_RESULT result_string;

			try {
				result = qe.evaluateQuery(processed_synonyms, processed_clauses);
				result_string = ResultListManager::getStringValues(result);
			}
			catch (const char* msg) {
				Logger::WriteMessage(msg);
			}
			STRING_RESULT correct_result = "TRUE";

			Logger::WriteMessage(result_string.c_str());
			Assert::IsTrue(result_string.compare(correct_result) == 0);
		}

		TEST_METHOD(evaluateQuery_SelectBoolean_WithIdentIdent_ReturnsFalse)
		{
			// Query: "procedure p; stmt s; Select boolean such that with menu = night"
			// Get processed_synonyms and processed clauses
			QueryNode progline_node = QueryNode();
			progline_node.setSynonymNode({ QuerySynonymType::procedure }, "p");
			QueryNode stmt_node = QueryNode();
			stmt_node.setSynonymNode({ QuerySynonymType::stmt }, "s");
			std::unordered_map<std::string, QueryNode> processed_synonyms = { {"p", progline_node}, {"s", stmt_node} };

			// Select: p
			QueryNode child1 = QueryNode();
			child1.setBooleanNode();

			// with
			QueryNode child2 = QueryNode();
			child2.setNodeType({ QueryNodeType::with });

			// arg 1: menu
			QueryNode child_child1 = QueryNode();
			child_child1.setIdentityNode("menu");

			// arg 2: night
			QueryNode child_child2 = QueryNode();
			child_child2.setIdentityNode("night");

			// set children, make tree
			QueryNode child2_children[] = { child_child1, child_child2 };
			child2.setChildren(child2_children, 2);

			QueryNode root = QueryNode();
			root.setNodeType({ QueryNodeType::select });
			QueryNode root_children[] = { child1, child2 };
			root.setChildren(root_children, 2);

			QueryNode processed_clauses = root; //stores root node of the tree

			// Evaluate
			QueryEvaluator qe = QueryEvaluator(*pkb4);
			QUERY_RESULT result;
			STRING_RESULT result_string;

			try {
				result = qe.evaluateQuery(processed_synonyms, processed_clauses);
				result_string = ResultListManager::getStringValues(result);
			}
			catch (const char* msg) {
				Logger::WriteMessage(msg);
			}
			STRING_RESULT correct_result = "FALSE";

			Logger::WriteMessage(result_string.c_str());
			Assert::IsTrue(result_string.compare(correct_result) == 0);
		}

		TEST_METHOD(evaluateQuery_SelectBoolean_WithIntegerInteger_ReturnsTrue)
		{
			// Query: "procedure p; stmt s; Select boolean such that with 3 = 3"
			// Get processed_synonyms and processed clauses
			QueryNode progline_node = QueryNode();
			progline_node.setSynonymNode({ QuerySynonymType::procedure }, "p");
			QueryNode stmt_node = QueryNode();
			stmt_node.setSynonymNode({ QuerySynonymType::stmt }, "s");
			std::unordered_map<std::string, QueryNode> processed_synonyms = { {"p", progline_node}, {"s", stmt_node} };

			// Select: p
			QueryNode child1 = QueryNode();
			child1.setBooleanNode();

			// with
			QueryNode child2 = QueryNode();
			child2.setNodeType({ QueryNodeType::with });

			// arg 1: 3
			QueryNode child_child1 = QueryNode();
			child_child1.setIntegerNode(3);

			// arg 2: 3
			QueryNode child_child2 = QueryNode();
			child_child2.setIntegerNode(3);

			// set children, make tree
			QueryNode child2_children[] = { child_child1, child_child2 };
			child2.setChildren(child2_children, 2);

			QueryNode root = QueryNode();
			root.setNodeType({ QueryNodeType::select });
			QueryNode root_children[] = { child1, child2 };
			root.setChildren(root_children, 2);

			QueryNode processed_clauses = root; //stores root node of the tree

			// Evaluate
			QueryEvaluator qe = QueryEvaluator(*pkb4);
			QUERY_RESULT result;
			STRING_RESULT result_string;

			try {
				result = qe.evaluateQuery(processed_synonyms, processed_clauses);
				result_string = ResultListManager::getStringValues(result);
			}
			catch (const char* msg) {
				Logger::WriteMessage(msg);
			}
			STRING_RESULT correct_result = "TRUE";

			Logger::WriteMessage(result_string.c_str());
			Assert::IsTrue(result_string.compare(correct_result) == 0);
		}

		TEST_METHOD(evaluateQuery_SelectBoolean_WithIntegerInteger_ReturnsFalse)
		{
			// Query: "procedure p; stmt s; Select boolean such that with 2 = 3"
			// Get processed_synonyms and processed clauses
			QueryNode progline_node = QueryNode();
			progline_node.setSynonymNode({ QuerySynonymType::procedure }, "p");
			QueryNode stmt_node = QueryNode();
			stmt_node.setSynonymNode({ QuerySynonymType::stmt }, "s");
			std::unordered_map<std::string, QueryNode> processed_synonyms = { {"p", progline_node}, {"s", stmt_node} };

			// Select: p
			QueryNode child1 = QueryNode();
			child1.setBooleanNode();

			// with
			QueryNode child2 = QueryNode();
			child2.setNodeType({ QueryNodeType::with });

			// arg 1: 3
			QueryNode child_child1 = QueryNode();
			child_child1.setIntegerNode(2);

			// arg 2: 3
			QueryNode child_child2 = QueryNode();
			child_child2.setIntegerNode(3);

			// set children, make tree
			QueryNode child2_children[] = { child_child1, child_child2 };
			child2.setChildren(child2_children, 2);

			QueryNode root = QueryNode();
			root.setNodeType({ QueryNodeType::select });
			QueryNode root_children[] = { child1, child2 };
			root.setChildren(root_children, 2);

			QueryNode processed_clauses = root; //stores root node of the tree

			// Evaluate
			QueryEvaluator qe = QueryEvaluator(*pkb4);
			QUERY_RESULT result;
			STRING_RESULT result_string;

			try {
				result = qe.evaluateQuery(processed_synonyms, processed_clauses);
				result_string = ResultListManager::getStringValues(result);
			}
			catch (const char* msg) {
				Logger::WriteMessage(msg);
			}
			STRING_RESULT correct_result = "TRUE";

			Logger::WriteMessage(result_string.c_str());
			Assert::IsTrue(result_string.compare(correct_result) == 0);
		}

		TEST_METHOD(evaluateQuery_SelectN_WithIntegerN_ReturnsN)
		{
			// Query: "constant c; prog_line n; Select n such that with 3 = n"
			// Get processed_synonyms and processed clauses
			QueryNode progline_node = QueryNode();
			progline_node.setSynonymNode({ QuerySynonymType::prog_line }, "n");
			QueryNode stmt_node = QueryNode();
			stmt_node.setSynonymNode({ QuerySynonymType::constant }, "c");
			std::unordered_map<std::string, QueryNode> processed_synonyms = { {"n", progline_node}, {"c", stmt_node} };

			// Select: n
			QueryNode child1 = QueryNode();
			child1.setSynonymNode({ QuerySynonymType::prog_line }, "n");

			// with
			QueryNode child2 = QueryNode();
			child2.setNodeType({ QueryNodeType::with });

			// arg 1: 3
			QueryNode child_child1 = QueryNode();
			child_child1.setIntegerNode(3);

			// arg 2: n
			QueryNode child_child2 = QueryNode();
			child_child2.setSynonymNode({ QuerySynonymType::prog_line }, "n");

			// set children, make tree
			QueryNode child2_children[] = { child_child1, child_child2 };
			child2.setChildren(child2_children, 2);

			QueryNode root = QueryNode();
			root.setNodeType({ QueryNodeType::select });
			QueryNode tuple = QueryNode();
			tuple.setNodeType({ QueryNodeType::tuple });
			QueryNode tuple_children[] = { child1 };
			tuple.setChildren(tuple_children, 1);
			QueryNode root_children[] = { tuple, child2 };
			root.setChildren(root_children, 2);

			QueryNode processed_clauses = root; //stores root node of the tree

			// Evaluate
			QueryEvaluator qe = QueryEvaluator(*pkb4);
			QUERY_RESULT result;
			STRING_RESULT result_string;

			try {
				result = qe.evaluateQuery(processed_synonyms, processed_clauses);
				result_string = ResultListManager::getStringValues(result);
			}
			catch (const char* msg) {
				Logger::WriteMessage(msg);
			}
			STRING_RESULT correct_result = "3";

			Logger::WriteMessage(result_string.c_str());
			Assert::IsTrue(result_string.compare(correct_result) == 0);
		}

		TEST_METHOD(evaluateQuery_SelectN_WithIntegerN_ReturnsEmpty)
		{
			// Query: "constant c; prog_line n; Select n such that with 100 = n"
			// Get processed_synonyms and processed clauses
			QueryNode progline_node = QueryNode();
			progline_node.setSynonymNode({ QuerySynonymType::prog_line }, "n");
			QueryNode stmt_node = QueryNode();
			stmt_node.setSynonymNode({ QuerySynonymType::constant }, "c");
			std::unordered_map<std::string, QueryNode> processed_synonyms = { {"n", progline_node}, {"c", stmt_node} };

			// Select: n
			QueryNode child1 = QueryNode();
			child1.setSynonymNode({ QuerySynonymType::prog_line }, "n");

			// with
			QueryNode child2 = QueryNode();
			child2.setNodeType({ QueryNodeType::with });

			// arg 1: 3
			QueryNode child_child1 = QueryNode();
			child_child1.setIntegerNode(100);

			// arg 2: n
			QueryNode child_child2 = QueryNode();
			child_child2.setSynonymNode({ QuerySynonymType::prog_line }, "n");

			// set children, make tree
			QueryNode child2_children[] = { child_child1, child_child2 };
			child2.setChildren(child2_children, 2);

			QueryNode root = QueryNode();
			root.setNodeType({ QueryNodeType::select });
			QueryNode tuple = QueryNode();
			tuple.setNodeType({ QueryNodeType::tuple });
			QueryNode tuple_children[] = { child1 };
			tuple.setChildren(tuple_children, 1);
			QueryNode root_children[] = { tuple, child2 };
			root.setChildren(root_children, 2);

			QueryNode processed_clauses = root; //stores root node of the tree

			// Evaluate
			QueryEvaluator qe = QueryEvaluator(*pkb4);
			QUERY_RESULT result;
			STRING_RESULT result_string;

			try {
				result = qe.evaluateQuery(processed_synonyms, processed_clauses);
				result_string = ResultListManager::getStringValues(result);
			}
			catch (const char* msg) {
				Logger::WriteMessage(msg);
			}
			STRING_RESULT correct_result = "";

			Logger::WriteMessage(result_string.c_str());
			Assert::IsTrue(result_string.compare(correct_result) == 0);
		}

		TEST_METHOD(evaluateQuery_SelectN1_WithN1N2_ReturnsN1)
		{
			// Query: "prog_line n1, n2; Select n1 such that with n1 = n2"
			// Get processed_synonyms and processed clauses
			QueryNode progline_node = QueryNode();
			progline_node.setSynonymNode({ QuerySynonymType::prog_line }, "n1");
			QueryNode stmt_node = QueryNode();
			stmt_node.setSynonymNode({ QuerySynonymType::prog_line }, "n2");
			std::unordered_map<std::string, QueryNode> processed_synonyms = { {"n1", progline_node}, {"n2", stmt_node} };

			// Select: n1
			QueryNode child1 = QueryNode();
			child1.setSynonymNode({ QuerySynonymType::prog_line }, "n1");

			// with
			QueryNode child2 = QueryNode();
			child2.setNodeType({ QueryNodeType::with });

			// arg 1: n1
			QueryNode child_child1 = QueryNode();
			child_child1.setSynonymNode({ QuerySynonymType::prog_line }, "n1");

			// arg 2: n2
			QueryNode child_child2 = QueryNode();
			child_child2.setSynonymNode({ QuerySynonymType::prog_line }, "n2");

			// set children, make tree
			QueryNode child2_children[] = { child_child1, child_child2 };
			child2.setChildren(child2_children, 2);

			QueryNode root = QueryNode();
			root.setNodeType({ QueryNodeType::select });
			QueryNode tuple = QueryNode();
			tuple.setNodeType({ QueryNodeType::tuple });
			QueryNode tuple_children[] = { child1 };
			tuple.setChildren(tuple_children, 1);
			QueryNode root_children[] = { tuple, child2 };
			root.setChildren(root_children, 2);

			QueryNode processed_clauses = root; //stores root node of the tree

			// Evaluate
			QueryEvaluator qe = QueryEvaluator(*pkb4);
			QUERY_RESULT result;
			STRING_RESULT result_string;

			try {
				result = qe.evaluateQuery(processed_synonyms, processed_clauses);
				result_string = ResultListManager::getStringValues(result);
			}
			catch (const char* msg) {
				Logger::WriteMessage(msg);
			}
			STRING_RESULT correct_result = "1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12";

			Logger::WriteMessage(result_string.c_str());
			Assert::IsTrue(result_string.compare(correct_result) == 0);
		}

		TEST_METHOD(evaluateQuery_SelectC_WithC3_WithCV_ReturnsC)
		{
			// Query: "call c1; Select c1 such that with c1.stmt# = 3 and c1.procName = evening"
			// Get processed_synonyms and processed clauses
			QueryNode call_node = QueryNode();
			call_node.setSynonymNode({ QuerySynonymType::call }, "c1");
			std::unordered_map<std::string, QueryNode> processed_synonyms = { {"c1", call_node } };

			// Select: c1
			QueryNode child0 = QueryNode();
			child0.setSynonymNode({ QuerySynonymType::call }, "c1");

			// with
			QueryNode child1 = QueryNode();
			child1.setNodeType({ QueryNodeType::with });

			// arg 1: c1.stmt#
			QueryNode child_child1 = QueryNode();
			child_child1.setAttrNode("c1", "stmt#");

			// arg 2: 3
			QueryNode child_child2 = QueryNode();
			child_child2.setIntegerNode(3);

			// with
			QueryNode child2 = QueryNode();
			child2.setNodeType({ QueryNodeType::with });

			// arg 1: c1.varName
			QueryNode child_child2_1 = QueryNode();
			child_child2_1.setAttrNode("c1", "procName");

			// arg 2: evening
			QueryNode child_child2_2 = QueryNode();
			child_child2_2.setIdentityNode("evening");


			// set children, make tree
			QueryNode child1_children[] = { child_child1, child_child2 };
			child1.setChildren(child1_children, 2);
			
			QueryNode child2_children[] = { child_child2_1, child_child2_2 };
			child2.setChildren(child2_children, 2);

			QueryNode root = QueryNode();
			root.setNodeType({ QueryNodeType::select });
			QueryNode tuple = QueryNode();
			tuple.setNodeType({ QueryNodeType::tuple });
			QueryNode tuple_children[] = { child0 };
			tuple.setChildren(tuple_children, 1);
			QueryNode root_children[] = { tuple, child1, child2};
			root.setChildren(root_children, 3);

			QueryNode processed_clauses = root; //stores root node of the tree

			// Evaluate
			QueryEvaluator qe = QueryEvaluator(*pkb4);
			QUERY_RESULT result;
			STRING_RESULT result_string;

			try {
				result = qe.evaluateQuery(processed_synonyms, processed_clauses);
				result_string = ResultListManager::getStringValues(result);
			}
			catch (const char* msg) {
				Logger::WriteMessage(msg);
			}
			STRING_RESULT correct_result = "3";

			Logger::WriteMessage(result_string.c_str());
			Assert::IsTrue(result_string.compare(correct_result) == 0);
		}

		TEST_METHOD(evaluateQuery_SelectBoolean_WithC3_WithCV_ReturnsC)
		{
			// Query: "call c1; Select BOOLEAN such that with c1.stmt# = 3 and c1.varName = evening"
			// Get processed_synonyms and processed clauses
			QueryNode call_node = QueryNode();
			call_node.setSynonymNode({ QuerySynonymType::call }, "c1");
			std::unordered_map<std::string, QueryNode> processed_synonyms = { {"c1", call_node } };

			// Select: boolean
			QueryNode child0 = QueryNode();
			child0.setBooleanNode();

			// with
			QueryNode child1 = QueryNode();
			child1.setNodeType({ QueryNodeType::with });

			// arg 1: c1.stmt#
			QueryNode child_child1 = QueryNode();
			child_child1.setAttrNode("c1", "stmt#");

			// arg 2: 3
			QueryNode child_child2 = QueryNode();
			child_child2.setIntegerNode(3);

			// with
			QueryNode child2 = QueryNode();
			child2.setNodeType({ QueryNodeType::with });

			// arg 1: c1.stmt#
			QueryNode child_child2_1 = QueryNode();
			child_child2_1.setAttrNode("c1", "stmt#");

			// arg 2: 3
			QueryNode child_child2_2 = QueryNode();
			child_child2_2.setIntegerNode(3);


			// set children, make tree
			QueryNode child1_children[] = { child_child1, child_child2 };
			child1.setChildren(child1_children, 2);

			QueryNode child2_children[] = { child_child2_1, child_child2_2 };
			child2.setChildren(child2_children, 2);

			QueryNode root = QueryNode();
			root.setNodeType({ QueryNodeType::select });
			QueryNode root_children[] = { child0, child1, child2 };
			root.setChildren(root_children, 3);

			QueryNode processed_clauses = root; //stores root node of the tree

			// Evaluate
			QueryEvaluator qe = QueryEvaluator(*pkb4);
			QUERY_RESULT result;
			STRING_RESULT result_string;

			try {
				result = qe.evaluateQuery(processed_synonyms, processed_clauses);
				result_string = ResultListManager::getStringValues(result);
			}
			catch (const char* msg) {
				Logger::WriteMessage(msg);
			}
			STRING_RESULT correct_result = "TRUE";

			Logger::WriteMessage(result_string.c_str());
			Assert::IsTrue(result_string.compare(correct_result) == 0);
		}
	};
}