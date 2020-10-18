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
const std::regex attr_ref_format_("[a-zA-Z][a-zA-Z0-9]*\\.(procName|varName|value|stmt#)");
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

INDEX QueryPreProcessor::getNextClauseIndex(CLAUSES c, INDEX current_index, INDEX such_that_index, INDEX pattern_index) {
	int next_index;

	if (such_that_index != -1 && pattern_index != -1) {
		next_index = std::min(such_that_index, pattern_index);
	}
	else if (such_that_index == -1) {
		// 'such that' is not found
		next_index = pattern_index;
	}
	else {
		// 'pattern' is not found
		next_index = such_that_index;
	}

	return next_index;
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

int QueryPreProcessor::getTokenPriority(TOKEN t) {
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
	else {
		// argument is an identity
		arg_node.setIdentityNode(arg.substr(1, arg.length() - 2));

	}

	return arg_node;

}

QueryNode QueryPreProcessor::createRelationNode(PROCESSED_SYNONYMS proc_s, SINGLE_CLAUSE c) {
	//returns Querynode of QueryNodeType::unassigned if clause is invalid
	bool is_valid = true;
	QueryNode relation_node = QueryNode();

	int open_brac_index = c.find("(");
	RELATIONSHIP rel = trimWhitespaces(c.substr(0, open_brac_index));
	ARGUMENTS args = getArguments(c);

	if (!QueryValidator::isValidRelationArguments(proc_s, rel, args)) {
		is_valid = false;
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
	else {
		return null_node_;
	}
}

QueryNode QueryPreProcessor::createPatternNode(PROCESSED_SYNONYMS proc_s, SINGLE_CLAUSE c) {
	//returns Querynode of QueryNodeType::unassigned if clause is invalid
	bool is_valid = true;
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
	else {
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

	if (QueryValidator::isValidClause(c)) {
		select_node.setNodeType({ QueryNodeType::select });

		RESULT result_clause;
		int such_that_index = c.find("such that");
		int pattern_index = c.find("pattern");
		int next_index = getNextClauseIndex(c, 0, such_that_index, pattern_index);

		if (next_index == -1) {
			// no such that nor pattern clause
			result_clause = trimWhitespaces(c.substr(6));

			if (QueryValidator::isValidResultFormat(result_clause)) {
				// create result clause node and set as child
				QueryNode result_node = createResultNode(proc_s, result_clause);

				if (result_node.getNodeType() != QueryNodeType::unassigned) {
					QueryNode select_children[] = { result_node };
					select_node.setChildren(select_children, 1);
				}
				else {
					is_valid = false;
				}
			}
			else {
				is_valid = false;
			}
		}
		else {
			// select node will have maximum 3 children
			QueryNode select_children[3];
			int child_index = 0;

			// extract result clause
			result_clause = trimWhitespaces(c.substr(6, next_index - 6));

			if (QueryValidator::isValidResultFormat(result_clause)) {
				// create result clause node and set as first child
				QueryNode result_node = createResultNode(proc_s, result_clause);

				if (result_node.getNodeType() != QueryNodeType::unassigned) {
					select_children[child_index] = result_node;
					child_index++;
				}
				else {
					is_valid = false;
				}
			}
			else {
				is_valid = false;
			}

			while (next_index != -1) {
				// check if next clause is "such that" or "pattern"
				// get clause
				// check format validity
				// get clause arguments
				// check argument validity
				// create clause node
				// add to children array
				if (next_index == such_that_index) {
					SINGLE_CLAUSE current_c = trimWhitespaces(c.substr(next_index + 9, pattern_index - (next_index + 9)));

					if (!QueryValidator::isValidRelationFormat(current_c)) {
						is_valid = false;
						break;
					}

					QueryNode such_that_node = QueryNode();
					such_that_node.setNodeType({ QueryNodeType::such_that });

					QueryNode relation_node = createRelationNode(proc_s, current_c);

					if (relation_node.getNodeType() != QueryNodeType::unassigned) {
						QueryNode such_that_node_children[1] = { relation_node };
						such_that_node.setChildren(such_that_node_children, 1);
						select_children[child_index] = such_that_node;
						child_index++;
					}
					else {
						is_valid = false;
						break;
					}
				}
				else {
					SINGLE_CLAUSE current_c = trimWhitespaces(c.substr(next_index, such_that_index - next_index));

					if (!QueryValidator::isValidPatternFormat(current_c)) {
						is_valid = false;
						break;
					}

					QueryNode pattern_node = createPatternNode(proc_s, current_c);

					if (pattern_node.getNodeType() != QueryNodeType::unassigned) {
						select_children[child_index] = pattern_node;
						child_index++;
					}
					else {
						is_valid = false;
						break;
					}
				}

				such_that_index = c.find("such that", next_index + 1);
				pattern_index = c.find("pattern", next_index + 1);
				next_index = getNextClauseIndex(c, next_index, such_that_index, pattern_index);
			}

			// set children of select node
			select_node.setChildren(select_children, child_index);
		}
	}

	if (is_valid) {
		return select_node;
	}
	else {
		return null_node_;
	}
}