#include "NextStarEvaluator.h"

bool NextStarEvaluator::checkNextStarClauseHolds(QuerySuchThatClause& currSuchThatClause) {
	std::string firstArg = currSuchThatClause.getFirstArgument();
	ArgumentTypes firstArgType = currSuchThatClause.getFirstArgumentType();
	std::string secondArg = currSuchThatClause.getSecondArgument();
	ArgumentTypes secondArgType = currSuchThatClause.getSecondArgumentType();
	bool nextStarClauseHolds = false;

	if (firstArgType == ArgumentTypes::ANY || firstArgType == ArgumentTypes::STMT || firstArgType == ArgumentTypes::PROG_LINE) {
		if (firstArg == secondArg && firstArgType != ArgumentTypes::ANY) {
			nextStarClauseHolds = !PKB::getWhileStmtList().empty();
		}
		else {
			nextStarClauseHolds = NextEvaluator::checkNextClauseHolds(currSuchThatClause);
		}
	}
	else if (firstArgType == ArgumentTypes::INT) {
		if (secondArgType == ArgumentTypes::ANY || secondArgType == ArgumentTypes::STMT || secondArgType == ArgumentTypes::PROG_LINE) {
			nextStarClauseHolds = NextEvaluator::checkNextClauseHolds(currSuchThatClause);
		}
		else if (secondArgType == ArgumentTypes::INT) {
			int firstInt = std::stoi(firstArg);
			int secondInt = std::stoi(secondArg);

			nextStarClauseHolds = PKB::getAllNextStarPair().count(std::make_pair(firstInt, secondInt)) == 1;
		}
		else {
			int firstInt = std::stoi(firstArg);

			if (PKB::stmtHasNext(firstInt)) {
				std::unordered_set<int> nextLineSet = PKB::getNextStarListOfStmt(firstInt);
				nextStarClauseHolds = checkLineTypeIsNextStar(secondArgType, nextLineSet);
			}
		}
	}
	else {
		if (secondArgType == ArgumentTypes::ANY || secondArgType == ArgumentTypes::STMT || secondArgType == ArgumentTypes::PROG_LINE) {
			nextStarClauseHolds = checkLineTypeIsNextStar(firstArgType, PKB::getAllPreviousStmt());
		}
		else if (secondArgType == ArgumentTypes::INT) {
			int secondInt = std::stoi(secondArg);

			if (PKB::stmtHasPrev(secondInt)) {
				std::unordered_set<int> prevLineSet = PKB::getPrevStarListOfStmt(secondInt);
				nextStarClauseHolds = checkLineTypeIsNextStar(firstArgType, prevLineSet);
			}
		}
		else {
			nextStarClauseHolds = checkLineTypesAreNextStar(firstArgType, secondArgType, firstArg == secondArg);
		}
	}

	return nextStarClauseHolds;
}

bool NextStarEvaluator::checkLineTypeIsNextStar(ArgumentTypes& lineType, std::unordered_set<int>& nextStarLineSet) {
	return NextEvaluator::checkLineTypeIsNext(lineType, nextStarLineSet);
}

