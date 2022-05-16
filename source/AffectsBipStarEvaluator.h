#pragma once

#include "AffectsBipEvaluator.h"
#include "QueryEvaluatorUtil.h"

/*
	The AffectsBipStarEvaluator is responsible for evaluating queries with AffectsBip* suchthat clauses.
	It first checks the validity of the suchthat clause queried. If valid, it accesses the PKB and
	retrieves the neccessary information required by the clause queried.
*/

class AffectsBipStarEvaluator {
public:
	static bool checkAffectsBipStarClauseHolds(QuerySuchThatClause currSuchThatClause);

	static std::unordered_map<std::string, std::vector<std::string>> evaluateAffectsBipStarClause(QuerySuchThatClause& currSuchThatClause);

	static int estimateAffectsBipStarClauseTableSize(QuerySuchThatClause& currSuchThatClause);
};