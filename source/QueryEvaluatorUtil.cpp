#include <string>

#include "QueryEvaluatorUtil.h"

bool QueryEvaluatorUtil::containsResultSynonym(std::string resultSynonym, QuerySuchThatClause& currSuchThatClause) {
	std::string firstArg = currSuchThatClause.getFirstArgument();
	std::string secondArg = currSuchThatClause.getSecondArgument();

	return resultSynonym == firstArg || resultSynonym == secondArg;
}

bool QueryEvaluatorUtil::containsResultSynonym(std::string resultSynonym, QueryPatternClause& currPatternClause) {
	std::string patternName = currPatternClause.getPatternName();
	std::string firstArg = currPatternClause.getFirstArgument();

	return resultSynonym == patternName || resultSynonym == firstArg;
}

bool QueryEvaluatorUtil::containsResultSynonym(std::string resultSynonym, std::vector<QuerySuchThatClause>& suchThatClauses, std::vector<QueryPatternClause>& patternClauses) {
	for (QuerySuchThatClause& currSuchThatClause : suchThatClauses) {
		if (containsResultSynonym(resultSynonym, currSuchThatClause)) {
			return true;
		}
	}

	for (QueryPatternClause& currPatternClause : patternClauses) {
		if (containsResultSynonym(resultSynonym, currPatternClause)) {
			return true;
		}
	}

	return false;
}

bool QueryEvaluatorUtil::hasSynonym(ArgumentTypes& argType) {
	bool isSynonym = true;

	switch (argType) {
	case ArgumentTypes::INT:
		isSynonym = false;
		break;
	case ArgumentTypes::STRING:
		isSynonym = false;
		break;
	case ArgumentTypes::ANY:
		isSynonym = false;
		break;
	}

	return isSynonym;
}

bool QueryEvaluatorUtil::hasSynonym(QueryClause& currClause) {
	ArgumentTypes firstArgType = currClause.getFirstArgumentType();
	ArgumentTypes secondArgType = currClause.getSecondArgumentType();

	return hasSynonym(firstArgType) || hasSynonym(secondArgType);
}

bool QueryEvaluatorUtil::hasSharedSynonym(std::unordered_map<std::string, int>& synonymMap) {
	for (const auto& pair : synonymMap) {
		if (pair.second > 1) {
			return true;
		}
	}

	return false;
}

bool QueryEvaluatorUtil::checkPairOfClausesHold(std::unordered_map<std::string, std::vector<std::string>>& firstTable, std::unordered_map<std::string, std::vector<std::string>>& secondTable) {
	bool pairOfClausesHold = false;
	std::vector<std::string> sharedSynonymList = getSharedSynonymList(firstTable, secondTable);

	if (sharedSynonymList.empty()) {
		return pairOfClausesHold;
	}

	std::string firstSyn = sharedSynonymList.at(0);
	std::vector<std::string> firstList = firstTable.at(firstSyn);
	int firstListSize = firstList.size();
	std::vector<std::string> secondList = secondTable.at(firstSyn);
	int secondListSize = secondList.size();

	if (sharedSynonymList.size() == 1) {
		if (firstList.size() < secondList.size()) {
			for (int i = 0; i < firstListSize; i++) {
				for (int j = 0; j < secondListSize; j++) {
					if (firstList.at(i) == secondList.at(j)) {
						pairOfClausesHold = true;
						break;
					}
				}
			}
		}
		else {
			for (int i = 0; i < secondListSize; i++) {
				for (int j = 0; j < firstListSize; j++) {
					if (secondList.at(i) == firstList.at(j)) {
						pairOfClausesHold = true;
						break;
					}
				}
			}
		}
	}
	else if (sharedSynonymList.size() == 2) {
		std::string secondSyn = sharedSynonymList.at(1);
		std::vector<std::string> firstListOther = firstTable.at(secondSyn);
		std::vector<std::string> secondListOther = secondTable.at(secondSyn);

		if (firstList.size() < secondList.size()) {
			for (int i = 0; i < firstListSize; i++) {
				for (int j = 0; j < secondListSize; j++) {
					if (firstList.at(i) == secondList.at(j) && firstListOther.at(i) == secondListOther.at(j)) {
						pairOfClausesHold = true;
						break;
					}
				}
			}
		}
		else {
			for (int i = 0; i < secondListSize; i++) {
				for (int j = 0; j < firstListSize; j++) {
					if (secondList.at(i) == firstList.at(j) && secondListOther.at(i) == firstListOther.at(j)) {
						pairOfClausesHold = true;
						break;
					}
				}
			}
		}
	}

	return pairOfClausesHold;
}

