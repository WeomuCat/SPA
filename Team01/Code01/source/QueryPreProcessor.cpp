#include <queue>
#include <regex>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "PKB/ASTNode/ConstantNode.h"
#include "PKB/ASTNode/VariableNode.h"
#include "PKB/ASTNode/ExpressionTypeEnum.h"
#include "PKB/ASTNode/ExpressionNode.h"
#include "QueryNode.h"
#include "QueryNodeType.h"
#include "QueryPreProcessor.h"
#include "QueryValidator.h"

const std::regex name_format_("[a-zA-Z][a-zA-Z0-9]*");
const std::regex integer_format_("[0-9]+");
const std::regex identity_format_("\"\\s*[a-zA-Z][a-zA-Z0-9]*\\s*\"");
const std::regex attr_ref_format_("[a-zA-Z][a-zA-Z0-9]*\\.(procName|varName|value|stmt#)");
const std::regex clause_pattern_if_format_("pattern\\s+[a-zA-Z][a-zA-Z0-9]*\\s*\\(\\s*(_|\"?\\s*[a-zA-Z][a-zA-Z0-9]*\\s*\"?)\\s*,\\s*_\\s*,\\s*_\\s*\\)");
const std::regex clause_pattern_while_format_("pattern\\s+[a-zA-Z][a-zA-Z0-9]*\\s*\\(\\s*(_|\"?\\s*[a-zA-Z][a-zA-Z0-9]*\\s*\"?)\\s*,\\s*_\\s*\\)");
const QueryNode null_node_ = QueryNode();

STRING QueryPreProcessor::trimWhitespaces(STRING s) {
	int start = s.find_first_not_of(" \n\r\t\f\v");
	int end = s.find_last_not_of(" \n\r\t\f\v");

	if (start == std::string::npos) {
		return "";
	}
	else {
		return s.substr(start, end - start + 1);
	}
}

SPLIT_DECLARATIONS QueryPreProcessor::splitDeclarations(DECLARATIONS d) {
	SPLIT_DECLARATIONS split_d;

	std::string delimiter = ";";
	int index = 0;
	int split_index = d.find(delimiter);

	if (split_index == -1) {
		split_d.push_back(d);
	}
	else {
		while (split_index != -1) {
			split_d.push_back(d.substr(index, split_index - index));
			index = split_index + 1;
			split_index = d.find(delimiter, index);
		}

		if (index != d.length()) {
			split_d.push_back(d.substr(index));
		}
	}

	return split_d;
}

QueryNode QueryPreProcessor::createElemNode(PROCESSED_SYNONYMS proc_s, ELEMENT e) {
	QueryNode elem_node = QueryNode();

	if (std::regex_match(e, std::regex(name_format_))) {
		// element is a synonym
		elem_node.setSynonymNode({ proc_s.find(e)->second.getSynonymType() }, e);
	}
	else {
		// element is an attribute reference
		// split element into synonym and attrRef
		int fullstop_index = e.find(".");
		STRING syn = trimWhitespaces(e.substr(0, fullstop_index));
		ATTRIBUTE_STRING attribute = trimWhitespaces(e.substr(fullstop_index + 1));;
		elem_node.setAttrNode(syn, attribute);
	}

	return elem_node;
}

