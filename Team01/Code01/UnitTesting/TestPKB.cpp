#include "stdafx.h"
#include "CppUnitTest.h"
#include "PKB.h"
#include "PKBBuilder.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
	
	// Tests PKBBuilder as well.
	TEST_CLASS(PKB_GetterSetterTest) {
	public:
		TEST_METHOD(addAssign_getAssigns_True) {
			PKBBuilder pkb_builder;
			ASSIGN_NODE_PTR expected = std::make_shared<AssignNode>();
			int expected_num = 1;
			expected->setStatementNumber(expected_num);
			pkb_builder.addAssignNode(expected);
			PKB pkb = pkb_builder.build();

			ASSIGN_NODE_PTR actual_pkb = pkb.getAssigns().at(0);
			ASSIGN_NODE_PTR actual_builder = pkb_builder.getAssigns().at(0);
			Assert::IsTrue(std::addressof(*expected) == std::addressof(*actual_pkb));
			Assert::IsTrue(std::addressof(*expected) == std::addressof(*actual_builder));
			
			int num = pkb.getAssignNumList().at(0);
			Assert::AreEqual(expected_num, num);;
		}
		TEST_METHOD(addCall_getCall_True) {
			PKBBuilder pkb_builder;
			CALL_NODE_PTR expected = std::make_shared<CallNode>();
			int expected_num = 1;
			PROC_NAME expected_proc_name = "main";
			expected->setCalleeProcedureName(expected_proc_name);
			expected->setStatementNumber(expected_num);
			pkb_builder.addCallNode(expected);
			PKB pkb = pkb_builder.build();

			CALL_NODE_PTR actual_pkb = pkb.getCalls().at(0);
			CALL_NODE_PTR actual_builder = pkb_builder.getCalls().at(0);
			Assert::IsTrue(std::addressof(*expected) == std::addressof(*actual_pkb));
			Assert::IsTrue(std::addressof(*expected) == std::addressof(*actual_builder));

			int num = pkb.getCallNumList().at(0);
			Assert::AreEqual(expected_num, num);

			std::string actual_var_name = pkb.getCallProcNameList().at(0);
			Assert::AreEqual(expected_proc_name, actual_var_name);
		}
		
		TEST_METHOD(addConstant_getConstants_True) {
			PKBBuilder pkb_builder;
			CONSTANT_NODE_PTR expected = std::make_shared<ConstantNode>();
			std::string expected_num = "1";
			expected->setValue(expected_num);
			pkb_builder.addConstantNode(expected);
			PKB pkb = pkb_builder.build();
			
			CONSTANT_NODE_PTR actual_pkb = pkb.getConstants().at(0);
			CONSTANT_NODE_PTR actual_builder = pkb_builder.getConstants().at(0);
			Assert::IsTrue(std::addressof(*expected) == std::addressof(*actual_pkb));
			Assert::IsTrue(std::addressof(*expected) == std::addressof(*actual_builder));
			
			std::string num = pkb.getConstantValueList().at(0);
			Assert::AreEqual(expected_num, num);
		}

		TEST_METHOD(addIf_getIfs_True) {
			PKBBuilder pkb_builder;
			IF_NODE_PTR expected = std::make_shared<IfNode>();
			int expected_num = 1;
			expected->setStatementNumber(expected_num);
			pkb_builder.addIfNode(expected);
			PKB pkb = pkb_builder.build();
			
			IF_NODE_PTR actual_pkb = pkb.getIfs().at(0);
			IF_NODE_PTR actual_builder = pkb_builder.getIfs().at(0);
			Assert::IsTrue(std::addressof(*expected) == std::addressof(*actual_pkb));
			Assert::IsTrue(std::addressof(*expected) == std::addressof(*actual_builder));
			
			int num = pkb.getIfNumList().at(0);
			Assert::AreEqual(expected_num, num);			
		}

		TEST_METHOD(addPrint_getPrints_True) {
			PKBBuilder pkb_builder;
			PRINT_NODE_PTR expected = std::make_shared<PrintNode>();
			VAR_NODE_PTR expected_var = std::make_shared<VariableNode>();
			int expected_num = 1;
			std::string expected_var_name = "x";
			expected_var->setVariableName(expected_var_name);
			expected->setStatementNumber(expected_num);
			expected->setVariableNode(expected_var);
			pkb_builder.addPrintNode(expected);
			pkb_builder.addVariableNode(expected_var);
			PKB pkb = pkb_builder.build();
			
			PRINT_NODE_PTR actual_pkb = pkb.getPrints().at(0);
			PRINT_NODE_PTR actual_builder = pkb_builder.getPrints().at(0);
			Assert::IsTrue(std::addressof(*expected) == std::addressof(*actual_pkb));
			Assert::IsTrue(std::addressof(*expected) == std::addressof(*actual_builder));
			
			int num = pkb.getPrintNumList().at(0);			
			Assert::AreEqual(expected_num, num);

			std::string actual_var_name = pkb.getPrintVarNameList().at(0);
			Assert::AreEqual(expected_var_name, actual_var_name);
		}

		TEST_METHOD(addProcedure_getProcedures_True) {
			PKBBuilder pkb_builder;
			PROC_NODE_PTR expected = std::make_shared<ProcedureNode>();
			std::string expected_name = "main";
			expected->setProcedureName(expected_name);
			pkb_builder.addProcedureNode(expected);
			PKB pkb = pkb_builder.build();
			
			PROC_NODE_PTR actual_pkb = pkb.getProcedures().at(0);
			PROC_NODE_PTR actual_builder = pkb_builder.getProcedures().at(0);
			Assert::IsTrue(std::addressof(*expected) == std::addressof(*actual_pkb));
			Assert::IsTrue(std::addressof(*expected) == std::addressof(*actual_builder));
			
			std::string name = pkb.getProcedureNameList().at(0);
			Assert::AreEqual(expected_name, name);			
		}

		TEST_METHOD(addRead_getReads_True) {
			PKBBuilder pkb_builder;
			READ_NODE_PTR expected = std::make_shared<ReadNode>();
			VAR_NODE_PTR expected_var = std::make_shared<VariableNode>();
			int expected_num = 1;
			std::string expected_var_name = "x";
			expected_var->setVariableName(expected_var_name);
			expected->setStatementNumber(expected_num);
			expected->setVariableNode(expected_var);
			pkb_builder.addReadNode(expected);
			pkb_builder.addVariableNode(expected_var);
			PKB pkb = pkb_builder.build();
			
			READ_NODE_PTR actual_pkb = pkb.getReads().at(0);
			READ_NODE_PTR actual_builder = pkb_builder.getReads().at(0);
			Assert::IsTrue(std::addressof(*expected) == std::addressof(*actual_pkb));
			Assert::IsTrue(std::addressof(*expected) == std::addressof(*actual_builder));
			
			int num = pkb.getReadNumList().at(0);
			Assert::AreEqual(expected_num, num);

			std::string actual_var_name = pkb.getReadVarNameList().at(0);
			Assert::AreEqual(expected_var_name, actual_var_name);
		}

		TEST_METHOD(addStatementList_getStatementLists_True) {
			PKBBuilder pkb_builder;
			STMT_LIST_NODE_PTR expected = std::make_shared<StatementListNode>();
			pkb_builder.addStatementListNode(expected);
			PKB pkb = pkb_builder.build();

			STMT_LIST_NODE_PTR actual_pkb = pkb.getStatementLists().at(0);
			STMT_LIST_NODE_PTR actual_builder = pkb_builder.getStatementLists().at(0);
			Assert::IsTrue(std::addressof(*expected) == std::addressof(*actual_pkb));
			Assert::IsTrue(std::addressof(*expected) == std::addressof(*actual_builder));
		}

		TEST_METHOD(addTry_getTrys_True) {
			PKBBuilder pkb_builder;
			TRY_NODE_PTR expected = std::make_shared<TryNode>();
			int expected_num = 1;
			expected->setStatementNumber(expected_num);
			pkb_builder.addTryNode(expected);
			PKB pkb = pkb_builder.build();

			TRY_NODE_PTR actual_pkb = pkb.getTrys().at(0);
			TRY_NODE_PTR actual_builder = pkb_builder.getTrys().at(0);
			Assert::IsTrue(std::addressof(*expected) == std::addressof(*actual_pkb));
			Assert::IsTrue(std::addressof(*expected) == std::addressof(*actual_builder));

			int num = pkb.getTryNumList().at(0);
			Assert::AreEqual(expected_num, num);
		}

		TEST_METHOD(addStatement_getStatements_True) {
			PKBBuilder pkb_builder;
			ASSIGN_NODE_PTR expected = std::make_shared<AssignNode>();
			int expected_num = 1;
			expected->setStatementNumber(expected_num);
			pkb_builder.addStatementNode(expected);
			PKB pkb = pkb_builder.build();

			STMT_NODE_PTR actual_pkb = pkb.getStatements().at(0);
			STMT_NODE_PTR actual_builder = pkb_builder.getStatements().at(0);
			Assert::IsTrue(std::addressof(*expected) == std::addressof(*actual_pkb));
			Assert::IsTrue(std::addressof(*expected) == std::addressof(*actual_builder));

			int num = pkb.getStatementNumList().at(0);
			Assert::AreEqual(expected_num, num);
		}

		TEST_METHOD(addVariable_getVariables_True) {
			PKBBuilder pkb_builder;
			VAR_NODE_PTR expected = std::make_shared<VariableNode>();
			std::string expected_name = "x";
			expected->setVariableName(expected_name);
			pkb_builder.addVariableNode(expected);
			PKB pkb = pkb_builder.build();
			
			VAR_NODE_PTR actual_pkb = pkb.getVariables().at(0);
			VAR_NODE_PTR actual_builder = pkb_builder.getVariables().at(0);
			Assert::IsTrue(std::addressof(*expected) == std::addressof(*actual_pkb));
			Assert::IsTrue(std::addressof(*expected) == std::addressof(*actual_builder));

			std::string name = pkb.getVariableNameList().at(0);
			Assert::AreEqual(expected_name, name);
		}

		TEST_METHOD(addWhile_getWhiles_True) {
			PKBBuilder pkb_builder;
			WHILE_NODE_PTR expected = std::make_shared<WhileNode>();
			int expected_num = 1;
			expected->setStatementNumber(expected_num);
			pkb_builder.addWhileNode(expected);
			PKB pkb = pkb_builder.build();

			WHILE_NODE_PTR actual_pkb = pkb.getWhiles().at(0);
			WHILE_NODE_PTR actual_builder = pkb_builder.getWhiles().at(0);
			Assert::IsTrue(std::addressof(*expected) == std::addressof(*actual_pkb));
			Assert::IsTrue(std::addressof(*expected) == std::addressof(*actual_builder));

			int num = pkb.getWhileNumList().at(0);
			Assert::AreEqual(expected_num, num);
		}

		TEST_METHOD(setProgramNode_getProgramNode_True) {
			PKBBuilder pkb_builder;
			PROGRAM_NODE_PTR expected = std::make_shared<ProgramNode>();
			pkb_builder.setProgramNode(expected);
			PKB pkb = pkb_builder.build();

			PROGRAM_NODE_PTR actual = pkb.getProgramNode();
			Assert::IsTrue(std::addressof(*expected) == std::addressof(*actual));
		}


	};

	TEST_CLASS(PKB_RelationshipTests) {
	public:
		/*
		- procedure main {
		1   print x;
		2   print y;
		3   print z;
		- }
		*/
		std::shared_ptr<PKB> pkb1;

		/*
		- procedure main {
		1   while (x > 0) {
		2     if (y > 0) then {
		3       read x;
		4       read y;
		-     } else {
		5       read z;
		6       y = y + 1;
		-     }
		-   }
		- }
		*/
		std::shared_ptr<PKB> pkb2;

		// FollowsT only
		/*
		- procedure {
		1   stmt
		2   stmt
		3   stmt
		4   while {
		5     stmt
		6     stmt
		-   }
		7   stmt
		8   stmt
		9   stmt
		- }
		*/
		std::shared_ptr<PKB> pkb3;

		// ParentT only
		/*
		- procedure {
		1   while {
		2     while {
		3       while {
		4         stmt
		-       }
		5       while {
		6         stmt
		-       }
		-     }
		7     while {
		8       stmt
		9       stmt
		-     }
		-   }
		- }
		*/
		std::shared_ptr<PKB> pkb4;

		// Calls & CallsT only
		/*
		procedure a { call b, c }
		procedure b { call c, d, f }
		procedure c { }
		procedure d { }
		procedure e { call f }
		procedure f { call g }
		procedure g { call h, i, j }
		procedure h { }
		procedure i { }
		procedure j { }
		*/
		std::shared_ptr<PKB> pkb5;

		// NextT only
		/*
		- procedure {
		1   while {
		2     if {
		3       while {
		4         stmt
		-       }
		-     } else {
		5       while {
		6         stmt
		-       }
		-     }
		7     stmt
		-   }
		8   stmt
		- }
		*/
		std::shared_ptr<PKB> pkb6;

		// Affects and Affect*
		/*
		- procedure {
		1 	a = 5;
		2	b = a;
		3	a = 4;
		4	b = a;
		5	a = 3;
		6	print a;
		7	read a;
		8	b = a;
		9	print a;
		- }
		
		*/
		std::shared_ptr<PKB> pkb7;

		// Affects*
		/*
		- procedure {
		1   b = a;
		2	while () {
		3	  b = d;
		4	  c = 5;
		5     d = c;
		-	}
		6	if () then {
		7	  a = b;
		-	} else {
		8	  c = b;
		9	  d = c;
		-	}
		- }
		*/

		std::shared_ptr<PKB> pkb8;

		TEST_METHOD_INITIALIZE(PKBInitialize) {
			PKBBuilder b1;
			b1.addFollows(1, 2);
			b1.addFollows(2, 3);
			b1.addUses("main", "x");
			b1.addUses("main", "y");
			b1.addUses("main", "z");
			b1.addUses(1, "x");
			b1.addUses(2, "y");
			b1.addUses(3, "z");
			pkb1 = std::make_shared<PKB>(b1.build());

			PKBBuilder b2;
			b2.addFollows(3, 4);
			b2.addFollows(5, 6);
			b2.addParent(1, 2);
			b2.addParent(2, 3);
			b2.addParent(2, 4);
			b2.addParent(2, 5);
			b2.addParent(2, 6);
			b2.addUses("main", "x");
			b2.addUses("main", "y");
			b2.addUses(1, "x");
			b2.addUses(1, "y");
			b2.addUses(2, "y");
			b2.addUses(6, "y");
			b2.addModifies("main", "x");
			b2.addModifies("main", "y");
			b2.addModifies("main", "z");
			b2.addModifies(1, "x");
			b2.addModifies(2, "y");
			b2.addModifies(3, "z");
			b2.addModifies(2, "x");
			b2.addModifies(2, "y");
			b2.addModifies(2, "z");
			b2.addModifies(3, "x");
			b2.addModifies(4, "y");
			b2.addModifies(5, "z");
			b2.addModifies(6, "y");
			b2.addNext(1, 2);
			b2.addNext(2, 3);
			b2.addNext(2, 5);
			b2.addNext(3, 4);
			b2.addNext(4, 1);
			b2.addNext(5, 6);
			b2.addNext(6, 1);
			pkb2 = std::make_shared<PKB>(b2.build());

			PKBBuilder b3;
			b3.addFollows(1, 2);
			b3.addFollows(2, 3);
			b3.addFollows(3, 4);
			b3.addFollows(4, 7);
			b3.addFollows(5, 6);
			b3.addFollows(7, 8);
			b3.addFollows(8, 9);
			pkb3 = std::make_shared<PKB>(b3.build());

			PKBBuilder b4;
			b4.addParent(1, 2);
			b4.addParent(1, 7);
			b4.addParent(2, 3);
			b4.addParent(2, 5);
			b4.addParent(3, 4);
			b4.addParent(5, 6);
			b4.addParent(7, 8);
			b4.addParent(7, 9);
			pkb4 = std::make_shared<PKB>(b4.build());

			PKBBuilder b5;
			b5.addCalls("a", "b");
			b5.addCalls("a", "c");
			b5.addCalls("b", "c");
			b5.addCalls("b", "d");
			b5.addCalls("b", "f");
			b5.addCalls("e", "f");
			b5.addCalls("f", "g");
			b5.addCalls("g", "h");
			b5.addCalls("g", "i");
			b5.addCalls("g", "j");
			pkb5 = std::make_shared<PKB>(b5.build());

			PKBBuilder b6;
			b6.addNext(1, 2);
			b6.addNext(1, 8);
			b6.addNext(2, 3);
			b6.addNext(2, 5);
			b6.addNext(3, 4);
			b6.addNext(3, 7);
			b6.addNext(4, 3);
			b6.addNext(5, 6);
			b6.addNext(5, 7);
			b6.addNext(6, 5);
			b6.addNext(7, 1);
			pkb6 = std::make_shared<PKB>(b6.build());

			{
				PKBBuilder b7;
				b7.addModifies("procedure", "a");
				b7.addModifies("procedure", "b");
				b7.addModifies(1, "a");
				b7.addModifies(2, "b");
				b7.addModifies(3, "a");
				b7.addModifies(4, "b");
				b7.addModifies(5, "a");
				b7.addModifies(7, "a");
				b7.addModifies(8, "b");

				b7.addUses("procedure", "a");
				b7.addUses(2, "a");
				b7.addUses(4, "a");
				b7.addUses(6, "a");
				b7.addUses(8, "a");
				b7.addUses(9, "a");

				b7.addNext(1, 2);
				b7.addNext(2, 3);
				b7.addNext(3, 4);
				b7.addNext(4, 5);
				b7.addNext(5, 6);
				b7.addNext(6, 7);
				b7.addNext(7, 8);
				b7.addNext(8, 9);

				ASSIGN_NODE_PTR a1 = std::make_shared<AssignNode>();
				ASSIGN_NODE_PTR a2 = std::make_shared<AssignNode>();
				ASSIGN_NODE_PTR a3 = std::make_shared<AssignNode>();
				ASSIGN_NODE_PTR a4 = std::make_shared<AssignNode>();
				ASSIGN_NODE_PTR a5 = std::make_shared<AssignNode>();
				ASSIGN_NODE_PTR a8 = std::make_shared<AssignNode>();

				PRINT_NODE_PTR p6 = std::make_shared<PrintNode>();
				PRINT_NODE_PTR p9 = std::make_shared<PrintNode>();

				READ_NODE_PTR r7 = std::make_shared<ReadNode>();
		
				a1->setStatementNumber(1);
				a2->setStatementNumber(2);
				a3->setStatementNumber(3);
				a4->setStatementNumber(4);
				a5->setStatementNumber(5);
				a8->setStatementNumber(8);

				p6->setStatementNumber(6);
				p9->setStatementNumber(9);

				r7->setStatementNumber(7);

				b7.addAssignNode(a1);
				b7.addAssignNode(a2);
				b7.addAssignNode(a3);
				b7.addAssignNode(a4);
				b7.addAssignNode(a5);
				b7.addAssignNode(a8);

				b7.addPrintNode(p6);
				b7.addPrintNode(p9);

				b7.addReadNode(r7);

				pkb7 = std::make_shared<PKB>(b7.build());
			}

			{
				PKBBuilder b8;
				b8.addModifies("procedure", "a");
				b8.addModifies("procedure", "b");
				b8.addModifies("procedure", "c");
				b8.addModifies("procedure", "d");
				b8.addModifies(1, "b");
				b8.addModifies(2, "b");
				b8.addModifies(2, "c");
				b8.addModifies(2, "d");
				b8.addModifies(3, "b");
				b8.addModifies(4, "c");
				b8.addModifies(5, "d");
				b8.addModifies(6, "a");
				b8.addModifies(6, "c");
				b8.addModifies(6, "d");
				b8.addModifies(7, "a");
				b8.addModifies(8, "c");
				b8.addModifies(9, "d");

				b8.addUses("procedure", "a");
				b8.addUses("procedure", "b");
				b8.addUses("procedure", "c");
				b8.addUses("procedure", "d");
				b8.addUses(1, "a");
				b8.addUses(2, "d");
				b8.addUses(2, "c");
				b8.addUses(3, "d");
				b8.addUses(5, "c");
				b8.addUses(6, "b");
				b8.addUses(6, "c");
				b8.addUses(7, "b");
				b8.addUses(8, "b");
				b8.addUses(9, "c");

				b8.addNext(1, 2);
				b8.addNext(2, 3);
				b8.addNext(2, 6);
				b8.addNext(3, 4);
				b8.addNext(4, 5);
				b8.addNext(5, 2);
				b8.addNext(6, 7);
				b8.addNext(6, 8);
				b8.addNext(8, 9);

				ASSIGN_NODE_PTR a1 = std::make_shared<AssignNode>();
				ASSIGN_NODE_PTR a3 = std::make_shared<AssignNode>();
				ASSIGN_NODE_PTR a4 = std::make_shared<AssignNode>();
				ASSIGN_NODE_PTR a5 = std::make_shared<AssignNode>();
				ASSIGN_NODE_PTR a7 = std::make_shared<AssignNode>();
				ASSIGN_NODE_PTR a8 = std::make_shared<AssignNode>();
				ASSIGN_NODE_PTR a9 = std::make_shared<AssignNode>();

				a1->setStatementNumber(1);
				a3->setStatementNumber(3);
				a4->setStatementNumber(4);
				a5->setStatementNumber(5);
				a7->setStatementNumber(7);
				a8->setStatementNumber(8);
				a9->setStatementNumber(9);

				b8.addAssignNode(a1);
				b8.addAssignNode(a3);
				b8.addAssignNode(a4);
				b8.addAssignNode(a5);
				b8.addAssignNode(a7);
				b8.addAssignNode(a8);
				b8.addAssignNode(a9);

				pkb8 = std::make_shared<PKB>(b8.build());
			}
		}

		/* isFollows */
		TEST_METHOD(isFollows_Stored_True) {
			Assert::IsTrue(pkb1->isFollows(1, 2));
			Assert::IsTrue(pkb1->isFollows(2, 3));

			Assert::IsTrue(pkb2->isFollows(3, 4));
			Assert::IsTrue(pkb2->isFollows(5, 6));
		}

		TEST_METHOD(isFollows_Transitive_False) {
			Assert::IsFalse(pkb1->isFollows(1, 3));

			Assert::IsFalse(pkb3->isFollows(1, 9));
		}

		TEST_METHOD(isFollows_InvalidStmtNum_False) {
			Assert::IsFalse(pkb1->isFollows(0, 1));
			Assert::IsFalse(pkb1->isFollows(3, 4));
		}

		TEST_METHOD(isFollows_S1GeqS2_False) {
			Assert::IsFalse(pkb1->isFollows(1, 1));
			Assert::IsFalse(pkb1->isFollows(2, 1));
			Assert::IsFalse(pkb1->isFollows(3, 2));
		}

		TEST_METHOD(isFollows_IfBlocks_False) {
			Assert::IsFalse(pkb2->isFollows(4, 5));
		}

		TEST_METHOD(isFollows_Children_False) {
			Assert::IsFalse(pkb2->isFollows(1, 2));
			Assert::IsFalse(pkb2->isFollows(1, 3));
			Assert::IsFalse(pkb2->isFollows(2, 3));
			Assert::IsFalse(pkb2->isFollows(2, 6));
		}

		/* isFollowsTransitive */
		TEST_METHOD(isFollowsT_Stored_True) {
			Assert::IsTrue(pkb1->isFollowsTransitive(1, 2));
			Assert::IsTrue(pkb1->isFollowsTransitive(2, 3));

			Assert::IsTrue(pkb2->isFollowsTransitive(3, 4));
			Assert::IsTrue(pkb2->isFollowsTransitive(5, 6));
		}

		TEST_METHOD(isFollowsT_Transitive_True) {
			Assert::IsTrue(pkb1->isFollowsTransitive(1, 3));

			Assert::IsTrue(pkb3->isFollowsTransitive(1, 9));
		}

		TEST_METHOD(isFollowsT_InvalidStmtNum_False) {
			Assert::IsFalse(pkb1->isFollowsTransitive(0, 1));
			Assert::IsFalse(pkb1->isFollowsTransitive(3, 4));
		}

		TEST_METHOD(isFollowsT_S1GeqS2_False) {
			Assert::IsFalse(pkb1->isFollowsTransitive(1, 1));
			Assert::IsFalse(pkb1->isFollowsTransitive(2, 1));
			Assert::IsFalse(pkb1->isFollowsTransitive(3, 2));
		}

		TEST_METHOD(isFollowsT_IfBlocks_False) {
			Assert::IsFalse(pkb2->isFollowsTransitive(4, 5));
		}

		TEST_METHOD(isFollowsT_Children_False) {
			Assert::IsFalse(pkb2->isFollowsTransitive(1, 2));
			Assert::IsFalse(pkb2->isFollowsTransitive(1, 3));
			Assert::IsFalse(pkb2->isFollowsTransitive(2, 3));
			Assert::IsFalse(pkb2->isFollowsTransitive(2, 6));
		}

		/* isParent */
		TEST_METHOD(isParent_Stored_True) {
			Assert::IsTrue(pkb2->isParent(1, 2));
			Assert::IsTrue(pkb2->isParent(2, 3));
			Assert::IsTrue(pkb2->isParent(2, 4));
			Assert::IsTrue(pkb2->isParent(2, 5));
			Assert::IsTrue(pkb2->isParent(2, 6));
		}

		TEST_METHOD(isParent_Transitive_False) {
			Assert::IsFalse(pkb2->isParent(1, 3));
			Assert::IsFalse(pkb2->isParent(1, 4));
			Assert::IsFalse(pkb2->isParent(1, 5));
			Assert::IsFalse(pkb2->isParent(1, 6));

			Assert::IsFalse(pkb4->isParent(1, 4));
			Assert::IsFalse(pkb4->isParent(1, 6));
			Assert::IsFalse(pkb4->isParent(1, 8));
			Assert::IsFalse(pkb4->isParent(1, 9));
		}

		TEST_METHOD(isParent_InvalidStmtNum_False) {
			Assert::IsFalse(pkb1->isParent(0, 1));
			Assert::IsFalse(pkb1->isParent(3, 4));
		}

		TEST_METHOD(isParent_S1GeqS2_False) {
			Assert::IsFalse(pkb2->isParent(1, 1));
			Assert::IsFalse(pkb2->isParent(2, 1));
			Assert::IsFalse(pkb2->isParent(3, 2));
		}

		/* isParentTransitive */
		TEST_METHOD(isParentT_Stored_True) {
			Assert::IsTrue(pkb2->isParentTransitive(1, 2));
			Assert::IsTrue(pkb2->isParentTransitive(2, 3));
			Assert::IsTrue(pkb2->isParentTransitive(2, 4));
			Assert::IsTrue(pkb2->isParentTransitive(2, 5));
			Assert::IsTrue(pkb2->isParentTransitive(2, 6));
		}

		TEST_METHOD(isParentT_Transitive_True) {
			Assert::IsTrue(pkb2->isParentTransitive(1, 3));
			Assert::IsTrue(pkb2->isParentTransitive(1, 4));
			Assert::IsTrue(pkb2->isParentTransitive(1, 5));
			Assert::IsTrue(pkb2->isParentTransitive(1, 6));

			Assert::IsTrue(pkb4->isParentTransitive(1, 4));
			Assert::IsTrue(pkb4->isParentTransitive(1, 6));
			Assert::IsTrue(pkb4->isParentTransitive(1, 8));
			Assert::IsTrue(pkb4->isParentTransitive(1, 9));
		}

		TEST_METHOD(isParentT_InvalidStmtNum_False) {
			Assert::IsFalse(pkb1->isParentTransitive(0, 1));
			Assert::IsFalse(pkb1->isParentTransitive(3, 4));
		}

		TEST_METHOD(isParentT_S1GeqS2_False) {
			Assert::IsFalse(pkb2->isParentTransitive(1, 1));
			Assert::IsFalse(pkb2->isParentTransitive(2, 1));
			Assert::IsFalse(pkb2->isParentTransitive(3, 2));
		}

		/* isUses */
		TEST_METHOD(isUses_Stored_True) {
			Assert::IsTrue(pkb1->isUses("main", "x"));
			Assert::IsTrue(pkb1->isUses("main", "y"));
			Assert::IsTrue(pkb1->isUses("main", "z"));
			Assert::IsTrue(pkb1->isUses(1, "x"));
			Assert::IsTrue(pkb1->isUses(2, "y"));
			Assert::IsTrue(pkb1->isUses(3, "z"));

			Assert::IsTrue(pkb2->isUses("main", "x"));
			Assert::IsTrue(pkb2->isUses("main", "y"));
			Assert::IsTrue(pkb2->isUses(1, "x"));
			Assert::IsTrue(pkb2->isUses(1, "y"));
			Assert::IsTrue(pkb2->isUses(2, "y"));
			Assert::IsTrue(pkb2->isUses(6, "y"));
		}

		TEST_METHOD(isUses_InvalidArgs_False) {
			Assert::IsFalse(pkb1->isUses(0, ""));
			Assert::IsFalse(pkb1->isUses("", "a"));
		}

		/* isModifies */
		TEST_METHOD(isModifies_Stored_True) {
			Assert::IsTrue(pkb2->isModifies("main", "x"));
			Assert::IsTrue(pkb2->isModifies("main", "y"));
			Assert::IsTrue(pkb2->isModifies("main", "z"));
			Assert::IsTrue(pkb2->isModifies(1, "x"));
			Assert::IsTrue(pkb2->isModifies(2, "y"));
			Assert::IsTrue(pkb2->isModifies(3, "z"));
			Assert::IsTrue(pkb2->isModifies(2, "x"));
			Assert::IsTrue(pkb2->isModifies(2, "y"));
			Assert::IsTrue(pkb2->isModifies(2, "z"));
			Assert::IsTrue(pkb2->isModifies(3, "x"));
			Assert::IsTrue(pkb2->isModifies(4, "y"));
			Assert::IsTrue(pkb2->isModifies(5, "z"));
			Assert::IsTrue(pkb2->isModifies(6, "y"));
		}

		TEST_METHOD(isModifies_InvalidArgs_False) {
			Assert::IsFalse(pkb1->isModifies(0, ""));
			Assert::IsFalse(pkb1->isModifies("", "a"));
		}

		/* isCalls */
		TEST_METHOD(isCalls_Stored_True) {
			Assert::IsTrue(pkb5->isCalls("a", "b"));
			Assert::IsTrue(pkb5->isCalls("a", "c"));
			Assert::IsTrue(pkb5->isCalls("b", "c"));
			Assert::IsTrue(pkb5->isCalls("b", "d"));
			Assert::IsTrue(pkb5->isCalls("b", "f"));
			Assert::IsTrue(pkb5->isCalls("e", "f"));
			Assert::IsTrue(pkb5->isCalls("f", "g"));
			Assert::IsTrue(pkb5->isCalls("g", "h"));
			Assert::IsTrue(pkb5->isCalls("g", "i"));
			Assert::IsTrue(pkb5->isCalls("g", "j"));
		}

		TEST_METHOD(isCalls_Transitive_False) {
			Assert::IsFalse(pkb5->isCalls("a", "h"));
			Assert::IsFalse(pkb5->isCalls("a", "i"));
			Assert::IsFalse(pkb5->isCalls("a", "j"));
			Assert::IsFalse(pkb5->isCalls("a", "d"));
			Assert::IsFalse(pkb5->isCalls("e", "h"));
			Assert::IsFalse(pkb5->isCalls("e", "i"));
			Assert::IsFalse(pkb5->isCalls("e", "j"));
		}

		TEST_METHOD(isCalls_InvalidProcName_False) {
			Assert::IsFalse(pkb5->isCalls("", ""));
			Assert::IsFalse(pkb5->isCalls("k", "l"));
		}

		/* isCallsTransitive */
		TEST_METHOD(isCallsT_Stored_True) {
			Assert::IsTrue(pkb5->isCallsTransitive("a", "b"));
			Assert::IsTrue(pkb5->isCallsTransitive("a", "c"));
			Assert::IsTrue(pkb5->isCallsTransitive("b", "c"));
			Assert::IsTrue(pkb5->isCallsTransitive("b", "d"));
			Assert::IsTrue(pkb5->isCallsTransitive("b", "f"));
			Assert::IsTrue(pkb5->isCallsTransitive("e", "f"));
			Assert::IsTrue(pkb5->isCallsTransitive("f", "g"));
			Assert::IsTrue(pkb5->isCallsTransitive("g", "h"));
			Assert::IsTrue(pkb5->isCallsTransitive("g", "i"));
			Assert::IsTrue(pkb5->isCallsTransitive("g", "j"));
		}

		TEST_METHOD(isCallsT_Transitive_True) {
			Assert::IsTrue(pkb5->isCallsTransitive("a", "h"));
			Assert::IsTrue(pkb5->isCallsTransitive("a", "i"));
			Assert::IsTrue(pkb5->isCallsTransitive("a", "j"));
			Assert::IsTrue(pkb5->isCallsTransitive("a", "d"));
			Assert::IsTrue(pkb5->isCallsTransitive("e", "h"));
			Assert::IsTrue(pkb5->isCallsTransitive("e", "i"));
			Assert::IsTrue(pkb5->isCallsTransitive("e", "j"));
		}

		TEST_METHOD(isCallsT_InvalidProcName_False) {
			Assert::IsFalse(pkb5->isCallsTransitive("", ""));
			Assert::IsFalse(pkb5->isCallsTransitive("k", "l"));
		}

		/* isNext */
		TEST_METHOD(isNext_Stored_True) {
			Assert::IsTrue(pkb2->isNext(1, 2));
			Assert::IsTrue(pkb2->isNext(2, 3));
			Assert::IsTrue(pkb2->isNext(2, 5));
			Assert::IsTrue(pkb2->isNext(3, 4));
			Assert::IsTrue(pkb2->isNext(4, 1));
			Assert::IsTrue(pkb2->isNext(5, 6));
			Assert::IsTrue(pkb2->isNext(6, 1));
		}

		TEST_METHOD(isNext_Transitive_False) {
			Assert::IsFalse(pkb2->isNext(1, 1));
			Assert::IsFalse(pkb2->isNext(2, 2));
			Assert::IsFalse(pkb2->isNext(3, 3));
			Assert::IsFalse(pkb2->isNext(4, 4));
			Assert::IsFalse(pkb2->isNext(5, 5));
			Assert::IsFalse(pkb2->isNext(6, 6));

			Assert::IsFalse(pkb2->isNext(2, 1));
			Assert::IsFalse(pkb2->isNext(6, 4));
		}

		TEST_METHOD(isNext_InvalidStmtNum_False) {
			Assert::IsFalse(pkb2->isNext(0, 1));
			Assert::IsFalse(pkb2->isNext(6, 7));
		}

		/* isNextTransitive */
		TEST_METHOD(isNextT_Stored_True) {
			Assert::IsTrue(pkb2->isNextTransitive(1, 2));
			Assert::IsTrue(pkb2->isNextTransitive(2, 3));
			Assert::IsTrue(pkb2->isNextTransitive(2, 5));
			Assert::IsTrue(pkb2->isNextTransitive(3, 4));
			Assert::IsTrue(pkb2->isNextTransitive(4, 1));
			Assert::IsTrue(pkb2->isNextTransitive(5, 6));
			Assert::IsTrue(pkb2->isNextTransitive(6, 1));

			Assert::IsTrue(pkb6->isNextTransitive(1, 2));
			Assert::IsTrue(pkb6->isNextTransitive(1, 8));
			Assert::IsTrue(pkb6->isNextTransitive(2, 3));
			Assert::IsTrue(pkb6->isNextTransitive(2, 5));
			Assert::IsTrue(pkb6->isNextTransitive(3, 4));
			Assert::IsTrue(pkb6->isNextTransitive(3, 7));
			Assert::IsTrue(pkb6->isNextTransitive(4, 3));
			Assert::IsTrue(pkb6->isNextTransitive(5, 6));
			Assert::IsTrue(pkb6->isNextTransitive(5, 7));
			Assert::IsTrue(pkb6->isNextTransitive(6, 5));
			Assert::IsTrue(pkb6->isNextTransitive(7, 1));
		}

		TEST_METHOD(isNextT_Transitive_True) {
			Assert::IsTrue(pkb2->isNextTransitive(1, 1));
			Assert::IsTrue(pkb2->isNextTransitive(2, 2));
			Assert::IsTrue(pkb2->isNextTransitive(3, 3));
			Assert::IsTrue(pkb2->isNextTransitive(4, 4));
			Assert::IsTrue(pkb2->isNextTransitive(5, 5));
			Assert::IsTrue(pkb2->isNextTransitive(6, 6));

			Assert::IsTrue(pkb2->isNextTransitive(2, 1));
			Assert::IsTrue(pkb2->isNextTransitive(6, 4));

			Assert::IsTrue(pkb6->isNextTransitive(1, 1));
			Assert::IsTrue(pkb6->isNextTransitive(2, 2));
			Assert::IsTrue(pkb6->isNextTransitive(3, 3));
			Assert::IsTrue(pkb6->isNextTransitive(4, 4));
			Assert::IsTrue(pkb6->isNextTransitive(5, 5));
			Assert::IsTrue(pkb6->isNextTransitive(6, 6));
			Assert::IsTrue(pkb6->isNextTransitive(7, 7));

			Assert::IsTrue(pkb6->isNextTransitive(4, 7));
			Assert::IsTrue(pkb6->isNextTransitive(3, 1));
			Assert::IsTrue(pkb6->isNextTransitive(7, 6));
		}

		TEST_METHOD(isNextT_InvalidStmtNum_False) {
			Assert::IsFalse(pkb2->isNextTransitive(0, 1));
			Assert::IsFalse(pkb2->isNextTransitive(6, 7));
		}

		TEST_METHOD(isAffects_Default_True) {
			Assert::IsTrue(pkb7->isAffects(1, 2));
			Assert::IsTrue(pkb7->isAffects(3, 4));
		}

		TEST_METHOD(isAffects_Transitive_True) {
			Assert::IsTrue(pkb8->isAffectsTransitive(1, 7));
			Assert::IsTrue(pkb8->isAffectsTransitive(3, 7));
		}

		TEST_METHOD(isAffects_InvalidStmtNum_False) {
			Assert::IsFalse(pkb7->isAffects(1, 4));
			Assert::IsFalse(pkb7->isAffectsTransitive(1, 4));
		}
	};
}
