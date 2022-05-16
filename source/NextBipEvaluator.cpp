#include "NextBipEvaluator.h"

bool NextBipEvaluator::checkNextBipClauseHolds(QuerySuchThatClause& currSuchThatClause) {
	std::string firstArg = currSuchThatClause.getFirstArgument();
	ArgumentTypes firstArgType = currSuchThatClause.getFirstArgumentType();
	std::string secondArg = currSuchThatClause.getSecondArgument();
	ArgumentTypes secondArgType = currSuchThatClause.getSecondArgumentType();
	bool nextBipClauseHolds = false;

	// NextBip(s, s) is not possible
	if (firstArg == secondArg && firstArgType != ArgumentTypes::ANY) {
		return nextBipClauseHolds;
	}

	if (firstArgType == ArgumentTypes::ANY || firstArgType == ArgumentTypes::STMT || firstArgType == ArgumentTypes::PROG_LINE) {
		if (secondArgType == ArgumentTypes::ANY || secondArgType == ArgumentTypes::STMT || secondArgType == ArgumentTypes::PROG_LINE) {
			nextBipClauseHolds = PKB::programHasNextBipStmt();
		}
		else if (secondArgType == ArgumentTypes::INT) {
			nextBipClauseHolds = PKB::stmtHasPrevBip(std::stoi(secondArg));
		}
		else {
			nextBipClauseHolds = checkLineTypeIsNextBip(secondArgType, PKB::getAllNextBipStmt());
		}
	}
	else if (firstArgType == ArgumentTypes::INT) {
		if (secondArgType == ArgumentTypes::ANY || secondArgType == ArgumentTypes::STMT || secondArgType == ArgumentTypes::PROG_LINE) {
			nextBipClauseHolds = PKB::stmtHasNextBip(std::stoi(firstArg));
		}
		else if (secondArgType == ArgumentTypes::INT) {
			int firstInt = std::stoi(firstArg);
			int secondInt = std::stoi(secondArg);

			nextBipClauseHolds = PKB::getAllNextBipPair().count(std::make_pair(firstInt, secondInt)) == 1;
		}
		else {
			int firstInt = std::stoi(firstArg);

			if (PKB::stmtHasNextBip(firstInt)) {
				std::unordered_set<int> nextBipLineSet = PKB::getNextBipOfStmt(firstInt);
				nextBipClauseHolds = checkLineTypeIsNextBip(secondArgType, nextBipLineSet);
			}
		}
	}
	else {
		if (secondArgType == ArgumentTypes::ANY || secondArgType == ArgumentTypes::STMT || secondArgType == ArgumentTypes::PROG_LINE) {
			nextBipClauseHolds = checkLineTypeIsNextBip(firstArgType, PKB::getAllPreviousBipStmt());
		}
		else if (secondArgType == ArgumentTypes::INT) {
			int secondInt = std::stoi(secondArg);

			if (PKB::stmtHasPrevBip(secondInt)) {
				std::unordered_set<int> prevBipLineSet = PKB::getPrevBipOfStmt(secondInt);
				nextBipClauseHolds = checkLineTypeIsNextBip(firstArgType, prevBipLineSet);
			}
		}
		else {
			nextBipClauseHolds = checkLineTypesAreNextBip(firstArgType, secondArgType);
		}
	}

	return nextBipClauseHolds;
}

bool NextBipEvaluator::checkLineTypeIsNextBip(ArgumentTypes& lineType, std::unordered_set<int>& nextBipLineSet) {
	bool lineTypeIsNextBip = false;
	std::unordered_set<int> lineTypeSet = QueryEvaluatorUtil::getStmtTypeSet(lineType);

	if (lineTypeSet.size() < nextBipLineSet.size()) {
		for (const int& line : lineTypeSet) {
			if (nextBipLineSet.count(line) == 1) {
				lineTypeIsNextBip = true;
				break;
			}
		}
	}
	else {
		for (const int& line : nextBipLineSet) {
			if (lineTypeSet.count(line) == 1) {
				lineTypeIsNextBip = true;
				break;
			}
		}
	}

	return lineTypeIsNextBip;
}

