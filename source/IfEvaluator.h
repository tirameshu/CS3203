#pragma once

#include "QueryEvaluatorUtil.h"

/*
	The IfEvaluator is responsible for evaluating queries with if-pattern clauses.
	It first checks the validity of the if-pattern clause queried. If valid, it accesses the PKB and
	retrieves the neccessary information required by the clause queried.
*/

class IfEvaluator {
public:
	static bool checkIfClauseHolds(QueryPatternClause& currPatternClause);

	static std::unordered_map<std::string, std::vector<std::string>> evaluateIfClause(QueryPatternClause& currPatternClause);

	static int estimateIfClauseTableSize(QueryPatternClause& currPatternClause);
};