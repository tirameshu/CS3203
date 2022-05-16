#pragma once

#include "QueryEvaluatorUtil.h"

/*
	The ModifiesEvaluator is responsible for evaluating queries with Modifies suchthat clauses.
	It first checks the validity of the suchthat clause queried. If valid, it accesses the PKB and
	retrieves the neccessary information required by the clause queried.
*/

class ModifiesEvaluator {
public:
	static bool checkModifiesClauseHolds(QuerySuchThatClause& currSuchThatClause);

	static std::unordered_map<std::string, std::vector<std::string>> evaluateModifiesClause(QuerySuchThatClause& currSuchThatClause);

	static int estimateModifiesClauseTableSize(QuerySuchThatClause& currSuchThatClause);

private:
	static bool checkStmtTypeIsModifies(ArgumentTypes& stmtType);

	static bool checkStmtTypeIsModifies(ArgumentTypes& stmtType, std::unordered_set<int>& modifiesStmtSet);

	static std::unordered_set<int> getStmtTypeIsModifiesSet(ArgumentTypes& stmtType);

	static std::vector<std::string> getStmtTypeIsModifiesList(ArgumentTypes& stmtType, std::unordered_set<int>& modifiesStmtSet);

	static int getTotalVarsModifiedForStmtType(ArgumentTypes& stmtType);
};