bool NextBipEvaluator::checkLineTypesAreNextBip(ArgumentTypes& firstLineType, ArgumentTypes& secondLineType) {
	bool lineTypesAreNextBip = false;

	ArgumentTypes nextBipLineType;
	std::unordered_set<int> currLineTypeSet;
	std::unordered_set<int> currNextBipLineSet;
	std::unordered_set<int> (*getNextBipSetOfLine)(int);

	std::unordered_set<int> firstLineTypeSet = QueryEvaluatorUtil::getStmtTypeSet(firstLineType);
	std::unordered_set<int> secondLineTypeSet = QueryEvaluatorUtil::getStmtTypeSet(secondLineType);

	if (firstLineTypeSet.size() < secondLineTypeSet.size()) {
		nextBipLineType = secondLineType;
		currLineTypeSet = firstLineTypeSet;
		currNextBipLineSet = PKB::getAllPreviousBipStmt();
		getNextBipSetOfLine = &PKB::getNextBipOfStmt;
	}
	else {
		nextBipLineType = firstLineType;
		currLineTypeSet = secondLineTypeSet;
		currNextBipLineSet = PKB::getAllNextBipStmt();
		getNextBipSetOfLine = &PKB::getPrevBipOfStmt;
	}

	if (currLineTypeSet.size() < currNextBipLineSet.size()) {
		for (const int& line : currLineTypeSet) {
			if (currNextBipLineSet.count(line) == 1) {
				std::unordered_set<int> nextBipLineSet = getNextBipSetOfLine(line);

				if (checkLineTypeIsNextBip(nextBipLineType, nextBipLineSet)) {
					lineTypesAreNextBip = true;
					break;
				}
			}
		}
	}
	else {
		for (const int& line : currNextBipLineSet) {
			if (currLineTypeSet.count(line) == 1) {
				std::unordered_set<int> nextBipLineSet = getNextBipSetOfLine(line);

				if (checkLineTypeIsNextBip(nextBipLineType, nextBipLineSet)) {
					lineTypesAreNextBip = true;
					break;
				}
			}
		}
	}

	return lineTypesAreNextBip;
}

