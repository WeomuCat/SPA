#include <regex>
#include <string>

#include "QueryValidator.h"

const std::regex name_format_("[a-zA-Z][a-zA-Z0-9]*");
const std::regex integer_format_("[0-9]+");
const std::regex identity_format_("\"\\s*[a-zA-Z][a-zA-Z0-9]*\\s*\"");
const std::regex attr_rel_format_("[a-zA-Z][a-zA-Z0-9]*\\.(procName|varName|value|stmt#)");
const std::regex brac_tuple_format_("<\\s*([a-zA-Z][a-zA-Z0-9]*|[a-zA-Z][a-zA-Z0-9]*\\.(procName|varName|value|stmt#))(\\s*,\\s*([a-zA-Z][a-zA-Z0-9]*|[a-zA-Z][a-zA-Z0-9]*\\.(procName|varName|value|stmt#)))*\\s*>");
const std::regex declaration_format_("(stmt|read|print|while|if|assign|variable|constant|procedure)\\s+[a-zA-Z][a-zA-Z0-9]*\\s*(\\,\\s*[a-zA-Z][a-zA-Z0-9]*)*\\s*");
const std::regex clause_select_format_("Select\\s+([a-zA-Z][a-zA-Z0-9]*|<.*>).*");
const std::regex clause_relation_format_("(Follows|Follows\\*|Parent|Parent\\*|Uses|Modifies|Calls|Calls\\*|Next|Next\\*)\\s*\\(\\s*[a-zA-Z0-9_][a-zA-Z0-9]*\\s*,\\s*\"?\\s*[a-zA-Z0-9_][a-zA-Z0-9]*\\s*\"?\\s*\\)");
const std::regex clause_pattern_format_("pattern\\s+[a-zA-Z][a-zA-Z0-9]*\\s*\\(\\s*(_|\"?\\s*[a-zA-Z][a-zA-Z0-9]*\\s*\"?)\\s*,\\s*(_\\s*\"\\s*[^\\s].*\\s*\"\\s*_|_)\\s*\\)");
const std::regex stmt_ref_format_("([a-zA-Z][a-zA-Z0-9]*|_|[0-9]+)");
const std::regex ent_ref_format_("([a-zA-Z][a-zA-Z0-9]*|_|\"\\s*[a-zA-Z][a-zA-Z0-9]*\\s*\")");
const std::regex line_ref_format_("([a-zA-Z][a-zA-Z0-9]*|_|[0-9]+)");
const std::regex expression_spec_format_("(_\\s*\"\\s*([a-zA-Z][a-zA-Z0-9]*|[0-9]+)\\s*\"\\s*_|_)");