std::unordered_set<std::string> QueryEvaluatorUtil::convertIntSetToStringSet(std::unordered_set<int>& intSet) {
	std::unordered_set<std::string> stringSet;

	for (const int& elem : intSet) {
		stringSet.emplace(std::to_string(elem));
	}

	return stringSet;
}

std::vector<std::string> QueryEvaluatorUtil::convertIntSetToStringList(std::unordered_set<int>& intSet) {
	std::vector<std::string> stringList;

	for (const int& elem : intSet) {
		stringList.push_back(std::to_string(elem));
	}

	return stringList;
}

std::vector<std::string> QueryEvaluatorUtil::convertStringSetToStringList(std::unordered_set<std::string>& stringSet) {
	std::vector<std::string> stringList;

	for (const std::string& elem : stringSet) {
		stringList.push_back(elem);
	}

	return stringList;
}

std::unordered_set<std::string> QueryEvaluatorUtil::getDesignEntitySet(ArgumentTypes& resultType) {
	std::unordered_set<std::string> designEntityResultSet;

	switch (resultType) {
	case ArgumentTypes::STMT:
		designEntityResultSet = convertIntSetToStringSet(PKB::getStmtList());
		break;
	case ArgumentTypes::READ:
		designEntityResultSet = convertIntSetToStringSet(PKB::getReadStmtList());
		break;
	case ArgumentTypes::PRINT:
		designEntityResultSet = convertIntSetToStringSet(PKB::getPrintStmtList());
		break;
	case ArgumentTypes::WHILE:
		designEntityResultSet = convertIntSetToStringSet(PKB::getWhileStmtList());
		break;
	case ArgumentTypes::IF:
		designEntityResultSet = convertIntSetToStringSet(PKB::getIfStmtList());
		break;
	case ArgumentTypes::ASSIGN:
		designEntityResultSet = convertIntSetToStringSet(PKB::getAssignStmtList());
		break;
	case ArgumentTypes::CALL:
		designEntityResultSet = convertIntSetToStringSet(PKB::getCallStmtList());
		break;
	case ArgumentTypes::VARIABLE:
		designEntityResultSet = PKB::getVariableList();
		break;
	case ArgumentTypes::CONSTANT:
		designEntityResultSet = PKB::getConstantList();
		break;
	case ArgumentTypes::PROCEDURE:
		designEntityResultSet = PKB::getProcedureList();
		break;
	case ArgumentTypes::PROG_LINE:
		designEntityResultSet = convertIntSetToStringSet(PKB::getStmtList());
		break;
	}

	return designEntityResultSet;
}

std::unordered_set<std::string> QueryEvaluatorUtil::getAttrRefSet(ArgumentTypes& resultType, AttrName& resultAttrName) {
	std::unordered_set<std::string> attrRefResultSet;

	switch (resultAttrName) {
	case AttrName::NONE:
		attrRefResultSet = getDesignEntitySet(resultType);
		break;
	case AttrName::PROC_NAME:
		if (resultType == ArgumentTypes::CALL) {
			attrRefResultSet = PKB::getListOfAllCallees();
		}
		else {
			attrRefResultSet = getDesignEntitySet(resultType);
		}
		break;
	case AttrName::VAR_NAME:
		if (resultType == ArgumentTypes::VARIABLE) {
			attrRefResultSet = getDesignEntitySet(resultType);
		}
		else if (resultType == ArgumentTypes::READ) {
			attrRefResultSet = PKB::getAllReadModifiedVars();
		}
		else {
			attrRefResultSet = PKB::getAllPrintUsedVarList();
		}
		break;
	case AttrName::VALUE:
		attrRefResultSet = getDesignEntitySet(resultType);
		break;
	case AttrName::STMT_NUM:
		attrRefResultSet = getDesignEntitySet(resultType);
		break;
	}

	return attrRefResultSet;
}