std::unordered_map<std::string, std::vector<std::string>> NextBipEvaluator::evaluateNextBipClause(QuerySuchThatClause& currSuchThatClause) {
	std::string firstArg = currSuchThatClause.getFirstArgument();
	ArgumentTypes firstArgType = currSuchThatClause.getFirstArgumentType();
	std::string secondArg = currSuchThatClause.getSecondArgument();
	ArgumentTypes secondArgType = currSuchThatClause.getSecondArgumentType();
	std::unordered_map<std::string, std::vector<std::string>> nextBipClauseTable;

	// NextBip(s, s) is not possible
	if (firstArg == secondArg) {
		return nextBipClauseTable;
	}

	if (firstArgType == ArgumentTypes::ANY) {
		if (secondArgType == ArgumentTypes::STMT || secondArgType == ArgumentTypes::PROG_LINE) {
			nextBipClauseTable.emplace(secondArg, QueryEvaluatorUtil::convertIntSetToStringList(PKB::getAllNextBipStmt()));
		}
		else {
			nextBipClauseTable.emplace(secondArg, getLineTypeIsNextBipList(secondArgType, PKB::getAllNextBipStmt()));
		}
	}
	else if (firstArgType == ArgumentTypes::INT) {
		if (secondArgType == ArgumentTypes::STMT || secondArgType == ArgumentTypes::PROG_LINE) {
			int firstInt = std::stoi(firstArg);

			if (PKB::stmtHasNextBip(firstInt)) {
				std::vector<std::string> nextBipLineList = QueryEvaluatorUtil::convertIntSetToStringList(PKB::getNextBipOfStmt(firstInt));
				nextBipClauseTable.emplace(secondArg, nextBipLineList);
			}
		}
		else {
			int firstInt = std::stoi(firstArg);

			if (PKB::stmtHasNextBip(firstInt)) {
				std::unordered_set<int> nextBipLineSet = PKB::getNextBipOfStmt(firstInt);
				nextBipClauseTable.emplace(secondArg, getLineTypeIsNextBipList(secondArgType, nextBipLineSet));
			}
		}
	}
	else if (firstArgType == ArgumentTypes::STMT || firstArgType == ArgumentTypes::PROG_LINE) {
		if (secondArgType == ArgumentTypes::ANY) {
			nextBipClauseTable.emplace(firstArg, QueryEvaluatorUtil::convertIntSetToStringList(PKB::getAllPreviousBipStmt()));
		}
		else if (secondArgType == ArgumentTypes::INT) {
			int secondInt = std::stoi(secondArg);

			if (PKB::stmtHasPrevBip(secondInt)) {
				std::vector<std::string> prevBipLineList = QueryEvaluatorUtil::convertIntSetToStringList(PKB::getPrevBipOfStmt(secondInt));
				nextBipClauseTable.emplace(firstArg, prevBipLineList);
			}
		}
		else if (secondArgType == ArgumentTypes::STMT || secondArgType == ArgumentTypes::PROG_LINE) {
			std::vector<std::string> prevBipLineList;
			std::vector<std::string> nextBipLineList;

			for (const std::pair<int, int>& nextBipPair : PKB::getAllNextBipPair()) {
				prevBipLineList.push_back(std::to_string(nextBipPair.first));
				nextBipLineList.push_back(std::to_string(nextBipPair.second));
			}

			nextBipClauseTable.emplace(firstArg, prevBipLineList);
			nextBipClauseTable.emplace(secondArg, nextBipLineList);
		}
		else {
			std::vector<std::string> prevBipLineList;
			std::vector<std::string> nextBipLineList;

			for (const std::string& nextBipLine : getLineTypeIsNextBipList(secondArgType, PKB::getAllNextBipStmt())) {
				for (int prevBipLine : PKB::getPrevBipOfStmt(std::stoi(nextBipLine))) {
					prevBipLineList.push_back(std::to_string(prevBipLine));
					nextBipLineList.push_back(nextBipLine);
				}
			}

			nextBipClauseTable.emplace(firstArg, prevBipLineList);
			nextBipClauseTable.emplace(secondArg, nextBipLineList);
		}
	}
	else {
		if (secondArgType == ArgumentTypes::ANY) {
			nextBipClauseTable.emplace(firstArg, getLineTypeIsNextBipList(firstArgType, PKB::getAllPreviousBipStmt()));
		}
		else if (secondArgType == ArgumentTypes::INT) {
			int secondInt = std::stoi(secondArg);

			if (PKB::stmtHasPrevBip(secondInt)) {
				std::unordered_set<int> prevBipLineSet = PKB::getPrevBipOfStmt(secondInt);
				nextBipClauseTable.emplace(firstArg, getLineTypeIsNextBipList(firstArgType, prevBipLineSet));
			}
		}
		else if (secondArgType == ArgumentTypes::STMT || secondArgType == ArgumentTypes::PROG_LINE) {
			std::vector<std::string> prevBipLineList;
			std::vector<std::string> nextBipLineList;

			for (const std::string& prevBipLine : getLineTypeIsNextBipList(firstArgType, PKB::getAllPreviousBipStmt())) {
				for (int nextBipLine : PKB::getNextBipOfStmt(std::stoi(prevBipLine))) {
					prevBipLineList.push_back(prevBipLine);
					nextBipLineList.push_back(std::to_string(nextBipLine));
				}
			}

			nextBipClauseTable.emplace(firstArg, prevBipLineList);
			nextBipClauseTable.emplace(secondArg, nextBipLineList);
		}
		else {
			std::pair<std::vector<std::string>, std::vector<std::string>> nextBipListPair = getLineTypesAreNextBipList(firstArgType, secondArgType);
			nextBipClauseTable.emplace(firstArg, nextBipListPair.first);
			nextBipClauseTable.emplace(secondArg, nextBipListPair.second);
		}
	}

	return nextBipClauseTable;
}

std::vector<std::string> NextBipEvaluator::getLineTypeIsNextBipList(ArgumentTypes& lineType, std::unordered_set<int>& nextBipLineSet) {
	std::vector<std::string> lineTypeIsNextBipList;
	std::unordered_set<int> lineTypeSet = QueryEvaluatorUtil::getStmtTypeSet(lineType);

	if (lineTypeSet.size() < nextBipLineSet.size()) {
		for (const int& line : lineTypeSet) {
			if (nextBipLineSet.count(line) == 1) {
				lineTypeIsNextBipList.push_back(std::to_string(line));
			}
		}
	}
	else {
		for (const int& line : nextBipLineSet) {
			if (lineTypeSet.count(line) == 1) {
				lineTypeIsNextBipList.push_back(std::to_string(line));
			}
		}
	}

	return lineTypeIsNextBipList;
}

