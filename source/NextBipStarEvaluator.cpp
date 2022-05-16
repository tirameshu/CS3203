#include "NextBipStarEvaluator.h"

bool NextBipStarEvaluator::checkNextBipStarClauseHolds(QuerySuchThatClause& currSuchThatClause) {
	std::string firstArg = currSuchThatClause.getFirstArgument();
	ArgumentTypes firstArgType = currSuchThatClause.getFirstArgumentType();
	std::string secondArg = currSuchThatClause.getSecondArgument();
	ArgumentTypes secondArgType = currSuchThatClause.getSecondArgumentType();
	bool nextBipStarClauseHolds = false;

	if (firstArgType == ArgumentTypes::ANY || firstArgType == ArgumentTypes::STMT || firstArgType == ArgumentTypes::PROG_LINE) {
		if (firstArg == secondArg && firstArgType != ArgumentTypes::ANY) {
			nextBipStarClauseHolds = !PKB::getWhileStmtList().empty();
		}
		else {
			nextBipStarClauseHolds = NextBipEvaluator::checkNextBipClauseHolds(currSuchThatClause);
		}
	}
	else if (firstArgType == ArgumentTypes::INT) {
		if (secondArgType == ArgumentTypes::ANY || secondArgType == ArgumentTypes::STMT || secondArgType == ArgumentTypes::PROG_LINE) {
			nextBipStarClauseHolds = NextBipEvaluator::checkNextBipClauseHolds(currSuchThatClause);
		}
		else if (secondArgType == ArgumentTypes::INT) {
			int firstInt = std::stoi(firstArg);
			int secondInt = std::stoi(secondArg);

			nextBipStarClauseHolds = PKB::getAllNextBipStarPair().count(std::make_pair(firstInt, secondInt)) == 1;
		}
		else {
			int firstInt = std::stoi(firstArg);

			if (PKB::stmtHasNextBip(firstInt)) {
				std::unordered_set<int> nextBipLineSet = PKB::getNextBipStarListOfStmt(firstInt);
				nextBipStarClauseHolds = checkLineTypeIsNextBipStar(secondArgType, nextBipLineSet);
			}
		}
	}
	else {
		if (secondArgType == ArgumentTypes::ANY || secondArgType == ArgumentTypes::STMT || secondArgType == ArgumentTypes::PROG_LINE) {
			nextBipStarClauseHolds = checkLineTypeIsNextBipStar(firstArgType, PKB::getAllPreviousBipStmt());
		}
		else if (secondArgType == ArgumentTypes::INT) {
			int secondInt = std::stoi(secondArg);

			if (PKB::stmtHasPrevBip(secondInt)) {
				std::unordered_set<int> prevBipLineSet = PKB::getPrevBipStarListOfStmt(secondInt);
				nextBipStarClauseHolds = checkLineTypeIsNextBipStar(firstArgType, prevBipLineSet);
			}
		}
		else {
			nextBipStarClauseHolds = checkLineTypesAreNextBipStar(firstArgType, secondArgType, firstArg == secondArg);
		}
	}

	return nextBipStarClauseHolds;
}

bool NextBipStarEvaluator::checkLineTypeIsNextBipStar(ArgumentTypes& lineType, std::unordered_set<int>& nextBipStarLineSet) {
	return NextBipEvaluator::checkLineTypeIsNextBip(lineType, nextBipStarLineSet);
}

