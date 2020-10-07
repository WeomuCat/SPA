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
		/*
		procedure main {
			print a;
			print b;
			print c;
			print d;
			print e;
		}
		*/
		std::shared_ptr<PKB> pkb; // ohh you can right click and rename it to pkb1! ohh i see!

		/*
		procedure meow {
			a = woof;
			b = a + b + c;
			c = 2 / (b + 3) % d + o * g;
		}
		*/
		std::shared_ptr<PKB> pkb2;

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
			QueryNode child2 = QueryNode();
			child2.setSynonymNode({ QuerySynonymType::variable }, "v");
			QueryNode child1_children[] = { child2 };
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
			QueryNode child2 = QueryNode();
			child2.setSynonymNode({ QuerySynonymType::print }, "pn");
			QueryNode child1_children[] = { child2 };
			child1.setChildren(child1_children, 1);

			QueryNode processed_clauses = child1; //stores root node of the tree

			QueryEvaluator qe = QueryEvaluator(*pkb);
			QUERY_RESULT result = qe.evaluateQuery(processed_synonyms, processed_clauses);
			STRING_RESULT result_string = ResultListManager::getStringValues(result);
			STRING_RESULT correct_result = "1, 2, 3, 4, 5";

			Logger::WriteMessage(result_string.c_str());
			Assert::IsTrue(result_string.compare(correct_result) == 0);
		}
		
		TEST_METHOD(evaluateQuery_SelectS1FollowsS1S2_ReturnsS1)
		{
			// Query: "stmt s1, s2; variable v; Select s1 such that Follows(s1,s2)"
			// Get processed_synonyms and processed clauses
			QueryNode variable_node = QueryNode();
			variable_node.setSynonymNode({ QuerySynonymType::variable }, "v");
			QueryNode s1_node = QueryNode();
			variable_node.setSynonymNode({ QuerySynonymType::stmt }, "s1");
			QueryNode s2_node = QueryNode();
			variable_node.setSynonymNode({ QuerySynonymType::stmt }, "s2");
			std::unordered_map<std::string, QueryNode> processed_synonyms = { {"v", variable_node}, {"s1", s1_node}, {"s2", s2_node} };

			QueryNode child1 = QueryNode();
			child1.setSynonymNode({ QuerySynonymType::stmt }, "s1");
			QueryNode child2 = QueryNode();
			child2.setNodeType({ QueryNodeType::such_that });
			QueryNode child_child1 = QueryNode();
			child_child1.setNodeType({ QueryNodeType::follows});
			QueryNode child_child_child1 = QueryNode();
			child_child_child1.setSynonymNode({ QuerySynonymType::stmt }, "s1");
			QueryNode child_child_child2 = QueryNode();
			child_child_child2.setSynonymNode({ QuerySynonymType::stmt }, "s2");
			QueryNode child_child1_children[] = { child_child_child1, child_child_child2 };
			child_child1.setChildren(child_child1_children, 2);
			QueryNode child2_children[] = {child_child1};
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
			STRING_RESULT correct_result = "1, 2, 3, 4";

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
			QueryNode root_children[] = { child1, child2 };
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
			QueryNode root_children[] = { child1, child2 };
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
			QueryNode root_children[] = { child1, child2 };
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
				Logger::WriteMessage("Evaluate exception caught");
			}
			STRING_RESULT correct_result = "";

			Logger::WriteMessage(result_string.c_str());
			Assert::IsTrue(result_string.compare(correct_result) == 0);
		}

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
				Logger::WriteMessage("Evaluate exception caught");
			}
			STRING_RESULT correct_result = "";

			Logger::WriteMessage(result_string.c_str());
			Assert::IsTrue(result_string.compare(correct_result) == 0);
		}

		TEST_METHOD(evaluateQuery_SelectVFollowsT1_S_ReturnsV)
		{
			// Query: "variable v; Select v such that FollowsT(1,s)"
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
				Logger::WriteMessage("Evaluate exception caught");
			}
			STRING_RESULT correct_result = "a, a, a, a, b, b, b, b, c, c, c, c, d, d, d, d, e, e, e, e";
			
			Logger::WriteMessage(result_string.c_str());
			Assert::IsTrue(result_string.compare(correct_result) == 0);
		}

		TEST_METHOD(evaluateQuery_SelectPnFollowsT1_Pn_Returns2)
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
				Logger::WriteMessage("Evaluate exception caught");
			}
			STRING_RESULT correct_result = "2, 3, 4, 5";

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
				Logger::WriteMessage("Evaluate exception caught");
			}
			STRING_RESULT correct_result = "2, 3, 3, 4, 4, 4, 5, 5, 5, 5";

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
				Logger::WriteMessage("Evaluate exception caught");
			}
			STRING_RESULT correct_result = "a, a, b, b, c, c, d, d, e, e";

			Logger::WriteMessage(result_string.c_str());
			Assert::IsTrue(result_string.compare(correct_result) == 0);
		}

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
			QueryNode root_children[] = { child1, child2 };
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
			child_child_child2.setSynonymNode({QuerySynonymType::print}, "pn");
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
			STRING_RESULT correct_result = "";

			Logger::WriteMessage(result_string.c_str());
			Assert::IsTrue(result_string.compare(correct_result) == 0);
		}

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
			QueryNode root_children[] = { child1, child2 };
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
			QueryNode root_children[] = { child1, child2 };
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
			QueryNode root_children[] = { child1, child2 };
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

		TEST_METHOD(evaluateQuery_SelectVUsesIfs_v_ReturnsEmpty)
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
			QueryNode root_children[] = { child1, child2 };
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

		TEST_METHOD(evaluateQuery_SelectVUsesSP_v_ReturnsV)
		{
			// Query: "variable v; procedure p; Select v such that UsesS(p,v)"
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
			child_child1.setNodeType({ QueryNodeType::usesS });
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
			QueryNode root_children[] = { child1, child2 };
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
			// Query: "variable v; procedure p; Select v such that UsesS(p,v)"
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
			child_child1.setNodeType({ QueryNodeType::usesS });
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
			QueryNode root_children[] = { child1, child2 };
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
			QueryNode root_children[] = { child1, child2 };
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
			// Query: "variable v; procedure p; Select v such that ModifiesS(p,v)"
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
			child_child1.setNodeType({ QueryNodeType::modifiesS });
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
			QueryNode root_children[] = { child1, child2 };
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
			QueryNode root_children[] = { child1, child2 };
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

			child_child_child2.setNodeType({ QueryNodeType::expression });
			child_child_child2.setASTNode(ewoof);

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
			STRING_RESULT correct_result = "1";
			
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
			QueryNode root_children[] = { child1, child2 };
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

			child_child_child2.setNodeType({ QueryNodeType::expression });
			child_child_child2.setASTNode(eb);

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
			QueryNode root_children[] = { child1, child2 };
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

			child_child_child2.setNodeType({ QueryNodeType::expression });
			child_child_child2.setASTNode(expr_node);

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
			STRING_RESULT correct_result = "3";

			Logger::WriteMessage("Result: ");
			Logger::WriteMessage(result_string.c_str());
			Assert::IsTrue(result_string.compare(correct_result) == 0);
		}
	};
}