QueryNode QueryPreProcessor::createResultNode(PROCESSED_SYNONYMS proc_s, RESULT r) {
	//returns Querynode of QueryNodeType::unassigned if result is invalid
	bool is_valid = true;
	QueryNode result_node = QueryNode();

	if (r.compare("BOOLEAN") == 0) {
		// result clause is a boolean
		result_node.setNodeType({ QueryNodeType::boolean });
	}
	else if (std::regex_match(r, std::regex(name_format_)) || std::regex_match(r, std::regex(attr_ref_format_))) {
		// result clause is a tuple of only one element
		result_node.setNodeType({ QueryNodeType::tuple });

		if (QueryValidator::isValidElem(proc_s, r)) {
			QueryNode result_child[] = { createElemNode(proc_s, r) };
			result_node.setChildren(result_child, 1);
		}
		else {
			is_valid = false;
		}
	}
	else {
		// result clause is a tuple of multiple elements
		result_node.setNodeType({ QueryNodeType::tuple });

		int open_brac_index = r.find("<");
		int comma_index = r.find(",");
		int closed_brac_index = r.find(">");

		if (comma_index == -1) {
			// only one element between brackets
			ELEMENT e = trimWhitespaces(r.substr(open_brac_index + 1,
				closed_brac_index - open_brac_index - 1));

			if (QueryValidator::isValidElem(proc_s, e)) {
				QueryNode result_child[] = { createElemNode(proc_s, e) };
				result_node.setChildren(result_child, 1);
			}
			else {
				is_valid = false;
			}
		}
		else {
			// loop through elements between brackets
			// create elem node
			// add as children

			int curr_index = open_brac_index + 1;

			while (comma_index != -1) {
				ELEMENT e = trimWhitespaces(r.substr(curr_index, comma_index - curr_index));

				if (QueryValidator::isValidElem(proc_s, e)) {
					QueryNode result_child[] = { createElemNode(proc_s, e) };
					result_node.setChildren(result_child, 1);

					curr_index = comma_index + 1;
					comma_index = r.find(",", curr_index);
				}
				else {
					is_valid = false;
					break;
				}
			}

			// create final element & add as child

			ELEMENT e = trimWhitespaces(r.substr(curr_index, closed_brac_index - curr_index));

			if (QueryValidator::isValidElem(proc_s, e)) {
				QueryNode result_child[] = { createElemNode(proc_s, e) };
				result_node.setChildren(result_child, 1);
			}
			else {
				is_valid = false;
			}
		}
	}

	if (is_valid) {
		return result_node;
	}
	else {
		return null_node_;
	}
}

NEXT_INDICES QueryPreProcessor::getNextClauseIndex(CLAUSES c, INDEX current_index) {
	NEXT_INDICES next_indices;
	INDEX starting_index;
	INDEX ending_index;
	std::priority_queue<int, std::vector<int>, std::greater<int>> index_queue;
	
	if (current_index != -1) {
		INDEX such_that_index = c.find("such that", current_index);
		INDEX pattern_index = c.find("pattern", current_index);
		INDEX with_index = c.find("with", current_index);

		if (such_that_index != -1) {
			index_queue.push(such_that_index);
		}
		if (pattern_index != -1) {
			index_queue.push(pattern_index);
		}
		if (with_index != -1) {
			index_queue.push(with_index);
		}

		if (index_queue.empty()) {
			starting_index = -1;
		}
		else {
			starting_index = index_queue.top();
			index_queue.pop();
		}

		if (starting_index == such_that_index) {
			// check if two consec suchthat-cl
			INDEX next_such_that = c.find("such that", starting_index + 1);

			if (next_such_that != -1) {
				index_queue.push(next_such_that);
			}
		}
		else if (starting_index == pattern_index) {
			// check if two consec pattern-cl
			INDEX next_pattern = c.find("pattern", starting_index + 1);

			if (next_pattern != -1) {
				index_queue.push(next_pattern);
			}
		}
		else {
			// check if two consec with-cl
			INDEX next_with = c.find("with", starting_index + 1);

			if (next_with != -1) {
				index_queue.push(next_with);
			}
		}

		if (index_queue.empty()) {
			ending_index = -1;
		}
		else {
			ending_index = index_queue.top();
			index_queue.pop();
		}
	}
	else {
		starting_index = -1;
		ending_index = -1;
	}

	next_indices.push_back(starting_index);
	next_indices.push_back(ending_index);

	return next_indices;
}

ARGUMENTS QueryPreProcessor::getArguments(SINGLE_CLAUSE c) {
	ARGUMENTS args;

	int open_brac_index = c.find("(");
	int comma_index = c.find(",");
	int closed_brac_index = c.rfind(")");

	int curr_index = open_brac_index + 1;

	while (comma_index != -1) {
		SINGLE_ARGUMENT a = trimWhitespaces(c.substr(curr_index, comma_index - curr_index));
		args.push_back(a);

		curr_index = comma_index + 1;
		comma_index = c.find(",", curr_index);
	}

	SINGLE_ARGUMENT a = trimWhitespaces(c.substr(curr_index, closed_brac_index - curr_index));
	args.push_back(a);

	return args;
}

