#include <regex>
#include <string>

#include "QueryValidator.h"

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

VALIDATION_RESULT QueryValidator ::isValidStructure(QUERY q) {
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

VALIDATION_RESULT QueryValidator::isValidDeclaration(SINGLE_DECLARATION single_d) {
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

VALIDATION_RESULT QueryValidator::isValidClause(CLAUSES c) {
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

VALIDATION_RESULT QueryValidator::isSynonymDeclared(PROCESSED_SYNONYMS proc_s, SYNONYM_NAME s) {
	return (proc_s.find(s) != proc_s.end());
}

VALIDATION_RESULT QueryValidator::isValidRelationFormat(SINGLE_CLAUSE single_c) {
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

VALIDATION_RESULT QueryValidator::isStatementArgument(PROCESSED_SYNONYMS proc_s, ARGUMENT a) {
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

VALIDATION_RESULT QueryValidator::isValidRelationArguments(PROCESSED_SYNONYMS proc_s, RELATIONSHIP rel,
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

VALIDATION_RESULT QueryValidator::isValidPatternFormat(SINGLE_CLAUSE single_c) {
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

VALIDATION_RESULT QueryValidator::isValidPatternArguments(PROCESSED_SYNONYMS proc_s, SYNONYM_NAME s,
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