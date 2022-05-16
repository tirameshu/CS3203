#pragma once

#include "QueryEvaluatorUtil.h"

/*
	The AssignEvaluator is responsible for evaluating queries with assign-pattern clauses.
	It first checks the validity of the assign-pattern clause queried. If valid, it accesses the PKB and
	retrieves the neccessary information required by the clause queried.
*/

class AssignEvaluator {
public:
	static bool checkAssignClauseHolds(QueryPatternClause& currPatternClause);

	static std::unordered_map<std::string, std::vector<std::string>> evaluateAssignClause(QueryPatternClause& currPatternClause);

	static int estimateAssignClauseTableSize(QueryPatternClause& currPatternClause);

private:
	static bool checkStringIsConstVal(std::string expression);

	static bool checkStringIsVarName(std::string expression);

	static bool checkStmtTypeIsAssign(std::unordered_set<int>& assignStmtSetForFilter, std::unordered_set<int>& assignStmtSetToFilter);

	static std::vector<std::string> getStmtTypeIsAssignList(std::unordered_set<int>& assignStmtSetForFilter, std::unordered_set<int>& assignStmtSetToFilter);

};