bool NextStarEvaluator::checkLineTypesAreNextStar(ArgumentTypes& firstLineType, ArgumentTypes& secondLineType, bool isSameLineRef) {
	bool lineTypesAreNextStar = false;

	ArgumentTypes nextLineType;
	std::unordered_set<int> currLineTypeSet;
	std::unordered_set<int> currNextLineSet;
	std::unordered_set<int> (*getNextStarSetOfLine)(int);

	std::unordered_set<int> firstLineTypeSet = QueryEvaluatorUtil::getStmtTypeSet(firstLineType);
	std::unordered_set<int> secondLineTypeSet = QueryEvaluatorUtil::getStmtTypeSet(secondLineType);

	if (firstLineTypeSet.size() < secondLineTypeSet.size()) {
		nextLineType = secondLineType;
		currLineTypeSet = firstLineTypeSet;
		currNextLineSet = PKB::getAllPreviousStmt();
		getNextStarSetOfLine = &PKB::getNextStarListOfStmt;
	}
	else {
		nextLineType = firstLineType;
		currLineTypeSet = secondLineTypeSet;
		currNextLineSet = PKB::getAllNextStmt();
		getNextStarSetOfLine = &PKB::getPrevStarListOfStmt;
	}

	if (currLineTypeSet.size() < currNextLineSet.size()) {
		for (const int& line : currLineTypeSet) {
			if (currNextLineSet.count(line) == 1) {
				std::unordered_set<int> nextLineSet = getNextStarSetOfLine(line);

				if (isSameLineRef) {
					if (nextLineSet.count(line) == 1) {
						lineTypesAreNextStar = true;
						break;
					}
				}
				else {
					if (checkLineTypeIsNextStar(nextLineType, nextLineSet)) {
						lineTypesAreNextStar = true;
						break;
					}
				}
			}
		}
	}
	else {
		for (const int& line : currNextLineSet) {
			if (currLineTypeSet.count(line) == 1) {
				std::unordered_set<int> nextLineSet = getNextStarSetOfLine(line);

				if (isSameLineRef) {
					if (nextLineSet.count(line) == 1) {
						lineTypesAreNextStar = true;
						break;
					}
				}
				else {
					if (checkLineTypeIsNextStar(nextLineType, nextLineSet)) {
						lineTypesAreNextStar = true;
						break;
					}
				}
			}
		}
	}

	return lineTypesAreNextStar;
}

std::unordered_map<std::string, std::vector<std::string>> NextStarEvaluator::evaluateNextStarClause(QuerySuchThatClause& currSuchThatClause) {
	std::string firstArg = currSuchThatClause.getFirstArgument();
	ArgumentTypes firstArgType = currSuchThatClause.getFirstArgumentType();
	std::string secondArg = currSuchThatClause.getSecondArgument();
	ArgumentTypes secondArgType = currSuchThatClause.getSecondArgumentType();
	std::unordered_map<std::string, std::vector<std::string>> nextStarClauseTable;

	if (firstArgType == ArgumentTypes::ANY) {
		nextStarClauseTable = NextEvaluator::evaluateNextClause(currSuchThatClause);
	}
	else if (firstArgType == ArgumentTypes::INT) {
		if (secondArgType == ArgumentTypes::STMT || secondArgType == ArgumentTypes::PROG_LINE) {
			int firstInt = std::stoi(firstArg);

			if (PKB::stmtHasNext(firstInt)) {
				std::vector<std::string> nextLineList = QueryEvaluatorUtil::convertIntSetToStringList(PKB::getNextStarListOfStmt(firstInt));
				nextStarClauseTable.emplace(secondArg, nextLineList);
			}
		}
		else {
			int firstInt = std::stoi(firstArg);

			if (PKB::stmtHasNext(firstInt)) {
				std::unordered_set<int> nextLineSet = PKB::getNextStarListOfStmt(firstInt);
				nextStarClauseTable.emplace(secondArg, getLineTypeIsNextStarList(secondArgType, nextLineSet));
			}
		}
	}
	else if (firstArgType == ArgumentTypes::STMT || firstArgType == ArgumentTypes::PROG_LINE) {
		if (secondArgType == ArgumentTypes::ANY) {
			nextStarClauseTable = NextEvaluator::evaluateNextClause(currSuchThatClause);
		}
		else if (secondArgType == ArgumentTypes::INT) {
			int secondInt = std::stoi(secondArg);

			if (PKB::stmtHasPrev(secondInt)) {
				std::vector<std::string> prevLineList = QueryEvaluatorUtil::convertIntSetToStringList(PKB::getPrevStarListOfStmt(secondInt));
				nextStarClauseTable.emplace(firstArg, prevLineList);
			}
		}
		else if (secondArgType == ArgumentTypes::STMT || secondArgType == ArgumentTypes::PROG_LINE) {
			std::vector<std::string> prevLineList;
			std::vector<std::string> nextLineList;

			if (firstArg == secondArg) {
				for (const std::pair<int, int>& nextPair : PKB::getAllNextStarPair()) {
					int prevLine = nextPair.first;
					int nextLine = nextPair.second;

					if (prevLine == nextLine) {
						prevLineList.push_back(std::to_string(prevLine));
					}
				}

				nextStarClauseTable.emplace(firstArg, prevLineList);
			}
			else {
				for (const std::pair<int, int>& nextPair : PKB::getAllNextStarPair()) {
					prevLineList.push_back(std::to_string(nextPair.first));
					nextLineList.push_back(std::to_string(nextPair.second));
				}

				nextStarClauseTable.emplace(firstArg, prevLineList);
				nextStarClauseTable.emplace(secondArg, nextLineList);
			}
		}
		else {
			std::vector<std::string> prevLineList;
			std::vector<std::string> nextLineList;

			for (const std::string& nextLine : getLineTypeIsNextStarList(secondArgType, PKB::getAllNextStmt())) {
				for (int prevLine : PKB::getPrevStarListOfStmt(std::stoi(nextLine))) {
					prevLineList.push_back(std::to_string(prevLine));
					nextLineList.push_back(nextLine);
				}
			}

			nextStarClauseTable.emplace(firstArg, prevLineList);
			nextStarClauseTable.emplace(secondArg, nextLineList);
		}
	}
	else {
		if (secondArgType == ArgumentTypes::ANY) {
			nextStarClauseTable = NextEvaluator::evaluateNextClause(currSuchThatClause);
		}
		else if (secondArgType == ArgumentTypes::INT) {
			int secondInt = std::stoi(secondArg);

			if (PKB::stmtHasPrev(secondInt)) {
				std::unordered_set<int> prevLineSet = PKB::getPrevStarListOfStmt(secondInt);
				nextStarClauseTable.emplace(firstArg, getLineTypeIsNextStarList(firstArgType, prevLineSet));
			}
		}
		else if (secondArgType == ArgumentTypes::STMT || secondArgType == ArgumentTypes::PROG_LINE) {
			std::vector<std::string> prevLineList;
			std::vector<std::string> nextLineList;

			for (const std::string& prevLine : getLineTypeIsNextStarList(firstArgType, PKB::getAllPreviousStmt())) {
				for (int nextLine : PKB::getNextStarListOfStmt(std::stoi(prevLine))) {
					prevLineList.push_back(prevLine);
					nextLineList.push_back(std::to_string(nextLine));
				}
			}

			nextStarClauseTable.emplace(firstArg, prevLineList);
			nextStarClauseTable.emplace(secondArg, nextLineList);
		}
		else {
			std::pair<std::vector<std::string>, std::vector<std::string>> nextListPair = getLineTypesAreNextStarList(firstArgType, secondArgType, firstArg == secondArg);

			nextStarClauseTable.emplace(firstArg, nextListPair.first);

			if (firstArg != secondArg) {
				nextStarClauseTable.emplace(secondArg, nextListPair.second);
			}
		}
	}

	return nextStarClauseTable;
}

