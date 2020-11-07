#include "QueryValidator.h"

const std::regex name_format_("[a-zA-Z][a-zA-Z0-9]*");
const std::regex integer_format_("[0-9]+");
const std::regex identity_format_("\"\\s*[a-zA-Z][a-zA-Z0-9]*\\s*\"");
const std::regex attr_ref_format_("[a-zA-Z][a-zA-Z0-9]*\\.(procName|varName|value|stmt#)");
const std::regex brac_tuple_format_("<\\s*([a-zA-Z][a-zA-Z0-9]*|[a-zA-Z][a-zA-Z0-9]*\\.(procName|varName|value|stmt#))(\\s*,\\s*([a-zA-Z][a-zA-Z0-9]*|[a-zA-Z][a-zA-Z0-9]*\\.(procName|varName|value|stmt#)))*\\s*>");
const std::regex declaration_format_("(stmt|read|print|call|while|if|assign|variable|constant|prog_line|procedure)\\s+[a-zA-Z][a-zA-Z0-9]*\\s*(\\,\\s*[a-zA-Z][a-zA-Z0-9]*)*\\s*");

const std::regex clause_select_format_("Select\\s+([a-zA-Z][a-zA-Z0-9]*|<.*>).*");
const std::regex clause_relation_format_("(Follows|Follows\\*|Parent|Parent\\*|Uses|Modifies|Calls|Calls\\*|Next|Next\\*|Affects|Affects\\*)\\s*\\(\\s*\"?\\s*[a-zA-Z0-9_][a-zA-Z0-9]*\\s*\"?\\s*,\\s*\"?\\s*[a-zA-Z0-9_][a-zA-Z0-9]*\\s*\"?\\s*\\)");
const std::regex clause_pattern_assign_format_("[a-zA-Z][a-zA-Z0-9]*\\s*\\(\\s*(_|\"?\\s*[a-zA-Z][a-zA-Z0-9]*\\s*\"?)\\s*,\\s*(\"\\s*[^\\s].*\\s*\"|_\\s*\"\\s*[^\\s].*\\s*\"\\s*_|_)\\s*\\)");
const std::regex clause_pattern_if_format_("[a-zA-Z][a-zA-Z0-9]*\\s*\\(\\s*(_|\"?\\s*[a-zA-Z][a-zA-Z0-9]*\\s*\"?)\\s*,\\s*_\\s*,\\s*_\\s*\\)");
const std::regex clause_pattern_while_format_("[a-zA-Z][a-zA-Z0-9]*\\s*\\(\\s*(_|\"?\\s*[a-zA-Z][a-zA-Z0-9]*\\s*\"?)\\s*,\\s*_\\s*\\)");
const std::regex clause_with_format_("(\"\\s*[a-zA-Z][a-zA-Z0-9]*\\s*\"|[0-9]+|[a-zA-Z][a-zA-Z0-9]*(.(procName|varName|value|stmt#))?)\\s*=\\s*(\"\\s*[a-zA-Z][a-zA-Z0-9]*\\s*\"|[0-9]+|[a-zA-Z][a-zA-Z0-9]*(.(procName|varName|value|stmt#))?)");

const std::regex stmt_ref_format_("([a-zA-Z][a-zA-Z0-9]*|_|[0-9]+)");
const std::regex ent_ref_format_("([a-zA-Z][a-zA-Z0-9]*|_|\"\\s*[a-zA-Z][a-zA-Z0-9]*\\s*\")");
const std::regex line_ref_format_("([a-zA-Z][a-zA-Z0-9]*|_|[0-9]+)");
const std::regex expression_spec_format_("(\"\\s*([a-zA-Z][a-zA-Z0-9]*|[0-9]+)\\s*\"|_\\s*\"\\s*([a-zA-Z][a-zA-Z0-9]*|[0-9]+)\\s*\"\\s*_|_)");

