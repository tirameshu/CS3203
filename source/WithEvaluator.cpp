#include "WithEvaluator.h"

bool WithEvaluator::checkWithClauseHolds(QueryWithClause& currWithClause) {
	std::string firstArg = currWithClause.getFirstArgument();
	ArgumentTypes firstArgType = currWithClause.getFirstArgumentType();
	AttrName firstAttrName = currWithClause.getFirstAttrName();
	std::string secondArg = currWithClause.getSecondArgument();
	ArgumentTypes secondArgType = currWithClause.getSecondArgumentType();
	AttrName secondAttrName = currWithClause.getSecondAttrName();
	bool withClauseHolds = false;

	std::unordered_set<std::string> firstRefSet = getRefSet(firstArg, firstArgType, firstAttrName);
	std::unordered_set<std::string> secondRefSet = getRefSet(secondArg, secondArgType, secondAttrName);

	if (firstRefSet.size() < secondRefSet.size()) {
		for (const std::string& ref : firstRefSet) {
			if (secondRefSet.count(ref) == 1) {
				withClauseHolds = true;
				break;
			}
		}
	}
	else {
		for (const std::string& ref : secondRefSet) {
			if (firstRefSet.count(ref) == 1) {
				withClauseHolds = true;
				break;
			}
		}
	}

	return withClauseHolds;
}

bool WithEvaluator::checkIsSynonym(ArgumentTypes& refType) {
	return refType != ArgumentTypes::INT && refType != ArgumentTypes::STRING;
}

bool WithEvaluator::checkRefIsIntType(ArgumentTypes& refType, AttrName& refAttrName) {
	return refType == ArgumentTypes::INT || refType == ArgumentTypes::PROG_LINE || refAttrName == AttrName::VALUE || refAttrName == AttrName::STMT_NUM;
}

std::unordered_map<std::string, std::vector<std::string>> WithEvaluator::evaluateWithClause(QueryWithClause& currWithClause) {
	std::string firstArg = currWithClause.getFirstArgument();
	ArgumentTypes firstArgType = currWithClause.getFirstArgumentType();
	AttrName firstAttrName = currWithClause.getFirstAttrName();
	std::string secondArg = currWithClause.getSecondArgument();
	ArgumentTypes secondArgType = currWithClause.getSecondArgumentType();
	AttrName secondAttrName = currWithClause.getSecondAttrName();
	std::unordered_map<std::string, std::vector<std::string>> withClauseTable;

	std::unordered_set<std::string> firstRefSet = getRefSet(firstArg, firstArgType, firstAttrName);
	std::unordered_set<std::string> secondRefSet = getRefSet(secondArg, secondArgType, secondAttrName);

	if (checkRefIsIntType(firstArgType, firstAttrName)) {
		std::vector<std::string> withRefList = getWithRefList(firstRefSet, secondRefSet);
		std::unordered_set<std::string> synonymSet = getSynonymSet(firstArg, firstArgType, secondArg, secondArgType);

		for (const std::string& synonym : synonymSet) {
			withClauseTable.emplace(synonym, withRefList);
		}
	}
	else {
		if (firstArgType == ArgumentTypes::STRING) {
			std::vector<std::string> withRefList = getWithRefList(firstRefSet, secondRefSet);
			withClauseTable.emplace(secondArg, getWithSynonymList(secondArgType, withRefList));
		}
		else if (secondArgType == ArgumentTypes::STRING) {
			std::vector<std::string> withRefList = getWithRefList(firstRefSet, secondRefSet);
			withClauseTable.emplace(firstArg, getWithSynonymList(firstArgType, withRefList));
		}
		else {
			bool isSameRefArg = firstArg == secondArg;
			auto resultLists = getWithRefPairOfList(firstArgType, firstRefSet, secondArgType, secondRefSet, isSameRefArg);
			withClauseTable.emplace(firstArg, resultLists.first); // firstWithRefList

			if (!isSameRefArg) {
				withClauseTable.emplace(secondArg, resultLists.second); // secondWithRefList
			}
		}
	}

	return withClauseTable;
}

