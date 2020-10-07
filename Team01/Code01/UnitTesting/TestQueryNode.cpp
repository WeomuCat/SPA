#include "stdafx.h"
#include "CppUnitTest.h"
#include "../source/QueryNode.h"
#include "../source/QueryNodeType.h"
#include "../source/PKB/ASTNode/ConstantNode.h"
#include "../source/PKB/ASTNode/ExpressionNode.h"
#include "../source/PKB/ASTNode/AssignNode.h"

#include <string>
#include <iostream>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{
	TEST_CLASS(TestQueryNode)
	{
	public:

		TEST_METHOD(setNodeType_Select_Success)
		{
			QueryNode query_node;
			QueryNodeType chosen_type = { QueryNodeType::select };
			query_node.setNodeType({ QueryNodeType::select });
			QUERY_NODE_TYPE type = query_node.getNodeType();
			Assert::IsTrue(type == chosen_type);
		}

		TEST_METHOD(setNodeType_Synonym_Success)
		{
			QueryNode query_node;
			QueryNodeType chosen_type = { QueryNodeType::synonym };
			query_node.setNodeType({ QueryNodeType::synonym });
			QUERY_NODE_TYPE type = query_node.getNodeType();
			Assert::IsTrue(type == chosen_type);
		}

		TEST_METHOD(setNodeType_SuchThat_Success)
		{
			QueryNode query_node;
			QueryNodeType chosen_type = { QueryNodeType::such_that };
			query_node.setNodeType({ QueryNodeType::such_that });
			QUERY_NODE_TYPE type = query_node.getNodeType();
			Assert::IsTrue(type == chosen_type);
		}

		TEST_METHOD(setNodeType_Parent_Success)
		{
			QueryNode query_node;
			QueryNodeType chosen_type = { QueryNodeType::parent };
			query_node.setNodeType({ QueryNodeType::parent });
			QUERY_NODE_TYPE type = query_node.getNodeType();
			Assert::IsTrue(type == chosen_type);
		}

		TEST_METHOD(setNodeType_ParentT_Success)
		{
			QueryNode query_node;
			QueryNodeType chosen_type = { QueryNodeType::parentT };
			query_node.setNodeType({ QueryNodeType::parentT });
			QUERY_NODE_TYPE type = query_node.getNodeType();
			Assert::IsTrue(type == chosen_type);
		}

		TEST_METHOD(setNodeType_Follows_Success)
		{
			QueryNode query_node;
			QueryNodeType chosen_type = { QueryNodeType::follows };
			query_node.setNodeType({ QueryNodeType::follows });
			QUERY_NODE_TYPE type = query_node.getNodeType();
			Assert::IsTrue(type == chosen_type);
		}

		TEST_METHOD(setNodeType_FollowsT_Success)
		{
			QueryNode query_node;
			QueryNodeType chosen_type = { QueryNodeType::followsT };
			query_node.setNodeType({ QueryNodeType::followsT });
			QUERY_NODE_TYPE type = query_node.getNodeType();
			Assert::IsTrue(type == chosen_type);
		}

		TEST_METHOD(setNodeType_SelectRegex_Success)
		{
			QueryNode query_node;
			QueryNodeType chosen_type = { QueryNodeType::select };
			query_node.setNodeType({ "select" });
			QUERY_NODE_TYPE type = query_node.getNodeType();
			Assert::IsTrue(type == chosen_type);
		}
		
		TEST_METHOD(setNodeType_SynonymRegex_Success)
		{
			QueryNode query_node;
			QueryNodeType chosen_type = { QueryNodeType::synonym };
			query_node.setNodeType({ "synonym" });
			QUERY_NODE_TYPE type = query_node.getNodeType();
			Assert::IsTrue(type == chosen_type);
		}
		
		TEST_METHOD(setNodeType_SuchThatRegex_Success)
		{
			QueryNode query_node;
			QueryNodeType chosen_type = { QueryNodeType::such_that };
			query_node.setNodeType({ "such that" });
			QUERY_NODE_TYPE type = query_node.getNodeType();
			Assert::IsTrue(type == chosen_type);
		}

		TEST_METHOD(setNodeType_PatternRegex_Success)
		{
			QueryNode query_node;
			QueryNodeType chosen_type = { QueryNodeType::pattern };
			query_node.setNodeType({ "pattern" });
			QUERY_NODE_TYPE type = query_node.getNodeType();
			Assert::IsTrue(type == chosen_type);
		}

		TEST_METHOD(setNodeType_FollowsRegex_Success)
		{
			QueryNode query_node;
			QueryNodeType chosen_type = { QueryNodeType::follows };
			query_node.setNodeType({ "Follows" });
			QUERY_NODE_TYPE type = query_node.getNodeType();
			Assert::IsTrue(type == chosen_type);
		}

		TEST_METHOD(setNodeType_FollowsTRegex_Success)
		{
			QueryNode query_node;
			QueryNodeType chosen_type = { QueryNodeType::followsT };
			query_node.setNodeType({ "Follows*" });
			QUERY_NODE_TYPE type = query_node.getNodeType();
			Assert::IsTrue(type == chosen_type);
		}

		TEST_METHOD(setNodeType_ParentRegex_Success)
		{
			QueryNode query_node;
			QueryNodeType chosen_type = { QueryNodeType::parent };
			query_node.setNodeType({ "Parent" });
			QUERY_NODE_TYPE type = query_node.getNodeType();
			Assert::IsTrue(type == chosen_type);
		}

		TEST_METHOD(setNodeType_ParentTRegex_Success)
		{
			QueryNode query_node;
			QueryNodeType chosen_type = { QueryNodeType::parentT };
			query_node.setNodeType({ "Parent*" });
			QUERY_NODE_TYPE type = query_node.getNodeType();
			Assert::IsTrue(type == chosen_type);
		}

		TEST_METHOD(setNodeType_UsesRegex_Success)
		{
			QueryNode query_node;
			QueryNodeType chosen_type = { QueryNodeType::usesS };
			query_node.setNodeType({ "Uses" });
			QUERY_NODE_TYPE type = query_node.getNodeType();
			Assert::IsTrue(type == chosen_type);
		}

		TEST_METHOD(setNodeType_ModifiesRegex_Success)
		{
			QueryNode query_node;
			QueryNodeType chosen_type = { QueryNodeType::modifiesS };
			query_node.setNodeType({ "Modifies" });
			QUERY_NODE_TYPE type = query_node.getNodeType();
			Assert::IsTrue(type == chosen_type);
		}
		TEST_METHOD(setSynonymNode_AssignA_Success) {
			QueryNode query_node;
			QuerySynonymType chosen_synonym = { QuerySynonymType::assign };
			query_node.setSynonymNode({ QuerySynonymType::assign }, "a1");
			Assert::IsTrue(query_node.getSynonymType() == chosen_synonym);
			Assert::IsTrue(query_node.getString().compare("a1") == 0);
		}

		TEST_METHOD(getInteger_DefaultInteger_Success) {
			QueryNode query_node;
			QuerySynonymType chosen_synonym = { QuerySynonymType::assign };
			query_node.setSynonymNode({ QuerySynonymType::assign }, "a1");
			Assert::IsTrue(query_node.getInteger() == -1);
		}

		TEST_METHOD(getInteger_IntegerContent_Success) {
			QueryNode query_node;
			query_node.setIntegerNode(5);
			Assert::IsTrue(query_node.getInteger() == 5);
		}

		TEST_METHOD(getSetASTNode_ConstantNode_Success) {
			QueryNode query_node;
			std::shared_ptr<ConstantNode> const_node = std::make_shared<ConstantNode>();
			const_node->setValue("5");
			std::shared_ptr<ExpressionNode> expr_node = std::make_shared<ExpressionNode>();
			expr_node->setLeftAstNode(const_node);

			query_node.setASTNode(expr_node);

			AST_NODE node = query_node.getAstNode();
			EXPR_NODE_PTR child_expr_node = std::static_pointer_cast<ExpressionNode>(node);
			AST_NODE_PTR lhs = child_expr_node->getLeftAstNode();
			std::shared_ptr<ConstantNode> child_node = std::static_pointer_cast<ConstantNode>(lhs);


			Assert::IsTrue(child_node->getValue().compare("5") == 0);
		}

		TEST_METHOD(getSetASTNode_VariableNode_Success) {
			QueryNode query_node;
			std::shared_ptr<VariableNode> const_node = std::make_shared<VariableNode>();
			const_node->setVariableName("mango");
			std::shared_ptr<ExpressionNode> expr_node = std::make_shared<ExpressionNode>();
			expr_node->setLeftAstNode(const_node);

			query_node.setASTNode(expr_node);

			AST_NODE node = query_node.getAstNode();
			EXPR_NODE_PTR child_expr_node = std::static_pointer_cast<ExpressionNode>(node);
			AST_NODE_PTR lhs = child_expr_node->getLeftAstNode();
			std::shared_ptr<VariableNode> child_node = std::static_pointer_cast<VariableNode>(lhs);


			Assert::IsTrue(child_node->getVariableName().compare("mango") == 0);
		}

		TEST_METHOD(setSynonymNode_RegexStmtS_Success) {
			QueryNode query_node;
			QuerySynonymType chosen_synonym = { QuerySynonymType::stmt };
			query_node.setSynonymNode("stmt", "s");
			Assert::IsTrue(query_node.getSynonymType() == chosen_synonym);
			Assert::IsTrue(query_node.getString().compare("s") == 0);
		}

		TEST_METHOD(setSynonymNode_RegexReadR_Success) {
			QueryNode query_node;
			QuerySynonymType chosen_synonym = { QuerySynonymType::read };
			query_node.setSynonymNode("read", "r");
			Assert::IsTrue(query_node.getSynonymType() == chosen_synonym);
			Assert::IsTrue(query_node.getString().compare("r") == 0);
		}

		TEST_METHOD(setSynonymNode_RegexPrintP_Success) {
			QueryNode query_node;
			QuerySynonymType chosen_synonym = { QuerySynonymType::print };
			query_node.setSynonymNode("print", "p");
			Assert::IsTrue(query_node.getSynonymType() == chosen_synonym);
			Assert::IsTrue(query_node.getString().compare("p") == 0);
		}

		TEST_METHOD(setSynonymNode_RegexWhileW_Success) {
			QueryNode query_node;
			QuerySynonymType chosen_synonym = { QuerySynonymType::whiles };
			query_node.setSynonymNode("while", "w");
			Assert::IsTrue(query_node.getSynonymType() == chosen_synonym);
			Assert::IsTrue(query_node.getString().compare("w") == 0);
		}

		TEST_METHOD(setSynonymNode_RegexIfIfs_Success) {
			QueryNode query_node;
			QuerySynonymType chosen_synonym = { QuerySynonymType::ifs };
			query_node.setSynonymNode("if", "ifs");
			Assert::IsTrue(query_node.getSynonymType() == chosen_synonym);
			Assert::IsTrue(query_node.getString().compare("ifs") == 0);
		}

		TEST_METHOD(setSynonymNode_RegexAssignA_Success) {
			QueryNode query_node;
			QuerySynonymType chosen_synonym = { QuerySynonymType::assign };
			query_node.setSynonymNode("assign", "a1");
			Assert::IsTrue(query_node.getSynonymType() == chosen_synonym);
			Assert::IsTrue(query_node.getString().compare("a1") == 0);
		}

		TEST_METHOD(setSynonymNode_RegexVariableV_Success) {
			QueryNode query_node;
			QuerySynonymType chosen_synonym = { QuerySynonymType::variable };
			query_node.setSynonymNode("variable", "v");
			Assert::IsTrue(query_node.getSynonymType() == chosen_synonym);
			Assert::IsTrue(query_node.getString().compare("v") == 0);
		}

		TEST_METHOD(setSynonymNode_RegexConstantC_Success) {
			QueryNode query_node;
			QuerySynonymType chosen_synonym = { QuerySynonymType::constant };
			query_node.setSynonymNode("constant", "c");
			Assert::IsTrue(query_node.getSynonymType() == chosen_synonym);
			Assert::IsTrue(query_node.getString().compare("c") == 0);
		}

		TEST_METHOD(setSynonymNode_RegexProcedureP_Success) {
			QueryNode query_node;
			QuerySynonymType chosen_synonym = { QuerySynonymType::procedure };
			query_node.setSynonymNode("procedure", "p");
			Assert::IsTrue(query_node.getSynonymType() == chosen_synonym);
			Assert::IsTrue(query_node.getString().compare("p") == 0);
		}

	};
}