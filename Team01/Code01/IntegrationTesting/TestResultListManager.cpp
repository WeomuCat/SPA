#include "stdafx.h"
#include "CppUnitTest.h"

#include "../source/PKB/PKB.h"
#include "../source/PKB/PKBBuilder.h"
#include "../source/QP/ResultListManager.h"
#include "../source/QP/QueryNode.h"
#include "../source/QP/QuerySynonymType.h"


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

		TEST_METHOD_INITIALIZE(Initialize) {

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
				builder.addPrintNode(p1);
				builder.addPrintNode(p2);

				// Add Read nodes
				std::shared_ptr<ReadNode> r1 = std::make_shared<ReadNode>();
				std::shared_ptr<ReadNode> r2 = std::make_shared<ReadNode>();
				r1->setStatementNumber(2);
				r1->setVariableNode(v2);
				r2->setStatementNumber(6);
				r2->setVariableNode(v6);
				builder.addReadNode(r1);
				builder.addReadNode(r2);

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
				builder.addCallNode(c1);
				builder.addCallNode(c2);
				builder.addCallNode(c3);


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

		TEST_METHOD(merge_OneCol_Empty_Empty_Success)
		{
			ResultList r1;
			SYNONYM_VALUES_LIST values1 = {};
			r1.addColumn("c", values1);

			ResultList r2;
			SYNONYM_VALUES_LIST values2 = {};
			r2.addColumn("c", values2);

			ResultList result = ResultListManager::merge(r1, r2, *processed_synonyms, *pkb);

			int num_rows = result.getNumRows();
			int num_cols = result.getNumColumns();
			std::string result_string = ResultListManager::getStringValues(result.getValuesOfSynonym("c"));

			// Log results
			std::string mes1 = "numRows: " + std::to_string(num_rows) + "\n";
			std::string mes2 = "numCols: " + std::to_string(num_cols) + "\n";
			std::string print_result_string = "Result: " + result_string;
			Logger::WriteMessage(mes1.c_str());
			Logger::WriteMessage(mes2.c_str());
			Logger::WriteMessage(print_result_string.c_str());

			int expected_num_rows = 0;
			int expected_num_cols = 1;
			std::string expected_result = "";

			Assert::IsTrue(num_rows == expected_num_rows);
			Assert::IsTrue(num_cols == expected_num_cols);
			Assert::IsTrue(result_string.compare(expected_result) == 0);
		}

		TEST_METHOD(merge_CallVarname_Empty_Success)
		{
			ResultList r1;
			SYNONYM_VALUES_LIST values1 = {};
			r1.addColumn("c", values1);

			ResultList r2;
			SYNONYM_VALUES_LIST values2 = { "master", "mummy" };
			r2.addColumn("c", values2);

			ResultList result = ResultListManager::merge(r1, r2, *processed_synonyms, *pkb);

			int num_rows = result.getNumRows();
			int num_cols = result.getNumColumns();
			std::string result_string = ResultListManager::getStringValues(result.getValuesOfSynonym("c"));

			// Log results
			std::string mes1 = "numRows: " + std::to_string(num_rows) + "\n";
			std::string mes2 = "numCols: " + std::to_string(num_cols) + "\n";
			std::string print_result_string = "Result: " + result_string;
			Logger::WriteMessage(mes1.c_str());
			Logger::WriteMessage(mes2.c_str());
			Logger::WriteMessage(print_result_string.c_str());

			int expected_num_rows = 0;
			int expected_num_cols = 1;
			std::string expected_result = "";

			Assert::IsTrue(num_rows == expected_num_rows);
			Assert::IsTrue(num_cols == expected_num_cols);
			Assert::IsTrue(result_string.compare(expected_result) == 0);
		}

		TEST_METHOD(merge_ReadVarname_Empty_Success)
		{
			SYNONYM_NAME target_synonym = "r";

			ResultList r1;
			SYNONYM_VALUES_LIST values1 = { };
			r1.addColumn(target_synonym, values1);

			ResultList r2;
			SYNONYM_VALUES_LIST values2 = { "meowmeow", "meow" };
			r2.addColumn(target_synonym, values2);

			ResultList result = ResultListManager::merge(r1, r2, *processed_synonyms, *pkb);

			int num_rows = result.getNumRows();
			int num_cols = result.getNumColumns();
			std::string result_string = ResultListManager::getStringValues(result.getValuesOfSynonym(target_synonym));

			// Log results
			std::string mes1 = "numRows: " + std::to_string(num_rows) + "\n";
			std::string mes2 = "numCols: " + std::to_string(num_cols) + "\n";
			std::string print_result_string = "Result: " + result_string;
			Logger::WriteMessage(mes1.c_str());
			Logger::WriteMessage(mes2.c_str());
			Logger::WriteMessage(print_result_string.c_str());

			int expected_num_rows = 0;
			int expected_num_cols = 1;
			std::string expected_result = "";

			Assert::IsTrue(num_rows == expected_num_rows);
			Assert::IsTrue(num_cols == expected_num_cols);
			Assert::IsTrue(result_string.compare(expected_result) == 0);
		}

		TEST_METHOD(merge_PrintVarname_Empty_Success)
		{
			SYNONYM_NAME target_synonym = "pn";

			ResultList r1;
			SYNONYM_VALUES_LIST values1 = {};
			r1.addColumn(target_synonym, values1);

			ResultList r2;
			SYNONYM_VALUES_LIST values2 = { "paw", "pawpaw" };
			r2.addColumn(target_synonym, values2);

			ResultList result = ResultListManager::merge(r1, r2, *processed_synonyms, *pkb);

			int num_rows = result.getNumRows();
			int num_cols = result.getNumColumns();
			std::string result_string = ResultListManager::getStringValues(result.getValuesOfSynonym(target_synonym));

			// Log results
			std::string mes1 = "numRows: " + std::to_string(num_rows) + "\n";
			std::string mes2 = "numCols: " + std::to_string(num_cols) + "\n";
			std::string print_result_string = "Result: " + result_string;
			Logger::WriteMessage(mes1.c_str());
			Logger::WriteMessage(mes2.c_str());
			Logger::WriteMessage(print_result_string.c_str());

			int expected_num_rows = 0;
			int expected_num_cols = 1;
			std::string expected_result = "";

			Assert::IsTrue(num_rows == expected_num_rows);
			Assert::IsTrue(num_cols == expected_num_cols);
			Assert::IsTrue(result_string.compare(expected_result) == 0);
		}

		TEST_METHOD(merge_CallVarname_CallVarname_SameCol_DifferentNumRows_SameValues_Success)
		{
			ResultList r1;
			SYNONYM_VALUES_LIST values1 = { "mummy" };
			r1.addColumn("c", values1);

			ResultList r2;
			SYNONYM_VALUES_LIST values2 = { "master", "daddy", "mummy" };
			r2.addColumn("c", values2);

			ResultList result = ResultListManager::merge(r1, r2, *processed_synonyms, *pkb);

			int num_rows = result.getNumRows();
			int num_cols = result.getNumColumns();
			std::string result_string = ResultListManager::getStringValues(result.getValuesOfSynonym("c"));

			// Log results
			std::string mes1 = "numRows: " + std::to_string(num_rows) + "\n";
			std::string mes2 = "numCols: " + std::to_string(num_cols) + "\n";
			std::string print_result_string = "Result: " + result_string;
			Logger::WriteMessage(mes1.c_str());
			Logger::WriteMessage(mes2.c_str());
			Logger::WriteMessage(print_result_string.c_str());

			int expected_num_rows = 1;
			int expected_num_cols = 1;
			std::string expected_result = "5";

			Assert::IsTrue(num_rows == expected_num_rows);
			Assert::IsTrue(num_cols == expected_num_cols);
			Assert::IsTrue(result_string.compare(expected_result) == 0);
		}

		TEST_METHOD(merge_ReadVarname_ReadVarname_SameCol_DifferentNumRows_SameValues_Success)
		{
			SYNONYM_NAME target_synonym = "r";

			ResultList r1;
			SYNONYM_VALUES_LIST values1 = { "meow" };
			r1.addColumn(target_synonym, values1);

			ResultList r2;
			SYNONYM_VALUES_LIST values2 = { "meowmeow", "meow" };
			r2.addColumn(target_synonym, values2);

			ResultList result = ResultListManager::merge(r1, r2, *processed_synonyms, *pkb);

			int num_rows = result.getNumRows();
			int num_cols = result.getNumColumns();
			std::string result_string = ResultListManager::getStringValues(result.getValuesOfSynonym(target_synonym));

			// Log results
			std::string mes1 = "numRows: " + std::to_string(num_rows) + "\n";
			std::string mes2 = "numCols: " + std::to_string(num_cols) + "\n";
			std::string print_result_string = "Result: " + result_string;
			Logger::WriteMessage(mes1.c_str());
			Logger::WriteMessage(mes2.c_str());
			Logger::WriteMessage(print_result_string.c_str());

			int expected_num_rows = 1;
			int expected_num_cols = 1;
			std::string expected_result = "2";

			Assert::IsTrue(num_rows == expected_num_rows);
			Assert::IsTrue(num_cols == expected_num_cols);
			Assert::IsTrue(result_string.compare(expected_result) == 0);
		}

		TEST_METHOD(merge_PrintVarname_PrintVarname_SameCol_DifferentNumRows_SameValues_Success)
		{
			SYNONYM_NAME target_synonym = "pn";

			ResultList r1;
			SYNONYM_VALUES_LIST values1 = { "pawpaw" };
			r1.addColumn(target_synonym, values1);

			ResultList r2;
			SYNONYM_VALUES_LIST values2 = { "paw", "pawpaw" };
			r2.addColumn(target_synonym, values2);

			ResultList result = ResultListManager::merge(r1, r2, *processed_synonyms, *pkb);

			int num_rows = result.getNumRows();
			int num_cols = result.getNumColumns();
			std::string result_string = ResultListManager::getStringValues(result.getValuesOfSynonym(target_synonym));

			// Log results
			std::string mes1 = "numRows: " + std::to_string(num_rows) + "\n";
			std::string mes2 = "numCols: " + std::to_string(num_cols) + "\n";
			std::string print_result_string = "Result: " + result_string;
			Logger::WriteMessage(mes1.c_str());
			Logger::WriteMessage(mes2.c_str());
			Logger::WriteMessage(print_result_string.c_str());

			int expected_num_rows = 1;
			int expected_num_cols = 1;
			std::string expected_result = "7";

			Assert::IsTrue(num_rows == expected_num_rows);
			Assert::IsTrue(num_cols == expected_num_cols);
			Assert::IsTrue(result_string.compare(expected_result) == 0);
		}

		TEST_METHOD(merge_CallVarname_CallStmtnum_SameCol_SameNumRows_SameValues_Success)
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
			std::string result_string = ResultListManager::getStringValues(result.getValuesOfSynonym("c"));

			// Log results
			std::string mes1 = "numRows: " + std::to_string(num_rows) + "\n";
			std::string mes2 = "numCols: " + std::to_string(num_cols) + "\n";
			std::string print_result_string = "Result: " + result_string;
			Logger::WriteMessage(mes1.c_str());
			Logger::WriteMessage(mes2.c_str());
			Logger::WriteMessage(print_result_string.c_str());

			int expected_num_rows = 3;
			int expected_num_cols = 1;
			std::string expected_result = "1, 4, 5";

			Assert::IsTrue(num_rows == expected_num_rows);
			Assert::IsTrue(num_cols == expected_num_cols);
			Assert::IsTrue(result_string.compare(expected_result) == 0);
		}

		TEST_METHOD(merge_ReadVarname_ReadStmtnum_SameCol_DifferentNumRows_SameValues_Success)
		{
			SYNONYM_NAME target_synonym = "r";

			ResultList r1;
			SYNONYM_VALUES_LIST values1 = { "6", "2" };
			r1.addColumn(target_synonym, values1);

			ResultList r2;
			SYNONYM_VALUES_LIST values2 = { "meowmeow", "meow" };
			r2.addColumn(target_synonym, values2);

			ResultList result = ResultListManager::merge(r1, r2, *processed_synonyms, *pkb);

			int num_rows = result.getNumRows();
			int num_cols = result.getNumColumns();
			std::string result_string = ResultListManager::getStringValues(result.getValuesOfSynonym(target_synonym));

			// Log results
			std::string mes1 = "numRows: " + std::to_string(num_rows) + "\n";
			std::string mes2 = "numCols: " + std::to_string(num_cols) + "\n";
			std::string print_result_string = "Result: " + result_string;
			Logger::WriteMessage(mes1.c_str());
			Logger::WriteMessage(mes2.c_str());
			Logger::WriteMessage(print_result_string.c_str());

			int expected_num_rows = 2;
			int expected_num_cols = 1;
			std::string expected_result = "6, 2";

			Assert::IsTrue(num_rows == expected_num_rows);
			Assert::IsTrue(num_cols == expected_num_cols);
			Assert::IsTrue(result_string.compare(expected_result) == 0);
		}

		TEST_METHOD(merge_PrintVarname_PrintStmtnum_SameCol_DifferentNumRows_SameValues_Success)
		{
			SYNONYM_NAME target_synonym = "pn";

			ResultList r1;
			SYNONYM_VALUES_LIST values1 = { "3", "7" };
			r1.addColumn(target_synonym, values1);

			ResultList r2;
			SYNONYM_VALUES_LIST values2 = { "paw", "pawpaw" };
			r2.addColumn(target_synonym, values2);

			ResultList result = ResultListManager::merge(r1, r2, *processed_synonyms, *pkb);

			int num_rows = result.getNumRows();
			int num_cols = result.getNumColumns();
			std::string result_string = ResultListManager::getStringValues(result.getValuesOfSynonym(target_synonym));

			// Log results
			std::string mes1 = "numRows: " + std::to_string(num_rows) + "\n";
			std::string mes2 = "numCols: " + std::to_string(num_cols) + "\n";
			std::string print_result_string = "Result: " + result_string;
			Logger::WriteMessage(mes1.c_str());
			Logger::WriteMessage(mes2.c_str());
			Logger::WriteMessage(print_result_string.c_str());

			int expected_num_rows = 2;
			int expected_num_cols = 1;
			std::string expected_result = "3, 7";

			Assert::IsTrue(num_rows == expected_num_rows);
			Assert::IsTrue(num_cols == expected_num_cols);
			Assert::IsTrue(result_string.compare(expected_result) == 0);
		}

		TEST_METHOD(merge_CallVarname_CallStmtnum_SameCol_SameNumRows_DifferentValues_Success)
		{
			ResultList r1;
			SYNONYM_VALUES_LIST values1 = { "1", "5", "4" };
			r1.addColumn("c", values1);

			ResultList r2;
			SYNONYM_VALUES_LIST values2 = { "master", "daddy", "mummy" };
			r2.addColumn("c", values2);

			ResultList result = ResultListManager::merge(r1, r2, *processed_synonyms, *pkb);

			int num_rows = result.getNumRows();
			int num_cols = result.getNumColumns();
			std::string result_string = ResultListManager::getStringValues(result.getValuesOfSynonym("c"));

			// Log results
			std::string mes1 = "numRows: " + std::to_string(num_rows) + "\n";
			std::string mes2 = "numCols: " + std::to_string(num_cols) + "\n";
			std::string print_result_string = "Result: " + result_string;
			Logger::WriteMessage(mes1.c_str());
			Logger::WriteMessage(mes2.c_str());
			Logger::WriteMessage(print_result_string.c_str());

			int expected_num_rows = 3;
			int expected_num_cols = 1;
			std::string expected_result = "1, 5, 4";

			Assert::IsTrue(num_rows == expected_num_rows);
			Assert::IsTrue(num_cols == expected_num_cols);
			Assert::IsTrue(result_string.compare(expected_result) == 0);
		}

		TEST_METHOD(merge_CallVarname_CallStmtnum_SameCol_DifferentNumRows_DifferentValues_Success)
		{
			ResultList r1;
			SYNONYM_VALUES_LIST values1 = { "1", "5", "4" };
			r1.addColumn("c", values1);

			ResultList r2;
			SYNONYM_VALUES_LIST values2 = { "master" };
			r2.addColumn("c", values2);

			ResultList result = ResultListManager::merge(r1, r2, *processed_synonyms, *pkb);

			int num_rows = result.getNumRows();
			int num_cols = result.getNumColumns();
			std::string result_string = ResultListManager::getStringValues(result.getValuesOfSynonym("c"));

			// Log results
			std::string mes1 = "numRows: " + std::to_string(num_rows) + "\n";
			std::string mes2 = "numCols: " + std::to_string(num_cols) + "\n";
			std::string print_result_string = "Result: " + result_string;
			Logger::WriteMessage(mes1.c_str());
			Logger::WriteMessage(mes2.c_str());
			Logger::WriteMessage(print_result_string.c_str());

			int expected_num_rows = 1;
			int expected_num_cols = 1;
			std::string expected_result = "1";

			Assert::IsTrue(num_rows == expected_num_rows);
			Assert::IsTrue(num_cols == expected_num_cols);
			Assert::IsTrue(result_string.compare(expected_result) == 0);
		}

		TEST_METHOD(merge_ReadVarname_ReadStmtnum_SameCol_DifferentNumRows_DifferentValues_Success)
		{
			SYNONYM_NAME target_synonym = "r";

			ResultList r1;
			SYNONYM_VALUES_LIST values1 = { "6" };
			r1.addColumn(target_synonym, values1);

			ResultList r2;
			SYNONYM_VALUES_LIST values2 = { "meowmeow", "meow" };
			r2.addColumn(target_synonym, values2);

			ResultList result = ResultListManager::merge(r1, r2, *processed_synonyms, *pkb);

			int num_rows = result.getNumRows();
			int num_cols = result.getNumColumns();
			std::string result_string = ResultListManager::getStringValues(result.getValuesOfSynonym(target_synonym));

			// Log results
			std::string mes1 = "numRows: " + std::to_string(num_rows) + "\n";
			std::string mes2 = "numCols: " + std::to_string(num_cols) + "\n";
			std::string print_result_string = "Result: " + result_string;
			Logger::WriteMessage(mes1.c_str());
			Logger::WriteMessage(mes2.c_str());
			Logger::WriteMessage(print_result_string.c_str());

			int expected_num_rows = 1;
			int expected_num_cols = 1;
			std::string expected_result = "6";

			Assert::IsTrue(num_rows == expected_num_rows);
			Assert::IsTrue(num_cols == expected_num_cols);
			Assert::IsTrue(result_string.compare(expected_result) == 0);
		}

		TEST_METHOD(merge_PrintVarname_PrintStmtnum_SameCol_DifferentNumRows_DifferentValues_Success)
		{
			SYNONYM_NAME target_synonym = "pn";

			ResultList r1;
			SYNONYM_VALUES_LIST values1 = { "7" };
			r1.addColumn(target_synonym, values1);

			ResultList r2;
			SYNONYM_VALUES_LIST values2 = { "paw", "pawpaw" };
			r2.addColumn(target_synonym, values2);

			ResultList result = ResultListManager::merge(r1, r2, *processed_synonyms, *pkb);

			int num_rows = result.getNumRows();
			int num_cols = result.getNumColumns();
			std::string result_string = ResultListManager::getStringValues(result.getValuesOfSynonym(target_synonym));

			// Log results
			std::string mes1 = "numRows: " + std::to_string(num_rows) + "\n";
			std::string mes2 = "numCols: " + std::to_string(num_cols) + "\n";
			std::string print_result_string = "Result: " + result_string;
			Logger::WriteMessage(mes1.c_str());
			Logger::WriteMessage(mes2.c_str());
			Logger::WriteMessage(print_result_string.c_str());

			int expected_num_rows = 1;
			int expected_num_cols = 1;
			std::string expected_result = "7";

			Assert::IsTrue(num_rows == expected_num_rows);
			Assert::IsTrue(num_cols == expected_num_cols);
			Assert::IsTrue(result_string.compare(expected_result) == 0);
		}

		TEST_METHOD(merge_ReadVarname_PrintVarname_SameCol_DifferentNumRows_DifferentValues_Success)
		{
			SYNONYM_NAME target_synonym = "r";
			SYNONYM_NAME target_synonym_2 = "pn";

			ResultList r1;
			SYNONYM_VALUES_LIST values1 = { "meow", "meowmeow" };	// StmtNum: 2, 6
			r1.addColumn(target_synonym, values1);

			ResultList r2;
			SYNONYM_VALUES_LIST values2 = { "paw", "pawpaw" };		// StmtNum: 3, 7
			r2.addColumn(target_synonym_2, values2);

			int expected_num_rows = 4;
			int expected_num_cols = 2;
			std::string target_synonym_result = "2, 2, 6, 6";
			std::string target_synonym_2_result = "3, 7, 3, 7";


			ResultList result = ResultListManager::merge(r1, r2, *processed_synonyms, *pkb);

			int num_rows = result.getNumRows();
			int num_cols = result.getNumColumns();
			std::string result_string1 = ResultListManager::getStringValues(result.getValuesOfSynonym(target_synonym)); // get target_synonym
			std::string result_string2 = ResultListManager::getStringValues(result.getValuesOfSynonym(target_synonym_2)); // get target_synonym_2


			// Log results
			std::string mes1 = "numRows: " + std::to_string(num_rows) + "\n";
			std::string mes2 = "numCols: " + std::to_string(num_cols) + "\n";
			std::string print_result_string = "Target Synonym 1: " + result_string1;
			std::string print_result_string2 = "Target Synonym 2: " + result_string2;

			Logger::WriteMessage(mes1.c_str());
			Logger::WriteMessage(mes2.c_str());
			Logger::WriteMessage(print_result_string.c_str());
			Logger::WriteMessage(print_result_string2.c_str());


			Assert::IsTrue(num_rows == expected_num_rows);
			Assert::IsTrue(num_cols == expected_num_cols);
			Assert::IsTrue(result_string1.compare(target_synonym_result) == 0);
			Assert::IsTrue(result_string2.compare(target_synonym_2_result) == 0);

		}

		TEST_METHOD(merge_CallVarname_PrintVarname_DifferentCol_DifferentNumRows_DifferentValues_Success)
		{
			SYNONYM_NAME target_synonym = "c";
			SYNONYM_NAME target_synonym_2 = "pn";

			ResultList r1;
			SYNONYM_VALUES_LIST values1 = { "master", "daddy", "mummy" };	// StmtNum: 1, 4, 5
			r1.addColumn(target_synonym, values1);

			ResultList r2;
			SYNONYM_VALUES_LIST values2 = { "paw", "pawpaw" };		// StmtNum: 3, 7
			r2.addColumn(target_synonym_2, values2);

			int expected_num_rows = 6;
			int expected_num_cols = 2;
			std::string target_synonym_result = "1, 1, 4, 4, 5, 5";
			std::string target_synonym_2_result = "3, 7, 3, 7, 3, 7";


			ResultList result = ResultListManager::merge(r1, r2, *processed_synonyms, *pkb);

			int num_rows = result.getNumRows();
			int num_cols = result.getNumColumns();
			std::string result_string1 = ResultListManager::getStringValues(result.getValuesOfSynonym(target_synonym)); // get target_synonym
			std::string result_string2 = ResultListManager::getStringValues(result.getValuesOfSynonym(target_synonym_2)); // get target_synonym_2


			// Log results
			std::string mes1 = "numRows: " + std::to_string(num_rows) + "\n";
			std::string mes2 = "numCols: " + std::to_string(num_cols) + "\n";
			std::string print_result_string = "Target Synonym 1: " + result_string1 + "\n";
			std::string print_result_string2 = "Target Synonym 2: " + result_string2 + "\n";

			Logger::WriteMessage(mes1.c_str());
			Logger::WriteMessage(mes2.c_str());
			Logger::WriteMessage(print_result_string.c_str());
			Logger::WriteMessage(print_result_string2.c_str());


			// Assert Results
			Assert::IsTrue(num_rows == expected_num_rows);
			Assert::IsTrue(num_cols == expected_num_cols);
			Assert::IsTrue(result_string1.compare(target_synonym_result) == 0);
			Assert::IsTrue(result_string2.compare(target_synonym_2_result) == 0);

		}

		TEST_METHOD(merge_CallVarnamePrintVarName_PrintVarname_DifferentCol_DifferentNumRows_DifferentValues_Success)
		{
			SYNONYM_NAME target_synonym = "c";
			SYNONYM_NAME target_synonym_2 = "pn";

			ResultList r1;
			SYNONYM_VALUES_LIST values1 = { "master", "daddy", "mummy" };		// StmtNum: 1, 4, 5
			SYNONYM_VALUES_LIST values1_2 = { "pawpaw", "pawpaw", "paw" };		// StmtNum: 7, 7, 3
			r1.addColumn(target_synonym, values1);
			r1.addColumn(target_synonym_2, values1_2);

			ResultList r2;
			SYNONYM_VALUES_LIST values2 = { "paw", "pawpaw" };					// StmtNum: 3, 7
			r2.addColumn(target_synonym_2, values2);

			int expected_num_rows = 3;
			int expected_num_cols = 2;
			std::string target_synonym_result = "1, 4, 5";
			std::string target_synonym_2_result = "7, 7, 3";


			ResultList result = ResultListManager::merge(r1, r2, *processed_synonyms, *pkb);

			int num_rows = result.getNumRows();
			int num_cols = result.getNumColumns();
			std::string result_string1 = ResultListManager::getStringValues(result.getValuesOfSynonym(target_synonym)); // get target_synonym
			std::string result_string2 = ResultListManager::getStringValues(result.getValuesOfSynonym(target_synonym_2)); // get target_synonym_2


			// Log results
			std::string mes1 = "numRows: " + std::to_string(num_rows) + "\n";
			std::string mes2 = "numCols: " + std::to_string(num_cols) + "\n";
			std::string print_result_string = "Target Synonym 1: " + result_string1 + "\n";
			std::string print_result_string2 = "Target Synonym 2: " + result_string2 + "\n";

			Logger::WriteMessage(mes1.c_str());
			Logger::WriteMessage(mes2.c_str());
			Logger::WriteMessage(print_result_string.c_str());
			Logger::WriteMessage(print_result_string2.c_str());


			// Assert Results
			Assert::IsTrue(num_rows == expected_num_rows);
			Assert::IsTrue(num_cols == expected_num_cols);
			Assert::IsTrue(result_string1.compare(target_synonym_result) == 0);
			Assert::IsTrue(result_string2.compare(target_synonym_2_result) == 0);

		}

		TEST_METHOD(merge_CallVarnamePrintStmtnum_PrintVarname_DifferentCol_DifferentNumRows_DifferentValues_Success)
		{
			SYNONYM_NAME target_synonym = "c";
			SYNONYM_NAME target_synonym_2 = "pn";

			ResultList r1;
			SYNONYM_VALUES_LIST values1 = { "master", "daddy", "mummy" };		// StmtNum: 1, 4, 5
			SYNONYM_VALUES_LIST values1_2 = { "7", "7", "3" };					// StmtNum: 7, 7, 3
			r1.addColumn(target_synonym, values1);
			r1.addColumn(target_synonym_2, values1_2);

			ResultList r2;
			SYNONYM_VALUES_LIST values2 = { "paw", "pawpaw" };					// StmtNum: 3, 7
			r2.addColumn(target_synonym_2, values2);

			int expected_num_rows = 3;
			int expected_num_cols = 2;
			std::string target_synonym_result = "1, 4, 5";
			std::string target_synonym_2_result = "7, 7, 3";


			ResultList result = ResultListManager::merge(r1, r2, *processed_synonyms, *pkb);

			int num_rows = result.getNumRows();
			int num_cols = result.getNumColumns();
			std::string result_string1 = ResultListManager::getStringValues(result.getValuesOfSynonym(target_synonym)); // get target_synonym
			std::string result_string2 = ResultListManager::getStringValues(result.getValuesOfSynonym(target_synonym_2)); // get target_synonym_2


			// Log results
			std::string mes1 = "numRows: " + std::to_string(num_rows) + "\n";
			std::string mes2 = "numCols: " + std::to_string(num_cols) + "\n";
			std::string print_result_string = "Target Synonym 1: " + result_string1 + "\n";
			std::string print_result_string2 = "Target Synonym 2: " + result_string2 + "\n";

			Logger::WriteMessage(mes1.c_str());
			Logger::WriteMessage(mes2.c_str());
			Logger::WriteMessage(print_result_string.c_str());
			Logger::WriteMessage(print_result_string2.c_str());


			// Assert Results
			Assert::IsTrue(num_rows == expected_num_rows);
			Assert::IsTrue(num_cols == expected_num_cols);
			Assert::IsTrue(result_string1.compare(target_synonym_result) == 0);
			Assert::IsTrue(result_string2.compare(target_synonym_2_result) == 0);

		}

		TEST_METHOD(merge_CallVarnamePrintStmtnum_PrintVarnameCallStmtnum_DifferentCol_DifferentNumRows_DifferentValues_Success)
		{
			SYNONYM_NAME target_synonym = "c";
			SYNONYM_NAME target_synonym_2 = "pn";

			ResultList r1;
			SYNONYM_VALUES_LIST values1 = { "master", "daddy", "mummy" };		// StmtNum: 1, 4, 5
			SYNONYM_VALUES_LIST values1_2 = { "7", "7", "3" };					// StmtNum: 7, 7, 3
			r1.addColumn(target_synonym, values1);
			r1.addColumn(target_synonym_2, values1_2);

			ResultList r2;
			SYNONYM_VALUES_LIST values2 = { "paw", "pawpaw" };					// StmtNum: 3, 7
			SYNONYM_VALUES_LIST values2_2 = { "1", "1"};						// StmtNum: 1, 1
			r2.addColumn(target_synonym_2, values2);
			r2.addColumn(target_synonym, values2_2);


			int expected_num_rows = 1;
			int expected_num_cols = 2;
			std::string target_synonym_result = "1";
			std::string target_synonym_2_result = "7";


			ResultList result = ResultListManager::merge(r1, r2, *processed_synonyms, *pkb);

			int num_rows = result.getNumRows();
			int num_cols = result.getNumColumns();
			std::string result_string1 = ResultListManager::getStringValues(result.getValuesOfSynonym(target_synonym)); // get target_synonym
			std::string result_string2 = ResultListManager::getStringValues(result.getValuesOfSynonym(target_synonym_2)); // get target_synonym_2


			// Log results
			std::string mes1 = "numRows: " + std::to_string(num_rows) + "\n";
			std::string mes2 = "numCols: " + std::to_string(num_cols) + "\n";
			std::string print_result_string = "Target Synonym 1: " + result_string1 + "\n";
			std::string print_result_string2 = "Target Synonym 2: " + result_string2 + "\n";

			Logger::WriteMessage(mes1.c_str());
			Logger::WriteMessage(mes2.c_str());
			Logger::WriteMessage(print_result_string.c_str());
			Logger::WriteMessage(print_result_string2.c_str());


			// Assert Results
			Assert::IsTrue(num_rows == expected_num_rows);
			Assert::IsTrue(num_cols == expected_num_cols);
			Assert::IsTrue(result_string1.compare(target_synonym_result) == 0);
			Assert::IsTrue(result_string2.compare(target_synonym_2_result) == 0);

		}

		TEST_METHOD(merge_CallVarnamePrintStmtnum_PrintVarnameCallStmtnum_NoSameRowValues_Success)
		{
			SYNONYM_NAME target_synonym = "c";
			SYNONYM_NAME target_synonym_2 = "pn";

			ResultList r1;
			SYNONYM_VALUES_LIST values1 = { "daddy", "mummy" };					// StmtNum: 4, 5
			SYNONYM_VALUES_LIST values1_2 = { "7", "3" };						// StmtNum: 7, 3
			r1.addColumn(target_synonym, values1);
			r1.addColumn(target_synonym_2, values1_2);

			ResultList r2;
			SYNONYM_VALUES_LIST values2 = { "paw", "pawpaw" };					// StmtNum: 3, 7
			SYNONYM_VALUES_LIST values2_2 = { "1", "1" };						// StmtNum: 1, 1
			r2.addColumn(target_synonym_2, values2);
			r2.addColumn(target_synonym, values2_2);


			int expected_num_rows = 0;
			int expected_num_cols = 2;
			std::string target_synonym_result = "";
			std::string target_synonym_2_result = "";


			ResultList result = ResultListManager::merge(r1, r2, *processed_synonyms, *pkb);

			int num_rows = result.getNumRows();
			int num_cols = result.getNumColumns();
			std::string result_string1 = ResultListManager::getStringValues(result.getValuesOfSynonym(target_synonym)); // get target_synonym
			std::string result_string2 = ResultListManager::getStringValues(result.getValuesOfSynonym(target_synonym_2)); // get target_synonym_2


			// Log results
			std::string mes1 = "numRows: " + std::to_string(num_rows) + "\n";
			std::string mes2 = "numCols: " + std::to_string(num_cols) + "\n";
			std::string print_result_string = "Target Synonym 1: " + result_string1 + "\n";
			std::string print_result_string2 = "Target Synonym 2: " + result_string2 + "\n";

			Logger::WriteMessage(mes1.c_str());
			Logger::WriteMessage(mes2.c_str());
			Logger::WriteMessage(print_result_string.c_str());
			Logger::WriteMessage(print_result_string2.c_str());


			// Assert Results
			Assert::IsTrue(num_rows == expected_num_rows);
			Assert::IsTrue(num_cols == expected_num_cols);
			Assert::IsTrue(result_string1.compare(target_synonym_result) == 0);
			Assert::IsTrue(result_string2.compare(target_synonym_2_result) == 0);

		}

		TEST_METHOD(merge_CallVarnamePrintStmtnum_ReadEmpty_NoSameRowValues_Success)
		{
			SYNONYM_NAME target_synonym = "c";
			SYNONYM_NAME target_synonym_2 = "pn";
			SYNONYM_NAME target_synonym_3 = "r";

			ResultList r1;
			SYNONYM_VALUES_LIST values1 = { "daddy", "mummy" };					// StmtNum: 4, 5
			SYNONYM_VALUES_LIST values1_2 = { "7", "3" };						// StmtNum: 7, 3
			r1.addColumn(target_synonym, values1);
			r1.addColumn(target_synonym_2, values1_2);

			ResultList r2;
			SYNONYM_VALUES_LIST values2 = {};
			r2.addColumn(target_synonym_3, values2);


			int expected_num_rows = 0;
			int expected_num_cols = 3;
			std::string target_synonym_result = "";
			std::string target_synonym_2_result = "";


			ResultList result = ResultListManager::merge(r1, r2, *processed_synonyms, *pkb);

			int num_rows = result.getNumRows();
			int num_cols = result.getNumColumns();
			std::string result_string1 = ResultListManager::getStringValues(result.getValuesOfSynonym(target_synonym)); // get target_synonym
			std::string result_string2 = ResultListManager::getStringValues(result.getValuesOfSynonym(target_synonym_2)); // get target_synonym_2


			// Log results
			std::string mes1 = "numRows: " + std::to_string(num_rows) + "\n";
			std::string mes2 = "numCols: " + std::to_string(num_cols) + "\n";
			std::string print_result_string = "Target Synonym 1: " + result_string1 + "\n";
			std::string print_result_string2 = "Target Synonym 2: " + result_string2 + "\n";

			Logger::WriteMessage(mes1.c_str());
			Logger::WriteMessage(mes2.c_str());
			Logger::WriteMessage(print_result_string.c_str());
			Logger::WriteMessage(print_result_string2.c_str());


			// Assert Results
			Assert::IsTrue(num_rows == expected_num_rows);
			Assert::IsTrue(num_cols == expected_num_cols);
			Assert::IsTrue(result_string1.compare(target_synonym_result) == 0);
			Assert::IsTrue(result_string2.compare(target_synonym_2_result) == 0);

		}
	};
}