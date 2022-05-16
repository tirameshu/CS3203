#include "NextEvaluator.h"

bool NextEvaluator::checkNextClauseHolds(QuerySuchThatClause& currSuchThatClause) {
	std::string firstArg = currSuchThatClause.getFirstArgument();
	ArgumentTypes firstArgType = currSuchThatClause.getFirstArgumentType();
	std::string secondArg = currSuchThatClause.getSecondArgument();
	ArgumentTypes secondArgType = currSuchThatClause.getSecondArgumentType();
	bool nextClauseHolds = false;

	// Next(s, s) is not possible
	if (firstArg == secondArg && firstArgType != ArgumentTypes::ANY) {
		return nextClauseHolds;
	}

	if (firstArgType == ArgumentTypes::ANY || firstArgType == ArgumentTypes::STMT || firstArgType == ArgumentTypes::PROG_LINE) {
		if (secondArgType == ArgumentTypes::ANY || secondArgType == ArgumentTypes::STMT || secondArgType == ArgumentTypes::PROG_LINE) {
			nextClauseHolds = PKB::programHasNextStmt();
		}
		else if (secondArgType == ArgumentTypes::INT) {
			nextClauseHolds = PKB::stmtHasPrev(std::stoi(secondArg));
		}
		else {
			nextClauseHolds = checkLineTypeIsNext(secondArgType, PKB::getAllNextStmt());
		}
	}
	else if (firstArgType == ArgumentTypes::INT) {
		if (secondArgType == ArgumentTypes::ANY || secondArgType == ArgumentTypes::STMT || secondArgType == ArgumentTypes::PROG_LINE) {
			nextClauseHolds = PKB::stmtHasNext(std::stoi(firstArg));
		}
		else if (secondArgType == ArgumentTypes::INT) {
			int firstInt = std::stoi(firstArg);
			int secondInt = std::stoi(secondArg);

			nextClauseHolds = PKB::getAllNextPair().count(std::make_pair(firstInt, secondInt)) == 1;
		}
		else {
			int firstInt = std::stoi(firstArg);

			if (PKB::stmtHasNext(firstInt)) {
				std::unordered_set<int> nextLineSet = PKB::getNextOfStmt(firstInt);
				nextClauseHolds = checkLineTypeIsNext(secondArgType, nextLineSet);
			}
		}
	}
	else {
		if (secondArgType == ArgumentTypes::ANY || secondArgType == ArgumentTypes::STMT || secondArgType == ArgumentTypes::PROG_LINE) {
			nextClauseHolds = checkLineTypeIsNext(firstArgType, PKB::getAllPreviousStmt());
		}
		else if (secondArgType == ArgumentTypes::INT) {
			int secondInt = std::stoi(secondArg);

			if (PKB::stmtHasPrev(secondInt)) {
				std::unordered_set<int> prevLineSet = PKB::getPrevOfStmt(secondInt);
				nextClauseHolds = checkLineTypeIsNext(firstArgType, prevLineSet);
			}
		}
		else {
			nextClauseHolds = checkLineTypesAreNext(firstArgType, secondArgType);
		}
	}

	return nextClauseHolds;
}

bool NextEvaluator::checkLineTypeIsNext(ArgumentTypes& lineType, std::unordered_set<int>& nextLineSet) {
	bool lineTypeIsNext = false;
	std::unordered_set<int> lineTypeSet = QueryEvaluatorUtil::getStmtTypeSet(lineType);

	if (lineTypeSet.size() < nextLineSet.size()) {
		for (const int& line : lineTypeSet) {
			if (nextLineSet.count(line) == 1) {
				lineTypeIsNext = true;
				break;
			}
		}
	}
	else {
		for (const int& line : nextLineSet) {
			if (lineTypeSet.count(line) == 1) {
				lineTypeIsNext = true;
				break;
			}
		}
	}

	return lineTypeIsNext;
}

