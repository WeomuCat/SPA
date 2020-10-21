#include "stdafx.h"
#include "CppUnitTest.h"

#include "../source/PKB.h"
#include "../source/ResultListManager.h"
#include "../source/QueryNode.h"
#include "../source/QuerySynonymType.h"


using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace IntegrationTesting
{
	TEST_CLASS(TestResultListManager)
	{
	public:
		/*
		procedure woof {
			call master;		1
			read meow;			2
			print paw;			3
			call daddy;			4
			call mummy;			5
			read meowmeow;		6
			print pawpaw;		7
		}
		
		*/
		std::shared_ptr<PKB> pkb;

		std::shared_ptr<PROCESSED_SYNONYMS> processed_synonyms;

		TEST_METHOD_INITIALIZE(PKBInitialize) {

			{
				PKBBuilder builder;

				std::shared_ptr<ProcedureNode> proc = std::make_shared<ProcedureNode>();
				proc->setProcedureName("woof");
				builder.addProcedureNode(proc);

				// Add Variable nodes
				std::shared_ptr<VariableNode> v1 = std::make_shared<VariableNode>();
				std::shared_ptr<VariableNode> v2 = std::make_shared<VariableNode>();
				std::shared_ptr<VariableNode> v3 = std::make_shared<VariableNode>();
				std::shared_ptr<VariableNode> v4 = std::make_shared<VariableNode>();
				std::shared_ptr<VariableNode> v5 = std::make_shared<VariableNode>();
				std::shared_ptr<VariableNode> v6 = std::make_shared<VariableNode>();
				std::shared_ptr<VariableNode> v7 = std::make_shared<VariableNode>();

				v1->setVariableName("master");
				v2->setVariableName("meow");
				v3->setVariableName("paw");
				v4->setVariableName("daddy");
				v5->setVariableName("mummy");
				v6->setVariableName("meowmeow");
				v7->setVariableName("pawpaw");

				builder.addVariableNode(v1);
				builder.addVariableNode(v2);
				builder.addVariableNode(v3);
				builder.addVariableNode(v4);
				builder.addVariableNode(v5);
				builder.addVariableNode(v6);
				builder.addVariableNode(v7);

				// Add Print nodes
				std::shared_ptr<PrintNode> p1 = std::make_shared<PrintNode>();
				std::shared_ptr<PrintNode> p2 = std::make_shared<PrintNode>();
				p1->setStatementNumber(3);
				p1->setVariableNode(v3);
				p2->setStatementNumber(7);
				p2->setVariableNode(v7);

				// Add Read nodes
				std::shared_ptr<ReadNode> r1 = std::make_shared<ReadNode>();
				std::shared_ptr<ReadNode> r2 = std::make_shared<ReadNode>();
				r1->setStatementNumber(2);
				r1->setVariableNode(v2);
				r1->setStatementNumber(6);
				r1->setVariableNode(v6);

				// Add Call nodes
				std::shared_ptr<CallNode> c1 = std::make_shared<CallNode>();
				std::shared_ptr<CallNode> c2 = std::make_shared<CallNode>();
				std::shared_ptr<CallNode> c3 = std::make_shared<CallNode>();
				c1->setCallerProcedureName("woof");
				c1->setCalleeProcedureName("master");
				c1->setStatementNumber(1);
				c2->setCallerProcedureName("woof");
				c2->setCalleeProcedureName("daddy");
				c2->setStatementNumber(4);
				c3->setCallerProcedureName("woof");
				c3->setCalleeProcedureName("mummy");
				c3->setStatementNumber(5);

				// Add Statement nodes
				std::shared_ptr<StatementNode> s1 = std::make_shared<StatementNode>();
				std::shared_ptr<StatementNode> s2 = std::make_shared<StatementNode>();
				std::shared_ptr<StatementNode> s3 = std::make_shared<StatementNode>();
				std::shared_ptr<StatementNode> s4 = std::make_shared<StatementNode>();
				std::shared_ptr<StatementNode> s5 = std::make_shared<StatementNode>();
				std::shared_ptr<StatementNode> s6 = std::make_shared<StatementNode>();
				std::shared_ptr<StatementNode> s7 = std::make_shared<StatementNode>();
				s1->setStatementNumber(1);
				s2->setStatementNumber(2);
				s3->setStatementNumber(3);
				s4->setStatementNumber(4);
				s5->setStatementNumber(5);
				s6->setStatementNumber(6);
				s7->setStatementNumber(7);

				builder.addStatementNode(s1);
				builder.addStatementNode(s2);
				builder.addStatementNode(s3);
				builder.addStatementNode(s4);
				builder.addStatementNode(s5);
				builder.addStatementNode(s6);
				builder.addStatementNode(s7);

				pkb = std::make_shared<PKB>(builder.build());
			}

			{
				QueryNode proc_node = QueryNode();
				proc_node.setSynonymNode({ QuerySynonymType::procedure }, "p");
				QueryNode call_node = QueryNode();
				call_node.setSynonymNode({ QuerySynonymType::call }, "c");
				QueryNode read_node = QueryNode();
				read_node.setSynonymNode({ QuerySynonymType::read }, "r");
				QueryNode print_node = QueryNode();
				print_node.setSynonymNode({ QuerySynonymType::print }, "pn");
				PROCESSED_SYNONYMS processed_synonyms1 = { {"p", proc_node}, {"c", call_node}, {"r", read_node}, {"pn", print_node} };

				processed_synonyms = std::make_shared<PROCESSED_SYNONYMS>(processed_synonyms1);
			}
		}

		TEST_METHOD(merge_CallVarname_CallStmtnum_SameValues_Success)
		{
			ResultList r1;
			SYNONYM_VALUES_LIST values1 = { "1", "4", "5" };
			r1.addColumn("c", values1);

			ResultList r2;
			SYNONYM_VALUES_LIST values2 = { "master", "daddy", "mummy" };
			r2.addColumn("c", values2);

			ResultList result = ResultListManager::merge(r1, r2, *processed_synonyms, *pkb);

			int num_rows = result.getNumRows();
			int num_cols = result.getNumColumns();

			// Log results
			std::string mes1 = "numRows: " + std::to_string(num_rows) + "\n";
			std::string mes2 = "numCols: " + std::to_string(num_cols) + "\n";
			std::string result_string = "Result: " + ResultListManager::getStringValues(result.getValuesOfSynonym("c"));
			Logger::WriteMessage(mes1.c_str());
			Logger::WriteMessage(mes2.c_str());
			Logger::WriteMessage(result_string.c_str());

			int expected_num_rows = 3;
			int expected_num_cols = 1;
			std::string expected_result = "1, 4, 5";

			Assert::IsTrue(num_rows == expected_num_rows);
			Assert::IsTrue(num_cols == expected_num_cols);
			Assert::IsTrue(result_string.compare(expected_result) == 0);
		}

	};
}