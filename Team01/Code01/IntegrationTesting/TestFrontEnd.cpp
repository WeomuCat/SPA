#include "stdafx.h"
#include "CppUnitTest.h"
#include "../source/Parser.h"
#include "../source/PKB.h"
#include "../source/PKBBuilder.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace IntegrationTesting
{		
	TEST_CLASS(ParserPKBIntegrationTest)
	{
	public:
		
		TEST_METHOD(parseSource1_addToPKB)
		{
			Parser parser;
			std::string filename = "../IntegrationTesting/IntegrationTest_Source_1.txt";
			PKB pkb = parser.parseFile(filename);

			//Test Follows
			Assert::IsTrue(pkb.isFollows(2, 3));
			Assert::IsTrue(pkb.isFollows(4, 5));

			//Test Parents
			Assert::IsTrue(pkb.isParent(1, 2));
			Assert::IsTrue(pkb.isParent(1, 3));
			Assert::IsTrue(pkb.isParent(3, 4));
			Assert::IsTrue(pkb.isParent(3, 5));
			Assert::IsTrue(pkb.isParent(3, 6));

			//Test Modifies
			Assert::IsTrue(pkb.isModifies(1, "c1"));
			Assert::IsTrue(pkb.isModifies(2, "c1"));

			Assert::IsTrue(pkb.isModifies(1, "a1"));
			Assert::IsTrue(pkb.isModifies(3, "a1"));
			Assert::IsTrue(pkb.isModifies(4, "a1"));

			Assert::IsTrue(pkb.isModifies(1, "b1"));
			Assert::IsTrue(pkb.isModifies(3, "b1"));
			Assert::IsTrue(pkb.isModifies(5, "b1"));

			Assert::IsTrue(pkb.isModifies(1, "d1"));
			Assert::IsTrue(pkb.isModifies(3, "d1"));
			Assert::IsTrue(pkb.isModifies(6, "d1"));

			Assert::IsTrue(pkb.isModifies("main", "a1"));
			Assert::IsTrue(pkb.isModifies("main", "b1"));
			Assert::IsTrue(pkb.isModifies("main", "c1"));
			Assert::IsTrue(pkb.isModifies("main", "d1"));

			//Test Uses
			Assert::IsTrue(pkb.isUses(1, "a1"));
			Assert::IsTrue(pkb.isUses(3, "a1"));
			Assert::IsTrue(pkb.isUses(4, "a1"));

			Assert::IsTrue(pkb.isUses(1, "a2"));
			Assert::IsTrue(pkb.isUses(3, "a2"));
			Assert::IsTrue(pkb.isUses(4, "a2"));

			Assert::IsTrue(pkb.isUses(1, "b2"));
			Assert::IsTrue(pkb.isUses(3, "b2"));
			Assert::IsTrue(pkb.isUses(5, "b2"));

			Assert::IsTrue(pkb.isUses(1, "b3"));
			Assert::IsTrue(pkb.isUses(3, "b3"));
			Assert::IsTrue(pkb.isUses(5, "b3"));

			Assert::IsTrue(pkb.isUses(1, "c2"));
			Assert::IsTrue(pkb.isUses(2, "c2"));

			Assert::IsTrue(pkb.isUses(1, "d2"));
			Assert::IsTrue(pkb.isUses(3, "d2"));
			Assert::IsTrue(pkb.isUses(6, "d2"));

			Assert::IsTrue(pkb.isUses(1, "d3"));
			Assert::IsTrue(pkb.isUses(3, "d3"));
			Assert::IsTrue(pkb.isUses(6, "d3"));

			Assert::IsTrue(pkb.isUses(1, "x"));
			Assert::IsTrue(pkb.isUses(3, "x"));
			Assert::IsTrue(pkb.isUses(1, "w"));

			Assert::IsTrue(pkb.isUses("main", "a1"));
			Assert::IsTrue(pkb.isUses("main", "a2"));
			Assert::IsTrue(pkb.isUses("main", "b2"));
			Assert::IsTrue(pkb.isUses("main", "b3"));
			Assert::IsTrue(pkb.isUses("main", "c2"));
			Assert::IsTrue(pkb.isUses("main", "d2"));
			Assert::IsTrue(pkb.isUses("main", "d3"));
			Assert::IsTrue(pkb.isUses("main", "w"));
			Assert::IsTrue(pkb.isUses("main", "x"));

		}
		TEST_METHOD(parseSource2_addToPKB)
		{
			Parser parser;
			std::string filename = "../IntegrationTesting/IntegrationTest_Source_2.txt";
			PKB pkb = parser.parseFile(filename);
			
			//Test FollowsTransitive
			Assert::IsTrue(pkb.isFollowsTransitive(4, 5));
			Assert::IsTrue(pkb.isFollowsTransitive(4, 6));
			Assert::IsTrue(pkb.isFollowsTransitive(4, 7));

			Assert::IsTrue(pkb.isFollowsTransitive(5, 6));
			Assert::IsTrue(pkb.isFollowsTransitive(5, 7));

			Assert::IsTrue(pkb.isFollowsTransitive(6, 7));


			//Test ParentsTransitive
			Assert::IsTrue(pkb.isParentTransitive(1, 2));
			Assert::IsTrue(pkb.isParentTransitive(1, 3));
			Assert::IsTrue(pkb.isParentTransitive(1, 4));
			Assert::IsTrue(pkb.isParentTransitive(1, 5));
			Assert::IsTrue(pkb.isParentTransitive(1, 6));
			Assert::IsTrue(pkb.isParentTransitive(1, 7));

			Assert::IsTrue(pkb.isParentTransitive(2, 3));
			Assert::IsTrue(pkb.isParentTransitive(2, 4));
			Assert::IsTrue(pkb.isParentTransitive(2, 5));
			Assert::IsTrue(pkb.isParentTransitive(2, 6));
			Assert::IsTrue(pkb.isParentTransitive(2, 7));

			Assert::IsTrue(pkb.isParentTransitive(3, 4));
			Assert::IsTrue(pkb.isParentTransitive(3, 5));
			Assert::IsTrue(pkb.isParentTransitive(3, 6));
			Assert::IsTrue(pkb.isParentTransitive(3, 7));
		}

	};
}