bool NextEvaluator::checkLineTypesAreNext(ArgumentTypes& firstLineType, ArgumentTypes& secondLineType) {
	bool lineTypesAreNext = false;

	ArgumentTypes nextLineType;
	std::unordered_set<int> currLineTypeSet;
	std::unordered_set<int> currNextLineSet;
	std::unordered_set<int> (*getNextSetOfLine)(int);

	std::unordered_set<int> firstLineTypeSet = QueryEvaluatorUtil::getStmtTypeSet(firstLineType);
	std::unordered_set<int> secondLineTypeSet = QueryEvaluatorUtil::getStmtTypeSet(secondLineType);

	if (firstLineTypeSet.size() < secondLineTypeSet.size()) {
		nextLineType = secondLineType;
		currLineTypeSet = firstLineTypeSet;
		currNextLineSet = PKB::getAllPreviousStmt();
		getNextSetOfLine = &PKB::getNextOfStmt;
	}
	else {
		nextLineType = firstLineType;
		currLineTypeSet = secondLineTypeSet;
		currNextLineSet = PKB::getAllNextStmt();
		getNextSetOfLine = &PKB::getPrevOfStmt;
	}

	if (currLineTypeSet.size() < currNextLineSet.size()) {
		for (const int& line : currLineTypeSet) {
			if (currNextLineSet.count(line) == 1) {
				std::unordered_set<int> nextLineSet = getNextSetOfLine(line);

				if (checkLineTypeIsNext(nextLineType, nextLineSet)) {
					lineTypesAreNext = true;
					break;
				}
			}
		}
	}
	else {
		for (const int& line : currNextLineSet) {
			if (currLineTypeSet.count(line) == 1) {
				std::unordered_set<int> nextLineSet = getNextSetOfLine(line);

				if (checkLineTypeIsNext(nextLineType, nextLineSet)) {
					lineTypesAreNext = true;
					break;
				}
			}
		}
	}

	return lineTypesAreNext;
}