bool NextBipStarEvaluator::checkLineTypesAreNextBipStar(ArgumentTypes& firstLineType, ArgumentTypes& secondLineType, bool isSameLineRef) {
	bool lineTypesAreNextBipStar = false;

	ArgumentTypes nextBipLineType;
	std::unordered_set<int> currLineTypeSet;
	std::unordered_set<int> currNextBipLineSet;
	std::unordered_set<int> (*getNextBipStarSetOfLine)(int);

	std::unordered_set<int> firstLineTypeSet = QueryEvaluatorUtil::getStmtTypeSet(firstLineType);
	std::unordered_set<int> secondLineTypeSet = QueryEvaluatorUtil::getStmtTypeSet(secondLineType);

	if (firstLineTypeSet.size() < secondLineTypeSet.size()) {
		nextBipLineType = secondLineType;
		currLineTypeSet = firstLineTypeSet;
		currNextBipLineSet = PKB::getAllPreviousBipStmt();
		getNextBipStarSetOfLine = &PKB::getNextBipStarListOfStmt;
	}
	else {
		nextBipLineType = firstLineType;
		currLineTypeSet = secondLineTypeSet;
		currNextBipLineSet = PKB::getAllNextBipStmt();
		getNextBipStarSetOfLine = &PKB::getPrevBipStarListOfStmt;
	}

	if (currLineTypeSet.size() < currNextBipLineSet.size()) {
		for (const int& line : currLineTypeSet) {
			if (currNextBipLineSet.count(line) == 1) {
				std::unordered_set<int> nextBipLineSet = getNextBipStarSetOfLine(line);

				if (isSameLineRef) {
					if (nextBipLineSet.count(line) == 1) {
						lineTypesAreNextBipStar = true;
						break;
					}
				}
				else {
					if (checkLineTypeIsNextBipStar(nextBipLineType, nextBipLineSet)) {
						lineTypesAreNextBipStar = true;
						break;
					}
				}
			}
		}
	}
	else {
		for (const int& line : currNextBipLineSet) {
			if (currLineTypeSet.count(line) == 1) {
				std::unordered_set<int> nextBipLineSet = getNextBipStarSetOfLine(line);

				if (isSameLineRef) {
					if (nextBipLineSet.count(line) == 1) {
						lineTypesAreNextBipStar = true;
						break;
					}
				}
				else {
					if (checkLineTypeIsNextBipStar(nextBipLineType, nextBipLineSet)) {
						lineTypesAreNextBipStar = true;
						break;
					}
				}
			}
		}
	}

	return lineTypesAreNextBipStar;
}

