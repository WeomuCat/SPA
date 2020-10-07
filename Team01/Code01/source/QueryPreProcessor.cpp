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


const std::regex name_format_("[a-zA-Z][a-zA-Z0-9]*");
const std::regex integer_format_("[0-9]+");
const std::regex identity_format_("\"\\s*[a-zA-Z][a-zA-Z0-9]*\\s*\"");
const std::regex declaration_format_("(stmt|read|print|while|if|assign|variable|constant|procedure)\\s+[a-zA-Z][a-zA-Z0-9]*\\s*(\\,\\s*[a-zA-Z][a-zA-Z0-9]*)*\\s*");
const std::regex clause_select_format_("Select\\s+[a-zA-Z][a-zA-Z0-9]*.*");
const std::regex clause_relation_format_("(Follows|Follows\\*|Parent|Parent\\*|Uses|Modifies)\\s*\\(\\s*[a-zA-Z0-9_][a-zA-Z0-9]*\\s*,\\s*\"?\\s*[a-zA-Z0-9_][a-zA-Z0-9]*\\s*\"?\\s*\\)");
const std::regex clause_pattern_format_("pattern\\s+[a-zA-Z][a-zA-Z0-9]*\\s*\\(\\s*(_|\"?\\s*[a-zA-Z][a-zA-Z0-9]*\\s*\"?)\\s*,\\s*(_\\s*\"\\s*[^\\s].*\\s*\"\\s*_|_)\\s*\\)");
const std::regex stmt_ref_format_("([a-zA-Z][a-zA-Z0-9]*|_|[0-9]+)");
const std::regex ent_ref_format_("([a-zA-Z][a-zA-Z0-9]*|_|\"\\s*[a-zA-Z][a-zA-Z0-9]*\\s*\")");
const std::regex expression_spec_format_("(_\\s*\"\\s*([a-zA-Z][a-zA-Z0-9]*|[0-9]+)\\s*\"\\s*_|_)");

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