std::unordered_set<int> QueryEvaluatorUtil::getStmtTypeSet(ArgumentTypes& stmtType) {
	std::unordered_set<int> stmtTypeSet;

	switch (stmtType) {
	case ArgumentTypes::READ:
		stmtTypeSet = PKB::getReadStmtList();
		break;
	case ArgumentTypes::PRINT:
		stmtTypeSet = PKB::getPrintStmtList();
		break;
	case ArgumentTypes::WHILE:
		stmtTypeSet = PKB::getWhileStmtList();
		break;
	case ArgumentTypes::IF:
		stmtTypeSet = PKB::getIfStmtList();
		break;
	case ArgumentTypes::ASSIGN:
		stmtTypeSet = PKB::getAssignStmtList();
		break;
	case ArgumentTypes::CALL:
		stmtTypeSet = PKB::getCallStmtList();
		break;
	}

	return stmtTypeSet;
}

std::vector<std::string> QueryEvaluatorUtil::getSynonymList(QueryClause& currClause) {
	std::vector<std::string> synonymList;

	std::string firstArg = currClause.getFirstArgument();
	ArgumentTypes firstArgType = currClause.getFirstArgumentType();
	std::string secondArg = currClause.getSecondArgument();
	ArgumentTypes secondArgType = currClause.getSecondArgumentType();

	if (hasSynonym(firstArgType)) {
		synonymList.push_back(firstArg);
	}

	if (hasSynonym(secondArgType)) {
		synonymList.push_back(secondArg);
	}

	return synonymList;
}

std::vector<std::string> QueryEvaluatorUtil::getSynonymList(QueryPatternClause& currPatternClause) {
	std::vector<std::string> synonymList;

	std::string patternName = currPatternClause.getPatternName();
	std::string firstArg = currPatternClause.getFirstArgument();
	ArgumentTypes firstArgType = currPatternClause.getFirstArgumentType();

	synonymList.push_back(patternName);

	if (hasSynonym(firstArgType)) {
		synonymList.push_back(firstArg);
	}

	return synonymList;
}

std::unordered_map<std::string, int> QueryEvaluatorUtil::getSynonymMap(std::vector<QuerySuchThatClause>& suchThatClauses, std::vector<QueryPatternClause>& patternClauses) {
	std::unordered_map<std::string, int> synonymMap;

	for (const QuerySuchThatClause& currSuchThatClause : suchThatClauses) {
		std::string firstArg = currSuchThatClause.getFirstArgument();
		ArgumentTypes firstArgType = currSuchThatClause.getFirstArgumentType();
		std::string secondArg = currSuchThatClause.getSecondArgument();
		ArgumentTypes secondArgType = currSuchThatClause.getSecondArgumentType();

		if (hasSynonym(firstArgType)) {
			if (synonymMap.count(firstArg) == 0) {
				synonymMap.emplace(firstArg, 1);
			}
			else {
				auto itr = synonymMap.find(firstArg);
				itr->second = itr->second + 1;
			}
		}

		if (hasSynonym(secondArgType) && secondArg != firstArg) {
			if (synonymMap.count(secondArg) == 0) {
				synonymMap.emplace(secondArg, 1);
			}
			else {
				auto itr = synonymMap.find(secondArg);
				itr->second = itr->second + 1;
			}
		}
	}

	for (const QueryPatternClause& currPatternClause : patternClauses) {
		std::string patternName = currPatternClause.getPatternName();
		std::string firstArg = currPatternClause.getFirstArgument();
		ArgumentTypes firstArgType = currPatternClause.getFirstArgumentType();
		
		if (synonymMap.count(patternName) == 0) {
			synonymMap.emplace(patternName, 1);
		}
		else {
			auto itr = synonymMap.find(patternName);
			itr->second = itr->second + 1;
		}

		if (hasSynonym(firstArgType)) {
			if (synonymMap.count(firstArg) == 0) {
				synonymMap.emplace(firstArg, 1);
			}
			else {
				auto itr = synonymMap.find(firstArg);
				itr->second = itr->second + 1;
			}
		}
	}

	return synonymMap;
}

std::unordered_set<std::string> QueryEvaluatorUtil::getResultSet(std::string resultSynonym, std::unordered_map<std::string, std::vector<std::string>>& clauseTable) {
	std::unordered_set<std::string> resultSet;

	if (clauseTable.count(resultSynonym) == 1) {
		auto curr = clauseTable.at(resultSynonym);

		for (const std::string& stmt : curr) {
			resultSet.emplace(stmt);
		}
	}

	return resultSet;
}