std::unordered_set<std::string> WithEvaluator::getRefSet(std::string& refArg, ArgumentTypes& refType, AttrName& refAttrName) {
	std::unordered_set<std::string> refSet;

	if (!checkIsSynonym(refType)) {
		refSet.emplace(refArg);
	}
	else {
		refSet = QueryEvaluatorUtil::getAttrRefSet(refType, refAttrName);
	}

	return refSet;
}

std::vector<std::string> WithEvaluator::getWithRefList(std::unordered_set<std::string>& firstRefSet, std::unordered_set<std::string>& secondRefSet) {
	std::vector<std::string> withRefList;

	if (firstRefSet.size() < secondRefSet.size()) {
		for (const std::string& ref : firstRefSet) {
			if (secondRefSet.count(ref) == 1) {
				withRefList.push_back(ref);
			}
		}
	}
	else {
		for (const std::string& ref : secondRefSet) {
			if (firstRefSet.count(ref) == 1) {
				withRefList.push_back(ref);
			}
		}
	}

	return withRefList;
}

std::unordered_set<std::string> WithEvaluator::getSynonymSet(std::string& firstRefArg, ArgumentTypes& firstRefType, std::string& secondRefArg, ArgumentTypes& secondRefType) {
	std::unordered_set<std::string> synonymSet;

	if (checkIsSynonym(firstRefType)) {
		synonymSet.emplace(firstRefArg);
	}
	
	if (checkIsSynonym(secondRefType)) {
		synonymSet.emplace(secondRefArg);
	}

	return synonymSet;
}

std::unordered_set<std::string> WithEvaluator::getSynonymElemSet(ArgumentTypes& refType, const std::string& entityName) {
	std::unordered_set<std::string> synonymStmtSet;

	switch (refType) {
	case ArgumentTypes::READ:
		synonymStmtSet = QueryEvaluatorUtil::convertIntSetToStringSet(PKB::getVarToReadStmtMap().at(entityName));
		break;
	case ArgumentTypes::PRINT:
		synonymStmtSet = QueryEvaluatorUtil::convertIntSetToStringSet(PKB::getVarToPrintStmtMap().at(entityName));
		break;
	case ArgumentTypes::CALL:
		synonymStmtSet = QueryEvaluatorUtil::convertIntSetToStringSet(PKB::getProcToCallerStmtsMap().at(entityName));
		break;
	case ArgumentTypes::PROCEDURE:
		synonymStmtSet.emplace(entityName);
		break;
	case ArgumentTypes::VARIABLE:
		synonymStmtSet.emplace(entityName);
		break;
	}

	return synonymStmtSet;
}

std::vector<std::string> WithEvaluator::getWithSynonymList(ArgumentTypes& refType, std::vector<std::string>& withRefList) {
	std::vector<std::string> withSynonymList;

	if (refType == ArgumentTypes::PROCEDURE || refType == ArgumentTypes::VARIABLE) {
		withSynonymList = withRefList;
	}
	else {
		for (std::string& entityName : withRefList) {
			for (const std::string& elem : getSynonymElemSet(refType, entityName)) {
				withSynonymList.push_back(elem);
			}
		}
	}

	return withSynonymList;
}

std::pair<std::vector<std::string>, std::vector<std::string>> WithEvaluator::getWithRefPairOfList(ArgumentTypes& firstRefType, std::unordered_set<std::string>& firstRefSet, ArgumentTypes& secondRefType, std::unordered_set<std::string>& secondRefSet, bool isSameRefArg) {
	std::vector<std::string> firstWithRefList;
	std::vector<std::string> secondWithRefList;

	if (firstRefSet.size() < secondRefSet.size()) {
		for (const std::string& ref : firstRefSet) {
			if (secondRefSet.count(ref) == 1) {
				std::unordered_set<std::string> firstElemSet = getSynonymElemSet(firstRefType, ref);
				std::unordered_set<std::string> secondElemSet = getSynonymElemSet(secondRefType, ref);

				for (const std::string& firstElem : firstElemSet) {
					if (isSameRefArg) {
						firstWithRefList.push_back(firstElem);
					}
					else {
						for (const std::string& secondElem : secondElemSet) {
							firstWithRefList.push_back(firstElem);
							secondWithRefList.push_back(secondElem);
						}
					}
				}
			}
		}
	}
	else {
		for (const std::string& ref : secondRefSet) {
			if (firstRefSet.count(ref) == 1) {
				std::unordered_set<std::string> firstElemSet = getSynonymElemSet(firstRefType, ref);
				std::unordered_set<std::string> secondElemSet = getSynonymElemSet(secondRefType, ref);

				for (const std::string& firstElem : firstElemSet) {
					if (isSameRefArg) {
						firstWithRefList.push_back(firstElem);
					}
					else {
						for (const std::string& secondElem : secondElemSet) {
							firstWithRefList.push_back(firstElem);
							secondWithRefList.push_back(secondElem);
						}
					}
				}
			}
		}
	}

	return std::make_pair(firstWithRefList, secondWithRefList);
}