std::vector<std::string> NextStarEvaluator::getLineTypeIsNextStarList(ArgumentTypes& lineType, std::unordered_set<int>& nextStarLineSet) {
	return NextEvaluator::getLineTypeIsNextList(lineType, nextStarLineSet);
}

std::pair<std::vector<std::string>, std::vector<std::string>> NextStarEvaluator::getLineTypesAreNextStarList(ArgumentTypes& firstLineType, ArgumentTypes& secondLineType, bool isSameLineRef) {
	std::vector<std::string> lineTypeIsPrevList;
	std::vector<std::string> lineTypeIsNextList;

	ArgumentTypes nextLineType;
	std::unordered_set<int> currLineTypeSet;
	std::unordered_set<int> currNextLineSet;
	std::unordered_set<int> (*getNextStarSetOfLine)(int);

	std::unordered_set<int> firstLineTypeSet = QueryEvaluatorUtil::getStmtTypeSet(firstLineType);
	std::unordered_set<int> secondLineTypeSet = QueryEvaluatorUtil::getStmtTypeSet(secondLineType);
	bool isFirstLineType = firstLineTypeSet.size() < secondLineTypeSet.size();

	if (isFirstLineType) {
		nextLineType = secondLineType;
		currLineTypeSet = firstLineTypeSet;
		currNextLineSet = PKB::getAllPreviousStmt();
		getNextStarSetOfLine = &PKB::getNextStarListOfStmt;
	}
	else {
		nextLineType = firstLineType;
		currLineTypeSet = secondLineTypeSet;
		currNextLineSet = PKB::getAllNextStmt();
		getNextStarSetOfLine = &PKB::getPrevStarListOfStmt;
	}

	if (currLineTypeSet.size() < currNextLineSet.size()) {
		for (const int& line : currLineTypeSet) {
			if (currNextLineSet.count(line) == 1) {
				std::unordered_set<int> nextLineSet = getNextStarSetOfLine(line);

				if (isSameLineRef) {
					if (nextLineSet.count(line) == 1) {
						lineTypeIsPrevList.push_back(std::to_string(line));
					}
				}
				else {
					std::vector<std::string> lineTypeNextList = getLineTypeIsNextStarList(nextLineType, nextLineSet);

					for (std::string& nextLine : lineTypeNextList) {
						if (isFirstLineType) {
							lineTypeIsPrevList.push_back(std::to_string(line));
							lineTypeIsNextList.push_back(nextLine);
						}
						else {
							lineTypeIsPrevList.push_back(nextLine);
							lineTypeIsNextList.push_back(std::to_string(line));
						}
					}
				}
			}
		}
	}
	else {
		for (const int& line : currNextLineSet) {
			if (currLineTypeSet.count(line) == 1) {
				std::unordered_set<int> nextLineSet = getNextStarSetOfLine(line);

				if (isSameLineRef) {
					if (nextLineSet.count(line) == 1) {
						lineTypeIsPrevList.push_back(std::to_string(line));
					}
				}
				else {
					std::vector<std::string> lineTypeNextList = getLineTypeIsNextStarList(nextLineType, nextLineSet);

					for (std::string& nextLine : lineTypeNextList) {
						if (isFirstLineType) {
							lineTypeIsPrevList.push_back(std::to_string(line));
							lineTypeIsNextList.push_back(nextLine);
						}
						else {
							lineTypeIsPrevList.push_back(nextLine);
							lineTypeIsNextList.push_back(std::to_string(line));
						}
					}
				}
			}
		}
	}

	return std::make_pair(lineTypeIsPrevList, lineTypeIsNextList);
}

