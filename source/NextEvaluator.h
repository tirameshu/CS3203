#pragma once

#include "QueryEvaluatorUtil.h"

/*
	The NextEvaluator is responsible for evaluating queries with Next suchthat clauses.
	It first checks the validity of the suchthat clause queried. If valid, it accesses the PKB and
	retrieves the neccessary information required by the clause queried.
*/

class NextEvaluator {
public:
	static bool checkNextClauseHolds(QuerySuchThatClause& currSuchThatClause);

	static bool checkLineTypeIsNext(ArgumentTypes& lineType, std::unordered_set<int>& nextLineSet);

	static std::unordered_map<std::string, std::vector<std::string>> evaluateNextClause(QuerySuchThatClause& currSuchThatClause);

	static std::vector<std::string> getLineTypeIsNextList(ArgumentTypes& lineType, std::unordered_set<int>& nextLineSet);

	static int estimateNextClauseTableSize(QuerySuchThatClause& currSuchThatClause);

private:
	static bool checkLineTypesAreNext(ArgumentTypes& firstLineType, ArgumentTypes& secondLineType);

	static std::pair<std::vector<std::string>, std::vector<std::string>> getLineTypesAreNextList(ArgumentTypes& firstLineType, ArgumentTypes& secondLineType);

	static int getStmtTypesAreNextListSize(ArgumentTypes& firstLineType, ArgumentTypes& secondLineType);
};