/*
Validation rules:
		- Query is not empty
		- Has a declaration
		- Has 'Select'
*/
VALIDATION_RESULT QueryValidator ::isValidStructure(QUERY q) {
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

/*
Validation rules:
	- Declaration must not be empty
	- Follows the format: design-entity synonym (‘,’ synonym)* ‘;’
	- Declared design entity is valid
	- Synonym names follows the format: LETTER ( LETTER | DIGIT )*
*/
VALIDATION_RESULT QueryValidator::isValidDeclaration(SINGLE_DECLARATION single_d) {
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

/*
Validation rules:
	- There is a result clause after 'Select'
	- Has at most one 'such that' and 'pattern' clauses
*/
VALIDATION_RESULT QueryValidator::isValidClause(CLAUSES c) {
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

/*
Validation rules:
	- Elements should either be a:
		- synonym
		- attribute reference
	- If element is a synonym, it has been declared
	- If element is an attribute reference:
		- synonym is declared
		- declared attribute name is valid
*/
VALIDATION_RESULT QueryValidator::isValidElem(PROCESSED_SYNONYMS proc_s, ELEMENT elem) {
	if (std::regex_match(elem, name_format_) && QueryValidator::isSynonymDeclared(proc_s, elem)) {
		return true;
	}
	else if (std::regex_match(elem, attr_rel_format_) && QueryValidator::isSynonymDeclared(proc_s, elem.substr(0, elem.find(".")))) {
		return true;
	}
	else {
		return false;
	}
}

/*
Validation rules:
	- Clause should be either a boolean or a tuple
	- Tuples should follow the format: elem | ‘<’ elem ( ‘,’ elem )* ‘>’,
		where elem: synonym | attrRef
*/
VALIDATION_RESULT QueryValidator::isValidResultFormat(RESULT res) {
	if (std::regex_match(res, std::regex("BOOLEAN"))) {
		return true;
	}
	else if (std::regex_match(res, name_format_) || std::regex_match(res, attr_rel_format_)) {
		return true;
	}
	else if (std::regex_match(res, brac_tuple_format_)) {
		return true;
	}
	else {
		return false;
	}
}

/*
Validation rules:
	- Check if declared relationship is valid
	- Check if relationship has correct format+number of arguments
*/
VALIDATION_RESULT QueryValidator::isValidRelationFormat(SINGLE_CLAUSE single_c) {
	if (!std::regex_match(single_c, clause_relation_format_)) {
		return false;
	}
	else {
		return true;
	}
}

/*
List of synonyms that return statement number:
	- stmt
	- read
	- print
	- call
	- while
	- if
	- assign
	- prog_line
*/
VALIDATION_RESULT QueryValidator::isStatementRef(PROCESSED_SYNONYMS proc_s, ARGUMENT a) {
	if (!std::regex_match(a, stmt_ref_format_)) {
		return false;
	}
	else if (a.compare("_") == 0) {
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
		else if (proc_s.find(a)->second.getSynonymType() == QuerySynonymType::prog_line) {
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

/*
List of synonyms that return entity refernces:
	- variable
	- procedure
*/
VALIDATION_RESULT QueryValidator::isEntityRef(PROCESSED_SYNONYMS proc_s, ARGUMENT a) {
	if (!std::regex_match(a, ent_ref_format_)) {
		return false;
	}
	else if (a.compare("_") == 0) {
		return true;
	}
	else if (std::regex_match(a, identity_format_)) {
		return true;
	}
	else if (std::regex_match(a, name_format_)) {
		if (proc_s.find(a)->second.getSynonymType() == QuerySynonymType::variable) {
			return true;
		}
		else if (proc_s.find(a)->second.getSynonymType() == QuerySynonymType::procedure) {
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

/*
List of synonyms that return line refernces:
	- prog_line
*/
VALIDATION_RESULT QueryValidator::isLineRef(PROCESSED_SYNONYMS proc_s, ARGUMENT a) {
	if (!std::regex_match(a, line_ref_format_)) {
		return false;
	}
	else if (a.compare("_") == 0) {
		return true;
	}
	else if (std::regex_match(a, integer_format_)) {
		return true;
	}
	else if (std::regex_match(a, name_format_)) {
		if (proc_s.find(a)->second.getSynonymType() == QuerySynonymType::prog_line) {
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

/*
Validation rules:
	- If argument is a synonym, check if it has been declared
	- Check if type of arguments are correct
		- Follows and FollowsT can only have statement references as arguments
		- Parent and ParentT can only have statement references as arguments
		- UsesS can only have:
			- statement references (excluding read & ‘_’) as their first argument
			- entity references as their second argument
		- UsesP can only have:
			- entity references (excluding variable & ‘_’) as their first argument
			- entity references as their second argument
		- ModifiesS can only have:
			- statement references (excluding print & ‘_’) as their first argument
			- entity references as their second argument
		- ModifiesP can only have:
			- entity references (excluding variable & ‘_’) as their first argument
			- entity references as their second argument
		- Calls and CallsT can only have entity references (excluding variable) as arguments
		- Next and NextT can only have line references as arguments

*/
VALIDATION_RESULT QueryValidator::isValidRelationArguments(PROCESSED_SYNONYMS proc_s, RELATIONSHIP rel,
	ARGUMENT first_arg, ARGUMENT second_arg) {

	if (std::regex_match(first_arg, name_format_) && !isSynonymDeclared(proc_s, first_arg)) {
		return false;
	}
	else if (std::regex_match(second_arg, name_format_) && !isSynonymDeclared(proc_s, second_arg)) {
		return false;
	}

	if (rel.compare("Follows") == 0 || rel.compare("Follows*") == 0) {
		if (!isStatementRef(proc_s, first_arg)) {
			return false;
		}
		else if (!isStatementRef(proc_s, second_arg)) {
			return false;
		}
		else {
			return true;
		}
	}
	else if (rel.compare("Parent") == 0 || rel.compare("Parent*") == 0) {
		if (!isStatementRef(proc_s, first_arg)) {
			return false;
		}
		else if (!isStatementRef(proc_s, second_arg)) {
			return false;
		}
		else {
			return true;
		}
	}
	else if (rel.compare("Uses") == 0) {
		if (first_arg.compare("_") == 0) {
			return false;
		}
		else if (!std::regex_match(first_arg, stmt_ref_format_) && !std::regex_match(first_arg, ent_ref_format_)) {
			return false;
		}
		else if (!std::regex_match(second_arg, ent_ref_format_)) {
			return false;
		}
		else if (std::regex_match(first_arg, name_format_)) {
			if (second_arg.compare("_") == 0) {
				return true;
			}
			else if (std::regex_match(second_arg, identity_format_)) {
				return true;
			}
			else if (proc_s.find(second_arg)->second.getSynonymType() == QuerySynonymType::variable) {
				if (proc_s.find(first_arg)->second.getSynonymType() == QuerySynonymType::read) {
					return false;
				}
				else if (isStatementRef(proc_s, first_arg)) {
					return true;
				}
				else if (proc_s.find(first_arg)->second.getSynonymType() == QuerySynonymType::variable) {
					return false;
				}
				else if (isEntityRef(proc_s, first_arg)) {
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
	else if (rel.compare("Modifies") == 0) {
		if (first_arg.compare("_") == 0) {
			return false;
		}
		else if (!std::regex_match(first_arg, stmt_ref_format_) && !std::regex_match(first_arg, ent_ref_format_)) {
			return false;
		}
		else if (!std::regex_match(second_arg, ent_ref_format_)) {
			return false;
		}
		else if (std::regex_match(first_arg, name_format_)) {
			if (second_arg.compare("_") == 0) {
				return true;
			}
			else if (std::regex_match(second_arg, identity_format_)) {
				return true;
			}
			else if (proc_s.find(second_arg)->second.getSynonymType() == QuerySynonymType::variable) {
				if (proc_s.find(first_arg)->second.getSynonymType() == QuerySynonymType::print) {
					return false;
				}
				else if (isStatementRef(proc_s, first_arg)) {
					return true;
				}
				else if (proc_s.find(first_arg)->second.getSynonymType() == QuerySynonymType::variable) {
					return false;
				}
				else if (isEntityRef(proc_s, first_arg)) {
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
	else if (rel.compare("Calls") == 0 || rel.compare("Calls*") == 0) {
		if (!isEntityRef(proc_s, first_arg)) {
			return false;
		}
		else if (!isEntityRef(proc_s, second_arg)) {
			return false;
		}
		if (std::regex_match(first_arg, name_format_) && proc_s.find(first_arg)->second.getSynonymType() == QuerySynonymType::variable) {
			return false;
		}
		else if (std::regex_match(second_arg, name_format_) && proc_s.find(second_arg)->second.getSynonymType() == QuerySynonymType::variable) {
			return false;
		}
		else {
			return true;
		}
	}
	else if (rel.compare("Next") == 0 || rel.compare("Next*") == 0) {
	if (!isLineRef(proc_s, first_arg)) {
		return false;
	}
	else if (!isLineRef(proc_s, second_arg)) {
		return false;
	}
	else {
		return true;
	}
	}

	return false;
}

/*
Validation rules:
	- Check if pattern has correct format+number of arguments
*/
VALIDATION_RESULT QueryValidator::isValidPatternFormat(SINGLE_CLAUSE single_c) {
	if (!std::regex_match(single_c, clause_pattern_format_)) {
		return false;
	}
	else {
		return true;
	}

}

/*
Validation rules:
	- Check if synonym has been declared
	- Check if synonym is declared as 'assign'
	- If first argument is synonym, check if it has been declared
	- Check if type of arguments are correct
*/
VALIDATION_RESULT QueryValidator::isValidPatternArguments(PROCESSED_SYNONYMS proc_s, SYNONYM_NAME s,
	ARGUMENT first_arg, ARGUMENT second_arg) {

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

/*
Validation rules:
	- 
*/
static VALIDATION_RESULT isValidWithFormat(SINGLE_CLAUSE single_c) {
	return true;
}

/*
Validation rules:
	- 
*/
static VALIDATION_RESULT isValidRef(PROCESSED_SYNONYMS proc_s, ARGUMENT a) {
	return true;
}