INFIX_EXPR QueryPreProcessor::tokenizeExpression(EXPRESSION e) {
	INFIX_EXPR infix_e;
	std::string token;

	for (int i = 0; i < e.size(); i++) {
		char c = e[i];

		if (c == ' ') {
			if (token.size() > 0) {
				infix_e.push_back(token);
				token.clear();
			}
		}
		else if (c == '+' || c == '-' || c == '*' || c == '/' || c == '%' || c == '(' || c == ')') {
			// token is operator

			if (token.size() > 0) {
				infix_e.push_back(token);
				token.clear();
			}

			token.push_back(c);
		}
		else {
			// token is character/numeral

			if (token.size() > 0 && !std::regex_match(token, integer_format_) && !std::regex_match(token, name_format_)) {
				infix_e.push_back(token);
				token.clear();
			}

			token.push_back(c);
		}
	}

	infix_e.push_back(token);
	token.clear();

	return infix_e;
}

INTEGER QueryPreProcessor::getTokenPriority(TOKEN t) {
	// var & const < '+' & '-' < '*' & '/'

	if (t.compare("+") == 0 || t.compare("-") == 0) {
		return 1;
	}
	else if (t.compare("*") == 0 || t.compare("/") == 0 || t.compare("%") == 0) {
		return 2;
	}
	else {
		return 0;
	}
}

POSTFIX_EXPR QueryPreProcessor::infixToPostfix(INFIX_EXPR e) {
	POSTFIX_EXPR postfix_e;
	std::vector<TOKEN> op_stack;

	for (int i = 0; i < e.size(); i++) {
		// get token
		TOKEN t = e[i];

		if (std::regex_match(t, name_format_) || std::regex_match(t, integer_format_)) {
			// add to postfix expr if is a factor
			postfix_e.push_back(t);
		}
		else if (t.compare("(") == 0) {
			// add to stack if is '('
			op_stack.push_back(t);
		}
		else if (t.compare(")") == 0) {
			// pop and add from stack to postfix expr until '('
			while (op_stack.size() != 0 && op_stack.back().compare("(") != 0) {
				postfix_e.push_back(op_stack.back());
				op_stack.pop_back();
			}

			// pop '(' from stack
			op_stack.pop_back();
		}
		else {
			if (op_stack.size() == 0 ||getTokenPriority(t) > getTokenPriority(op_stack.back())) {
				// push onto stack if token has higher priority
				op_stack.push_back(t);
			}
			else {
				while (op_stack.size() != 0 && getTokenPriority(t) <= getTokenPriority(op_stack.back())) {
					// while stack is not empty & token has lower priority
					// pop and add from stack to postfix expr
					postfix_e.push_back(op_stack.back());
					op_stack.pop_back();
				}
				// push the token onto the stack
				op_stack.push_back(t);
			}
		}
	}

	while (op_stack.size() != 0) {
		// pop and add remaining from stack to postfix expr
		postfix_e.push_back(op_stack.back());
		op_stack.pop_back();
	}

	return postfix_e;
}

