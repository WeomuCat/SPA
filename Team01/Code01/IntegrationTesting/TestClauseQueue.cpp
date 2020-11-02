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

			CLAUSE_PTR_LIST all_clauses = { &such_that1 };

			ClauseQueue clause_queue;
			clause_queue.addAllClauses(all_clauses);
			CLAUSE first_clause = clause_queue.pop();
			Assert::IsTrue(QueryNode::isEquals(such_that1, first_clause));
		}

		TEST_METHOD(pop_2_Rank1_Rank10_success)
		{
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

			CLAUSE_PTR_LIST all_clauses = { &such_that1, &such_that2 };


			ClauseQueue clause_queue;
			clause_queue.addAllClauses(all_clauses);

			CLAUSE first_clause = clause_queue.pop();
			Assert::IsTrue(QueryNode::isEquals(such_that2, first_clause));

			CLAUSE second_clause = clause_queue.pop();
			Assert::IsTrue(QueryNode::isEquals(such_that1, second_clause));

		}
	};

}