#include <string>

#include "stdafx.h"
#include "CppUnitTest.h"
#include "QueryValidator.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{
	TEST_CLASS(TestQueryValidator)
	{
	public:

		TEST_METHOD(isValidStructure_Valid_Success) {
			QueryValidator qv = QueryValidator();

			QUERY q1 = "stmt s; Select s";
			QUERY q2 = " stmt s; Select s";
			QUERY q3 = "stmt s; Select s ";
			QUERY q4 = " stmt s; Select s ";
			QUERY q5 = "  stmt s; Select s  ";
			QUERY q6 = "stmt s; Select s such that Follows(1, s)";
			QUERY q7 = "stmt s; Select s pattern a(_, _)";
			QUERY q8 = "stmt s; Select s such that Follows(1, s) pattern a(_, _)";
			QUERY q9 = "stmt s; Select s pattern a(_, _) such that Follows(1, s)";

			Assert::IsTrue(qv.isValidStructure(q1));
			Assert::IsTrue(qv.isValidStructure(q2));
			Assert::IsTrue(qv.isValidStructure(q3));
			Assert::IsTrue(qv.isValidStructure(q4));
			Assert::IsTrue(qv.isValidStructure(q5));
			Assert::IsTrue(qv.isValidStructure(q6));
			Assert::IsTrue(qv.isValidStructure(q7));
			Assert::IsTrue(qv.isValidStructure(q8));
			Assert::IsTrue(qv.isValidStructure(q9));
		}

		TEST_METHOD(isValidStructure_Invalid_Success) {
			QueryValidator qv = QueryValidator();

			QUERY q1 = "";
			QUERY q2 = "Select s";
			QUERY q3 = "stmt s;";
			QUERY q4 = "stmt s Select s";
			QUERY q5 = "stmt s; select s";

			Assert::IsFalse(qv.isValidStructure(q1));
			Assert::IsFalse(qv.isValidStructure(q2));
			Assert::IsFalse(qv.isValidStructure(q3));
			Assert::IsFalse(qv.isValidStructure(q4));
			Assert::IsFalse(qv.isValidStructure(q5));
		}

		TEST_METHOD(isValidDeclaration_Valid_Success) {
			QueryValidator qv = QueryValidator();

			SINGLE_DECLARATION d1 = "stmt s";
			SINGLE_DECLARATION d2 = "read re";
			SINGLE_DECLARATION d3 = "print pr";
			SINGLE_DECLARATION d4 = "while w";
			SINGLE_DECLARATION d5 = "if ifs";
			SINGLE_DECLARATION d6 = "assign a";
			SINGLE_DECLARATION d7 = "variable v";
			SINGLE_DECLARATION d8 = "constant c";
			SINGLE_DECLARATION d9 = "procedure p";

			SINGLE_DECLARATION d10 = "stmt s1, s2, s3, s4, s5";
			SINGLE_DECLARATION d11 = "stmt s1,s2,s3,s4,s5";
			SINGLE_DECLARATION d12 = "stmt        s1,    s2,      s3,     s4,     s5";

			Assert::IsTrue(qv.isValidDeclaration(d1));
			Assert::IsTrue(qv.isValidDeclaration(d2));
			Assert::IsTrue(qv.isValidDeclaration(d3));
			Assert::IsTrue(qv.isValidDeclaration(d4));
			Assert::IsTrue(qv.isValidDeclaration(d5));
			Assert::IsTrue(qv.isValidDeclaration(d6));
			Assert::IsTrue(qv.isValidDeclaration(d7));
			Assert::IsTrue(qv.isValidDeclaration(d8));
			Assert::IsTrue(qv.isValidDeclaration(d9));
			Assert::IsTrue(qv.isValidDeclaration(d10));
			Assert::IsTrue(qv.isValidDeclaration(d11));
			Assert::IsTrue(qv.isValidDeclaration(d12));
		}

		TEST_METHOD(isValidDeclaration_Invalid_Success) {
			QueryValidator qv = QueryValidator();

			SINGLE_DECLARATION d1 = "";
			SINGLE_DECLARATION d2 = ";";
			SINGLE_DECLARATION d3 = "stmt";
			SINGLE_DECLARATION d4 = "test t";
			SINGLE_DECLARATION d5 = "stmt 1";
			SINGLE_DECLARATION d6 = "stmt s1 s2 s3 s4 s5";
			SINGLE_DECLARATION d7 = "Stmt s";
			SINGLE_DECLARATION d8 = "stmt s,";

			Assert::IsFalse(qv.isValidDeclaration(d1));
			Assert::IsFalse(qv.isValidDeclaration(d2));
			Assert::IsFalse(qv.isValidDeclaration(d3));
			Assert::IsFalse(qv.isValidDeclaration(d4));
			Assert::IsFalse(qv.isValidDeclaration(d5));
			Assert::IsFalse(qv.isValidDeclaration(d6));
			Assert::IsFalse(qv.isValidDeclaration(d7));
			Assert::IsFalse(qv.isValidDeclaration(d8));
		}

		TEST_METHOD(isValidClause_Valid_Success) {
			QueryValidator qv = QueryValidator();

			CLAUSES c1 = "Select s such that Follows(1, s)";
			CLAUSES c2 = "Select s such that Follows*(1, s)";
			CLAUSES c3 = "Select s such that Parent(1, s)";
			CLAUSES c4 = "Select s such that Parent*(1, s)";
			CLAUSES c5 = "Select s such that Uses(1, s)";
			CLAUSES c6 = "Select s such that Modifies(1, s)";
			CLAUSES c7 = "Select s pattern a(_,_)";
			CLAUSES c8 = "Select s";
			CLAUSES c9 = "Select s such that    Follows   (  1  ,   s   )";
			CLAUSES c10 = "Select s such that Follows(1, s) pattern a(_,_)";
			CLAUSES c11 = "Select s pattern a(_,_) such that Follows(1, s)";

			Assert::IsTrue(qv.isValidClause(c1));
			Assert::IsTrue(qv.isValidClause(c2));
			Assert::IsTrue(qv.isValidClause(c3));
			Assert::IsTrue(qv.isValidClause(c4));
			Assert::IsTrue(qv.isValidClause(c5));
			Assert::IsTrue(qv.isValidClause(c6));
			Assert::IsTrue(qv.isValidClause(c7));
			Assert::IsTrue(qv.isValidClause(c8));
			Assert::IsTrue(qv.isValidClause(c9));
			Assert::IsTrue(qv.isValidClause(c10));
			Assert::IsTrue(qv.isValidClause(c11));
		}

		TEST_METHOD(isValidClause_Invalid_Success) {
			QueryValidator qv = QueryValidator();

			CLAUSES c1 = "Select";
			CLAUSES c2 = "Select s such that Follows(1, s) such that Follows(1, s)";

			Assert::IsFalse(qv.isValidClause(c1));
			Assert::IsFalse(qv.isValidClause(c2));
		}

		TEST_METHOD(isSynonymDeclared_Valid_Success) {
			QueryValidator qv = QueryValidator();
			PROCESSED_SYNONYMS proc_s;
			SYNONYM_NAME syn = "s";

			QueryNode stmt_s = QueryNode();
			stmt_s.setSynonymNode("stmt", syn);

			proc_s.insert({ syn, stmt_s });

			Assert::IsTrue(qv.isSynonymDeclared(proc_s, syn));
		}

		TEST_METHOD(isSynonymDeclared_Invalid_Success) {
			QueryValidator qv = QueryValidator();
			PROCESSED_SYNONYMS proc_s;
			SYNONYM_NAME syn = "s1";

			QueryNode stmt_s = QueryNode();
			stmt_s.setSynonymNode("stmt", "s");

			proc_s.insert({ "s", stmt_s });

			Assert::IsFalse(qv.isSynonymDeclared(proc_s, syn));
		}

		TEST_METHOD(isValidRelationFormat_Valid_Success) {
			QueryValidator qv = QueryValidator();

			SINGLE_CLAUSE r1 = "Follows(1, s)";
			SINGLE_CLAUSE r2 = "Follows*(1, s)";
			SINGLE_CLAUSE r3 = "Parent(1, s)";
			SINGLE_CLAUSE r4 = "Parent*(1, s)";
			SINGLE_CLAUSE r5 = "Uses(1, s)";
			SINGLE_CLAUSE r6 = "Modifies(1, s)";
			SINGLE_CLAUSE r7 = "Follows  (   1   ,   s       )";
			SINGLE_CLAUSE r8 = "Follows(1,s)";

			Assert::IsTrue(qv.isValidRelationFormat(r1));
			Assert::IsTrue(qv.isValidRelationFormat(r2));
			Assert::IsTrue(qv.isValidRelationFormat(r3));
			Assert::IsTrue(qv.isValidRelationFormat(r4));
			Assert::IsTrue(qv.isValidRelationFormat(r5));
			Assert::IsTrue(qv.isValidRelationFormat(r6));
			Assert::IsTrue(qv.isValidRelationFormat(r7));
			Assert::IsTrue(qv.isValidRelationFormat(r8));
		}

		TEST_METHOD(isValidRelationFormat_Invalid_Success) {
			QueryValidator qv = QueryValidator();

			SINGLE_CLAUSE r1 = "follows(1, s)";
			SINGLE_CLAUSE r2 = "Follows *(1, s)";
			SINGLE_CLAUSE r3 = "Follows(1)";
			SINGLE_CLAUSE r4 = "Follows(1 s)";
			SINGLE_CLAUSE r5 = "Test(1, s)";
			SINGLE_CLAUSE r6 = "Follows(1, s, s)";

			Assert::IsFalse(qv.isValidRelationFormat(r1));
			Assert::IsFalse(qv.isValidRelationFormat(r2));
			Assert::IsFalse(qv.isValidRelationFormat(r3));
			Assert::IsFalse(qv.isValidRelationFormat(r4));
			Assert::IsFalse(qv.isValidRelationFormat(r5));
			Assert::IsFalse(qv.isValidRelationFormat(r6));
		}

		TEST_METHOD(isStatementArgument_Valid_Success)
		{
			QueryValidator qv = QueryValidator();
			PROCESSED_SYNONYMS proc_s;

			QueryNode stmt_s = QueryNode();
			stmt_s.setSynonymNode("stmt", "s");
			proc_s.insert({ "s", stmt_s });

			QueryNode read_r = QueryNode();
			read_r.setSynonymNode("read", "r");
			proc_s.insert({ "r", read_r });

			QueryNode print_pn = QueryNode();
			print_pn.setSynonymNode("print", "pn");
			proc_s.insert({ "pn", print_pn });

			QueryNode while_w = QueryNode();
			while_w.setSynonymNode("while", "w");
			proc_s.insert({ "w", while_w });

			QueryNode if_ifs = QueryNode();
			if_ifs.setSynonymNode("if", "ifs");
			proc_s.insert({ "ifs", if_ifs });

			QueryNode assign_a = QueryNode();
			assign_a.setSynonymNode("assign", "a");
			proc_s.insert({ "a", assign_a });

			ARGUMENT arg1 = "1";
			ARGUMENT arg2 = "_";
			ARGUMENT arg3 = "s";
			ARGUMENT arg4 = "r";
			ARGUMENT arg5 = "pn";
			ARGUMENT arg6 = "w";
			ARGUMENT arg7 = "ifs";
			ARGUMENT arg8 = "a";

			Assert::IsTrue(qv.isStatementArgument(proc_s, arg1));
			Assert::IsTrue(qv.isStatementArgument(proc_s, arg2));
			Assert::IsTrue(qv.isStatementArgument(proc_s, arg3));
			Assert::IsTrue(qv.isStatementArgument(proc_s, arg4));
			Assert::IsTrue(qv.isStatementArgument(proc_s, arg5));
			Assert::IsTrue(qv.isStatementArgument(proc_s, arg6));
			Assert::IsTrue(qv.isStatementArgument(proc_s, arg7));
			Assert::IsTrue(qv.isStatementArgument(proc_s, arg8));
		}

		TEST_METHOD(isStatementArgument_Invalid_Success)
		{
			QueryValidator qv = QueryValidator();
			PROCESSED_SYNONYMS proc_s;

			QueryNode var_v = QueryNode();
			var_v.setSynonymNode("variable", "v");
			proc_s.insert({ "v", var_v });

			QueryNode const_c = QueryNode();
			const_c.setSynonymNode("constant", "c");
			proc_s.insert({ "c", const_c });

			QueryNode procedure_p = QueryNode();
			procedure_p.setSynonymNode("procedure", "p");
			proc_s.insert({ "p", procedure_p });

			ARGUMENT arg1 = "v";
			ARGUMENT arg2 = "c";
			ARGUMENT arg3 = "p";

			Assert::IsFalse(qv.isStatementArgument(proc_s, arg1));
			Assert::IsFalse(qv.isStatementArgument(proc_s, arg2));
			Assert::IsFalse(qv.isStatementArgument(proc_s, arg3));
		}

		TEST_METHOD(isValidRelationArguments_Follows_Valid_Success)
		{
			QueryValidator qv = QueryValidator();
			PROCESSED_SYNONYMS proc_s;

			QueryNode stmt_s = QueryNode();
			stmt_s.setSynonymNode("stmt", "s");
			proc_s.insert({ "s", stmt_s });

			QueryNode read_r = QueryNode();
			read_r.setSynonymNode("read", "r");
			proc_s.insert({ "r", read_r });

			QueryNode print_pn = QueryNode();
			print_pn.setSynonymNode("print", "pn");
			proc_s.insert({ "pn", print_pn });

			QueryNode while_w = QueryNode();
			while_w.setSynonymNode("while", "w");
			proc_s.insert({ "w", while_w });

			QueryNode if_ifs = QueryNode();
			if_ifs.setSynonymNode("if", "ifs");
			proc_s.insert({ "ifs", if_ifs });

			QueryNode assign_a = QueryNode();
			assign_a.setSynonymNode("assign", "a");
			proc_s.insert({ "a", assign_a });

			RELATIONSHIP r = "Follows";

			ARGUMENT arg1_1 = "1";
			ARGUMENT arg1_2 = "2";

			ARGUMENT arg2_1 = "1";
			ARGUMENT arg2_2 = "_";

			ARGUMENT arg3_1 = "1";
			ARGUMENT arg3_2 = "s";

			ARGUMENT arg4_1 = "1";
			ARGUMENT arg4_2 = "r";

			ARGUMENT arg5_1 = "1";
			ARGUMENT arg5_2 = "pn";

			ARGUMENT arg6_1 = "1";
			ARGUMENT arg6_2 = "w";

			ARGUMENT arg7_1 = "1";
			ARGUMENT arg7_2 = "ifs";

			ARGUMENT arg8_1 = "1";
			ARGUMENT arg8_2 = "a";

			ARGUMENT arg9_1 = "s";
			ARGUMENT arg9_2 = "a";

			Assert::IsTrue(qv.isValidRelationArguments(proc_s, r, arg1_1, arg1_2));
			Assert::IsTrue(qv.isValidRelationArguments(proc_s, r, arg2_1, arg2_2));
			Assert::IsTrue(qv.isValidRelationArguments(proc_s, r, arg3_1, arg3_2));
			Assert::IsTrue(qv.isValidRelationArguments(proc_s, r, arg4_1, arg4_2));
			Assert::IsTrue(qv.isValidRelationArguments(proc_s, r, arg5_1, arg5_2));
			Assert::IsTrue(qv.isValidRelationArguments(proc_s, r, arg6_1, arg6_2));
			Assert::IsTrue(qv.isValidRelationArguments(proc_s, r, arg7_1, arg7_2));
			Assert::IsTrue(qv.isValidRelationArguments(proc_s, r, arg8_1, arg8_2));
			Assert::IsTrue(qv.isValidRelationArguments(proc_s, r, arg9_1, arg9_2));
		}

		TEST_METHOD(isValidRelationArguments_Follows_Invalid_Success)
		{
			QueryValidator qv = QueryValidator();
			PROCESSED_SYNONYMS proc_s;

			QueryNode var_v = QueryNode();
			var_v.setSynonymNode("variable", "v");
			proc_s.insert({ "v", var_v });

			QueryNode const_c = QueryNode();
			const_c.setSynonymNode("constant", "c");
			proc_s.insert({ "c", const_c });

			QueryNode procedure_p = QueryNode();
			procedure_p.setSynonymNode("procedure", "p");
			proc_s.insert({ "p", procedure_p });

			RELATIONSHIP r = "Follows";

			ARGUMENT arg1_1 = "1";
			ARGUMENT arg1_2 = "undeclared";

			ARGUMENT arg2_1 = "1";
			ARGUMENT arg2_2 = "v";

			ARGUMENT arg3_1 = "1";
			ARGUMENT arg3_2 = "c";

			ARGUMENT arg4_1 = "1";
			ARGUMENT arg4_2 = "p";

			ARGUMENT arg5_1 = "v";
			ARGUMENT arg5_2 = "1";

			ARGUMENT arg6_1 = "c";
			ARGUMENT arg6_2 = "1";

			ARGUMENT arg7_1 = "p";
			ARGUMENT arg7_2 = "1";

			Assert::IsFalse(qv.isValidRelationArguments(proc_s, r, arg1_1, arg1_2));
			Assert::IsFalse(qv.isValidRelationArguments(proc_s, r, arg2_1, arg2_2));
			Assert::IsFalse(qv.isValidRelationArguments(proc_s, r, arg3_1, arg3_2));
			Assert::IsFalse(qv.isValidRelationArguments(proc_s, r, arg4_1, arg4_2));
			Assert::IsFalse(qv.isValidRelationArguments(proc_s, r, arg5_1, arg5_2));
			Assert::IsFalse(qv.isValidRelationArguments(proc_s, r, arg6_1, arg6_2));
			Assert::IsFalse(qv.isValidRelationArguments(proc_s, r, arg7_1, arg7_2));
		}

		TEST_METHOD(isValidRelationArguments_FollowsT_Valid_Success)
		{
			QueryValidator qv = QueryValidator();
			PROCESSED_SYNONYMS proc_s;

			QueryNode stmt_s = QueryNode();
			stmt_s.setSynonymNode("stmt", "s");
			proc_s.insert({ "s", stmt_s });

			QueryNode read_r = QueryNode();
			read_r.setSynonymNode("read", "r");
			proc_s.insert({ "r", read_r });

			QueryNode print_pn = QueryNode();
			print_pn.setSynonymNode("print", "pn");
			proc_s.insert({ "pn", print_pn });

			QueryNode while_w = QueryNode();
			while_w.setSynonymNode("while", "w");
			proc_s.insert({ "w", while_w });

			QueryNode if_ifs = QueryNode();
			if_ifs.setSynonymNode("if", "ifs");
			proc_s.insert({ "ifs", if_ifs });

			QueryNode assign_a = QueryNode();
			assign_a.setSynonymNode("assign", "a");
			proc_s.insert({ "a", assign_a });

			RELATIONSHIP r = "Follows*";

			ARGUMENT arg1_1 = "1";
			ARGUMENT arg1_2 = "2";

			ARGUMENT arg2_1 = "1";
			ARGUMENT arg2_2 = "_";

			ARGUMENT arg3_1 = "1";
			ARGUMENT arg3_2 = "s";

			ARGUMENT arg4_1 = "1";
			ARGUMENT arg4_2 = "r";

			ARGUMENT arg5_1 = "1";
			ARGUMENT arg5_2 = "pn";

			ARGUMENT arg6_1 = "1";
			ARGUMENT arg6_2 = "w";

			ARGUMENT arg7_1 = "1";
			ARGUMENT arg7_2 = "ifs";

			ARGUMENT arg8_1 = "1";
			ARGUMENT arg8_2 = "a";

			ARGUMENT arg9_1 = "s";
			ARGUMENT arg9_2 = "a";

			Assert::IsTrue(qv.isValidRelationArguments(proc_s, r, arg1_1, arg1_2));
			Assert::IsTrue(qv.isValidRelationArguments(proc_s, r, arg2_1, arg2_2));
			Assert::IsTrue(qv.isValidRelationArguments(proc_s, r, arg3_1, arg3_2));
			Assert::IsTrue(qv.isValidRelationArguments(proc_s, r, arg4_1, arg4_2));
			Assert::IsTrue(qv.isValidRelationArguments(proc_s, r, arg5_1, arg5_2));
			Assert::IsTrue(qv.isValidRelationArguments(proc_s, r, arg6_1, arg6_2));
			Assert::IsTrue(qv.isValidRelationArguments(proc_s, r, arg7_1, arg7_2));
			Assert::IsTrue(qv.isValidRelationArguments(proc_s, r, arg8_1, arg8_2));
			Assert::IsTrue(qv.isValidRelationArguments(proc_s, r, arg9_1, arg9_2));
		}

		TEST_METHOD(isValidRelationArguments_FollowsT_Invalid_Success)
		{
			QueryValidator qv = QueryValidator();
			PROCESSED_SYNONYMS proc_s;

			QueryNode var_v = QueryNode();
			var_v.setSynonymNode("variable", "v");
			proc_s.insert({ "v", var_v });

			QueryNode const_c = QueryNode();
			const_c.setSynonymNode("constant", "c");
			proc_s.insert({ "c", const_c });

			QueryNode procedure_p = QueryNode();
			procedure_p.setSynonymNode("procedure", "p");
			proc_s.insert({ "p", procedure_p });

			RELATIONSHIP r = "FollowsT";

			ARGUMENT arg1_1 = "1";
			ARGUMENT arg1_2 = "undeclared";

			ARGUMENT arg2_1 = "1";
			ARGUMENT arg2_2 = "v";

			ARGUMENT arg3_1 = "1";
			ARGUMENT arg3_2 = "c";

			ARGUMENT arg4_1 = "1";
			ARGUMENT arg4_2 = "p";

			ARGUMENT arg5_1 = "v";
			ARGUMENT arg5_2 = "1";

			ARGUMENT arg6_1 = "c";
			ARGUMENT arg6_2 = "1";

			ARGUMENT arg7_1 = "p";
			ARGUMENT arg7_2 = "1";

			Assert::IsFalse(qv.isValidRelationArguments(proc_s, r, arg1_1, arg1_2));
			Assert::IsFalse(qv.isValidRelationArguments(proc_s, r, arg2_1, arg2_2));
			Assert::IsFalse(qv.isValidRelationArguments(proc_s, r, arg3_1, arg3_2));
			Assert::IsFalse(qv.isValidRelationArguments(proc_s, r, arg4_1, arg4_2));
			Assert::IsFalse(qv.isValidRelationArguments(proc_s, r, arg5_1, arg5_2));
			Assert::IsFalse(qv.isValidRelationArguments(proc_s, r, arg6_1, arg6_2));
			Assert::IsFalse(qv.isValidRelationArguments(proc_s, r, arg7_1, arg7_2));
		}

		TEST_METHOD(isValidRelationArguments_Parent_Valid_Success)
		{
			QueryValidator qv = QueryValidator();
			PROCESSED_SYNONYMS proc_s;

			QueryNode stmt_s = QueryNode();
			stmt_s.setSynonymNode("stmt", "s");
			proc_s.insert({ "s", stmt_s });

			QueryNode read_r = QueryNode();
			read_r.setSynonymNode("read", "r");
			proc_s.insert({ "r", read_r });

			QueryNode print_pn = QueryNode();
			print_pn.setSynonymNode("print", "pn");
			proc_s.insert({ "pn", print_pn });

			QueryNode while_w = QueryNode();
			while_w.setSynonymNode("while", "w");
			proc_s.insert({ "w", while_w });

			QueryNode if_ifs = QueryNode();
			if_ifs.setSynonymNode("if", "ifs");
			proc_s.insert({ "ifs", if_ifs });

			QueryNode assign_a = QueryNode();
			assign_a.setSynonymNode("assign", "a");
			proc_s.insert({ "a", assign_a });

			RELATIONSHIP r = "Parent";

			ARGUMENT arg1_1 = "1";
			ARGUMENT arg1_2 = "2";

			ARGUMENT arg2_1 = "1";
			ARGUMENT arg2_2 = "_";

			ARGUMENT arg3_1 = "1";
			ARGUMENT arg3_2 = "s";

			ARGUMENT arg4_1 = "1";
			ARGUMENT arg4_2 = "r";

			ARGUMENT arg5_1 = "1";
			ARGUMENT arg5_2 = "pn";

			ARGUMENT arg6_1 = "1";
			ARGUMENT arg6_2 = "w";

			ARGUMENT arg7_1 = "1";
			ARGUMENT arg7_2 = "ifs";

			ARGUMENT arg8_1 = "1";
			ARGUMENT arg8_2 = "a";

			ARGUMENT arg9_1 = "s";
			ARGUMENT arg9_2 = "a";

			Assert::IsTrue(qv.isValidRelationArguments(proc_s, r, arg1_1, arg1_2));
			Assert::IsTrue(qv.isValidRelationArguments(proc_s, r, arg2_1, arg2_2));
			Assert::IsTrue(qv.isValidRelationArguments(proc_s, r, arg3_1, arg3_2));
			Assert::IsTrue(qv.isValidRelationArguments(proc_s, r, arg4_1, arg4_2));
			Assert::IsTrue(qv.isValidRelationArguments(proc_s, r, arg5_1, arg5_2));
			Assert::IsTrue(qv.isValidRelationArguments(proc_s, r, arg6_1, arg6_2));
			Assert::IsTrue(qv.isValidRelationArguments(proc_s, r, arg7_1, arg7_2));
			Assert::IsTrue(qv.isValidRelationArguments(proc_s, r, arg8_1, arg8_2));
			Assert::IsTrue(qv.isValidRelationArguments(proc_s, r, arg9_1, arg9_2));
		}

		TEST_METHOD(isValidRelationArguments_Parent_Invalid_Success)
		{
			QueryValidator qv = QueryValidator();
			PROCESSED_SYNONYMS proc_s;

			QueryNode var_v = QueryNode();
			var_v.setSynonymNode("variable", "v");
			proc_s.insert({ "v", var_v });

			QueryNode const_c = QueryNode();
			const_c.setSynonymNode("constant", "c");
			proc_s.insert({ "c", const_c });

			QueryNode procedure_p = QueryNode();
			procedure_p.setSynonymNode("procedure", "p");
			proc_s.insert({ "p", procedure_p });

			RELATIONSHIP r = "Parent";

			ARGUMENT arg1_1 = "1";
			ARGUMENT arg1_2 = "undeclared";

			ARGUMENT arg2_1 = "1";
			ARGUMENT arg2_2 = "v";

			ARGUMENT arg3_1 = "1";
			ARGUMENT arg3_2 = "c";

			ARGUMENT arg4_1 = "1";
			ARGUMENT arg4_2 = "p";

			ARGUMENT arg5_1 = "v";
			ARGUMENT arg5_2 = "1";

			ARGUMENT arg6_1 = "c";
			ARGUMENT arg6_2 = "1";

			ARGUMENT arg7_1 = "p";
			ARGUMENT arg7_2 = "1";

			Assert::IsFalse(qv.isValidRelationArguments(proc_s, r, arg1_1, arg1_2));
			Assert::IsFalse(qv.isValidRelationArguments(proc_s, r, arg2_1, arg2_2));
			Assert::IsFalse(qv.isValidRelationArguments(proc_s, r, arg3_1, arg3_2));
			Assert::IsFalse(qv.isValidRelationArguments(proc_s, r, arg4_1, arg4_2));
			Assert::IsFalse(qv.isValidRelationArguments(proc_s, r, arg5_1, arg5_2));
			Assert::IsFalse(qv.isValidRelationArguments(proc_s, r, arg6_1, arg6_2));
			Assert::IsFalse(qv.isValidRelationArguments(proc_s, r, arg7_1, arg7_2));
		}

		TEST_METHOD(isValidRelationArguments_ParentT_Valid_Success)
		{
			QueryValidator qv = QueryValidator();
			PROCESSED_SYNONYMS proc_s;

			QueryNode stmt_s = QueryNode();
			stmt_s.setSynonymNode("stmt", "s");
			proc_s.insert({ "s", stmt_s });

			QueryNode read_r = QueryNode();
			read_r.setSynonymNode("read", "r");
			proc_s.insert({ "r", read_r });

			QueryNode print_pn = QueryNode();
			print_pn.setSynonymNode("print", "pn");
			proc_s.insert({ "pn", print_pn });

			QueryNode while_w = QueryNode();
			while_w.setSynonymNode("while", "w");
			proc_s.insert({ "w", while_w });

			QueryNode if_ifs = QueryNode();
			if_ifs.setSynonymNode("if", "ifs");
			proc_s.insert({ "ifs", if_ifs });

			QueryNode assign_a = QueryNode();
			assign_a.setSynonymNode("assign", "a");
			proc_s.insert({ "a", assign_a });

			RELATIONSHIP r = "Parent*";

			ARGUMENT arg1_1 = "1";
			ARGUMENT arg1_2 = "2";

			ARGUMENT arg2_1 = "1";
			ARGUMENT arg2_2 = "_";

			ARGUMENT arg3_1 = "1";
			ARGUMENT arg3_2 = "s";

			ARGUMENT arg4_1 = "1";
			ARGUMENT arg4_2 = "r";

			ARGUMENT arg5_1 = "1";
			ARGUMENT arg5_2 = "pn";

			ARGUMENT arg6_1 = "1";
			ARGUMENT arg6_2 = "w";

			ARGUMENT arg7_1 = "1";
			ARGUMENT arg7_2 = "ifs";

			ARGUMENT arg8_1 = "1";
			ARGUMENT arg8_2 = "a";

			ARGUMENT arg9_1 = "s";
			ARGUMENT arg9_2 = "a";

			Assert::IsTrue(qv.isValidRelationArguments(proc_s, r, arg1_1, arg1_2));
			Assert::IsTrue(qv.isValidRelationArguments(proc_s, r, arg2_1, arg2_2));
			Assert::IsTrue(qv.isValidRelationArguments(proc_s, r, arg3_1, arg3_2));
			Assert::IsTrue(qv.isValidRelationArguments(proc_s, r, arg4_1, arg4_2));
			Assert::IsTrue(qv.isValidRelationArguments(proc_s, r, arg5_1, arg5_2));
			Assert::IsTrue(qv.isValidRelationArguments(proc_s, r, arg6_1, arg6_2));
			Assert::IsTrue(qv.isValidRelationArguments(proc_s, r, arg7_1, arg7_2));
			Assert::IsTrue(qv.isValidRelationArguments(proc_s, r, arg8_1, arg8_2));
			Assert::IsTrue(qv.isValidRelationArguments(proc_s, r, arg9_1, arg9_2));
		}

		TEST_METHOD(isValidRelationArguments_ParentT_Invalid_Success)
		{
			QueryValidator qv = QueryValidator();
			PROCESSED_SYNONYMS proc_s;

			QueryNode var_v = QueryNode();
			var_v.setSynonymNode("variable", "v");
			proc_s.insert({ "v", var_v });

			QueryNode const_c = QueryNode();
			const_c.setSynonymNode("constant", "c");
			proc_s.insert({ "c", const_c });

			QueryNode procedure_p = QueryNode();
			procedure_p.setSynonymNode("procedure", "p");
			proc_s.insert({ "p", procedure_p });

			RELATIONSHIP r = "ParentT";

			ARGUMENT arg1_1 = "1";
			ARGUMENT arg1_2 = "undeclared";

			ARGUMENT arg2_1 = "1";
			ARGUMENT arg2_2 = "v";

			ARGUMENT arg3_1 = "1";
			ARGUMENT arg3_2 = "c";

			ARGUMENT arg4_1 = "1";
			ARGUMENT arg4_2 = "p";

			ARGUMENT arg5_1 = "v";
			ARGUMENT arg5_2 = "1";

			ARGUMENT arg6_1 = "c";
			ARGUMENT arg6_2 = "1";

			ARGUMENT arg7_1 = "p";
			ARGUMENT arg7_2 = "1";

			Assert::IsFalse(qv.isValidRelationArguments(proc_s, r, arg1_1, arg1_2));
			Assert::IsFalse(qv.isValidRelationArguments(proc_s, r, arg2_1, arg2_2));
			Assert::IsFalse(qv.isValidRelationArguments(proc_s, r, arg3_1, arg3_2));
			Assert::IsFalse(qv.isValidRelationArguments(proc_s, r, arg4_1, arg4_2));
			Assert::IsFalse(qv.isValidRelationArguments(proc_s, r, arg5_1, arg5_2));
			Assert::IsFalse(qv.isValidRelationArguments(proc_s, r, arg6_1, arg6_2));
			Assert::IsFalse(qv.isValidRelationArguments(proc_s, r, arg7_1, arg7_2));
		}

		TEST_METHOD(isValidRelationArguments_Uses_Valid_Success)
		{
			QueryValidator qv = QueryValidator();
			PROCESSED_SYNONYMS proc_s;

			QueryNode var_v = QueryNode();
			var_v.setSynonymNode("variable", "v");
			proc_s.insert({ "v", var_v });

			QueryNode print_pn = QueryNode();
			print_pn.setSynonymNode("print", "pn");
			proc_s.insert({ "pn", print_pn });

			QueryNode while_w = QueryNode();
			while_w.setSynonymNode("while", "w");
			proc_s.insert({ "w", while_w });

			QueryNode if_ifs = QueryNode();
			if_ifs.setSynonymNode("if", "ifs");
			proc_s.insert({ "ifs", if_ifs });

			QueryNode assign_a = QueryNode();
			assign_a.setSynonymNode("assign", "a");
			proc_s.insert({ "a", assign_a });

			QueryNode procedure_p = QueryNode();
			procedure_p.setSynonymNode("procedure", "p");
			proc_s.insert({ "p", procedure_p });

			QueryNode stmt_s = QueryNode();
			stmt_s.setSynonymNode("stmt", "s");
			proc_s.insert({ "s", stmt_s });

			RELATIONSHIP r = "Uses";

			ARGUMENT arg1_1 = "a";
			ARGUMENT arg1_2 = "v";

			ARGUMENT arg2_1 = "p";
			ARGUMENT arg2_2 = "v";

			ARGUMENT arg3_1 = "ifs";
			ARGUMENT arg3_2 = "v";

			ARGUMENT arg4_1 = "w";
			ARGUMENT arg4_2 = "v";

			ARGUMENT arg5_1 = "p";
			ARGUMENT arg5_2 = "v";

			ARGUMENT arg6_1 = "1";
			ARGUMENT arg6_2 = "v";

			ARGUMENT arg7_1 = "a";
			ARGUMENT arg7_2 = "_";

			ARGUMENT arg8_1 = "a";
			ARGUMENT arg8_2 = "\"identity\"";

			ARGUMENT arg9_1 = "s";
			ARGUMENT arg9_2 = "v";

			Assert::IsTrue(qv.isValidRelationArguments(proc_s, r, arg1_1, arg1_2));
			Assert::IsTrue(qv.isValidRelationArguments(proc_s, r, arg2_1, arg2_2));
			Assert::IsTrue(qv.isValidRelationArguments(proc_s, r, arg3_1, arg3_2));
			Assert::IsTrue(qv.isValidRelationArguments(proc_s, r, arg4_1, arg4_2));
			Assert::IsTrue(qv.isValidRelationArguments(proc_s, r, arg5_1, arg5_2));
			Assert::IsTrue(qv.isValidRelationArguments(proc_s, r, arg6_1, arg6_2));
			Assert::IsTrue(qv.isValidRelationArguments(proc_s, r, arg7_1, arg7_2));
			Assert::IsTrue(qv.isValidRelationArguments(proc_s, r, arg8_1, arg8_2));
			Assert::IsTrue(qv.isValidRelationArguments(proc_s, r, arg9_1, arg9_2));
		}

		TEST_METHOD(isValidRelationArguments_Uses_Invalid_Success)
		{
			QueryValidator qv = QueryValidator();
			PROCESSED_SYNONYMS proc_s;

			QueryNode var_v = QueryNode();
			var_v.setSynonymNode("variable", "v");
			proc_s.insert({ "v", var_v });

			QueryNode read_r = QueryNode();
			read_r.setSynonymNode("read", "r");
			proc_s.insert({ "r", read_r });

			QueryNode while_w = QueryNode();
			while_w.setSynonymNode("while", "w");
			proc_s.insert({ "w", while_w });

			QueryNode assign_a = QueryNode();
			assign_a.setSynonymNode("assign", "a");
			proc_s.insert({ "a", assign_a });

			RELATIONSHIP r = "Uses";

			ARGUMENT arg1_1 = "_";
			ARGUMENT arg1_2 = "v";

			ARGUMENT arg2_1 = "r";
			ARGUMENT arg2_2 = "v";

			ARGUMENT arg3_1 = "a";
			ARGUMENT arg3_2 = "1";

			ARGUMENT arg4_1 = "undeclared";
			ARGUMENT arg4_2 = "v";

			ARGUMENT arg5_1 = "\"identity\"";
			ARGUMENT arg5_2 = "v";

			ARGUMENT arg6_1 = "a";
			ARGUMENT arg6_2 = "w";

			Assert::IsFalse(qv.isValidRelationArguments(proc_s, r, arg1_1, arg1_2));
			Assert::IsFalse(qv.isValidRelationArguments(proc_s, r, arg2_1, arg2_2));
			Assert::IsFalse(qv.isValidRelationArguments(proc_s, r, arg3_1, arg3_2));
			Assert::IsFalse(qv.isValidRelationArguments(proc_s, r, arg4_1, arg4_2));
			Assert::IsFalse(qv.isValidRelationArguments(proc_s, r, arg5_1, arg5_2));
			Assert::IsFalse(qv.isValidRelationArguments(proc_s, r, arg6_1, arg6_2));
		}

		TEST_METHOD(isValidRelationArguments_Modifies_Valid_Success)
		{
			QueryValidator qv = QueryValidator();
			PROCESSED_SYNONYMS proc_s;

			QueryNode var_v = QueryNode();
			var_v.setSynonymNode("variable", "v");
			proc_s.insert({ "v", var_v });

			QueryNode read_r = QueryNode();
			read_r.setSynonymNode("read", "r");
			proc_s.insert({ "r", read_r });

			QueryNode while_w = QueryNode();
			while_w.setSynonymNode("while", "w");
			proc_s.insert({ "w", while_w });

			QueryNode if_ifs = QueryNode();
			if_ifs.setSynonymNode("if", "ifs");
			proc_s.insert({ "ifs", if_ifs });

			QueryNode assign_a = QueryNode();
			assign_a.setSynonymNode("assign", "a");
			proc_s.insert({ "a", assign_a });

			QueryNode procedure_p = QueryNode();
			procedure_p.setSynonymNode("procedure", "p");
			proc_s.insert({ "p", procedure_p });

			QueryNode stmt_s = QueryNode();
			stmt_s.setSynonymNode("stmt", "s");
			proc_s.insert({ "s", stmt_s });

			RELATIONSHIP r = "Modifies";

			ARGUMENT arg1_1 = "a";
			ARGUMENT arg1_2 = "v";

			ARGUMENT arg2_1 = "r";
			ARGUMENT arg2_2 = "v";

			ARGUMENT arg3_1 = "ifs";
			ARGUMENT arg3_2 = "v";

			ARGUMENT arg4_1 = "w";
			ARGUMENT arg4_2 = "v";

			ARGUMENT arg5_1 = "p";
			ARGUMENT arg5_2 = "v";

			ARGUMENT arg6_1 = "1";
			ARGUMENT arg6_2 = "v";

			ARGUMENT arg7_1 = "a";
			ARGUMENT arg7_2 = "_";

			ARGUMENT arg8_1 = "a";
			ARGUMENT arg8_2 = "\"identity\"";

			ARGUMENT arg9_1 = "s";
			ARGUMENT arg9_2 = "v";

			Assert::IsTrue(qv.isValidRelationArguments(proc_s, r, arg1_1, arg1_2));
			Assert::IsTrue(qv.isValidRelationArguments(proc_s, r, arg2_1, arg2_2));
			Assert::IsTrue(qv.isValidRelationArguments(proc_s, r, arg3_1, arg3_2));
			Assert::IsTrue(qv.isValidRelationArguments(proc_s, r, arg4_1, arg4_2));
			Assert::IsTrue(qv.isValidRelationArguments(proc_s, r, arg5_1, arg5_2));
			Assert::IsTrue(qv.isValidRelationArguments(proc_s, r, arg6_1, arg6_2));
			Assert::IsTrue(qv.isValidRelationArguments(proc_s, r, arg7_1, arg7_2));
			Assert::IsTrue(qv.isValidRelationArguments(proc_s, r, arg8_1, arg8_2));
			Assert::IsTrue(qv.isValidRelationArguments(proc_s, r, arg9_1, arg9_2));
		}

		TEST_METHOD(isValidRelationArguments_Modifies_Invalid_Success)
		{
			QueryValidator qv = QueryValidator();
			PROCESSED_SYNONYMS proc_s;

			QueryNode var_v = QueryNode();
			var_v.setSynonymNode("variable", "v");
			proc_s.insert({ "v", var_v });

			QueryNode print_pn = QueryNode();
			print_pn.setSynonymNode("print", "pn");
			proc_s.insert({ "pn", print_pn });

			QueryNode while_w = QueryNode();
			while_w.setSynonymNode("while", "w");
			proc_s.insert({ "w", while_w });

			QueryNode assign_a = QueryNode();
			assign_a.setSynonymNode("assign", "a");
			proc_s.insert({ "a", assign_a });

			RELATIONSHIP r = "Modifies";

			ARGUMENT arg1_1 = "_";
			ARGUMENT arg1_2 = "v";

			ARGUMENT arg2_1 = "pn";
			ARGUMENT arg2_2 = "v";

			ARGUMENT arg3_1 = "a";
			ARGUMENT arg3_2 = "1";

			ARGUMENT arg4_1 = "undeclared";
			ARGUMENT arg4_2 = "v";

			ARGUMENT arg5_1 = "\"identity\"";
			ARGUMENT arg5_2 = "v";

			ARGUMENT arg6_1 = "a";
			ARGUMENT arg6_2 = "w";

			Assert::IsFalse(qv.isValidRelationArguments(proc_s, r, arg1_1, arg1_2));
			Assert::IsFalse(qv.isValidRelationArguments(proc_s, r, arg2_1, arg2_2));
			Assert::IsFalse(qv.isValidRelationArguments(proc_s, r, arg3_1, arg3_2));
			Assert::IsFalse(qv.isValidRelationArguments(proc_s, r, arg4_1, arg4_2));
			Assert::IsFalse(qv.isValidRelationArguments(proc_s, r, arg5_1, arg5_2));
			Assert::IsFalse(qv.isValidRelationArguments(proc_s, r, arg6_1, arg6_2));
		}

		TEST_METHOD(isValidPatternFormat_Valid_Success)
		{
			QueryValidator qv = QueryValidator();

			SINGLE_CLAUSE p1 = "pattern a(_, _)";
			SINGLE_CLAUSE p2 = "pattern a(a, _)";
			SINGLE_CLAUSE p3 = "pattern a(\"identity\", _)";
			SINGLE_CLAUSE p4 = "pattern a(_, _\"x\"_)";
			SINGLE_CLAUSE p5 = "pattern a(_, _\"1\"_)";
			SINGLE_CLAUSE p6 = "pattern a ( _    , _   )";

			Assert::IsTrue(qv.isValidPatternFormat(p1));
			Assert::IsTrue(qv.isValidPatternFormat(p2));
			Assert::IsTrue(qv.isValidPatternFormat(p3));
			Assert::IsTrue(qv.isValidPatternFormat(p4));
			Assert::IsTrue(qv.isValidPatternFormat(p5));
			Assert::IsTrue(qv.isValidPatternFormat(p6));
		}

		TEST_METHOD(isValidPatternFormat_Invalid_Success)
		{
			QueryValidator qv = QueryValidator();

			SINGLE_CLAUSE p1 = "pattern (_, _)";
			SINGLE_CLAUSE p2 = "pattern 1(_, _)";
			SINGLE_CLAUSE p3 = "pattern a(1, _)";
			SINGLE_CLAUSE p4 = "pattern a(_, 1)";
			SINGLE_CLAUSE p5 = "pattern a(_, v)";
			SINGLE_CLAUSE p6 = "pattern a(_, _\"x\")";
			SINGLE_CLAUSE p7 = "pattern a(_, \"x\"_)";
			SINGLE_CLAUSE p8 = "pattern a(_\"x\"_, _)";

			Assert::IsFalse(qv.isValidPatternFormat(p1));
			Assert::IsFalse(qv.isValidPatternFormat(p2));
			Assert::IsFalse(qv.isValidPatternFormat(p3));
			Assert::IsFalse(qv.isValidPatternFormat(p4));
			Assert::IsFalse(qv.isValidPatternFormat(p5));
			Assert::IsFalse(qv.isValidPatternFormat(p6));
			Assert::IsFalse(qv.isValidPatternFormat(p7));
			Assert::IsFalse(qv.isValidPatternFormat(p8));
		}

		TEST_METHOD(isValidPatternArguments_Valid_Success)
		{
			QueryValidator qv = QueryValidator();
			PROCESSED_SYNONYMS proc_s;

			QueryNode stmt_s = QueryNode();
			stmt_s.setSynonymNode("stmt", "s");
			proc_s.insert({ "s", stmt_s });

			QueryNode var_v = QueryNode();
			var_v.setSynonymNode("variable", "v");
			proc_s.insert({ "v", var_v });


			QueryNode assign_a = QueryNode();
			assign_a.setSynonymNode("assign", "a");
			proc_s.insert({ "a", assign_a });

			SYNONYM_NAME syn = "a";

			ARGUMENT arg1_1 = "_";
			ARGUMENT arg1_2 = "_";

			ARGUMENT arg2_1 = "v";
			ARGUMENT arg2_2 = "_";

			ARGUMENT arg3_1 = "\"identity\"";
			ARGUMENT arg3_2 = "_";

			ARGUMENT arg4_1 = "_";
			ARGUMENT arg4_2 = "_\"x\"_";

			ARGUMENT arg5_1 = "_";
			ARGUMENT arg5_2 = "_\"1\"_";

			Assert::IsTrue(qv.isValidPatternArguments(proc_s, syn, arg1_1, arg1_2));
			Assert::IsTrue(qv.isValidPatternArguments(proc_s, syn, arg2_1, arg2_2));
			Assert::IsTrue(qv.isValidPatternArguments(proc_s, syn, arg3_1, arg3_2));
			Assert::IsTrue(qv.isValidPatternArguments(proc_s, syn, arg4_1, arg4_2));
			Assert::IsTrue(qv.isValidPatternArguments(proc_s, syn, arg5_1, arg5_2));
		}

		TEST_METHOD(isValidPatternArguments_Invalid_Success)
		{
			QueryValidator qv = QueryValidator();
			PROCESSED_SYNONYMS proc_s;

			QueryNode stmt_s = QueryNode();
			stmt_s.setSynonymNode("stmt", "s");
			proc_s.insert({ "s", stmt_s });

			QueryNode var_v = QueryNode();
			var_v.setSynonymNode("variable", "v");
			proc_s.insert({ "v", var_v });


			QueryNode assign_a = QueryNode();
			assign_a.setSynonymNode("assign", "a");
			proc_s.insert({ "a", assign_a });

			SYNONYM_NAME syn1 = "a";
			SYNONYM_NAME syn2 = "a1";
			SYNONYM_NAME syn3 = "a1";

			ARGUMENT arg1_1 = "_";
			ARGUMENT arg1_2 = "_";

			ARGUMENT arg2_1 = "1";
			ARGUMENT arg2_2 = "_";

			ARGUMENT arg3_1 = "_";
			ARGUMENT arg3_2 = "1";

			ARGUMENT arg4_1 = "_";
			ARGUMENT arg4_2 = "\"x\"_";

			ARGUMENT arg5_1 = "_";
			ARGUMENT arg5_2 = "_\"x\"";

			ARGUMENT arg6_1 = "_";
			ARGUMENT arg6_2 = "v";

			ARGUMENT arg7_1 = "_\"x\"_";
			ARGUMENT arg7_2 = "_";

			Assert::IsFalse(qv.isValidPatternArguments(proc_s, syn2, arg1_1, arg1_2));
			Assert::IsFalse(qv.isValidPatternArguments(proc_s, syn3, arg1_1, arg1_2));
			Assert::IsFalse(qv.isValidPatternArguments(proc_s, syn1, arg2_1, arg2_2));
			Assert::IsFalse(qv.isValidPatternArguments(proc_s, syn1, arg3_1, arg3_2));
			Assert::IsFalse(qv.isValidPatternArguments(proc_s, syn1, arg4_1, arg4_2));
			Assert::IsFalse(qv.isValidPatternArguments(proc_s, syn1, arg5_1, arg5_2));
			Assert::IsFalse(qv.isValidPatternArguments(proc_s, syn1, arg6_1, arg6_2));
			Assert::IsFalse(qv.isValidPatternArguments(proc_s, syn1, arg7_1, arg7_2));
		}
	};
}