std::vector<std::string> QueryEvaluatorUtil::getSharedSynonymList(std::unordered_map<std::string, std::vector<std::string>>& firstTable, std::unordered_map<std::string, std::vector<std::string>>& secondTable) {
	std::vector<std::string> sharedSynonymList;

	if (firstTable.size() < secondTable.size()) {
		for (const auto& firstTablePair : firstTable) {
			auto currSynonym = firstTablePair.first;

			if (secondTable.count(currSynonym) == 1) {
				sharedSynonymList.push_back(currSynonym);
			}
		}
	}
	else {
		for (const auto& secondTablePair : secondTable) {
			auto currSynonym = secondTablePair.first;

			if (firstTable.count(currSynonym) == 1) {
				sharedSynonymList.push_back(currSynonym);
			}
		}
	}

	return sharedSynonymList;
}

std::vector<std::string> QueryEvaluatorUtil::getNotSharedSynonymList(std::unordered_map<std::string, std::vector<std::string>>& firstTable, std::unordered_map<std::string, std::vector<std::string>>& secondTable) {
	std::vector<std::string> notSharedSynonymList;

	for (const auto& firstTablePair : firstTable) {
		auto currSynonym = firstTablePair.first;

		if (secondTable.count(currSynonym) != 1) {
			notSharedSynonymList.push_back(currSynonym);
		}
	}

	for (const auto& secondTablePair : secondTable) {
		auto currSynonym = secondTablePair.first;

		if (firstTable.count(currSynonym) != 1) {
			notSharedSynonymList.push_back(currSynonym);
		}
	}

	return notSharedSynonymList;
}

