#include "ParentStarEvaluator.h"

bool ParentStarEvaluator::checkParentStarClauseHolds(QuerySuchThatClause& currSuchThatClause) {
	std::string firstArg = currSuchThatClause.getFirstArgument();
	ArgumentTypes firstArgType = currSuchThatClause.getFirstArgumentType();
	std::string secondArg = currSuchThatClause.getSecondArgument();
	ArgumentTypes secondArgType = currSuchThatClause.getSecondArgumentType();
	bool parentStarClauseHolds = false;

	if (firstArg == secondArg && firstArgType != ArgumentTypes::ANY) {
		return parentStarClauseHolds;
	}

	if (firstArgType == ArgumentTypes::ANY || firstArgType == ArgumentTypes::STMT || firstArgType == ArgumentTypes::PROG_LINE) {
		parentStarClauseHolds = ParentEvaluator::checkParentClauseHolds(currSuchThatClause);
	}
	else if (firstArgType == ArgumentTypes::INT) {
		if (secondArgType == ArgumentTypes::ANY || secondArgType == ArgumentTypes::STMT || secondArgType == ArgumentTypes::PROG_LINE) {
			parentStarClauseHolds = ParentEvaluator::checkParentClauseHolds(currSuchThatClause);
		}
		else if (secondArgType == ArgumentTypes::INT) {
			int firstInt = std::stoi(firstArg);
			int secondInt = std::stoi(secondArg);

			if (firstInt < secondInt) {
				parentStarClauseHolds = PKB::getAllParentStarPair().count(std::make_pair(firstInt, secondInt)) == 1;
			}
		}
		else {
			int firstInt = std::stoi(firstArg);

			if (PKB::getAllParentStmt().count(firstInt) == 1) {
				std::unordered_set<int> descendantStmtSet = PKB::getDescendantsListOfStmt(firstInt);
				parentStarClauseHolds = checkStmtTypeIsParentStar(secondArgType, descendantStmtSet);
			}
		}
	}
	else {
		if (secondArgType == ArgumentTypes::ANY || secondArgType == ArgumentTypes::STMT || secondArgType == ArgumentTypes::PROG_LINE) {
			parentStarClauseHolds = checkStmtTypeIsParentStar(firstArgType, PKB::getAllParentStmt());
		}
		else if (secondArgType == ArgumentTypes::INT) {
			int secondInt = std::stoi(secondArg);

			if (PKB::getAllChildStmt().count(secondInt) == 1) {
				std::unordered_set<int> ancestorStmtSet = PKB::getAncestorsListOfStmt(secondInt);
				parentStarClauseHolds = checkStmtTypeIsParentStar(firstArgType, ancestorStmtSet);
			}
		}
		else {
			parentStarClauseHolds = checkStmtTypesAreParentStar(firstArgType, secondArgType);
		}
	}

	return parentStarClauseHolds;
}

bool ParentStarEvaluator::checkStmtTypeIsParentStar(ArgumentTypes& stmtType, std::unordered_set<int>& parentStarStmtSet) {
	return ParentEvaluator::checkStmtTypeIsParent(stmtType, parentStarStmtSet);
}

