#include <regex>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "PKB/ASTNode/ConstantNode.h"
#include "PKB/ASTNode/VariableNode.h"
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
	int closed_brac_index = c.find(")");

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

QueryNode QueryPreProcessor::createExpressionNode(EXPRESSION e) {
	QueryNode exp_node = QueryNode();

	if (e.compare("_") == 0) {
		// is wild card
		exp_node.setNodeType({ QueryNodeType::wild_card });
	}
	else {
		// get expression
		int open_quote_index = e.find("\"");
		int close_quote_index = e.rfind("\"");
		std::string trimmed_exp = trimWhitespaces(e.substr(open_quote_index + 1,
			close_quote_index - open_quote_index - 1));

		// create expression
		std::shared_ptr<ExpressionNode> expr_node = std::make_shared<ExpressionNode>();

		if (std::regex_match(trimmed_exp, std::regex(name_format_))) {
			// expression is a var_name
			std::shared_ptr<VariableNode> var_node = std::make_shared<VariableNode>();
			var_node->setVariableName(trimmed_exp);
			expr_node->setLeftAstNode(var_node);
			exp_node.setASTNode(expr_node);
		}
		else {
			// expression is a const_value
			std::shared_ptr<ConstantNode> const_node = std::make_shared<ConstantNode>();
			const_node->setValue(trimmed_exp);
			expr_node->setLeftAstNode(const_node);
			exp_node.setASTNode(expr_node);
		}
	}
	
	return exp_node;
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
	/*
	int comma_index = c.find(",");
	int closed_brac_index = c.find(")");
	ARGUMENT first_arg = trimWhitespaces(c.substr(open_brac_index + 1,
		comma_index - open_brac_index - 1));
	ARGUMENT second_arg = trimWhitespaces(c.substr(comma_index + 1,
		closed_brac_index - comma_index - 1));
	*/
	// get relation
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
			// pattern-assign & pattern-while has 3 arguments
			QueryNode pattern_node_children[] = { syn_node, first_arg_node, second_arg_node };
			pattern_node.setChildren(pattern_node_children, 3);
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
			bool isNotLast = true;

			while (split_index != -1) {
				QueryNode new_node = QueryNode();
				SYNONYM_NAME syn_name;

				if (isNotLast) {
					syn_name = trimWhitespaces(single_d.substr(index, split_index - index));
				}
				else {
					syn_name = trimWhitespaces(single_d.substr(index));
				}

				new_node.setSynonymNode(design_entity, syn_name);
				proc_s.insert({ syn_name, new_node });

				index = split_index + 1;
				split_index = single_d.find(delimiter, index);

				if (split_index == -1 && isNotLast) {
					split_index = index;
					isNotLast = false;
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