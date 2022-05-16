#include "Optimizer.h"

Optimizer::Optimizer(bool isBoolean, std::vector<std::string>& synonyms, std::vector<ArgumentTypes>& argTypes, std::vector<QuerySuchThatClause>& suchThatClauses, std::vector<QueryPatternClause>& patternClauses, std::vector<QueryWithClause>& withClauses) {
	isBooleanResult = isBoolean;
	resultSynonyms = synonyms;
	resultTypes = argTypes;
	querySuchThatClauses = suchThatClauses;
	queryPatternClauses = patternClauses;
	queryWithClauses = withClauses;
}

bool Optimizer::groupClauses() {
	bool hasImmediateFalseClauses = !addClausesToLists();

	if (hasImmediateFalseClauses) {
		return false;
	}

	filterResultSynonyms();

	std::unordered_set<std::string> visitedSyns;

	for (std::string syn : synInClauses) {
		if (visitedSyns.count(syn) == 0) {
			std::unordered_set<std::string> connectedSyns;
			DFSUtil(syn, visitedSyns, connectedSyns);
			filterConnectedGroups(connectedSyns);
		}
	}

	return true;
}

bool Optimizer::addClausesToLists() {
	// To check for repeated clauses
	std::unordered_set<ClauseWrapper, ClauseWrapperHasher> clauseSet;

	for (QuerySuchThatClause& suchThatClause : querySuchThatClauses) {
		int currWeight = 0;

		if (!QueryEvaluatorUtil::hasSynonym(suchThatClause)) {
			if (!SuchThatEvaluator::checkSuchThatClauseHolds(suchThatClause)) {
				return false;
			}
			continue;
		}

		currWeight = SuchThatEvaluator::estimateSuchThatClauseTableSize(suchThatClause);

		if (currWeight == 0) {
			return false;
		}

		ClauseWrapper currClause = ClauseWrapper{ suchThatClause, {}, {}, ClauseType::SUCH_THAT, currWeight };

		if (clauseSet.count(currClause) == 1) {
			continue;
		}

		clauseSet.emplace(currClause);

		std::vector<std::string> synList = QueryEvaluatorUtil::getSynonymList(suchThatClause);
		addClauseSynonyms(synList);
		addClauseToAdjList(synList, currClause);
	}

	for (QueryWithClause& withClause : queryWithClauses) {
		int currWeight = 0;

		if (!QueryEvaluatorUtil::hasSynonym(withClause)) {
			if (!WithEvaluator::checkWithClauseHolds(withClause)) {
				return false;
			}
			continue;
		}

		currWeight = WithEvaluator::estimateWithClauseTableSize(withClause);

		if (currWeight == 0) {
			return false;
		}

		ClauseWrapper currClause = ClauseWrapper{ {}, {}, withClause, ClauseType::WITH, currWeight };

		if (clauseSet.count(currClause) == 1) {
			continue;
		}

		clauseSet.emplace(currClause);

		std::vector<std::string> synList = QueryEvaluatorUtil::getSynonymList(withClause);
		addClauseSynonyms(synList);
		addClauseToAdjList(synList, currClause);
	}

	for (QueryPatternClause& patternClause : queryPatternClauses) {
		int currWeight = PatternEvaluator::estimatePatternClauseTableSize(patternClause);

		if (currWeight == 0) {
			return false;
		}

		ClauseWrapper currClause = ClauseWrapper{ {}, patternClause, {}, ClauseType::PATTERN, currWeight };

		if (clauseSet.count(currClause) == 1) {
			continue;
		}

		clauseSet.emplace(currClause);

		std::vector<std::string> synList = QueryEvaluatorUtil::getSynonymList(patternClause);
		addClauseSynonyms(synList);
		addClauseToAdjList(synList, currClause);
	}

	return true;
}

bool Optimizer::addClauseSynonyms(std::vector<std::string>& synList) {
	for (std::string& syn : synList) {
		synInClauses.emplace(syn);
	}

	return true;
}