bool ParentStarEvaluator::checkStmtTypesAreParentStar(ArgumentTypes& firstStmtType, ArgumentTypes& secondStmtType) {
	bool stmtTypesAreParentStar = false;

	ArgumentTypes nextStmtType;
	std::unordered_set<int> currStmtTypeSet;
	std::unordered_set<int> currParentStmtSet;
	std::unordered_set<int> (*getParentStarSetOfStmt)(int);

	std::unordered_set<int> firstStmtTypeSet = QueryEvaluatorUtil::getStmtTypeSet(firstStmtType);
	std::unordered_set<int> secondStmtTypeSet = QueryEvaluatorUtil::getStmtTypeSet(secondStmtType);

	if (firstStmtTypeSet.size() < secondStmtTypeSet.size()) {
		nextStmtType = secondStmtType;
		currStmtTypeSet = firstStmtTypeSet;
		currParentStmtSet = PKB::getAllParentStmt();
		getParentStarSetOfStmt = &PKB::getDescendantsListOfStmt;
	}
	else {
		nextStmtType = firstStmtType;
		currStmtTypeSet = secondStmtTypeSet;
		currParentStmtSet = PKB::getAllChildStmt();
		getParentStarSetOfStmt = &PKB::getAncestorsListOfStmt;
	}

	if (currStmtTypeSet.size() < currParentStmtSet.size()) {
		for (const int& stmt : currStmtTypeSet) {
			if (currParentStmtSet.count(stmt) == 1) {
				std::unordered_set<int> nextParentStmtSet = getParentStarSetOfStmt(stmt);

				if (checkStmtTypeIsParentStar(nextStmtType, nextParentStmtSet)) {
					stmtTypesAreParentStar = true;
					break;
				}
			}
		}
	}
	else {
		for (const int& stmt : currParentStmtSet) {
			if (currStmtTypeSet.count(stmt) == 1) {
				std::unordered_set<int> nextParentStmtSet = getParentStarSetOfStmt(stmt);

				if (checkStmtTypeIsParentStar(nextStmtType, nextParentStmtSet)) {
					stmtTypesAreParentStar = true;
					break;
				}
			}
		}
	}

	return stmtTypesAreParentStar;
}