QueryNode QueryPreProcessor::createExpressionNode(EXPRESSION e) {
	/*
		expression-spec:    ‘_’ ‘"’ factor ‘"’ ‘_’ | ‘_’

		factor: var_name | const_value
	*/
	QueryNode exp_node = QueryNode();

	if (std::regex_match(e, std::regex("_"))) {
		// is wild card
		exp_node.setNodeType({ QueryNodeType::wild_card });
	}
	else {
		// partial matching of ast node
		int open_quote_index = e.find("\"");
		int close_quote_index = e.rfind("\"");
		std::string trimmed_exp = trimWhitespaces(e.substr(open_quote_index + 1,
			close_quote_index - open_quote_index - 1));
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

QueryNode QueryPreProcessor::createArgumentNode(PROCESSED_SYNONYMS proc_s, ARGUMENT arg) {
	QueryNode arg_node = QueryNode();

	if (std::regex_match(arg, name_format_)) {
		// argument is a synonym
		arg_node.setSynonymNode({ proc_s.find(arg)->second.getSynonymType() }, arg);
	}
	else if (std::regex_match(arg, std::regex("_"))) {
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

QueryNode QueryPreProcessor::createRelationNode(PROCESSED_SYNONYMS proc_s, RELATIONSHIP rel,
	ARGUMENT first_arg, ARGUMENT second_arg) {

	QueryNode relation_node = QueryNode();
	relation_node.setNodeType((NODE_TYPE_STRING)rel);

	QueryNode first_arg_node = createArgumentNode(proc_s, first_arg);
	QueryNode second_arg_node = createArgumentNode(proc_s, second_arg);

	QueryNode relation_node_children[2] = { first_arg_node, second_arg_node };
	relation_node.setChildren(relation_node_children, 2);

	return relation_node;
}

QueryNode QueryPreProcessor::createPatternNode(PROCESSED_SYNONYMS proc_s, SYNONYM_NAME s,
	ARGUMENT first_arg, ARGUMENT second_arg) {

	QueryNode pattern_node = QueryNode();
	pattern_node.setNodeType({ QueryNodeType::pattern });

	QueryNode syn_node = createArgumentNode(proc_s, s);
	QueryNode first_arg_node = createArgumentNode(proc_s, first_arg);
	QueryNode second_arg_node = createExpressionNode(second_arg);

	QueryNode pattern_node_children[3] = { syn_node, first_arg_node, second_arg_node };
	pattern_node.setChildren(pattern_node_children, 3);

	return pattern_node;

}

VALIDATION_RESULT QueryPreProcessor::isValidStructure(QUERY q) {
	/*
	Validation rules:
		- Query is not empty
		- Has a declaration
		- Has 'Select'
	*/
	if (q.length() <= 0) {
		return false;
	}
	else if (q.find(";") == -1) {
		return false;
	}
	else if (q.find("Select") == -1) {
		return false;
	}
	else {
		return true;
	}
}

VALIDATION_RESULT QueryPreProcessor::isValidDeclaration(SINGLE_DECLARATION single_d) {
	/*
	Validation rules:
		- Declaration must not be empty
		- Follows the format: design-entity synonym (‘,’ synonym)* ‘;’
		- Declared design entityy is valid
		- Synonym names follows the format: LETTER ( LETTER | DIGIT )*
	*/
	if (single_d.length() <= 0) {
		return false;
	}
	else if (!std::regex_match(single_d, declaration_format_)) {
		return false;
	}
	else {
		return true;
	}
}

VALIDATION_RESULT QueryPreProcessor::isValidClause(CLAUSES c) {
	/*
	Validation rules:
		- There is a synonym after 'Select'
		- Has at most one 'such that' and 'pattern' clauses
	*/
	if (!std::regex_match(c, clause_select_format_)) {
		return false;
	}
	else if (c.find("such that", c.find("such that") + 1) != -1) {
		return false;
	}
	else if (c.find("pattern", c.find("pattern") + 1) != -1) {
		return false;
	}
	else {
		return true;
	}

}

VALIDATION_RESULT QueryPreProcessor::isSynonymDeclared(PROCESSED_SYNONYMS proc_s, SYNONYM_NAME s) {
	return (proc_s.find(s) != proc_s.end());
}

VALIDATION_RESULT QueryPreProcessor::isValidRelationFormat(SINGLE_CLAUSE single_c) {
	/*
	Validation rules:
		- Check if declared relationship is valid
		- Check if relationship has correct format+number of arguments
	*/
	if (!std::regex_match(single_c, clause_relation_format_)) {
		return false;
	}
	else {
		return true;
	}
}

VALIDATION_RESULT QueryPreProcessor::isStatementArgument(PROCESSED_SYNONYMS proc_s, ARGUMENT a) {
	/*
	List of synonyms that return statement number:
		- stmt
		- read
		- print
		- call
		- while
		- if
		- assign
	*/
	if (!std::regex_match(a, stmt_ref_format_)) {
		return false;
	}
	else if (std::regex_match(a, std::regex("_"))) {
		return true;
	}
	else if (std::regex_match(a, integer_format_)) {
		return true;
	}
	else if (std::regex_match(a, name_format_)) {
		if (proc_s.find(a)->second.getSynonymType() == QuerySynonymType::stmt) {
			return true;
		}
		else if (proc_s.find(a)->second.getSynonymType() == QuerySynonymType::read) {
			return true;
		}
		else if (proc_s.find(a)->second.getSynonymType() == QuerySynonymType::print) {
			return true;
		}
		else if (proc_s.find(a)->second.getSynonymType() == QuerySynonymType::call) {
			return true;
		}
		else if (proc_s.find(a)->second.getSynonymType() == QuerySynonymType::whiles) {
			return true;
		}
		else if (proc_s.find(a)->second.getSynonymType() == QuerySynonymType::ifs) {
			return true;
		}
		else if (proc_s.find(a)->second.getSynonymType() == QuerySynonymType::assign) {
			return true;
		}
		else {
			return false;
		}
	}
	else {
		return false;
	}
}

VALIDATION_RESULT QueryPreProcessor::isValidRelationArguments(PROCESSED_SYNONYMS proc_s, RELATIONSHIP rel,
	ARGUMENT first_arg, ARGUMENT second_arg) {
	/*
	Validation rules:
		- If argument is a synonym, check if it has been declared
		- Check if type of arguments are correct
			- Follows and FollowsT can only have 'stmt' as design entity
			- Parent and ParentT can only have 'stmt' as design entity
			- If arguments of UsesS are synonyms, only the following combinations are allowed:
					- assign, variable
					- print, variable
					- if, variable
					- while, variable
					- procedure, variable
					- call, variable
					*note second arguments are all variables
			- If arguments of ModifiesS are synonyms, only the following combinations are allowed:
					- assign, variable
					- read, variable
					- if, variable
					- while, variable
					- procedure, variable
					- call, variable
					*note second arguments are all variables
			- Modifies and Uses can not have '_' as first argument
	*/

	if (std::regex_match(first_arg, name_format_) && !isSynonymDeclared(proc_s, first_arg)) {
		return false;
	}
	else if (std::regex_match(second_arg, name_format_) && !isSynonymDeclared(proc_s, second_arg)) {
		return false;
	}

	if (std::regex_match(rel, std::regex("Follows")) || std::regex_match(rel, std::regex("Follows\\*"))) {
		if (!isStatementArgument(proc_s, first_arg)) {
			return false;
		}
		else if (!isStatementArgument(proc_s, second_arg)) {
			return false;
		}
		else {
			return true;
		}
	}
	else if (std::regex_match(rel, std::regex("Parent")) || std::regex_match(rel, std::regex("Parent\\*"))) {
		if (!isStatementArgument(proc_s, first_arg)) {
			return false;
		}
		else if (!isStatementArgument(proc_s, second_arg)) {
			return false;
		}
		else {
			return true;
		}
	}
	else if (std::regex_match(rel, std::regex("Uses"))) {
		if (std::regex_match(first_arg, std::regex("_"))) {
			return false;
		}
		else if (!std::regex_match(first_arg, stmt_ref_format_) || !std::regex_match(second_arg, ent_ref_format_)) {
			return false;
		}
		else if (std::regex_match(first_arg, name_format_)) {
			if (std::regex_match(second_arg, std::regex("_"))) {
				return true;
			}
			else if (std::regex_match(second_arg, identity_format_)) {
				return true;
			}
			else if (proc_s.find(second_arg)->second.getSynonymType() == QuerySynonymType::variable) {
				if (proc_s.find(first_arg)->second.getSynonymType() == QuerySynonymType::read) {
					return false;
				}
				else if (isStatementArgument(proc_s, first_arg)) {
					return true;
				}
				else if (proc_s.find(first_arg)->second.getSynonymType() == QuerySynonymType::procedure) {
					return true;
				}
				else {
					return false;
				}
			}
			else {
				return false;
			}
		}
		else {
			return true;
		}
	}
	else if (std::regex_match(rel, std::regex("Modifies"))) {
		if (std::regex_match(first_arg, std::regex("_"))) {
			return false;
		}
		else if (!std::regex_match(first_arg, stmt_ref_format_) || !std::regex_match(second_arg, ent_ref_format_)) {
			return false;
		}
		else if (std::regex_match(first_arg, name_format_)) {
			if (std::regex_match(second_arg, std::regex("_"))) {
				return true;
			} 
			else if (std::regex_match(second_arg, identity_format_)) {
				return true;
			}
			else if (proc_s.find(second_arg)->second.getSynonymType() == QuerySynonymType::variable) {
				if (proc_s.find(first_arg)->second.getSynonymType() == QuerySynonymType::print) {
					return false;
				}
				else if (isStatementArgument(proc_s, first_arg)) {
					return true;
				}
				else if (proc_s.find(first_arg)->second.getSynonymType() == QuerySynonymType::procedure) {
					return true;
				}
				else {
					return false;
				}
			}
			else {
				return false;
			}
		}
		else {
			return true;
		}
	}

	return false;
}

VALIDATION_RESULT QueryPreProcessor::isValidPatternFormat(SINGLE_CLAUSE single_c) {
	/*
	Validation rules:
		- Check if declared relationship is valid
		- Check if relationship has correct format+number of arguments
	*/
	if (!std::regex_match(single_c, clause_pattern_format_)) {
		return false;
	}
	else {
		return true;
	}

}

VALIDATION_RESULT QueryPreProcessor::isValidPatternArguments(PROCESSED_SYNONYMS proc_s, SYNONYM_NAME s,
	ARGUMENT first_arg, ARGUMENT second_arg) {
	/*
	Validation rules:
		- Check if synonym has been declared
		- Check if synonym is declared as 'assign'
		- If first argument is synonym, check if it has been declared
		- Check if type of arguments are correct
	*/

	if (!isSynonymDeclared(proc_s, s)) {
		return false;
	}
	else if (proc_s.find(s)->second.getSynonymType() != QuerySynonymType::assign) {
		return false;
	}
	else if (std::regex_match(first_arg, name_format_) && !isSynonymDeclared(proc_s, first_arg)) {
		return false;
	}
	else if (!std::regex_match(first_arg, ent_ref_format_) || !std::regex_match(second_arg, expression_spec_format_)) {
		return false;
	}
	else {
		return true;
	}

}

SPLIT_QUERY QueryPreProcessor::splitQuery(QUERY q) {
	SPLIT_QUERY split_q;
	DECLARATIONS d = "";
	CLAUSES c = "";

	if (isValidStructure(q)) {
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

		if (!isValidDeclaration(single_d)) {
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

	if (isValidClause(c)) {
		select_node.setNodeType({ QueryNodeType::select });

		SYNONYM_NAME select_syn;
		int such_that_index = c.find("such that");
		int pattern_index = c.find("pattern");
		int next_index = getNextClauseIndex(c, 0, such_that_index, pattern_index);

		if (next_index == -1) {
			// no such that nor pattern clause
			select_syn = trimWhitespaces(c.substr(6));

			if (isSynonymDeclared(proc_s, select_syn)) {
				// create synonym node and set as child
				QueryNode select_syn_node = QueryNode();
				select_syn_node.setSynonymNode({ proc_s.find(select_syn)->second.getSynonymType() }, select_syn);
				QueryNode select_children[] = { select_syn_node };
				select_node.setChildren(select_children, 1);
			}
			else {
				is_valid = false;
			}
		}
		else {
			// select node will have maximum 3 children
			QueryNode select_children[3];
			int child_index = 0;

			// extract select synonym
			select_syn = trimWhitespaces(c.substr(6, next_index - 6));
			QueryNode select_syn_node = QueryNode();

			if (isSynonymDeclared(proc_s, select_syn)) {
				// create synonym node and set as first child
				select_syn_node.setSynonymNode({ proc_s.find(select_syn)->second.getSynonymType() }, select_syn);
				select_children[child_index] = select_syn_node;
				child_index++;
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

					if (!isValidRelationFormat(current_c)) {
						is_valid = false;
						break;
					}

					QueryNode such_that_node = QueryNode();
					such_that_node.setNodeType({ QueryNodeType::such_that });

					// might want to refactor this part into another func?
					int open_brac_index = current_c.find("(");
					int comma_index = current_c.find(",");
					int closed_brac_index = current_c.find(")");
					ARGUMENT first_arg = trimWhitespaces(current_c.substr(open_brac_index + 1,
						comma_index - open_brac_index - 1));
					ARGUMENT second_arg = trimWhitespaces(current_c.substr(comma_index + 1,
						closed_brac_index - comma_index - 1));

					// get relation
					RELATIONSHIP rel = trimWhitespaces(current_c.substr(0, open_brac_index));

					if (!isValidRelationArguments(proc_s, rel, first_arg, second_arg)) {
						is_valid = false;
						break;
					}

					QueryNode relation_node = createRelationNode(proc_s, rel, first_arg, second_arg);

					QueryNode such_that_node_children[1] = { relation_node };
					such_that_node.setChildren(such_that_node_children, 1);
					select_children[child_index] = such_that_node;
					child_index++;
				}
				else {
					SINGLE_CLAUSE current_c = trimWhitespaces(c.substr(next_index, such_that_index - next_index));

					if (!isValidPatternFormat(current_c)) {
						is_valid = false;
						break;
					}

					int open_brac_index = current_c.find("(");
					int comma_index = current_c.find(",");
					int closed_brac_index = current_c.find(")");
					ARGUMENT first_arg = trimWhitespaces(current_c.substr(open_brac_index + 1,
						comma_index - open_brac_index - 1));
					ARGUMENT second_arg = trimWhitespaces(current_c.substr(comma_index + 1,
						closed_brac_index - comma_index - 1));

					// get synonym
					int syn_start_index = current_c.find(" ");
					SYNONYM_NAME pattern_syn = trimWhitespaces(current_c.substr(syn_start_index,
						open_brac_index - syn_start_index));

					if (!isValidPatternArguments(proc_s, pattern_syn, first_arg, second_arg)) {
						is_valid = false;
						break;
					}

					QueryNode pattern_node = createPatternNode(proc_s, pattern_syn, first_arg, second_arg);

					select_children[child_index] = pattern_node;
					child_index++;
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
		PROCESSED_CLAUSES null_node = QueryNode();

		return null_node;
	}
}