bool Optimizer::addClauseToAdjList(std::vector<std::string>& synList, ClauseWrapper& clause) {
	int synListSize = synList.size();

	if (synListSize == 1) {
		std::string firstSyn = synList[0];

		if (adjList.count(firstSyn) == 0) {
			std::vector<ClauseEdge> clauseEdges;

			adjList.emplace(firstSyn, clauseEdges);
		}
		// Self-directed edge
		adjList.at(firstSyn).push_back(ClauseEdge{ firstSyn, firstSyn, clause });
	}
	else {
		std::string firstSyn = synList[0];
		std::string secondSyn = synList[1];

		if (adjList.count(firstSyn) == 0) {
			std::vector<ClauseEdge> clauseEdges;

			adjList.emplace(firstSyn, clauseEdges);
		}
		// May be a self-directed edge
		adjList.at(firstSyn).push_back(ClauseEdge{ firstSyn, secondSyn, clause });

		if (firstSyn != secondSyn) {
			if (adjList.count(secondSyn) == 0) {
				std::vector<ClauseEdge> clauseEdges;

				adjList.emplace(secondSyn, clauseEdges);
			}

			adjList.at(secondSyn).push_back(ClauseEdge{ secondSyn, firstSyn, clause });
		}
	}

	return true;
}

bool Optimizer::filterResultSynonyms() {
	int numResultSyn = resultSynonyms.size();

	for (int i = 0; i < numResultSyn; i++) {
		std::string syn = resultSynonyms[i];
		ArgumentTypes synType = resultTypes[i];

		if (synInClauses.count(syn) == 0) {
			resultSynNotInClauses.push_back(syn);
			resultSynTypesNotInClauses.push_back(synType);
		}
		else {
			resultSynInClauses.emplace(syn);
		}
	}

	return true;
}

bool Optimizer::DFSUtil(std::string& srcSyn, std::unordered_set<std::string>& visitedSyns, std::unordered_set<std::string>& connectedSyns) {
	visitedSyns.emplace(srcSyn);
	connectedSyns.emplace(srcSyn);
	auto edges = adjList.at(srcSyn);

	for (ClauseEdge& edge : edges) {
		std::string destSyn = edge.dest;

		if (visitedSyns.count(destSyn) == 0) {
			DFSUtil(destSyn, visitedSyns, connectedSyns);
		}
	}

	return true;
}

bool Optimizer::filterConnectedGroups(std::unordered_set<std::string>& connectedSyns) {
	if (isBooleanResult) {
		groupWithNoResultsSyn.push_back(generateConnectedClauseGroup(connectedSyns));
	}
	else {
		if (!containsResultSynonym(connectedSyns)) {
			groupWithNoResultsSyn.push_back(generateConnectedClauseGroup(connectedSyns));
		}
		else {
			groupWithResultsSyn.push_back(generateConnectedClauseGroup(connectedSyns));
		}
	}

	return true;
}

bool Optimizer::containsResultSynonym(std::unordered_set<std::string>& connectedSyns) {
	bool hasResultSynonym = false;

	if (connectedSyns.size() < resultSynInClauses.size()) {
		for (const std::string& syn : connectedSyns) {
			if (resultSynInClauses.count(syn) == 1) {
				hasResultSynonym = true;
				break;
			}
		}
	}
	else {
		for (const std::string& syn : resultSynInClauses) {
			if (connectedSyns.count(syn) == 1) {
				hasResultSynonym = true;
				break;
			}
		}
	}

	return hasResultSynonym;
}

bool Optimizer::checkClauseHolds(ClauseWrapper& clause) {
	bool clauseHolds = false;
	ClauseType currClauseType = clause.clausetype;

	switch (currClauseType) {
	case ClauseType::SUCH_THAT:
		clauseHolds = SuchThatEvaluator::checkSuchThatClauseHolds(clause.suchThatClause);
		break;
	case ClauseType::PATTERN:
		clauseHolds = PatternEvaluator::checkPatternClauseHolds(clause.patternClause);
		break;
	case ClauseType::WITH:
		clauseHolds = WithEvaluator::checkWithClauseHolds(clause.withClause);
		break;
	}

	return clauseHolds;
}