/*
Validation rules:
		- Query is not empty
		- Has 'Select'
		- If is not 'Select BOOLEAN', should have declarations
*/
VALIDATION_RESULT QueryValidator ::isValidStructure(INPUT_QUERY q) {
	if (q.length() <= 0) {
		return false;
	}
	else if (q.find("Select") == -1) {
		return false;
	}
	else if (q.find("BOOLEAN") == -1 && q.find(";") == -1) {
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
*/
VALIDATION_RESULT QueryValidator::isValidClause(CLAUSES c) {
	if (!std::regex_match(c, clause_select_format_)) {
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
	- procName can only belong to:
		- procedure
		- call
	- varName can only belong to:
		- variable
		- read
		- print
	- value can only belong to:
		- constant
	- stmt# can only belong to:
		- stmt
		- read
		- print
		- call
		- while
		- if
		- assign
*/
VALIDATION_RESULT QueryValidator::isValidAttr(PROCESSED_SYNONYMS proc_s, SINGLE_ARGUMENT a) {
	SYNONYM_NAME s = a.substr(0, a.find("."));
	ATTRIBUTE_STRING attr_name = a.substr(a.find(".") + 1);

	if (!isSynonymDeclared(proc_s, s)) {
		return false;
	}
	else if (attr_name.compare("procName") == 0) {
		if (proc_s.find(s)->second.getSynonymType() == QuerySynonymType::procedure) {
			return true;
		}
		else if (proc_s.find(s)->second.getSynonymType() == QuerySynonymType::call) {
			return true;
		}
		else {
			return false;
		}
	}
	else if (attr_name.compare("varName") == 0) {
		if (proc_s.find(s)->second.getSynonymType() == QuerySynonymType::variable) {
			return true;
		}
		else if (proc_s.find(s)->second.getSynonymType() == QuerySynonymType::read) {
			return true;
		}
		else if (proc_s.find(s)->second.getSynonymType() == QuerySynonymType::print) {
			return true;
		}
		else {
			return false;
		}
	}
	else if (attr_name.compare("value") == 0) {
		if (proc_s.find(s)->second.getSynonymType() == QuerySynonymType::constant) {
			return true;
		}
		else {
			return false;
		}
	}
	else if (attr_name.compare("stmt#") == 0) {
		if (proc_s.find(s)->second.getSynonymType() == QuerySynonymType::stmt) {
			return true;
		}
		else if (proc_s.find(s)->second.getSynonymType() == QuerySynonymType::read) {
			return true;
		}
		else if (proc_s.find(s)->second.getSynonymType() == QuerySynonymType::print) {
			return true;
		}
		else if (proc_s.find(s)->second.getSynonymType() == QuerySynonymType::call) {
			return true;
		}
		else if (proc_s.find(s)->second.getSynonymType() == QuerySynonymType::whiles) {
			return true;
		}
		else if (proc_s.find(s)->second.getSynonymType() == QuerySynonymType::ifs) {
			return true;
		}
		else if (proc_s.find(s)->second.getSynonymType() == QuerySynonymType::assign) {
			return true;
		}
		else {
			return false;
		}
	}
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
	else if (std::regex_match(elem, attr_ref_format_) && isValidAttr(proc_s, elem)) {
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
	else if (std::regex_match(res, name_format_) || std::regex_match(res, attr_ref_format_)) {
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
VALIDATION_RESULT QueryValidator::isStatementRef(PROCESSED_SYNONYMS proc_s, SINGLE_ARGUMENT a) {
	if (!std::regex_match(a, stmt_ref_format_)) {
		return false;
	}
	else if (a.compare("_") == 0) {
		return true;
	}
	else if (std::regex_match(a, integer_format_)) {
		return true;
	}
	else if (std::regex_match(a, name_format_) && isSynonymDeclared(proc_s, a)) {
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
VALIDATION_RESULT QueryValidator::isEntityRef(PROCESSED_SYNONYMS proc_s, SINGLE_ARGUMENT a) {
	if (!std::regex_match(a, ent_ref_format_)) {
		return false;
	}
	else if (a.compare("_") == 0) {
		return true;
	}
	else if (std::regex_match(a, identity_format_)) {
		return true;
	}
	else if (std::regex_match(a, name_format_) && isSynonymDeclared(proc_s, a)) {
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
VALIDATION_RESULT QueryValidator::isLineRef(PROCESSED_SYNONYMS proc_s, SINGLE_ARGUMENT a) {
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
		else if (proc_s.find(a)->second.getSynonymType() == QuerySynonymType::stmt) {
			return true;
		}
		else if (proc_s.find(a)->second.getSynonymType() == QuerySynonymType::assign) {
			return true;
		}
		else if (proc_s.find(a)->second.getSynonymType() == QuerySynonymType::call) {
			return true;
		}
		else if (proc_s.find(a)->second.getSynonymType() == QuerySynonymType::read) {
			return true;
		}
		else if (proc_s.find(a)->second.getSynonymType() == QuerySynonymType::print) {
			return true;
		}
		else if (proc_s.find(a)->second.getSynonymType() == QuerySynonymType::whiles) {
			return true;
		}
		else if (proc_s.find(a)->second.getSynonymType() == QuerySynonymType::ifs) {
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
	- Check if type of arguments are correct:
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
		- Affects and AffectsT can only have statement references as arguments

*/
VALIDATION_RESULT QueryValidator::isValidRelationArguments(PROCESSED_SYNONYMS proc_s, RELATIONSHIP rel,
	ARGUMENTS args) {

	SINGLE_ARGUMENT first_arg = args[0];
	SINGLE_ARGUMENT second_arg = args[1];

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
		else if (!isEntityRef(proc_s, second_arg)) {
			return false;
		}
		else if (std::regex_match(second_arg, name_format_) && proc_s.find(second_arg)->second.getSynonymType() == QuerySynonymType::procedure) {
			return false;
		}
		else if (std::regex_match(first_arg, name_format_)) {
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
		else if (!isEntityRef(proc_s, second_arg)) {
			return false;
		}
		else if (std::regex_match(second_arg, name_format_) && proc_s.find(second_arg)->second.getSynonymType() == QuerySynonymType::procedure) {
			return false;
		}
		else if (std::regex_match(first_arg, name_format_)) {
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
	else if (rel.compare("Affects") == 0 || rel.compare("Affects*") == 0) {
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

	return false;
}

/*
Validation rules:
	- Check if pattern has correct format+number of arguments
	* note pattern-while format is a subset of pattern-assign format
*/
VALIDATION_RESULT QueryValidator::isValidPatternFormat(SINGLE_CLAUSE single_c) {
	if (!std::regex_match(single_c, clause_pattern_assign_format_) && !std::regex_match(single_c, clause_pattern_if_format_)) {
		return false;
	}
	else {
		return true;
	}

}

/*
Validation rules:
	- Check if synonym has been declared
	- Synonym should be either of the following types:
		- assign
		- if
		- while
	- Check if type of arguments are correct:
		- pattern-assign can only have:
			- entity references as their first argument
			- expressions as their second argument (might validate elsewhere)
		- pattern-if can only have:
			- entity references as their first argument
			- wild cards as their second argument
			- wild cards as their third argument
		- pattern-while can only have:
			- entity references as their first argument
			- wild cards as their second argument
*/
VALIDATION_RESULT QueryValidator::isValidPatternArguments(PROCESSED_SYNONYMS proc_s, SYNONYM_NAME s,
	ARGUMENTS args) {

	int args_no = args.size();
	SINGLE_ARGUMENT first_arg = args[0];
	SINGLE_ARGUMENT second_arg = args[1];

	if (!isSynonymDeclared(proc_s, s)) {
		return false;
	}
	else if (proc_s.find(s)->second.getSynonymType() == QuerySynonymType::assign && args_no == 2) {
		if (isEntityRef(proc_s, first_arg) && !std::regex_match(second_arg, integer_format_) && 
			!std::regex_match(second_arg, name_format_) && !std::regex_match(second_arg, attr_ref_format_)) {
			return true;
		}
		else {
			return false;
		}
	}
	else if (proc_s.find(s)->second.getSynonymType() == QuerySynonymType::ifs && args_no == 3) {
		SINGLE_ARGUMENT third_arg = args[2];
		if (isEntityRef(proc_s, first_arg) && second_arg.compare("_") == 0 && third_arg.compare("_") == 0) {
			return true;
		}
		else {
			return false;
		}
	}
	else if (proc_s.find(s)->second.getSynonymType() == QuerySynonymType::whiles && args_no == 2) {
		if (isEntityRef(proc_s, first_arg) && second_arg.compare("_") == 0) {
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
	- first two elements are values
	- last element is an operator
	- there are n values and n-1 operators

	alternatively,
	- stack does not underflow
	- at the end, only one value is left on the stack
*/
VALIDATION_RESULT QueryValidator::isValidPostfixExpr(POSTFIX_EXPR e) {
	int counter = 0;
	bool is_valid = true;

	for (int i = 0; i < e.size(); i++) {
		TOKEN t = e[i];

		if (std::regex_match(t, name_format_) || std::regex_match(t, integer_format_)) {
			counter++;
		}
		else {
			counter = counter - 2;

			if (counter < 0) {
				is_valid = false;
				break;
			}

			counter++;
		}
	}

	if (counter != 1) {
		is_valid = false;
	}

	return is_valid;
}

/*
Validation rules:
	- Check if pattern has correct format+number of references
*/
VALIDATION_RESULT QueryValidator::isValidWithFormat(SINGLE_CLAUSE single_c) {
	if (!std::regex_match(single_c, clause_with_format_)) {
		return false;
	}
	else {
		return true;
	}
}

/*
Validation rules:
	- Arguments should have matching value types
		- procName: NAME
		- varName: NAME
		- value: INTEGER
		- stmt#: INTEGER
*/
VALIDATION_RESULT QueryValidator::isValidWithArguments(PROCESSED_SYNONYMS proc_s, ARGUMENTS args) {
	SINGLE_ARGUMENT first_arg = args[0];
	SINGLE_ARGUMENT second_arg = args[1];

	if (std::regex_match(first_arg, integer_format_)) {
		if (isStatementRef(proc_s, second_arg)) {
			return true;
		}
		else if (std::regex_match(second_arg, attr_ref_format_) && isValidAttr(proc_s, second_arg)) {
			ATTRIBUTE_STRING attr_name = second_arg.substr(second_arg.find(".") + 1);

			if (attr_name.compare("value") == 0 || attr_name.compare("stmt#") == 0) {
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
	else if (std::regex_match(first_arg, identity_format_)) {
		if (isEntityRef(proc_s, second_arg)) {
			return true;
		}
		else if (std::regex_match(second_arg, attr_ref_format_) && isValidAttr(proc_s, second_arg)) {
			ATTRIBUTE_STRING attr_name = second_arg.substr(second_arg.find(".") + 1);

			if (attr_name.compare("procName") == 0 || attr_name.compare("varName") == 0) {
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
	else if (std::regex_match(first_arg, name_format_)) {
		if (isEntityRef(proc_s, first_arg)) {
			// entity ref synonym
			if (isEntityRef(proc_s, second_arg)) {
				return true;
			}
			else if (std::regex_match(second_arg, attr_ref_format_) && isValidAttr(proc_s, second_arg)) {
				ATTRIBUTE_STRING attr_name = second_arg.substr(second_arg.find(".") + 1);

				if (attr_name.compare("procName") == 0 || attr_name.compare("varName") == 0) {
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
			// statement ref synonym
			if (isStatementRef(proc_s, second_arg)) {
				return true;
			}
			else if (std::regex_match(second_arg, attr_ref_format_) && isValidAttr(proc_s, second_arg)) {
				ATTRIBUTE_STRING attr_name = second_arg.substr(second_arg.find(".") + 1);

				if (attr_name.compare("value") == 0 || attr_name.compare("stmt#") == 0) {
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
	}
	else if (std::regex_match(first_arg, attr_ref_format_)) {
		// first arg is attribute ref
		ATTRIBUTE_STRING attr_name = first_arg.substr(first_arg.find(".") + 1);
		if (!isValidAttr(proc_s, first_arg)) {
			return false;
		}
		else if (attr_name.compare("value") == 0 || attr_name.compare("stmt#") == 0) {
			if (isStatementRef(proc_s, second_arg)) {
				return true;
			}
			else if (std::regex_match(second_arg, attr_ref_format_) && isValidAttr(proc_s, second_arg)) {
				ATTRIBUTE_STRING attr_name = second_arg.substr(second_arg.find(".") + 1);

				if (attr_name.compare("value") == 0 || attr_name.compare("stmt#") == 0) {
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
			if (isEntityRef(proc_s, second_arg)) {
				return true;
			}
			else if (std::regex_match(second_arg, attr_ref_format_) && isValidAttr(proc_s, second_arg)) {
				ATTRIBUTE_STRING attr_name = second_arg.substr(second_arg.find(".") + 1);

				if (attr_name.compare("procName") == 0 || attr_name.compare("varName") == 0) {
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
	}
	else {
		return false;
	}
}