int NextStarEvaluator::estimateNextStarClauseTableSize(QuerySuchThatClause& currSuchThatClause) {
	std::string firstArg = currSuchThatClause.getFirstArgument();
	ArgumentTypes firstArgType = currSuchThatClause.getFirstArgumentType();
	std::string secondArg = currSuchThatClause.getSecondArgument();
	ArgumentTypes secondArgType = currSuchThatClause.getSecondArgumentType();
	int estimatedTableSize = 0;

	if (firstArgType == ArgumentTypes::ANY) {
		estimatedTableSize = NextEvaluator::estimateNextClauseTableSize(currSuchThatClause);
	}
	else if (firstArgType == ArgumentTypes::INT) {
		if (secondArgType == ArgumentTypes::STMT || secondArgType == ArgumentTypes::PROG_LINE) {
			int firstInt = std::stoi(firstArg);

			if (PKB::stmtHasNext(firstInt)) {
				estimatedTableSize = PKB::getNextStarListOfStmt(firstInt).size();
			}
		}
		else { // check this
			int firstInt = std::stoi(firstArg);

			if (PKB::stmtHasNext(firstInt)) {
				estimatedTableSize = std::min(QueryEvaluatorUtil::getStmtTypeSet(secondArgType).size(), PKB::getNextStarListOfStmt(firstInt).size());
			}
		}
	}
	else if (firstArgType == ArgumentTypes::STMT || firstArgType == ArgumentTypes::PROG_LINE) {
		if (secondArgType == ArgumentTypes::ANY) {
			estimatedTableSize = NextEvaluator::estimateNextClauseTableSize(currSuchThatClause);
		}
		else if (secondArgType == ArgumentTypes::INT) {
			int secondInt = std::stoi(secondArg);

			if (PKB::stmtHasPrev(secondInt)) {
				estimatedTableSize = PKB::getPrevStarListOfStmt(secondInt).size();
			}
		}
		else if (secondArgType == ArgumentTypes::STMT || secondArgType == ArgumentTypes::PROG_LINE) {
			if (firstArg == secondArg) {
				for (const std::pair<int, int>& nextPair : PKB::getAllNextStarPair()) {
					int prevLine = nextPair.first;
					int nextLine = nextPair.second;

					if (prevLine == nextLine) {
						estimatedTableSize += 1;
					}
				}
			}
			else {
				estimatedTableSize = PKB::getAllNextStarPair().size();
			}
		}
		else {
			for (const std::string& nextLine : getLineTypeIsNextStarList(secondArgType, PKB::getAllNextStmt())) {
				estimatedTableSize += PKB::getPrevStarListOfStmt(std::stoi(nextLine)).size();
			}
		}
	}
	else {
		if (secondArgType == ArgumentTypes::ANY) {
			estimatedTableSize = NextEvaluator::estimateNextClauseTableSize(currSuchThatClause);
		}
		else if (secondArgType == ArgumentTypes::INT) {
			int secondInt = std::stoi(secondArg);

			if (PKB::stmtHasPrev(secondInt)) {
				estimatedTableSize = std::min(QueryEvaluatorUtil::getStmtTypeSet(firstArgType).size(), PKB::getPrevStarListOfStmt(secondInt).size());
			}
		}
		else if (secondArgType == ArgumentTypes::STMT || secondArgType == ArgumentTypes::PROG_LINE) {
			for (const std::string& prevLine : getLineTypeIsNextStarList(firstArgType, PKB::getAllPreviousStmt())) {
				estimatedTableSize += PKB::getNextStarListOfStmt(std::stoi(prevLine)).size();
			}
		}
		else {
			estimatedTableSize = getStmtTypesAreNextStarListSize(firstArgType, secondArgType, firstArg == secondArg);
		}
	}

	return estimatedTableSize;
}

