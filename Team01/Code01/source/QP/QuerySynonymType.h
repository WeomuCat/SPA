#pragma once

enum class QuerySynonymType {
	unassigned,
	stmt,
	read,
	print,
	call,
	whiles,
	ifs,
	assign,
	variable,
	constant,
	prog_line,
	procedure
};