QueryNode QueryPreProcessor::createExpressionNode(EXPRESSION e) {
	//returns Querynode of QueryNodeType::unassigned if result is invalid
	bool is_valid = true;
	QueryNode q_expr_node = QueryNode();

	if (e.compare("_") == 0) {
		// is wild card
		q_expr_node.setNodeType({ QueryNodeType::wild_card });
	}
	else {
		// get expression
		int open_quote_index = e.find("\"");
		int close_quote_index = e.rfind("\"");
		std::string trimmed_expr = trimWhitespaces(e.substr(open_quote_index + 1,
			close_quote_index - open_quote_index - 1));

		INFIX_EXPR infix_expr = tokenizeExpression(trimmed_expr);
		POSTFIX_EXPR postfix_expr = infixToPostfix(infix_expr);

		if (!QueryValidator::isValidPostfixExpr(postfix_expr)) {
			is_valid = false;
		}
		
		if (is_valid) {
			std::vector<std::shared_ptr<ASTNode>> term_stack;

			for (int i = 0; i < postfix_expr.size(); i++) {
				// create expression by going through postfix expression tokens
				TOKEN t = postfix_expr[i];

				if (std::regex_match(t, name_format_)) {
					// token is var
					std::shared_ptr<VariableNode> var_node = std::make_shared<VariableNode>();
					var_node->setVariableName(t);
					term_stack.push_back(var_node);
				}
				else if (std::regex_match(t, integer_format_)) {
					//token is const
					std::shared_ptr<ConstantNode> const_node = std::make_shared<ConstantNode>();
					const_node->setValue(t);
					term_stack.push_back(const_node);
				}
				else {
					//token is expr
					std::shared_ptr<ExpressionNode> expr_node = std::make_shared<ExpressionNode>();

					if (t.compare("+") == 0) {
						expr_node->setExpressionType({ ExpressionTypeEnum::plus });
					}
					else if (t.compare("-") == 0) {
						expr_node->setExpressionType({ ExpressionTypeEnum::min });
					}
					else if (t.compare("*") == 0) {
						expr_node->setExpressionType({ ExpressionTypeEnum::times });
					}
					else if (t.compare("/") == 0) {
						expr_node->setExpressionType({ ExpressionTypeEnum::div });
					}
					else {
						// operator is '%'
						expr_node->setExpressionType({ ExpressionTypeEnum::mod });
					}

					expr_node->setRightAstNode(term_stack.back());
					term_stack.pop_back();
					expr_node->setLeftAstNode(term_stack.back());
					term_stack.pop_back();
					term_stack.push_back(expr_node);
				}
			}

			// set ast node as child

			if (e.find("_") != -1) {
				// partial matching expression
				q_expr_node.setNodeType({ QueryNodeType::partial_expression });
			}
			else {
				// full matching expression
				q_expr_node.setNodeType({ QueryNodeType::expression });
			}

			if (term_stack.back()->getNodeType() == NodeTypeEnum::variableNode || term_stack.back()->getNodeType() == NodeTypeEnum::constantNode) {
				// if final node is a single var/const
				// create expr ast node and set final node as left node
				std::shared_ptr<ExpressionNode> expr_node = std::make_shared<ExpressionNode>();
				expr_node->setLeftAstNode(term_stack.back());
				term_stack.pop_back();
				term_stack.push_back(expr_node);
			}

			q_expr_node.setASTNode(term_stack.back());
			term_stack.pop_back();
		}
	}

	if (is_valid) {
		return q_expr_node;
	}
	else {
		return null_node_;
	}
}

QueryNode QueryPreProcessor::createArgumentNode(PROCESSED_SYNONYMS proc_s, SINGLE_ARGUMENT arg) {
	QueryNode arg_node = QueryNode();

	if (std::regex_match(arg, name_format_)) {
		// argument is a synonym
		arg_node.setSynonymNode({ proc_s.find(arg)->second.getSynonymType() }, arg);
	}
	else if (arg.compare("_") == 0) {
		// argument is a wild card
		arg_node.setNodeType({ QueryNodeType::wild_card });

	}
	else if (std::regex_match(arg, integer_format_)) {
		// argument is an integer
		arg_node.setIntegerNode(std::stoi(arg));
	}
	else if (std::regex_match(arg, identity_format_)) {
		// argument is an identity
		arg_node.setIdentityNode(arg.substr(1, arg.length() - 2));

	}
	else {
		// argument is an attribute
		SYNONYM_NAME s = arg.substr(0, arg.find("."));
		ATTRIBUTE_STRING attr_name = arg.substr(arg.find(".") + 1);
		arg_node.setAttrNode(s, attr_name);
	}

	return arg_node;
}