int NextStarEvaluator::getStmtTypesAreNextStarListSize(ArgumentTypes& firstLineType, ArgumentTypes& secondLineType, bool isSameLineRef) {
	int listSize = 0;

	ArgumentTypes nextLineType;
	std::unordered_set<int> currLineTypeSet;
	std::unordered_set<int> currNextLineSet;
	std::unordered_set<int>(*getNextStarSetOfLine)(int);

	std::unordered_set<int> firstLineTypeSet = QueryEvaluatorUtil::getStmtTypeSet(firstLineType);
	std::unordered_set<int> secondLineTypeSet = QueryEvaluatorUtil::getStmtTypeSet(secondLineType);
	bool isFirstLineType = firstLineTypeSet.size() < secondLineTypeSet.size();

	if (isFirstLineType) {
		nextLineType = secondLineType;
		currLineTypeSet = firstLineTypeSet;
		currNextLineSet = PKB::getAllPreviousStmt();
		getNextStarSetOfLine = &PKB::getNextStarListOfStmt;
	}
	else {
		nextLineType = firstLineType;
		currLineTypeSet = secondLineTypeSet;
		currNextLineSet = PKB::getAllNextStmt();
		getNextStarSetOfLine = &PKB::getPrevStarListOfStmt;
	}

	if (currLineTypeSet.size() < currNextLineSet.size()) {
		for (const int& line : currLineTypeSet) {
			if (currNextLineSet.count(line) == 1) {
				std::unordered_set<int>& nextLineSet = getNextStarSetOfLine(line);

				if (isSameLineRef) {
					if (nextLineSet.count(line) == 1) {
						listSize += 1;
					}
				}
				else {
					listSize += std::min(QueryEvaluatorUtil::getStmtTypeSet(nextLineType).size(), nextLineSet.size());
				}
			}
		}
	}
	else {
		for (const int& line : currNextLineSet) {
			if (currLineTypeSet.count(line) == 1) {
				std::unordered_set<int> nextLineSet = getNextStarSetOfLine(line);

				if (isSameLineRef) {
					if (nextLineSet.count(line) == 1) {
						listSize += 1;
					}
				}
				else {
					listSize += std::min(QueryEvaluatorUtil::getStmtTypeSet(nextLineType).size(), nextLineSet.size());
				}
			}
		}
	}

	return listSize;
}