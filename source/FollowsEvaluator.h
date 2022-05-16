#pragma once

#include "QueryEvaluatorUtil.h"

/*
	The FollowsEvaluator is responsible for evaluating queries with Follows suchthat clauses.
	It first checks the validity of the suchthat clause queried. If valid, it accesses the PKB and
	retrieves the neccessary information required by the clause queried.
*/

class FollowsEvaluator {
public:
	static bool checkFollowsClauseHolds(QuerySuchThatClause& currSuchThatClause);

	static bool checkStmtTypeIsFollows(ArgumentTypes& stmtType, std::unordered_set<int>& followsStmtSet);

	static std::unordered_map<std::string, std::vector<std::string>> evaluateFollowsClause(QuerySuchThatClause& currSuchThatClause);

	static std::vector<std::string> getStmtTypeIsFollowsList(ArgumentTypes& stmtType, std::unordered_set<int>& followsStmtSet);

	static int estimateFollowsClauseTableSize(QuerySuchThatClause& currSuchThatClause);

private:	
	static bool checkStmtTypesAreFollows(ArgumentTypes& firstStmtType, ArgumentTypes& secondStmtType);	

	static std::pair<std::vector<std::string>, std::vector<std::string>> getStmtTypesAreFollowsList(ArgumentTypes& firstStmtType, ArgumentTypes& secondStmtType);

	static int getStmtTypesAreFollowsListSize(ArgumentTypes& firstStmtType, ArgumentTypes& secondStmtType);
};