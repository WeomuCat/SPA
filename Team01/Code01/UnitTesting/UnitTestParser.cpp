#include <typeinfo>

#include "stdafx.h"
#include "CppUnitTest.h"

#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>

#define private public
#include "..\source\Parser.h"
#undef private
#include "..\source\PKB.h"

typedef  std::shared_ptr<Parser> PARSER_PTR;


using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
	TEST_CLASS(ParseStmtTest) {
	public:
		// Initializes Parser with parent nodes
		/*
		Parser initParser(PARSER_PTR parser) {
			parser->program_node_ = (std::make_shared<ProgramNode>)();
			parser->pkb_builder_ = PKBBuilder();
			parser->pkb_builder_.setProgramNode(parser->program_node_);

			parser->current_parent_node_ = parser->program_node_;

			parser->stmt_num_ = 0;

			return parser.get();
		}
		*/
		/*
			Tests parseAssign(STMT_TOKEN_QUEUE stmt_tok_queue, PROCESS_TOKEN_QUEUE proc_tok_queue)
			stmt: a = 1
		*/
		
		TEST_METHOD(parse_assign_node_valid) {
			PARSER_PTR parser_ptr = std::make_shared<Parser>(Parser());
			
			// Initialize parser
			parser_ptr->program_node_ = std::make_shared<ProgramNode>();
			parser_ptr->pkb_builder_ = PKBBuilder();
			parser_ptr->pkb_builder_.setProgramNode(parser_ptr->program_node_);

			parser_ptr->current_parent_node_ = parser_ptr->program_node_;

			parser_ptr->stmt_num_ = 0;
		
			// Create token lists
			parser_ptr->stmt_token_queue_.push_back("assign");
			parser_ptr->stmt_token_queue_.push_back("a");
			parser_ptr->process_token_stream_.push_back("1");
			
			// Run parseAssign for relevant parser
			// int result = parser_ptr->parseAssign(&parser_ptr->stmt_token_queue_, &parser_ptr->process_token_stream_);

			// Confirm correct parsing of assign
			
		}
		

	};
}