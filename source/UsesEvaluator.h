#pragma once

#include "QueryEvaluatorUtil.h"

/*
	The UsesEvaluator is responsible for evaluating queries with Uses suchthat clauses.
	It first checks the validity of the suchthat clause queried. If valid, it accesses the PKB and
	retrieves the neccessary information required by the clause queried.
*/

class UsesEvaluator {
public:
	static bool checkUsesClauseHolds(QuerySuchThatClause& currSuchThatClause);

	static std::unordered_map<std::string, std::vector<std::string>> evaluateUsesClause(QuerySuchThatClause& currSuchThatClause);

	static int estimateUsesClauseTableSize(QuerySuchThatClause& currSuchThatClause);

private:
	static bool checkStmtTypeIsUses(ArgumentTypes& stmtType);

	static bool checkStmtTypeIsUses(ArgumentTypes& stmtType, std::unordered_set<int>& usesStmtSet);

	static std::unordered_set<int> getStmtTypeIsUsesSet(ArgumentTypes& stmtType);

	static std::vector<std::string> getStmtTypeIsUsesList(ArgumentTypes& stmtType, std::unordered_set<int>& usesStmtSet);

	static std::vector<std::string> getUsesStmtList(std::unordered_map<int, std::unordered_set<std::string>>& stmtUsesMap);

	static int getTotalVarsUsedForStmtType(ArgumentTypes& stmtType);
};
