#include <string>

#include "stdafx.h"
#include "CppUnitTest.h"
#include "../source/QP/QueryValidator.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{
	TEST_CLASS(TestQueryValidator)
	{
	public:

		TEST_METHOD(isValidStructure_Valid_Success) {
			INPUT_QUERY q1 = "stmt s; Select s";
			INPUT_QUERY q2 = " stmt s; Select s";
			INPUT_QUERY q3 = "stmt s; Select s ";
			INPUT_QUERY q4 = " stmt s; Select s ";
			INPUT_QUERY q5 = "  stmt s; Select s  ";
			INPUT_QUERY q6 = "stmt s; Select s such that Follows(1, s)";
			INPUT_QUERY q7 = "stmt s; Select s pattern a(_, _)";
			INPUT_QUERY q8 = "stmt s; Select s such that Follows(1, s) pattern a(_, _)";
			INPUT_QUERY q9 = "stmt s; Select s pattern a(_, _) such that Follows(1, s)";
			INPUT_QUERY q10 = "Select BOOLEAN";

			Assert::IsTrue(QueryValidator::isValidStructure(q1));
			Assert::IsTrue(QueryValidator::isValidStructure(q2));
			Assert::IsTrue(QueryValidator::isValidStructure(q3));
			Assert::IsTrue(QueryValidator::isValidStructure(q4));
			Assert::IsTrue(QueryValidator::isValidStructure(q5));
			Assert::IsTrue(QueryValidator::isValidStructure(q6));
			Assert::IsTrue(QueryValidator::isValidStructure(q7));
			Assert::IsTrue(QueryValidator::isValidStructure(q8));
			Assert::IsTrue(QueryValidator::isValidStructure(q9));
			Assert::IsTrue(QueryValidator::isValidStructure(q10));
		}

		TEST_METHOD(isValidStructure_Invalid_Success) {
			INPUT_QUERY q1 = "";
			INPUT_QUERY q2 = "Select s";
			INPUT_QUERY q3 = "stmt s;";
			INPUT_QUERY q4 = "stmt s Select s";
			INPUT_QUERY q5 = "stmt s; select s";

			Assert::IsFalse(QueryValidator::isValidStructure(q1));
			Assert::IsFalse(QueryValidator::isValidStructure(q2));
			Assert::IsFalse(QueryValidator::isValidStructure(q3));
			Assert::IsFalse(QueryValidator::isValidStructure(q4));
			Assert::IsFalse(QueryValidator::isValidStructure(q5));
		}

		TEST_METHOD(isValidDeclaration_Valid_Success) {
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

			Assert::IsTrue(QueryValidator::isValidDeclaration(d1));
			Assert::IsTrue(QueryValidator::isValidDeclaration(d2));
			Assert::IsTrue(QueryValidator::isValidDeclaration(d3));
			Assert::IsTrue(QueryValidator::isValidDeclaration(d4));
			Assert::IsTrue(QueryValidator::isValidDeclaration(d5));
			Assert::IsTrue(QueryValidator::isValidDeclaration(d6));
			Assert::IsTrue(QueryValidator::isValidDeclaration(d7));
			Assert::IsTrue(QueryValidator::isValidDeclaration(d8));
			Assert::IsTrue(QueryValidator::isValidDeclaration(d9));
			Assert::IsTrue(QueryValidator::isValidDeclaration(d10));
			Assert::IsTrue(QueryValidator::isValidDeclaration(d11));
			Assert::IsTrue(QueryValidator::isValidDeclaration(d12));
		}

		TEST_METHOD(isValidDeclaration_Invalid_Success) {
			SINGLE_DECLARATION d1 = "";
			SINGLE_DECLARATION d2 = ";";
			SINGLE_DECLARATION d3 = "stmt";
			SINGLE_DECLARATION d4 = "test t";
			SINGLE_DECLARATION d5 = "stmt 1";
			SINGLE_DECLARATION d6 = "stmt s1 s2 s3 s4 s5";
			SINGLE_DECLARATION d7 = "Stmt s";
			SINGLE_DECLARATION d8 = "stmt s,";

			Assert::IsFalse(QueryValidator::isValidDeclaration(d1));
			Assert::IsFalse(QueryValidator::isValidDeclaration(d2));
			Assert::IsFalse(QueryValidator::isValidDeclaration(d3));
			Assert::IsFalse(QueryValidator::isValidDeclaration(d4));
			Assert::IsFalse(QueryValidator::isValidDeclaration(d5));
			Assert::IsFalse(QueryValidator::isValidDeclaration(d6));
			Assert::IsFalse(QueryValidator::isValidDeclaration(d7));
			Assert::IsFalse(QueryValidator::isValidDeclaration(d8));
		}

		TEST_METHOD(isValidClause_Valid_Success) {
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

			Assert::IsTrue(QueryValidator::isValidClause(c1));
			Assert::IsTrue(QueryValidator::isValidClause(c2));
			Assert::IsTrue(QueryValidator::isValidClause(c3));
			Assert::IsTrue(QueryValidator::isValidClause(c4));
			Assert::IsTrue(QueryValidator::isValidClause(c5));
			Assert::IsTrue(QueryValidator::isValidClause(c6));
			Assert::IsTrue(QueryValidator::isValidClause(c7));
			Assert::IsTrue(QueryValidator::isValidClause(c8));
			Assert::IsTrue(QueryValidator::isValidClause(c9));
			Assert::IsTrue(QueryValidator::isValidClause(c10));
			Assert::IsTrue(QueryValidator::isValidClause(c11));
		}

		TEST_METHOD(isValidClause_Invalid_Success) {
			CLAUSES c1 = "Select";

			Assert::IsFalse(QueryValidator::isValidClause(c1));
		}

		TEST_METHOD(isSynonymDeclared_Valid_Success) {
			PROCESSED_SYNONYMS proc_s;
			SYNONYM_NAME syn = "s";

			QueryNode stmt_s = QueryNode();
			stmt_s.setSynonymNode("stmt", syn);

			proc_s.insert({ syn, stmt_s });

			Assert::IsTrue(QueryValidator::isSynonymDeclared(proc_s, syn));
		}

		TEST_METHOD(isSynonymDeclared_Invalid_Success) {
			PROCESSED_SYNONYMS proc_s;
			SYNONYM_NAME syn = "s1";

			QueryNode stmt_s = QueryNode();
			stmt_s.setSynonymNode("stmt", "s");

			proc_s.insert({ "s", stmt_s });

			Assert::IsFalse(QueryValidator::isSynonymDeclared(proc_s, syn));
		}

		TEST_METHOD(isValidAttr_Valid_Success) {
			PROCESSED_SYNONYMS proc_s;

			QueryNode stmt_s = QueryNode();
			stmt_s.setSynonymNode("stmt", "s");
			proc_s.insert({ "s", stmt_s });

			QueryNode procedure_p = QueryNode();
			procedure_p.setSynonymNode("procedure", "p");
			proc_s.insert({ "p", procedure_p });

			QueryNode variable_v = QueryNode();
			variable_v.setSynonymNode("variable", "v");
			proc_s.insert({ "v", variable_v });

			QueryNode constant_c = QueryNode();
			constant_c.setSynonymNode("constant", "c");
			proc_s.insert({ "c", constant_c });

			QueryNode call_cl = QueryNode();
			call_cl.setSynonymNode("call", "cl");
			proc_s.insert({ "cl", call_cl });

			QueryNode read_r = QueryNode();
			read_r.setSynonymNode("read", "r");
			proc_s.insert({ "r", read_r });

			QueryNode print_pr = QueryNode();
			print_pr.setSynonymNode("print", "pr");
			proc_s.insert({ "pr", print_pr });

			QueryNode while_w = QueryNode();
			while_w.setSynonymNode("while", "w");
			proc_s.insert({ "w", while_w });

			QueryNode if_ifs = QueryNode();
			if_ifs.setSynonymNode("if", "ifs");
			proc_s.insert({ "ifs", if_ifs });

			QueryNode assign_a = QueryNode();
			assign_a.setSynonymNode("assign", "a");
			proc_s.insert({ "a", assign_a });

			SINGLE_ARGUMENT a1 = "p.procName";
			SINGLE_ARGUMENT a2 = "cl.procName";

			SINGLE_ARGUMENT a3 = "v.varName";
			SINGLE_ARGUMENT a4 = "r.varName";
			SINGLE_ARGUMENT a5 = "pr.varName";

			SINGLE_ARGUMENT a6 = "c.value";

			SINGLE_ARGUMENT a7 = "s.stmt#";
			SINGLE_ARGUMENT a8 = "r.stmt#";
			SINGLE_ARGUMENT a9 = "pr.stmt#";
			SINGLE_ARGUMENT a10 = "cl.stmt#";
			SINGLE_ARGUMENT a11 = "w.stmt#";
			SINGLE_ARGUMENT a12 = "ifs.stmt#";
			SINGLE_ARGUMENT a13 = "a.stmt#";

			Assert::IsTrue(QueryValidator::isValidElem(proc_s, a1));
			Assert::IsTrue(QueryValidator::isValidElem(proc_s, a2));
			Assert::IsTrue(QueryValidator::isValidElem(proc_s, a3));
			Assert::IsTrue(QueryValidator::isValidElem(proc_s, a4));
			Assert::IsTrue(QueryValidator::isValidElem(proc_s, a5));
			Assert::IsTrue(QueryValidator::isValidElem(proc_s, a6));
			Assert::IsTrue(QueryValidator::isValidElem(proc_s, a7));
			Assert::IsTrue(QueryValidator::isValidElem(proc_s, a8));
			Assert::IsTrue(QueryValidator::isValidElem(proc_s, a9));
			Assert::IsTrue(QueryValidator::isValidElem(proc_s, a10));
			Assert::IsTrue(QueryValidator::isValidElem(proc_s, a11));
			Assert::IsTrue(QueryValidator::isValidElem(proc_s, a12));
			Assert::IsTrue(QueryValidator::isValidElem(proc_s, a13));
		}

		TEST_METHOD(isValidAttr_Invalid_Success) {
			PROCESSED_SYNONYMS proc_s;

			QueryNode stmt_s = QueryNode();
			stmt_s.setSynonymNode("stmt", "s");
			proc_s.insert({ "s", stmt_s });

			QueryNode procedure_p = QueryNode();
			procedure_p.setSynonymNode("procedure", "p");
			proc_s.insert({ "p", procedure_p });

			QueryNode variable_v = QueryNode();
			variable_v.setSynonymNode("variable", "v");
			proc_s.insert({ "v", variable_v });

			QueryNode constant_c = QueryNode();
			constant_c.setSynonymNode("constant", "c");
			proc_s.insert({ "c", constant_c });

			SINGLE_ARGUMENT a1 = "p1.procName";
			SINGLE_ARGUMENT a2 = "v.procName";
			SINGLE_ARGUMENT a3 = "v1.varName";
			SINGLE_ARGUMENT a4 = "a.varName";
			SINGLE_ARGUMENT a5 = "c1.value";
			SINGLE_ARGUMENT a6 = "p.value";
			SINGLE_ARGUMENT a7 = "s1.stmt#";
			SINGLE_ARGUMENT a8 = "v.stmt#";

			Assert::IsFalse(QueryValidator::isValidElem(proc_s, a1));
			Assert::IsFalse(QueryValidator::isValidElem(proc_s, a2));
			Assert::IsFalse(QueryValidator::isValidElem(proc_s, a3));
			Assert::IsFalse(QueryValidator::isValidElem(proc_s, a4));
			Assert::IsFalse(QueryValidator::isValidElem(proc_s, a5));
			Assert::IsFalse(QueryValidator::isValidElem(proc_s, a6));
			Assert::IsFalse(QueryValidator::isValidElem(proc_s, a7));
			Assert::IsFalse(QueryValidator::isValidElem(proc_s, a8));
		}

		TEST_METHOD(isValidElem_Valid_Success) {
			PROCESSED_SYNONYMS proc_s;

			QueryNode stmt_s = QueryNode();
			stmt_s.setSynonymNode("stmt", "s");
			proc_s.insert({ "s", stmt_s });

			QueryNode procedure_p = QueryNode();
			procedure_p.setSynonymNode("procedure", "p");
			proc_s.insert({ "p", procedure_p });

			QueryNode variable_v = QueryNode();
			variable_v.setSynonymNode("variable", "v");
			proc_s.insert({ "v", variable_v });
				
			QueryNode constant_c = QueryNode();
			constant_c.setSynonymNode("constant", "c");
			proc_s.insert({ "c", constant_c });

			ELEMENT e1 = "s";
			ELEMENT e2 = "p.procName";
			ELEMENT e3 = "v.varName";
			ELEMENT e4 = "c.value";
			ELEMENT e5 = "s.stmt#";

			Assert::IsTrue(QueryValidator::isValidElem(proc_s, e1));
			Assert::IsTrue(QueryValidator::isValidElem(proc_s, e2));
			Assert::IsTrue(QueryValidator::isValidElem(proc_s, e3));
			Assert::IsTrue(QueryValidator::isValidElem(proc_s, e4));
			Assert::IsTrue(QueryValidator::isValidElem(proc_s, e5));
		}

		TEST_METHOD(isValidElem_Invalid_Success) {
			PROCESSED_SYNONYMS proc_s;

			QueryNode stmt_s = QueryNode();
			stmt_s.setSynonymNode("stmt", "s");
			proc_s.insert({ "s", stmt_s });

			QueryNode procedure_p = QueryNode();
			procedure_p.setSynonymNode("procedure", "p");
			proc_s.insert({ "p", procedure_p });

			ELEMENT e1 = "s1";
			ELEMENT e2 = "p .procName";
			ELEMENT e3 = "p. procName";
			ELEMENT e4 = "p.procname";
			ELEMENT e5 = "p.unknown";
			ELEMENT e6 = " p.procName";
			ELEMENT e7 = "p.procName ";

			Assert::IsFalse(QueryValidator::isValidElem(proc_s, e1));
			Assert::IsFalse(QueryValidator::isValidElem(proc_s, e2));
			Assert::IsFalse(QueryValidator::isValidElem(proc_s, e3));
			Assert::IsFalse(QueryValidator::isValidElem(proc_s, e4));
			Assert::IsFalse(QueryValidator::isValidElem(proc_s, e5));
			Assert::IsFalse(QueryValidator::isValidElem(proc_s, e6));
			Assert::IsFalse(QueryValidator::isValidElem(proc_s, e7));
		}

		TEST_METHOD(isValidResultFormat_Valid_Success) {
			RESULT r1 = "BOOLEAN";
			RESULT r2 = "s";
			RESULT r3 = "s1";
			RESULT r4 = "<s>";
			RESULT r5 = "<s1, s2>";
			RESULT r6 = "<s1, s2, s3, s4, s5, s6>";
			RESULT r7 = "<       s1       ,   s2          >";
			RESULT r8 = "<s1,s2>";
			RESULT r9 = "<s1.stmt#, s2>";
			RESULT r10 = "<s1, s2.stmt#>";
			RESULT r11 = "<s1.stmt#, s2.stmt#>";
			RESULT r12 = "<s1.stmt#>";

			Assert::IsTrue(QueryValidator::isValidResultFormat(r1));
			Assert::IsTrue(QueryValidator::isValidResultFormat(r2));
			Assert::IsTrue(QueryValidator::isValidResultFormat(r3));
			Assert::IsTrue(QueryValidator::isValidResultFormat(r4));
			Assert::IsTrue(QueryValidator::isValidResultFormat(r5));
			Assert::IsTrue(QueryValidator::isValidResultFormat(r6));
			Assert::IsTrue(QueryValidator::isValidResultFormat(r7));
			Assert::IsTrue(QueryValidator::isValidResultFormat(r8));
			Assert::IsTrue(QueryValidator::isValidResultFormat(r9));
			Assert::IsTrue(QueryValidator::isValidResultFormat(r10));
			Assert::IsTrue(QueryValidator::isValidResultFormat(r11));
			Assert::IsTrue(QueryValidator::isValidResultFormat(r12));
		}

		TEST_METHOD(isValidResultFormat_Invalid_Success) {
			RESULT r1 = "1";
			RESULT r2 = "<1>";
			RESULT r3 = "<s1 s2>";
			RESULT r4 = " <s1, s2>";
			RESULT r5 = "<s1, s2> ";
			RESULT r6 = "<s1.unknown, s2>";
			RESULT r7 = "<s1, s2.unknown>";
			RESULT r8 = "<s1.unknown>";

			Assert::IsFalse(QueryValidator::isValidResultFormat(r1));
			Assert::IsFalse(QueryValidator::isValidResultFormat(r2));
			Assert::IsFalse(QueryValidator::isValidResultFormat(r3));
			Assert::IsFalse(QueryValidator::isValidResultFormat(r4));
			Assert::IsFalse(QueryValidator::isValidResultFormat(r5));
			Assert::IsFalse(QueryValidator::isValidResultFormat(r6));
			Assert::IsFalse(QueryValidator::isValidResultFormat(r7));
			Assert::IsFalse(QueryValidator::isValidResultFormat(r8));
		}

		TEST_METHOD(isValidRelationFormat_Valid_Success) {
			SINGLE_CLAUSE r1 = "Follows(1, s)";
			SINGLE_CLAUSE r2 = "Follows*(1, s)";
			SINGLE_CLAUSE r3 = "Parent(1, s)";
			SINGLE_CLAUSE r4 = "Parent*(1, s)";
			SINGLE_CLAUSE r5 = "Uses(1, s)";
			SINGLE_CLAUSE r6 = "Modifies(1, s)";
			SINGLE_CLAUSE r7 = "Follows  (   1   ,   s       )";
			SINGLE_CLAUSE r8 = "Follows(1,s)";

			SINGLE_CLAUSE r9 = "Calls(1, s)";
			SINGLE_CLAUSE r10 = "Calls*(1, s)";
			SINGLE_CLAUSE r11 = "Next(1, s)";
			SINGLE_CLAUSE r12 = "Next*(1, s)";
			SINGLE_CLAUSE r13 = "Affects(1, s)";
			SINGLE_CLAUSE r14 = "Affects*(1, s)";

			Assert::IsTrue(QueryValidator::isValidRelationFormat(r1));
			Assert::IsTrue(QueryValidator::isValidRelationFormat(r2));
			Assert::IsTrue(QueryValidator::isValidRelationFormat(r3));
			Assert::IsTrue(QueryValidator::isValidRelationFormat(r4));
			Assert::IsTrue(QueryValidator::isValidRelationFormat(r5));
			Assert::IsTrue(QueryValidator::isValidRelationFormat(r6));
			Assert::IsTrue(QueryValidator::isValidRelationFormat(r7));
			Assert::IsTrue(QueryValidator::isValidRelationFormat(r8));
			Assert::IsTrue(QueryValidator::isValidRelationFormat(r9));
			Assert::IsTrue(QueryValidator::isValidRelationFormat(r10));
			Assert::IsTrue(QueryValidator::isValidRelationFormat(r11));
			Assert::IsTrue(QueryValidator::isValidRelationFormat(r12));
			Assert::IsTrue(QueryValidator::isValidRelationFormat(r13));
			Assert::IsTrue(QueryValidator::isValidRelationFormat(r14));
		}

		TEST_METHOD(isValidRelationFormat_Invalid_Success) {
			SINGLE_CLAUSE r1 = "follows(1, s)";
			SINGLE_CLAUSE r2 = "Follows *(1, s)";
			SINGLE_CLAUSE r3 = "Follows(1)";
			SINGLE_CLAUSE r4 = "Follows(1 s)";
			SINGLE_CLAUSE r5 = "Test(1, s)";
			SINGLE_CLAUSE r6 = "Follows(1, s, s)";

			Assert::IsFalse(QueryValidator::isValidRelationFormat(r1));
			Assert::IsFalse(QueryValidator::isValidRelationFormat(r2));
			Assert::IsFalse(QueryValidator::isValidRelationFormat(r3));
			Assert::IsFalse(QueryValidator::isValidRelationFormat(r4));
			Assert::IsFalse(QueryValidator::isValidRelationFormat(r5));
			Assert::IsFalse(QueryValidator::isValidRelationFormat(r6));
		}

		TEST_METHOD(isStatementRef_Valid_Success) {
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

			QueryNode call_cl = QueryNode();
			call_cl.setSynonymNode("call", "cl");
			proc_s.insert({ "cl", call_cl });

			QueryNode prog_pl = QueryNode();
			prog_pl.setSynonymNode("prog_line", "pl");
			proc_s.insert({ "pl", prog_pl });

			SINGLE_ARGUMENT arg1 = "1";
			SINGLE_ARGUMENT arg2 = "_";
			SINGLE_ARGUMENT arg3 = "s";
			SINGLE_ARGUMENT arg4 = "r";
			SINGLE_ARGUMENT arg5 = "pn";
			SINGLE_ARGUMENT arg6 = "w";
			SINGLE_ARGUMENT arg7 = "ifs";
			SINGLE_ARGUMENT arg8 = "a";
			SINGLE_ARGUMENT arg9 = "cl";
			SINGLE_ARGUMENT arg10 = "pl";

			Assert::IsTrue(QueryValidator::isStatementRef(proc_s, arg1));
			Assert::IsTrue(QueryValidator::isStatementRef(proc_s, arg2));
			Assert::IsTrue(QueryValidator::isStatementRef(proc_s, arg3));
			Assert::IsTrue(QueryValidator::isStatementRef(proc_s, arg4));
			Assert::IsTrue(QueryValidator::isStatementRef(proc_s, arg5));
			Assert::IsTrue(QueryValidator::isStatementRef(proc_s, arg6));
			Assert::IsTrue(QueryValidator::isStatementRef(proc_s, arg7));
			Assert::IsTrue(QueryValidator::isStatementRef(proc_s, arg8));
			Assert::IsTrue(QueryValidator::isStatementRef(proc_s, arg9));
			Assert::IsTrue(QueryValidator::isStatementRef(proc_s, arg10));
		}

		TEST_METHOD(isStatementRef_Invalid_Success) {
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

			SINGLE_ARGUMENT arg1 = "v";
			SINGLE_ARGUMENT arg2 = "c";
			SINGLE_ARGUMENT arg3 = "p";

			Assert::IsFalse(QueryValidator::isStatementRef(proc_s, arg1));
			Assert::IsFalse(QueryValidator::isStatementRef(proc_s, arg2));
			Assert::IsFalse(QueryValidator::isStatementRef(proc_s, arg3));
		}

		TEST_METHOD(isEntityRef_Valid_Success) {
			PROCESSED_SYNONYMS proc_s;

			QueryNode var_v = QueryNode();
			var_v.setSynonymNode("variable", "v");
			proc_s.insert({ "v", var_v });

			QueryNode procedure_p = QueryNode();
			procedure_p.setSynonymNode("procedure", "p");
			proc_s.insert({ "p", procedure_p });

			SINGLE_ARGUMENT arg1 = "\"identity\"";
			SINGLE_ARGUMENT arg2 = "_";
			SINGLE_ARGUMENT arg3 = "v";
			SINGLE_ARGUMENT arg4 = "p";

			Assert::IsTrue(QueryValidator::isEntityRef(proc_s, arg1));
			Assert::IsTrue(QueryValidator::isEntityRef(proc_s, arg2));
			Assert::IsTrue(QueryValidator::isEntityRef(proc_s, arg3));
			Assert::IsTrue(QueryValidator::isEntityRef(proc_s, arg4));
		}

		TEST_METHOD(isEntityRef_Invalid_Success) {
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

			QueryNode call_cl = QueryNode();
			call_cl.setSynonymNode("call", "cl");
			proc_s.insert({ "cl", call_cl });

			QueryNode prog_pl = QueryNode();
			prog_pl.setSynonymNode("prog_line", "pl");
			proc_s.insert({ "pl", prog_pl });

			QueryNode const_c = QueryNode();
			const_c.setSynonymNode("constant", "c");
			proc_s.insert({ "c", const_c });

			SINGLE_ARGUMENT arg1 = "s";
			SINGLE_ARGUMENT arg2 = "r";
			SINGLE_ARGUMENT arg3 = "pn";
			SINGLE_ARGUMENT arg4 = "w";
			SINGLE_ARGUMENT arg5 = "ifs";
			SINGLE_ARGUMENT arg6 = "a";
			SINGLE_ARGUMENT arg7 = "cl";
			SINGLE_ARGUMENT arg8 = "pl";
			SINGLE_ARGUMENT arg9 = "c";

			Assert::IsFalse(QueryValidator::isEntityRef(proc_s, arg1));
			Assert::IsFalse(QueryValidator::isEntityRef(proc_s, arg2));
			Assert::IsFalse(QueryValidator::isEntityRef(proc_s, arg3));
			Assert::IsFalse(QueryValidator::isEntityRef(proc_s, arg4));
			Assert::IsFalse(QueryValidator::isEntityRef(proc_s, arg5));
			Assert::IsFalse(QueryValidator::isEntityRef(proc_s, arg6));
			Assert::IsFalse(QueryValidator::isEntityRef(proc_s, arg7));
			Assert::IsFalse(QueryValidator::isEntityRef(proc_s, arg8));
			Assert::IsFalse(QueryValidator::isEntityRef(proc_s, arg9));
		}

		TEST_METHOD(isLineRef_Valid_Success) {
			PROCESSED_SYNONYMS proc_s;

			QueryNode prog_pl = QueryNode();
			prog_pl.setSynonymNode("prog_line", "pl");
			proc_s.insert({ "pl", prog_pl });

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

			QueryNode call_cl = QueryNode();
			call_cl.setSynonymNode("call", "cl");
			proc_s.insert({ "cl", call_cl });

			SINGLE_ARGUMENT arg1 = "1";
			SINGLE_ARGUMENT arg2 = "_";
			SINGLE_ARGUMENT arg3 = "pl";
			SINGLE_ARGUMENT arg4 = "s";
			SINGLE_ARGUMENT arg5 = "r";
			SINGLE_ARGUMENT arg6 = "pn";
			SINGLE_ARGUMENT arg7 = "w";
			SINGLE_ARGUMENT arg8 = "ifs";
			SINGLE_ARGUMENT arg9 = "a";
			SINGLE_ARGUMENT arg10 = "cl";

			Assert::IsTrue(QueryValidator::isLineRef(proc_s, arg1));
			Assert::IsTrue(QueryValidator::isLineRef(proc_s, arg2));
			Assert::IsTrue(QueryValidator::isLineRef(proc_s, arg3));
			Assert::IsTrue(QueryValidator::isLineRef(proc_s, arg4));
			Assert::IsTrue(QueryValidator::isLineRef(proc_s, arg5));
			Assert::IsTrue(QueryValidator::isLineRef(proc_s, arg6));
			Assert::IsTrue(QueryValidator::isLineRef(proc_s, arg7));
			Assert::IsTrue(QueryValidator::isLineRef(proc_s, arg8));
			Assert::IsTrue(QueryValidator::isLineRef(proc_s, arg9));
			Assert::IsTrue(QueryValidator::isLineRef(proc_s, arg10));
		}

		TEST_METHOD(isLineRef_Invalid_Success) {
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
			
			SINGLE_ARGUMENT arg1 = "v";
			SINGLE_ARGUMENT arg2 = "c";
			SINGLE_ARGUMENT arg3 = "p";

			Assert::IsFalse(QueryValidator::isLineRef(proc_s, arg1));
			Assert::IsFalse(QueryValidator::isLineRef(proc_s, arg2));
			Assert::IsFalse(QueryValidator::isLineRef(proc_s, arg3));
		}

		TEST_METHOD(isValidRelationArguments_Follows_Valid_Success) {
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

			QueryNode prog_pl = QueryNode();
			prog_pl.setSynonymNode("prog_line", "pl");
			proc_s.insert({ "pl", prog_pl });

			RELATIONSHIP r = "Follows";

			SINGLE_ARGUMENT arg1_1 = "1";
			SINGLE_ARGUMENT arg1_2 = "2";

			SINGLE_ARGUMENT arg2_1 = "1";
			SINGLE_ARGUMENT arg2_2 = "_";

			SINGLE_ARGUMENT arg3_1 = "1";
			SINGLE_ARGUMENT arg3_2 = "s";

			SINGLE_ARGUMENT arg4_1 = "1";
			SINGLE_ARGUMENT arg4_2 = "r";

			SINGLE_ARGUMENT arg5_1 = "1";
			SINGLE_ARGUMENT arg5_2 = "pn";

			SINGLE_ARGUMENT arg6_1 = "1";
			SINGLE_ARGUMENT arg6_2 = "w";

			SINGLE_ARGUMENT arg7_1 = "1";
			SINGLE_ARGUMENT arg7_2 = "ifs";

			SINGLE_ARGUMENT arg8_1 = "1";
			SINGLE_ARGUMENT arg8_2 = "a";

			SINGLE_ARGUMENT arg9_1 = "s";
			SINGLE_ARGUMENT arg9_2 = "a";

			SINGLE_ARGUMENT arg10_1 = "1";
			SINGLE_ARGUMENT arg10_2 = "pl";

			ARGUMENTS args1;
			args1.push_back(arg1_1);
			args1.push_back(arg1_2);

			ARGUMENTS args2;
			args2.push_back(arg2_1);
			args2.push_back(arg2_2);

			ARGUMENTS args3;
			args3.push_back(arg3_1);
			args3.push_back(arg3_2);

			ARGUMENTS args4;
			args4.push_back(arg4_1);
			args4.push_back(arg4_2);

			ARGUMENTS args5;
			args5.push_back(arg5_1);
			args5.push_back(arg5_2);

			ARGUMENTS args6;
			args6.push_back(arg6_1);
			args6.push_back(arg6_2);

			ARGUMENTS args7;
			args7.push_back(arg7_1);
			args7.push_back(arg7_2);

			ARGUMENTS args8;
			args8.push_back(arg8_1);
			args8.push_back(arg8_2);

			ARGUMENTS args9;
			args9.push_back(arg9_1);
			args9.push_back(arg9_2);

			ARGUMENTS args10;
			args10.push_back(arg10_1);
			args10.push_back(arg10_2);

			Assert::IsTrue(QueryValidator::isValidRelationArguments(proc_s, r, args1));
			Assert::IsTrue(QueryValidator::isValidRelationArguments(proc_s, r, args2));
			Assert::IsTrue(QueryValidator::isValidRelationArguments(proc_s, r, args3));
			Assert::IsTrue(QueryValidator::isValidRelationArguments(proc_s, r, args4));
			Assert::IsTrue(QueryValidator::isValidRelationArguments(proc_s, r, args5));
			Assert::IsTrue(QueryValidator::isValidRelationArguments(proc_s, r, args6));
			Assert::IsTrue(QueryValidator::isValidRelationArguments(proc_s, r, args7));
			Assert::IsTrue(QueryValidator::isValidRelationArguments(proc_s, r, args8));
			Assert::IsTrue(QueryValidator::isValidRelationArguments(proc_s, r, args9));
			Assert::IsTrue(QueryValidator::isValidRelationArguments(proc_s, r, args10));
		}

		TEST_METHOD(isValidRelationArguments_Follows_Invalid_Success) {
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

			SINGLE_ARGUMENT arg1_1 = "1";
			SINGLE_ARGUMENT arg1_2 = "undeclared";

			SINGLE_ARGUMENT arg2_1 = "1";
			SINGLE_ARGUMENT arg2_2 = "v";

			SINGLE_ARGUMENT arg3_1 = "1";
			SINGLE_ARGUMENT arg3_2 = "c";

			SINGLE_ARGUMENT arg4_1 = "1";
			SINGLE_ARGUMENT arg4_2 = "p";

			SINGLE_ARGUMENT arg5_1 = "v";
			SINGLE_ARGUMENT arg5_2 = "1";

			SINGLE_ARGUMENT arg6_1 = "c";
			SINGLE_ARGUMENT arg6_2 = "1";

			SINGLE_ARGUMENT arg7_1 = "p";
			SINGLE_ARGUMENT arg7_2 = "1";

			ARGUMENTS args1;
			args1.push_back(arg1_1);
			args1.push_back(arg1_2);

			ARGUMENTS args2;
			args2.push_back(arg2_1);
			args2.push_back(arg2_2);

			ARGUMENTS args3;
			args3.push_back(arg3_1);
			args3.push_back(arg3_2);

			ARGUMENTS args4;
			args4.push_back(arg4_1);
			args4.push_back(arg4_2);

			ARGUMENTS args5;
			args5.push_back(arg5_1);
			args5.push_back(arg5_2);

			ARGUMENTS args6;
			args6.push_back(arg6_1);
			args6.push_back(arg6_2);

			ARGUMENTS args7;
			args7.push_back(arg7_1);
			args7.push_back(arg7_2);

			Assert::IsFalse(QueryValidator::isValidRelationArguments(proc_s, r, args1));
			Assert::IsFalse(QueryValidator::isValidRelationArguments(proc_s, r, args2));
			Assert::IsFalse(QueryValidator::isValidRelationArguments(proc_s, r, args3));
			Assert::IsFalse(QueryValidator::isValidRelationArguments(proc_s, r, args4));
			Assert::IsFalse(QueryValidator::isValidRelationArguments(proc_s, r, args5));
			Assert::IsFalse(QueryValidator::isValidRelationArguments(proc_s, r, args6));
			Assert::IsFalse(QueryValidator::isValidRelationArguments(proc_s, r, args7));
		}

		TEST_METHOD(isValidRelationArguments_FollowsT_Valid_Success) {
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

			QueryNode prog_pl = QueryNode();
			prog_pl.setSynonymNode("prog_line", "pl");
			proc_s.insert({ "pl", prog_pl });

			RELATIONSHIP r = "Follows*";

			SINGLE_ARGUMENT arg1_1 = "1";
			SINGLE_ARGUMENT arg1_2 = "2";

			SINGLE_ARGUMENT arg2_1 = "1";
			SINGLE_ARGUMENT arg2_2 = "_";

			SINGLE_ARGUMENT arg3_1 = "1";
			SINGLE_ARGUMENT arg3_2 = "s";

			SINGLE_ARGUMENT arg4_1 = "1";
			SINGLE_ARGUMENT arg4_2 = "r";

			SINGLE_ARGUMENT arg5_1 = "1";
			SINGLE_ARGUMENT arg5_2 = "pn";

			SINGLE_ARGUMENT arg6_1 = "1";
			SINGLE_ARGUMENT arg6_2 = "w";

			SINGLE_ARGUMENT arg7_1 = "1";
			SINGLE_ARGUMENT arg7_2 = "ifs";

			SINGLE_ARGUMENT arg8_1 = "1";
			SINGLE_ARGUMENT arg8_2 = "a";

			SINGLE_ARGUMENT arg9_1 = "s";
			SINGLE_ARGUMENT arg9_2 = "a";

			SINGLE_ARGUMENT arg10_1 = "1";
			SINGLE_ARGUMENT arg10_2 = "pl";

			ARGUMENTS args1;
			args1.push_back(arg1_1);
			args1.push_back(arg1_2);

			ARGUMENTS args2;
			args2.push_back(arg2_1);
			args2.push_back(arg2_2);

			ARGUMENTS args3;
			args3.push_back(arg3_1);
			args3.push_back(arg3_2);

			ARGUMENTS args4;
			args4.push_back(arg4_1);
			args4.push_back(arg4_2);

			ARGUMENTS args5;
			args5.push_back(arg5_1);
			args5.push_back(arg5_2);

			ARGUMENTS args6;
			args6.push_back(arg6_1);
			args6.push_back(arg6_2);

			ARGUMENTS args7;
			args7.push_back(arg7_1);
			args7.push_back(arg7_2);

			ARGUMENTS args8;
			args8.push_back(arg8_1);
			args8.push_back(arg8_2);

			ARGUMENTS args9;
			args9.push_back(arg9_1);
			args9.push_back(arg9_2);

			ARGUMENTS args10;
			args10.push_back(arg10_1);
			args10.push_back(arg10_2);

			Assert::IsTrue(QueryValidator::isValidRelationArguments(proc_s, r, args1));
			Assert::IsTrue(QueryValidator::isValidRelationArguments(proc_s, r, args2));
			Assert::IsTrue(QueryValidator::isValidRelationArguments(proc_s, r, args3));
			Assert::IsTrue(QueryValidator::isValidRelationArguments(proc_s, r, args4));
			Assert::IsTrue(QueryValidator::isValidRelationArguments(proc_s, r, args5));
			Assert::IsTrue(QueryValidator::isValidRelationArguments(proc_s, r, args6));
			Assert::IsTrue(QueryValidator::isValidRelationArguments(proc_s, r, args7));
			Assert::IsTrue(QueryValidator::isValidRelationArguments(proc_s, r, args8));
			Assert::IsTrue(QueryValidator::isValidRelationArguments(proc_s, r, args9));
			Assert::IsTrue(QueryValidator::isValidRelationArguments(proc_s, r, args10));
		}

		TEST_METHOD(isValidRelationArguments_FollowsT_Invalid_Success) {
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

			RELATIONSHIP r = "Follows*";

			SINGLE_ARGUMENT arg1_1 = "1";
			SINGLE_ARGUMENT arg1_2 = "undeclared";

			SINGLE_ARGUMENT arg2_1 = "1";
			SINGLE_ARGUMENT arg2_2 = "v";

			SINGLE_ARGUMENT arg3_1 = "1";
			SINGLE_ARGUMENT arg3_2 = "c";

			SINGLE_ARGUMENT arg4_1 = "1";
			SINGLE_ARGUMENT arg4_2 = "p";

			SINGLE_ARGUMENT arg5_1 = "v";
			SINGLE_ARGUMENT arg5_2 = "1";

			SINGLE_ARGUMENT arg6_1 = "c";
			SINGLE_ARGUMENT arg6_2 = "1";

			SINGLE_ARGUMENT arg7_1 = "p";
			SINGLE_ARGUMENT arg7_2 = "1";

			ARGUMENTS args1;
			args1.push_back(arg1_1);
			args1.push_back(arg1_2);

			ARGUMENTS args2;
			args2.push_back(arg2_1);
			args2.push_back(arg2_2);

			ARGUMENTS args3;
			args3.push_back(arg3_1);
			args3.push_back(arg3_2);

			ARGUMENTS args4;
			args4.push_back(arg4_1);
			args4.push_back(arg4_2);

			ARGUMENTS args5;
			args5.push_back(arg5_1);
			args5.push_back(arg5_2);

			ARGUMENTS args6;
			args6.push_back(arg6_1);
			args6.push_back(arg6_2);

			ARGUMENTS args7;
			args7.push_back(arg7_1);
			args7.push_back(arg7_2);

			Assert::IsFalse(QueryValidator::isValidRelationArguments(proc_s, r, args1));
			Assert::IsFalse(QueryValidator::isValidRelationArguments(proc_s, r, args2));
			Assert::IsFalse(QueryValidator::isValidRelationArguments(proc_s, r, args3));
			Assert::IsFalse(QueryValidator::isValidRelationArguments(proc_s, r, args4));
			Assert::IsFalse(QueryValidator::isValidRelationArguments(proc_s, r, args5));
			Assert::IsFalse(QueryValidator::isValidRelationArguments(proc_s, r, args6));
			Assert::IsFalse(QueryValidator::isValidRelationArguments(proc_s, r, args7));
		}

		TEST_METHOD(isValidRelationArguments_Parent_Valid_Success) {
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

			QueryNode prog_pl = QueryNode();
			prog_pl.setSynonymNode("prog_line", "pl");
			proc_s.insert({ "pl", prog_pl });

			RELATIONSHIP r = "Parent";

			SINGLE_ARGUMENT arg1_1 = "1";
			SINGLE_ARGUMENT arg1_2 = "2";

			SINGLE_ARGUMENT arg2_1 = "1";
			SINGLE_ARGUMENT arg2_2 = "_";

			SINGLE_ARGUMENT arg3_1 = "1";
			SINGLE_ARGUMENT arg3_2 = "s";

			SINGLE_ARGUMENT arg4_1 = "1";
			SINGLE_ARGUMENT arg4_2 = "r";

			SINGLE_ARGUMENT arg5_1 = "1";
			SINGLE_ARGUMENT arg5_2 = "pn";

			SINGLE_ARGUMENT arg6_1 = "1";
			SINGLE_ARGUMENT arg6_2 = "w";

			SINGLE_ARGUMENT arg7_1 = "1";
			SINGLE_ARGUMENT arg7_2 = "ifs";

			SINGLE_ARGUMENT arg8_1 = "1";
			SINGLE_ARGUMENT arg8_2 = "a";

			SINGLE_ARGUMENT arg9_1 = "s";
			SINGLE_ARGUMENT arg9_2 = "a";

			SINGLE_ARGUMENT arg10_1 = "1";
			SINGLE_ARGUMENT arg10_2 = "pl";

			ARGUMENTS args1;
			args1.push_back(arg1_1);
			args1.push_back(arg1_2);

			ARGUMENTS args2;
			args2.push_back(arg2_1);
			args2.push_back(arg2_2);

			ARGUMENTS args3;
			args3.push_back(arg3_1);
			args3.push_back(arg3_2);

			ARGUMENTS args4;
			args4.push_back(arg4_1);
			args4.push_back(arg4_2);

			ARGUMENTS args5;
			args5.push_back(arg5_1);
			args5.push_back(arg5_2);

			ARGUMENTS args6;
			args6.push_back(arg6_1);
			args6.push_back(arg6_2);

			ARGUMENTS args7;
			args7.push_back(arg7_1);
			args7.push_back(arg7_2);

			ARGUMENTS args8;
			args8.push_back(arg8_1);
			args8.push_back(arg8_2);

			ARGUMENTS args9;
			args9.push_back(arg9_1);
			args9.push_back(arg9_2);

			ARGUMENTS args10;
			args10.push_back(arg10_1);
			args10.push_back(arg10_2);

			Assert::IsTrue(QueryValidator::isValidRelationArguments(proc_s, r, args1));
			Assert::IsTrue(QueryValidator::isValidRelationArguments(proc_s, r, args2));
			Assert::IsTrue(QueryValidator::isValidRelationArguments(proc_s, r, args3));
			Assert::IsTrue(QueryValidator::isValidRelationArguments(proc_s, r, args4));
			Assert::IsTrue(QueryValidator::isValidRelationArguments(proc_s, r, args5));
			Assert::IsTrue(QueryValidator::isValidRelationArguments(proc_s, r, args6));
			Assert::IsTrue(QueryValidator::isValidRelationArguments(proc_s, r, args7));
			Assert::IsTrue(QueryValidator::isValidRelationArguments(proc_s, r, args8));
			Assert::IsTrue(QueryValidator::isValidRelationArguments(proc_s, r, args9));
			Assert::IsTrue(QueryValidator::isValidRelationArguments(proc_s, r, args10));
		}

		TEST_METHOD(isValidRelationArguments_Parent_Invalid_Success) {
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

			SINGLE_ARGUMENT arg1_1 = "1";
			SINGLE_ARGUMENT arg1_2 = "undeclared";

			SINGLE_ARGUMENT arg2_1 = "1";
			SINGLE_ARGUMENT arg2_2 = "v";

			SINGLE_ARGUMENT arg3_1 = "1";
			SINGLE_ARGUMENT arg3_2 = "c";

			SINGLE_ARGUMENT arg4_1 = "1";
			SINGLE_ARGUMENT arg4_2 = "p";

			SINGLE_ARGUMENT arg5_1 = "v";
			SINGLE_ARGUMENT arg5_2 = "1";

			SINGLE_ARGUMENT arg6_1 = "c";
			SINGLE_ARGUMENT arg6_2 = "1";

			SINGLE_ARGUMENT arg7_1 = "p";
			SINGLE_ARGUMENT arg7_2 = "1";

			ARGUMENTS args1;
			args1.push_back(arg1_1);
			args1.push_back(arg1_2);

			ARGUMENTS args2;
			args2.push_back(arg2_1);
			args2.push_back(arg2_2);

			ARGUMENTS args3;
			args3.push_back(arg3_1);
			args3.push_back(arg3_2);

			ARGUMENTS args4;
			args4.push_back(arg4_1);
			args4.push_back(arg4_2);

			ARGUMENTS args5;
			args5.push_back(arg5_1);
			args5.push_back(arg5_2);

			ARGUMENTS args6;
			args6.push_back(arg6_1);
			args6.push_back(arg6_2);

			ARGUMENTS args7;
			args7.push_back(arg7_1);
			args7.push_back(arg7_2);

			Assert::IsFalse(QueryValidator::isValidRelationArguments(proc_s, r, args1));
			Assert::IsFalse(QueryValidator::isValidRelationArguments(proc_s, r, args2));
			Assert::IsFalse(QueryValidator::isValidRelationArguments(proc_s, r, args3));
			Assert::IsFalse(QueryValidator::isValidRelationArguments(proc_s, r, args4));
			Assert::IsFalse(QueryValidator::isValidRelationArguments(proc_s, r, args5));
			Assert::IsFalse(QueryValidator::isValidRelationArguments(proc_s, r, args6));
			Assert::IsFalse(QueryValidator::isValidRelationArguments(proc_s, r, args7));
		}

		TEST_METHOD(isValidRelationArguments_ParentT_Valid_Success) {
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

			QueryNode prog_pl = QueryNode();
			prog_pl.setSynonymNode("prog_line", "pl");
			proc_s.insert({ "pl", prog_pl });

			RELATIONSHIP r = "Parent*";

			SINGLE_ARGUMENT arg1_1 = "1";
			SINGLE_ARGUMENT arg1_2 = "2";

			SINGLE_ARGUMENT arg2_1 = "1";
			SINGLE_ARGUMENT arg2_2 = "_";

			SINGLE_ARGUMENT arg3_1 = "1";
			SINGLE_ARGUMENT arg3_2 = "s";

			SINGLE_ARGUMENT arg4_1 = "1";
			SINGLE_ARGUMENT arg4_2 = "r";

			SINGLE_ARGUMENT arg5_1 = "1";
			SINGLE_ARGUMENT arg5_2 = "pn";

			SINGLE_ARGUMENT arg6_1 = "1";
			SINGLE_ARGUMENT arg6_2 = "w";

			SINGLE_ARGUMENT arg7_1 = "1";
			SINGLE_ARGUMENT arg7_2 = "ifs";

			SINGLE_ARGUMENT arg8_1 = "1";
			SINGLE_ARGUMENT arg8_2 = "a";

			SINGLE_ARGUMENT arg9_1 = "s";
			SINGLE_ARGUMENT arg9_2 = "a";

			SINGLE_ARGUMENT arg10_1 = "1";
			SINGLE_ARGUMENT arg10_2 = "pl";

			ARGUMENTS args1;
			args1.push_back(arg1_1);
			args1.push_back(arg1_2);

			ARGUMENTS args2;
			args2.push_back(arg2_1);
			args2.push_back(arg2_2);

			ARGUMENTS args3;
			args3.push_back(arg3_1);
			args3.push_back(arg3_2);

			ARGUMENTS args4;
			args4.push_back(arg4_1);
			args4.push_back(arg4_2);

			ARGUMENTS args5;
			args5.push_back(arg5_1);
			args5.push_back(arg5_2);

			ARGUMENTS args6;
			args6.push_back(arg6_1);
			args6.push_back(arg6_2);

			ARGUMENTS args7;
			args7.push_back(arg7_1);
			args7.push_back(arg7_2);

			ARGUMENTS args8;
			args8.push_back(arg8_1);
			args8.push_back(arg8_2);

			ARGUMENTS args9;
			args9.push_back(arg9_1);
			args9.push_back(arg9_2);

			ARGUMENTS args10;
			args10.push_back(arg10_1);
			args10.push_back(arg10_2);

			Assert::IsTrue(QueryValidator::isValidRelationArguments(proc_s, r, args1));
			Assert::IsTrue(QueryValidator::isValidRelationArguments(proc_s, r, args2));
			Assert::IsTrue(QueryValidator::isValidRelationArguments(proc_s, r, args3));
			Assert::IsTrue(QueryValidator::isValidRelationArguments(proc_s, r, args4));
			Assert::IsTrue(QueryValidator::isValidRelationArguments(proc_s, r, args5));
			Assert::IsTrue(QueryValidator::isValidRelationArguments(proc_s, r, args6));
			Assert::IsTrue(QueryValidator::isValidRelationArguments(proc_s, r, args7));
			Assert::IsTrue(QueryValidator::isValidRelationArguments(proc_s, r, args8));
			Assert::IsTrue(QueryValidator::isValidRelationArguments(proc_s, r, args9));
			Assert::IsTrue(QueryValidator::isValidRelationArguments(proc_s, r, args10));
		}

		TEST_METHOD(isValidRelationArguments_ParentT_Invalid_Success) {
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

			RELATIONSHIP r = "Parent*";

			SINGLE_ARGUMENT arg1_1 = "1";
			SINGLE_ARGUMENT arg1_2 = "undeclared";

			SINGLE_ARGUMENT arg2_1 = "1";
			SINGLE_ARGUMENT arg2_2 = "v";

			SINGLE_ARGUMENT arg3_1 = "1";
			SINGLE_ARGUMENT arg3_2 = "c";

			SINGLE_ARGUMENT arg4_1 = "1";
			SINGLE_ARGUMENT arg4_2 = "p";

			SINGLE_ARGUMENT arg5_1 = "v";
			SINGLE_ARGUMENT arg5_2 = "1";

			SINGLE_ARGUMENT arg6_1 = "c";
			SINGLE_ARGUMENT arg6_2 = "1";

			SINGLE_ARGUMENT arg7_1 = "p";
			SINGLE_ARGUMENT arg7_2 = "1";

			ARGUMENTS args1;
			args1.push_back(arg1_1);
			args1.push_back(arg1_2);

			ARGUMENTS args2;
			args2.push_back(arg2_1);
			args2.push_back(arg2_2);

			ARGUMENTS args3;
			args3.push_back(arg3_1);
			args3.push_back(arg3_2);

			ARGUMENTS args4;
			args4.push_back(arg4_1);
			args4.push_back(arg4_2);

			ARGUMENTS args5;
			args5.push_back(arg5_1);
			args5.push_back(arg5_2);

			ARGUMENTS args6;
			args6.push_back(arg6_1);
			args6.push_back(arg6_2);

			ARGUMENTS args7;
			args7.push_back(arg7_1);
			args7.push_back(arg7_2);

			Assert::IsFalse(QueryValidator::isValidRelationArguments(proc_s, r, args1));
			Assert::IsFalse(QueryValidator::isValidRelationArguments(proc_s, r, args2));
			Assert::IsFalse(QueryValidator::isValidRelationArguments(proc_s, r, args3));
			Assert::IsFalse(QueryValidator::isValidRelationArguments(proc_s, r, args4));
			Assert::IsFalse(QueryValidator::isValidRelationArguments(proc_s, r, args5));
			Assert::IsFalse(QueryValidator::isValidRelationArguments(proc_s, r, args6));
			Assert::IsFalse(QueryValidator::isValidRelationArguments(proc_s, r, args7));
		}

		TEST_METHOD(isValidRelationArguments_UsesS_Valid_Success) {
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

			QueryNode stmt_s = QueryNode();
			stmt_s.setSynonymNode("stmt", "s");
			proc_s.insert({ "s", stmt_s });

			RELATIONSHIP r = "Uses";

			SINGLE_ARGUMENT arg1_1 = "a";
			SINGLE_ARGUMENT arg1_2 = "v";

			SINGLE_ARGUMENT arg2_1 = "pn";
			SINGLE_ARGUMENT arg2_2 = "v";

			SINGLE_ARGUMENT arg3_1 = "ifs";
			SINGLE_ARGUMENT arg3_2 = "v";

			SINGLE_ARGUMENT arg4_1 = "w";
			SINGLE_ARGUMENT arg4_2 = "v";

			SINGLE_ARGUMENT arg5_1 = "1";
			SINGLE_ARGUMENT arg5_2 = "v";

			SINGLE_ARGUMENT arg6_1 = "a";
			SINGLE_ARGUMENT arg6_2 = "_";

			SINGLE_ARGUMENT arg7_1 = "a";
			SINGLE_ARGUMENT arg7_2 = "\"identity\"";

			SINGLE_ARGUMENT arg8_1 = "s";
			SINGLE_ARGUMENT arg8_2 = "v";

			ARGUMENTS args1;
			args1.push_back(arg1_1);
			args1.push_back(arg1_2);

			ARGUMENTS args2;
			args2.push_back(arg2_1);
			args2.push_back(arg2_2);

			ARGUMENTS args3;
			args3.push_back(arg3_1);
			args3.push_back(arg3_2);

			ARGUMENTS args4;
			args4.push_back(arg4_1);
			args4.push_back(arg4_2);

			ARGUMENTS args5;
			args5.push_back(arg5_1);
			args5.push_back(arg5_2);

			ARGUMENTS args6;
			args6.push_back(arg6_1);
			args6.push_back(arg6_2);

			ARGUMENTS args7;
			args7.push_back(arg7_1);
			args7.push_back(arg7_2);

			ARGUMENTS args8;
			args8.push_back(arg8_1);
			args8.push_back(arg8_2);

			Assert::IsTrue(QueryValidator::isValidRelationArguments(proc_s, r, args1));
			Assert::IsTrue(QueryValidator::isValidRelationArguments(proc_s, r, args2));
			Assert::IsTrue(QueryValidator::isValidRelationArguments(proc_s, r, args3));
			Assert::IsTrue(QueryValidator::isValidRelationArguments(proc_s, r, args4));
			Assert::IsTrue(QueryValidator::isValidRelationArguments(proc_s, r, args5));
			Assert::IsTrue(QueryValidator::isValidRelationArguments(proc_s, r, args6));
			Assert::IsTrue(QueryValidator::isValidRelationArguments(proc_s, r, args7));
			Assert::IsTrue(QueryValidator::isValidRelationArguments(proc_s, r, args8));
		}

		TEST_METHOD(isValidRelationArguments_UsesP_Valid_Success) {
			PROCESSED_SYNONYMS proc_s;

			QueryNode var_v = QueryNode();
			var_v.setSynonymNode("variable", "v");
			proc_s.insert({ "v", var_v });

			QueryNode procedure_p = QueryNode();
			procedure_p.setSynonymNode("procedure", "p");
			proc_s.insert({ "p", procedure_p });

			RELATIONSHIP r = "Uses";

			SINGLE_ARGUMENT arg1_1 = "\"identity\"";
			SINGLE_ARGUMENT arg1_2 = "v";

			SINGLE_ARGUMENT arg2_1 = "p";
			SINGLE_ARGUMENT arg2_2 = "v";

			SINGLE_ARGUMENT arg3_1 = "p";
			SINGLE_ARGUMENT arg3_2 = "\"identity\"";

			ARGUMENTS args1;
			args1.push_back(arg1_1);
			args1.push_back(arg1_2);

			ARGUMENTS args2;
			args2.push_back(arg2_1);
			args2.push_back(arg2_2);

			ARGUMENTS args3;
			args3.push_back(arg3_1);
			args3.push_back(arg3_2);

			Assert::IsTrue(QueryValidator::isValidRelationArguments(proc_s, r, args1));
			Assert::IsTrue(QueryValidator::isValidRelationArguments(proc_s, r, args2));
			Assert::IsTrue(QueryValidator::isValidRelationArguments(proc_s, r, args3));
		}

		TEST_METHOD(isValidRelationArguments_Uses_Invalid_Success) {
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

			QueryNode stmt_s = QueryNode();
			stmt_s.setSynonymNode("stmt", "s");
			proc_s.insert({ "s", stmt_s });

			QueryNode procedure_p = QueryNode();
			procedure_p.setSynonymNode("procedure", "p");
			proc_s.insert({ "p", procedure_p });

			RELATIONSHIP r = "Uses";

			SINGLE_ARGUMENT arg1_1 = "_";
			SINGLE_ARGUMENT arg1_2 = "v";

			SINGLE_ARGUMENT arg2_1 = "r";
			SINGLE_ARGUMENT arg2_2 = "v";

			SINGLE_ARGUMENT arg3_1 = "a";
			SINGLE_ARGUMENT arg3_2 = "1";

			SINGLE_ARGUMENT arg4_1 = "undeclared";
			SINGLE_ARGUMENT arg4_2 = "v";

			SINGLE_ARGUMENT arg5_1 = "a";
			SINGLE_ARGUMENT arg5_2 = "w";

			SINGLE_ARGUMENT arg6_1 = "s";
			SINGLE_ARGUMENT arg6_2 = "p";

			SINGLE_ARGUMENT arg7_1 = "v";
			SINGLE_ARGUMENT arg7_2 = "_";

			ARGUMENTS args1;
			args1.push_back(arg1_1);
			args1.push_back(arg1_2);

			ARGUMENTS args2;
			args2.push_back(arg2_1);
			args2.push_back(arg2_2);

			ARGUMENTS args3;
			args3.push_back(arg3_1);
			args3.push_back(arg3_2);

			ARGUMENTS args4;
			args4.push_back(arg4_1);
			args4.push_back(arg4_2);

			ARGUMENTS args5;
			args5.push_back(arg5_1);
			args5.push_back(arg5_2);

			ARGUMENTS args6;
			args6.push_back(arg6_1);
			args6.push_back(arg6_2);

			ARGUMENTS args7;
			args7.push_back(arg7_1);
			args7.push_back(arg7_2);

			Assert::IsFalse(QueryValidator::isValidRelationArguments(proc_s, r, args1));
			Assert::IsFalse(QueryValidator::isValidRelationArguments(proc_s, r, args2));
			Assert::IsFalse(QueryValidator::isValidRelationArguments(proc_s, r, args3));
			Assert::IsFalse(QueryValidator::isValidRelationArguments(proc_s, r, args4));
			Assert::IsFalse(QueryValidator::isValidRelationArguments(proc_s, r, args5));
			Assert::IsFalse(QueryValidator::isValidRelationArguments(proc_s, r, args6));
			Assert::IsFalse(QueryValidator::isValidRelationArguments(proc_s, r, args7));
		}

		TEST_METHOD(isValidRelationArguments_ModifiesS_Valid_Success) {
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

			QueryNode stmt_s = QueryNode();
			stmt_s.setSynonymNode("stmt", "s");
			proc_s.insert({ "s", stmt_s });

			RELATIONSHIP r = "Modifies";

			SINGLE_ARGUMENT arg1_1 = "a";
			SINGLE_ARGUMENT arg1_2 = "v";

			SINGLE_ARGUMENT arg2_1 = "r";
			SINGLE_ARGUMENT arg2_2 = "v";

			SINGLE_ARGUMENT arg3_1 = "ifs";
			SINGLE_ARGUMENT arg3_2 = "v";

			SINGLE_ARGUMENT arg4_1 = "w";
			SINGLE_ARGUMENT arg4_2 = "v";

			SINGLE_ARGUMENT arg5_1 = "1";
			SINGLE_ARGUMENT arg5_2 = "v";

			SINGLE_ARGUMENT arg6_1 = "a";
			SINGLE_ARGUMENT arg6_2 = "_";

			SINGLE_ARGUMENT arg7_1 = "a";
			SINGLE_ARGUMENT arg7_2 = "\"identity\"";

			SINGLE_ARGUMENT arg8_1 = "s";
			SINGLE_ARGUMENT arg8_2 = "v";

			ARGUMENTS args1;
			args1.push_back(arg1_1);
			args1.push_back(arg1_2);

			ARGUMENTS args2;
			args2.push_back(arg2_1);
			args2.push_back(arg2_2);

			ARGUMENTS args3;
			args3.push_back(arg3_1);
			args3.push_back(arg3_2);

			ARGUMENTS args4;
			args4.push_back(arg4_1);
			args4.push_back(arg4_2);

			ARGUMENTS args5;
			args5.push_back(arg5_1);
			args5.push_back(arg5_2);

			ARGUMENTS args6;
			args6.push_back(arg6_1);
			args6.push_back(arg6_2);

			ARGUMENTS args7;
			args7.push_back(arg7_1);
			args7.push_back(arg7_2);

			ARGUMENTS args8;
			args8.push_back(arg8_1);
			args8.push_back(arg8_2);

			Assert::IsTrue(QueryValidator::isValidRelationArguments(proc_s, r, args1));
			Assert::IsTrue(QueryValidator::isValidRelationArguments(proc_s, r, args2));
			Assert::IsTrue(QueryValidator::isValidRelationArguments(proc_s, r, args3));
			Assert::IsTrue(QueryValidator::isValidRelationArguments(proc_s, r, args4));
			Assert::IsTrue(QueryValidator::isValidRelationArguments(proc_s, r, args5));
			Assert::IsTrue(QueryValidator::isValidRelationArguments(proc_s, r, args6));
			Assert::IsTrue(QueryValidator::isValidRelationArguments(proc_s, r, args7));
			Assert::IsTrue(QueryValidator::isValidRelationArguments(proc_s, r, args8));
		}


		TEST_METHOD(isValidRelationArguments_ModifiesP_Valid_Success) {
			PROCESSED_SYNONYMS proc_s;

			QueryNode var_v = QueryNode();
			var_v.setSynonymNode("variable", "v");
			proc_s.insert({ "v", var_v });

			QueryNode procedure_p = QueryNode();
			procedure_p.setSynonymNode("procedure", "p");
			proc_s.insert({ "p", procedure_p });

			RELATIONSHIP r = "Modifies";

			SINGLE_ARGUMENT arg1_1 = "\"identity\"";
			SINGLE_ARGUMENT arg1_2 = "v";

			SINGLE_ARGUMENT arg2_1 = "p";
			SINGLE_ARGUMENT arg2_2 = "v";

			SINGLE_ARGUMENT arg3_1 = "p";
			SINGLE_ARGUMENT arg3_2 = "\"identity\"";

			ARGUMENTS args1;
			args1.push_back(arg1_1);
			args1.push_back(arg1_2);

			ARGUMENTS args2;
			args2.push_back(arg2_1);
			args2.push_back(arg2_2);

			ARGUMENTS args3;
			args3.push_back(arg3_1);
			args3.push_back(arg3_2);

			Assert::IsTrue(QueryValidator::isValidRelationArguments(proc_s, r, args1));
			Assert::IsTrue(QueryValidator::isValidRelationArguments(proc_s, r, args2));
			Assert::IsTrue(QueryValidator::isValidRelationArguments(proc_s, r, args3));
		}

		TEST_METHOD(isValidRelationArguments_Modifies_Invalid_Success) {
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

			SINGLE_ARGUMENT arg1_1 = "_";
			SINGLE_ARGUMENT arg1_2 = "v";

			SINGLE_ARGUMENT arg2_1 = "pn";
			SINGLE_ARGUMENT arg2_2 = "v";

			SINGLE_ARGUMENT arg3_1 = "a";
			SINGLE_ARGUMENT arg3_2 = "1";

			SINGLE_ARGUMENT arg4_1 = "undeclared";
			SINGLE_ARGUMENT arg4_2 = "v";

			SINGLE_ARGUMENT arg5_1 = "a";
			SINGLE_ARGUMENT arg5_2 = "w";

			SINGLE_ARGUMENT arg6_1 = "s";
			SINGLE_ARGUMENT arg6_2 = "p";

			SINGLE_ARGUMENT arg7_1 = "v";
			SINGLE_ARGUMENT arg7_2 = "_";

			ARGUMENTS args1;
			args1.push_back(arg1_1);
			args1.push_back(arg1_2);

			ARGUMENTS args2;
			args2.push_back(arg2_1);
			args2.push_back(arg2_2);

			ARGUMENTS args3;
			args3.push_back(arg3_1);
			args3.push_back(arg3_2);

			ARGUMENTS args4;
			args4.push_back(arg4_1);
			args4.push_back(arg4_2);

			ARGUMENTS args5;
			args5.push_back(arg5_1);
			args5.push_back(arg5_2);

			ARGUMENTS args6;
			args6.push_back(arg6_1);
			args6.push_back(arg6_2);

			ARGUMENTS args7;
			args7.push_back(arg7_1);
			args7.push_back(arg7_2);

			Assert::IsFalse(QueryValidator::isValidRelationArguments(proc_s, r, args1));
			Assert::IsFalse(QueryValidator::isValidRelationArguments(proc_s, r, args2));
			Assert::IsFalse(QueryValidator::isValidRelationArguments(proc_s, r, args3));
			Assert::IsFalse(QueryValidator::isValidRelationArguments(proc_s, r, args4));
			Assert::IsFalse(QueryValidator::isValidRelationArguments(proc_s, r, args5));
			Assert::IsFalse(QueryValidator::isValidRelationArguments(proc_s, r, args6));
			Assert::IsFalse(QueryValidator::isValidRelationArguments(proc_s, r, args7));
		}

		TEST_METHOD(isValidRelationArguments_Calls_Valid_Success) {
			PROCESSED_SYNONYMS proc_s;

			QueryNode procedure_p1 = QueryNode();
			procedure_p1.setSynonymNode("procedure", "p1");
			proc_s.insert({ "p1", procedure_p1 });

			QueryNode procedure_p2 = QueryNode();
			procedure_p2.setSynonymNode("procedure", "p2");
			proc_s.insert({ "p2", procedure_p2 });

			RELATIONSHIP r = "Calls";

			SINGLE_ARGUMENT arg1_1 = "p1";
			SINGLE_ARGUMENT arg1_2 = "p2";

			SINGLE_ARGUMENT arg2_1 = "_";
			SINGLE_ARGUMENT arg2_2 = "p2";

			SINGLE_ARGUMENT arg3_1 = "\"identity\"";
			SINGLE_ARGUMENT arg3_2 = "p2";

			SINGLE_ARGUMENT arg4_1 = "p1";
			SINGLE_ARGUMENT arg4_2 = "_";

			SINGLE_ARGUMENT arg5_1 = "p1";
			SINGLE_ARGUMENT arg5_2 = "\"identity\"";

			ARGUMENTS args1;
			args1.push_back(arg1_1);
			args1.push_back(arg1_2);

			ARGUMENTS args2;
			args2.push_back(arg2_1);
			args2.push_back(arg2_2);

			ARGUMENTS args3;
			args3.push_back(arg3_1);
			args3.push_back(arg3_2);

			ARGUMENTS args4;
			args4.push_back(arg4_1);
			args4.push_back(arg4_2);

			ARGUMENTS args5;
			args5.push_back(arg5_1);
			args5.push_back(arg5_2);

			Assert::IsTrue(QueryValidator::isValidRelationArguments(proc_s, r, args1));
			Assert::IsTrue(QueryValidator::isValidRelationArguments(proc_s, r, args2));
			Assert::IsTrue(QueryValidator::isValidRelationArguments(proc_s, r, args3));
			Assert::IsTrue(QueryValidator::isValidRelationArguments(proc_s, r, args4));
			Assert::IsTrue(QueryValidator::isValidRelationArguments(proc_s, r, args5));
		}

		TEST_METHOD(isValidRelationArguments_Calls_Invalid_Success) {
			PROCESSED_SYNONYMS proc_s;

			QueryNode var_v = QueryNode();
			var_v.setSynonymNode("variable", "v");
			proc_s.insert({ "v", var_v });

			QueryNode assign_a = QueryNode();
			assign_a.setSynonymNode("assign", "a");
			proc_s.insert({ "a", assign_a });

			QueryNode procedure_p = QueryNode();
			procedure_p.setSynonymNode("procedure", "p");
			proc_s.insert({ "p", procedure_p });

			RELATIONSHIP r = "Calls";

			SINGLE_ARGUMENT arg1_1 = "undeclared";
			SINGLE_ARGUMENT arg1_2 = "p";

			SINGLE_ARGUMENT arg2_1 = "1";
			SINGLE_ARGUMENT arg2_2 = "p";

			SINGLE_ARGUMENT arg3_1 = "a";
			SINGLE_ARGUMENT arg3_2 = "p";

			SINGLE_ARGUMENT arg4_1 = "v";
			SINGLE_ARGUMENT arg4_2 = "p";

			SINGLE_ARGUMENT arg5_1 = "p";
			SINGLE_ARGUMENT arg5_2 = "1";

			SINGLE_ARGUMENT arg6_1 = "p";
			SINGLE_ARGUMENT arg6_2 = "a";

			SINGLE_ARGUMENT arg7_1 = "p";
			SINGLE_ARGUMENT arg7_2 = "v";

			ARGUMENTS args1;
			args1.push_back(arg1_1);
			args1.push_back(arg1_2);

			ARGUMENTS args2;
			args2.push_back(arg2_1);
			args2.push_back(arg2_2);

			ARGUMENTS args3;
			args3.push_back(arg3_1);
			args3.push_back(arg3_2);

			ARGUMENTS args4;
			args4.push_back(arg4_1);
			args4.push_back(arg4_2);

			ARGUMENTS args5;
			args5.push_back(arg5_1);
			args5.push_back(arg5_2);

			ARGUMENTS args6;
			args6.push_back(arg6_1);
			args6.push_back(arg6_2);

			ARGUMENTS args7;
			args7.push_back(arg7_1);
			args7.push_back(arg7_2);

			Assert::IsFalse(QueryValidator::isValidRelationArguments(proc_s, r, args1));
			Assert::IsFalse(QueryValidator::isValidRelationArguments(proc_s, r, args2));
			Assert::IsFalse(QueryValidator::isValidRelationArguments(proc_s, r, args3));
			Assert::IsFalse(QueryValidator::isValidRelationArguments(proc_s, r, args4));
			Assert::IsFalse(QueryValidator::isValidRelationArguments(proc_s, r, args5));
			Assert::IsFalse(QueryValidator::isValidRelationArguments(proc_s, r, args6));
			Assert::IsFalse(QueryValidator::isValidRelationArguments(proc_s, r, args7));
		}

		TEST_METHOD(isValidRelationArguments_CallsT_Valid_Success) {
			PROCESSED_SYNONYMS proc_s;

			QueryNode procedure_p1 = QueryNode();
			procedure_p1.setSynonymNode("procedure", "p1");
			proc_s.insert({ "p1", procedure_p1 });

			QueryNode procedure_p2 = QueryNode();
			procedure_p2.setSynonymNode("procedure", "p2");
			proc_s.insert({ "p2", procedure_p2 });

			RELATIONSHIP r = "Calls*";

			SINGLE_ARGUMENT arg1_1 = "p1";
			SINGLE_ARGUMENT arg1_2 = "p2";

			SINGLE_ARGUMENT arg2_1 = "_";
			SINGLE_ARGUMENT arg2_2 = "p2";

			SINGLE_ARGUMENT arg3_1 = "\"identity\"";
			SINGLE_ARGUMENT arg3_2 = "p2";

			SINGLE_ARGUMENT arg4_1 = "p1";
			SINGLE_ARGUMENT arg4_2 = "_";

			SINGLE_ARGUMENT arg5_1 = "p1";
			SINGLE_ARGUMENT arg5_2 = "\"identity\"";

			ARGUMENTS args1;
			args1.push_back(arg1_1);
			args1.push_back(arg1_2);

			ARGUMENTS args2;
			args2.push_back(arg2_1);
			args2.push_back(arg2_2);

			ARGUMENTS args3;
			args3.push_back(arg3_1);
			args3.push_back(arg3_2);

			ARGUMENTS args4;
			args4.push_back(arg4_1);
			args4.push_back(arg4_2);

			ARGUMENTS args5;
			args5.push_back(arg5_1);
			args5.push_back(arg5_2);

			Assert::IsTrue(QueryValidator::isValidRelationArguments(proc_s, r, args1));
			Assert::IsTrue(QueryValidator::isValidRelationArguments(proc_s, r, args2));
			Assert::IsTrue(QueryValidator::isValidRelationArguments(proc_s, r, args3));
			Assert::IsTrue(QueryValidator::isValidRelationArguments(proc_s, r, args4));
			Assert::IsTrue(QueryValidator::isValidRelationArguments(proc_s, r, args5));
		}

		TEST_METHOD(isValidRelationArguments_CallsT_Invalid_Success) {
			PROCESSED_SYNONYMS proc_s;

			QueryNode var_v = QueryNode();
			var_v.setSynonymNode("variable", "v");
			proc_s.insert({ "v", var_v });

			QueryNode assign_a = QueryNode();
			assign_a.setSynonymNode("assign", "a");
			proc_s.insert({ "a", assign_a });

			QueryNode procedure_p = QueryNode();
			procedure_p.setSynonymNode("procedure", "p");
			proc_s.insert({ "p", procedure_p });

			RELATIONSHIP r = "Calls*";

			SINGLE_ARGUMENT arg1_1 = "undeclared";
			SINGLE_ARGUMENT arg1_2 = "p";

			SINGLE_ARGUMENT arg2_1 = "1";
			SINGLE_ARGUMENT arg2_2 = "p";

			SINGLE_ARGUMENT arg3_1 = "a";
			SINGLE_ARGUMENT arg3_2 = "p";

			SINGLE_ARGUMENT arg4_1 = "v";
			SINGLE_ARGUMENT arg4_2 = "p";

			SINGLE_ARGUMENT arg5_1 = "p";
			SINGLE_ARGUMENT arg5_2 = "1";

			SINGLE_ARGUMENT arg6_1 = "p";
			SINGLE_ARGUMENT arg6_2 = "a";

			SINGLE_ARGUMENT arg7_1 = "p";
			SINGLE_ARGUMENT arg7_2 = "v";

			ARGUMENTS args1;
			args1.push_back(arg1_1);
			args1.push_back(arg1_2);

			ARGUMENTS args2;
			args2.push_back(arg2_1);
			args2.push_back(arg2_2);

			ARGUMENTS args3;
			args3.push_back(arg3_1);
			args3.push_back(arg3_2);

			ARGUMENTS args4;
			args4.push_back(arg4_1);
			args4.push_back(arg4_2);

			ARGUMENTS args5;
			args5.push_back(arg5_1);
			args5.push_back(arg5_2);

			ARGUMENTS args6;
			args6.push_back(arg6_1);
			args6.push_back(arg6_2);

			ARGUMENTS args7;
			args7.push_back(arg7_1);
			args7.push_back(arg7_2);

			Assert::IsFalse(QueryValidator::isValidRelationArguments(proc_s, r, args1));
			Assert::IsFalse(QueryValidator::isValidRelationArguments(proc_s, r, args2));
			Assert::IsFalse(QueryValidator::isValidRelationArguments(proc_s, r, args3));
			Assert::IsFalse(QueryValidator::isValidRelationArguments(proc_s, r, args4));
			Assert::IsFalse(QueryValidator::isValidRelationArguments(proc_s, r, args5));
			Assert::IsFalse(QueryValidator::isValidRelationArguments(proc_s, r, args6));
			Assert::IsFalse(QueryValidator::isValidRelationArguments(proc_s, r, args7));
		}

		TEST_METHOD(isValidRelationArguments_Next_Valid_Success) {
			PROCESSED_SYNONYMS proc_s;

			QueryNode prog_pl1 = QueryNode();
			prog_pl1.setSynonymNode("prog_line", "pl1");
			proc_s.insert({ "pl1", prog_pl1 });

			QueryNode prog_pl2 = QueryNode();
			prog_pl2.setSynonymNode("prog_line", "pl2");
			proc_s.insert({ "pl2", prog_pl2 });

			RELATIONSHIP r = "Next";

			SINGLE_ARGUMENT arg1_1 = "pl1";
			SINGLE_ARGUMENT arg1_2 = "pl2";

			SINGLE_ARGUMENT arg2_1 = "_";
			SINGLE_ARGUMENT arg2_2 = "pl2";

			SINGLE_ARGUMENT arg3_1 = "1";
			SINGLE_ARGUMENT arg3_2 = "pl2";

			SINGLE_ARGUMENT arg4_1 = "pl1";
			SINGLE_ARGUMENT arg4_2 = "_";

			SINGLE_ARGUMENT arg5_1 = "pl1";
			SINGLE_ARGUMENT arg5_2 = "1";

			ARGUMENTS args1;
			args1.push_back(arg1_1);
			args1.push_back(arg1_2);

			ARGUMENTS args2;
			args2.push_back(arg2_1);
			args2.push_back(arg2_2);

			ARGUMENTS args3;
			args3.push_back(arg3_1);
			args3.push_back(arg3_2);

			ARGUMENTS args4;
			args4.push_back(arg4_1);
			args4.push_back(arg4_2);

			ARGUMENTS args5;
			args5.push_back(arg5_1);
			args5.push_back(arg5_2);

			Assert::IsTrue(QueryValidator::isValidRelationArguments(proc_s, r, args1));
			Assert::IsTrue(QueryValidator::isValidRelationArguments(proc_s, r, args2));
			Assert::IsTrue(QueryValidator::isValidRelationArguments(proc_s, r, args3));
			Assert::IsTrue(QueryValidator::isValidRelationArguments(proc_s, r, args4));
			Assert::IsTrue(QueryValidator::isValidRelationArguments(proc_s, r, args5));
		}

		TEST_METHOD(isValidRelationArguments_Next_Invalid_Success) {
			PROCESSED_SYNONYMS proc_s;

			QueryNode prog_pl = QueryNode();
			prog_pl.setSynonymNode("prog_line", "pl");
			proc_s.insert({ "pl", prog_pl });

			QueryNode var_v = QueryNode();
			var_v.setSynonymNode("variable", "v");
			proc_s.insert({ "v", var_v });

			RELATIONSHIP r = "Next";

			SINGLE_ARGUMENT arg1_1 = "undeclared";
			SINGLE_ARGUMENT arg1_2 = "pl";

			SINGLE_ARGUMENT arg2_1 = "\"identity\"";
			SINGLE_ARGUMENT arg2_2 = "pl";

			SINGLE_ARGUMENT arg3_1 = "v";
			SINGLE_ARGUMENT arg3_2 = "pl";

			SINGLE_ARGUMENT arg4_1 = "pl";
			SINGLE_ARGUMENT arg4_2 = "\"identity\"";

			SINGLE_ARGUMENT arg5_1 = "pl";
			SINGLE_ARGUMENT arg5_2 = "v";

			ARGUMENTS args1;
			args1.push_back(arg1_1);
			args1.push_back(arg1_2);

			ARGUMENTS args2;
			args2.push_back(arg2_1);
			args2.push_back(arg2_2);

			ARGUMENTS args3;
			args3.push_back(arg3_1);
			args3.push_back(arg3_2);

			ARGUMENTS args4;
			args4.push_back(arg4_1);
			args4.push_back(arg4_2);

			ARGUMENTS args5;
			args5.push_back(arg5_1);
			args5.push_back(arg5_2);

			Assert::IsFalse(QueryValidator::isValidRelationArguments(proc_s, r, args1));
			Assert::IsFalse(QueryValidator::isValidRelationArguments(proc_s, r, args2));
			Assert::IsFalse(QueryValidator::isValidRelationArguments(proc_s, r, args3));
			Assert::IsFalse(QueryValidator::isValidRelationArguments(proc_s, r, args4));
			Assert::IsFalse(QueryValidator::isValidRelationArguments(proc_s, r, args5));
		}

		TEST_METHOD(isValidRelationArguments_NextT_Valid_Success) {
			PROCESSED_SYNONYMS proc_s;

			QueryNode prog_pl1 = QueryNode();
			prog_pl1.setSynonymNode("prog_line", "pl1");
			proc_s.insert({ "pl1", prog_pl1 });

			QueryNode prog_pl2 = QueryNode();
			prog_pl2.setSynonymNode("prog_line", "pl2");
			proc_s.insert({ "pl2", prog_pl2 });

			RELATIONSHIP r = "Next*";

			SINGLE_ARGUMENT arg1_1 = "pl1";
			SINGLE_ARGUMENT arg1_2 = "pl2";

			SINGLE_ARGUMENT arg2_1 = "_";
			SINGLE_ARGUMENT arg2_2 = "pl2";

			SINGLE_ARGUMENT arg3_1 = "1";
			SINGLE_ARGUMENT arg3_2 = "pl2";

			SINGLE_ARGUMENT arg4_1 = "pl1";
			SINGLE_ARGUMENT arg4_2 = "_";

			SINGLE_ARGUMENT arg5_1 = "pl1";
			SINGLE_ARGUMENT arg5_2 = "1";

			ARGUMENTS args1;
			args1.push_back(arg1_1);
			args1.push_back(arg1_2);

			ARGUMENTS args2;
			args2.push_back(arg2_1);
			args2.push_back(arg2_2);

			ARGUMENTS args3;
			args3.push_back(arg3_1);
			args3.push_back(arg3_2);

			ARGUMENTS args4;
			args4.push_back(arg4_1);
			args4.push_back(arg4_2);

			ARGUMENTS args5;
			args5.push_back(arg5_1);
			args5.push_back(arg5_2);

			Assert::IsTrue(QueryValidator::isValidRelationArguments(proc_s, r, args1));
			Assert::IsTrue(QueryValidator::isValidRelationArguments(proc_s, r, args2));
			Assert::IsTrue(QueryValidator::isValidRelationArguments(proc_s, r, args3));
			Assert::IsTrue(QueryValidator::isValidRelationArguments(proc_s, r, args4));
			Assert::IsTrue(QueryValidator::isValidRelationArguments(proc_s, r, args5));
		}

		TEST_METHOD(isValidRelationArguments_NextT_Invalid_Success) {
			PROCESSED_SYNONYMS proc_s;

			QueryNode prog_pl = QueryNode();
			prog_pl.setSynonymNode("prog_line", "pl");
			proc_s.insert({ "pl", prog_pl });

			QueryNode var_v = QueryNode();
			var_v.setSynonymNode("variable", "v");
			proc_s.insert({ "v", var_v });

			RELATIONSHIP r = "Next*";

			SINGLE_ARGUMENT arg1_1 = "undeclared";
			SINGLE_ARGUMENT arg1_2 = "pl";

			SINGLE_ARGUMENT arg2_1 = "\"identity\"";
			SINGLE_ARGUMENT arg2_2 = "pl";

			SINGLE_ARGUMENT arg3_1 = "v";
			SINGLE_ARGUMENT arg3_2 = "pl";

			SINGLE_ARGUMENT arg4_1 = "pl";
			SINGLE_ARGUMENT arg4_2 = "\"identity\"";

			SINGLE_ARGUMENT arg5_1 = "pl";
			SINGLE_ARGUMENT arg5_2 = "v";

			ARGUMENTS args1;
			args1.push_back(arg1_1);
			args1.push_back(arg1_2);

			ARGUMENTS args2;
			args2.push_back(arg2_1);
			args2.push_back(arg2_2);

			ARGUMENTS args3;
			args3.push_back(arg3_1);
			args3.push_back(arg3_2);

			ARGUMENTS args4;
			args4.push_back(arg4_1);
			args4.push_back(arg4_2);

			ARGUMENTS args5;
			args5.push_back(arg5_1);
			args5.push_back(arg5_2);

			Assert::IsFalse(QueryValidator::isValidRelationArguments(proc_s, r, args1));
			Assert::IsFalse(QueryValidator::isValidRelationArguments(proc_s, r, args2));
			Assert::IsFalse(QueryValidator::isValidRelationArguments(proc_s, r, args3));
			Assert::IsFalse(QueryValidator::isValidRelationArguments(proc_s, r, args4));
			Assert::IsFalse(QueryValidator::isValidRelationArguments(proc_s, r, args5));
		}

		TEST_METHOD(isValidRelationArguments_Affects_Valid_Success) {
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

			QueryNode prog_pl = QueryNode();
			prog_pl.setSynonymNode("prog_line", "pl");
			proc_s.insert({ "pl", prog_pl });

			RELATIONSHIP r = "Affects";

			SINGLE_ARGUMENT arg1_1 = "1";
			SINGLE_ARGUMENT arg1_2 = "2";

			SINGLE_ARGUMENT arg2_1 = "1";
			SINGLE_ARGUMENT arg2_2 = "_";

			SINGLE_ARGUMENT arg3_1 = "1";
			SINGLE_ARGUMENT arg3_2 = "s";

			SINGLE_ARGUMENT arg4_1 = "1";
			SINGLE_ARGUMENT arg4_2 = "r";

			SINGLE_ARGUMENT arg5_1 = "1";
			SINGLE_ARGUMENT arg5_2 = "pn";

			SINGLE_ARGUMENT arg6_1 = "1";
			SINGLE_ARGUMENT arg6_2 = "w";

			SINGLE_ARGUMENT arg7_1 = "1";
			SINGLE_ARGUMENT arg7_2 = "ifs";

			SINGLE_ARGUMENT arg8_1 = "1";
			SINGLE_ARGUMENT arg8_2 = "a";

			SINGLE_ARGUMENT arg9_1 = "s";
			SINGLE_ARGUMENT arg9_2 = "a";

			SINGLE_ARGUMENT arg10_1 = "1";
			SINGLE_ARGUMENT arg10_2 = "pl";

			ARGUMENTS args1;
			args1.push_back(arg1_1);
			args1.push_back(arg1_2);

			ARGUMENTS args2;
			args2.push_back(arg2_1);
			args2.push_back(arg2_2);

			ARGUMENTS args3;
			args3.push_back(arg3_1);
			args3.push_back(arg3_2);

			ARGUMENTS args4;
			args4.push_back(arg4_1);
			args4.push_back(arg4_2);

			ARGUMENTS args5;
			args5.push_back(arg5_1);
			args5.push_back(arg5_2);

			ARGUMENTS args6;
			args6.push_back(arg6_1);
			args6.push_back(arg6_2);

			ARGUMENTS args7;
			args7.push_back(arg7_1);
			args7.push_back(arg7_2);

			ARGUMENTS args8;
			args8.push_back(arg8_1);
			args8.push_back(arg8_2);

			ARGUMENTS args9;
			args9.push_back(arg9_1);
			args9.push_back(arg9_2);

			ARGUMENTS args10;
			args10.push_back(arg10_1);
			args10.push_back(arg10_2);

			Assert::IsTrue(QueryValidator::isValidRelationArguments(proc_s, r, args1));
			Assert::IsTrue(QueryValidator::isValidRelationArguments(proc_s, r, args2));
			Assert::IsTrue(QueryValidator::isValidRelationArguments(proc_s, r, args3));
			Assert::IsTrue(QueryValidator::isValidRelationArguments(proc_s, r, args4));
			Assert::IsTrue(QueryValidator::isValidRelationArguments(proc_s, r, args5));
			Assert::IsTrue(QueryValidator::isValidRelationArguments(proc_s, r, args6));
			Assert::IsTrue(QueryValidator::isValidRelationArguments(proc_s, r, args7));
			Assert::IsTrue(QueryValidator::isValidRelationArguments(proc_s, r, args8));
			Assert::IsTrue(QueryValidator::isValidRelationArguments(proc_s, r, args9));
			Assert::IsTrue(QueryValidator::isValidRelationArguments(proc_s, r, args10));
		}

		TEST_METHOD(isValidRelationArguments_Affects_Invalid_Success) {
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

			RELATIONSHIP r = "Affects";

			SINGLE_ARGUMENT arg1_1 = "1";
			SINGLE_ARGUMENT arg1_2 = "undeclared";

			SINGLE_ARGUMENT arg2_1 = "1";
			SINGLE_ARGUMENT arg2_2 = "v";

			SINGLE_ARGUMENT arg3_1 = "1";
			SINGLE_ARGUMENT arg3_2 = "c";

			SINGLE_ARGUMENT arg4_1 = "1";
			SINGLE_ARGUMENT arg4_2 = "p";

			SINGLE_ARGUMENT arg5_1 = "v";
			SINGLE_ARGUMENT arg5_2 = "1";

			SINGLE_ARGUMENT arg6_1 = "c";
			SINGLE_ARGUMENT arg6_2 = "1";

			SINGLE_ARGUMENT arg7_1 = "p";
			SINGLE_ARGUMENT arg7_2 = "1";

			ARGUMENTS args1;
			args1.push_back(arg1_1);
			args1.push_back(arg1_2);

			ARGUMENTS args2;
			args2.push_back(arg2_1);
			args2.push_back(arg2_2);

			ARGUMENTS args3;
			args3.push_back(arg3_1);
			args3.push_back(arg3_2);

			ARGUMENTS args4;
			args4.push_back(arg4_1);
			args4.push_back(arg4_2);

			ARGUMENTS args5;
			args5.push_back(arg5_1);
			args5.push_back(arg5_2);

			ARGUMENTS args6;
			args6.push_back(arg6_1);
			args6.push_back(arg6_2);

			ARGUMENTS args7;
			args7.push_back(arg7_1);
			args7.push_back(arg7_2);

			Assert::IsFalse(QueryValidator::isValidRelationArguments(proc_s, r, args1));
			Assert::IsFalse(QueryValidator::isValidRelationArguments(proc_s, r, args2));
			Assert::IsFalse(QueryValidator::isValidRelationArguments(proc_s, r, args3));
			Assert::IsFalse(QueryValidator::isValidRelationArguments(proc_s, r, args4));
			Assert::IsFalse(QueryValidator::isValidRelationArguments(proc_s, r, args5));
			Assert::IsFalse(QueryValidator::isValidRelationArguments(proc_s, r, args6));
			Assert::IsFalse(QueryValidator::isValidRelationArguments(proc_s, r, args7));
		}

		TEST_METHOD(isValidRelationArguments_AffectsT_Valid_Success) {
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

			QueryNode prog_pl = QueryNode();
			prog_pl.setSynonymNode("prog_line", "pl");
			proc_s.insert({ "pl", prog_pl });

			RELATIONSHIP r = "Affects*";

			SINGLE_ARGUMENT arg1_1 = "1";
			SINGLE_ARGUMENT arg1_2 = "2";

			SINGLE_ARGUMENT arg2_1 = "1";
			SINGLE_ARGUMENT arg2_2 = "_";

			SINGLE_ARGUMENT arg3_1 = "1";
			SINGLE_ARGUMENT arg3_2 = "s";

			SINGLE_ARGUMENT arg4_1 = "1";
			SINGLE_ARGUMENT arg4_2 = "r";

			SINGLE_ARGUMENT arg5_1 = "1";
			SINGLE_ARGUMENT arg5_2 = "pn";

			SINGLE_ARGUMENT arg6_1 = "1";
			SINGLE_ARGUMENT arg6_2 = "w";

			SINGLE_ARGUMENT arg7_1 = "1";
			SINGLE_ARGUMENT arg7_2 = "ifs";

			SINGLE_ARGUMENT arg8_1 = "1";
			SINGLE_ARGUMENT arg8_2 = "a";

			SINGLE_ARGUMENT arg9_1 = "s";
			SINGLE_ARGUMENT arg9_2 = "a";

			SINGLE_ARGUMENT arg10_1 = "1";
			SINGLE_ARGUMENT arg10_2 = "pl";

			ARGUMENTS args1;
			args1.push_back(arg1_1);
			args1.push_back(arg1_2);

			ARGUMENTS args2;
			args2.push_back(arg2_1);
			args2.push_back(arg2_2);

			ARGUMENTS args3;
			args3.push_back(arg3_1);
			args3.push_back(arg3_2);

			ARGUMENTS args4;
			args4.push_back(arg4_1);
			args4.push_back(arg4_2);

			ARGUMENTS args5;
			args5.push_back(arg5_1);
			args5.push_back(arg5_2);

			ARGUMENTS args6;
			args6.push_back(arg6_1);
			args6.push_back(arg6_2);

			ARGUMENTS args7;
			args7.push_back(arg7_1);
			args7.push_back(arg7_2);

			ARGUMENTS args8;
			args8.push_back(arg8_1);
			args8.push_back(arg8_2);

			ARGUMENTS args9;
			args9.push_back(arg9_1);
			args9.push_back(arg9_2);

			ARGUMENTS args10;
			args10.push_back(arg10_1);
			args10.push_back(arg10_2);

			Assert::IsTrue(QueryValidator::isValidRelationArguments(proc_s, r, args1));
			Assert::IsTrue(QueryValidator::isValidRelationArguments(proc_s, r, args2));
			Assert::IsTrue(QueryValidator::isValidRelationArguments(proc_s, r, args3));
			Assert::IsTrue(QueryValidator::isValidRelationArguments(proc_s, r, args4));
			Assert::IsTrue(QueryValidator::isValidRelationArguments(proc_s, r, args5));
			Assert::IsTrue(QueryValidator::isValidRelationArguments(proc_s, r, args6));
			Assert::IsTrue(QueryValidator::isValidRelationArguments(proc_s, r, args7));
			Assert::IsTrue(QueryValidator::isValidRelationArguments(proc_s, r, args8));
			Assert::IsTrue(QueryValidator::isValidRelationArguments(proc_s, r, args9));
			Assert::IsTrue(QueryValidator::isValidRelationArguments(proc_s, r, args10));
		}

		TEST_METHOD(isValidRelationArguments_AffectsT_Invalid_Success) {
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

			RELATIONSHIP r = "Affects*";

			SINGLE_ARGUMENT arg1_1 = "1";
			SINGLE_ARGUMENT arg1_2 = "undeclared";

			SINGLE_ARGUMENT arg2_1 = "1";
			SINGLE_ARGUMENT arg2_2 = "v";

			SINGLE_ARGUMENT arg3_1 = "1";
			SINGLE_ARGUMENT arg3_2 = "c";

			SINGLE_ARGUMENT arg4_1 = "1";
			SINGLE_ARGUMENT arg4_2 = "p";

			SINGLE_ARGUMENT arg5_1 = "v";
			SINGLE_ARGUMENT arg5_2 = "1";

			SINGLE_ARGUMENT arg6_1 = "c";
			SINGLE_ARGUMENT arg6_2 = "1";

			SINGLE_ARGUMENT arg7_1 = "p";
			SINGLE_ARGUMENT arg7_2 = "1";

			ARGUMENTS args1;
			args1.push_back(arg1_1);
			args1.push_back(arg1_2);

			ARGUMENTS args2;
			args2.push_back(arg2_1);
			args2.push_back(arg2_2);

			ARGUMENTS args3;
			args3.push_back(arg3_1);
			args3.push_back(arg3_2);

			ARGUMENTS args4;
			args4.push_back(arg4_1);
			args4.push_back(arg4_2);

			ARGUMENTS args5;
			args5.push_back(arg5_1);
			args5.push_back(arg5_2);

			ARGUMENTS args6;
			args6.push_back(arg6_1);
			args6.push_back(arg6_2);

			ARGUMENTS args7;
			args7.push_back(arg7_1);
			args7.push_back(arg7_2);

			Assert::IsFalse(QueryValidator::isValidRelationArguments(proc_s, r, args1));
			Assert::IsFalse(QueryValidator::isValidRelationArguments(proc_s, r, args2));
			Assert::IsFalse(QueryValidator::isValidRelationArguments(proc_s, r, args3));
			Assert::IsFalse(QueryValidator::isValidRelationArguments(proc_s, r, args4));
			Assert::IsFalse(QueryValidator::isValidRelationArguments(proc_s, r, args5));
			Assert::IsFalse(QueryValidator::isValidRelationArguments(proc_s, r, args6));
			Assert::IsFalse(QueryValidator::isValidRelationArguments(proc_s, r, args7));
		}

		TEST_METHOD(isValidPatternFormat_Valid_Success) {
			SINGLE_CLAUSE p1 = "a(_, _)";
			SINGLE_CLAUSE p2 = "a(a, _)";
			SINGLE_CLAUSE p3 = "a(\"identity\", _)";
			SINGLE_CLAUSE p4 = "a(_, _\"x\"_)";
			SINGLE_CLAUSE p5 = "a(_, _\"1\"_)";
			SINGLE_CLAUSE p6 = "a ( _    , _   )";
			SINGLE_CLAUSE p7 = "if( _, _, _)";

			Assert::IsTrue(QueryValidator::isValidPatternFormat(p1));
			Assert::IsTrue(QueryValidator::isValidPatternFormat(p2));
			Assert::IsTrue(QueryValidator::isValidPatternFormat(p3));
			Assert::IsTrue(QueryValidator::isValidPatternFormat(p4));
			Assert::IsTrue(QueryValidator::isValidPatternFormat(p5));
			Assert::IsTrue(QueryValidator::isValidPatternFormat(p6));
			Assert::IsTrue(QueryValidator::isValidPatternFormat(p7));
		}

		TEST_METHOD(isValidPatternFormat_Invalid_Success) {
			SINGLE_CLAUSE p1 = "(_, _)";
			SINGLE_CLAUSE p2 = "1(_, _)";
			SINGLE_CLAUSE p3 = "a(1, _)";
			SINGLE_CLAUSE p4 = "a(_, 1)";
			SINGLE_CLAUSE p5 = "a(_, v)";
			SINGLE_CLAUSE p6 = "a(_, _\"x\")";
			SINGLE_CLAUSE p7 = "a(_, \"x\"_)";
			SINGLE_CLAUSE p8 = "a(_\"x\"_, _)";
			SINGLE_CLAUSE p9 = "if(_, _, 1)";

			Assert::IsFalse(QueryValidator::isValidPatternFormat(p1));
			Assert::IsFalse(QueryValidator::isValidPatternFormat(p2));
			Assert::IsFalse(QueryValidator::isValidPatternFormat(p3));
			Assert::IsFalse(QueryValidator::isValidPatternFormat(p4));
			Assert::IsFalse(QueryValidator::isValidPatternFormat(p5));
			Assert::IsFalse(QueryValidator::isValidPatternFormat(p6));
			Assert::IsFalse(QueryValidator::isValidPatternFormat(p7));
			Assert::IsFalse(QueryValidator::isValidPatternFormat(p8));
			Assert::IsFalse(QueryValidator::isValidPatternFormat(p9));
		}

		TEST_METHOD(isValidPatternArguments_Assign_Valid_Success) {
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

			SINGLE_ARGUMENT arg1_1 = "_";
			SINGLE_ARGUMENT arg1_2 = "_";

			SINGLE_ARGUMENT arg2_1 = "v";
			SINGLE_ARGUMENT arg2_2 = "_";

			SINGLE_ARGUMENT arg3_1 = "\"identity\"";
			SINGLE_ARGUMENT arg3_2 = "_";

			SINGLE_ARGUMENT arg4_1 = "_";
			SINGLE_ARGUMENT arg4_2 = "_\"x\"_";

			SINGLE_ARGUMENT arg5_1 = "_";
			SINGLE_ARGUMENT arg5_2 = "_\"1\"_";

			SINGLE_ARGUMENT arg6_1 = "_";
			SINGLE_ARGUMENT arg6_2 = "\"1\"";

			SINGLE_ARGUMENT arg7_1 = "_";
			SINGLE_ARGUMENT arg7_2 = "\"x\"";

			SINGLE_ARGUMENT arg8_1 = "_";
			SINGLE_ARGUMENT arg8_2 = "\"3 + b / (c - d)\"";

			ARGUMENTS args1;
			args1.push_back(arg1_1);
			args1.push_back(arg1_2);

			ARGUMENTS args2;
			args2.push_back(arg2_1);
			args2.push_back(arg2_2);

			ARGUMENTS args3;
			args3.push_back(arg3_1);
			args3.push_back(arg3_2);

			ARGUMENTS args4;
			args4.push_back(arg4_1);
			args4.push_back(arg4_2);

			ARGUMENTS args5;
			args5.push_back(arg5_1);
			args5.push_back(arg5_2);

			ARGUMENTS args6;
			args6.push_back(arg6_1);
			args6.push_back(arg6_2);

			ARGUMENTS args7;
			args7.push_back(arg7_1);
			args7.push_back(arg7_2);

			ARGUMENTS args8;
			args8.push_back(arg8_1);
			args8.push_back(arg8_2);

			Assert::IsTrue(QueryValidator::isValidPatternArguments(proc_s, syn, args1));
			Assert::IsTrue(QueryValidator::isValidPatternArguments(proc_s, syn, args2));
			Assert::IsTrue(QueryValidator::isValidPatternArguments(proc_s, syn, args3));
			Assert::IsTrue(QueryValidator::isValidPatternArguments(proc_s, syn, args4));
			Assert::IsTrue(QueryValidator::isValidPatternArguments(proc_s, syn, args5));
			Assert::IsTrue(QueryValidator::isValidPatternArguments(proc_s, syn, args6));
			Assert::IsTrue(QueryValidator::isValidPatternArguments(proc_s, syn, args7));
			Assert::IsTrue(QueryValidator::isValidPatternArguments(proc_s, syn, args8));
		}

		TEST_METHOD(isValidPatternArguments_If_Valid_Success) {
			PROCESSED_SYNONYMS proc_s;

			QueryNode stmt_s = QueryNode();
			stmt_s.setSynonymNode("stmt", "s");
			proc_s.insert({ "s", stmt_s });

			QueryNode var_v = QueryNode();
			var_v.setSynonymNode("variable", "v");
			proc_s.insert({ "v", var_v });

			QueryNode if_ifs = QueryNode();
			if_ifs.setSynonymNode("if", "ifs");
			proc_s.insert({ "ifs", if_ifs });

			SYNONYM_NAME syn = "ifs";

			SINGLE_ARGUMENT arg1_1 = "_";
			SINGLE_ARGUMENT arg1_2 = "_";
			SINGLE_ARGUMENT arg1_3 = "_";

			SINGLE_ARGUMENT arg2_1 = "v";
			SINGLE_ARGUMENT arg2_2 = "_";
			SINGLE_ARGUMENT arg2_3 = "_";

			SINGLE_ARGUMENT arg3_1 = "\"identity\"";
			SINGLE_ARGUMENT arg3_2 = "_";
			SINGLE_ARGUMENT arg3_3 = "_";

			ARGUMENTS args1;
			args1.push_back(arg1_1);
			args1.push_back(arg1_2);
			args1.push_back(arg1_3);

			ARGUMENTS args2;
			args2.push_back(arg2_1);
			args2.push_back(arg2_2);
			args2.push_back(arg2_3);

			ARGUMENTS args3;
			args3.push_back(arg3_1);
			args3.push_back(arg3_2);
			args3.push_back(arg3_3);

			Assert::IsTrue(QueryValidator::isValidPatternArguments(proc_s, syn, args1));
			Assert::IsTrue(QueryValidator::isValidPatternArguments(proc_s, syn, args2));
			Assert::IsTrue(QueryValidator::isValidPatternArguments(proc_s, syn, args3));
		}

		TEST_METHOD(isValidPatternArguments_While_Valid_Success) {
			PROCESSED_SYNONYMS proc_s;

			QueryNode stmt_s = QueryNode();
			stmt_s.setSynonymNode("stmt", "s");
			proc_s.insert({ "s", stmt_s });

			QueryNode var_v = QueryNode();
			var_v.setSynonymNode("variable", "v");
			proc_s.insert({ "v", var_v });

			QueryNode while_w = QueryNode();
			while_w.setSynonymNode("while", "w");
			proc_s.insert({ "w", while_w });

			SYNONYM_NAME syn = "w";

			SINGLE_ARGUMENT arg1_1 = "_";
			SINGLE_ARGUMENT arg1_2 = "_";

			SINGLE_ARGUMENT arg2_1 = "v";
			SINGLE_ARGUMENT arg2_2 = "_";

			SINGLE_ARGUMENT arg3_1 = "\"identity\"";
			SINGLE_ARGUMENT arg3_2 = "_";

			ARGUMENTS args1;
			args1.push_back(arg1_1);
			args1.push_back(arg1_2);

			ARGUMENTS args2;
			args2.push_back(arg2_1);
			args2.push_back(arg2_2);

			ARGUMENTS args3;
			args3.push_back(arg3_1);
			args3.push_back(arg3_2);

			Assert::IsTrue(QueryValidator::isValidPatternArguments(proc_s, syn, args1));
			Assert::IsTrue(QueryValidator::isValidPatternArguments(proc_s, syn, args2));
			Assert::IsTrue(QueryValidator::isValidPatternArguments(proc_s, syn, args3));
		}

		TEST_METHOD(isValidPatternArguments_Assign_Invalid_Success) {
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

			SINGLE_ARGUMENT arg1_1 = "_";
			SINGLE_ARGUMENT arg1_2 = "_";

			SINGLE_ARGUMENT arg2_1 = "1";
			SINGLE_ARGUMENT arg2_2 = "_";

			SINGLE_ARGUMENT arg3_1 = "_";
			SINGLE_ARGUMENT arg3_2 = "1";

			SINGLE_ARGUMENT arg4_1 = "_";
			SINGLE_ARGUMENT arg4_2 = "v";

			SINGLE_ARGUMENT arg5_1 = "_\"x\"_";
			SINGLE_ARGUMENT arg5_2 = "_";

			ARGUMENTS args1;
			args1.push_back(arg1_1);
			args1.push_back(arg1_2);

			ARGUMENTS args2;
			args2.push_back(arg2_1);
			args2.push_back(arg2_2);

			ARGUMENTS args3;
			args3.push_back(arg3_1);
			args3.push_back(arg3_2);

			ARGUMENTS args4;
			args4.push_back(arg4_1);
			args4.push_back(arg4_2);

			ARGUMENTS args5;
			args5.push_back(arg5_1);
			args5.push_back(arg5_2);

			Assert::IsFalse(QueryValidator::isValidPatternArguments(proc_s, syn2, args1));
			Assert::IsFalse(QueryValidator::isValidPatternArguments(proc_s, syn1, args2));
			Assert::IsFalse(QueryValidator::isValidPatternArguments(proc_s, syn1, args3));
			Assert::IsFalse(QueryValidator::isValidPatternArguments(proc_s, syn1, args4));
			Assert::IsFalse(QueryValidator::isValidPatternArguments(proc_s, syn1, args5));
		}

		TEST_METHOD(isValidPatternArguments_If_Invalid_Success) {
			PROCESSED_SYNONYMS proc_s;

			QueryNode stmt_s = QueryNode();
			stmt_s.setSynonymNode("stmt", "s");
			proc_s.insert({ "s", stmt_s });

			QueryNode var_v = QueryNode();
			var_v.setSynonymNode("variable", "v");
			proc_s.insert({ "v", var_v });

			QueryNode if_ifs = QueryNode();
			if_ifs.setSynonymNode("if", "ifs");
			proc_s.insert({ "ifs", if_ifs });

			SYNONYM_NAME syn1 = "ifs";
			SYNONYM_NAME syn2 = "if1";

			SINGLE_ARGUMENT arg1_1 = "_";
			SINGLE_ARGUMENT arg1_2 = "_";
			SINGLE_ARGUMENT arg1_3 = "_";

			SINGLE_ARGUMENT arg2_1 = "_";
			SINGLE_ARGUMENT arg2_2 = "_";

			SINGLE_ARGUMENT arg3_1 = "_";
			SINGLE_ARGUMENT arg3_2 = "1";
			SINGLE_ARGUMENT arg3_3 = "_";

			SINGLE_ARGUMENT arg4_1 = "_";
			SINGLE_ARGUMENT arg4_2 = "_";
			SINGLE_ARGUMENT arg4_3 = "1";

			SINGLE_ARGUMENT arg5_1 = "_";
			SINGLE_ARGUMENT arg5_2 = "_\"x\"_";
			SINGLE_ARGUMENT arg5_3 = "_";

			SINGLE_ARGUMENT arg6_1 = "_";
			SINGLE_ARGUMENT arg6_2 = "v";
			SINGLE_ARGUMENT arg6_3 = "_";

			SINGLE_ARGUMENT arg7_1 = "_\"x\"_";
			SINGLE_ARGUMENT arg7_2 = "_";
			SINGLE_ARGUMENT arg7_3 = "_";

			ARGUMENTS args1;
			args1.push_back(arg1_1);
			args1.push_back(arg1_2);
			args1.push_back(arg1_3);

			ARGUMENTS args2;
			args2.push_back(arg2_1);
			args2.push_back(arg2_2);

			ARGUMENTS args3;
			args3.push_back(arg3_1);
			args3.push_back(arg3_2);
			args3.push_back(arg3_3);

			ARGUMENTS args4;
			args4.push_back(arg4_1);
			args4.push_back(arg4_2);
			args4.push_back(arg4_3);

			ARGUMENTS args5;
			args5.push_back(arg5_1);
			args5.push_back(arg5_2);
			args5.push_back(arg5_3);

			ARGUMENTS args6;
			args6.push_back(arg6_1);
			args6.push_back(arg6_2);
			args6.push_back(arg6_3);

			ARGUMENTS args7;
			args7.push_back(arg7_1);
			args7.push_back(arg7_2);
			args7.push_back(arg7_3);

			Assert::IsFalse(QueryValidator::isValidPatternArguments(proc_s, syn2, args1));
			Assert::IsFalse(QueryValidator::isValidPatternArguments(proc_s, syn1, args2));
			Assert::IsFalse(QueryValidator::isValidPatternArguments(proc_s, syn1, args3));
			Assert::IsFalse(QueryValidator::isValidPatternArguments(proc_s, syn1, args4));
			Assert::IsFalse(QueryValidator::isValidPatternArguments(proc_s, syn1, args5));
			Assert::IsFalse(QueryValidator::isValidPatternArguments(proc_s, syn1, args6));
			Assert::IsFalse(QueryValidator::isValidPatternArguments(proc_s, syn1, args7));
		}

		TEST_METHOD(isValidPatternArguments_While_Invalid_Success) {
			PROCESSED_SYNONYMS proc_s;

			QueryNode stmt_s = QueryNode();
			stmt_s.setSynonymNode("stmt", "s");
			proc_s.insert({ "s", stmt_s });

			QueryNode var_v = QueryNode();
			var_v.setSynonymNode("variable", "v");
			proc_s.insert({ "v", var_v });

			QueryNode while_w = QueryNode();
			while_w.setSynonymNode("while", "w");
			proc_s.insert({ "w", while_w });

			SYNONYM_NAME syn1 = "w";
			SYNONYM_NAME syn2 = "w1";

			SINGLE_ARGUMENT arg1_1 = "_";
			SINGLE_ARGUMENT arg1_2 = "_";

			SINGLE_ARGUMENT arg2_1 = "1";
			SINGLE_ARGUMENT arg2_2 = "_";

			SINGLE_ARGUMENT arg3_1 = "_";
			SINGLE_ARGUMENT arg3_2 = "1";

			SINGLE_ARGUMENT arg4_1 = "_\"x\"";
			SINGLE_ARGUMENT arg4_2 = "_";

			SINGLE_ARGUMENT arg5_1 = "_";
			SINGLE_ARGUMENT arg5_2 = "_\"x\"_";

			SINGLE_ARGUMENT arg6_1 = "_";
			SINGLE_ARGUMENT arg6_2 = "v";

			SINGLE_ARGUMENT arg7_1 = "_\"x\"_";
			SINGLE_ARGUMENT arg7_2 = "_";

			ARGUMENTS args1;
			args1.push_back(arg1_1);
			args1.push_back(arg1_2);

			ARGUMENTS args2;
			args2.push_back(arg2_1);
			args2.push_back(arg2_2);

			ARGUMENTS args3;
			args3.push_back(arg3_1);
			args3.push_back(arg3_2);

			ARGUMENTS args4;
			args4.push_back(arg4_1);
			args4.push_back(arg4_2);

			ARGUMENTS args5;
			args5.push_back(arg5_1);
			args5.push_back(arg5_2);

			ARGUMENTS args6;
			args6.push_back(arg6_1);
			args6.push_back(arg6_2);

			ARGUMENTS args7;
			args7.push_back(arg7_1);
			args7.push_back(arg7_2);

			Assert::IsFalse(QueryValidator::isValidPatternArguments(proc_s, syn2, args1));
			Assert::IsFalse(QueryValidator::isValidPatternArguments(proc_s, syn1, args2));
			Assert::IsFalse(QueryValidator::isValidPatternArguments(proc_s, syn1, args3));
			Assert::IsFalse(QueryValidator::isValidPatternArguments(proc_s, syn1, args4));
			Assert::IsFalse(QueryValidator::isValidPatternArguments(proc_s, syn1, args5));
			Assert::IsFalse(QueryValidator::isValidPatternArguments(proc_s, syn1, args6));
			Assert::IsFalse(QueryValidator::isValidPatternArguments(proc_s, syn1, args7));
		}

		TEST_METHOD(isValidPostfixExpr_Valid_Success) {
			POSTFIX_EXPR e1;
			POSTFIX_EXPR e2;
			POSTFIX_EXPR e3;
			POSTFIX_EXPR e4;
			POSTFIX_EXPR e5;

			e1.push_back("1");

			e2.push_back("v");

			e3.push_back("1");
			e3.push_back("v");
			e3.push_back("+");

			e4.push_back("1");
			e4.push_back("1");
			e4.push_back("2");
			e4.push_back("-");
			e4.push_back("*");

			e5.push_back("3");
			e5.push_back("bar");
			e5.push_back("c");
			e5.push_back("duck");
			e5.push_back("-");
			e5.push_back("/");
			e5.push_back("+");

			Assert::IsTrue(QueryValidator::isValidPostfixExpr(e1));
			Assert::IsTrue(QueryValidator::isValidPostfixExpr(e2));
			Assert::IsTrue(QueryValidator::isValidPostfixExpr(e3));
			Assert::IsTrue(QueryValidator::isValidPostfixExpr(e4));
			Assert::IsTrue(QueryValidator::isValidPostfixExpr(e5));
		}

		TEST_METHOD(isValidPostfixExpr_Invalid_Success) {
			POSTFIX_EXPR e1;
			POSTFIX_EXPR e2;
			POSTFIX_EXPR e3;

			e1.push_back("+");

			e2.push_back("1 +");

			e3.push_back("1");
			e3.push_back("v");

			Assert::IsFalse(QueryValidator::isValidPostfixExpr(e1));
			Assert::IsFalse(QueryValidator::isValidPostfixExpr(e2));
			Assert::IsFalse(QueryValidator::isValidPostfixExpr(e3));
		}

		TEST_METHOD(isValidWithFormat_Valid_Success) {
			SINGLE_CLAUSE p1 = "a = a";
			SINGLE_CLAUSE p2 = "12 = 12";
			SINGLE_CLAUSE p3 = "\"x\" = \"x\"";
			SINGLE_CLAUSE p4 = "p.procName = p.procName";
			SINGLE_CLAUSE p5 = "a = 12";
			SINGLE_CLAUSE p6 = "v = \"hello\"";
			SINGLE_CLAUSE p7 = "c.value = 1";

			Assert::IsTrue(QueryValidator::isValidWithFormat(p1));
			Assert::IsTrue(QueryValidator::isValidWithFormat(p2));
			Assert::IsTrue(QueryValidator::isValidWithFormat(p3));
			Assert::IsTrue(QueryValidator::isValidWithFormat(p4));
			Assert::IsTrue(QueryValidator::isValidWithFormat(p5));
			Assert::IsTrue(QueryValidator::isValidWithFormat(p6));
			Assert::IsTrue(QueryValidator::isValidWithFormat(p7));
		}

		TEST_METHOD(isValidWithFormat_Invalid_Success) {
			SINGLE_CLAUSE p1 = "a";
			SINGLE_CLAUSE p2 = "a=";
			SINGLE_CLAUSE p3 = "=1";
			SINGLE_CLAUSE p4 = "c.undeclared = a";
			SINGLE_CLAUSE p5 = "a = c.undeclared";
			SINGLE_CLAUSE p6 = "\"x + y\" = 1";
			SINGLE_CLAUSE p7 = "1 = \"x + y\"";
			SINGLE_CLAUSE p8 = "=";

			Assert::IsFalse(QueryValidator::isValidWithFormat(p1));
			Assert::IsFalse(QueryValidator::isValidWithFormat(p2));
			Assert::IsFalse(QueryValidator::isValidWithFormat(p3));
			Assert::IsFalse(QueryValidator::isValidWithFormat(p4));
			Assert::IsFalse(QueryValidator::isValidWithFormat(p5));
			Assert::IsFalse(QueryValidator::isValidWithFormat(p6));
			Assert::IsFalse(QueryValidator::isValidWithFormat(p7));
			Assert::IsFalse(QueryValidator::isValidWithFormat(p8));
		}

		TEST_METHOD(isValidWithArguments_Valid_Success) {
			PROCESSED_SYNONYMS proc_s;

			QueryNode stmt_s = QueryNode();
			stmt_s.setSynonymNode("stmt", "s");
			proc_s.insert({ "s", stmt_s });

			QueryNode procedure_p = QueryNode();
			procedure_p.setSynonymNode("procedure", "p");
			proc_s.insert({ "p", procedure_p });

			QueryNode variable_v = QueryNode();
			variable_v.setSynonymNode("variable", "v");
			proc_s.insert({ "v", variable_v });

			QueryNode constant_c = QueryNode();
			constant_c.setSynonymNode("constant", "c");
			proc_s.insert({ "c", constant_c });

			QueryNode call_cl = QueryNode();
			call_cl.setSynonymNode("call", "cl");
			proc_s.insert({ "cl", call_cl });

			QueryNode read_r = QueryNode();
			read_r.setSynonymNode("read", "r");
			proc_s.insert({ "r", read_r });

			QueryNode print_pr = QueryNode();
			print_pr.setSynonymNode("print", "pr");
			proc_s.insert({ "pr", print_pr });

			QueryNode while_w = QueryNode();
			while_w.setSynonymNode("while", "w");
			proc_s.insert({ "w", while_w });

			QueryNode if_ifs = QueryNode();
			if_ifs.setSynonymNode("if", "ifs");
			proc_s.insert({ "ifs", if_ifs });

			QueryNode assign_a = QueryNode();
			assign_a.setSynonymNode("assign", "a");
			proc_s.insert({ "a", assign_a });

			SINGLE_ARGUMENT arg1_1 = "a";
			SINGLE_ARGUMENT arg1_2 = "s";

			SINGLE_ARGUMENT arg2_1 = "1";
			SINGLE_ARGUMENT arg2_2 = "2";

			SINGLE_ARGUMENT arg3_1 = "\"identity\"";
			SINGLE_ARGUMENT arg3_2 = "\"test\"";

			SINGLE_ARGUMENT arg4_1 = "s.stmt#";
			SINGLE_ARGUMENT arg4_2 = "a.stmt#";

			SINGLE_ARGUMENT arg5_1 = "p.procName";
			SINGLE_ARGUMENT arg5_2 = "\"hello\"";

			SINGLE_ARGUMENT arg6_1 = "v";
			SINGLE_ARGUMENT arg6_2 = "\"variable\"";

			SINGLE_ARGUMENT arg7_1 = "c.value";
			SINGLE_ARGUMENT arg7_2 = "1";

			SINGLE_ARGUMENT arg8_1 = "1";
			SINGLE_ARGUMENT arg8_2 = "s";

			ARGUMENTS args1;
			args1.push_back(arg1_1);
			args1.push_back(arg1_2);

			ARGUMENTS args2;
			args2.push_back(arg2_1);
			args2.push_back(arg2_2);

			ARGUMENTS args3;
			args3.push_back(arg3_1);
			args3.push_back(arg3_2);

			ARGUMENTS args4;
			args4.push_back(arg4_1);
			args4.push_back(arg4_2);

			ARGUMENTS args5;
			args5.push_back(arg5_1);
			args5.push_back(arg5_2);

			ARGUMENTS args6;
			args6.push_back(arg6_1);
			args6.push_back(arg6_2);

			ARGUMENTS args7;
			args7.push_back(arg7_1);
			args7.push_back(arg7_2);

			ARGUMENTS args8;
			args8.push_back(arg8_1);
			args8.push_back(arg8_2);

			Assert::IsTrue(QueryValidator::isValidWithArguments(proc_s, args1));
			Assert::IsTrue(QueryValidator::isValidWithArguments(proc_s, args2));
			Assert::IsTrue(QueryValidator::isValidWithArguments(proc_s, args3));
			Assert::IsTrue(QueryValidator::isValidWithArguments(proc_s, args4));
			Assert::IsTrue(QueryValidator::isValidWithArguments(proc_s, args5));
			Assert::IsTrue(QueryValidator::isValidWithArguments(proc_s, args6));
			Assert::IsTrue(QueryValidator::isValidWithArguments(proc_s, args7));
			Assert::IsTrue(QueryValidator::isValidWithArguments(proc_s, args8));
		}

		TEST_METHOD(isValidWithArguments_Invalid_Success) {
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

			QueryNode procedure_p = QueryNode();
			procedure_p.setSynonymNode("procedure", "p");
			proc_s.insert({ "p", procedure_p });

			SINGLE_ARGUMENT arg1_1 = "_";
			SINGLE_ARGUMENT arg1_2 = "a";

			SINGLE_ARGUMENT arg2_1 = "1";
			SINGLE_ARGUMENT arg2_2 = "v";

			SINGLE_ARGUMENT arg3_1 = "p.varName";
			SINGLE_ARGUMENT arg3_2 = "\"test\"";

			SINGLE_ARGUMENT arg4_1 = "p.procName";
			SINGLE_ARGUMENT arg4_2 = "1";

			SINGLE_ARGUMENT arg5_1 = "\"hello\"";
			SINGLE_ARGUMENT arg5_2 = "1";

			SINGLE_ARGUMENT arg6_1 = "a";
			SINGLE_ARGUMENT arg6_2 = "v";

			ARGUMENTS args1;
			args1.push_back(arg1_1);
			args1.push_back(arg1_2);

			ARGUMENTS args2;
			args2.push_back(arg2_1);
			args2.push_back(arg2_2);

			ARGUMENTS args3;
			args3.push_back(arg3_1);
			args3.push_back(arg3_2);

			ARGUMENTS args4;
			args4.push_back(arg4_1);
			args4.push_back(arg4_2);

			ARGUMENTS args5;
			args5.push_back(arg5_1);
			args5.push_back(arg5_2);

			ARGUMENTS args6;
			args6.push_back(arg6_1);
			args6.push_back(arg6_2);

			Assert::IsFalse(QueryValidator::isValidWithArguments(proc_s, args1));
			Assert::IsFalse(QueryValidator::isValidWithArguments(proc_s, args2));
			Assert::IsFalse(QueryValidator::isValidWithArguments(proc_s, args3));
			Assert::IsFalse(QueryValidator::isValidWithArguments(proc_s, args4));
			Assert::IsFalse(QueryValidator::isValidWithArguments(proc_s, args5));
			Assert::IsFalse(QueryValidator::isValidWithArguments(proc_s, args6));
		}
	};
}