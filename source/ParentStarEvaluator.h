#pragma once

#include "ParentEvaluator.h"
#include "QueryEvaluatorUtil.h"

/*
	The ParentStarEvaluator is responsible for evaluating queries with Parent* suchthat clauses.
	It first checks the validity of the suchthat clause queried. If valid, it accesses the PKB and
	retrieves the neccessary information required by the clause queried.
*/

class ParentStarEvaluator {
public:
	static bool checkParentStarClauseHolds(QuerySuchThatClause& currSuchThatClause);

	static std::unordered_map<std::string, std::vector<std::string>> evaluateParentStarClause(QuerySuchThatClause& currSuchThatClause);

	static int estimateParentStarClauseTableSize(QuerySuchThatClause& currSuchThatClause);

private:
	static bool checkStmtTypeIsParentStar(ArgumentTypes& stmtType, std::unordered_set<int>& parentStarStmtSet);

	static bool checkStmtTypesAreParentStar(ArgumentTypes& firstStmtType, ArgumentTypes& secondStmtType);

	static std::vector<std::string> getStmtTypeIsParentStarList(ArgumentTypes& stmtType, std::unordered_set<int>& parentStarStmtSet);

	static std::pair<std::vector<std::string>, std::vector<std::string>> getStmtTypesAreParentStarList(ArgumentTypes& firstStmtType, ArgumentTypes& secondStmtType);

	static int getStmtTypesAreParentStarListSize(ArgumentTypes& firstStmtType, ArgumentTypes& secondStmtType);
};
