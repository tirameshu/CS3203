#pragma once

#include "QueryEvaluatorUtil.h"

/*
	The CallsEvaluator is responsible for evaluating queries with Calls suchthat clauses.
	It first checks the validity of the suchthat clause queried. If valid, it accesses the PKB and
	retrieves the neccessary information required by the clause queried.
*/

class CallsEvaluator {
public:
	static bool checkCallsClauseHolds(QuerySuchThatClause& currSuchThatClause);

	static std::unordered_map<std::string, std::vector<std::string>> evaluateCallsClause(QuerySuchThatClause& currSuchThatClause);

	static int estimateCallsClauseTableSize(QuerySuchThatClause& currSuchThatClause);
};