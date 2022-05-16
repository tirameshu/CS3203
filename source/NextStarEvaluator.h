#pragma once

#include "NextEvaluator.h"
#include "QueryEvaluatorUtil.h"

/*
	The NextStarEvaluator is responsible for evaluating queries with Next* suchthat clauses.
	It first checks the validity of the suchthat clause queried. If valid, it accesses the PKB and
	retrieves the neccessary information required by the clause queried.
*/

class NextStarEvaluator {
public:
	static bool checkNextStarClauseHolds(QuerySuchThatClause& currSuchThatClause);

	static std::unordered_map<std::string, std::vector<std::string>> evaluateNextStarClause(QuerySuchThatClause& currSuchThatClause);

	static int estimateNextStarClauseTableSize(QuerySuchThatClause& currSuchThatClause);

private:
	static bool checkLineTypeIsNextStar(ArgumentTypes& lineType, std::unordered_set<int>& nextStarLineSet);

	static bool checkLineTypesAreNextStar(ArgumentTypes& firstLineType, ArgumentTypes& secondLineType, bool isSameLineRef);

	static std::vector<std::string> getLineTypeIsNextStarList(ArgumentTypes& lineType, std::unordered_set<int>& nextStarLineSet);
	
	static std::pair<std::vector<std::string>, std::vector<std::string>> getLineTypesAreNextStarList(ArgumentTypes& firstLineType, ArgumentTypes& secondLineType, bool isSameLineRef);

	static int getStmtTypesAreNextStarListSize(ArgumentTypes& firstLineType, ArgumentTypes& secondLineType, bool isSameLineRef);
};