// If there are no such clauses, true by default
bool Optimizer::checkClausesWithNoResultsSynHold() {
	bool clausesWithNoResultsSynHold = true;
	std::sort(groupWithNoResultsSyn.begin(), groupWithNoResultsSyn.end());

	for (ConnectedClauseGroup& clauses : groupWithNoResultsSyn) {
		if (clauses.numClauses == 1) {
			if (!checkClauseHolds(clauses.minClauseEdge.clause)) {
				clausesWithNoResultsSynHold = false;
				break;
			}
			continue;
		}

		std::unordered_map<std::string, std::vector<std::string>> resultTable = evaluateClauseGroup(clauses);

		if (resultTable.empty()) {
			clausesWithNoResultsSynHold = false;
			break;
		}
		else if (resultTable.begin()->second.empty()) {
			clausesWithNoResultsSynHold = false;
			break;
		}
	}

	return clausesWithNoResultsSynHold;
}

std::vector<std::string> Optimizer::getResultSynNotInClauses() {
	return resultSynNotInClauses;
}

std::vector<ArgumentTypes> Optimizer::getResultSynTypesNotInClauses() {
	return resultSynTypesNotInClauses;
}

std::unordered_map<std::string, std::vector<std::string>> Optimizer::evaluateClauseGroup(ConnectedClauseGroup& clauses) {
	std::unordered_set<std::string> visitedSyns;
	std::unordered_set<ClauseWrapper, ClauseWrapperHasher> clauseSet;
	std::priority_queue<ClauseEdge> clauseEdgeHeap;
	std::unordered_map<std::string, std::vector<std::string>> resultTable;

	clauseEdgeHeap.emplace(clauses.minClauseEdge);
	clauseSet.emplace(clauses.minClauseEdge.clause);
	bool isFirstClause = true;

	while (!clauseEdgeHeap.empty()) {
		ClauseEdge currClauseEdge = clauseEdgeHeap.top();
		clauseEdgeHeap.pop();

		std::unordered_set<std::string> synList;
		synList.emplace(currClauseEdge.src);
		synList.emplace(currClauseEdge.dest);

		for (const std::string& syn : synList) {
			if (visitedSyns.count(syn) == 0) {
				visitedSyns.emplace(syn);

				auto& edges = adjList.at(syn);

				for (ClauseEdge& edge : edges) {
					ClauseWrapper& currClause = edge.clause;

					if (clauseSet.count(currClause) == 0) {
						clauseSet.emplace(currClause);
						clauseEdgeHeap.emplace(edge);
					}
				}
			}
		}

		if (isFirstClause) {
			isFirstClause = false;
			resultTable = evaluateClause(currClauseEdge.clause);
		}
		else {
			resultTable = QueryEvaluatorUtil::hashJoinClauseResults(resultTable, evaluateClause(currClauseEdge.clause));
		}

		if (resultTable.empty()) {
			return resultTable;
		}

		if (resultTable.begin()->second.empty()) {
			return resultTable;
		}
	}

	return resultTable;
}

std::unordered_map<std::string, std::vector<std::string>> Optimizer::evaluateClause(ClauseWrapper& clause) {
	std::unordered_map<std::string, std::vector<std::string>> resultTable;
	ClauseType currClauseType = clause.clausetype;

	switch (currClauseType) {
	case ClauseType::SUCH_THAT:
		resultTable = SuchThatEvaluator::evaluateSuchThatClause(clause.suchThatClause);
		break;
	case ClauseType::PATTERN:
		resultTable = PatternEvaluator::evaluatePatternClause(clause.patternClause);
		break;
	case ClauseType::WITH:
		resultTable = WithEvaluator::evaluateWithClause(clause.withClause);
		break;
	}

	return resultTable;
}

std::unordered_map<std::string, std::vector<std::string>> Optimizer::evaluateClausesWithResultsSyn() {
	std::unordered_map<std::string, std::vector<std::string>> resultTable;
	bool isFirstClauseGroup = true;
	std::sort(groupWithResultsSyn.begin(), groupWithResultsSyn.end());
	std::vector<std::unordered_map<std::string, std::vector<std::string>>> resultTableCache;

	for (ConnectedClauseGroup& clauses : groupWithResultsSyn) {
		std::unordered_map<std::string, std::vector<std::string>>& tempTable = evaluateClauseGroup(clauses);

		tempTable = filterResultTable(tempTable);

		if (tempTable.empty()) {
			return resultTable;
		}

		resultTableCache.push_back(tempTable);
	}

	for (auto& tempTable : resultTableCache) {
		if (isFirstClauseGroup) {
			isFirstClauseGroup = false;
			resultTable = tempTable;
		}
		else {
			resultTable = QueryEvaluatorUtil::crossProductPairOfEntityTables(resultTable, tempTable);
		}
	}

	return resultTable;
}