std::unordered_map<std::string, std::vector<std::string>> NextBipStarEvaluator::evaluateNextBipStarClause(QuerySuchThatClause& currSuchThatClause) {
	std::string firstArg = currSuchThatClause.getFirstArgument();
	ArgumentTypes firstArgType = currSuchThatClause.getFirstArgumentType();
	std::string secondArg = currSuchThatClause.getSecondArgument();
	ArgumentTypes secondArgType = currSuchThatClause.getSecondArgumentType();
	std::unordered_map<std::string, std::vector<std::string>> nextBipStarClauseTable;

	if (firstArgType == ArgumentTypes::ANY) {
		nextBipStarClauseTable = NextBipEvaluator::evaluateNextBipClause(currSuchThatClause);
	}
	else if (firstArgType == ArgumentTypes::INT) {
		if (secondArgType == ArgumentTypes::STMT || secondArgType == ArgumentTypes::PROG_LINE) {
			int firstInt = std::stoi(firstArg);

			if (PKB::stmtHasNextBip(firstInt)) {
				std::vector<std::string> nextBipLineList = QueryEvaluatorUtil::convertIntSetToStringList(PKB::getNextBipStarListOfStmt(firstInt));
				nextBipStarClauseTable.emplace(secondArg, nextBipLineList);
			}
		}
		else {
			int firstInt = std::stoi(firstArg);

			if (PKB::stmtHasNextBip(firstInt)) {
				std::unordered_set<int> nextBipLineSet = PKB::getNextBipStarListOfStmt(firstInt);
				nextBipStarClauseTable.emplace(secondArg, getLineTypeIsNextBipStarList(secondArgType, nextBipLineSet));
			}
		}
	}
	else if (firstArgType == ArgumentTypes::STMT || firstArgType == ArgumentTypes::PROG_LINE) {
		if (secondArgType == ArgumentTypes::ANY) {
			nextBipStarClauseTable = NextBipEvaluator::evaluateNextBipClause(currSuchThatClause);
		}
		else if (secondArgType == ArgumentTypes::INT) {
			int secondInt = std::stoi(secondArg);

			if (PKB::stmtHasPrevBip(secondInt)) {
				std::vector<std::string> prevBipLineList = QueryEvaluatorUtil::convertIntSetToStringList(PKB::getPrevBipStarListOfStmt(secondInt));
				nextBipStarClauseTable.emplace(firstArg, prevBipLineList);
			}
		}
		else if (secondArgType == ArgumentTypes::STMT || secondArgType == ArgumentTypes::PROG_LINE) {
			std::vector<std::string> prevBipLineList;
			std::vector<std::string> nextBipLineList;

			if (firstArg == secondArg) {
				for (const std::pair<int, int>& nextBipPair : PKB::getAllNextBipStarPair()) {
					int prevBipLine = nextBipPair.first;
					int nextBipLine = nextBipPair.second;

					if (prevBipLine == nextBipLine) {
						prevBipLineList.push_back(std::to_string(prevBipLine));
					}
				}

				nextBipStarClauseTable.emplace(firstArg, prevBipLineList);
			}
			else {
				for (const std::pair<int, int>& nextBipPair : PKB::getAllNextBipStarPair()) {
					prevBipLineList.push_back(std::to_string(nextBipPair.first));
					nextBipLineList.push_back(std::to_string(nextBipPair.second));
				}

				nextBipStarClauseTable.emplace(firstArg, prevBipLineList);
				nextBipStarClauseTable.emplace(secondArg, nextBipLineList);
			}
		}
		else {
			std::vector<std::string> prevBipLineList;
			std::vector<std::string> nextBipLineList;

			for (const std::string& nextBipLine : getLineTypeIsNextBipStarList(secondArgType, PKB::getAllNextBipStmt())) {
				for (int prevBipLine : PKB::getPrevBipStarListOfStmt(std::stoi(nextBipLine))) {
					prevBipLineList.push_back(std::to_string(prevBipLine));
					nextBipLineList.push_back(nextBipLine);
				}
			}

			nextBipStarClauseTable.emplace(firstArg, prevBipLineList);
			nextBipStarClauseTable.emplace(secondArg, nextBipLineList);
		}
	}
	else {
		if (secondArgType == ArgumentTypes::ANY) {
			nextBipStarClauseTable = NextBipEvaluator::evaluateNextBipClause(currSuchThatClause);
		}
		else if (secondArgType == ArgumentTypes::INT) {
			int secondInt = std::stoi(secondArg);

			if (PKB::stmtHasPrevBip(secondInt)) {
				std::unordered_set<int> prevBipLineSet = PKB::getPrevBipStarListOfStmt(secondInt);
				nextBipStarClauseTable.emplace(firstArg, getLineTypeIsNextBipStarList(firstArgType, prevBipLineSet));
			}
		}
		else if (secondArgType == ArgumentTypes::STMT || secondArgType == ArgumentTypes::PROG_LINE) {
			std::vector<std::string> prevBipLineList;
			std::vector<std::string> nextBipLineList;

			for (const std::string& prevBipLine : getLineTypeIsNextBipStarList(firstArgType, PKB::getAllPreviousBipStmt())) {
				for (int nextBipLine : PKB::getNextBipStarListOfStmt(std::stoi(prevBipLine))) {
					prevBipLineList.push_back(prevBipLine);
					nextBipLineList.push_back(std::to_string(nextBipLine));
				}
			}

			nextBipStarClauseTable.emplace(firstArg, prevBipLineList);
			nextBipStarClauseTable.emplace(secondArg, nextBipLineList);
		}
		else {
			std::pair<std::vector<std::string>, std::vector<std::string>> nextBipListPair = getLineTypesAreNextBipStarList(firstArgType, secondArgType, firstArg == secondArg);

			nextBipStarClauseTable.emplace(firstArg, nextBipListPair.first);

			if (firstArg != secondArg) {
				nextBipStarClauseTable.emplace(secondArg, nextBipListPair.second);
			}
		}
	}

	return nextBipStarClauseTable;
}

std::vector<std::string> NextBipStarEvaluator::getLineTypeIsNextBipStarList(ArgumentTypes& lineType, std::unordered_set<int>& nextBipStarLineSet) {
	return NextBipEvaluator::getLineTypeIsNextBipList(lineType, nextBipStarLineSet);
}