QueryNode QueryPreProcessor::createRelationNode(PROCESSED_SYNONYMS proc_s, SINGLE_CLAUSE c) {
	// returns QueryNode of QueryNodeType::unassigned if clause is invalid syntatically
	// returns Follows QueryNode with no children if clause is invalid semantically

	bool is_valid = true;
	bool is_syntax_valid = true;
	QueryNode relation_node = QueryNode();

	int open_brac_index = c.find("(");
	RELATIONSHIP rel = trimWhitespaces(c.substr(0, open_brac_index));
	ARGUMENTS args = getArguments(c);

	if (!QueryValidator::isValidRelationArguments(proc_s, rel, args)) {
		is_valid = false;
		
		if (std::regex_match(args[0], name_format_) && !QueryValidator::isSynonymDeclared(proc_s, args[0])) {
			is_syntax_valid = false;
		}
		else if (std::regex_match(args[1], name_format_) && !QueryValidator::isSynonymDeclared(proc_s, args[1])) {
			is_syntax_valid = false;
		}
	}

	if (is_valid) {
		if (rel.compare("Uses") == 0 || rel.compare("Modifies") == 0) {
			if (QueryValidator::isStatementRef(proc_s, args[0])) {
				if (rel.compare("Uses") == 0) {
					relation_node.setNodeType("UsesS");
				}
				else {
					relation_node.setNodeType("ModifiesS");
				}
			}
			else {
				if (std::regex_match(rel, std::regex("Uses"))) {
					relation_node.setNodeType("UsesP");
				}
				else {
					relation_node.setNodeType("ModifiesP");
				}
			}
		}
		else {
			relation_node.setNodeType((NODE_TYPE_STRING)rel);
		}

		QueryNode first_arg_node = createArgumentNode(proc_s, args[0]);
		QueryNode second_arg_node = createArgumentNode(proc_s, args[1]);

		QueryNode relation_node_children[] = { first_arg_node, second_arg_node };
		relation_node.setChildren(relation_node_children, 2);

		return relation_node;
	}
	else if (is_syntax_valid) {
		// semantics invalid
		QueryNode invalid_follows_node = QueryNode();
		invalid_follows_node.setNodeType({ QueryNodeType::follows });

		return invalid_follows_node;
	}
	else {
		// syntax invalid
		return null_node_;
	}
}

QueryNode QueryPreProcessor::createPatternNode(PROCESSED_SYNONYMS proc_s, SINGLE_CLAUSE c) {
	// returns QueryNode of QueryNodeType::unassigned if clause is invalid syntatically
	// returns pattern QueryNode with no children if clause is invalid semantically

	bool is_valid = true;
	bool is_syntax_valid = true;
	QueryNode pattern_node = QueryNode();
	pattern_node.setNodeType({ QueryNodeType::pattern });

	// get synonym
	int syn_start_index = c.find(" ");
	int open_brac_index = c.find("(");
	SYNONYM_NAME s = trimWhitespaces(c.substr(syn_start_index,
		open_brac_index - syn_start_index));

	ARGUMENTS args = getArguments(c);

	if (!QueryValidator::isValidPatternArguments(proc_s, s, args)) {
		is_valid = false;

		if (!QueryValidator::isSynonymDeclared(proc_s, s)) {
			is_syntax_valid = false;
		}
		else if (std::regex_match(args[0], name_format_) && !QueryValidator::isSynonymDeclared(proc_s, args[0])) {
			is_syntax_valid = false;
		}
		else if (proc_s.find(s)->second.getSynonymType() == QuerySynonymType::ifs && !std::regex_match(args[0], clause_pattern_if_format_)) {
			is_syntax_valid = false;
		}
		else if (proc_s.find(s)->second.getSynonymType() == QuerySynonymType::whiles && std::regex_match(args[0], clause_pattern_while_format_)) {
			is_syntax_valid = false;
		}
	}

	if (is_valid) {
		QueryNode syn_node = createArgumentNode(proc_s, s);
		QueryNode first_arg_node = createArgumentNode(proc_s, args[0]);
		QueryNode second_arg_node = createExpressionNode(args[1]);

		if (proc_s.find(s)->second.getSynonymType() == QuerySynonymType::ifs) {
			// pattern-if has 3 arguments
			QueryNode third_arg_node = createExpressionNode(args[2]);
			QueryNode pattern_node_children[] = { syn_node, first_arg_node, second_arg_node, third_arg_node };
			pattern_node.setChildren(pattern_node_children, 4);
		}
		else {
			// pattern-assign & pattern-while has 3 argument
			// check if expr is valid for pattern-assign (done in createExpressionNode)
			if (second_arg_node.getNodeType() != QueryNodeType::unassigned) {
				QueryNode pattern_node_children[] = { syn_node, first_arg_node, second_arg_node };
				pattern_node.setChildren(pattern_node_children, 3);
			}
			else {
				return null_node_;
			}
		}

		return pattern_node;
	}
	else if (is_syntax_valid) {
		// semantics invalid
		QueryNode invalid_pattern_node = QueryNode();
		invalid_pattern_node.setNodeType({ QueryNodeType::pattern });

		return invalid_pattern_node;
	}
	else {
		// syntax invalid
		return null_node_;
	}

}

