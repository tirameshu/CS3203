#pragma once

#include "QueryEvaluatorUtil.h"

/*
	The ParentEvaluator is responsible for evaluating queries with Parent suchthat clauses.
	It first checks the validity of the suchthat clause queried. If valid, it accesses the PKB and
	retrieves the neccessary information required by the clause queried.
*/

class ParentEvaluator {
public:
	static bool checkParentClauseHolds(QuerySuchThatClause currSuchThatClause);

	static bool checkStmtTypeIsParent(ArgumentTypes& stmtType, std::unordered_set<int>& parentStmtSet);

	static std::unordered_map<std::string, std::vector<std::string>> evaluateParentClause(QuerySuchThatClause& currSuchThatClause);

	static std::vector<std::string> getStmtTypeIsParentList(ArgumentTypes& stmtType, std::unordered_set<int>& parentStmtSet);

	static int estimateParentClauseTableSize(QuerySuchThatClause& currSuchThatClause);

private:
	static bool checkStmtTypesAreParent(ArgumentTypes& firstStmtType, ArgumentTypes& secondStmtType);

	static std::pair<std::vector<std::string>, std::vector<std::string>> getStmtTypesAreParentList(ArgumentTypes& firstStmtType, ArgumentTypes& secondStmtType);

	static int getStmtTypesAreParentListSize(ArgumentTypes& firstStmtType, ArgumentTypes& secondStmtType);
};