std::pair<std::vector<std::string>, std::vector<std::string>> NextBipStarEvaluator::getLineTypesAreNextBipStarList(ArgumentTypes& firstLineType, ArgumentTypes& secondLineType, bool isSameLineRef) {
	std::vector<std::string> lineTypeIsPrevBipList;
	std::vector<std::string> lineTypeIsNextBipList;

	ArgumentTypes nextBipLineType;
	std::unordered_set<int> currLineTypeSet;
	std::unordered_set<int> currNextBipLineSet;
	std::unordered_set<int> (*getNextBipStarSetOfLine)(int);

	std::unordered_set<int> firstLineTypeSet = QueryEvaluatorUtil::getStmtTypeSet(firstLineType);
	std::unordered_set<int> secondLineTypeSet = QueryEvaluatorUtil::getStmtTypeSet(secondLineType);
	bool isFirstLineType = firstLineTypeSet.size() < secondLineTypeSet.size();

	if (isFirstLineType) {
		nextBipLineType = secondLineType;
		currLineTypeSet = firstLineTypeSet;
		currNextBipLineSet = PKB::getAllPreviousBipStmt();
		getNextBipStarSetOfLine = &PKB::getNextBipStarListOfStmt;
	}
	else {
		nextBipLineType = firstLineType;
		currLineTypeSet = secondLineTypeSet;
		currNextBipLineSet = PKB::getAllNextBipStmt();
		getNextBipStarSetOfLine = &PKB::getPrevBipStarListOfStmt;
	}

	if (currLineTypeSet.size() < currNextBipLineSet.size()) {
		for (const int& line : currLineTypeSet) {
			if (currNextBipLineSet.count(line) == 1) {
				std::unordered_set<int> nextBipLineSet = getNextBipStarSetOfLine(line);

				if (isSameLineRef) {
					if (nextBipLineSet.count(line) == 1) {
						lineTypeIsPrevBipList.push_back(std::to_string(line));
					}
				}
				else {
					std::vector<std::string> lineTypeNextBipList = getLineTypeIsNextBipStarList(nextBipLineType, nextBipLineSet);

					for (std::string& nextBipLine : lineTypeNextBipList) {
						if (isFirstLineType) {
							lineTypeIsPrevBipList.push_back(std::to_string(line));
							lineTypeIsNextBipList.push_back(nextBipLine);
						}
						else {
							lineTypeIsPrevBipList.push_back(nextBipLine);
							lineTypeIsNextBipList.push_back(std::to_string(line));
						}
					}
				}
			}
		}
	}
	else {
		for (const int& line : currNextBipLineSet) {
			if (currLineTypeSet.count(line) == 1) {
				std::unordered_set<int> nextBipLineSet = getNextBipStarSetOfLine(line);

				if (isSameLineRef) {
					if (nextBipLineSet.count(line) == 1) {
						lineTypeIsPrevBipList.push_back(std::to_string(line));
					}
				}
				else {
					std::vector<std::string> lineTypeNextBipList = getLineTypeIsNextBipStarList(nextBipLineType, nextBipLineSet);

					for (std::string& nextBipLine : lineTypeNextBipList) {
						if (isFirstLineType) {
							lineTypeIsPrevBipList.push_back(std::to_string(line));
							lineTypeIsNextBipList.push_back(nextBipLine);
						}
						else {
							lineTypeIsPrevBipList.push_back(nextBipLine);
							lineTypeIsNextBipList.push_back(std::to_string(line));
						}
					}
				}
			}
		}
	}

	return std::make_pair(lineTypeIsPrevBipList, lineTypeIsNextBipList);
}

int NextBipStarEvaluator::estimateNextBipStarClauseTableSize(QuerySuchThatClause& currSuchThatClause) {
	std::string firstArg = currSuchThatClause.getFirstArgument();
	ArgumentTypes firstArgType = currSuchThatClause.getFirstArgumentType();
	std::string secondArg = currSuchThatClause.getSecondArgument();
	ArgumentTypes secondArgType = currSuchThatClause.getSecondArgumentType();
	int estimatedTableSize = 0;

	if (firstArgType == ArgumentTypes::ANY) {
		estimatedTableSize = NextBipEvaluator::estimateNextBipClauseTableSize(currSuchThatClause);
	}
	else if (firstArgType == ArgumentTypes::INT) {
		if (secondArgType == ArgumentTypes::STMT || secondArgType == ArgumentTypes::PROG_LINE) {
			int firstInt = std::stoi(firstArg);

			if (PKB::stmtHasNextBip(firstInt)) {
				estimatedTableSize = PKB::getNextBipStarListOfStmt(firstInt).size();
			}
		}
		else { // check this
			int firstInt = std::stoi(firstArg);

			if (PKB::stmtHasNextBip(firstInt)) {
				estimatedTableSize = std::min(QueryEvaluatorUtil::getStmtTypeSet(secondArgType).size(), PKB::getNextBipStarListOfStmt(firstInt).size());
			}
		}
	}
	else if (firstArgType == ArgumentTypes::STMT || firstArgType == ArgumentTypes::PROG_LINE) {
		if (secondArgType == ArgumentTypes::ANY) {
			estimatedTableSize = NextBipEvaluator::estimateNextBipClauseTableSize(currSuchThatClause);
		}
		else if (secondArgType == ArgumentTypes::INT) {
			int secondInt = std::stoi(secondArg);

			if (PKB::stmtHasPrevBip(secondInt)) {
				estimatedTableSize = PKB::getPrevBipStarListOfStmt(secondInt).size();
			}
		}
		else if (secondArgType == ArgumentTypes::STMT || secondArgType == ArgumentTypes::PROG_LINE) {
			std::vector<std::string> prevBipLineList;
			std::vector<std::string> nextBipLineList;

			if (firstArg == secondArg) {
				for (const std::pair<int, int>& nextBipPair : PKB::getAllNextBipStarPair()) {
					int prevBipLine = nextBipPair.first;
					int nextBipLine = nextBipPair.second;

					if (prevBipLine == nextBipLine) {
						estimatedTableSize += 1;
					}
				}
			}
			else {
				estimatedTableSize = PKB::getAllNextBipStarPair().size();
			}
		}
		else {
			for (const std::string& nextBipLine : getLineTypeIsNextBipStarList(secondArgType, PKB::getAllNextBipStmt())) {
				estimatedTableSize += PKB::getPrevBipStarListOfStmt(std::stoi(nextBipLine)).size();
			}
		}
	}
	else {
		if (secondArgType == ArgumentTypes::ANY) {
			estimatedTableSize = NextBipEvaluator::estimateNextBipClauseTableSize(currSuchThatClause);
		}
		else if (secondArgType == ArgumentTypes::INT) { // check this
			int secondInt = std::stoi(secondArg);

			if (PKB::stmtHasPrevBip(secondInt)) {
				estimatedTableSize = std::min(QueryEvaluatorUtil::getStmtTypeSet(firstArgType).size(), PKB::getPrevBipStarListOfStmt(secondInt).size());
			}
		}
		else if (secondArgType == ArgumentTypes::STMT || secondArgType == ArgumentTypes::PROG_LINE) {
			for (const std::string& prevBipLine : getLineTypeIsNextBipStarList(firstArgType, PKB::getAllPreviousBipStmt())) {
				estimatedTableSize += PKB::getNextBipStarListOfStmt(std::stoi(prevBipLine)).size();
			}
		}
		else {
			estimatedTableSize = getStmtTypesAreNextBipStarListSize(firstArgType, secondArgType, firstArg == secondArg);
		}
	}

	return estimatedTableSize;
}