QueryNode QueryPreProcessor::createWithNode(PROCESSED_SYNONYMS proc_s, SINGLE_CLAUSE c) {
	// returns QueryNode of QueryNodeType::unassigned if clause is invalid syntatically
	// returns pattern QueryNode with no children if clause is invalid semantically

	bool is_valid = true;
	bool is_syntax_valid = true;
	QueryNode with_node = QueryNode();
	with_node.setNodeType({ QueryNodeType::with });

	int equals_index = c.find("=");
	ARGUMENTS refs;
	SINGLE_ARGUMENT first_ref = trimWhitespaces(c.substr(0, equals_index));;
	SINGLE_ARGUMENT second_ref = trimWhitespaces(c.substr(equals_index+1));;

	refs.push_back(first_ref);
	refs.push_back(second_ref);

	if (!QueryValidator::isValidWithArguments(proc_s, refs)) {
		is_valid = false;

		if (std::regex_match(first_ref, name_format_) && !QueryValidator::isSynonymDeclared(proc_s, first_ref)) {
			is_syntax_valid = false;
		}
		else if (std::regex_match(second_ref, name_format_) && !QueryValidator::isSynonymDeclared(proc_s, second_ref)) {
			is_syntax_valid = false;
		}
		else if (std::regex_match(first_ref, attr_ref_format_) &&
			!QueryValidator::isSynonymDeclared(proc_s, first_ref.substr(0, first_ref.find(".")))) {
			is_syntax_valid = false;
		}
		else if (std::regex_match(second_ref, attr_ref_format_) &&
			!QueryValidator::isSynonymDeclared(proc_s, second_ref.substr(0, second_ref.find(".")))) {
			is_syntax_valid = false;
		}
	}

	if (is_valid) {
		QueryNode first_ref_node = createArgumentNode(proc_s, first_ref);
		QueryNode second_ref_node = createArgumentNode(proc_s, second_ref);

		QueryNode with_node_children[] = { first_ref_node, second_ref_node };
		with_node.setChildren(with_node_children, 2);

		return with_node;
	}
	else if (is_syntax_valid) {
		// semantics invalid
		QueryNode invalid_with_node = QueryNode();
		invalid_with_node.setNodeType({ QueryNodeType::with });

		return invalid_with_node;
	}
	else {
		// syntax invalid
		return null_node_;
	}
}

SPLIT_QUERY QueryPreProcessor::splitQuery(QUERY q) {
	SPLIT_QUERY split_q;
	DECLARATIONS d = "";
	CLAUSES c = "";

	if (QueryValidator::isValidStructure(q)) {
		// split the query into declarations and clauses
		std::string delimiter = ";";
		int split_index = q.rfind(delimiter);
		d = trimWhitespaces(q.substr(0, split_index));
		c = trimWhitespaces(q.substr(split_index + 1));
	}
	split_q.push_back(d);
	split_q.push_back(c);

	return split_q;
}