std::unordered_map<std::string, std::vector<std::string>> NextEvaluator::evaluateNextClause(QuerySuchThatClause& currSuchThatClause) {
	std::string firstArg = currSuchThatClause.getFirstArgument();
	ArgumentTypes firstArgType = currSuchThatClause.getFirstArgumentType();
	std::string secondArg = currSuchThatClause.getSecondArgument();
	ArgumentTypes secondArgType = currSuchThatClause.getSecondArgumentType();
	std::unordered_map<std::string, std::vector<std::string>> nextClauseTable;

	// Next(s, s) is not possible
	if (firstArg == secondArg) {
		return nextClauseTable;
	}

	if (firstArgType == ArgumentTypes::ANY) {
		if (secondArgType == ArgumentTypes::STMT || secondArgType == ArgumentTypes::PROG_LINE) {
			nextClauseTable.emplace(secondArg, QueryEvaluatorUtil::convertIntSetToStringList(PKB::getAllNextStmt()));
		}
		else {
			nextClauseTable.emplace(secondArg, getLineTypeIsNextList(secondArgType, PKB::getAllNextStmt()));
		}
	}
	else if (firstArgType == ArgumentTypes::INT) {
		if (secondArgType == ArgumentTypes::STMT || secondArgType == ArgumentTypes::PROG_LINE) {
			int firstInt = std::stoi(firstArg);

			if (PKB::stmtHasNext(firstInt)) {
				std::vector<std::string> nextLineList = QueryEvaluatorUtil::convertIntSetToStringList(PKB::getNextOfStmt(firstInt));
				nextClauseTable.emplace(secondArg, nextLineList);
			}
		}
		else {
			int firstInt = std::stoi(firstArg);

			if (PKB::stmtHasNext(firstInt)) {
				std::unordered_set<int> nextLineSet = PKB::getNextOfStmt(firstInt);
				nextClauseTable.emplace(secondArg, getLineTypeIsNextList(secondArgType, nextLineSet));
			}
		}
	}
	else if (firstArgType == ArgumentTypes::STMT || firstArgType == ArgumentTypes::PROG_LINE) {
		if (secondArgType == ArgumentTypes::ANY) {
			nextClauseTable.emplace(firstArg, QueryEvaluatorUtil::convertIntSetToStringList(PKB::getAllPreviousStmt()));
		}
		else if (secondArgType == ArgumentTypes::INT) {
			int secondInt = std::stoi(secondArg);

			if (PKB::stmtHasPrev(secondInt)) {
				std::vector<std::string> prevLineList = QueryEvaluatorUtil::convertIntSetToStringList(PKB::getPrevOfStmt(secondInt));
				nextClauseTable.emplace(firstArg, prevLineList);
			}
		}
		else if (secondArgType == ArgumentTypes::STMT || secondArgType == ArgumentTypes::PROG_LINE) {
			std::vector<std::string> prevLineList;
			std::vector<std::string> nextLineList;

			for (const std::pair<int, int>& nextPair : PKB::getAllNextPair()) {
				prevLineList.push_back(std::to_string(nextPair.first));
				nextLineList.push_back(std::to_string(nextPair.second));
			}

			nextClauseTable.emplace(firstArg, prevLineList);
			nextClauseTable.emplace(secondArg, nextLineList);
		}
		else {
			std::vector<std::string> prevLineList;
			std::vector<std::string> nextLineList;

			for (const std::string& nextLine : getLineTypeIsNextList(secondArgType, PKB::getAllNextStmt())) {
				for (int prevLine : PKB::getPrevOfStmt(std::stoi(nextLine))) {
					prevLineList.push_back(std::to_string(prevLine));
					nextLineList.push_back(nextLine);
				}
			}

			nextClauseTable.emplace(firstArg, prevLineList);
			nextClauseTable.emplace(secondArg, nextLineList);
		}
	}
	else {
		if (secondArgType == ArgumentTypes::ANY) {
			nextClauseTable.emplace(firstArg, getLineTypeIsNextList(firstArgType, PKB::getAllPreviousStmt()));
		}
		else if (secondArgType == ArgumentTypes::INT) {
			int secondInt = std::stoi(secondArg);

			if (PKB::stmtHasPrev(secondInt)) {
				std::unordered_set<int> prevLineSet = PKB::getPrevOfStmt(secondInt);
				nextClauseTable.emplace(firstArg, getLineTypeIsNextList(firstArgType, prevLineSet));
			}
		}
		else if (secondArgType == ArgumentTypes::STMT || secondArgType == ArgumentTypes::PROG_LINE) {
			std::vector<std::string> prevLineList;
			std::vector<std::string> nextLineList;

			for (const std::string& prevLine : getLineTypeIsNextList(firstArgType, PKB::getAllPreviousStmt())) {
				for (int nextLine : PKB::getNextOfStmt(std::stoi(prevLine))) {
					prevLineList.push_back(prevLine);
					nextLineList.push_back(std::to_string(nextLine));
				}
			}

			nextClauseTable.emplace(firstArg, prevLineList);
			nextClauseTable.emplace(secondArg, nextLineList);
		}
		else {
			std::pair<std::vector<std::string>, std::vector<std::string>> nextListPair = getLineTypesAreNextList(firstArgType, secondArgType);
			nextClauseTable.emplace(firstArg, nextListPair.first);
			nextClauseTable.emplace(secondArg, nextListPair.second);
		}
	}

	return nextClauseTable;
}

std::vector<std::string> NextEvaluator::getLineTypeIsNextList(ArgumentTypes& lineType, std::unordered_set<int>& nextLineSet) {
	std::vector<std::string> lineTypeIsNextList;
	std::unordered_set<int> lineTypeSet = QueryEvaluatorUtil::getStmtTypeSet(lineType);

	if (lineTypeSet.size() < nextLineSet.size()) {
		for (const int& line : lineTypeSet) {
			if (nextLineSet.count(line) == 1) {
				lineTypeIsNextList.push_back(std::to_string(line));
			}
		}
	}
	else {
		for (const int& line : nextLineSet) {
			if (lineTypeSet.count(line) == 1) {
				lineTypeIsNextList.push_back(std::to_string(line));
			}
		}
	}

	return lineTypeIsNextList;
}

