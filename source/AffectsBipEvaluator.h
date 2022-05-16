#pragma once

#include "QueryEvaluatorUtil.h"

/*
	The AffectsBipEvaluator is responsible for evaluating queries with AffectsBip suchthat clauses. 
	It first checks the validity of the suchthat clause queried. If valid, it accesses the PKB and
	retrieves the neccessary information required by the clause queried.
*/

class AffectsBipEvaluator {
public:
	static bool checkAffectsBipClauseHolds(QuerySuchThatClause currSuchThatClause);

	static std::unordered_map<std::string, std::vector<std::string>> evaluateAffectsBipClause(QuerySuchThatClause& currSuchThatClause);

	static int estimateAffectsBipClauseTableSize(QuerySuchThatClause& currSuchThatClause);
};