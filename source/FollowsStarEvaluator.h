#pragma once

#include "FollowsEvaluator.h"
#include "QueryEvaluatorUtil.h"

/*
	The FolowsStarEvaluator is responsible for evaluating queries with Follows* suchthat clauses.
	It first checks the validity of the suchthat clause queried. If valid, it accesses the PKB and
	retrieves the neccessary information required by the clause queried.
*/

class FollowsStarEvaluator {
public:
	static bool checkFollowsStarClauseHolds(QuerySuchThatClause& currSuchThatClause);

	static std::unordered_map<std::string, std::vector<std::string>> evaluateFollowsStarClause(QuerySuchThatClause& currSuchThatClause);

	static int estimateFollowsStarClauseTableSize(QuerySuchThatClause& currSuchThatClause);

private:
	static bool checkStmtTypeIsFollowsStar(ArgumentTypes& stmtType, std::unordered_set<int>& followsStarStmtSet);

	static bool checkStmtTypesAreFollowsStar(ArgumentTypes& firstStmtType, ArgumentTypes& secondStmtType);

	static std::vector<std::string> getStmtTypeIsFollowsStarList(ArgumentTypes& stmtType, std::unordered_set<int>& followsStarStmtSet);

	static std::pair<std::vector<std::string>, std::vector<std::string>> getStmtTypesAreFollowsStarList(ArgumentTypes& firstStmtType, ArgumentTypes& secondStmtType);

	static int getStmtTypesAreFollowsStarListSize(ArgumentTypes& firstStmtType, ArgumentTypes& secondStmtType);
};