std::pair<std::vector<std::string>, std::vector<std::string>> NextEvaluator::getLineTypesAreNextList(ArgumentTypes& firstLineType, ArgumentTypes& secondLineType) {
	std::vector<std::string> lineTypeIsPrevList;
	std::vector<std::string> lineTypeIsNextList;

	ArgumentTypes nextLineType;
	std::unordered_set<int> currLineTypeSet;
	std::unordered_set<int> currNextLineSet;
	std::unordered_set<int> (*getNextSetOfLine)(int);

	std::unordered_set<int> firstLineTypeSet = QueryEvaluatorUtil::getStmtTypeSet(firstLineType);
	std::unordered_set<int> secondLineTypeSet = QueryEvaluatorUtil::getStmtTypeSet(secondLineType);
	bool isFirstLineType = firstLineTypeSet.size() < secondLineTypeSet.size();

	if (isFirstLineType) {
		nextLineType = secondLineType;
		currLineTypeSet = firstLineTypeSet;
		currNextLineSet = PKB::getAllPreviousStmt();
		getNextSetOfLine = &PKB::getNextOfStmt;
	}
	else {
		nextLineType = firstLineType;
		currLineTypeSet = secondLineTypeSet;
		currNextLineSet = PKB::getAllNextStmt();
		getNextSetOfLine = &PKB::getPrevOfStmt;
	}

	if (currLineTypeSet.size() < currNextLineSet.size()) {
		for (const int& line : currLineTypeSet) {
			if (currNextLineSet.count(line) == 1) {
				std::unordered_set<int> nextLineSet = getNextSetOfLine(line);
				std::vector<std::string> nextLineTypeNextList = getLineTypeIsNextList(nextLineType, nextLineSet);

				for (std::string& nextLine : nextLineTypeNextList) {
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
	else {
		for (const int& line : currNextLineSet) {
			if (currLineTypeSet.count(line) == 1) {
				std::unordered_set<int> nextLineSet = getNextSetOfLine(line);
				std::vector<std::string> nextLineTypeNextList = getLineTypeIsNextList(nextLineType, nextLineSet);

				for (std::string& nextLine : nextLineTypeNextList) {
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

	return std::make_pair(lineTypeIsPrevList, lineTypeIsNextList);
}

int NextEvaluator::estimateNextClauseTableSize(QuerySuchThatClause& currSuchThatClause) {
	std::string firstArg = currSuchThatClause.getFirstArgument();
	ArgumentTypes firstArgType = currSuchThatClause.getFirstArgumentType();
	std::string secondArg = currSuchThatClause.getSecondArgument();
	ArgumentTypes secondArgType = currSuchThatClause.getSecondArgumentType();
	int estimatedTableSize = 0;

	// Next(s, s) is not possible
	if (firstArg == secondArg) {
		return estimatedTableSize;
	}

	if (firstArgType == ArgumentTypes::ANY) {
		if (secondArgType == ArgumentTypes::STMT || secondArgType == ArgumentTypes::PROG_LINE) {
			estimatedTableSize = PKB::getAllNextStmt().size();
		}
		else { // check this
			estimatedTableSize = std::min(QueryEvaluatorUtil::getStmtTypeSet(secondArgType).size(), PKB::getAllNextStmt().size());
		}
	}
	else if (firstArgType == ArgumentTypes::INT) {
		if (secondArgType == ArgumentTypes::STMT || secondArgType == ArgumentTypes::PROG_LINE) {
			int firstInt = std::stoi(firstArg);

			if (PKB::stmtHasNext(firstInt)) {
				estimatedTableSize = PKB::getNextOfStmt(firstInt).size();
			}
		}
		else { // check this
			int firstInt = std::stoi(firstArg);

			if (PKB::stmtHasNext(firstInt)) {
				estimatedTableSize = std::min(QueryEvaluatorUtil::getStmtTypeSet(secondArgType).size(), PKB::getNextOfStmt(firstInt).size());
			}
		}
	}
	else if (firstArgType == ArgumentTypes::STMT || firstArgType == ArgumentTypes::PROG_LINE) {
		if (secondArgType == ArgumentTypes::ANY) {
			estimatedTableSize = PKB::getAllPreviousStmt().size();
		}
		else if (secondArgType == ArgumentTypes::INT) {
			int secondInt = std::stoi(secondArg);

			if (PKB::stmtHasPrev(secondInt)) {
				estimatedTableSize = PKB::getPrevOfStmt(secondInt).size();
			}
		}
		else if (secondArgType == ArgumentTypes::STMT || secondArgType == ArgumentTypes::PROG_LINE) {
			estimatedTableSize = PKB::getAllNextPair().size();
		}
		else {
			for (const std::string& nextLine : getLineTypeIsNextList(secondArgType, PKB::getAllNextStmt())) {
				estimatedTableSize += PKB::getPrevOfStmt(std::stoi(nextLine)).size();
			}
		}
	}
	else { // check this
		if (secondArgType == ArgumentTypes::ANY) {
			estimatedTableSize = std::min(QueryEvaluatorUtil::getStmtTypeSet(firstArgType).size(), PKB::getAllPreviousStmt().size());
		}
		else if (secondArgType == ArgumentTypes::INT) { // check this
			int secondInt = std::stoi(secondArg);

			if (PKB::stmtHasPrev(secondInt)) {
				estimatedTableSize = std::min(QueryEvaluatorUtil::getStmtTypeSet(firstArgType).size(), PKB::getPrevOfStmt(secondInt).size());
			}
		}
		else if (secondArgType == ArgumentTypes::STMT || secondArgType == ArgumentTypes::PROG_LINE) {
			for (const std::string& prevLine : getLineTypeIsNextList(firstArgType, PKB::getAllPreviousStmt())) {
				estimatedTableSize += PKB::getNextOfStmt(std::stoi(prevLine)).size();
			}
		}
		else {
			estimatedTableSize = getStmtTypesAreNextListSize(firstArgType, secondArgType);
		}
	}

	return estimatedTableSize;
}

int NextEvaluator::getStmtTypesAreNextListSize(ArgumentTypes& firstLineType, ArgumentTypes& secondLineType) {
	int listSize = 0;

	ArgumentTypes nextLineType;
	std::unordered_set<int> currLineTypeSet;
	std::unordered_set<int> currNextLineSet;
	std::unordered_set<int>(*getNextSetOfLine)(int);

	std::unordered_set<int> firstLineTypeSet = QueryEvaluatorUtil::getStmtTypeSet(firstLineType);
	std::unordered_set<int> secondLineTypeSet = QueryEvaluatorUtil::getStmtTypeSet(secondLineType);
	bool isFirstLineType = firstLineTypeSet.size() < secondLineTypeSet.size();

	if (isFirstLineType) {
		nextLineType = secondLineType;
		currLineTypeSet = firstLineTypeSet;
		currNextLineSet = PKB::getAllPreviousStmt();
		getNextSetOfLine = &PKB::getNextOfStmt;
	}
	else {
		nextLineType = firstLineType;
		currLineTypeSet = secondLineTypeSet;
		currNextLineSet = PKB::getAllNextStmt();
		getNextSetOfLine = &PKB::getPrevOfStmt;
	}

	if (currLineTypeSet.size() < currNextLineSet.size()) {
		for (const int& line : currLineTypeSet) {
			if (currNextLineSet.count(line) == 1) {
				listSize += std::min(QueryEvaluatorUtil::getStmtTypeSet(nextLineType).size(), getNextSetOfLine(line).size());
			}
		}
	}
	else {
		for (const int& line : currNextLineSet) {
			if (currLineTypeSet.count(line) == 1) {
				listSize += std::min(QueryEvaluatorUtil::getStmtTypeSet(nextLineType).size(), getNextSetOfLine(line).size());
			}
		}
	}

	return listSize;
}