int WithEvaluator::estimateWithClauseTableSize(QueryWithClause& currWithClause) {
	std::string firstArg = currWithClause.getFirstArgument();
	ArgumentTypes firstArgType = currWithClause.getFirstArgumentType();
	AttrName firstAttrName = currWithClause.getFirstAttrName();
	std::string secondArg = currWithClause.getSecondArgument();
	ArgumentTypes secondArgType = currWithClause.getSecondArgumentType();
	AttrName secondAttrName = currWithClause.getSecondAttrName();
	int estimatedTableSize = 0;

	std::unordered_set<std::string>& firstRefSet = getRefSet(firstArg, firstArgType, firstAttrName);
	std::unordered_set<std::string>& secondRefSet = getRefSet(secondArg, secondArgType, secondAttrName);

	if (checkRefIsIntType(firstArgType, firstAttrName)) { // check this
		estimatedTableSize = std::min(firstRefSet.size(), secondRefSet.size());
	}
	else {
		if (firstArgType == ArgumentTypes::STRING) {
			if (secondRefSet.count(firstArg) == 1) {
				estimatedTableSize = getSynonymSetSize(secondArgType, firstArg);
			}
		}
		else if (secondArgType == ArgumentTypes::STRING) {
			if (firstRefSet.count(secondArg) == 1) {
				estimatedTableSize = getSynonymSetSize(firstArgType, secondArg);
			}
		}
		else {
			estimatedTableSize = getWithRefPairOfListSize(firstArgType, firstRefSet, secondArgType, secondRefSet, firstArg == secondArg);
		}
	}

	return estimatedTableSize;
}

int WithEvaluator::getSynonymSetSize(ArgumentTypes& refType, const std::string& entityName) {
	int listSize = 0;

	if (refType == ArgumentTypes::PROCEDURE || refType == ArgumentTypes::VARIABLE) {
		listSize = 1;
	}
	else {
		switch (refType) {
		case ArgumentTypes::READ:
			listSize = PKB::getVarToReadStmtMap().at(entityName).size();
			break;
		case ArgumentTypes::PRINT:
			listSize = PKB::getVarToPrintStmtMap().at(entityName).size();
			break;
		case ArgumentTypes::CALL:
			listSize = PKB::getProcToCallerStmtsMap().at(entityName).size();
			break;
		case ArgumentTypes::PROCEDURE:
			listSize = 1;
			break;
		case ArgumentTypes::VARIABLE:
			listSize = 1;
			break;
		}
	}

	return listSize;
}

int WithEvaluator::getWithRefPairOfListSize(ArgumentTypes& firstRefType, std::unordered_set<std::string>& firstRefSet, ArgumentTypes& secondRefType, std::unordered_set<std::string>& secondRefSet, bool isSameRefArg) {
	int listSize = 0;
	
	std::vector<std::string> firstWithRefList;
	std::vector<std::string> secondWithRefList;

	if (firstRefSet.size() < secondRefSet.size()) {
		for (const std::string& ref : firstRefSet) {
			if (secondRefSet.count(ref) == 1) {
				if (isSameRefArg) {
					listSize += getSynonymSetSize(firstRefType, ref);
				}
				else {
					listSize += getSynonymSetSize(firstRefType, ref) * getSynonymSetSize(secondRefType, ref);
				}
			}
		}
	}
	else {
		for (const std::string& ref : secondRefSet) {
			if (firstRefSet.count(ref) == 1) {
				if (isSameRefArg) {
					listSize += getSynonymSetSize(firstRefType, ref);
				}
				else {
					listSize += getSynonymSetSize(firstRefType, ref) * getSynonymSetSize(secondRefType, ref);
				}
			}
		}
	}

	return listSize;
}