PROCESSED_SYNONYMS QueryPreProcessor::preProcessSynonyms(DECLARATIONS d) {
	PROCESSED_SYNONYMS proc_s;
	VALIDATION_RESULT is_valid = true;

	SPLIT_DECLARATIONS split_d = splitDeclarations(d);


	for (int i = 0; i < split_d.size(); i++) {
		// loop through each declaration
		// check validity of each declaration

		// trim whitespaces at front and back of declaration
		SINGLE_DECLARATION single_d = trimWhitespaces(split_d[i]);

		if (!QueryValidator::isValidDeclaration(single_d)) {
			// break out of loop if not valid
			is_valid = false;
			break;
		}

		// get design entity
		int first_space_index = single_d.find(" ");
		STRING design_entity = single_d.substr(0, first_space_index);

		// break into synonyms
		// create synonym nodes
		// add node to map
		std::string delimiter = ",";
		int index = first_space_index + 1;
		int split_index = single_d.find(delimiter);

		if (split_index == -1) {
			QueryNode new_node = QueryNode();
			SYNONYM_NAME syn_name = trimWhitespaces(single_d.substr(index));

			new_node.setSynonymNode(design_entity, syn_name);
			proc_s.insert({ syn_name, new_node });
		}
		else {
			bool is_not_last = true;

			while (split_index != -1) {
				QueryNode new_node = QueryNode();
				SYNONYM_NAME syn_name;

				if (is_not_last) {
					syn_name = trimWhitespaces(single_d.substr(index, split_index - index));
				}
				else {
					syn_name = trimWhitespaces(single_d.substr(index));
				}

				new_node.setSynonymNode(design_entity, syn_name);
				proc_s.insert({ syn_name, new_node });

				index = split_index + 1;
				split_index = single_d.find(delimiter, index);

				if (split_index == -1 && is_not_last) {
					split_index = index;
					is_not_last = false;
				}
			}
		}
	}

	if (is_valid) {
		return proc_s;
	}
	else {
		PROCESSED_SYNONYMS empty_map;

		return empty_map;
	}
}

