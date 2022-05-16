#pragma once

#include "NextBipEvaluator.h"
#include "QueryEvaluatorUtil.h"

/*
	The NextBipStarEvaluator is responsible for evaluating queries with NextBip* suchthat clauses.
	It first checks the validity of the suchthat clause queried. If valid, it accesses the PKB and
	retrieves the neccessary information required by the clause queried.
*/

class NextBipStarEvaluator {
public:
	static bool checkNextBipStarClauseHolds(QuerySuchThatClause& currSuchThatClause);

	static std::unordered_map<std::string, std::vector<std::string>> evaluateNextBipStarClause(QuerySuchThatClause& currSuchThatClause);

	static int estimateNextBipStarClauseTableSize(QuerySuchThatClause& currSuchThatClause);

private:
	static bool checkLineTypeIsNextBipStar(ArgumentTypes& lineType, std::unordered_set<int>& nextBipStarLineSet);

	static bool checkLineTypesAreNextBipStar(ArgumentTypes& firstLineType, ArgumentTypes& secondLineType, bool isSameLineRef);

	static std::vector<std::string> getLineTypeIsNextBipStarList(ArgumentTypes& lineType, std::unordered_set<int>& nextBipStarLineSet);

	static std::pair<std::vector<std::string>, std::vector<std::string>> getLineTypesAreNextBipStarList(ArgumentTypes& firstLineType, ArgumentTypes& secondLineType, bool isSameLineRef);

	static int getStmtTypesAreNextBipStarListSize(ArgumentTypes& firstLineType, ArgumentTypes& secondLineType, bool isSameLineRef);
};