#include "WithClause.h"

WithClause::WithClause(QueryNode with_clause) {
	this->with_clause = with_clause;
}

void WithClause::getWithResult(PKB pkb, bool& clause_bool, ResultList& clause_result_list) {
	/*
	Format: with ref = ref
	ref: IDENT | INTEGER | attrRef | synonym
	attrRef: synonym.attrName
	attrName: procName | varName | value | stmt#

	Both refs must be of the same type (ie both strings or both integers)

	IDENT			-> string
	INTEGER			-> int
	synonym			-> int [prog_line ONLY]
	attrRef			-> int/string

	attrRef Types:
	1. [procName]	- string	-> procedure.procName | call.procName
	2. [varName]	- string	-> variable.varName | read.varName | print.varName
	3. [value]		- int		-> constant.value
	4. [stmt#]		- int		-> s.stmt# | read.stmt# | print.stmt# | assign.stmt# | call.stmt# | while.stmt# | if.stmt#


	synonyms:
	1. stmt
	2. read
	3. print
	4. call
	5. while
	6. if
	7. assign
	8. variable
	9. constant
	10. procedure
	11. prog_line		-> int

	Types:
	1. attrRef + INTEGER 		-> [int] with a.stmt# = 12 | with 12 = a.stmt#						-> INTEGER is int
	2. attrRef	+ IDENT			-> [string] with p.procName = "main" | with "main" = p.procName		-> IDENT is string
	3. attrRef + synonym		-> [int] with n = c.value | with c.value = n						-> the only synonym is prog_line
	4. attrRef + attrRef		-> [int] with a.stmt# = s.stmt# | with s.stmt# = a.stmt#
								-> [string] with p.procName = v.varName
	5. IDENT + INTEGER			-> INVALID															-> must compare same type				
	6. IDENT + synonym			-> INVALID															-> the only synonym is prog_line which is int
	7. IDENT + IDENT			-> [string] with "main" = "woof"
	8. INTEGER + INTEGER		-> [int] with 5 = 3
	9. INTEGER + synonym		-> [int] with n = 10 | with 10 = n									-> the only synonym is prog_line
	10. synonym + synonym		-> [int] with n1 = n2
	*/
}