PROCESSED_CLAUSES QueryPreProcessor::preProcessClauses(PROCESSED_SYNONYMS proc_s, CLAUSES c) {
	PROCESSED_CLAUSES select_node = QueryNode();
	VALIDATION_RESULT is_valid = true;
	VALIDATION_RESULT is_syntax_valid = true;
	bool is_bool = false;


	if (QueryValidator::isValidClause(c)) {
		select_node.setNodeType({ QueryNodeType::select });

		RESULT result_clause;
		NEXT_INDICES next_indices = getNextClauseIndex(c, 0);
		int clause_start_index = next_indices[0];
		int clause_end_index = next_indices[1];

		if (clause_start_index == -1) {
			// no such that nor pattern clause
			result_clause = trimWhitespaces(c.substr(6));

			if (QueryValidator::isValidResultFormat(result_clause)) {
				// create result clause node and set as child
				QueryNode result_node = createResultNode(proc_s, result_clause);

				if (result_node.getNodeType() == QueryNodeType::unassigned) {
					is_valid = false;
				}
				else {
					if (result_node.getNodeType() == QueryNodeType::boolean) {
						is_bool = true;
					}
					select_node.addChild(result_node);
				}
			}
			else {
				is_valid = false;
			}
		}
		else {
			// extract result clause
			result_clause = trimWhitespaces(c.substr(6, clause_start_index - 6));

			if (QueryValidator::isValidResultFormat(result_clause)) {
				// create result clause node and set as first child
				QueryNode result_node = createResultNode(proc_s, result_clause);

				if (result_node.getNodeType() == QueryNodeType::unassigned) {
					is_valid = false;
				}
				else {
					if (result_node.getNodeType() == QueryNodeType::boolean) {
						is_bool = true;
					}

					select_node.addChild(result_node);
				}
			}
			else {
				is_valid = false;
			}

			while (clause_start_index != -1) {
				// check type of clause
				// get clause
				// check format validity
				// get clause arguments
				// check argument validity
				// create clause node
				// add to children of select node
				if (c[clause_start_index] == 's') {
					// suchthat-cl
					SINGLE_CLAUSE current_c = trimWhitespaces(c.substr(clause_start_index + 9,
						clause_end_index - (clause_start_index + 9)));

					bool is_last = false;
					INDEX split_index = 0;
					INDEX and_index = current_c.find("and", split_index);

					if (and_index == -1) {
						is_last = true;
					}

					while (is_valid && (and_index != -1 || is_last)) {
						SINGLE_CLAUSE rel_c = trimWhitespaces(current_c.substr(split_index, and_index - split_index));

						if (!QueryValidator::isValidRelationFormat(rel_c)) {
							is_valid = false;
							is_syntax_valid = false;
							break;
						}

						QueryNode such_that_node = QueryNode();
						such_that_node.setNodeType({ QueryNodeType::such_that });

						QueryNode relation_node = createRelationNode(proc_s, rel_c);

						if (relation_node.getNodeType() == QueryNodeType::unassigned) {
							is_valid = false;
							is_syntax_valid = false;
							break;
						}
						else if (relation_node.getNodeType() == QueryNodeType::follows && relation_node.getChildren().size() == 0) {
							is_valid = false;
							break;

						}
						else {
							such_that_node.addChild(relation_node);
							select_node.addChild(such_that_node);
						}
						
						
						if (is_last) {
							is_last = false;
						}
						else {
							split_index = and_index + 3;
							and_index = current_c.find("and", split_index);

							if (and_index == -1) {
								is_last = true;
							}
						}
					}
				}
				else if (c[clause_start_index] == 'p') {
					// pattern-cl
					SINGLE_CLAUSE current_c = trimWhitespaces(c.substr(clause_start_index, clause_end_index - clause_start_index));

					if (!QueryValidator::isValidPatternFormat(current_c)) {
						is_valid = false;
						is_syntax_valid = false;
						break;
					}
					else if (current_c.find("and") != -1) {
						// if has pattern 'and', next clause should be pattern

						INDEX next_clause_start = getNextClauseIndex(c, clause_end_index)[0];

						if (c[next_clause_start] != 'p') {
							is_valid = false;
							is_syntax_valid = false;
							break;
						}
					}

					QueryNode pattern_node = createPatternNode(proc_s, current_c);

					if (pattern_node.getNodeType() == QueryNodeType::unassigned) {
						is_valid = false;
						is_syntax_valid = false;
						break;
					}
					else if (pattern_node.getNodeType() == QueryNodeType::pattern && pattern_node.getChildren().size() == 0) {
						is_valid = false;
						break;

					}
					else {
						select_node.addChild(pattern_node);
					}
				
				}
				else {
					// with-cl
					SINGLE_CLAUSE current_c = trimWhitespaces(c.substr(clause_start_index + 4,
						clause_end_index - (clause_start_index + 4)));

					bool is_last = false;
					INDEX split_index = 0;
					INDEX and_index = current_c.find("and", split_index);

					if (and_index == -1) {
						is_last = true;
					}

					while (is_valid && (and_index != -1 || is_last)) {
						SINGLE_CLAUSE with_c = trimWhitespaces(current_c.substr(split_index, and_index - split_index));

 						if (!QueryValidator::isValidWithFormat(with_c)) {
							is_valid = false;
							is_syntax_valid = false;
							break;
						}

						QueryNode with_node = createWithNode(proc_s, with_c);

						if (with_node.getNodeType() == QueryNodeType::unassigned) {
							is_valid = false;
							is_syntax_valid = false;
							break;
						}
						else if (with_node.getNodeType() == QueryNodeType::with && with_node.getChildren().size() == 0) {
							is_valid = false;
							break;

						}
						else {
							select_node.addChild(with_node);
						}

						if (is_last) {
							is_last = false;
						}
						else {
							split_index = and_index + 3;
							and_index = current_c.find("and", split_index);

							if (and_index == -1) {
								is_last = true;
							}
						}
					}
				}

				next_indices = getNextClauseIndex(c, clause_end_index);
				clause_start_index = next_indices[0];
				clause_end_index = next_indices[1];
			}
		}
	}

	if (is_valid) {
		return select_node;
	}
	else if (is_bool && is_syntax_valid) {
		// result-cl is bool and is semantically invalid
		QueryNode unassigned_node = QueryNode();
		QueryNode invalid_bool_node = QueryNode();
		invalid_bool_node.setNodeType({ QueryNodeType::boolean });
		QueryNode unassigned_node_children[] = { invalid_bool_node };
		unassigned_node.setChildren(unassigned_node_children, 1);

		return unassigned_node;
	}
	else {
		return null_node_;
	}
}