std::pair<std::vector<std::string>, std::vector<std::string>> NextBipEvaluator::getLineTypesAreNextBipList(ArgumentTypes& firstLineType, ArgumentTypes& secondLineType) {
	std::vector<std::string> lineTypeIsPrevBipList;
	std::vector<std::string> lineTypeIsNextBipList;

	ArgumentTypes nextBipLineType;
	std::unordered_set<int> currLineTypeSet;
	std::unordered_set<int> currNextBipLineSet;
	std::unordered_set<int> (*getNextBipSetOfLine)(int);

	std::unordered_set<int> firstLineTypeSet = QueryEvaluatorUtil::getStmtTypeSet(firstLineType);
	std::unordered_set<int> secondLineTypeSet = QueryEvaluatorUtil::getStmtTypeSet(secondLineType);
	bool isFirstLineType = firstLineTypeSet.size() < secondLineTypeSet.size();

	if (isFirstLineType) {
		nextBipLineType = secondLineType;
		currLineTypeSet = firstLineTypeSet;
		currNextBipLineSet = PKB::getAllPreviousBipStmt();
		getNextBipSetOfLine = &PKB::getNextBipOfStmt;
	}
	else {
		nextBipLineType = firstLineType;
		currLineTypeSet = secondLineTypeSet;
		currNextBipLineSet = PKB::getAllNextBipStmt();
		getNextBipSetOfLine = &PKB::getPrevBipOfStmt;
	}

	if (currLineTypeSet.size() < currNextBipLineSet.size()) {
		for (const int& line : currLineTypeSet) {
			if (currNextBipLineSet.count(line) == 1) {
				std::unordered_set<int> nextBipLineSet = getNextBipSetOfLine(line);
				std::vector<std::string> nextBipLineTypeNextList = getLineTypeIsNextBipList(nextBipLineType, nextBipLineSet);

				for (std::string& nextBipLine : nextBipLineTypeNextList) {
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
	else {
		for (const int& line : currNextBipLineSet) {
			if (currLineTypeSet.count(line) == 1) {
				std::unordered_set<int> nextBipLineSet = getNextBipSetOfLine(line);
				std::vector<std::string> nextBipLineTypeNextList = getLineTypeIsNextBipList(nextBipLineType, nextBipLineSet);

				for (std::string& nextBipLine : nextBipLineTypeNextList) {
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

	return std::make_pair(lineTypeIsPrevBipList, lineTypeIsNextBipList);
}

int NextBipEvaluator::estimateNextBipClauseTableSize(QuerySuchThatClause& currSuchThatClause) {
	std::string firstArg = currSuchThatClause.getFirstArgument();
	ArgumentTypes firstArgType = currSuchThatClause.getFirstArgumentType();
	std::string secondArg = currSuchThatClause.getSecondArgument();
	ArgumentTypes secondArgType = currSuchThatClause.getSecondArgumentType();
	int estimatedTableSize = 0;

	// NextBip(s, s) is not possible
	if (firstArg == secondArg) {
		return estimatedTableSize;
	}

	if (firstArgType == ArgumentTypes::ANY) {
		if (secondArgType == ArgumentTypes::STMT || secondArgType == ArgumentTypes::PROG_LINE) {
			estimatedTableSize = PKB::getAllNextBipStmt().size();
		}
		else { // check this
			estimatedTableSize = std::min(QueryEvaluatorUtil::getStmtTypeSet(secondArgType).size(), PKB::getAllNextBipStmt().size());
		}
	}
	else if (firstArgType == ArgumentTypes::INT) {
		if (secondArgType == ArgumentTypes::STMT || secondArgType == ArgumentTypes::PROG_LINE) {
			int firstInt = std::stoi(firstArg);

			if (PKB::stmtHasNextBip(firstInt)) {
				estimatedTableSize = PKB::getNextBipOfStmt(firstInt).size();
			}
		}
		else { // check this
			int firstInt = std::stoi(firstArg);

			if (PKB::stmtHasNextBip(firstInt)) {
				estimatedTableSize = std::min(QueryEvaluatorUtil::getStmtTypeSet(secondArgType).size(), PKB::getNextBipOfStmt(firstInt).size());
			}
		}
	}
	else if (firstArgType == ArgumentTypes::STMT || firstArgType == ArgumentTypes::PROG_LINE) {
		if (secondArgType == ArgumentTypes::ANY) {
			estimatedTableSize = PKB::getAllPreviousBipStmt().size();
		}
		else if (secondArgType == ArgumentTypes::INT) {
			int secondInt = std::stoi(secondArg);

			if (PKB::stmtHasPrevBip(secondInt)) {
				estimatedTableSize = PKB::getPrevBipOfStmt(secondInt).size();
			}
		}
		else if (secondArgType == ArgumentTypes::STMT || secondArgType == ArgumentTypes::PROG_LINE) {
			estimatedTableSize = PKB::getAllNextBipPair().size();
		}
		else {
			for (const std::string& nextBipLine : getLineTypeIsNextBipList(secondArgType, PKB::getAllNextBipStmt())) {
				estimatedTableSize += PKB::getPrevBipOfStmt(std::stoi(nextBipLine)).size();
			}
		}
	}
	else {
		if (secondArgType == ArgumentTypes::ANY) { // check this
			estimatedTableSize = std::min(QueryEvaluatorUtil::getStmtTypeSet(firstArgType).size(), PKB::getAllPreviousBipStmt().size());
		}
		else if (secondArgType == ArgumentTypes::INT) { // check this
			int secondInt = std::stoi(secondArg);

			if (PKB::stmtHasPrevBip(secondInt)) {
				estimatedTableSize = std::min(QueryEvaluatorUtil::getStmtTypeSet(firstArgType).size(), PKB::getPrevBipOfStmt(secondInt).size());
			}
		}
		else if (secondArgType == ArgumentTypes::STMT || secondArgType == ArgumentTypes::PROG_LINE) {
			for (const std::string& prevBipLine : getLineTypeIsNextBipList(firstArgType, PKB::getAllPreviousBipStmt())) {
				estimatedTableSize += PKB::getNextBipOfStmt(std::stoi(prevBipLine)).size();
			}
		}
		else {
			estimatedTableSize = getStmtTypesAreNextBipListSize(firstArgType, secondArgType);
		}
	}

	return estimatedTableSize;
}

int NextBipEvaluator::getStmtTypesAreNextBipListSize(ArgumentTypes& firstLineType, ArgumentTypes& secondLineType) {
	int listSize = 0;

	ArgumentTypes nextBipLineType;
	std::unordered_set<int> currLineTypeSet;
	std::unordered_set<int> currNextBipLineSet;
	std::unordered_set<int>(*getNextBipSetOfLine)(int);

	std::unordered_set<int> firstLineTypeSet = QueryEvaluatorUtil::getStmtTypeSet(firstLineType);
	std::unordered_set<int> secondLineTypeSet = QueryEvaluatorUtil::getStmtTypeSet(secondLineType);
	bool isFirstLineType = firstLineTypeSet.size() < secondLineTypeSet.size();

	if (isFirstLineType) {
		nextBipLineType = secondLineType;
		currLineTypeSet = firstLineTypeSet;
		currNextBipLineSet = PKB::getAllPreviousBipStmt();
		getNextBipSetOfLine = &PKB::getNextBipOfStmt;
	}
	else {
		nextBipLineType = firstLineType;
		currLineTypeSet = secondLineTypeSet;
		currNextBipLineSet = PKB::getAllNextBipStmt();
		getNextBipSetOfLine = &PKB::getPrevBipOfStmt;
	}

	if (currLineTypeSet.size() < currNextBipLineSet.size()) {
		for (const int& line : currLineTypeSet) {
			if (currNextBipLineSet.count(line) == 1) {
				listSize += std::min(QueryEvaluatorUtil::getStmtTypeSet(nextBipLineType).size(), getNextBipSetOfLine(line).size());
			}
		}
	}
	else {
		for (const int& line : currNextBipLineSet) {
			if (currLineTypeSet.count(line) == 1) {
				listSize += std::min(QueryEvaluatorUtil::getStmtTypeSet(nextBipLineType).size(), getNextBipSetOfLine(line).size());
			}
		}
	}

	return listSize;
}