std::unordered_map<std::string, std::vector<std::string>> QueryEvaluatorUtil::hashJoinClauseResults(std::unordered_map<std::string, std::vector<std::string>>& firstTable, std::unordered_map<std::string, std::vector<std::string>>& secondTable) {
	std::unordered_map<std::string, std::vector<std::string>> pairOfClausesTable;

	if (firstTable.empty()) {
		return pairOfClausesTable;
	}
	else if (secondTable.empty()) {
		return pairOfClausesTable;
	}

	if (firstTable.begin()->second.empty()) {
		return pairOfClausesTable;
	}
	else if (secondTable.begin()->second.empty()) {
		return pairOfClausesTable;
	}

	std::vector<std::string> sharedSynonymList = getSharedSynonymList(firstTable, secondTable);
	std::vector<std::string> notSharedSynonymList = getNotSharedSynonymList(firstTable, secondTable);

	for (const std::string& sharedSynonym : sharedSynonymList) {
		std::vector<std::string> stringList;
		pairOfClausesTable.emplace(sharedSynonym, stringList);
	}

	for (const std::string& notSharedSynonym : notSharedSynonymList) {
		std::vector<std::string> stringList;
		pairOfClausesTable.emplace(notSharedSynonym, stringList);
	}

	std::string firstSyn = sharedSynonymList.at(0);
	std::vector<std::string> firstList = firstTable.at(firstSyn);
	int firstListSize = firstList.size();
	std::vector<std::string> secondList = secondTable.at(firstSyn);
	int secondListSize = secondList.size();

	if (sharedSynonymList.size() == 1) {
		if (firstListSize < secondListSize) {
			std::unordered_map<std::string, std::vector<int>> hashmap;

			for (int i = 0; i < firstListSize; i++) {
				std::string currString = firstList.at(i);

				if (hashmap.count(currString) == 0) {
					std::vector<int> indexList;
					hashmap.emplace(currString, indexList);
				}

				hashmap.at(currString).push_back(i);
			}

			for (int j = 0; j < secondListSize; j++) {
				std::string currString = secondList.at(j);

				if (hashmap.count(currString) == 1) {
					std::vector<int> indexList = hashmap.at(currString);

					for (int i : indexList) {
						pairOfClausesTable.at(firstSyn).push_back(currString);

						for (const auto& pair : firstTable) {
							std::string currKey = pair.first;

							if (currKey != firstSyn) {
								pairOfClausesTable.at(currKey).push_back(pair.second.at(i));
							}
						}

						for (const auto& pair : secondTable) {
							std::string currKey = pair.first;

							if (currKey != firstSyn) {
								pairOfClausesTable.at(currKey).push_back(pair.second.at(j));
							}
						}
					}
				}
			}
		}
		else {
			std::unordered_map<std::string, std::vector<int>> hashmap;

			for (int i = 0; i < secondListSize; i++) {
				std::string currString = secondList.at(i);

				if (hashmap.count(currString) == 0) {
					std::vector<int> indexList;
					hashmap.emplace(currString, indexList);
				}

				hashmap.at(currString).push_back(i);
			}

			for (int j = 0; j < firstListSize; j++) {
				std::string currString = firstList.at(j);

				if (hashmap.count(currString) == 1) {
					std::vector<int> indexList = hashmap.at(currString);

					for (int i : indexList) {
						pairOfClausesTable.at(firstSyn).push_back(currString);

						for (const auto& pair : secondTable) {
							std::string currKey = pair.first;

							if (currKey != firstSyn) {
								pairOfClausesTable.at(currKey).push_back(pair.second.at(i));
							}
						}

						for (const auto& pair : firstTable) {
							std::string currKey = pair.first;

							if (currKey != firstSyn) {
								pairOfClausesTable.at(currKey).push_back(pair.second.at(j));
							}
						}
					}
				}
			}
		}
	}
	else if (sharedSynonymList.size() == 2) {
		std::string secondSyn = sharedSynonymList.at(1);
		std::vector<std::string> firstListOther = firstTable.at(secondSyn);
		std::vector<std::string> secondListOther = secondTable.at(secondSyn);

		if (firstListSize < secondListSize) {
			std::unordered_map<std::string, std::vector<int>> hashmap;

			for (int i = 0; i < firstListSize; i++) {
				std::string currString = firstList.at(i) + " " + firstListOther.at(i);

				if (hashmap.count(currString) == 0) {
					std::vector<int> indexList;
					hashmap.emplace(currString, indexList);
				}

				hashmap.at(currString).push_back(i);
			}

			for (int j = 0; j < secondListSize; j++) {
				std::string currString = secondList.at(j);
				std::string currStringOther = secondListOther.at(j);
				std::string keyString = currString + " " + currStringOther;

				if (hashmap.count(keyString) == 1) {
					std::vector<int> indexList = hashmap.at(keyString);

					for (int i : indexList) {
						pairOfClausesTable.at(firstSyn).push_back(currString);
						pairOfClausesTable.at(secondSyn).push_back(currStringOther);

						for (const auto& pair : firstTable) {
							std::string currKey = pair.first;

							if (currKey != firstSyn && currKey != secondSyn) {
								pairOfClausesTable.at(currKey).push_back(pair.second.at(i));
							}
						}

						for (const auto& pair : secondTable) {
							std::string currKey = pair.first;

							if (currKey != firstSyn && currKey != secondSyn) {
								pairOfClausesTable.at(currKey).push_back(pair.second.at(j));
							}
						}
					}
				}
			}
		}
		else {
			std::unordered_map<std::string, std::vector<int>> hashmap;

			for (int i = 0; i < secondListSize; i++) {
				std::string currString = secondList.at(i) + " " + secondListOther.at(i);

				if (hashmap.count(currString) == 0) {
					std::vector<int> indexList;
					hashmap.emplace(currString, indexList);
				}

				hashmap.at(currString).push_back(i);
			}

			for (int j = 0; j < firstListSize; j++) {
				std::string currString = firstList.at(j);
				std::string currStringOther = firstListOther.at(j);
				std::string keyString = currString + " " + currStringOther;

				if (hashmap.count(keyString) == 1) {
					std::vector<int> indexList = hashmap.at(keyString);

					for (int i : indexList) {
						pairOfClausesTable.at(firstSyn).push_back(currString);
						pairOfClausesTable.at(secondSyn).push_back(currStringOther);

						for (const auto& pair : secondTable) {
							std::string currKey = pair.first;

							if (currKey != firstSyn && currKey != secondSyn) {
								pairOfClausesTable.at(currKey).push_back(pair.second.at(i));
							}
						}

						for (const auto& pair : firstTable) {
							std::string currKey = pair.first;

							if (currKey != firstSyn && currKey != secondSyn) {
								pairOfClausesTable.at(currKey).push_back(pair.second.at(j));
							}
						}
					}
				}
			}
		}
	}

	return pairOfClausesTable;
}

