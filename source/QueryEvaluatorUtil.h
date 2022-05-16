#pragma once

#include <functional>

#include "PKB.h"
#include "Query.h"

/*
	QueryEvaluatorUtil comprises of utility methods used by the QueryEvaluator.
*/

namespace QueryEvaluatorUtil {
	bool containsResultSynonym(std::string resultSynonym, QuerySuchThatClause& currSuchThatClause);

	bool containsResultSynonym(std::string resultSynonym, QueryPatternClause& currPatternClause);

	bool containsResultSynonym(std::string resultSynonym, std::vector<QuerySuchThatClause>& suchThatClauses, std::vector<QueryPatternClause>& patternClauses);

	bool hasSynonym(ArgumentTypes& argType);

	bool hasSynonym(QueryClause& currClause);

	bool hasSharedSynonym(std::unordered_map<std::string, int>& synonymMap);

	bool checkPairOfClausesHold(std::unordered_map<std::string, std::vector<std::string>>& firstTable, std::unordered_map<std::string, std::vector<std::string>>& secondTable);

	std::unordered_set<std::string> convertIntSetToStringSet(std::unordered_set<int>& intSet);

	std::vector<std::string> convertIntSetToStringList(std::unordered_set<int>& intSet);

	std::vector<std::string> convertStringSetToStringList(std::unordered_set<std::string>& stringSet);

	std::unordered_set<std::string> getDesignEntitySet(ArgumentTypes& resultType);

	std::unordered_set<std::string> getAttrRefSet(ArgumentTypes& resultType, AttrName& resultAttrName);

	std::unordered_set<int> getStmtTypeSet(ArgumentTypes& stmtType);

	std::vector<std::string> getSynonymList(QueryClause& currClause);

	std::vector<std::string> getSynonymList(QueryPatternClause& currPatternClause);

	std::unordered_map<std::string, int> getSynonymMap(std::vector<QuerySuchThatClause>& suchThatClauses, std::vector<QueryPatternClause>& patternClauses);

	std::unordered_set<std::string> getResultSet(std::string resultSynonym, std::unordered_map<std::string, std::vector<std::string>>& clauseTable);

	std::vector<std::string> getSharedSynonymList(std::unordered_map<std::string, std::vector<std::string>>& firstTable, std::unordered_map<std::string, std::vector<std::string>>& secondTable);

	std::vector<std::string> getNotSharedSynonymList(std::unordered_map<std::string, std::vector<std::string>>& firstTable, std::unordered_map<std::string, std::vector<std::string>>& secondTable);

	std::unordered_map<std::string, std::vector<std::string>> hashJoinClauseResults(std::unordered_map<std::string, std::vector<std::string>>& firstTable, std::unordered_map<std::string, std::vector<std::string>>& secondTable);
	
	std::unordered_map<std::string, std::vector<std::string>> crossProductPairOfEntityTables(std::unordered_map<std::string, std::vector<std::string>>& firstEntityTable, std::unordered_map<std::string, std::vector<std::string>>& secondEntityTable);

	std::unordered_map<std::string, std::vector<std::string>> crossProductEntityTableLists(std::unordered_map<std::string, std::vector<std::string>>& entityTable);

	std::unordered_map<std::string, std::vector<std::string>> evaluateTupleSynonyms(std::vector<std::string>& resultSynonyms, std::vector<ArgumentTypes>& resultTypes);

	std::string getAttrRef(ArgumentTypes& resultType, AttrName& resultAttrName, std::string& stmt);
}
