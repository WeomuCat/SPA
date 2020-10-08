#pragma once

enum class QueryEvaluatorReturnType {
	synonym,	//for returning 1 synonym
	tuple,		//for returning multiple synonyms
	boolean		//for returning Select BOOLEAN
};