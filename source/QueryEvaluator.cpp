#include "QueryEvaluator.h"

std::unordered_set<std::string> QueryEvaluator::evaluateQuery(Query& query) {
	std::unordered_set<std::string> resultSet;
	bool isBooleanResult = query.checkBooleanResult();

	if (!query.getValidity() && !isBooleanResult) {
		return resultSet;
	}
	else if (!query.getValidity() && isBooleanResult) {
		if (!query.getSyntacticValidity()) {
			return resultSet;
		}
		else {
			resultSet.emplace("FALSE");
			return resultSet;
		}
	}

	std::vector<QuerySuchThatClause> suchThatClauses = query.getSuchThatClauses();
	int numSuchThatClauses = suchThatClauses.size();

	std::vector<QueryPatternClause> patternClauses = query.getPatternClauses();
	int numPatternClauses = patternClauses.size();

	std::vector<QueryWithClause> withClauses = query.getWithClauses();
	int numWithClauses = withClauses.size();

	int numClauses = numSuchThatClauses + numPatternClauses + numWithClauses;

	std::vector<std::string> resultSynonyms = query.getResultSynonyms();
	std::vector<ArgumentTypes> resultTypes = query.getResultTypes();
	std::vector<AttrName> resultAttrNames = query.getResultAttrNames();

	if (isBooleanResult) {
		if (numClauses == 0) {
			resultSet.emplace("TRUE");
		}
		else {
			Optimizer op = Optimizer(isBooleanResult, resultSynonyms, resultTypes, suchThatClauses, patternClauses, withClauses);
			bool hasNoImmediateFalseClauses = op.groupClauses();

			if (hasNoImmediateFalseClauses && op.checkClausesWithNoResultsSynHold()) {
				resultSet.emplace("TRUE");
			}
			else {
				resultSet.emplace("FALSE");
			}
		}
	}
	else {
		if (numClauses == 0) {
			std::unordered_map<std::string, std::vector<std::string>> tupleEntityTable = QueryEvaluatorUtil::evaluateTupleSynonyms(resultSynonyms, resultTypes);
			resultSet = formatQueryResults(tupleEntityTable, resultSynonyms, resultTypes, resultAttrNames);
		}
		else {
			Optimizer op = Optimizer(isBooleanResult, resultSynonyms, resultTypes, suchThatClauses, patternClauses, withClauses);
			bool hasNoImmediateFalseClauses = op.groupClauses();

			if (hasNoImmediateFalseClauses && op.checkClausesWithNoResultsSynHold()) {
				std::unordered_map<std::string, std::vector<std::string>> clausesResultTable = op.evaluateClausesWithResultsSyn();
				std::vector<std::string> resultSynNotInClauses = op.getResultSynNotInClauses();
				std::vector<ArgumentTypes> resultSynTypesNotInClauses = op.getResultSynTypesNotInClauses();

				if (resultSynNotInClauses.empty()) {
					resultSet = formatQueryResults(clausesResultTable, resultSynonyms, resultTypes, resultAttrNames);
				}
				else if (resultSynNotInClauses.size() == resultSynonyms.size()) {
					std::unordered_map<std::string, std::vector<std::string>> tupleEntityTable = QueryEvaluatorUtil::evaluateTupleSynonyms(resultSynNotInClauses, resultSynTypesNotInClauses);
					resultSet = formatQueryResults(tupleEntityTable, resultSynonyms, resultTypes, resultAttrNames);
				}
				else if (!clausesResultTable.empty()) {
					std::unordered_map<std::string, std::vector<std::string>> tupleEntityTable = QueryEvaluatorUtil::evaluateTupleSynonyms(resultSynNotInClauses, resultSynTypesNotInClauses);
					std::unordered_map<std::string, std::vector<std::string>> queryResultTable = QueryEvaluatorUtil::crossProductPairOfEntityTables(tupleEntityTable, clausesResultTable);
					resultSet = formatQueryResults(queryResultTable, resultSynonyms, resultTypes, resultAttrNames);
				}
			}
		}
	}

	return resultSet;
}

std::unordered_set<std::string> QueryEvaluator::formatQueryResults(std::unordered_map<std::string, std::vector<std::string>>& resultTable, std::vector<std::string>& resultSynonyms, std::vector<ArgumentTypes>& resultTypes, std::vector<AttrName>& resultAttrNames) {
	std::unordered_set<std::string> resultSet;

	if (resultTable.empty()) {
		return resultSet;
	}

	int numTuples = resultTable.begin()->second.size();

	// i is the row number in the table
	for (int i = 0; i < numTuples; i++) {
		std::string currentStr;
		bool isFirst = true;

		// j is the jth elem in the list of result synonyms
		for (int j = 0; j < resultSynonyms.size(); j++) {
			std::string currSynonym = resultSynonyms.at(j);
			std::string currEntity = resultTable.at(currSynonym).at(i);
			ArgumentTypes currArgType = resultTypes.at(j);
			AttrName currAttrName = resultAttrNames.at(j);
			std::string currElem = QueryEvaluatorUtil::getAttrRef(currArgType, currAttrName, currEntity);

			if (isFirst) {
				isFirst = false;
			}
			else {
				currentStr.append(" ");
			}

			currentStr.append(currElem);
		}

		resultSet.emplace(currentStr);
	}

	return resultSet;
}
