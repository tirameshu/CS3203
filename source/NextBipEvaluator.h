#pragma once

#include "QueryEvaluatorUtil.h"

/*
	The NextBipEvaluator is responsible for evaluating queries with NextBip suchthat clauses.
	It first checks the validity of the suchthat clause queried. If valid, it accesses the PKB and
	retrieves the neccessary information required by the clause queried.
*/

class NextBipEvaluator {
public:
	static bool checkNextBipClauseHolds(QuerySuchThatClause& currSuchThatClause);

	static bool checkLineTypeIsNextBip(ArgumentTypes& lineType, std::unordered_set<int>& nextBipLineSet);

	static std::unordered_map<std::string, std::vector<std::string>> evaluateNextBipClause(QuerySuchThatClause& currSuchThatClause);

	static std::vector<std::string> getLineTypeIsNextBipList(ArgumentTypes& lineType, std::unordered_set<int>& nextBipLineSet);

	static int estimateNextBipClauseTableSize(QuerySuchThatClause& currSuchThatClause);

private:
	static bool checkLineTypesAreNextBip(ArgumentTypes& firstLineType, ArgumentTypes& secondLineType);

	static std::pair<std::vector<std::string>, std::vector<std::string>> getLineTypesAreNextBipList(ArgumentTypes& firstLineType, ArgumentTypes& secondLineType);

	static int getStmtTypesAreNextBipListSize(ArgumentTypes& firstLineType, ArgumentTypes& secondLineType);
};