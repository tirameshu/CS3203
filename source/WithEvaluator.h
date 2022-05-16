#pragma once

#include "QueryEvaluatorUtil.h"

/*
	The WithEvaluator is responsible for evaluating queries with with-clauses.
	It first checks the validity of the with-clause queried. If valid, it accesses the PKB and
	retrieves the neccessary information required by the clause queried.
*/

class WithEvaluator {
public:
	static bool checkWithClauseHolds(QueryWithClause& currWithClause);

	static std::unordered_map<std::string, std::vector<std::string>> evaluateWithClause(QueryWithClause& currWithClause);

	static int estimateWithClauseTableSize(QueryWithClause& currWithClause);

private:
	static bool checkIsSynonym(ArgumentTypes& refType);

	static bool checkRefIsIntType(ArgumentTypes& refType, AttrName& refAttrName);

	static std::unordered_set<std::string> getRefSet(std::string& refArg, ArgumentTypes& refType, AttrName& refAttrName);

	static std::vector<std::string> getWithRefList(std::unordered_set<std::string>& firstRefSet, std::unordered_set<std::string>& secondRefSet);

	static std::unordered_set<std::string> getSynonymSet(std::string& firstRefArg, ArgumentTypes& firstRefType, std::string& secondRefArg, ArgumentTypes& secondRefType);

	static std::unordered_set<std::string> getSynonymElemSet(ArgumentTypes& refType, const std::string& entityName);

	static std::vector<std::string> getWithSynonymList(ArgumentTypes& refType, std::vector<std::string>& withRefList);

	static std::pair<std::vector<std::string>, std::vector<std::string>> getWithRefPairOfList(ArgumentTypes& firstRefType, std::unordered_set<std::string>& firstRefSet, ArgumentTypes& secondRefType, std::unordered_set<std::string>& secondRefSet, bool isSameRefArg);

	static int getSynonymSetSize(ArgumentTypes& refType, const std::string& entityName);

	static int getWithRefPairOfListSize(ArgumentTypes& firstRefType, std::unordered_set<std::string>& firstRefSet, ArgumentTypes& secondRefType, std::unordered_set<std::string>& secondRefSet, bool isSameRefArg);
};