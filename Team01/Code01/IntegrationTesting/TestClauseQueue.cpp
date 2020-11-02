#include "stdafx.h"
#include "CppUnitTest.h"

#include "../source/ClauseQueue.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace IntegrationTesting
{
	TEST_CLASS(TestClauseQueue)
	{
	public:
		TEST_METHOD(pop_1_success)
		{
			// Testing single pop
			// Clause List: such that NextT(1,3)
			// Clause order: such that NextT(1,3)
			
			// such that 
			QueryNode such_that1 = QueryNode();
			such_that1.setNodeType({ QueryNodeType::such_that });

			// NextT
			QueryNode clause1 = QueryNode();
			clause1.setNodeType({ QueryNodeType::nextT });

			// arg 1: 1
			QueryNode clause1_child1 = QueryNode();
			clause1_child1.setIntegerNode(1);

			// arg 2: 3
			QueryNode clause1_child2 = QueryNode();
			clause1_child2.setIntegerNode(3);

			// NextT (1, 3)
			QueryNode clause1_children[] = { clause1_child1, clause1_child2 };
			clause1.setChildren(clause1_children, 2);
			
			// such that NextT(1, 3)
			such_that1.addChild(clause1);


			/*================= Testing ====================*/

			CLAUSE_LIST all_clauses = { such_that1 };

			ClauseQueue clause_queue;
			clause_queue.addAllClauses(all_clauses);
			CLAUSE first_clause = clause_queue.pop();
			Assert::IsTrue(QueryNode::isEquals(such_that1, first_clause));
		}

		TEST_METHOD(pop_2_NextTs3_NextT13_success)
		{
			// Testing multiple pops
			// Clause List: such that NextT(s,3) such that NextT(1,3) 
			// Clause order: such that NextT(1,3) such that NextT(s,3)

			/*================= Clause 1 ====================*/

			// such that 
			QueryNode such_that1 = QueryNode();
			such_that1.setNodeType({ QueryNodeType::such_that });

			// NextT
			QueryNode clause1 = QueryNode();
			clause1.setNodeType({ QueryNodeType::nextT });

			// arg 1: s
			QueryNode clause1_child1 = QueryNode();
			clause1_child1.setSynonymNode({ QuerySynonymType::stmt }, "s");

			// arg 2: 3
			QueryNode clause1_child2 = QueryNode();
			clause1_child2.setIntegerNode(3);

			// NextT (s, 3)
			QueryNode clause1_children[] = { clause1_child1, clause1_child2 };
			clause1.setChildren(clause1_children, 2);

			// such that NextT(s, 3)
			such_that1.addChild(clause1);

			/*================= Clause 2 ====================*/

			// such that 
			QueryNode such_that2 = QueryNode();
			such_that2.setNodeType({ QueryNodeType::such_that });

			// NextT
			QueryNode clause2 = QueryNode();
			clause2.setNodeType({ QueryNodeType::nextT });

			// arg 1: 1
			QueryNode clause2_child1 = QueryNode();
			clause2_child1.setIntegerNode(1);

			// arg 2: 3
			QueryNode clause2_child2 = QueryNode();
			clause2_child2.setIntegerNode(3);

			// NextT (1, 3)
			QueryNode clause2_children[] = { clause2_child1, clause2_child2 };
			clause2.setChildren(clause2_children, 2);

			// such that NextT(1, 3)
			such_that2.addChild(clause2);


			/*================= Testing ====================*/

			CLAUSE_LIST all_clauses = { such_that1, such_that2 };


			ClauseQueue clause_queue;
			clause_queue.addAllClauses(all_clauses);

			CLAUSE first_clause = clause_queue.pop();
			Assert::IsTrue(QueryNode::isEquals(such_that2, first_clause));

			CLAUSE second_clause = clause_queue.pop();
			Assert::IsTrue(QueryNode::isEquals(such_that1, second_clause));

		}

		TEST_METHOD(pop_3_NextTs3_NextT13_FollowsTs3_success)
		{
			// Testing Relationship rank
			// Clause List: such that NextT(s,3) such that NextT(1,3) such that FollowsT(s,3)
			// Clause order: such that NextT(1,3) such that FollowsT(s, 3) such that NextT(s,3)

			/*================= Clause 1 ====================*/

			// such that 
			QueryNode such_that1 = QueryNode();
			such_that1.setNodeType({ QueryNodeType::such_that });

			// NextT
			QueryNode clause1 = QueryNode();
			clause1.setNodeType({ QueryNodeType::nextT });

			// arg 1: s
			QueryNode clause1_child1 = QueryNode();
			clause1_child1.setSynonymNode({ QuerySynonymType::stmt }, "s");

			// arg 2: 3
			QueryNode clause1_child2 = QueryNode();
			clause1_child2.setIntegerNode(3);

			// NextT (s, 3)
			QueryNode clause1_children[] = { clause1_child1, clause1_child2 };
			clause1.setChildren(clause1_children, 2);

			// such that NextT(s, 3)
			such_that1.addChild(clause1);

			/*================= Clause 2 ====================*/

			// such that 
			QueryNode such_that2 = QueryNode();
			such_that2.setNodeType({ QueryNodeType::such_that });

			// NextT
			QueryNode clause2 = QueryNode();
			clause2.setNodeType({ QueryNodeType::nextT });

			// arg 1: 1
			QueryNode clause2_child1 = QueryNode();
			clause2_child1.setIntegerNode(1);

			// arg 2: 3
			QueryNode clause2_child2 = QueryNode();
			clause2_child2.setIntegerNode(3);

			// NextT (1, 3)
			QueryNode clause2_children[] = { clause2_child1, clause2_child2 };
			clause2.setChildren(clause2_children, 2);

			// such that NextT(1, 3)
			such_that2.addChild(clause2);

			/*================= Clause 3 ====================*/

			// such that 
			QueryNode such_that3 = QueryNode();
			such_that3.setNodeType({ QueryNodeType::such_that });

			// FollowsT
			QueryNode clause3 = QueryNode();
			clause3.setNodeType({ QueryNodeType::followsT });

			// arg 1: s
			QueryNode clause3_child1 = QueryNode();
			clause3_child1.setSynonymNode({ QuerySynonymType::stmt }, "s");

			// arg 2: 3
			QueryNode clause3_child2 = QueryNode();
			clause3_child2.setIntegerNode(3);

			// FollowsT (s, 3)
			QueryNode clause3_children[] = { clause3_child1, clause3_child2 };
			clause3.setChildren(clause3_children, 2);

			// such that FollowsT(s,3)
			such_that3.addChild(clause3);


			/*================= Testing ====================*/

			CLAUSE_LIST all_clauses = { such_that1, such_that2, such_that3 };


			ClauseQueue clause_queue;
			clause_queue.addAllClauses(all_clauses);

			CLAUSE first_clause = clause_queue.pop();
			Assert::IsTrue(QueryNode::isEquals(such_that2, first_clause));

			CLAUSE second_clause = clause_queue.pop();
			Assert::IsTrue(QueryNode::isEquals(such_that3, second_clause));

			CLAUSE third_clause = clause_queue.pop();
			Assert::IsTrue(QueryNode::isEquals(such_that1, third_clause));
		}


		TEST_METHOD(pop_4_NextTs3_NextTss2_FollowsTs23_NextTs33_success)
		{
			// Testing common synonyms
			// Clause List: such that NextT(s,3) such that NextT(s,s2) such that FollowsT(s2,3) such that NextT(s3, 3)
			// Clause order: such that FollowsT(s2,3) such that NextT(s, s2) such that NextT(s,3) such that NextT(s3,3)

			/*================= Clause 1 ====================*/

			// such that 
			QueryNode such_that1 = QueryNode();
			such_that1.setNodeType({ QueryNodeType::such_that });

			// NextT
			QueryNode clause1 = QueryNode();
			clause1.setNodeType({ QueryNodeType::nextT });

			// arg 1: s
			QueryNode clause1_child1 = QueryNode();
			clause1_child1.setSynonymNode({ QuerySynonymType::stmt }, "s");

			// arg 2: 3
			QueryNode clause1_child2 = QueryNode();
			clause1_child2.setIntegerNode(3);

			// NextT (s, 3)
			QueryNode clause1_children[] = { clause1_child1, clause1_child2 };
			clause1.setChildren(clause1_children, 2);

			// such that NextT(s, 3)
			such_that1.addChild(clause1);

			/*================= Clause 2 ====================*/

			// such that 
			QueryNode such_that2 = QueryNode();
			such_that2.setNodeType({ QueryNodeType::such_that });

			// NextT
			QueryNode clause2 = QueryNode();
			clause2.setNodeType({ QueryNodeType::nextT });

			// arg 1: s
			QueryNode clause2_child1 = QueryNode();
			clause2_child1.setSynonymNode({ QuerySynonymType::stmt }, "s");

			// arg 2: s2
			QueryNode clause2_child2 = QueryNode();
			clause2_child2.setSynonymNode({ QuerySynonymType::stmt }, "s2");

			// NextT (s, s2)
			QueryNode clause2_children[] = { clause2_child1, clause2_child2 };
			clause2.setChildren(clause2_children, 2);

			// such that NextT(s, s2)
			such_that2.addChild(clause2);

			/*================= Clause 3 ====================*/

			// such that 
			QueryNode such_that3 = QueryNode();
			such_that3.setNodeType({ QueryNodeType::such_that });

			// FollowsT
			QueryNode clause3 = QueryNode();
			clause3.setNodeType({ QueryNodeType::followsT });

			// arg 1: s2
			QueryNode clause3_child1 = QueryNode();
			clause3_child1.setSynonymNode({ QuerySynonymType::stmt }, "s2");

			// arg 2: 3
			QueryNode clause3_child2 = QueryNode();
			clause3_child2.setIntegerNode(3);

			// FollowsT (s, 3)
			QueryNode clause3_children[] = { clause3_child1, clause3_child2 };
			clause3.setChildren(clause3_children, 2);

			// such that FollowsT(s,3)
			such_that3.addChild(clause3);

			/*================= Clause 4 ====================*/

			// such that 
			QueryNode such_that4 = QueryNode();
			such_that4.setNodeType({ QueryNodeType::such_that });

			// FollowsT
			QueryNode clause4 = QueryNode();
			clause4.setNodeType({ QueryNodeType::nextT });

			// arg 1: s3
			QueryNode clause4_child1 = QueryNode();
			clause4_child1.setSynonymNode({ QuerySynonymType::stmt }, "s3");

			// arg 2: 3
			QueryNode clause4_child2 = QueryNode();
			clause4_child2.setIntegerNode(3);

			// NextT (s3, 3)
			QueryNode clause4_children[] = { clause4_child1, clause4_child2 };
			clause4.setChildren(clause4_children, 2);

			// such that NextT(s3,3)
			such_that4.addChild(clause4);


			/*================= Testing ====================*/

			CLAUSE_LIST all_clauses = { such_that1, such_that2, such_that3, such_that4};


			ClauseQueue clause_queue;
			clause_queue.addAllClauses(all_clauses);

			CLAUSE first_clause = clause_queue.pop();
			Assert::IsTrue(QueryNode::isEquals(such_that3, first_clause));

			CLAUSE second_clause = clause_queue.pop();
			Assert::IsTrue(QueryNode::isEquals(such_that2, second_clause));

			CLAUSE third_clause = clause_queue.pop();
			Assert::IsTrue(QueryNode::isEquals(such_that1, third_clause));

			CLAUSE forth_clause = clause_queue.pop();
			Assert::IsTrue(QueryNode::isEquals(such_that4, forth_clause));
		}
	};

}