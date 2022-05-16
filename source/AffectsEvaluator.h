#pragma once

#include "QueryEvaluatorUtil.h"

/*
	The AffectsEvaluator is responsible for evaluating queries with Affects suchthat clauses.
	It first checks the validity of the suchthat clause queried. If valid, it accesses the PKB and
	retrieves the neccessary information required by the clause queried.
*/

class AffectsEvaluator {
public:
	static bool checkAffectsClauseHolds(QuerySuchThatClause currSuchThatClause);

	static std::unordered_map<std::string, std::vector<std::string>> evaluateAffectsClause(QuerySuchThatClause& currSuchThatClause);

	static int estimateAffectsClauseTableSize(QuerySuchThatClause& currSuchThatClause);
};