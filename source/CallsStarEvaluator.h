#pragma once

#include "CallsEvaluator.h"
#include "QueryEvaluatorUtil.h"

/*
	The CallsStarEvaluator is responsible for evaluating queries with Calls* suchthat clauses.
	It first checks the validity of the suchthat clause queried. If valid, it accesses the PKB and
	retrieves the neccessary information required by the clause queried.
*/

class CallsStarEvaluator {
public:
	static bool checkCallsStarClauseHolds(QuerySuchThatClause& currSuchThatClause);

	static std::unordered_map<std::string, std::vector<std::string>> evaluateCallsStarClause(QuerySuchThatClause& currSuchThatClause);

	static int estimateCallsStarClauseTableSize(QuerySuchThatClause& currSuchThatClause);
};