std::unordered_map<std::string, std::vector<std::string>> ParentStarEvaluator::evaluateParentStarClause(QuerySuchThatClause& currSuchThatClause) {
	std::string firstArg = currSuchThatClause.getFirstArgument();
	ArgumentTypes firstArgType = currSuchThatClause.getFirstArgumentType();
	std::string secondArg = currSuchThatClause.getSecondArgument();
	ArgumentTypes secondArgType = currSuchThatClause.getSecondArgumentType();
	std::unordered_map<std::string, std::vector<std::string>> parentStarClauseTable;

	if (firstArg == secondArg) {
		return parentStarClauseTable;
	}

	if (firstArgType == ArgumentTypes::ANY) {
		parentStarClauseTable = ParentEvaluator::evaluateParentClause(currSuchThatClause);
	}
	else if (firstArgType == ArgumentTypes::INT) {
		if (secondArgType == ArgumentTypes::STMT || secondArgType == ArgumentTypes::PROG_LINE) {
			int firstInt = std::stoi(firstArg);

			if (PKB::getAllParentStmt().count(firstInt) == 1) {
				std::vector<std::string> descendantStmtList = QueryEvaluatorUtil::convertIntSetToStringList(PKB::getDescendantsListOfStmt(firstInt));
				parentStarClauseTable.emplace(secondArg, descendantStmtList);
			}
		}
		else {
			int firstInt = std::stoi(firstArg);

			if (PKB::getAllParentStmt().count(firstInt) == 1) {
				std::unordered_set<int> descendantStmtSet = PKB::getDescendantsListOfStmt(firstInt);
				parentStarClauseTable.emplace(secondArg, getStmtTypeIsParentStarList(secondArgType, descendantStmtSet));
			}
		}
	}
	else if (firstArgType == ArgumentTypes::STMT || firstArgType == ArgumentTypes::PROG_LINE) {
		if (secondArgType == ArgumentTypes::ANY) {
			parentStarClauseTable = ParentEvaluator::evaluateParentClause(currSuchThatClause);
		}
		else if (secondArgType == ArgumentTypes::INT) {
			int secondInt = std::stoi(secondArg);

			if (PKB::getAllChildStmt().count(secondInt) == 1) {
				std::vector<std::string> ancestorStmtList = QueryEvaluatorUtil::convertIntSetToStringList(PKB::getAncestorsListOfStmt(secondInt));
				parentStarClauseTable.emplace(firstArg, ancestorStmtList);
			}
		}
		else if (secondArgType == ArgumentTypes::STMT || secondArgType == ArgumentTypes::PROG_LINE) {
			std::vector<std::string> ancestorStmtList;
			std::vector<std::string> descendantStmtList;

			for (const std::pair<int, int>& parentStarPair : PKB::getAllParentStarPair()) {
				ancestorStmtList.push_back(std::to_string(parentStarPair.first));
				descendantStmtList.push_back(std::to_string(parentStarPair.second));
			}

			parentStarClauseTable.emplace(firstArg, ancestorStmtList);
			parentStarClauseTable.emplace(secondArg, descendantStmtList);
		}
		else {
			std::vector<std::string> ancestorStmtList;
			std::vector<std::string> descendantStmtList;

			for (const std::string& stmt : getStmtTypeIsParentStarList(secondArgType, PKB::getAllChildStmt())) {
				for (const int& ancestor : PKB::getAncestorsListOfStmt(std::stoi(stmt))) {
					ancestorStmtList.push_back(std::to_string(ancestor));
					descendantStmtList.push_back(stmt);
				}
			}

			parentStarClauseTable.emplace(firstArg, ancestorStmtList);
			parentStarClauseTable.emplace(secondArg, descendantStmtList);
		}
	}
	else {
		if (secondArgType == ArgumentTypes::ANY) {
			parentStarClauseTable = ParentEvaluator::evaluateParentClause(currSuchThatClause);
		}
		else if (secondArgType == ArgumentTypes::INT) {
			int secondInt = std::stoi(secondArg);

			if (PKB::getAllChildStmt().count(secondInt) == 1) {
				std::unordered_set<int> ancestorStmtSet = PKB::getAncestorsListOfStmt(secondInt);
				parentStarClauseTable.emplace(firstArg, getStmtTypeIsParentStarList(firstArgType, ancestorStmtSet));
			}
		}
		else if (secondArgType == ArgumentTypes::STMT || secondArgType == ArgumentTypes::PROG_LINE) {
			std::vector<std::string> ancestorStmtList;
			std::vector<std::string> descendantStmtList;

			for (const std::string& stmt : getStmtTypeIsParentStarList(firstArgType, PKB::getAllParentStmt())) {
				for (const int& descendant : PKB::getDescendantsListOfStmt(std::stoi(stmt))) {
					ancestorStmtList.push_back(stmt);
					descendantStmtList.push_back(std::to_string(descendant));
				}
			}

			parentStarClauseTable.emplace(firstArg, ancestorStmtList);
			parentStarClauseTable.emplace(secondArg, descendantStmtList);
		}
		else {
			std::pair<std::vector<std::string>, std::vector<std::string>> parentStarListPair = getStmtTypesAreParentStarList(firstArgType, secondArgType);
			parentStarClauseTable.emplace(firstArg, parentStarListPair.first);
			parentStarClauseTable.emplace(secondArg, parentStarListPair.second);
		}
	}

	return parentStarClauseTable;
}

std::vector<std::string> ParentStarEvaluator::getStmtTypeIsParentStarList(ArgumentTypes& stmtType, std::unordered_set<int>& parentStarStmtSet) {
	return ParentEvaluator::getStmtTypeIsParentList(stmtType, parentStarStmtSet);
}

