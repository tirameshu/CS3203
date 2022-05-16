#pragma once

#include "AffectsEvaluator.h"
#include "QueryEvaluatorUtil.h"

/*
	The AffectsStarEvaluator is responsible for evaluating queries with Affects* suchthat clauses.
	It first checks the validity of the suchthat clause queried. If valid, it accesses the PKB and
	retrieves the neccessary information required by the clause queried.
*/

class AffectsStarEvaluator {
public:
	static bool checkAffectsStarClauseHolds(QuerySuchThatClause currSuchThatClause);

	static std::unordered_map<std::string, std::vector<std::string>> evaluateAffectsStarClause(QuerySuchThatClause& currSuchThatClause);

	static int estimateAffectsStarClauseTableSize(QuerySuchThatClause& currSuchThatClause);
};