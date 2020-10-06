#pragma once

enum class QueryNodeType {
	unassigned,
	select,
	boolean,
	tuple,
	synonym,
	integer,
	ident,
	wild_card, //"_"
	expression, //for ASTNode expressions
	such_that,
	parent,
	parentT,
	follows,
	followsT,
	usesS,
	modifiesS,
	calls,
	callsT,
	next,
	nextT,
	pattern
};