std::pair<std::vector<std::string>, std::vector<std::string>> ParentStarEvaluator::getStmtTypesAreParentStarList(ArgumentTypes& firstStmtType, ArgumentTypes& secondStmtType) {
	std::vector<std::string> stmtTypeIsAncestorList;
	std::vector<std::string> stmtTypeIsDescendantList;

	ArgumentTypes nextStmtType;
	std::unordered_set<int> currStmtTypeSet;
	std::unordered_set<int> currParentStmtSet;
	std::unordered_set<int> (*getParentStarSetOfStmt)(int);

	std::unordered_set<int> firstStmtTypeSet = QueryEvaluatorUtil::getStmtTypeSet(firstStmtType);
	std::unordered_set<int> secondStmtTypeSet = QueryEvaluatorUtil::getStmtTypeSet(secondStmtType);
	bool isFirstStmtType = firstStmtTypeSet.size() < secondStmtTypeSet.size();

	if (isFirstStmtType) {
		nextStmtType = secondStmtType;
		currStmtTypeSet = firstStmtTypeSet;
		currParentStmtSet = PKB::getAllParentStmt();
		getParentStarSetOfStmt = &PKB::getDescendantsListOfStmt;
	}
	else {
		nextStmtType = firstStmtType;
		currStmtTypeSet = secondStmtTypeSet;
		currParentStmtSet = PKB::getAllChildStmt();
		getParentStarSetOfStmt = &PKB::getAncestorsListOfStmt;
	}

	if (currStmtTypeSet.size() < currParentStmtSet.size()) {
		for (const int& stmt : currStmtTypeSet) {
			if (currParentStmtSet.count(stmt) == 1) {
				std::unordered_set<int> nextParentStmtSet = getParentStarSetOfStmt(stmt);
				std::vector<std::string> nextStmtTypeParentList = getStmtTypeIsParentStarList(nextStmtType, nextParentStmtSet);
				std::string stmtString = std::to_string(stmt);

				for (const std::string& stmt : nextStmtTypeParentList) {
					if (isFirstStmtType) {
						stmtTypeIsAncestorList.push_back(stmtString);
						stmtTypeIsDescendantList.push_back(stmt);
					}
					else {
						stmtTypeIsAncestorList.push_back(stmt);
						stmtTypeIsDescendantList.push_back(stmtString);
					}
				}
			}
		}
	}
	else {
		for (const int& stmt : currParentStmtSet) {
			if (currStmtTypeSet.count(stmt) == 1) {
				std::unordered_set<int> nextParentStmtSet = getParentStarSetOfStmt(stmt);
				std::vector<std::string> nextStmtTypeParentList = getStmtTypeIsParentStarList(nextStmtType, nextParentStmtSet);
				std::string stmtString = std::to_string(stmt);

				for (const std::string& stmt : nextStmtTypeParentList) {
					if (isFirstStmtType) {
						stmtTypeIsAncestorList.push_back(stmtString);
						stmtTypeIsDescendantList.push_back(stmt);
					}
					else {
						stmtTypeIsAncestorList.push_back(stmt);
						stmtTypeIsDescendantList.push_back(stmtString);
					}
				}
			}
		}
	}

	return std::make_pair(stmtTypeIsAncestorList, stmtTypeIsDescendantList);
}

