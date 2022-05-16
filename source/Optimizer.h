#pragma once

#include <queue>

#include "QueryEvaluatorUtil.h"
#include "SuchThatEvaluator.h"
#include "PatternEvaluator.h"
#include "WithEvaluator.h"
#include "ClauseType.h"

struct ClauseWrapper {
	QuerySuchThatClause suchThatClause;
	QueryPatternClause patternClause;
	QueryWithClause withClause;
	ClauseType clausetype;
	int weight;

	bool operator==(const ClauseWrapper& cw) const {
		if (clausetype == cw.clausetype && weight == cw.weight) {
			bool isSameClause = false;

			switch (clausetype) {
			case ClauseType::SUCH_THAT:
				isSameClause = suchThatClause == cw.suchThatClause;
				break;
			case ClauseType::PATTERN:
				isSameClause = patternClause == cw.patternClause;
				break;
			case ClauseType::WITH:
				isSameClause = withClause == cw.withClause;
				break;
			}

			return isSameClause;
		}
		else {
			return false;
		}
	}
};

struct ClauseEdge {
	std::string src;
	std::string dest;
	struct ClauseWrapper clause;

	// Should there be comparison for clausetype?
	bool operator<(const ClauseEdge& ce) const {
		return clause.weight > ce.clause.weight;
	}
};

struct ConnectedClauseGroup {
	std::unordered_set<std::string> connectedSyns;
	struct ClauseEdge minClauseEdge;
	int numClauses;
	int totalWeight;

	bool operator<(const ConnectedClauseGroup& cg) const {
		if (numClauses == cg.numClauses) {
			return totalWeight < cg.totalWeight;
		}
		else {
			return numClauses < cg.numClauses;
		}
	}
};

class ClauseWrapperHasher {
public:
	size_t operator()(const ClauseWrapper& cw) const {
		switch (cw.clausetype) {
		case ClauseType::SUCH_THAT:
			return cw.suchThatClause.getClauseHashValue();
		case ClauseType::PATTERN:
			return cw.patternClause.getClauseHashValue();
		case ClauseType::WITH:
			return cw.withClause.getClauseHashValue();
		}
	}
};

class Optimizer {
public:
	Optimizer(bool isBoolean, std::vector<std::string>& synonyms, std::vector<ArgumentTypes>& argTypes, std::vector<QuerySuchThatClause>& suchThatClauses, std::vector<QueryPatternClause>& patternClauses, std::vector<QueryWithClause>& withClauses);

	bool groupClauses();
	bool checkClausesWithNoSynHold();
	bool checkClausesWithNoResultsSynHold();

	std::vector<std::string> getResultSynNotInClauses();
	std::vector<ArgumentTypes> getResultSynTypesNotInClauses();
	std::unordered_map<std::string, std::vector<std::string>> evaluateClausesWithResultsSyn();

private:
	bool isBooleanResult;
	std::vector<std::string> resultSynonyms;
	std::vector<ArgumentTypes> resultTypes;
	std::vector<QuerySuchThatClause> querySuchThatClauses;
	std::vector<QueryPatternClause> queryPatternClauses;
	std::vector<QueryWithClause> queryWithClauses;

	std::unordered_set<std::string> synInClauses;
	std::vector<std::string> resultSynNotInClauses;
	std::vector<ArgumentTypes> resultSynTypesNotInClauses;
	std::unordered_set<std::string> resultSynInClauses;
	std::unordered_map<std::string, std::vector<ClauseEdge>> adjList;
	std::vector<ConnectedClauseGroup> groupWithNoResultsSyn;
	std::vector<ConnectedClauseGroup> groupWithResultsSyn;	

	bool addClausesToLists();
	bool addClauseSynonyms(std::vector<std::string>& synList);
	bool addClauseToAdjList(std::vector<std::string>& synList, ClauseWrapper& clause);
	bool filterResultSynonyms();
	bool DFSUtil(std::string& srcSyn, std::unordered_set<std::string>& visitedSyns, std::unordered_set<std::string>& connectedSyns);
	bool filterConnectedGroups(std::unordered_set<std::string>& connectedSyns);
	bool containsResultSynonym(std::unordered_set<std::string>& connectedSyns);

	bool checkClauseHolds(ClauseWrapper& currClause);
	std::unordered_map<std::string, std::vector<std::string>> evaluateClauseGroup(ConnectedClauseGroup& clauses);
	std::unordered_map<std::string, std::vector<std::string>> evaluateClause(ClauseWrapper& currClause);
	ConnectedClauseGroup generateConnectedClauseGroup(std::unordered_set<std::string>& connectedSyns);

	std::vector<std::string> getSynonymList(ClauseWrapper& currClause);
	ClauseEdge generateClauseEdge(ClauseWrapper& currClause);
	std::unordered_map<std::string, std::vector<std::string>> filterResultTable(std::unordered_map<std::string, std::vector<std::string>>& resultTable);
};