std::unordered_map<std::string, std::vector<std::string>> QueryEvaluatorUtil::crossProductPairOfEntityTables(std::unordered_map<std::string, std::vector<std::string>>& firstEntityTable, std::unordered_map<std::string, std::vector<std::string>>& secondEntityTable) {
	std::unordered_map<std::string, std::vector<std::string>> resultEntityTable;

	// Chaining of cross product can result in an empty table
	if (firstEntityTable.empty()) {
		return resultEntityTable;
	}
	else if (secondEntityTable.empty()) {
		return resultEntityTable;
	}

	// Tables may have synonyms but synonyms may not have any entities
	for (auto& pair : firstEntityTable) {
		if (pair.second.empty()) {
			return resultEntityTable;
		}

		std::vector<std::string> stringList;
		resultEntityTable.emplace(pair.first, stringList);
	}

	for (auto& pair : secondEntityTable) {
		if (pair.second.empty()) {
			return resultEntityTable;
		}

		std::vector<std::string> stringList;
		resultEntityTable.emplace(pair.first, stringList);
	}

	int numRowsInFirstTable = firstEntityTable.begin()->second.size();
	int numRowsInSecondTable = secondEntityTable.begin()->second.size();

	for (int i = 0; i < numRowsInFirstTable; i++) {
		for (int j = 0; j < numRowsInSecondTable; j++) {			
			for (auto& pair : firstEntityTable) {
				std::string currKey = pair.first;
				resultEntityTable.at(currKey).push_back(pair.second.at(i));
			}

			for (auto& pair : secondEntityTable) {
				std::string currKey = pair.first;
				resultEntityTable.at(currKey).push_back(pair.second.at(j));
			}
		}
	}

	return resultEntityTable;
}

std::unordered_map<std::string, std::vector<std::string>> QueryEvaluatorUtil::crossProductEntityTableLists(std::unordered_map<std::string, std::vector<std::string>>& entityTable) {
	std::unordered_map<std::string, std::vector<std::string>> resultEntityTable;
	bool isFirst = true;

	for (auto& pair : entityTable) {
		if (isFirst) {
			std::string firstSynonym = pair.first;
			std::vector<std::string> firstEntityList = pair.second;

			resultEntityTable.emplace(firstSynonym, firstEntityList);
			isFirst = false;
		}
		else {
			std::unordered_map<std::string, std::vector<std::string>> tempEntityTable;
			std::string currSynonym = pair.first;
			std::vector<std::string> currEntityList = pair.second;
			tempEntityTable.emplace(currSynonym, currEntityList);

			resultEntityTable = crossProductPairOfEntityTables(resultEntityTable, tempEntityTable);
		}
	}

	return resultEntityTable;
}

std::unordered_map<std::string, std::vector<std::string>> QueryEvaluatorUtil::evaluateTupleSynonyms(std::vector<std::string>& resultSynonyms, std::vector<ArgumentTypes>& resultTypes) {
	std::unordered_set<std::string> uniqueSynonymsSet;
	std::unordered_map<std::string, std::vector<std::string>> uniqueSynonymsEntityTable;

	for (int i = 0; i < resultSynonyms.size(); i++) {
		std::string synonym = resultSynonyms[i];

		if (uniqueSynonymsSet.count(synonym) == 0) {
			uniqueSynonymsSet.emplace(synonym);
			uniqueSynonymsEntityTable.emplace(synonym, QueryEvaluatorUtil::convertStringSetToStringList(QueryEvaluatorUtil::getDesignEntitySet(resultTypes[i])));
		}
	}

	return QueryEvaluatorUtil::crossProductEntityTableLists(uniqueSynonymsEntityTable);
}

std::string QueryEvaluatorUtil::getAttrRef(ArgumentTypes& resultType, AttrName& resultAttrName, std::string& entity) {
	if (resultAttrName == AttrName::PROC_NAME && resultType == ArgumentTypes::CALL) {
		return PKB::getCallerStmtToProcMap().at(std::stoi(entity));
	}
	else if (resultAttrName == AttrName::VAR_NAME) {
		if (resultType == ArgumentTypes::READ) {
			return PKB::getReadStmtToVarMap().at(std::stoi(entity));
		}
		else if (resultType == ArgumentTypes::PRINT) {
			return PKB::getPrintStmtToVarMap().at(std::stoi(entity));
		}
	}

	return entity;
}