int NextBipStarEvaluator::getStmtTypesAreNextBipStarListSize(ArgumentTypes& firstLineType, ArgumentTypes& secondLineType, bool isSameLineRef) {
	int listSize = 0;

	ArgumentTypes nextBipLineType;
	std::unordered_set<int> currLineTypeSet;
	std::unordered_set<int> currNextBipLineSet;
	std::unordered_set<int>(*getNextBipStarSetOfLine)(int);

	std::unordered_set<int> firstLineTypeSet = QueryEvaluatorUtil::getStmtTypeSet(firstLineType);
	std::unordered_set<int> secondLineTypeSet = QueryEvaluatorUtil::getStmtTypeSet(secondLineType);
	bool isFirstLineType = firstLineTypeSet.size() < secondLineTypeSet.size();

	if (isFirstLineType) {
		nextBipLineType = secondLineType;
		currLineTypeSet = firstLineTypeSet;
		currNextBipLineSet = PKB::getAllPreviousBipStmt();
		getNextBipStarSetOfLine = &PKB::getNextBipStarListOfStmt;
	}
	else {
		nextBipLineType = firstLineType;
		currLineTypeSet = secondLineTypeSet;
		currNextBipLineSet = PKB::getAllNextBipStmt();
		getNextBipStarSetOfLine = &PKB::getPrevBipStarListOfStmt;
	}

	if (currLineTypeSet.size() < currNextBipLineSet.size()) {
		for (const int& line : currLineTypeSet) {
			if (currNextBipLineSet.count(line) == 1) {
				std::unordered_set<int> nextBipLineSet = getNextBipStarSetOfLine(line);

				if (isSameLineRef) {
					if (nextBipLineSet.count(line) == 1) {
						listSize += 1;
					}
				}
				else {
					listSize += std::min(QueryEvaluatorUtil::getStmtTypeSet(nextBipLineType).size(), nextBipLineSet.size());
				}
			}
		}
	}
	else {
		for (const int& line : currNextBipLineSet) {
			if (currLineTypeSet.count(line) == 1) {
				std::unordered_set<int> nextBipLineSet = getNextBipStarSetOfLine(line);

				if (isSameLineRef) {
					if (nextBipLineSet.count(line) == 1) {
						listSize += 1;
					}
				}
				else {
					listSize += std::min(QueryEvaluatorUtil::getStmtTypeSet(nextBipLineType).size(), nextBipLineSet.size());
				}
			}
		}
	}

	return listSize;
}