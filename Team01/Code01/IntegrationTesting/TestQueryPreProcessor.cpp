#include <typeinfo>
#include <string>

#include "stdafx.h"
#include "CppUnitTest.h"
#include "../source/QueryPreProcessor.h"
#include "../source/QueryNode.h"
#include "../source/PKB/ASTNode/ExpressionNode.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace IntegrationTesting
{
	TEST_CLASS(TestQueryPreProcessor) {
	public:

		/* Split query  */
		TEST_METHOD(splitQuery_Valid_Success) {
			QueryPreProcessor qpp = QueryPreProcessor();

			SPLIT_QUERY split_base = qpp.splitQuery("stmt s; Select s");
			SPLIT_QUERY split_base_var1 = qpp.splitQuery(" stmt s; Select s");
			SPLIT_QUERY split_base_var2 = qpp.splitQuery("stmt s; Select s ");
			SPLIT_QUERY split_base_var3 = qpp.splitQuery(" stmt s; Select s ");
			SPLIT_QUERY split_base_var4 = qpp.splitQuery("  stmt s; Select s  ");

			SPLIT_QUERY correct_split_base;
			correct_split_base.push_back("stmt s");
			correct_split_base.push_back("Select s");

			Assert::IsTrue(split_base[0].compare(correct_split_base[0]) == 0);
			Assert::IsTrue(split_base[1].compare(correct_split_base[1]) == 0);

			Assert::IsTrue(split_base_var1[0].compare(correct_split_base[0]) == 0);
			Assert::IsTrue(split_base_var1[1].compare(correct_split_base[1]) == 0);

			Assert::IsTrue(split_base_var2[0].compare(correct_split_base[0]) == 0);
			Assert::IsTrue(split_base_var2[1].compare(correct_split_base[1]) == 0);

			Assert::IsTrue(split_base_var3[0].compare(correct_split_base[0]) == 0);
			Assert::IsTrue(split_base_var3[1].compare(correct_split_base[1]) == 0);

			Assert::IsTrue(split_base_var4[0].compare(correct_split_base[0]) == 0);
			Assert::IsTrue(split_base_var4[1].compare(correct_split_base[1]) == 0);
		}

		TEST_METHOD(splitQuery_Invalid_Success) {
			QueryPreProcessor qpp = QueryPreProcessor();

			SPLIT_QUERY split_base = qpp.splitQuery("");
			SPLIT_QUERY split_base_var1 = qpp.splitQuery(" ");
			SPLIT_QUERY split_base_var2 = qpp.splitQuery("stmt s;");
			SPLIT_QUERY split_base_var3 = qpp.splitQuery("Select s");
			SPLIT_QUERY split_base_var4 = qpp.splitQuery("stmt s; select s");
			SPLIT_QUERY split_base_var5 = qpp.splitQuery("test");

			Assert::IsTrue(split_base[0].compare("") == 0 && split_base[1].compare("") == 0);
			Assert::IsTrue(split_base_var1[0].compare("") == 0 && split_base_var1[1].compare("") == 0);

			Assert::IsTrue(split_base_var2[0].compare("") == 0 && split_base_var2[1].compare("") == 0);
			Assert::IsTrue(split_base_var3[0].compare("") == 0 && split_base_var3[1].compare("") == 0);

			Assert::IsTrue(split_base_var4[0].compare("") == 0 && split_base_var4[1].compare("") == 0);
			Assert::IsTrue(split_base_var5[0].compare("") == 0 && split_base_var5[1].compare("") == 0);
		}

		/* Parse declaration */
		TEST_METHOD(preProcessSynonyms_Single_Valid_Success) {
			QueryPreProcessor qpp = QueryPreProcessor();

			DECLARATIONS stmt = "stmt s";
			DECLARATIONS read = "read re";
			DECLARATIONS print = "print pr";
			DECLARATIONS whiles = "while w";
			DECLARATIONS ifs = "if ifs";
			DECLARATIONS assign = "assign a";
			DECLARATIONS variable = "variable v";
			DECLARATIONS constant = "constant c";
			DECLARATIONS procedure = "procedure p";

			PROCESSED_SYNONYMS proc_stmt = qpp.preProcessSynonyms(stmt);
			PROCESSED_SYNONYMS proc_read = qpp.preProcessSynonyms(read);
			PROCESSED_SYNONYMS proc_print = qpp.preProcessSynonyms(print);
			PROCESSED_SYNONYMS proc_while = qpp.preProcessSynonyms(whiles);
			PROCESSED_SYNONYMS proc_ifs = qpp.preProcessSynonyms(ifs);
			PROCESSED_SYNONYMS proc_assign = qpp.preProcessSynonyms(assign);
			PROCESSED_SYNONYMS proc_variable = qpp.preProcessSynonyms(variable);
			PROCESSED_SYNONYMS proc_constant = qpp.preProcessSynonyms(constant);
			PROCESSED_SYNONYMS proc_procedure = qpp.preProcessSynonyms(procedure);
			
			Assert::IsTrue(proc_stmt.at("s").getNodeType() == QueryNodeType::synonym);
			Assert::IsTrue(proc_stmt.at("s").getSynonymType() == QuerySynonymType::stmt);
			Assert::IsTrue(proc_stmt.at("s").getString().compare("s") == 0);

			Assert::IsTrue(proc_read.at("re").getNodeType() == QueryNodeType::synonym);
			Assert::IsTrue(proc_read.at("re").getSynonymType() == QuerySynonymType::read);
			Assert::IsTrue(proc_read.at("re").getString().compare("re") == 0);

			Assert::IsTrue(proc_print.at("pr").getNodeType() == QueryNodeType::synonym);
			Assert::IsTrue(proc_print.at("pr").getSynonymType() == QuerySynonymType::print);
			Assert::IsTrue(proc_print.at("pr").getString().compare("pr") == 0);

			Assert::IsTrue(proc_while.at("w").getNodeType() == QueryNodeType::synonym);
			Assert::IsTrue(proc_while.at("w").getSynonymType() == QuerySynonymType::whiles);
			Assert::IsTrue(proc_while.at("w").getString().compare("w") == 0);

			Assert::IsTrue(proc_ifs.at("ifs").getNodeType() == QueryNodeType::synonym);
			Assert::IsTrue(proc_ifs.at("ifs").getSynonymType() == QuerySynonymType::ifs);
			Assert::IsTrue(proc_ifs.at("ifs").getString().compare("ifs") == 0);

			Assert::IsTrue(proc_assign.at("a").getNodeType() == QueryNodeType::synonym);
			Assert::IsTrue(proc_assign.at("a").getSynonymType() == QuerySynonymType::assign);
			Assert::IsTrue(proc_assign.at("a").getString().compare("a") == 0);

			Assert::IsTrue(proc_variable.at("v").getNodeType() == QueryNodeType::synonym);
			Assert::IsTrue(proc_variable.at("v").getSynonymType() == QuerySynonymType::variable);
			Assert::IsTrue(proc_variable.at("v").getString().compare("v") == 0);

			Assert::IsTrue(proc_constant.at("c").getNodeType() == QueryNodeType::synonym);
			Assert::IsTrue(proc_constant.at("c").getSynonymType() == QuerySynonymType::constant);
			Assert::IsTrue(proc_constant.at("c").getString().compare("c") == 0);

			Assert::IsTrue(proc_procedure.at("p").getNodeType() == QueryNodeType::synonym);
			Assert::IsTrue(proc_procedure.at("p").getSynonymType() == QuerySynonymType::procedure);
			Assert::IsTrue(proc_procedure.at("p").getString().compare("p") == 0);
		}
		
		TEST_METHOD(preProcessSynonyms_Multiple_Single_Valid_Success) {
			QueryPreProcessor qpp = QueryPreProcessor();

			DECLARATIONS stmt = "stmt s1, s2, s3";
			DECLARATIONS read = "read re1, re2, re3";
			DECLARATIONS assign = "assign a1, a2, a3";

			PROCESSED_SYNONYMS proc_stmt = qpp.preProcessSynonyms(stmt);
			PROCESSED_SYNONYMS proc_read = qpp.preProcessSynonyms(read);
			PROCESSED_SYNONYMS proc_assign = qpp.preProcessSynonyms(assign);


			Assert::IsTrue(proc_stmt.at("s1").getNodeType() == QueryNodeType::synonym);
			Assert::IsTrue(proc_stmt.at("s1").getSynonymType() == QuerySynonymType::stmt);
			Assert::IsTrue(proc_stmt.at("s1").getString().compare("s1") == 0);
			Assert::IsTrue(proc_stmt.at("s2").getNodeType() == QueryNodeType::synonym);
			Assert::IsTrue(proc_stmt.at("s2").getSynonymType() == QuerySynonymType::stmt);
			Assert::IsTrue(proc_stmt.at("s2").getString().compare("s2") == 0);
			Assert::IsTrue(proc_stmt.at("s3").getNodeType() == QueryNodeType::synonym);
			Assert::IsTrue(proc_stmt.at("s3").getSynonymType() == QuerySynonymType::stmt);
			Assert::IsTrue(proc_stmt.at("s3").getString().compare("s3") == 0);

			Assert::IsTrue(proc_read.at("re1").getNodeType() == QueryNodeType::synonym);
			Assert::IsTrue(proc_read.at("re1").getSynonymType() == QuerySynonymType::read);
			Assert::IsTrue(proc_read.at("re1").getString().compare("re1") == 0);
			Assert::IsTrue(proc_read.at("re2").getNodeType() == QueryNodeType::synonym);
			Assert::IsTrue(proc_read.at("re2").getSynonymType() == QuerySynonymType::read);
			Assert::IsTrue(proc_read.at("re2").getString().compare("re2") == 0);
			Assert::IsTrue(proc_read.at("re3").getNodeType() == QueryNodeType::synonym);
			Assert::IsTrue(proc_read.at("re3").getSynonymType() == QuerySynonymType::read);
			Assert::IsTrue(proc_read.at("re3").getString().compare("re3") == 0);

			Assert::IsTrue(proc_assign.at("a1").getNodeType() == QueryNodeType::synonym);
			Assert::IsTrue(proc_assign.at("a1").getSynonymType() == QuerySynonymType::assign);
			Assert::IsTrue(proc_assign.at("a1").getString().compare("a1") == 0);
			Assert::IsTrue(proc_assign.at("a2").getNodeType() == QueryNodeType::synonym);
			Assert::IsTrue(proc_assign.at("a2").getSynonymType() == QuerySynonymType::assign);
			Assert::IsTrue(proc_assign.at("a2").getString().compare("a2") == 0);
			Assert::IsTrue(proc_assign.at("a3").getNodeType() == QueryNodeType::synonym);
			Assert::IsTrue(proc_assign.at("a3").getSynonymType() == QuerySynonymType::assign);
			Assert::IsTrue(proc_assign.at("a3").getString().compare("a3") == 0);
		}

		TEST_METHOD(preProcessSynonyms_Multiple_Different_Valid_Success) {
			QueryPreProcessor qpp = QueryPreProcessor();

			DECLARATIONS query = "stmt s; read re; while w; assign a; variable v;";

			PROCESSED_SYNONYMS proc_query = qpp.preProcessSynonyms(query);

			Assert::IsTrue(proc_query.at("s").getNodeType() == QueryNodeType::synonym);
			Assert::IsTrue(proc_query.at("s").getSynonymType() == QuerySynonymType::stmt);
			Assert::IsTrue(proc_query.at("s").getString().compare("s") == 0);

			Assert::IsTrue(proc_query.at("re").getNodeType() == QueryNodeType::synonym);
			Assert::IsTrue(proc_query.at("re").getSynonymType() == QuerySynonymType::read);
			Assert::IsTrue(proc_query.at("re").getString().compare("re") == 0);

			Assert::IsTrue(proc_query.at("w").getNodeType() == QueryNodeType::synonym);
			Assert::IsTrue(proc_query.at("w").getSynonymType() == QuerySynonymType::whiles);
			Assert::IsTrue(proc_query.at("w").getString().compare("w") == 0);

			Assert::IsTrue(proc_query.at("a").getNodeType() == QueryNodeType::synonym);
			Assert::IsTrue(proc_query.at("a").getSynonymType() == QuerySynonymType::assign);
			Assert::IsTrue(proc_query.at("a").getString().compare("a") == 0);

			Assert::IsTrue(proc_query.at("v").getNodeType() == QueryNodeType::synonym);
			Assert::IsTrue(proc_query.at("v").getSynonymType() == QuerySynonymType::variable);
			Assert::IsTrue(proc_query.at("v").getString().compare("v") == 0);
		}

		TEST_METHOD(preProcessSynonyms_Invalid_Success) {
			QueryPreProcessor qpp = QueryPreProcessor();

			DECLARATIONS q1 = "";
			DECLARATIONS q2 = "test";
			DECLARATIONS q3 = "   ";
			DECLARATIONS q4 = ";";
			DECLARATIONS q5 = "1";
			DECLARATIONS q6 = "stmt";
			DECLARATIONS q7 = "stmt s v";
			DECLARATIONS q8 = "stmt 1";
			DECLARATIONS q9 = "stmt 1,";

			PROCESSED_SYNONYMS proc_q1 = qpp.preProcessSynonyms(q1);
			PROCESSED_SYNONYMS proc_q2 = qpp.preProcessSynonyms(q2);
			PROCESSED_SYNONYMS proc_q3 = qpp.preProcessSynonyms(q3);
			PROCESSED_SYNONYMS proc_q4 = qpp.preProcessSynonyms(q4);
			PROCESSED_SYNONYMS proc_q5 = qpp.preProcessSynonyms(q5);
			PROCESSED_SYNONYMS proc_q6 = qpp.preProcessSynonyms(q6);
			PROCESSED_SYNONYMS proc_q7 = qpp.preProcessSynonyms(q7);
			PROCESSED_SYNONYMS proc_q8 = qpp.preProcessSynonyms(q8);
			PROCESSED_SYNONYMS proc_q9 = qpp.preProcessSynonyms(q9);

			Assert::IsTrue(proc_q1.empty());
			Assert::IsTrue(proc_q2.empty());
			Assert::IsTrue(proc_q3.empty());
			Assert::IsTrue(proc_q4.empty());
			Assert::IsTrue(proc_q5.empty());
			Assert::IsTrue(proc_q6.empty());
			Assert::IsTrue(proc_q7.empty());
			Assert::IsTrue(proc_q8.empty());
			Assert::IsTrue(proc_q9.empty());
		}

		/* Parse clauses */
		TEST_METHOD(preProcessClauses_Select_Valid_Success) {
			QueryPreProcessor qpp = QueryPreProcessor();

			DECLARATIONS d = "stmt s; read re; print pr; while w; if ifs; assign a; variable v; constant c; procedure p";
			PROCESSED_SYNONYMS proc_s = qpp.preProcessSynonyms(d);

			CLAUSES c1 = "Select s";
			CLAUSES c2 = "Select ifs";
			CLAUSES c3 = "Select v";
			CLAUSES c4 = "Select BOOLEAN";
			CLAUSES c5 = "Select p.procName";
			CLAUSES c6 = "Select <s>";
			CLAUSES c7 = "Select <v.varName>";
			CLAUSES c8 = "Select <s, v>";
			CLAUSES c9 = "Select <s.stmt#, c.value>";
			CLAUSES c10 = "Select <s, re, pr>";

			PROCESSED_CLAUSES proc_c1 = qpp.preProcessClauses(proc_s, c1);
			PROCESSED_CLAUSES proc_c2 = qpp.preProcessClauses(proc_s, c2);
			PROCESSED_CLAUSES proc_c3 = qpp.preProcessClauses(proc_s, c3);
			PROCESSED_CLAUSES proc_c4 = qpp.preProcessClauses(proc_s, c4);
			PROCESSED_CLAUSES proc_c5 = qpp.preProcessClauses(proc_s, c5);
			PROCESSED_CLAUSES proc_c6 = qpp.preProcessClauses(proc_s, c6);
			PROCESSED_CLAUSES proc_c7 = qpp.preProcessClauses(proc_s, c7);
			PROCESSED_CLAUSES proc_c8 = qpp.preProcessClauses(proc_s, c8);
			PROCESSED_CLAUSES proc_c9 = qpp.preProcessClauses(proc_s, c9);
			PROCESSED_CLAUSES proc_c10 = qpp.preProcessClauses(proc_s, c10);
			
			Assert::IsTrue(proc_c1.getNodeType() == QueryNodeType::select);
			Assert::IsTrue(proc_c1.getChildren()[0].getNodeType() == QueryNodeType::tuple);
			Assert::IsTrue(proc_c1.getChildren()[0].getChildren()[0].getNodeType() == QueryNodeType::synonym);
			Assert::IsTrue(proc_c1.getChildren()[0].getChildren()[0].getSynonymType() == QuerySynonymType::stmt);
			Assert::IsTrue(proc_c1.getChildren()[0].getChildren()[0].getString().compare("s") == 0);

			Assert::IsTrue(proc_c2.getNodeType() == QueryNodeType::select);
			Assert::IsTrue(proc_c2.getChildren()[0].getNodeType() == QueryNodeType::tuple);
			Assert::IsTrue(proc_c2.getChildren()[0].getChildren()[0].getNodeType() == QueryNodeType::synonym);
			Assert::IsTrue(proc_c2.getChildren()[0].getChildren()[0].getSynonymType() == QuerySynonymType::ifs);
			Assert::IsTrue(proc_c2.getChildren()[0].getChildren()[0].getString().compare("ifs") == 0);

			Assert::IsTrue(proc_c3.getNodeType() == QueryNodeType::select);
			Assert::IsTrue(proc_c3.getChildren()[0].getNodeType() == QueryNodeType::tuple);
			Assert::IsTrue(proc_c3.getChildren()[0].getChildren()[0].getNodeType() == QueryNodeType::synonym);
			Assert::IsTrue(proc_c3.getChildren()[0].getChildren()[0].getSynonymType() == QuerySynonymType::variable);
			Assert::IsTrue(proc_c3.getChildren()[0].getChildren()[0].getString().compare("v") == 0);

			Assert::IsTrue(proc_c4.getNodeType() == QueryNodeType::select);
			Assert::IsTrue(proc_c4.getChildren()[0].getNodeType() == QueryNodeType::boolean);

			Assert::IsTrue(proc_c5.getNodeType() == QueryNodeType::select);
			Assert::IsTrue(proc_c5.getChildren()[0].getNodeType() == QueryNodeType::tuple);
			Assert::IsTrue(proc_c5.getChildren()[0].getChildren()[0].getNodeType() == QueryNodeType::attr);
			Assert::IsTrue(proc_c5.getChildren()[0].getChildren()[0].getString().compare("p") == 0);
			Assert::IsTrue(proc_c5.getChildren()[0].getChildren()[0].getAttr() == AttributeType::procName);
			
			Assert::IsTrue(proc_c6.getNodeType() == QueryNodeType::select);
			Assert::IsTrue(proc_c6.getChildren()[0].getNodeType() == QueryNodeType::tuple);
			Assert::IsTrue(proc_c6.getChildren()[0].getChildren()[0].getNodeType() == QueryNodeType::synonym);
			Assert::IsTrue(proc_c6.getChildren()[0].getChildren()[0].getSynonymType() == QuerySynonymType::stmt);
			Assert::IsTrue(proc_c6.getChildren()[0].getChildren()[0].getString().compare("s") == 0);

			Assert::IsTrue(proc_c7.getNodeType() == QueryNodeType::select);
			Assert::IsTrue(proc_c7.getChildren()[0].getNodeType() == QueryNodeType::tuple);
			Assert::IsTrue(proc_c7.getChildren()[0].getChildren()[0].getNodeType() == QueryNodeType::attr);
			Assert::IsTrue(proc_c7.getChildren()[0].getChildren()[0].getString().compare("v") == 0);
			Assert::IsTrue(proc_c7.getChildren()[0].getChildren()[0].getAttr() == AttributeType::varName);
			
			Assert::IsTrue(proc_c8.getNodeType() == QueryNodeType::select);
			Assert::IsTrue(proc_c8.getChildren()[0].getNodeType() == QueryNodeType::tuple);
			Assert::IsTrue(proc_c8.getChildren()[0].getChildren()[0].getNodeType() == QueryNodeType::synonym);
			Assert::IsTrue(proc_c8.getChildren()[0].getChildren()[0].getSynonymType() == QuerySynonymType::stmt);
			Assert::IsTrue(proc_c8.getChildren()[0].getChildren()[0].getString().compare("s") == 0);
			Assert::IsTrue(proc_c8.getChildren()[0].getChildren()[1].getNodeType() == QueryNodeType::synonym);
			Assert::IsTrue(proc_c8.getChildren()[0].getChildren()[1].getSynonymType() == QuerySynonymType::variable);
			Assert::IsTrue(proc_c8.getChildren()[0].getChildren()[1].getString().compare("v") == 0);

			Assert::IsTrue(proc_c9.getNodeType() == QueryNodeType::select);
			Assert::IsTrue(proc_c9.getChildren()[0].getNodeType() == QueryNodeType::tuple);
			Assert::IsTrue(proc_c9.getChildren()[0].getChildren()[0].getNodeType() == QueryNodeType::attr);
			Assert::IsTrue(proc_c9.getChildren()[0].getChildren()[0].getString().compare("s") == 0);
			Assert::IsTrue(proc_c9.getChildren()[0].getChildren()[0].getAttr() == AttributeType::stmtNum);
			Assert::IsTrue(proc_c9.getChildren()[0].getChildren()[1].getNodeType() == QueryNodeType::attr);
			Assert::IsTrue(proc_c9.getChildren()[0].getChildren()[1].getString().compare("c") == 0);
			Assert::IsTrue(proc_c9.getChildren()[0].getChildren()[1].getAttr() == AttributeType::value);

			Assert::IsTrue(proc_c10.getNodeType() == QueryNodeType::select);
			Assert::IsTrue(proc_c10.getChildren()[0].getNodeType() == QueryNodeType::tuple);
			Assert::IsTrue(proc_c10.getChildren()[0].getChildren()[0].getNodeType() == QueryNodeType::synonym);
			Assert::IsTrue(proc_c10.getChildren()[0].getChildren()[0].getSynonymType() == QuerySynonymType::stmt);
			Assert::IsTrue(proc_c10.getChildren()[0].getChildren()[0].getString().compare("s") == 0);
			Assert::IsTrue(proc_c10.getChildren()[0].getChildren()[1].getNodeType() == QueryNodeType::synonym);
			Assert::IsTrue(proc_c10.getChildren()[0].getChildren()[1].getSynonymType() == QuerySynonymType::read);
			Assert::IsTrue(proc_c10.getChildren()[0].getChildren()[1].getString().compare("re") == 0);
			Assert::IsTrue(proc_c10.getChildren()[0].getChildren()[2].getNodeType() == QueryNodeType::synonym);
			Assert::IsTrue(proc_c10.getChildren()[0].getChildren()[2].getSynonymType() == QuerySynonymType::print);
			Assert::IsTrue(proc_c10.getChildren()[0].getChildren()[2].getString().compare("pr") == 0);
		}

		TEST_METHOD(preProcessClauses_Select_Invalid_Success) {
			QueryPreProcessor qpp = QueryPreProcessor();

			DECLARATIONS d = "stmt s; read re; print pr; while w; if ifs; assign a; variable v; constant c; procedure p";
			PROCESSED_SYNONYMS proc_s = qpp.preProcessSynonyms(d);

			CLAUSES c1 = "Select";
			CLAUSES c2 = "Select s1";
			CLAUSES c3 = "select v";
			CLAUSES c4 = "Select 1";
			CLAUSES c5 = "Select <1>";
			CLAUSES c6 = "<s1 s2>";
			CLAUSES c7 = " <s1, s2>";
			CLAUSES c8 = "<s1, s2> ";
			CLAUSES c9 = "<s1.unknown, s2>";
			CLAUSES c10 = "<s1, s2.unknown>";
			CLAUSES c11 = "<s1.unknown>";


			PROCESSED_CLAUSES proc_c1 = qpp.preProcessClauses(proc_s, c1);
			PROCESSED_CLAUSES proc_c2 = qpp.preProcessClauses(proc_s, c2);
			PROCESSED_CLAUSES proc_c3 = qpp.preProcessClauses(proc_s, c3);
			PROCESSED_CLAUSES proc_c4 = qpp.preProcessClauses(proc_s, c4);
			PROCESSED_CLAUSES proc_c5 = qpp.preProcessClauses(proc_s, c5);
			PROCESSED_CLAUSES proc_c6 = qpp.preProcessClauses(proc_s, c6);
			PROCESSED_CLAUSES proc_c7 = qpp.preProcessClauses(proc_s, c7);
			PROCESSED_CLAUSES proc_c8 = qpp.preProcessClauses(proc_s, c8);
			PROCESSED_CLAUSES proc_c9 = qpp.preProcessClauses(proc_s, c9);
			PROCESSED_CLAUSES proc_c10 = qpp.preProcessClauses(proc_s, c10);
			PROCESSED_CLAUSES proc_c11 = qpp.preProcessClauses(proc_s, c11);

			Assert::IsTrue(proc_c1.getNodeType() == QueryNodeType::unassigned);
			Assert::IsTrue(proc_c2.getNodeType() == QueryNodeType::unassigned);
			Assert::IsTrue(proc_c3.getNodeType() == QueryNodeType::unassigned);
			Assert::IsTrue(proc_c4.getNodeType() == QueryNodeType::unassigned);
			Assert::IsTrue(proc_c5.getNodeType() == QueryNodeType::unassigned);
			Assert::IsTrue(proc_c6.getNodeType() == QueryNodeType::unassigned);
			Assert::IsTrue(proc_c7.getNodeType() == QueryNodeType::unassigned);
			Assert::IsTrue(proc_c8.getNodeType() == QueryNodeType::unassigned);
			Assert::IsTrue(proc_c9.getNodeType() == QueryNodeType::unassigned);
			Assert::IsTrue(proc_c10.getNodeType() == QueryNodeType::unassigned);
			Assert::IsTrue(proc_c11.getNodeType() == QueryNodeType::unassigned);
		}

		TEST_METHOD(preProcessClauses_Follows_Valid_Success) {
			QueryPreProcessor qpp = QueryPreProcessor();

			DECLARATIONS d = "stmt s; assign a;";
			PROCESSED_SYNONYMS proc_s = qpp.preProcessSynonyms(d);

			CLAUSES c1 = "Select s such that Follows(1, s)";
			CLAUSES c2 = "Select s such that Follows(s, a)";
			CLAUSES c3 = "Select s such that Follows*(_, s)";

			PROCESSED_CLAUSES proc_c1 = qpp.preProcessClauses(proc_s, c1);
			PROCESSED_CLAUSES proc_c2 = qpp.preProcessClauses(proc_s, c2);
			PROCESSED_CLAUSES proc_c3 = qpp.preProcessClauses(proc_s, c3);

			Assert::IsTrue(proc_c1.getChildren()[1].getNodeType() == QueryNodeType::such_that);
			Assert::IsTrue(proc_c1.getChildren()[1].getChildren()[0].getNodeType() == QueryNodeType::follows);
			Assert::IsTrue(proc_c1.getChildren()[1].getChildren()[0].getChildren()[0].getNodeType() == QueryNodeType::integer);
			Assert::IsTrue(proc_c1.getChildren()[1].getChildren()[0].getChildren()[0].getInteger() == 1);
			Assert::IsTrue(proc_c1.getChildren()[1].getChildren()[0].getChildren()[1].getNodeType() == QueryNodeType::synonym);
			Assert::IsTrue(proc_c1.getChildren()[1].getChildren()[0].getChildren()[1].getSynonymType() == QuerySynonymType::stmt);
			Assert::IsTrue(proc_c1.getChildren()[1].getChildren()[0].getChildren()[1].getString().compare("s") == 0);

			Assert::IsTrue(proc_c2.getChildren()[1].getNodeType() == QueryNodeType::such_that);
			Assert::IsTrue(proc_c2.getChildren()[1].getChildren()[0].getNodeType() == QueryNodeType::follows);
			Assert::IsTrue(proc_c2.getChildren()[1].getChildren()[0].getChildren()[0].getNodeType() == QueryNodeType::synonym);
			Assert::IsTrue(proc_c2.getChildren()[1].getChildren()[0].getChildren()[0].getSynonymType() == QuerySynonymType::stmt);
			Assert::IsTrue(proc_c2.getChildren()[1].getChildren()[0].getChildren()[0].getString().compare("s") == 0);
			Assert::IsTrue(proc_c2.getChildren()[1].getChildren()[0].getChildren()[1].getNodeType() == QueryNodeType::synonym);
			Assert::IsTrue(proc_c2.getChildren()[1].getChildren()[0].getChildren()[1].getSynonymType() == QuerySynonymType::assign);
			Assert::IsTrue(proc_c2.getChildren()[1].getChildren()[0].getChildren()[1].getString().compare("a") == 0);

			Assert::IsTrue(proc_c3.getChildren()[1].getNodeType() == QueryNodeType::such_that);
			Assert::IsTrue(proc_c3.getChildren()[1].getChildren()[0].getNodeType() == QueryNodeType::followsT);
			Assert::IsTrue(proc_c3.getChildren()[1].getChildren()[0].getChildren()[0].getNodeType() == QueryNodeType::wild_card);
			Assert::IsTrue(proc_c3.getChildren()[1].getChildren()[0].getChildren()[1].getNodeType() == QueryNodeType::synonym);
			Assert::IsTrue(proc_c3.getChildren()[1].getChildren()[0].getChildren()[1].getSynonymType() == QuerySynonymType::stmt);
			Assert::IsTrue(proc_c3.getChildren()[1].getChildren()[0].getChildren()[1].getString().compare("s") == 0);
		}

		TEST_METHOD(preProcessClauses_Follows_Invalid_Success) {
			QueryPreProcessor qpp = QueryPreProcessor();

			DECLARATIONS d = "stmt s;";
			PROCESSED_SYNONYMS proc_s = qpp.preProcessSynonyms(d);

			CLAUSES c1 = "Select s Such that Follows(1, s)";
			CLAUSES c2 = "Select s such that follows(s, a)";
			CLAUSES c3 = "Select s such that Follows*("", s)";
			CLAUSES c4 = "Select s such that FollowsT(_, s)";
			CLAUSES c5 = "Select s such that Follows*(, s)";
			CLAUSES c6 = "Select s such that Follows(_, )";
			CLAUSES c7 = "Select s such that Follows(1, "")";


			PROCESSED_CLAUSES proc_c1 = qpp.preProcessClauses(proc_s, c1);
			PROCESSED_CLAUSES proc_c2 = qpp.preProcessClauses(proc_s, c2);
			PROCESSED_CLAUSES proc_c3 = qpp.preProcessClauses(proc_s, c3);
			PROCESSED_CLAUSES proc_c4 = qpp.preProcessClauses(proc_s, c4);
			PROCESSED_CLAUSES proc_c5 = qpp.preProcessClauses(proc_s, c5);
			PROCESSED_CLAUSES proc_c6 = qpp.preProcessClauses(proc_s, c6);
			PROCESSED_CLAUSES proc_c7 = qpp.preProcessClauses(proc_s, c7);

			Assert::IsTrue(proc_c1.getNodeType() == QueryNodeType::unassigned);
			Assert::IsTrue(proc_c2.getNodeType() == QueryNodeType::unassigned);
			Assert::IsTrue(proc_c3.getNodeType() == QueryNodeType::unassigned);
			Assert::IsTrue(proc_c4.getNodeType() == QueryNodeType::unassigned);
			Assert::IsTrue(proc_c5.getNodeType() == QueryNodeType::unassigned);
			Assert::IsTrue(proc_c6.getNodeType() == QueryNodeType::unassigned);
			Assert::IsTrue(proc_c7.getNodeType() == QueryNodeType::unassigned);
		}

		TEST_METHOD(preProcessClauses_Parent_Valid_Success) {
			QueryPreProcessor qpp = QueryPreProcessor();

			DECLARATIONS d = "stmt s; assign a;";
			PROCESSED_SYNONYMS proc_s = qpp.preProcessSynonyms(d);

			CLAUSES c1 = "Select s such that Parent(1, s)";
			CLAUSES c2 = "Select s such that Parent(s, a)";
			CLAUSES c3 = "Select s such that Parent*(_, s)";

			PROCESSED_CLAUSES proc_c1 = qpp.preProcessClauses(proc_s, c1);
			PROCESSED_CLAUSES proc_c2 = qpp.preProcessClauses(proc_s, c2);
			PROCESSED_CLAUSES proc_c3 = qpp.preProcessClauses(proc_s, c3);

			Assert::IsTrue(proc_c1.getChildren()[1].getNodeType() == QueryNodeType::such_that);
			Assert::IsTrue(proc_c1.getChildren()[1].getChildren()[0].getNodeType() == QueryNodeType::parent);
			Assert::IsTrue(proc_c1.getChildren()[1].getChildren()[0].getChildren()[0].getNodeType() == QueryNodeType::integer);
			Assert::IsTrue(proc_c1.getChildren()[1].getChildren()[0].getChildren()[0].getInteger() == 1);
			Assert::IsTrue(proc_c1.getChildren()[1].getChildren()[0].getChildren()[1].getNodeType() == QueryNodeType::synonym);
			Assert::IsTrue(proc_c1.getChildren()[1].getChildren()[0].getChildren()[1].getSynonymType() == QuerySynonymType::stmt);
			Assert::IsTrue(proc_c1.getChildren()[1].getChildren()[0].getChildren()[1].getString().compare("s") == 0);

			Assert::IsTrue(proc_c2.getChildren()[1].getNodeType() == QueryNodeType::such_that);
			Assert::IsTrue(proc_c2.getChildren()[1].getChildren()[0].getNodeType() == QueryNodeType::parent);
			Assert::IsTrue(proc_c2.getChildren()[1].getChildren()[0].getChildren()[0].getNodeType() == QueryNodeType::synonym);
			Assert::IsTrue(proc_c2.getChildren()[1].getChildren()[0].getChildren()[0].getSynonymType() == QuerySynonymType::stmt);
			Assert::IsTrue(proc_c2.getChildren()[1].getChildren()[0].getChildren()[0].getString().compare("s") == 0);
			Assert::IsTrue(proc_c2.getChildren()[1].getChildren()[0].getChildren()[1].getNodeType() == QueryNodeType::synonym);
			Assert::IsTrue(proc_c2.getChildren()[1].getChildren()[0].getChildren()[1].getSynonymType() == QuerySynonymType::assign);
			Assert::IsTrue(proc_c2.getChildren()[1].getChildren()[0].getChildren()[1].getString().compare("a") == 0);

			Assert::IsTrue(proc_c3.getChildren()[1].getNodeType() == QueryNodeType::such_that);
			Assert::IsTrue(proc_c3.getChildren()[1].getChildren()[0].getNodeType() == QueryNodeType::parentT);
			Assert::IsTrue(proc_c3.getChildren()[1].getChildren()[0].getChildren()[0].getNodeType() == QueryNodeType::wild_card);
			Assert::IsTrue(proc_c3.getChildren()[1].getChildren()[0].getChildren()[1].getNodeType() == QueryNodeType::synonym);
			Assert::IsTrue(proc_c3.getChildren()[1].getChildren()[0].getChildren()[1].getSynonymType() == QuerySynonymType::stmt);
			Assert::IsTrue(proc_c3.getChildren()[1].getChildren()[0].getChildren()[1].getString().compare("s") == 0);
		}

		TEST_METHOD(preProcessClauses_Parent_Invalid_Success) {
			QueryPreProcessor qpp = QueryPreProcessor();

			DECLARATIONS d = "stmt s;";
			PROCESSED_SYNONYMS proc_s = qpp.preProcessSynonyms(d);

			CLAUSES c1 = "Select s Such that Parent(1, s)";
			CLAUSES c2 = "Select s such that parent(s, a)";
			CLAUSES c3 = "Select s such that Parent*("", s)";
			CLAUSES c4 = "Select s such that ParentT(_, s)";
			CLAUSES c5 = "Select s such that Parent*(, s)";
			CLAUSES c6 = "Select s such that Parent(_, )";
			CLAUSES c7 = "Select s such that Parent(1, "")";


			PROCESSED_CLAUSES proc_c1 = qpp.preProcessClauses(proc_s, c1);
			PROCESSED_CLAUSES proc_c2 = qpp.preProcessClauses(proc_s, c2);
			PROCESSED_CLAUSES proc_c3 = qpp.preProcessClauses(proc_s, c3);
			PROCESSED_CLAUSES proc_c4 = qpp.preProcessClauses(proc_s, c4);
			PROCESSED_CLAUSES proc_c5 = qpp.preProcessClauses(proc_s, c5);
			PROCESSED_CLAUSES proc_c6 = qpp.preProcessClauses(proc_s, c6);
			PROCESSED_CLAUSES proc_c7 = qpp.preProcessClauses(proc_s, c7);

			Assert::IsTrue(proc_c1.getNodeType() == QueryNodeType::unassigned);
			Assert::IsTrue(proc_c2.getNodeType() == QueryNodeType::unassigned);
			Assert::IsTrue(proc_c3.getNodeType() == QueryNodeType::unassigned);
			Assert::IsTrue(proc_c4.getNodeType() == QueryNodeType::unassigned);
			Assert::IsTrue(proc_c5.getNodeType() == QueryNodeType::unassigned);
			Assert::IsTrue(proc_c6.getNodeType() == QueryNodeType::unassigned);
			Assert::IsTrue(proc_c7.getNodeType() == QueryNodeType::unassigned);
		}

		TEST_METHOD(preProcessClauses_UsesS_Valid_Success) {
			QueryPreProcessor qpp = QueryPreProcessor();

			DECLARATIONS d = "variable v; assign a;";
			PROCESSED_SYNONYMS proc_s = qpp.preProcessSynonyms(d);

			CLAUSES c1 = "Select v such that Uses(a, v)";
			CLAUSES c2 = "Select v such that Uses(a, _)";
			CLAUSES c3 = "Select v such that Uses(1, v)";
			CLAUSES c4 = "Select v such that Uses(a, \"hello\")";


			PROCESSED_CLAUSES proc_c1 = qpp.preProcessClauses(proc_s, c1);
			PROCESSED_CLAUSES proc_c2 = qpp.preProcessClauses(proc_s, c2);
			PROCESSED_CLAUSES proc_c3 = qpp.preProcessClauses(proc_s, c3);
			PROCESSED_CLAUSES proc_c4 = qpp.preProcessClauses(proc_s, c4);

			Assert::IsTrue(proc_c1.getChildren()[1].getNodeType() == QueryNodeType::such_that);
			Assert::IsTrue(proc_c1.getChildren()[1].getChildren()[0].getNodeType() == QueryNodeType::usesS);
			Assert::IsTrue(proc_c1.getChildren()[1].getChildren()[0].getChildren()[0].getNodeType() == QueryNodeType::synonym);
			Assert::IsTrue(proc_c1.getChildren()[1].getChildren()[0].getChildren()[0].getSynonymType() == QuerySynonymType::assign);
			Assert::IsTrue(proc_c1.getChildren()[1].getChildren()[0].getChildren()[0].getString().compare("a") == 0);
			Assert::IsTrue(proc_c1.getChildren()[1].getChildren()[0].getChildren()[1].getNodeType() == QueryNodeType::synonym);
			Assert::IsTrue(proc_c1.getChildren()[1].getChildren()[0].getChildren()[1].getSynonymType() == QuerySynonymType::variable);
			Assert::IsTrue(proc_c1.getChildren()[1].getChildren()[0].getChildren()[1].getString().compare("v") == 0);

			Assert::IsTrue(proc_c2.getChildren()[1].getNodeType() == QueryNodeType::such_that);
			Assert::IsTrue(proc_c2.getChildren()[1].getChildren()[0].getNodeType() == QueryNodeType::usesS);
			Assert::IsTrue(proc_c2.getChildren()[1].getChildren()[0].getChildren()[0].getNodeType() == QueryNodeType::synonym);
			Assert::IsTrue(proc_c2.getChildren()[1].getChildren()[0].getChildren()[0].getSynonymType() == QuerySynonymType::assign);
			Assert::IsTrue(proc_c2.getChildren()[1].getChildren()[0].getChildren()[0].getString().compare("a") == 0);
			Assert::IsTrue(proc_c2.getChildren()[1].getChildren()[0].getChildren()[1].getNodeType() == QueryNodeType::wild_card);

			Assert::IsTrue(proc_c3.getChildren()[1].getNodeType() == QueryNodeType::such_that);
			Assert::IsTrue(proc_c3.getChildren()[1].getChildren()[0].getNodeType() == QueryNodeType::usesS);
			Assert::IsTrue(proc_c3.getChildren()[1].getChildren()[0].getChildren()[0].getNodeType() == QueryNodeType::integer);
			Assert::IsTrue(proc_c3.getChildren()[1].getChildren()[0].getChildren()[0].getInteger() == 1);
			Assert::IsTrue(proc_c3.getChildren()[1].getChildren()[0].getChildren()[1].getNodeType() == QueryNodeType::synonym);
			Assert::IsTrue(proc_c3.getChildren()[1].getChildren()[0].getChildren()[1].getSynonymType() == QuerySynonymType::variable);
			Assert::IsTrue(proc_c3.getChildren()[1].getChildren()[0].getChildren()[1].getString().compare("v") == 0);

			Assert::IsTrue(proc_c4.getChildren()[1].getNodeType() == QueryNodeType::such_that);
			Assert::IsTrue(proc_c4.getChildren()[1].getChildren()[0].getNodeType() == QueryNodeType::usesS);
			Assert::IsTrue(proc_c4.getChildren()[1].getChildren()[0].getChildren()[0].getNodeType() == QueryNodeType::synonym);
			Assert::IsTrue(proc_c4.getChildren()[1].getChildren()[0].getChildren()[0].getSynonymType() == QuerySynonymType::assign);
			Assert::IsTrue(proc_c4.getChildren()[1].getChildren()[0].getChildren()[0].getString().compare("a") == 0);
			Assert::IsTrue(proc_c4.getChildren()[1].getChildren()[0].getChildren()[1].getNodeType() == QueryNodeType::ident);
			Assert::IsTrue(proc_c4.getChildren()[1].getChildren()[0].getChildren()[1].getString().compare("hello") == 0);
		}

		TEST_METHOD(preProcessClauses_UsesP_Valid_Success) {
			QueryPreProcessor qpp = QueryPreProcessor();

			DECLARATIONS d = "variable v; procedure p;";
			PROCESSED_SYNONYMS proc_s = qpp.preProcessSynonyms(d);

			CLAUSES c1 = "Select v such that Uses(\"hello\", v)";
			CLAUSES c2 = "Select v such that Uses(p, v)";
			CLAUSES c3 = "Select v such that Uses(p, \"hello\")";
			CLAUSES c4 = "Select v such that Uses(p, _)";


			PROCESSED_CLAUSES proc_c1 = qpp.preProcessClauses(proc_s, c1);
			PROCESSED_CLAUSES proc_c2 = qpp.preProcessClauses(proc_s, c2);
			PROCESSED_CLAUSES proc_c3 = qpp.preProcessClauses(proc_s, c3);
			PROCESSED_CLAUSES proc_c4 = qpp.preProcessClauses(proc_s, c4);

			Assert::IsTrue(proc_c1.getChildren()[1].getNodeType() == QueryNodeType::such_that);
			Assert::IsTrue(proc_c1.getChildren()[1].getChildren()[0].getNodeType() == QueryNodeType::usesP);
			Assert::IsTrue(proc_c1.getChildren()[1].getChildren()[0].getChildren()[0].getNodeType() == QueryNodeType::ident);
			Assert::IsTrue(proc_c1.getChildren()[1].getChildren()[0].getChildren()[0].getString().compare("hello") == 0);
			Assert::IsTrue(proc_c1.getChildren()[1].getChildren()[0].getChildren()[1].getNodeType() == QueryNodeType::synonym);
			Assert::IsTrue(proc_c1.getChildren()[1].getChildren()[0].getChildren()[1].getSynonymType() == QuerySynonymType::variable);
			Assert::IsTrue(proc_c1.getChildren()[1].getChildren()[0].getChildren()[1].getString().compare("v") == 0);

			Assert::IsTrue(proc_c2.getChildren()[1].getNodeType() == QueryNodeType::such_that);
			Assert::IsTrue(proc_c2.getChildren()[1].getChildren()[0].getNodeType() == QueryNodeType::usesP);
			Assert::IsTrue(proc_c2.getChildren()[1].getChildren()[0].getChildren()[0].getNodeType() == QueryNodeType::synonym);
			Assert::IsTrue(proc_c2.getChildren()[1].getChildren()[0].getChildren()[0].getSynonymType() == QuerySynonymType::procedure);
			Assert::IsTrue(proc_c2.getChildren()[1].getChildren()[0].getChildren()[0].getString().compare("p") == 0);
			Assert::IsTrue(proc_c2.getChildren()[1].getChildren()[0].getChildren()[1].getNodeType() == QueryNodeType::synonym);
			Assert::IsTrue(proc_c2.getChildren()[1].getChildren()[0].getChildren()[1].getSynonymType() == QuerySynonymType::variable);
			Assert::IsTrue(proc_c2.getChildren()[1].getChildren()[0].getChildren()[1].getString().compare("v") == 0);

			Assert::IsTrue(proc_c3.getChildren()[1].getNodeType() == QueryNodeType::such_that);
			Assert::IsTrue(proc_c3.getChildren()[1].getChildren()[0].getNodeType() == QueryNodeType::usesP);
			Assert::IsTrue(proc_c3.getChildren()[1].getChildren()[0].getChildren()[0].getNodeType() == QueryNodeType::synonym);
			Assert::IsTrue(proc_c3.getChildren()[1].getChildren()[0].getChildren()[0].getSynonymType() == QuerySynonymType::procedure);
			Assert::IsTrue(proc_c3.getChildren()[1].getChildren()[0].getChildren()[0].getString().compare("p") == 0);
			Assert::IsTrue(proc_c3.getChildren()[1].getChildren()[0].getChildren()[1].getNodeType() == QueryNodeType::ident);
			Assert::IsTrue(proc_c3.getChildren()[1].getChildren()[0].getChildren()[1].getString().compare("hello") == 0);

			Assert::IsTrue(proc_c4.getChildren()[1].getNodeType() == QueryNodeType::such_that);
			Assert::IsTrue(proc_c4.getChildren()[1].getChildren()[0].getNodeType() == QueryNodeType::usesP);
			Assert::IsTrue(proc_c4.getChildren()[1].getChildren()[0].getChildren()[0].getNodeType() == QueryNodeType::synonym);
			Assert::IsTrue(proc_c4.getChildren()[1].getChildren()[0].getChildren()[0].getSynonymType() == QuerySynonymType::procedure);
			Assert::IsTrue(proc_c4.getChildren()[1].getChildren()[0].getChildren()[0].getString().compare("p") == 0);
			Assert::IsTrue(proc_c4.getChildren()[1].getChildren()[0].getChildren()[1].getNodeType() == QueryNodeType::wild_card);
		}

		TEST_METHOD(preProcessClauses_Uses_Invalid_Success) {
			QueryPreProcessor qpp = QueryPreProcessor();

			DECLARATIONS d = "variable v; assign a; procedure p; read re;";
			PROCESSED_SYNONYMS proc_s = qpp.preProcessSynonyms(d);

			CLAUSES c1 = "Select v Such that Uses(a, v)";
			CLAUSES c2 = "Select v such that uses(p, v)";
			CLAUSES c3 = "Select v such that Uses("", v)";
			CLAUSES c4 = "Select v such that Uses(_, v)";
			CLAUSES c5 = "Select v such that Uses(a, 1)";
			CLAUSES c6 = "Select v such that Uses(, 1)";
			CLAUSES c7 = "Select v such that Uses(a, )";
			CLAUSES c8 = "Select v such that UsesS(a, v)";
			CLAUSES c9 = "Select v such that Uses(re, v)";
			CLAUSES c10 = "Select v such that Usess(a, v)";


			PROCESSED_CLAUSES proc_c1 = qpp.preProcessClauses(proc_s, c1);
			PROCESSED_CLAUSES proc_c2 = qpp.preProcessClauses(proc_s, c2);
			PROCESSED_CLAUSES proc_c3 = qpp.preProcessClauses(proc_s, c3);
			PROCESSED_CLAUSES proc_c4 = qpp.preProcessClauses(proc_s, c4);
			PROCESSED_CLAUSES proc_c5 = qpp.preProcessClauses(proc_s, c5);
			PROCESSED_CLAUSES proc_c6 = qpp.preProcessClauses(proc_s, c6);
			PROCESSED_CLAUSES proc_c7 = qpp.preProcessClauses(proc_s, c7);
			PROCESSED_CLAUSES proc_c8 = qpp.preProcessClauses(proc_s, c8);
			PROCESSED_CLAUSES proc_c9 = qpp.preProcessClauses(proc_s, c9);
			PROCESSED_CLAUSES proc_c10 = qpp.preProcessClauses(proc_s, c10);

			Assert::IsTrue(proc_c1.getNodeType() == QueryNodeType::unassigned);
			Assert::IsTrue(proc_c2.getNodeType() == QueryNodeType::unassigned);
			Assert::IsTrue(proc_c3.getNodeType() == QueryNodeType::unassigned);
			Assert::IsTrue(proc_c4.getNodeType() == QueryNodeType::unassigned);
			Assert::IsTrue(proc_c5.getNodeType() == QueryNodeType::unassigned);
			Assert::IsTrue(proc_c6.getNodeType() == QueryNodeType::unassigned);
			Assert::IsTrue(proc_c7.getNodeType() == QueryNodeType::unassigned);
			Assert::IsTrue(proc_c8.getNodeType() == QueryNodeType::unassigned);
			Assert::IsTrue(proc_c9.getNodeType() == QueryNodeType::unassigned);
			Assert::IsTrue(proc_c10.getNodeType() == QueryNodeType::unassigned);
		}

		TEST_METHOD(preProcessClauses_ModifiesS_Valid_Success) {
			QueryPreProcessor qpp = QueryPreProcessor();

			DECLARATIONS d = "variable v; assign a;";
			PROCESSED_SYNONYMS proc_s = qpp.preProcessSynonyms(d);

			CLAUSES c1 = "Select v such that Modifies(a, v)";
			CLAUSES c2 = "Select v such that Modifies(a, _)";
			CLAUSES c3 = "Select v such that Modifies(1, v)";
			CLAUSES c4 = "Select v such that Modifies(a, \"hello\")";


			PROCESSED_CLAUSES proc_c1 = qpp.preProcessClauses(proc_s, c1);
			PROCESSED_CLAUSES proc_c2 = qpp.preProcessClauses(proc_s, c2);
			PROCESSED_CLAUSES proc_c3 = qpp.preProcessClauses(proc_s, c3);
			PROCESSED_CLAUSES proc_c4 = qpp.preProcessClauses(proc_s, c4);

			Assert::IsTrue(proc_c1.getChildren()[1].getNodeType() == QueryNodeType::such_that);
			Assert::IsTrue(proc_c1.getChildren()[1].getChildren()[0].getNodeType() == QueryNodeType::modifiesS);
			Assert::IsTrue(proc_c1.getChildren()[1].getChildren()[0].getChildren()[0].getNodeType() == QueryNodeType::synonym);
			Assert::IsTrue(proc_c1.getChildren()[1].getChildren()[0].getChildren()[0].getSynonymType() == QuerySynonymType::assign);
			Assert::IsTrue(proc_c1.getChildren()[1].getChildren()[0].getChildren()[0].getString().compare("a") == 0);
			Assert::IsTrue(proc_c1.getChildren()[1].getChildren()[0].getChildren()[1].getNodeType() == QueryNodeType::synonym);
			Assert::IsTrue(proc_c1.getChildren()[1].getChildren()[0].getChildren()[1].getSynonymType() == QuerySynonymType::variable);
			Assert::IsTrue(proc_c1.getChildren()[1].getChildren()[0].getChildren()[1].getString().compare("v") == 0);

			Assert::IsTrue(proc_c2.getChildren()[1].getNodeType() == QueryNodeType::such_that);
			Assert::IsTrue(proc_c2.getChildren()[1].getChildren()[0].getNodeType() == QueryNodeType::modifiesS);
			Assert::IsTrue(proc_c2.getChildren()[1].getChildren()[0].getChildren()[0].getNodeType() == QueryNodeType::synonym);
			Assert::IsTrue(proc_c2.getChildren()[1].getChildren()[0].getChildren()[0].getSynonymType() == QuerySynonymType::assign);
			Assert::IsTrue(proc_c2.getChildren()[1].getChildren()[0].getChildren()[0].getString().compare("a") == 0);
			Assert::IsTrue(proc_c2.getChildren()[1].getChildren()[0].getChildren()[1].getNodeType() == QueryNodeType::wild_card);

			Assert::IsTrue(proc_c3.getChildren()[1].getNodeType() == QueryNodeType::such_that);
			Assert::IsTrue(proc_c3.getChildren()[1].getChildren()[0].getNodeType() == QueryNodeType::modifiesS);
			Assert::IsTrue(proc_c3.getChildren()[1].getChildren()[0].getChildren()[0].getNodeType() == QueryNodeType::integer);
			Assert::IsTrue(proc_c3.getChildren()[1].getChildren()[0].getChildren()[0].getInteger() == 1);
			Assert::IsTrue(proc_c3.getChildren()[1].getChildren()[0].getChildren()[1].getNodeType() == QueryNodeType::synonym);
			Assert::IsTrue(proc_c3.getChildren()[1].getChildren()[0].getChildren()[1].getSynonymType() == QuerySynonymType::variable);
			Assert::IsTrue(proc_c3.getChildren()[1].getChildren()[0].getChildren()[1].getString().compare("v") == 0);

			Assert::IsTrue(proc_c4.getChildren()[1].getNodeType() == QueryNodeType::such_that);
			Assert::IsTrue(proc_c4.getChildren()[1].getChildren()[0].getNodeType() == QueryNodeType::modifiesS);
			Assert::IsTrue(proc_c4.getChildren()[1].getChildren()[0].getChildren()[0].getNodeType() == QueryNodeType::synonym);
			Assert::IsTrue(proc_c4.getChildren()[1].getChildren()[0].getChildren()[0].getSynonymType() == QuerySynonymType::assign);
			Assert::IsTrue(proc_c4.getChildren()[1].getChildren()[0].getChildren()[0].getString().compare("a") == 0);
			Assert::IsTrue(proc_c4.getChildren()[1].getChildren()[0].getChildren()[1].getNodeType() == QueryNodeType::ident);
			Assert::IsTrue(proc_c4.getChildren()[1].getChildren()[0].getChildren()[1].getString().compare("hello") == 0);
		}

		TEST_METHOD(preProcessClauses_ModifiesP_Valid_Success) {
			QueryPreProcessor qpp = QueryPreProcessor();

			DECLARATIONS d = "variable v; procedure p;";
			PROCESSED_SYNONYMS proc_s = qpp.preProcessSynonyms(d);

			CLAUSES c1 = "Select v such that Modifies(\"hello\", v)";
			CLAUSES c2 = "Select v such that Modifies(p, v)";
			CLAUSES c3 = "Select v such that Modifies(p, \"hello\")";
			CLAUSES c4 = "Select v such that Modifies(p, _)";


			PROCESSED_CLAUSES proc_c1 = qpp.preProcessClauses(proc_s, c1);
			PROCESSED_CLAUSES proc_c2 = qpp.preProcessClauses(proc_s, c2);
			PROCESSED_CLAUSES proc_c3 = qpp.preProcessClauses(proc_s, c3);
			PROCESSED_CLAUSES proc_c4 = qpp.preProcessClauses(proc_s, c4);

			Assert::IsTrue(proc_c1.getChildren()[1].getNodeType() == QueryNodeType::such_that);
			Assert::IsTrue(proc_c1.getChildren()[1].getChildren()[0].getNodeType() == QueryNodeType::modifiesP);
			Assert::IsTrue(proc_c1.getChildren()[1].getChildren()[0].getChildren()[0].getNodeType() == QueryNodeType::ident);
			Assert::IsTrue(proc_c1.getChildren()[1].getChildren()[0].getChildren()[0].getString().compare("hello") == 0);
			Assert::IsTrue(proc_c1.getChildren()[1].getChildren()[0].getChildren()[1].getNodeType() == QueryNodeType::synonym);
			Assert::IsTrue(proc_c1.getChildren()[1].getChildren()[0].getChildren()[1].getSynonymType() == QuerySynonymType::variable);
			Assert::IsTrue(proc_c1.getChildren()[1].getChildren()[0].getChildren()[1].getString().compare("v") == 0);

			Assert::IsTrue(proc_c2.getChildren()[1].getNodeType() == QueryNodeType::such_that);
			Assert::IsTrue(proc_c2.getChildren()[1].getChildren()[0].getNodeType() == QueryNodeType::modifiesP);
			Assert::IsTrue(proc_c2.getChildren()[1].getChildren()[0].getChildren()[0].getNodeType() == QueryNodeType::synonym);
			Assert::IsTrue(proc_c2.getChildren()[1].getChildren()[0].getChildren()[0].getSynonymType() == QuerySynonymType::procedure);
			Assert::IsTrue(proc_c2.getChildren()[1].getChildren()[0].getChildren()[0].getString().compare("p") == 0);
			Assert::IsTrue(proc_c2.getChildren()[1].getChildren()[0].getChildren()[1].getNodeType() == QueryNodeType::synonym);
			Assert::IsTrue(proc_c2.getChildren()[1].getChildren()[0].getChildren()[1].getSynonymType() == QuerySynonymType::variable);
			Assert::IsTrue(proc_c2.getChildren()[1].getChildren()[0].getChildren()[1].getString().compare("v") == 0);

			Assert::IsTrue(proc_c3.getChildren()[1].getNodeType() == QueryNodeType::such_that);
			Assert::IsTrue(proc_c3.getChildren()[1].getChildren()[0].getNodeType() == QueryNodeType::modifiesP);
			Assert::IsTrue(proc_c3.getChildren()[1].getChildren()[0].getChildren()[0].getNodeType() == QueryNodeType::synonym);
			Assert::IsTrue(proc_c3.getChildren()[1].getChildren()[0].getChildren()[0].getSynonymType() == QuerySynonymType::procedure);
			Assert::IsTrue(proc_c3.getChildren()[1].getChildren()[0].getChildren()[0].getString().compare("p") == 0);
			Assert::IsTrue(proc_c3.getChildren()[1].getChildren()[0].getChildren()[1].getNodeType() == QueryNodeType::ident);
			Assert::IsTrue(proc_c3.getChildren()[1].getChildren()[0].getChildren()[1].getString().compare("hello") == 0);

			Assert::IsTrue(proc_c4.getChildren()[1].getNodeType() == QueryNodeType::such_that);
			Assert::IsTrue(proc_c4.getChildren()[1].getChildren()[0].getNodeType() == QueryNodeType::modifiesP);
			Assert::IsTrue(proc_c4.getChildren()[1].getChildren()[0].getChildren()[0].getNodeType() == QueryNodeType::synonym);
			Assert::IsTrue(proc_c4.getChildren()[1].getChildren()[0].getChildren()[0].getSynonymType() == QuerySynonymType::procedure);
			Assert::IsTrue(proc_c4.getChildren()[1].getChildren()[0].getChildren()[0].getString().compare("p") == 0);
			Assert::IsTrue(proc_c4.getChildren()[1].getChildren()[0].getChildren()[1].getNodeType() == QueryNodeType::wild_card);
		}

		TEST_METHOD(preProcessClauses_Modifies_Invalid_Success) {
			QueryPreProcessor qpp = QueryPreProcessor();

			DECLARATIONS d = "variable v; assign a; procedure p; print pr;";
			PROCESSED_SYNONYMS proc_s = qpp.preProcessSynonyms(d);

			CLAUSES c1 = "Select v Such that Modifies(a, v)";
			CLAUSES c2 = "Select v such that modifies(p, v)";
			CLAUSES c3 = "Select v such that Modifies("", v)";
			CLAUSES c4 = "Select v such that Modifies(_, v)";
			CLAUSES c5 = "Select v such that Modifies(a, 1)";
			CLAUSES c6 = "Select v such that Modifies(, 1)";
			CLAUSES c7 = "Select v such that Modifies(a, )";
			CLAUSES c8 = "Select v such that ModifiesS(pr, v)";
			CLAUSES c9 = "Select v such that Modifiess(a, v)";


			PROCESSED_CLAUSES proc_c1 = qpp.preProcessClauses(proc_s, c1);
			PROCESSED_CLAUSES proc_c2 = qpp.preProcessClauses(proc_s, c2);
			PROCESSED_CLAUSES proc_c3 = qpp.preProcessClauses(proc_s, c3);
			PROCESSED_CLAUSES proc_c4 = qpp.preProcessClauses(proc_s, c4);
			PROCESSED_CLAUSES proc_c5 = qpp.preProcessClauses(proc_s, c5);
			PROCESSED_CLAUSES proc_c6 = qpp.preProcessClauses(proc_s, c6);
			PROCESSED_CLAUSES proc_c7 = qpp.preProcessClauses(proc_s, c7);
			PROCESSED_CLAUSES proc_c8 = qpp.preProcessClauses(proc_s, c8);
			PROCESSED_CLAUSES proc_c9 = qpp.preProcessClauses(proc_s, c9);

			Assert::IsTrue(proc_c1.getNodeType() == QueryNodeType::unassigned);
			Assert::IsTrue(proc_c2.getNodeType() == QueryNodeType::unassigned);
			Assert::IsTrue(proc_c3.getNodeType() == QueryNodeType::unassigned);
			Assert::IsTrue(proc_c4.getNodeType() == QueryNodeType::unassigned);
			Assert::IsTrue(proc_c5.getNodeType() == QueryNodeType::unassigned);
			Assert::IsTrue(proc_c6.getNodeType() == QueryNodeType::unassigned);
			Assert::IsTrue(proc_c7.getNodeType() == QueryNodeType::unassigned);
			Assert::IsTrue(proc_c8.getNodeType() == QueryNodeType::unassigned);
			Assert::IsTrue(proc_c9.getNodeType() == QueryNodeType::unassigned);
		}

		TEST_METHOD(preProcessClauses_Calls_Valid_Success) {
			QueryPreProcessor qpp = QueryPreProcessor();

			DECLARATIONS d = "procedure p1, p2;";
			PROCESSED_SYNONYMS proc_s = qpp.preProcessSynonyms(d);

			CLAUSES c1 = "Select p1 such that Calls(p1, p2)";
			CLAUSES c2 = "Select p1 such that Calls(_, p2)";
			CLAUSES c3 = "Select p1 such that Calls(\"hello\", p2)";
			CLAUSES c4 = "Select p1 such that Calls*(p1, p2)";

			PROCESSED_CLAUSES proc_c1 = qpp.preProcessClauses(proc_s, c1);
			PROCESSED_CLAUSES proc_c2 = qpp.preProcessClauses(proc_s, c2);
			PROCESSED_CLAUSES proc_c3 = qpp.preProcessClauses(proc_s, c3);
			PROCESSED_CLAUSES proc_c4 = qpp.preProcessClauses(proc_s, c4);

			Assert::IsTrue(proc_c1.getChildren()[1].getNodeType() == QueryNodeType::such_that);
			Assert::IsTrue(proc_c1.getChildren()[1].getChildren()[0].getNodeType() == QueryNodeType::calls);
			Assert::IsTrue(proc_c1.getChildren()[1].getChildren()[0].getChildren()[0].getNodeType() == QueryNodeType::synonym);
			Assert::IsTrue(proc_c1.getChildren()[1].getChildren()[0].getChildren()[0].getSynonymType() == QuerySynonymType::procedure);
			Assert::IsTrue(proc_c1.getChildren()[1].getChildren()[0].getChildren()[0].getString().compare("p1") == 0);
			Assert::IsTrue(proc_c1.getChildren()[1].getChildren()[0].getChildren()[1].getNodeType() == QueryNodeType::synonym);
			Assert::IsTrue(proc_c1.getChildren()[1].getChildren()[0].getChildren()[1].getSynonymType() == QuerySynonymType::procedure);
			Assert::IsTrue(proc_c1.getChildren()[1].getChildren()[0].getChildren()[1].getString().compare("p2") == 0);

			Assert::IsTrue(proc_c2.getChildren()[1].getNodeType() == QueryNodeType::such_that);
			Assert::IsTrue(proc_c2.getChildren()[1].getChildren()[0].getNodeType() == QueryNodeType::calls);
			Assert::IsTrue(proc_c2.getChildren()[1].getChildren()[0].getChildren()[0].getNodeType() == QueryNodeType::wild_card);
			Assert::IsTrue(proc_c2.getChildren()[1].getChildren()[0].getChildren()[1].getNodeType() == QueryNodeType::synonym);
			Assert::IsTrue(proc_c2.getChildren()[1].getChildren()[0].getChildren()[1].getSynonymType() == QuerySynonymType::procedure);
			Assert::IsTrue(proc_c2.getChildren()[1].getChildren()[0].getChildren()[1].getString().compare("p2") == 0);

			Assert::IsTrue(proc_c3.getChildren()[1].getNodeType() == QueryNodeType::such_that);
			Assert::IsTrue(proc_c3.getChildren()[1].getChildren()[0].getNodeType() == QueryNodeType::calls);
			Assert::IsTrue(proc_c3.getChildren()[1].getChildren()[0].getChildren()[0].getNodeType() == QueryNodeType::ident);
			Assert::IsTrue(proc_c3.getChildren()[1].getChildren()[0].getChildren()[0].getString().compare("hello") == 0);
			Assert::IsTrue(proc_c3.getChildren()[1].getChildren()[0].getChildren()[1].getNodeType() == QueryNodeType::synonym);
			Assert::IsTrue(proc_c3.getChildren()[1].getChildren()[0].getChildren()[1].getSynonymType() == QuerySynonymType::procedure);
			Assert::IsTrue(proc_c3.getChildren()[1].getChildren()[0].getChildren()[1].getString().compare("p2") == 0);

			Assert::IsTrue(proc_c4.getChildren()[1].getNodeType() == QueryNodeType::such_that);
			Assert::IsTrue(proc_c4.getChildren()[1].getChildren()[0].getNodeType() == QueryNodeType::callsT);
			Assert::IsTrue(proc_c4.getChildren()[1].getChildren()[0].getChildren()[0].getNodeType() == QueryNodeType::synonym);
			Assert::IsTrue(proc_c4.getChildren()[1].getChildren()[0].getChildren()[0].getSynonymType() == QuerySynonymType::procedure);
			Assert::IsTrue(proc_c4.getChildren()[1].getChildren()[0].getChildren()[0].getString().compare("p1") == 0);
			Assert::IsTrue(proc_c4.getChildren()[1].getChildren()[0].getChildren()[1].getNodeType() == QueryNodeType::synonym);
			Assert::IsTrue(proc_c4.getChildren()[1].getChildren()[0].getChildren()[1].getSynonymType() == QuerySynonymType::procedure);
			Assert::IsTrue(proc_c4.getChildren()[1].getChildren()[0].getChildren()[1].getString().compare("p2") == 0);
		}

		TEST_METHOD(preProcessClauses_Call_Invalid_Success) {
			QueryPreProcessor qpp = QueryPreProcessor();

			DECLARATIONS d = "procedure p1, p2;";
			PROCESSED_SYNONYMS proc_s = qpp.preProcessSynonyms(d);

			CLAUSES c1 = "Select p1 Such that Calls(1, p2)";
			CLAUSES c2 = "Select p1 such that calls(p1, p2)";
			CLAUSES c3 = "Select p1 such that Calls*(o, p2)";
			CLAUSES c4 = "Select p1 such that CallsT(p1, p2)";
			CLAUSES c5 = "Select p1 such that CallsT*(, s)";
			CLAUSES c6 = "Select p1 such that Calls(_, )";
			CLAUSES c7 = "Select p1 such that Calls(p1, 1)";


			PROCESSED_CLAUSES proc_c1 = qpp.preProcessClauses(proc_s, c1);
			PROCESSED_CLAUSES proc_c2 = qpp.preProcessClauses(proc_s, c2);
			PROCESSED_CLAUSES proc_c3 = qpp.preProcessClauses(proc_s, c3);
			PROCESSED_CLAUSES proc_c4 = qpp.preProcessClauses(proc_s, c4);
			PROCESSED_CLAUSES proc_c5 = qpp.preProcessClauses(proc_s, c5);
			PROCESSED_CLAUSES proc_c6 = qpp.preProcessClauses(proc_s, c6);
			PROCESSED_CLAUSES proc_c7 = qpp.preProcessClauses(proc_s, c7);

			Assert::IsTrue(proc_c1.getNodeType() == QueryNodeType::unassigned);
			Assert::IsTrue(proc_c2.getNodeType() == QueryNodeType::unassigned);
			Assert::IsTrue(proc_c3.getNodeType() == QueryNodeType::unassigned);
			Assert::IsTrue(proc_c4.getNodeType() == QueryNodeType::unassigned);
			Assert::IsTrue(proc_c5.getNodeType() == QueryNodeType::unassigned);
			Assert::IsTrue(proc_c6.getNodeType() == QueryNodeType::unassigned);
			Assert::IsTrue(proc_c7.getNodeType() == QueryNodeType::unassigned);
		}

		TEST_METHOD(preProcessClauses_Next_Valid_Success) {
			QueryPreProcessor qpp = QueryPreProcessor();

			DECLARATIONS d = "prog_line pl1, pl2;";
			PROCESSED_SYNONYMS proc_s = qpp.preProcessSynonyms(d);

			CLAUSES c1 = "Select pl1 such that Next(pl1, pl2)";
			CLAUSES c2 = "Select pl1 such that Next(_, pl2)";
			CLAUSES c3 = "Select pl1 such that Next(1, pl2)";
			CLAUSES c4 = "Select pl1 such that Next*(pl1, pl2)";

			PROCESSED_CLAUSES proc_c1 = qpp.preProcessClauses(proc_s, c1);
			PROCESSED_CLAUSES proc_c2 = qpp.preProcessClauses(proc_s, c2);
			PROCESSED_CLAUSES proc_c3 = qpp.preProcessClauses(proc_s, c3);
			PROCESSED_CLAUSES proc_c4 = qpp.preProcessClauses(proc_s, c4);

			Assert::IsTrue(proc_c1.getChildren()[1].getNodeType() == QueryNodeType::such_that);
			Assert::IsTrue(proc_c1.getChildren()[1].getChildren()[0].getNodeType() == QueryNodeType::next);
			Assert::IsTrue(proc_c1.getChildren()[1].getChildren()[0].getChildren()[0].getNodeType() == QueryNodeType::synonym);
			Assert::IsTrue(proc_c1.getChildren()[1].getChildren()[0].getChildren()[0].getSynonymType() == QuerySynonymType::prog_line);
			Assert::IsTrue(proc_c1.getChildren()[1].getChildren()[0].getChildren()[0].getString().compare("pl1") == 0);
			Assert::IsTrue(proc_c1.getChildren()[1].getChildren()[0].getChildren()[1].getNodeType() == QueryNodeType::synonym);
			Assert::IsTrue(proc_c1.getChildren()[1].getChildren()[0].getChildren()[1].getSynonymType() == QuerySynonymType::prog_line);
			Assert::IsTrue(proc_c1.getChildren()[1].getChildren()[0].getChildren()[1].getString().compare("pl2") == 0);

			Assert::IsTrue(proc_c2.getChildren()[1].getNodeType() == QueryNodeType::such_that);
			Assert::IsTrue(proc_c2.getChildren()[1].getChildren()[0].getNodeType() == QueryNodeType::next);
			Assert::IsTrue(proc_c2.getChildren()[1].getChildren()[0].getChildren()[0].getNodeType() == QueryNodeType::wild_card);
			Assert::IsTrue(proc_c2.getChildren()[1].getChildren()[0].getChildren()[1].getNodeType() == QueryNodeType::synonym);
			Assert::IsTrue(proc_c2.getChildren()[1].getChildren()[0].getChildren()[1].getSynonymType() == QuerySynonymType::prog_line);
			Assert::IsTrue(proc_c2.getChildren()[1].getChildren()[0].getChildren()[1].getString().compare("pl2") == 0);

			Assert::IsTrue(proc_c3.getChildren()[1].getNodeType() == QueryNodeType::such_that);
			Assert::IsTrue(proc_c3.getChildren()[1].getChildren()[0].getNodeType() == QueryNodeType::next);
			Assert::IsTrue(proc_c3.getChildren()[1].getChildren()[0].getChildren()[0].getNodeType() == QueryNodeType::integer);
			Assert::IsTrue(proc_c3.getChildren()[1].getChildren()[0].getChildren()[0].getInteger() == 1);
			Assert::IsTrue(proc_c3.getChildren()[1].getChildren()[0].getChildren()[1].getNodeType() == QueryNodeType::synonym);
			Assert::IsTrue(proc_c3.getChildren()[1].getChildren()[0].getChildren()[1].getSynonymType() == QuerySynonymType::prog_line);
			Assert::IsTrue(proc_c3.getChildren()[1].getChildren()[0].getChildren()[1].getString().compare("pl2") == 0);

			Assert::IsTrue(proc_c4.getChildren()[1].getNodeType() == QueryNodeType::such_that);
			Assert::IsTrue(proc_c4.getChildren()[1].getChildren()[0].getNodeType() == QueryNodeType::nextT);
			Assert::IsTrue(proc_c4.getChildren()[1].getChildren()[0].getChildren()[0].getNodeType() == QueryNodeType::synonym);
			Assert::IsTrue(proc_c4.getChildren()[1].getChildren()[0].getChildren()[0].getSynonymType() == QuerySynonymType::prog_line);
			Assert::IsTrue(proc_c4.getChildren()[1].getChildren()[0].getChildren()[0].getString().compare("pl1") == 0);
			Assert::IsTrue(proc_c4.getChildren()[1].getChildren()[0].getChildren()[1].getNodeType() == QueryNodeType::synonym);
			Assert::IsTrue(proc_c4.getChildren()[1].getChildren()[0].getChildren()[1].getSynonymType() == QuerySynonymType::prog_line);
			Assert::IsTrue(proc_c4.getChildren()[1].getChildren()[0].getChildren()[1].getString().compare("pl2") == 0);
		}

		TEST_METHOD(preProcessClauses_Next_Invalid_Success) {
			QueryPreProcessor qpp = QueryPreProcessor();

			DECLARATIONS d = "stmt s; variable v; prog_line pl1, pl2;";
			PROCESSED_SYNONYMS proc_s = qpp.preProcessSynonyms(d);

			CLAUSES c1 = "Select pl1 Such that Next(s, pl2)";
			CLAUSES c2 = "Select pl1 such that next(pl1, pl2)";
			CLAUSES c3 = "Select pl1 such that Next*(o, pl2)";
			CLAUSES c4 = "Select pl1 such that NextT(pl1, pl2)";
			CLAUSES c5 = "Select pl1 such that NextT*(, s)";
			CLAUSES c6 = "Select pl1 such that Next(_, )";
			CLAUSES c7 = "Select pl1 such that Next(pl1, v)";


			PROCESSED_CLAUSES proc_c1 = qpp.preProcessClauses(proc_s, c1);
			PROCESSED_CLAUSES proc_c2 = qpp.preProcessClauses(proc_s, c2);
			PROCESSED_CLAUSES proc_c3 = qpp.preProcessClauses(proc_s, c3);
			PROCESSED_CLAUSES proc_c4 = qpp.preProcessClauses(proc_s, c4);
			PROCESSED_CLAUSES proc_c5 = qpp.preProcessClauses(proc_s, c5);
			PROCESSED_CLAUSES proc_c6 = qpp.preProcessClauses(proc_s, c6);
			PROCESSED_CLAUSES proc_c7 = qpp.preProcessClauses(proc_s, c7);

			Assert::IsTrue(proc_c1.getNodeType() == QueryNodeType::unassigned);
			Assert::IsTrue(proc_c2.getNodeType() == QueryNodeType::unassigned);
			Assert::IsTrue(proc_c3.getNodeType() == QueryNodeType::unassigned);
			Assert::IsTrue(proc_c4.getNodeType() == QueryNodeType::unassigned);
			Assert::IsTrue(proc_c5.getNodeType() == QueryNodeType::unassigned);
			Assert::IsTrue(proc_c6.getNodeType() == QueryNodeType::unassigned);
			Assert::IsTrue(proc_c7.getNodeType() == QueryNodeType::unassigned);
		}

		TEST_METHOD(preProcessClauses_Pattern_Assign_Valid_Success) {
			QueryPreProcessor qpp = QueryPreProcessor();

			DECLARATIONS d = "assign a; variable v;";
			PROCESSED_SYNONYMS proc_s = qpp.preProcessSynonyms(d);

			CLAUSES c1 = "Select v pattern a(v, _)";
			CLAUSES c2 = "Select v pattern a(_, _)";
			CLAUSES c3 = "Select v pattern a(\"hello\", _)";
			CLAUSES c4 = "Select v pattern a(v, _\"v\"_)";
			CLAUSES c5 = "Select v pattern a(v, _\"1\"_)";
			CLAUSES c6 = "Select v pattern a(v, \"v\")";
			CLAUSES c7 = "Select v pattern a(v, \"1\")";
			CLAUSES c8 = "Select v pattern a(v, _\"3 * a\"_)";
			CLAUSES c9 = "Select v pattern a(v, \"3 + b / ( c - d )\")";

			PROCESSED_CLAUSES proc_c1 = qpp.preProcessClauses(proc_s, c1);
			PROCESSED_CLAUSES proc_c2 = qpp.preProcessClauses(proc_s, c2);
			PROCESSED_CLAUSES proc_c3 = qpp.preProcessClauses(proc_s, c3);
			PROCESSED_CLAUSES proc_c4 = qpp.preProcessClauses(proc_s, c4);
			PROCESSED_CLAUSES proc_c5 = qpp.preProcessClauses(proc_s, c5);
			PROCESSED_CLAUSES proc_c6 = qpp.preProcessClauses(proc_s, c6);
			PROCESSED_CLAUSES proc_c7 = qpp.preProcessClauses(proc_s, c7);
			PROCESSED_CLAUSES proc_c8 = qpp.preProcessClauses(proc_s, c8);
			PROCESSED_CLAUSES proc_c9 = qpp.preProcessClauses(proc_s, c9);
			
			Assert::IsTrue(proc_c1.getChildren()[1].getNodeType() == QueryNodeType::pattern);
			Assert::IsTrue(proc_c1.getChildren()[1].getChildren()[0].getNodeType() == QueryNodeType::synonym);
			Assert::IsTrue(proc_c1.getChildren()[1].getChildren()[0].getSynonymType() == QuerySynonymType::assign);
			Assert::IsTrue(proc_c1.getChildren()[1].getChildren()[0].getString().compare("a") == 0);
			Assert::IsTrue(proc_c1.getChildren()[1].getChildren()[1].getNodeType() == QueryNodeType::synonym);
			Assert::IsTrue(proc_c1.getChildren()[1].getChildren()[1].getSynonymType() == QuerySynonymType::variable);
			Assert::IsTrue(proc_c1.getChildren()[1].getChildren()[1].getString().compare("v") == 0);
			Assert::IsTrue(proc_c1.getChildren()[1].getChildren()[2].getNodeType() == QueryNodeType::wild_card);

			Assert::IsTrue(proc_c2.getChildren()[1].getNodeType() == QueryNodeType::pattern);
			Assert::IsTrue(proc_c2.getChildren()[1].getChildren()[0].getNodeType() == QueryNodeType::synonym);
			Assert::IsTrue(proc_c2.getChildren()[1].getChildren()[0].getSynonymType() == QuerySynonymType::assign);
			Assert::IsTrue(proc_c2.getChildren()[1].getChildren()[0].getString().compare("a") == 0);
			Assert::IsTrue(proc_c2.getChildren()[1].getChildren()[1].getNodeType() == QueryNodeType::wild_card);
			Assert::IsTrue(proc_c2.getChildren()[1].getChildren()[2].getNodeType() == QueryNodeType::wild_card);

			Assert::IsTrue(proc_c3.getChildren()[1].getNodeType() == QueryNodeType::pattern);
			Assert::IsTrue(proc_c3.getChildren()[1].getChildren()[0].getNodeType() == QueryNodeType::synonym);
			Assert::IsTrue(proc_c3.getChildren()[1].getChildren()[0].getSynonymType() == QuerySynonymType::assign);
			Assert::IsTrue(proc_c3.getChildren()[1].getChildren()[0].getString().compare("a") == 0);
			Assert::IsTrue(proc_c3.getChildren()[1].getChildren()[1].getNodeType() == QueryNodeType::ident);
			Assert::IsTrue(proc_c3.getChildren()[1].getChildren()[1].getString().compare("hello") == 0);
			Assert::IsTrue(proc_c3.getChildren()[1].getChildren()[2].getNodeType() == QueryNodeType::wild_card);

			Assert::IsTrue(proc_c4.getChildren()[1].getNodeType() == QueryNodeType::pattern);
			Assert::IsTrue(proc_c4.getChildren()[1].getChildren()[0].getNodeType() == QueryNodeType::synonym);
			Assert::IsTrue(proc_c4.getChildren()[1].getChildren()[0].getSynonymType() == QuerySynonymType::assign);
			Assert::IsTrue(proc_c4.getChildren()[1].getChildren()[0].getString().compare("a") == 0);
			Assert::IsTrue(proc_c4.getChildren()[1].getChildren()[1].getNodeType() == QueryNodeType::synonym);
			Assert::IsTrue(proc_c4.getChildren()[1].getChildren()[1].getSynonymType() == QuerySynonymType::variable);
			Assert::IsTrue(proc_c4.getChildren()[1].getChildren()[1].getString().compare("v") == 0);
			Assert::IsTrue(proc_c4.getChildren()[1].getChildren()[2].getNodeType() == QueryNodeType::partial_expression);
			Assert::IsTrue(proc_c4.getChildren()[1].getChildren()[2].getAstNode()->getNodeType() == NodeTypeEnum::expressionNode);
			std::shared_ptr<ExpressionNode> expr_node_c4 = std::static_pointer_cast<ExpressionNode>(proc_c4.getChildren()[1].getChildren()[2].getAstNode());
			Assert::IsTrue(expr_node_c4->getLeftAstNode()->getNodeType() == NodeTypeEnum::variableNode);

			Assert::IsTrue(proc_c5.getChildren()[1].getNodeType() == QueryNodeType::pattern);
			Assert::IsTrue(proc_c5.getChildren()[1].getChildren()[0].getNodeType() == QueryNodeType::synonym);
			Assert::IsTrue(proc_c5.getChildren()[1].getChildren()[0].getSynonymType() == QuerySynonymType::assign);
			Assert::IsTrue(proc_c5.getChildren()[1].getChildren()[0].getString().compare("a") == 0);
			Assert::IsTrue(proc_c5.getChildren()[1].getChildren()[1].getNodeType() == QueryNodeType::synonym);
			Assert::IsTrue(proc_c5.getChildren()[1].getChildren()[1].getSynonymType() == QuerySynonymType::variable);
			Assert::IsTrue(proc_c5.getChildren()[1].getChildren()[1].getString().compare("v") == 0);
			Assert::IsTrue(proc_c5.getChildren()[1].getChildren()[2].getNodeType() == QueryNodeType::partial_expression);
			Assert::IsTrue(proc_c5.getChildren()[1].getChildren()[2].getAstNode()->getNodeType() == NodeTypeEnum::expressionNode);
			std::shared_ptr<ExpressionNode> expr_node_c5 = std::static_pointer_cast<ExpressionNode>(proc_c5.getChildren()[1].getChildren()[2].getAstNode());
			Assert::IsTrue(expr_node_c5->getLeftAstNode()->getNodeType() == NodeTypeEnum::constantNode);

			Assert::IsTrue(proc_c6.getChildren()[1].getNodeType() == QueryNodeType::pattern);
			Assert::IsTrue(proc_c6.getChildren()[1].getChildren()[0].getNodeType() == QueryNodeType::synonym);
			Assert::IsTrue(proc_c6.getChildren()[1].getChildren()[0].getSynonymType() == QuerySynonymType::assign);
			Assert::IsTrue(proc_c6.getChildren()[1].getChildren()[0].getString().compare("a") == 0);
			Assert::IsTrue(proc_c6.getChildren()[1].getChildren()[1].getNodeType() == QueryNodeType::synonym);
			Assert::IsTrue(proc_c6.getChildren()[1].getChildren()[1].getSynonymType() == QuerySynonymType::variable);
			Assert::IsTrue(proc_c6.getChildren()[1].getChildren()[1].getString().compare("v") == 0);
			Assert::IsTrue(proc_c6.getChildren()[1].getChildren()[2].getNodeType() == QueryNodeType::expression);
			Assert::IsTrue(proc_c6.getChildren()[1].getChildren()[2].getAstNode()->getNodeType() == NodeTypeEnum::expressionNode);
			std::shared_ptr<ExpressionNode> expr_node_c6 = std::static_pointer_cast<ExpressionNode>(proc_c6.getChildren()[1].getChildren()[2].getAstNode());
			Assert::IsTrue(expr_node_c6->getLeftAstNode()->getNodeType() == NodeTypeEnum::variableNode);

			Assert::IsTrue(proc_c7.getChildren()[1].getNodeType() == QueryNodeType::pattern);
			Assert::IsTrue(proc_c7.getChildren()[1].getChildren()[0].getNodeType() == QueryNodeType::synonym);
			Assert::IsTrue(proc_c7.getChildren()[1].getChildren()[0].getSynonymType() == QuerySynonymType::assign);
			Assert::IsTrue(proc_c7.getChildren()[1].getChildren()[0].getString().compare("a") == 0);
			Assert::IsTrue(proc_c7.getChildren()[1].getChildren()[1].getNodeType() == QueryNodeType::synonym);
			Assert::IsTrue(proc_c7.getChildren()[1].getChildren()[1].getSynonymType() == QuerySynonymType::variable);
			Assert::IsTrue(proc_c7.getChildren()[1].getChildren()[1].getString().compare("v") == 0);
			Assert::IsTrue(proc_c7.getChildren()[1].getChildren()[2].getNodeType() == QueryNodeType::expression);
			Assert::IsTrue(proc_c7.getChildren()[1].getChildren()[2].getAstNode()->getNodeType() == NodeTypeEnum::expressionNode);
			std::shared_ptr<ExpressionNode> expr_node_c7 = std::static_pointer_cast<ExpressionNode>(proc_c7.getChildren()[1].getChildren()[2].getAstNode());
			Assert::IsTrue(expr_node_c7->getLeftAstNode()->getNodeType() == NodeTypeEnum::constantNode);

			Assert::IsTrue(proc_c8.getChildren()[1].getNodeType() == QueryNodeType::pattern);
			Assert::IsTrue(proc_c8.getChildren()[1].getChildren()[0].getNodeType() == QueryNodeType::synonym);
			Assert::IsTrue(proc_c8.getChildren()[1].getChildren()[0].getSynonymType() == QuerySynonymType::assign);
			Assert::IsTrue(proc_c8.getChildren()[1].getChildren()[0].getString().compare("a") == 0);
			Assert::IsTrue(proc_c8.getChildren()[1].getChildren()[1].getNodeType() == QueryNodeType::synonym);
			Assert::IsTrue(proc_c8.getChildren()[1].getChildren()[1].getSynonymType() == QuerySynonymType::variable);
			Assert::IsTrue(proc_c8.getChildren()[1].getChildren()[1].getString().compare("v") == 0);
			Assert::IsTrue(proc_c8.getChildren()[1].getChildren()[2].getNodeType() == QueryNodeType::partial_expression);
			Assert::IsTrue(proc_c8.getChildren()[1].getChildren()[2].getAstNode()->getNodeType() == NodeTypeEnum::expressionNode);
			std::shared_ptr<ExpressionNode> expr_node_c8 = std::static_pointer_cast<ExpressionNode>(proc_c8.getChildren()[1].getChildren()[2].getAstNode());
			Assert::IsTrue(expr_node_c8->getLeftAstNode()->getNodeType() == NodeTypeEnum::constantNode);
			Assert::IsTrue(expr_node_c8->getRightAstNode()->getNodeType() == NodeTypeEnum::variableNode);
			
			Assert::IsTrue(proc_c9.getChildren()[1].getNodeType() == QueryNodeType::pattern);
			Assert::IsTrue(proc_c9.getChildren()[1].getChildren()[0].getNodeType() == QueryNodeType::synonym);
			Assert::IsTrue(proc_c9.getChildren()[1].getChildren()[0].getSynonymType() == QuerySynonymType::assign);
			Assert::IsTrue(proc_c9.getChildren()[1].getChildren()[0].getString().compare("a") == 0);
			Assert::IsTrue(proc_c9.getChildren()[1].getChildren()[1].getNodeType() == QueryNodeType::synonym);
			Assert::IsTrue(proc_c9.getChildren()[1].getChildren()[1].getSynonymType() == QuerySynonymType::variable);
			Assert::IsTrue(proc_c9.getChildren()[1].getChildren()[1].getString().compare("v") == 0);
			Assert::IsTrue(proc_c9.getChildren()[1].getChildren()[2].getNodeType() == QueryNodeType::expression);
			Assert::IsTrue(proc_c9.getChildren()[1].getChildren()[2].getAstNode()->getNodeType() == NodeTypeEnum::expressionNode);
			std::shared_ptr<ExpressionNode> expr_node_c9_1 = std::static_pointer_cast<ExpressionNode>(proc_c9.getChildren()[1].getChildren()[2].getAstNode());
			Assert::IsTrue(expr_node_c9_1->getLeftAstNode()->getNodeType() == NodeTypeEnum::constantNode);
			Assert::IsTrue(expr_node_c9_1->getRightAstNode()->getNodeType() == NodeTypeEnum::expressionNode);
			std::shared_ptr<ExpressionNode> expr_node_c9_2 = std::static_pointer_cast<ExpressionNode>(expr_node_c9_1->getRightAstNode());
			Assert::IsTrue(expr_node_c9_2->getLeftAstNode()->getNodeType() == NodeTypeEnum::variableNode);
			Assert::IsTrue(expr_node_c9_2->getRightAstNode()->getNodeType() == NodeTypeEnum::expressionNode);
			std::shared_ptr<ExpressionNode> expr_node_c9_3 = std::static_pointer_cast<ExpressionNode>(expr_node_c9_2->getRightAstNode());
			Assert::IsTrue(expr_node_c9_3->getLeftAstNode()->getNodeType() == NodeTypeEnum::variableNode);
			Assert::IsTrue(expr_node_c9_3->getRightAstNode()->getNodeType() == NodeTypeEnum::variableNode);
		}

		TEST_METHOD(preProcessClauses_Pattern_If_Valid_Success) {
			QueryPreProcessor qpp = QueryPreProcessor();

			DECLARATIONS d = "if ifs; variable v;";
			PROCESSED_SYNONYMS proc_s = qpp.preProcessSynonyms(d);

			CLAUSES c1 = "Select v pattern ifs(_, _, _)";
			CLAUSES c2 = "Select v pattern ifs(v, _, _)";
			CLAUSES c3 = "Select v pattern ifs(\"hello\", _, _)";

			PROCESSED_CLAUSES proc_c1 = qpp.preProcessClauses(proc_s, c1);
			PROCESSED_CLAUSES proc_c2 = qpp.preProcessClauses(proc_s, c2);
			PROCESSED_CLAUSES proc_c3 = qpp.preProcessClauses(proc_s, c3);

			Assert::IsTrue(proc_c1.getChildren()[1].getNodeType() == QueryNodeType::pattern);
			Assert::IsTrue(proc_c1.getChildren()[1].getChildren()[0].getNodeType() == QueryNodeType::synonym);
			Assert::IsTrue(proc_c1.getChildren()[1].getChildren()[0].getSynonymType() == QuerySynonymType::ifs);
			Assert::IsTrue(proc_c1.getChildren()[1].getChildren()[0].getString().compare("ifs") == 0);
			Assert::IsTrue(proc_c1.getChildren()[1].getChildren()[1].getNodeType() == QueryNodeType::wild_card);
			Assert::IsTrue(proc_c1.getChildren()[1].getChildren()[2].getNodeType() == QueryNodeType::wild_card);
			Assert::IsTrue(proc_c1.getChildren()[1].getChildren()[3].getNodeType() == QueryNodeType::wild_card);

			Assert::IsTrue(proc_c2.getChildren()[1].getNodeType() == QueryNodeType::pattern);
			Assert::IsTrue(proc_c2.getChildren()[1].getChildren()[0].getNodeType() == QueryNodeType::synonym);
			Assert::IsTrue(proc_c2.getChildren()[1].getChildren()[0].getSynonymType() == QuerySynonymType::ifs);
			Assert::IsTrue(proc_c2.getChildren()[1].getChildren()[0].getString().compare("ifs") == 0);
			Assert::IsTrue(proc_c2.getChildren()[1].getChildren()[1].getNodeType() == QueryNodeType::synonym);
			Assert::IsTrue(proc_c2.getChildren()[1].getChildren()[1].getSynonymType() == QuerySynonymType::variable);
			Assert::IsTrue(proc_c2.getChildren()[1].getChildren()[1].getString().compare("v") == 0);
			Assert::IsTrue(proc_c2.getChildren()[1].getChildren()[2].getNodeType() == QueryNodeType::wild_card);
			Assert::IsTrue(proc_c2.getChildren()[1].getChildren()[3].getNodeType() == QueryNodeType::wild_card);

			Assert::IsTrue(proc_c3.getChildren()[1].getNodeType() == QueryNodeType::pattern);
			Assert::IsTrue(proc_c3.getChildren()[1].getChildren()[0].getNodeType() == QueryNodeType::synonym);
			Assert::IsTrue(proc_c3.getChildren()[1].getChildren()[0].getSynonymType() == QuerySynonymType::ifs);
			Assert::IsTrue(proc_c3.getChildren()[1].getChildren()[0].getString().compare("ifs") == 0);
			Assert::IsTrue(proc_c3.getChildren()[1].getChildren()[1].getNodeType() == QueryNodeType::ident);
			Assert::IsTrue(proc_c3.getChildren()[1].getChildren()[1].getString().compare("hello") == 0);
			Assert::IsTrue(proc_c3.getChildren()[1].getChildren()[2].getNodeType() == QueryNodeType::wild_card);
			Assert::IsTrue(proc_c3.getChildren()[1].getChildren()[3].getNodeType() == QueryNodeType::wild_card);
		}

		TEST_METHOD(preProcessClauses_Pattern_While_Valid_Success) {
			QueryPreProcessor qpp = QueryPreProcessor();

			DECLARATIONS d = "while w; variable v;";
			PROCESSED_SYNONYMS proc_s = qpp.preProcessSynonyms(d);

			CLAUSES c1 = "Select v pattern w(_, _)";
			CLAUSES c2 = "Select v pattern w(v, _)";
			CLAUSES c3 = "Select v pattern w(\"hello\", _)";


			PROCESSED_CLAUSES proc_c1 = qpp.preProcessClauses(proc_s, c1);
			PROCESSED_CLAUSES proc_c2 = qpp.preProcessClauses(proc_s, c2);
			PROCESSED_CLAUSES proc_c3 = qpp.preProcessClauses(proc_s, c3);

			Assert::IsTrue(proc_c1.getChildren()[1].getNodeType() == QueryNodeType::pattern);
			Assert::IsTrue(proc_c1.getChildren()[1].getChildren()[0].getNodeType() == QueryNodeType::synonym);
			Assert::IsTrue(proc_c1.getChildren()[1].getChildren()[0].getSynonymType() == QuerySynonymType::whiles);
			Assert::IsTrue(proc_c1.getChildren()[1].getChildren()[0].getString().compare("w") == 0);
			Assert::IsTrue(proc_c1.getChildren()[1].getChildren()[1].getNodeType() == QueryNodeType::wild_card);
			Assert::IsTrue(proc_c1.getChildren()[1].getChildren()[2].getNodeType() == QueryNodeType::wild_card);

			Assert::IsTrue(proc_c2.getChildren()[1].getNodeType() == QueryNodeType::pattern);
			Assert::IsTrue(proc_c2.getChildren()[1].getChildren()[0].getNodeType() == QueryNodeType::synonym);
			Assert::IsTrue(proc_c2.getChildren()[1].getChildren()[0].getSynonymType() == QuerySynonymType::whiles);
			Assert::IsTrue(proc_c2.getChildren()[1].getChildren()[0].getString().compare("w") == 0);
			Assert::IsTrue(proc_c2.getChildren()[1].getChildren()[1].getNodeType() == QueryNodeType::synonym);
			Assert::IsTrue(proc_c2.getChildren()[1].getChildren()[1].getSynonymType() == QuerySynonymType::variable);
			Assert::IsTrue(proc_c2.getChildren()[1].getChildren()[1].getString().compare("v") == 0);
			Assert::IsTrue(proc_c2.getChildren()[1].getChildren()[2].getNodeType() == QueryNodeType::wild_card);

			Assert::IsTrue(proc_c3.getChildren()[1].getNodeType() == QueryNodeType::pattern);
			Assert::IsTrue(proc_c3.getChildren()[1].getChildren()[0].getNodeType() == QueryNodeType::synonym);
			Assert::IsTrue(proc_c3.getChildren()[1].getChildren()[0].getSynonymType() == QuerySynonymType::whiles);
			Assert::IsTrue(proc_c3.getChildren()[1].getChildren()[0].getString().compare("w") == 0);
			Assert::IsTrue(proc_c3.getChildren()[1].getChildren()[1].getNodeType() == QueryNodeType::ident);
			Assert::IsTrue(proc_c3.getChildren()[1].getChildren()[1].getString().compare("hello") == 0);
			Assert::IsTrue(proc_c3.getChildren()[1].getChildren()[2].getNodeType() == QueryNodeType::wild_card);
		}
		TEST_METHOD(preProcessClauses_Pattern_Assign_Invalid_Success) {
			QueryPreProcessor qpp = QueryPreProcessor();

			DECLARATIONS d = "assign a; variable v;";
			PROCESSED_SYNONYMS proc_s = qpp.preProcessSynonyms(d);

			CLAUSES c1 = "Select v pattern (_, _)";
			CLAUSES c2 = "Select v pattern v(_, _)";
			CLAUSES c3 = "Select v pattern a(, _)";
			CLAUSES c4 = "Select v pattern a(_,)";
			CLAUSES c5 = "Select v Pattern a(_, _)";
			CLAUSES c6 = "Select v pattern a(_, v)";
			CLAUSES c7 = "Select v pattern a(_, \"1 +\")";
			CLAUSES c8 = "Select v pattern a(_, \"+\")";
			CLAUSES c9 = "Select v pattern a(_, \"1 b\")";
			CLAUSES c10 = "Select v pattern a(_, \"( 1 + 2\")";

			PROCESSED_CLAUSES proc_c1 = qpp.preProcessClauses(proc_s, c1);
			PROCESSED_CLAUSES proc_c2 = qpp.preProcessClauses(proc_s, c2);
			PROCESSED_CLAUSES proc_c3 = qpp.preProcessClauses(proc_s, c3);
			PROCESSED_CLAUSES proc_c4 = qpp.preProcessClauses(proc_s, c4);
			PROCESSED_CLAUSES proc_c5 = qpp.preProcessClauses(proc_s, c5);
			PROCESSED_CLAUSES proc_c6 = qpp.preProcessClauses(proc_s, c6);
			PROCESSED_CLAUSES proc_c7 = qpp.preProcessClauses(proc_s, c7);
			PROCESSED_CLAUSES proc_c8 = qpp.preProcessClauses(proc_s, c8);
			PROCESSED_CLAUSES proc_c9 = qpp.preProcessClauses(proc_s, c9);
			PROCESSED_CLAUSES proc_c10 = qpp.preProcessClauses(proc_s, c10);

			Assert::IsTrue(proc_c1.getNodeType() == QueryNodeType::unassigned);
			Assert::IsTrue(proc_c2.getNodeType() == QueryNodeType::unassigned);
			Assert::IsTrue(proc_c3.getNodeType() == QueryNodeType::unassigned);
			Assert::IsTrue(proc_c4.getNodeType() == QueryNodeType::unassigned);
			Assert::IsTrue(proc_c5.getNodeType() == QueryNodeType::unassigned);
			Assert::IsTrue(proc_c6.getNodeType() == QueryNodeType::unassigned);
			Assert::IsTrue(proc_c7.getNodeType() == QueryNodeType::unassigned);
			Assert::IsTrue(proc_c8.getNodeType() == QueryNodeType::unassigned);
			Assert::IsTrue(proc_c9.getNodeType() == QueryNodeType::unassigned);
			Assert::IsTrue(proc_c10.getNodeType() == QueryNodeType::unassigned);
		}

		TEST_METHOD(preProcessClauses_Pattern_If_Invalid_Success) {
			QueryPreProcessor qpp = QueryPreProcessor();

			DECLARATIONS d = "if ifs; variable v;";
			PROCESSED_SYNONYMS proc_s = qpp.preProcessSynonyms(d);

			CLAUSES c1 = "Select v pattern (_, _, _)";
			CLAUSES c2 = "Select v pattern i(_, _,_)";
			CLAUSES c3 = "Select v pattern ifs(_, 1, _)";
			CLAUSES c4 = "Select v pattern ifs(_, _, 1)";
			CLAUSES c5 = "Select v Pattern ifs(_, , _)";
			CLAUSES c6 = "Select v pattern ifs(_, \"x\", _)";
			CLAUSES c7 = "Select v pattern ifs(_, _, \"x\")";

			PROCESSED_CLAUSES proc_c1 = qpp.preProcessClauses(proc_s, c1);
			PROCESSED_CLAUSES proc_c2 = qpp.preProcessClauses(proc_s, c2);
			PROCESSED_CLAUSES proc_c3 = qpp.preProcessClauses(proc_s, c3);
			PROCESSED_CLAUSES proc_c4 = qpp.preProcessClauses(proc_s, c4);
			PROCESSED_CLAUSES proc_c5 = qpp.preProcessClauses(proc_s, c5);
			PROCESSED_CLAUSES proc_c6 = qpp.preProcessClauses(proc_s, c6);
			PROCESSED_CLAUSES proc_c7 = qpp.preProcessClauses(proc_s, c7);

			Assert::IsTrue(proc_c1.getNodeType() == QueryNodeType::unassigned);
			Assert::IsTrue(proc_c2.getNodeType() == QueryNodeType::unassigned);
			Assert::IsTrue(proc_c3.getNodeType() == QueryNodeType::unassigned);
			Assert::IsTrue(proc_c4.getNodeType() == QueryNodeType::unassigned);
			Assert::IsTrue(proc_c5.getNodeType() == QueryNodeType::unassigned);
			Assert::IsTrue(proc_c6.getNodeType() == QueryNodeType::unassigned);
			Assert::IsTrue(proc_c7.getNodeType() == QueryNodeType::unassigned);
		}

		TEST_METHOD(preProcessClauses_Pattern_While_Invalid_Success) {
			QueryPreProcessor qpp = QueryPreProcessor();

			DECLARATIONS d = "while w; variable v;";
			PROCESSED_SYNONYMS proc_s = qpp.preProcessSynonyms(d);

			CLAUSES c1 = "Select v pattern (_, _)";
			CLAUSES c2 = "Select v pattern v(_, _)";
			CLAUSES c3 = "Select v pattern w(, _)";
			CLAUSES c4 = "Select v pattern w(_,)";
			CLAUSES c5 = "Select v Pattern w(_, 1)";
			CLAUSES c6 = "Select v pattern w(_, v)";

			PROCESSED_CLAUSES proc_c1 = qpp.preProcessClauses(proc_s, c1);
			PROCESSED_CLAUSES proc_c2 = qpp.preProcessClauses(proc_s, c2);
			PROCESSED_CLAUSES proc_c3 = qpp.preProcessClauses(proc_s, c3);
			PROCESSED_CLAUSES proc_c4 = qpp.preProcessClauses(proc_s, c4);
			PROCESSED_CLAUSES proc_c5 = qpp.preProcessClauses(proc_s, c5);
			PROCESSED_CLAUSES proc_c6 = qpp.preProcessClauses(proc_s, c6);

			Assert::IsTrue(proc_c1.getNodeType() == QueryNodeType::unassigned);
			Assert::IsTrue(proc_c2.getNodeType() == QueryNodeType::unassigned);
			Assert::IsTrue(proc_c3.getNodeType() == QueryNodeType::unassigned);
			Assert::IsTrue(proc_c4.getNodeType() == QueryNodeType::unassigned);
			Assert::IsTrue(proc_c5.getNodeType() == QueryNodeType::unassigned);
			Assert::IsTrue(proc_c6.getNodeType() == QueryNodeType::unassigned);
		}
	};
}