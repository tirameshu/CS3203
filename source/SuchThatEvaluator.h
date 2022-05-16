#pragma once

#include "FollowsStarEvaluator.h"
#include "ModifiesEvaluator.h"
#include "ParentStarEvaluator.h"
#include "UsesEvaluator.h"
#include "CallsStarEvaluator.h"
#include "NextStarEvaluator.h"
#include "NextBipStarEvaluator.h"
#include "AffectsStarEvaluator.h"
#include "AffectsBipStarEvaluator.h"

/*
	The SuchThatEvaluator is responsible for evaluating queries with suchthat clauses.
	Depending on the type of suchthat clause queried, it calls the methods of the relevant
	suchthat clause Evaluators to evaluate the cluase queried.
*/

class SuchThatEvaluator {
public:
	static bool checkSuchThatClauseHolds(QuerySuchThatClause& currSuchThatClause);

	static bool checkSuchThatClausePossible(QuerySuchThatClause& currSuchThatClause);

	static std::unordered_map<std::string, std::vector<std::string>> evaluateSuchThatClause(QuerySuchThatClause& currSuchThatClause);

	static int estimateSuchThatClauseTableSize(QuerySuchThatClause& currSuchThatClause);
};