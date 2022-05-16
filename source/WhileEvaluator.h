#pragma once

#include "QueryEvaluatorUtil.h"

/*
	The WhileEvaluator is responsible for evaluating queries with while-pattern clauses.
	It first checks the validity of the while-pattern clause queried. If valid, it accesses the PKB and
	retrieves the neccessary information required by the clause queried.
*/

class WhileEvaluator {
public:
	static bool checkWhileClauseHolds(QueryPatternClause& currPatternClause);

	static std::unordered_map<std::string, std::vector<std::string>> evaluateWhileClause(QueryPatternClause& currPatternClause);

	static int estimateWhileClauseTableSize(QueryPatternClause& currPatternClause);
};