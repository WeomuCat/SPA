#include "stdafx.h"
#include "CppUnitTest.h"
#include "Pattern.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
	TEST_CLASS(QE_PatternTests) {
	public:
		// 2 / (b + 3) % d + o * g
		std::shared_ptr<ExpressionNode> haystack1;

		// 1 + 1 + 1 + 1 + (1 + cat) + 1
		std::shared_ptr<ExpressionNode> haystack2;

		TEST_METHOD_INITIALIZE(Initialize) {
			{
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
				// b + 3
				e1->setExpressionType(ExpressionTypeEnum::plus);
				e1->setLeftAstNode(v1);
				e1->setRightAstNode(c2);

				// 2 / e1
				e2->setExpressionType(ExpressionTypeEnum::div);
				e2->setLeftAstNode(c1);
				e2->setRightAstNode(e1);

				// e2 % d
				e3->setExpressionType(ExpressionTypeEnum::mod);
				e3->setLeftAstNode(e2);
				e3->setRightAstNode(v2);

				// o * g
				e4->setExpressionType(ExpressionTypeEnum::times);
				e4->setLeftAstNode(v3);
				e4->setRightAstNode(v4);

				// e3 + e4
				e5->setExpressionType(ExpressionTypeEnum::plus);
				e5->setLeftAstNode(e3);
				e5->setRightAstNode(e4);

				haystack1 = e5;
			}

			{
				std::shared_ptr<VariableNode> v1 = std::make_shared<VariableNode>();
				v1->setVariableName("cat");

				std::shared_ptr<ConstantNode> c1 = std::make_shared<ConstantNode>();
				std::shared_ptr<ConstantNode> c2 = std::make_shared<ConstantNode>();
				std::shared_ptr<ConstantNode> c3 = std::make_shared<ConstantNode>();
				std::shared_ptr<ConstantNode> c4 = std::make_shared<ConstantNode>();
				std::shared_ptr<ConstantNode> c5 = std::make_shared<ConstantNode>();
				std::shared_ptr<ConstantNode> c6 = std::make_shared<ConstantNode>();
				c1->setValue("1");
				c2->setValue("1");
				c3->setValue("1");
				c4->setValue("1");
				c5->setValue("1");
				c6->setValue("1");

				std::shared_ptr<ExpressionNode> e1 = std::make_shared<ExpressionNode>();
				std::shared_ptr<ExpressionNode> e2 = std::make_shared<ExpressionNode>();
				std::shared_ptr<ExpressionNode> e3 = std::make_shared<ExpressionNode>();
				std::shared_ptr<ExpressionNode> e4 = std::make_shared<ExpressionNode>();
				std::shared_ptr<ExpressionNode> e5 = std::make_shared<ExpressionNode>();
				std::shared_ptr<ExpressionNode> e6 = std::make_shared<ExpressionNode>();
				// 1 + 1
				e1->setExpressionType(ExpressionTypeEnum::plus);
				e1->setLeftAstNode(c1);
				e1->setRightAstNode(c2);

				// e1 + 1
				e2->setExpressionType(ExpressionTypeEnum::plus);
				e2->setLeftAstNode(e1);
				e2->setRightAstNode(c3);

				// e2 + 1
				e3->setExpressionType(ExpressionTypeEnum::plus);
				e3->setLeftAstNode(e2);
				e3->setRightAstNode(c4);

				// 1 + cat
				e4->setExpressionType(ExpressionTypeEnum::plus);
				e4->setLeftAstNode(c5);
				e4->setRightAstNode(v1);

				// e3 + e4
				e5->setExpressionType(ExpressionTypeEnum::plus);
				e5->setLeftAstNode(e3);
				e5->setRightAstNode(e4);

				// e5 + 1
				e6->setExpressionType(ExpressionTypeEnum::plus);
				e6->setLeftAstNode(e5);
				e6->setRightAstNode(c6);

				haystack2 = e6;
			}
		}

		// === Exact ===

		TEST_METHOD(exactExpressionTreeMatch_haystack1_True) {
			// Haystack: 2 / (b + 3) % d + o * g
			// Needle: 2 / (b + 3) % d + o * g
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
			// b + 3
			e1->setExpressionType(ExpressionTypeEnum::plus);
			e1->setLeftAstNode(v1);
			e1->setRightAstNode(c2);

			// 2 / e1
			e2->setExpressionType(ExpressionTypeEnum::div);
			e2->setLeftAstNode(c1);
			e2->setRightAstNode(e1);

			// e2 % d
			e3->setExpressionType(ExpressionTypeEnum::mod);
			e3->setLeftAstNode(e2);
			e3->setRightAstNode(v2);

			// o * g
			e4->setExpressionType(ExpressionTypeEnum::times);
			e4->setLeftAstNode(v3);
			e4->setRightAstNode(v4);

			// e3 + e4
			e5->setExpressionType(ExpressionTypeEnum::plus);
			e5->setLeftAstNode(e3);
			e5->setRightAstNode(e4);

			std::shared_ptr<ExpressionNode> haystack = haystack1;
			std::shared_ptr<ExpressionNode> needle = e5;
			bool match = Pattern::exactExpressionTreeMatch(haystack, needle);

			Assert::IsTrue(match);
		}

		TEST_METHOD(exactExpressionTreeMatch_haystack1_False1) {
			// Haystack: 2 / (b + 3) % d + o * g
			// Needle: 2 / (b + 3) % d + o * o
			std::shared_ptr<VariableNode> v1 = std::make_shared<VariableNode>();
			std::shared_ptr<VariableNode> v2 = std::make_shared<VariableNode>();
			std::shared_ptr<VariableNode> v3 = std::make_shared<VariableNode>();
			std::shared_ptr<VariableNode> v4 = std::make_shared<VariableNode>();
			v1->setVariableName("b");
			v2->setVariableName("d");
			v3->setVariableName("o");
			v4->setVariableName("o");

			std::shared_ptr<ConstantNode> c1 = std::make_shared<ConstantNode>();
			std::shared_ptr<ConstantNode> c2 = std::make_shared<ConstantNode>();
			c1->setValue("2");
			c2->setValue("3");

			std::shared_ptr<ExpressionNode> e1 = std::make_shared<ExpressionNode>();
			std::shared_ptr<ExpressionNode> e2 = std::make_shared<ExpressionNode>();
			std::shared_ptr<ExpressionNode> e3 = std::make_shared<ExpressionNode>();
			std::shared_ptr<ExpressionNode> e4 = std::make_shared<ExpressionNode>();
			std::shared_ptr<ExpressionNode> e5 = std::make_shared<ExpressionNode>();
			// b + 3
			e1->setExpressionType(ExpressionTypeEnum::plus);
			e1->setLeftAstNode(v1);
			e1->setRightAstNode(c2);

			// 2 / e1
			e2->setExpressionType(ExpressionTypeEnum::div);
			e2->setLeftAstNode(c1);
			e2->setRightAstNode(e1);

			// e2 % d
			e3->setExpressionType(ExpressionTypeEnum::mod);
			e3->setLeftAstNode(e2);
			e3->setRightAstNode(v2);

			// o * o
			e4->setExpressionType(ExpressionTypeEnum::times);
			e4->setLeftAstNode(v3);
			e4->setRightAstNode(v4);

			// e3 + e4
			e5->setExpressionType(ExpressionTypeEnum::plus);
			e5->setLeftAstNode(e3);
			e5->setRightAstNode(e4);

			std::shared_ptr<ExpressionNode> haystack = haystack1;
			std::shared_ptr<ExpressionNode> needle = e5;
			bool match = Pattern::exactExpressionTreeMatch(haystack, needle);

			Assert::IsFalse(match);
		}

		TEST_METHOD(exactExpressionTreeMatch_haystack1_False2) {
			// Haystack: 2 / (b + 3) % d + o * g
			// Needle: 2 / (b + 3) % d + o * g + g
			std::shared_ptr<VariableNode> v1 = std::make_shared<VariableNode>();
			std::shared_ptr<VariableNode> v2 = std::make_shared<VariableNode>();
			std::shared_ptr<VariableNode> v3 = std::make_shared<VariableNode>();
			std::shared_ptr<VariableNode> v4 = std::make_shared<VariableNode>();
			std::shared_ptr<VariableNode> v5 = std::make_shared<VariableNode>();
			v1->setVariableName("b");
			v2->setVariableName("d");
			v3->setVariableName("o");
			v4->setVariableName("g");
			v5->setVariableName("g");

			std::shared_ptr<ConstantNode> c1 = std::make_shared<ConstantNode>();
			std::shared_ptr<ConstantNode> c2 = std::make_shared<ConstantNode>();
			c1->setValue("2");
			c2->setValue("3");

			std::shared_ptr<ExpressionNode> e1 = std::make_shared<ExpressionNode>();
			std::shared_ptr<ExpressionNode> e2 = std::make_shared<ExpressionNode>();
			std::shared_ptr<ExpressionNode> e3 = std::make_shared<ExpressionNode>();
			std::shared_ptr<ExpressionNode> e4 = std::make_shared<ExpressionNode>();
			std::shared_ptr<ExpressionNode> e5 = std::make_shared<ExpressionNode>();
			std::shared_ptr<ExpressionNode> e6 = std::make_shared<ExpressionNode>();
			// b + 3
			e1->setExpressionType(ExpressionTypeEnum::plus);
			e1->setLeftAstNode(v1);
			e1->setRightAstNode(c2);

			// 2 / e1
			e2->setExpressionType(ExpressionTypeEnum::div);
			e2->setLeftAstNode(c1);
			e2->setRightAstNode(e1);

			// e2 % d
			e3->setExpressionType(ExpressionTypeEnum::mod);
			e3->setLeftAstNode(e2);
			e3->setRightAstNode(v2);

			// o * g
			e4->setExpressionType(ExpressionTypeEnum::times);
			e4->setLeftAstNode(v3);
			e4->setRightAstNode(v4);

			// e3 + e4
			e5->setExpressionType(ExpressionTypeEnum::plus);
			e5->setLeftAstNode(e3);
			e5->setRightAstNode(e4);

			// e5 + g
			e6->setExpressionType(ExpressionTypeEnum::plus);
			e6->setLeftAstNode(e5);
			e6->setRightAstNode(v5);

			std::shared_ptr<ExpressionNode> haystack = haystack1;
			std::shared_ptr<ExpressionNode> needle = e6;
			bool match = Pattern::exactExpressionTreeMatch(haystack, needle);

			Assert::IsFalse(match);
		}

		TEST_METHOD(exactExpressionTreeMatch_haystack2_True) {
			// Haystack: 1 + 1 + 1 + 1 + (1 + cat) + 1
			// Needle: 1 + 1 + 1 + 1 + (1 + cat) + 1
			std::shared_ptr<VariableNode> v1 = std::make_shared<VariableNode>();
			v1->setVariableName("cat");

			std::shared_ptr<ConstantNode> c1 = std::make_shared<ConstantNode>();
			std::shared_ptr<ConstantNode> c2 = std::make_shared<ConstantNode>();
			std::shared_ptr<ConstantNode> c3 = std::make_shared<ConstantNode>();
			std::shared_ptr<ConstantNode> c4 = std::make_shared<ConstantNode>();
			std::shared_ptr<ConstantNode> c5 = std::make_shared<ConstantNode>();
			std::shared_ptr<ConstantNode> c6 = std::make_shared<ConstantNode>();
			c1->setValue("1");
			c2->setValue("1");
			c3->setValue("1");
			c4->setValue("1");
			c5->setValue("1");
			c6->setValue("1");

			std::shared_ptr<ExpressionNode> e1 = std::make_shared<ExpressionNode>();
			std::shared_ptr<ExpressionNode> e2 = std::make_shared<ExpressionNode>();
			std::shared_ptr<ExpressionNode> e3 = std::make_shared<ExpressionNode>();
			std::shared_ptr<ExpressionNode> e4 = std::make_shared<ExpressionNode>();
			std::shared_ptr<ExpressionNode> e5 = std::make_shared<ExpressionNode>();
			std::shared_ptr<ExpressionNode> e6 = std::make_shared<ExpressionNode>();
			// 1 + 1
			e1->setExpressionType(ExpressionTypeEnum::plus);
			e1->setLeftAstNode(c1);
			e1->setRightAstNode(c2);

			// e1 + 1
			e2->setExpressionType(ExpressionTypeEnum::plus);
			e2->setLeftAstNode(e1);
			e2->setRightAstNode(c3);

			// e2 + 1
			e3->setExpressionType(ExpressionTypeEnum::plus);
			e3->setLeftAstNode(e2);
			e3->setRightAstNode(c4);

			// 1 + cat
			e4->setExpressionType(ExpressionTypeEnum::plus);
			e4->setLeftAstNode(c5);
			e4->setRightAstNode(v1);

			// e3 + e4
			e5->setExpressionType(ExpressionTypeEnum::plus);
			e5->setLeftAstNode(e3);
			e5->setRightAstNode(e4);

			// e5 + 1
			e6->setExpressionType(ExpressionTypeEnum::plus);
			e6->setLeftAstNode(e5);
			e6->setRightAstNode(c6);

			std::shared_ptr<ExpressionNode> haystack = haystack2;
			std::shared_ptr<ExpressionNode> needle = e6;
			bool match = Pattern::exactExpressionTreeMatch(haystack, needle);

			Assert::IsTrue(match);
		}

		TEST_METHOD(exactExpressionTreeMatch_haystack2_False1) {
			// Haystack: 1 + 1 + 1 + 1 + (1 + cat) + 1
			// Needle: 1 + 1 + 1 + 1 + (cat + 1) + 1
			std::shared_ptr<VariableNode> v1 = std::make_shared<VariableNode>();
			v1->setVariableName("cat");

			std::shared_ptr<ConstantNode> c1 = std::make_shared<ConstantNode>();
			std::shared_ptr<ConstantNode> c2 = std::make_shared<ConstantNode>();
			std::shared_ptr<ConstantNode> c3 = std::make_shared<ConstantNode>();
			std::shared_ptr<ConstantNode> c4 = std::make_shared<ConstantNode>();
			std::shared_ptr<ConstantNode> c5 = std::make_shared<ConstantNode>();
			std::shared_ptr<ConstantNode> c6 = std::make_shared<ConstantNode>();
			c1->setValue("1");
			c2->setValue("1");
			c3->setValue("1");
			c4->setValue("1");
			c5->setValue("1");
			c6->setValue("1");

			std::shared_ptr<ExpressionNode> e1 = std::make_shared<ExpressionNode>();
			std::shared_ptr<ExpressionNode> e2 = std::make_shared<ExpressionNode>();
			std::shared_ptr<ExpressionNode> e3 = std::make_shared<ExpressionNode>();
			std::shared_ptr<ExpressionNode> e4 = std::make_shared<ExpressionNode>();
			std::shared_ptr<ExpressionNode> e5 = std::make_shared<ExpressionNode>();
			std::shared_ptr<ExpressionNode> e6 = std::make_shared<ExpressionNode>();
			// 1 + 1
			e1->setExpressionType(ExpressionTypeEnum::plus);
			e1->setLeftAstNode(c1);
			e1->setRightAstNode(c2);

			// e1 + 1
			e2->setExpressionType(ExpressionTypeEnum::plus);
			e2->setLeftAstNode(e1);
			e2->setRightAstNode(c3);

			// e2 + 1
			e3->setExpressionType(ExpressionTypeEnum::plus);
			e3->setLeftAstNode(e2);
			e3->setRightAstNode(c4);

			// cat + 1
			e4->setExpressionType(ExpressionTypeEnum::plus);
			e4->setLeftAstNode(v1);
			e4->setRightAstNode(c5);

			// e3 + e4
			e5->setExpressionType(ExpressionTypeEnum::plus);
			e5->setLeftAstNode(e3);
			e5->setRightAstNode(e4);

			// e5 + 1
			e6->setExpressionType(ExpressionTypeEnum::plus);
			e6->setLeftAstNode(e5);
			e6->setRightAstNode(c6);

			std::shared_ptr<ExpressionNode> haystack = haystack2;
			std::shared_ptr<ExpressionNode> needle = e6;
			bool match = Pattern::exactExpressionTreeMatch(haystack, needle);

			Assert::IsFalse(match);
		}

		TEST_METHOD(exactExpressionTreeMatch_haystack2_False2) {
			// Haystack: 1 + 1 + 1 + 1 + (1 + cat) + 1
			// Needle: 1 + 1 + 1 + 1 + (1 + cat) + 1 + 1
			std::shared_ptr<VariableNode> v1 = std::make_shared<VariableNode>();
			v1->setVariableName("cat");

			std::shared_ptr<ConstantNode> c1 = std::make_shared<ConstantNode>();
			std::shared_ptr<ConstantNode> c2 = std::make_shared<ConstantNode>();
			std::shared_ptr<ConstantNode> c3 = std::make_shared<ConstantNode>();
			std::shared_ptr<ConstantNode> c4 = std::make_shared<ConstantNode>();
			std::shared_ptr<ConstantNode> c5 = std::make_shared<ConstantNode>();
			std::shared_ptr<ConstantNode> c6 = std::make_shared<ConstantNode>();
			std::shared_ptr<ConstantNode> c7 = std::make_shared<ConstantNode>();
			c1->setValue("1");
			c2->setValue("1");
			c3->setValue("1");
			c4->setValue("1");
			c5->setValue("1");
			c6->setValue("1");
			c7->setValue("1");

			std::shared_ptr<ExpressionNode> e1 = std::make_shared<ExpressionNode>();
			std::shared_ptr<ExpressionNode> e2 = std::make_shared<ExpressionNode>();
			std::shared_ptr<ExpressionNode> e3 = std::make_shared<ExpressionNode>();
			std::shared_ptr<ExpressionNode> e4 = std::make_shared<ExpressionNode>();
			std::shared_ptr<ExpressionNode> e5 = std::make_shared<ExpressionNode>();
			std::shared_ptr<ExpressionNode> e6 = std::make_shared<ExpressionNode>();
			std::shared_ptr<ExpressionNode> e7 = std::make_shared<ExpressionNode>();
			// 1 + 1
			e1->setExpressionType(ExpressionTypeEnum::plus);
			e1->setLeftAstNode(c1);
			e1->setRightAstNode(c2);

			// e1 + 1
			e2->setExpressionType(ExpressionTypeEnum::plus);
			e2->setLeftAstNode(e1);
			e2->setRightAstNode(c3);

			// e2 + 1
			e3->setExpressionType(ExpressionTypeEnum::plus);
			e3->setLeftAstNode(e2);
			e3->setRightAstNode(c4);

			// 1 + cat
			e4->setExpressionType(ExpressionTypeEnum::plus);
			e4->setLeftAstNode(c5);
			e4->setRightAstNode(v1);

			// e3 + e4
			e5->setExpressionType(ExpressionTypeEnum::plus);
			e5->setLeftAstNode(e3);
			e5->setRightAstNode(e4);

			// e5 + 1
			e6->setExpressionType(ExpressionTypeEnum::plus);
			e6->setLeftAstNode(e5);
			e6->setRightAstNode(c6);

			// e6 + 1
			e7->setExpressionType(ExpressionTypeEnum::plus);
			e7->setLeftAstNode(e6);
			e7->setRightAstNode(c7);

			std::shared_ptr<ExpressionNode> haystack = haystack2;
			std::shared_ptr<ExpressionNode> needle = e7;
			bool match = Pattern::exactExpressionTreeMatch(haystack, needle);

			Assert::IsFalse(match);
		}

		// === Partial===

		TEST_METHOD(partialExpressionTreeMatch_haystack1_True1) {
			// Haystack: 2 / (b + 3) % d + o * g
			// Needle: b
			std::shared_ptr<VariableNode> v1 = std::make_shared<VariableNode>();
			v1->setVariableName("b");

			std::shared_ptr<ExpressionNode> e1 = std::make_shared<ExpressionNode>();
			// b
			e1->setExpressionType(ExpressionTypeEnum::none);
			e1->setLeftAstNode(v1);

			std::shared_ptr<ExpressionNode> haystack = haystack1;
			std::shared_ptr<ExpressionNode> needle = e1;
			bool match = Pattern::partialExpressionTreeMatch(haystack, needle);

			Assert::IsTrue(match);
		}

		TEST_METHOD(partialExpressionTreeMatch_haystack1_True2) {
			// Haystack: 2 / (b + 3) % d + o * g
			// Needle: 3
			std::shared_ptr<ConstantNode> c1 = std::make_shared<ConstantNode>();
			c1->setValue("3");

			std::shared_ptr<ExpressionNode> e1 = std::make_shared<ExpressionNode>();
			// 3
			e1->setExpressionType(ExpressionTypeEnum::none);
			e1->setLeftAstNode(c1);

			std::shared_ptr<ExpressionNode> haystack = haystack1;
			std::shared_ptr<ExpressionNode> needle = e1;
			bool match = Pattern::partialExpressionTreeMatch(haystack, needle);

			Assert::IsTrue(match);
		}

		TEST_METHOD(partialExpressionTreeMatch_haystack1_True3) {
			// Haystack: 2 / (b + 3) % d + o * g
			// Needle: b + 3
			std::shared_ptr<VariableNode> v1 = std::make_shared<VariableNode>();
			v1->setVariableName("b");

			std::shared_ptr<ConstantNode> c1 = std::make_shared<ConstantNode>();
			c1->setValue("3");

			std::shared_ptr<ExpressionNode> e1 = std::make_shared<ExpressionNode>();
			// b + 3
			e1->setExpressionType(ExpressionTypeEnum::plus);
			e1->setLeftAstNode(v1);
			e1->setRightAstNode(c1);

			std::shared_ptr<ExpressionNode> haystack = haystack1;
			std::shared_ptr<ExpressionNode> needle = e1;
			bool match = Pattern::partialExpressionTreeMatch(haystack, needle);

			Assert::IsTrue(match);
		}

		TEST_METHOD(partialExpressionTreeMatch_haystack1_True4) {
			// Haystack: 2 / (b + 3) % d + o * g
			// Needle: 2 / (b + 3)
			std::shared_ptr<VariableNode> v1 = std::make_shared<VariableNode>();
			v1->setVariableName("b");

			std::shared_ptr<ConstantNode> c1 = std::make_shared<ConstantNode>();
			std::shared_ptr<ConstantNode> c2 = std::make_shared<ConstantNode>();
			c1->setValue("2");
			c2->setValue("3");

			std::shared_ptr<ExpressionNode> e1 = std::make_shared<ExpressionNode>();
			std::shared_ptr<ExpressionNode> e2 = std::make_shared<ExpressionNode>();
			// b + 3
			e1->setExpressionType(ExpressionTypeEnum::plus);
			e1->setLeftAstNode(v1);
			e1->setRightAstNode(c2);
			// 2 / e1
			e2->setExpressionType(ExpressionTypeEnum::div);
			e2->setLeftAstNode(c1);
			e2->setRightAstNode(e1);

			std::shared_ptr<ExpressionNode> haystack = haystack1;
			std::shared_ptr<ExpressionNode> needle = e2;
			bool match = Pattern::partialExpressionTreeMatch(haystack, needle);

			Assert::IsTrue(match);
		}

		TEST_METHOD(partialExpressionTreeMatch_haystack1_True5) {
			// Haystack: 2 / (b + 3) % d + o * g
			// Needle: 2 / (b + 3) % d
			std::shared_ptr<VariableNode> v1 = std::make_shared<VariableNode>();
			std::shared_ptr<VariableNode> v2 = std::make_shared<VariableNode>();
			v1->setVariableName("b");
			v2->setVariableName("d");

			std::shared_ptr<ConstantNode> c1 = std::make_shared<ConstantNode>();
			std::shared_ptr<ConstantNode> c2 = std::make_shared<ConstantNode>();
			c1->setValue("2");
			c2->setValue("3");

			std::shared_ptr<ExpressionNode> e1 = std::make_shared<ExpressionNode>();
			std::shared_ptr<ExpressionNode> e2 = std::make_shared<ExpressionNode>();
			std::shared_ptr<ExpressionNode> e3 = std::make_shared<ExpressionNode>();
			// b + 3
			e1->setExpressionType(ExpressionTypeEnum::plus);
			e1->setLeftAstNode(v1);
			e1->setRightAstNode(c2);
			// 2 / e1
			e2->setExpressionType(ExpressionTypeEnum::div);
			e2->setLeftAstNode(c1);
			e2->setRightAstNode(e1);
			// e2 % d
			e3->setExpressionType(ExpressionTypeEnum::mod);
			e3->setLeftAstNode(e2);
			e3->setRightAstNode(v2);

			std::shared_ptr<ExpressionNode> haystack = haystack1;
			std::shared_ptr<ExpressionNode> needle = e3;
			bool match = Pattern::partialExpressionTreeMatch(haystack, needle);

			Assert::IsTrue(match);
		}

		TEST_METHOD(partialExpressionTreeMatch_haystack1_True6) {
			// Haystack: 2 / (b + 3) % d + o * g
			// Needle: o * g
			std::shared_ptr<VariableNode> v1 = std::make_shared<VariableNode>();
			std::shared_ptr<VariableNode> v2 = std::make_shared<VariableNode>();
			v1->setVariableName("o");
			v2->setVariableName("g");

			std::shared_ptr<ExpressionNode> e1 = std::make_shared<ExpressionNode>();
			// o * g
			e1->setExpressionType(ExpressionTypeEnum::times);
			e1->setLeftAstNode(v1);
			e1->setRightAstNode(v2);

			std::shared_ptr<ExpressionNode> haystack = haystack1;
			std::shared_ptr<ExpressionNode> needle = e1;
			bool match = Pattern::partialExpressionTreeMatch(haystack, needle);

			Assert::IsTrue(match);
		}

		TEST_METHOD(partialExpressionTreeMatch_haystack1_True7) {
			// Haystack: 2 / (b + 3) % d + o * g
			// Needle: 2 / (b + 3) % d + o * g
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
			// b + 3
			e1->setExpressionType(ExpressionTypeEnum::plus);
			e1->setLeftAstNode(v1);
			e1->setRightAstNode(c2);

			// 2 / e1
			e2->setExpressionType(ExpressionTypeEnum::div);
			e2->setLeftAstNode(c1);
			e2->setRightAstNode(e1);

			// e2 % d
			e3->setExpressionType(ExpressionTypeEnum::mod);
			e3->setLeftAstNode(e2);
			e3->setRightAstNode(v2);

			// o * g
			e4->setExpressionType(ExpressionTypeEnum::times);
			e4->setLeftAstNode(v3);
			e4->setRightAstNode(v4);

			// e3 + e4
			e5->setExpressionType(ExpressionTypeEnum::plus);
			e5->setLeftAstNode(e3);
			e5->setRightAstNode(e4);

			std::shared_ptr<ExpressionNode> haystack = haystack1;
			std::shared_ptr<ExpressionNode> needle = e5;
			bool match = Pattern::partialExpressionTreeMatch(haystack, needle);

			Assert::IsTrue(match);
		}

		TEST_METHOD(partialExpressionTreeMatch_haystack1_False1) {
			// Haystack: 2 / (b + 3) % d + o * g
			// Needle: a
			std::shared_ptr<VariableNode> v1 = std::make_shared<VariableNode>();
			v1->setVariableName("a");

			std::shared_ptr<ExpressionNode> e1 = std::make_shared<ExpressionNode>();
			// a
			e1->setExpressionType(ExpressionTypeEnum::none);
			e1->setLeftAstNode(v1);

			std::shared_ptr<ExpressionNode> haystack = haystack1;
			std::shared_ptr<ExpressionNode> needle = e1;
			bool match = Pattern::partialExpressionTreeMatch(haystack, needle);

			Assert::IsFalse(match);
		}

		TEST_METHOD(partialExpressionTreeMatch_haystack1_False2) {
			// Haystack: 2 / (b + 3) % d + o * g
			// Needle: 1
			std::shared_ptr<ConstantNode> c1 = std::make_shared<ConstantNode>();
			c1->setValue("1");

			std::shared_ptr<ExpressionNode> e1 = std::make_shared<ExpressionNode>();
			// 1
			e1->setExpressionType(ExpressionTypeEnum::none);
			e1->setLeftAstNode(c1);

			std::shared_ptr<ExpressionNode> haystack = haystack1;
			std::shared_ptr<ExpressionNode> needle = e1;
			bool match = Pattern::partialExpressionTreeMatch(haystack, needle);

			Assert::IsFalse(match);
		}

		TEST_METHOD(partialExpressionTreeMatch_haystack1_False3) {
			// Haystack: 2 / (b + 3) % d + o * g
			// Needle: b + 2
			std::shared_ptr<VariableNode> v1 = std::make_shared<VariableNode>();
			v1->setVariableName("b");

			std::shared_ptr<ConstantNode> c1 = std::make_shared<ConstantNode>();
			c1->setValue("2");

			std::shared_ptr<ExpressionNode> e1 = std::make_shared<ExpressionNode>();
			// b + 2
			e1->setExpressionType(ExpressionTypeEnum::plus);
			e1->setLeftAstNode(v1);
			e1->setRightAstNode(c1);

			std::shared_ptr<ExpressionNode> haystack = haystack1;
			std::shared_ptr<ExpressionNode> needle = e1;
			bool match = Pattern::partialExpressionTreeMatch(haystack, needle);

			Assert::IsFalse(match);
		}

		TEST_METHOD(partialExpressionTreeMatch_haystack1_False4) {
			// Haystack: 2 / (b + 3) % d + o * g
			// Needle: 2 / (b - 3)
			std::shared_ptr<VariableNode> v1 = std::make_shared<VariableNode>();
			v1->setVariableName("b");

			std::shared_ptr<ConstantNode> c1 = std::make_shared<ConstantNode>();
			std::shared_ptr<ConstantNode> c2 = std::make_shared<ConstantNode>();
			c1->setValue("2");
			c2->setValue("3");

			std::shared_ptr<ExpressionNode> e1 = std::make_shared<ExpressionNode>();
			std::shared_ptr<ExpressionNode> e2 = std::make_shared<ExpressionNode>();
			// b - 3
			e1->setExpressionType(ExpressionTypeEnum::min);
			e1->setLeftAstNode(v1);
			e1->setRightAstNode(c2);
			// 2 / e1
			e2->setExpressionType(ExpressionTypeEnum::div);
			e2->setLeftAstNode(c1);
			e2->setRightAstNode(e1);

			std::shared_ptr<ExpressionNode> haystack = haystack1;
			std::shared_ptr<ExpressionNode> needle = e2;
			bool match = Pattern::partialExpressionTreeMatch(haystack, needle);

			Assert::IsFalse(match);
		}

		TEST_METHOD(partialExpressionTreeMatch_haystack1_False5) {
			// Haystack: 2 / (b + 3) % d + o * g
			// Needle: (b + 3) % d
			std::shared_ptr<VariableNode> v1 = std::make_shared<VariableNode>();
			std::shared_ptr<VariableNode> v2 = std::make_shared<VariableNode>();
			v1->setVariableName("b");
			v2->setVariableName("d");

			std::shared_ptr<ConstantNode> c1 = std::make_shared<ConstantNode>();
			c1->setValue("3");

			std::shared_ptr<ExpressionNode> e1 = std::make_shared<ExpressionNode>();
			std::shared_ptr<ExpressionNode> e2 = std::make_shared<ExpressionNode>();
			// b + 3
			e1->setExpressionType(ExpressionTypeEnum::plus);
			e1->setLeftAstNode(v1);
			e1->setRightAstNode(c1);
			// e1 % d
			e2->setExpressionType(ExpressionTypeEnum::mod);
			e2->setLeftAstNode(e1);
			e2->setRightAstNode(v2);

			std::shared_ptr<ExpressionNode> haystack = haystack1;
			std::shared_ptr<ExpressionNode> needle = e2;
			bool match = Pattern::partialExpressionTreeMatch(haystack, needle);

			Assert::IsFalse(match);
		}

		TEST_METHOD(partialExpressionTreeMatch_haystack1_False6) {
			// Haystack: 2 / (b + 3) % d + o * g
			// Needle: 2 / (b + 3) % d + o * o
			std::shared_ptr<VariableNode> v1 = std::make_shared<VariableNode>();
			std::shared_ptr<VariableNode> v2 = std::make_shared<VariableNode>();
			std::shared_ptr<VariableNode> v3 = std::make_shared<VariableNode>();
			std::shared_ptr<VariableNode> v4 = std::make_shared<VariableNode>();
			v1->setVariableName("b");
			v2->setVariableName("d");
			v3->setVariableName("o");
			v4->setVariableName("o");

			std::shared_ptr<ConstantNode> c1 = std::make_shared<ConstantNode>();
			std::shared_ptr<ConstantNode> c2 = std::make_shared<ConstantNode>();
			c1->setValue("2");
			c2->setValue("3");

			std::shared_ptr<ExpressionNode> e1 = std::make_shared<ExpressionNode>();
			std::shared_ptr<ExpressionNode> e2 = std::make_shared<ExpressionNode>();
			std::shared_ptr<ExpressionNode> e3 = std::make_shared<ExpressionNode>();
			std::shared_ptr<ExpressionNode> e4 = std::make_shared<ExpressionNode>();
			std::shared_ptr<ExpressionNode> e5 = std::make_shared<ExpressionNode>();
			// b + 3
			e1->setExpressionType(ExpressionTypeEnum::plus);
			e1->setLeftAstNode(v1);
			e1->setRightAstNode(c2);

			// 2 / e1
			e2->setExpressionType(ExpressionTypeEnum::div);
			e2->setLeftAstNode(c1);
			e2->setRightAstNode(e1);

			// e2 % d
			e3->setExpressionType(ExpressionTypeEnum::mod);
			e3->setLeftAstNode(e2);
			e3->setRightAstNode(v2);

			// o * o
			e4->setExpressionType(ExpressionTypeEnum::times);
			e4->setLeftAstNode(v3);
			e4->setRightAstNode(v4);

			// e3 + e4
			e5->setExpressionType(ExpressionTypeEnum::plus);
			e5->setLeftAstNode(e3);
			e5->setRightAstNode(e4);

			std::shared_ptr<ExpressionNode> haystack = haystack1;
			std::shared_ptr<ExpressionNode> needle = e5;
			bool match = Pattern::partialExpressionTreeMatch(haystack, needle);

			Assert::IsFalse(match);
		}

		TEST_METHOD(partialExpressionTreeMatch_haystack1_False7) {
			// Haystack: 2 / (b + 3) % d + o * g
			// Needle: 2 / (b + 3) % d + o * g + g
			std::shared_ptr<VariableNode> v1 = std::make_shared<VariableNode>();
			std::shared_ptr<VariableNode> v2 = std::make_shared<VariableNode>();
			std::shared_ptr<VariableNode> v3 = std::make_shared<VariableNode>();
			std::shared_ptr<VariableNode> v4 = std::make_shared<VariableNode>();
			std::shared_ptr<VariableNode> v5 = std::make_shared<VariableNode>();
			v1->setVariableName("b");
			v2->setVariableName("d");
			v3->setVariableName("o");
			v4->setVariableName("g");
			v5->setVariableName("g");

			std::shared_ptr<ConstantNode> c1 = std::make_shared<ConstantNode>();
			std::shared_ptr<ConstantNode> c2 = std::make_shared<ConstantNode>();
			c1->setValue("2");
			c2->setValue("3");

			std::shared_ptr<ExpressionNode> e1 = std::make_shared<ExpressionNode>();
			std::shared_ptr<ExpressionNode> e2 = std::make_shared<ExpressionNode>();
			std::shared_ptr<ExpressionNode> e3 = std::make_shared<ExpressionNode>();
			std::shared_ptr<ExpressionNode> e4 = std::make_shared<ExpressionNode>();
			std::shared_ptr<ExpressionNode> e5 = std::make_shared<ExpressionNode>();
			std::shared_ptr<ExpressionNode> e6 = std::make_shared<ExpressionNode>();
			// b + 3
			e1->setExpressionType(ExpressionTypeEnum::plus);
			e1->setLeftAstNode(v1);
			e1->setRightAstNode(c2);

			// 2 / e1
			e2->setExpressionType(ExpressionTypeEnum::div);
			e2->setLeftAstNode(c1);
			e2->setRightAstNode(e1);

			// e2 % d
			e3->setExpressionType(ExpressionTypeEnum::mod);
			e3->setLeftAstNode(e2);
			e3->setRightAstNode(v2);

			// o * g
			e4->setExpressionType(ExpressionTypeEnum::times);
			e4->setLeftAstNode(v3);
			e4->setRightAstNode(v4);

			// e3 + e4
			e5->setExpressionType(ExpressionTypeEnum::plus);
			e5->setLeftAstNode(e3);
			e5->setRightAstNode(e4);

			// e5 + g
			e6->setExpressionType(ExpressionTypeEnum::plus);
			e6->setLeftAstNode(e5);
			e6->setRightAstNode(v5);

			std::shared_ptr<ExpressionNode> haystack = haystack1;
			std::shared_ptr<ExpressionNode> needle = e6;
			bool match = Pattern::partialExpressionTreeMatch(haystack, needle);

			Assert::IsFalse(match);
		}

		TEST_METHOD(partialExpressionTreeMatch_haystack2_True1) {
			// Haystack: 1 + 1 + 1 + 1 + (1 + cat) + 1
			// Needle: 1 + cat
			std::shared_ptr<VariableNode> v1 = std::make_shared<VariableNode>();
			v1->setVariableName("cat");

			std::shared_ptr<ConstantNode> c1 = std::make_shared<ConstantNode>();
			c1->setValue("1");

			std::shared_ptr<ExpressionNode> e1 = std::make_shared<ExpressionNode>();
			// 1 + cat
			e1->setExpressionType(ExpressionTypeEnum::plus);
			e1->setLeftAstNode(c1);
			e1->setRightAstNode(v1);

			std::shared_ptr<ExpressionNode> haystack = haystack2;
			std::shared_ptr<ExpressionNode> needle = e1;
			bool match = Pattern::partialExpressionTreeMatch(haystack, needle);

			Assert::IsTrue(match);
		}

		TEST_METHOD(partialExpressionTreeMatch_haystack2_True2) {
			// Haystack: 1 + 1 + 1 + 1 + (1 + cat) + 1
			// Needle: 1 + 1 + 1
			std::shared_ptr<ConstantNode> c1 = std::make_shared<ConstantNode>();
			std::shared_ptr<ConstantNode> c2 = std::make_shared<ConstantNode>();
			std::shared_ptr<ConstantNode> c3 = std::make_shared<ConstantNode>();
			c1->setValue("1");
			c2->setValue("1");
			c3->setValue("1");

			std::shared_ptr<ExpressionNode> e1 = std::make_shared<ExpressionNode>();
			std::shared_ptr<ExpressionNode> e2 = std::make_shared<ExpressionNode>();
			// 1 + 1
			e1->setExpressionType(ExpressionTypeEnum::plus);
			e1->setLeftAstNode(c1);
			e1->setRightAstNode(c2);
			// e1 + 1
			e2->setExpressionType(ExpressionTypeEnum::plus);
			e2->setLeftAstNode(e1);
			e2->setRightAstNode(c3);

			std::shared_ptr<ExpressionNode> haystack = haystack2;
			std::shared_ptr<ExpressionNode> needle = e2;
			bool match = Pattern::partialExpressionTreeMatch(haystack, needle);

			Assert::IsTrue(match);
		}

		TEST_METHOD(partialExpressionTreeMatch_haystack2_False1) {
			// Haystack: 1 + 1 + 1 + 1 + (1 + cat) + 1
			// Needle: 1 + (1 + 1)
			std::shared_ptr<ConstantNode> c1 = std::make_shared<ConstantNode>();
			std::shared_ptr<ConstantNode> c2 = std::make_shared<ConstantNode>();
			std::shared_ptr<ConstantNode> c3 = std::make_shared<ConstantNode>();
			c1->setValue("1");
			c2->setValue("1");
			c3->setValue("1");

			std::shared_ptr<ExpressionNode> e1 = std::make_shared<ExpressionNode>();
			std::shared_ptr<ExpressionNode> e2 = std::make_shared<ExpressionNode>();
			// 1 + 1
			e1->setExpressionType(ExpressionTypeEnum::plus);
			e1->setLeftAstNode(c2);
			e1->setRightAstNode(c3);
			// 1 + e1
			e2->setExpressionType(ExpressionTypeEnum::plus);
			e2->setLeftAstNode(c1);
			e2->setRightAstNode(e1);

			std::shared_ptr<ExpressionNode> haystack = haystack2;
			std::shared_ptr<ExpressionNode> needle = e2;
			bool match = Pattern::partialExpressionTreeMatch(haystack, needle);

			Assert::IsFalse(match);
		}

		TEST_METHOD(partialExpressionTreeMatch_haystack2_False2) {
			// Haystack: 1 + 1 + 1 + 1 + (1 + cat) + 1
			// Needle: 1 + 1 + 1 + 1 + 1
			std::shared_ptr<ConstantNode> c1 = std::make_shared<ConstantNode>();
			std::shared_ptr<ConstantNode> c2 = std::make_shared<ConstantNode>();
			std::shared_ptr<ConstantNode> c3 = std::make_shared<ConstantNode>();
			std::shared_ptr<ConstantNode> c4 = std::make_shared<ConstantNode>();
			std::shared_ptr<ConstantNode> c5 = std::make_shared<ConstantNode>();
			c1->setValue("1");
			c2->setValue("1");
			c3->setValue("1");
			c4->setValue("1");
			c5->setValue("1");

			std::shared_ptr<ExpressionNode> e1 = std::make_shared<ExpressionNode>();
			std::shared_ptr<ExpressionNode> e2 = std::make_shared<ExpressionNode>();
			std::shared_ptr<ExpressionNode> e3 = std::make_shared<ExpressionNode>();
			std::shared_ptr<ExpressionNode> e4 = std::make_shared<ExpressionNode>();
			// 1 + 1
			e1->setExpressionType(ExpressionTypeEnum::plus);
			e1->setLeftAstNode(c1);
			e1->setRightAstNode(c2);
			// e1 + 1
			e2->setExpressionType(ExpressionTypeEnum::plus);
			e2->setLeftAstNode(e1);
			e2->setRightAstNode(c3);
			// e2 + 1
			e3->setExpressionType(ExpressionTypeEnum::plus);
			e3->setLeftAstNode(e2);
			e3->setRightAstNode(c4);
			// e3 + 1
			e4->setExpressionType(ExpressionTypeEnum::plus);
			e4->setLeftAstNode(e3);
			e4->setRightAstNode(c5);

			std::shared_ptr<ExpressionNode> haystack = haystack2;
			std::shared_ptr<ExpressionNode> needle = e4;
			bool match = Pattern::partialExpressionTreeMatch(haystack, needle);

			Assert::IsFalse(match);
		}
	};
}
