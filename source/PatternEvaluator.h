#pragma once

#include "AssignEvaluator.h"
#include "IfEvaluator.h"
#include "WhileEvaluator.h"

/*
	The PatternEvaluator is responsible for evaluating queries with pattern clauses. It first
	checks the validity of the pattern clause queried. If valid, it accesses the PKB and 
	retrieves the neccessary information required by the clause queried.
*/

class PatternEvaluator {
public:
	static bool checkPatternClauseHolds(QueryPatternClause& currPatternClause);

	static std::unordered_map<std::string, std::vector<std::string>> evaluatePatternClause(QueryPatternClause& currPatternClause);

	static int estimatePatternClauseTableSize(QueryPatternClause& currPatternClause);
};