ConnectedClauseGroup Optimizer::generateConnectedClauseGroup(std::unordered_set<std::string>& connectedSyns) {
	std::unordered_set<std::string> visitedSyns;
	std::queue<std::string> synQueue;
	std::unordered_set<ClauseWrapper, ClauseWrapperHasher> clauseSet;

	ClauseWrapper rootClause;
	int currMinWeight = INT_MAX;
	int numClauses = 0;
	int totalWeight = 0;

	std::string firstSyn = *connectedSyns.begin();
	visitedSyns.emplace(firstSyn);
	synQueue.push(firstSyn);

	bool isFirstClause = true;

	while (!synQueue.empty()) {
		std::string currSyn = synQueue.front();
		synQueue.pop();
		auto edges = adjList.at(currSyn);

		for (ClauseEdge& edge : edges) {
			std::string destSyn = edge.dest;
			ClauseWrapper currClause = edge.clause;

			if (visitedSyns.count(destSyn) == 0) {
				visitedSyns.emplace(destSyn);
				synQueue.push(destSyn);
			}

			if (clauseSet.count(currClause) == 0) {
				clauseSet.emplace(currClause);

				numClauses += 1;
				int currWeight = currClause.weight;
				totalWeight += currWeight;

				if (isFirstClause) {
					isFirstClause = false;
					rootClause = currClause;
					currMinWeight = currWeight;
				}
				else {
					if (currWeight < currMinWeight) {
						rootClause = currClause;
						currMinWeight = currWeight;
					}
				}
			}
		}
	}

	return ConnectedClauseGroup{ connectedSyns, generateClauseEdge(rootClause), numClauses, totalWeight };
}

std::vector<std::string> Optimizer::getSynonymList(ClauseWrapper& currClause) {
	std::vector<std::string> synList;
	ClauseType currClauseType = currClause.clausetype;

	switch (currClauseType) {
	case ClauseType::SUCH_THAT:
		synList = QueryEvaluatorUtil::getSynonymList(currClause.suchThatClause);
		break;
	case ClauseType::PATTERN:
		synList = QueryEvaluatorUtil::getSynonymList(currClause.patternClause);
		break;
	case ClauseType::WITH:
		synList = QueryEvaluatorUtil::getSynonymList(currClause.withClause);
		break;
	}

	return synList;
}

ClauseEdge Optimizer::generateClauseEdge(ClauseWrapper& currClause) {
	std::vector<std::string> synList = getSynonymList(currClause);
	int synListSize = synList.size();
	ClauseEdge currEdge;

	if (synListSize == 1) {
		std::string& firstSyn = synList[0];

		currEdge = ClauseEdge{ firstSyn, firstSyn, currClause };
	}
	else {
		std::string& firstSyn = synList[0];
		std::string& secondSyn = synList[1];

		currEdge = ClauseEdge{ firstSyn, secondSyn, currClause };
	}

	return currEdge;
}

std::unordered_map<std::string, std::vector<std::string>> Optimizer::filterResultTable(std::unordered_map<std::string, std::vector<std::string>>& resultTable) {
	std::unordered_map<std::string, std::vector<std::string>> filteredResultTable;

	if (resultTable.empty()) {
		return filteredResultTable;
	}

	bool canFilter = resultTable.size() != resultSynInClauses.size();

	for (auto& pair : resultTable) {
		if (pair.second.empty()) {
			return filteredResultTable;
		}

		if (canFilter) {
			const std::string& currSynString = pair.first;

			if (resultSynInClauses.count(currSynString) == 1) {
				filteredResultTable.emplace(currSynString, pair.second);
			}
		}
	}

	if (canFilter) {
		return filteredResultTable;
	}
	else {
		return resultTable;
	}
}