int ParentStarEvaluator::estimateParentStarClauseTableSize(QuerySuchThatClause& currSuchThatClause) {
	std::string firstArg = currSuchThatClause.getFirstArgument();
	ArgumentTypes firstArgType = currSuchThatClause.getFirstArgumentType();
	std::string secondArg = currSuchThatClause.getSecondArgument();
	ArgumentTypes secondArgType = currSuchThatClause.getSecondArgumentType();
	int estimatedTableSize = 0;

	if (firstArg == secondArg) {
		return estimatedTableSize;
	}

	if (firstArgType == ArgumentTypes::ANY) {
		estimatedTableSize = ParentEvaluator::estimateParentClauseTableSize(currSuchThatClause);
	}
	else if (firstArgType == ArgumentTypes::INT) {
		if (secondArgType == ArgumentTypes::STMT || secondArgType == ArgumentTypes::PROG_LINE) {
			int firstInt = std::stoi(firstArg);

			if (PKB::getAllParentStmt().count(firstInt) == 1) {
				estimatedTableSize = PKB::getDescendantsListOfStmt(firstInt).size();
			}
		}
		else { // check this
			int firstInt = std::stoi(firstArg);

			if (PKB::getAllParentStmt().count(firstInt) == 1) {
				estimatedTableSize = std::min(QueryEvaluatorUtil::getStmtTypeSet(secondArgType).size(), PKB::getDescendantsListOfStmt(firstInt).size());
			}
		}
	}
	else if (firstArgType == ArgumentTypes::STMT || firstArgType == ArgumentTypes::PROG_LINE) {
		if (secondArgType == ArgumentTypes::ANY) {
			estimatedTableSize = ParentEvaluator::estimateParentClauseTableSize(currSuchThatClause);
		}
		else if (secondArgType == ArgumentTypes::INT) {
			int secondInt = std::stoi(secondArg);

			if (PKB::getAllChildStmt().count(secondInt) == 1) {
				estimatedTableSize = PKB::getAncestorsListOfStmt(secondInt).size();
			}
		}
		else if (secondArgType == ArgumentTypes::STMT || secondArgType == ArgumentTypes::PROG_LINE) {
			estimatedTableSize = PKB::getAllParentStarPair().size();
		}
		else {
			for (const std::string& stmt : getStmtTypeIsParentStarList(secondArgType, PKB::getAllChildStmt())) {
				estimatedTableSize += PKB::getAncestorsListOfStmt(std::stoi(stmt)).size();
			}
		}
	}
	else {
		if (secondArgType == ArgumentTypes::ANY) {
			estimatedTableSize = ParentEvaluator::estimateParentClauseTableSize(currSuchThatClause);
		}
		else if (secondArgType == ArgumentTypes::INT) { // check this
			int secondInt = std::stoi(secondArg);

			if (PKB::getAllChildStmt().count(secondInt) == 1) {
				estimatedTableSize = std::min(QueryEvaluatorUtil::getStmtTypeSet(firstArgType).size(), PKB::getAncestorsListOfStmt(secondInt).size());
			}
		}
		else if (secondArgType == ArgumentTypes::STMT || secondArgType == ArgumentTypes::PROG_LINE) {
			for (const std::string& stmt : getStmtTypeIsParentStarList(firstArgType, PKB::getAllParentStmt())) {
				estimatedTableSize += PKB::getDescendantsListOfStmt(std::stoi(stmt)).size();
			}
		}
		else {
			estimatedTableSize = getStmtTypesAreParentStarListSize(firstArgType, secondArgType);
		}
	}

	return estimatedTableSize;
}

int ParentStarEvaluator::getStmtTypesAreParentStarListSize(ArgumentTypes& firstStmtType, ArgumentTypes& secondStmtType) {
	int listSize = 0;

	std::vector<std::string> stmtTypeIsAncestorList;
	std::vector<std::string> stmtTypeIsDescendantList;

	ArgumentTypes nextStmtType;
	std::unordered_set<int> currStmtTypeSet;
	std::unordered_set<int> currParentStmtSet;
	std::unordered_set<int> (*getParentStarSetOfStmt)(int);

	std::unordered_set<int> firstStmtTypeSet = QueryEvaluatorUtil::getStmtTypeSet(firstStmtType);
	std::unordered_set<int> secondStmtTypeSet = QueryEvaluatorUtil::getStmtTypeSet(secondStmtType);
	bool isFirstStmtType = firstStmtTypeSet.size() < secondStmtTypeSet.size();

	if (isFirstStmtType) {
		nextStmtType = secondStmtType;
		currStmtTypeSet = firstStmtTypeSet;
		currParentStmtSet = PKB::getAllParentStmt();
		getParentStarSetOfStmt = &PKB::getDescendantsListOfStmt;
	}
	else {
		nextStmtType = firstStmtType;
		currStmtTypeSet = secondStmtTypeSet;
		currParentStmtSet = PKB::getAllChildStmt();
		getParentStarSetOfStmt = &PKB::getAncestorsListOfStmt;
	}

	if (currStmtTypeSet.size() < currParentStmtSet.size()) {
		for (const int& stmt : currStmtTypeSet) {
			if (currParentStmtSet.count(stmt) == 1) {
				listSize += std::min(QueryEvaluatorUtil::getStmtTypeSet(nextStmtType).size(), getParentStarSetOfStmt(stmt).size());
			}
		}
	}
	else {
		for (const int& stmt : currParentStmtSet) {
			if (currStmtTypeSet.count(stmt) == 1) {
				listSize += std::min(QueryEvaluatorUtil::getStmtTypeSet(nextStmtType).size(), getParentStarSetOfStmt(stmt).size());
			}
		}
	}

	return listSize;
}