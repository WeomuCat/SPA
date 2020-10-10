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
	expression, //for full ASTNode expressions eg "c"
	partial_expression, //for partial ASTNode expressions eg _"c"_
	such_that,
	parent,
	parentT,
	follows,
	followsT,
	usesS,
	usesP,
	modifiesS,
	modifiesP,
	calls,
	callsT,
	next,
	nextT,
	pattern,
	with
};