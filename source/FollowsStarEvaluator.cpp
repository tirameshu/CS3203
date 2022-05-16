#include "FollowsStarEvaluator.h"

bool FollowsStarEvaluator::checkFollowsStarClauseHolds(QuerySuchThatClause& currSuchThatClause) {
	std::string firstArg = currSuchThatClause.getFirstArgument();
	ArgumentTypes firstArgType = currSuchThatClause.getFirstArgumentType();
	std::string secondArg = currSuchThatClause.getSecondArgument();
	ArgumentTypes secondArgType = currSuchThatClause.getSecondArgumentType();
	bool followsStarClauseHolds = false;

	if (firstArg == secondArg && firstArgType != ArgumentTypes::ANY) {
		return followsStarClauseHolds;
	}

	if (firstArgType == ArgumentTypes::ANY || firstArgType == ArgumentTypes::STMT || firstArgType == ArgumentTypes::PROG_LINE) {
		followsStarClauseHolds = FollowsEvaluator::checkFollowsClauseHolds(currSuchThatClause);
	}
	else if (firstArgType == ArgumentTypes::INT) {
		if (secondArgType == ArgumentTypes::ANY || secondArgType == ArgumentTypes::STMT || secondArgType == ArgumentTypes::PROG_LINE) {
			followsStarClauseHolds = FollowsEvaluator::checkFollowsClauseHolds(currSuchThatClause);
		}
		else if (secondArgType == ArgumentTypes::INT) {
			int firstInt = std::stoi(firstArg);
			int secondInt = std::stoi(secondArg);

			if (firstInt < secondInt) {
				followsStarClauseHolds = PKB::getAllFollowsStarPair().count(std::make_pair(firstInt, secondInt)) == 1;
			}
		}
		else {
			int firstInt = std::stoi(firstArg);

			if (PKB::getAllFollowedStmt().count(firstInt) == 1) {
				std::unordered_set<int> followerStmtSet = PKB::getFollowerListOfStmt(firstInt);
				followsStarClauseHolds = checkStmtTypeIsFollowsStar(secondArgType, followerStmtSet);
			}
		}
	}
	else {
		if (secondArgType == ArgumentTypes::ANY || secondArgType == ArgumentTypes::STMT || secondArgType == ArgumentTypes::PROG_LINE) {
			followsStarClauseHolds = checkStmtTypeIsFollowsStar(firstArgType, PKB::getAllFollowedStmt());
		}
		else if (secondArgType == ArgumentTypes::INT) {
			int secondInt = std::stoi(secondArg);

			if (PKB::getAllFollowerStmt().count(secondInt) == 1) {
				std::unordered_set<int> followedStmtSet = PKB::getFollowedListOfStmt(secondInt);
				followsStarClauseHolds = checkStmtTypeIsFollowsStar(firstArgType, followedStmtSet);
			}
		}
		else {
			followsStarClauseHolds = checkStmtTypesAreFollowsStar(firstArgType, secondArgType);
		}
	}

	return followsStarClauseHolds;
}

bool FollowsStarEvaluator::checkStmtTypeIsFollowsStar(ArgumentTypes& stmtType, std::unordered_set<int>& followsStarStmtSet) {
	return FollowsEvaluator::checkStmtTypeIsFollows(stmtType, followsStarStmtSet);
}

bool FollowsStarEvaluator::checkStmtTypesAreFollowsStar(ArgumentTypes& firstStmtType, ArgumentTypes& secondStmtType) {
	bool stmtTypesAreFollowsStar = false;

	ArgumentTypes nextStmtType;
	std::unordered_set<int> currStmtTypeSet;
	std::unordered_set<int> currFollowsStmtSet;
	std::unordered_set<int> (*getFollowsSetOfStmt)(int);

	std::unordered_set<int> firstStmtTypeSet = QueryEvaluatorUtil::getStmtTypeSet(firstStmtType);
	std::unordered_set<int> secondStmtTypeSet = QueryEvaluatorUtil::getStmtTypeSet(secondStmtType);

	if (firstStmtTypeSet.size() < secondStmtTypeSet.size()) {
		nextStmtType = secondStmtType;
		currStmtTypeSet = firstStmtTypeSet;
		currFollowsStmtSet = PKB::getAllFollowedStmt();
		getFollowsSetOfStmt = &PKB::getFollowerListOfStmt;
	}
	else {
		nextStmtType = firstStmtType;
		currStmtTypeSet = secondStmtTypeSet;
		currFollowsStmtSet = PKB::getAllFollowerStmt();
		getFollowsSetOfStmt = &PKB::getFollowedListOfStmt;
	}

	if (currStmtTypeSet.size() < currFollowsStmtSet.size()) {
		for (const int& stmt : currStmtTypeSet) {
			if (currFollowsStmtSet.count(stmt) == 1) {
				std::unordered_set<int> nextFollowsStmtSet = getFollowsSetOfStmt(stmt);

				if (checkStmtTypeIsFollowsStar(nextStmtType, nextFollowsStmtSet)) {
					stmtTypesAreFollowsStar = true;
					break;
				}
			}
		}
	}
	else {
		for (const int& stmt : currFollowsStmtSet) {
			if (currStmtTypeSet.count(stmt) == 1) {
				std::unordered_set<int> nextFollowsStmtSet = getFollowsSetOfStmt(stmt);

				if (checkStmtTypeIsFollowsStar(nextStmtType, nextFollowsStmtSet)) {
					stmtTypesAreFollowsStar = true;
					break;
				}
			}
		}
	}

	return stmtTypesAreFollowsStar;
}

std::unordered_map<std::string, std::vector<std::string>> FollowsStarEvaluator::evaluateFollowsStarClause(QuerySuchThatClause& currSuchThatClause) {
	std::string firstArg = currSuchThatClause.getFirstArgument();
	ArgumentTypes firstArgType = currSuchThatClause.getFirstArgumentType();
	std::string secondArg = currSuchThatClause.getSecondArgument();
	ArgumentTypes secondArgType = currSuchThatClause.getSecondArgumentType();
	std::unordered_map<std::string, std::vector<std::string>> followsStarClauseTable;

	if (firstArg == secondArg) {
		return followsStarClauseTable;
	}

	if (firstArgType == ArgumentTypes::ANY) {
		followsStarClauseTable = FollowsEvaluator::evaluateFollowsClause(currSuchThatClause);
	}
	else if (firstArgType == ArgumentTypes::INT) {
		if (secondArgType == ArgumentTypes::STMT || secondArgType == ArgumentTypes::PROG_LINE) {
			int firstInt = std::stoi(firstArg);

			if (PKB::getAllFollowedStmt().count(firstInt) == 1) {
				std::vector<std::string> followerStmtList = QueryEvaluatorUtil::convertIntSetToStringList(PKB::getFollowerListOfStmt(firstInt));
				followsStarClauseTable.emplace(secondArg, followerStmtList);
			}
		}
		else {
			int firstInt = std::stoi(firstArg);

			if (PKB::getAllFollowedStmt().count(firstInt) == 1) {
				std::unordered_set<int> followerStmtSet = PKB::getFollowerListOfStmt(firstInt);
				followsStarClauseTable.emplace(secondArg, getStmtTypeIsFollowsStarList(secondArgType, followerStmtSet));
			}
		}
	}
	else if (firstArgType == ArgumentTypes::STMT || firstArgType == ArgumentTypes::PROG_LINE) {
		if (secondArgType == ArgumentTypes::ANY) {
			followsStarClauseTable = FollowsEvaluator::evaluateFollowsClause(currSuchThatClause);
		}
		else if (secondArgType == ArgumentTypes::INT) {
			int secondInt = std::stoi(secondArg);

			if (PKB::getAllFollowerStmt().count(secondInt) == 1) {
				std::vector<std::string> followedStmtList = QueryEvaluatorUtil::convertIntSetToStringList(PKB::getFollowedListOfStmt(secondInt));
				followsStarClauseTable.emplace(firstArg, followedStmtList);
			}
		}
		else if (secondArgType == ArgumentTypes::STMT || secondArgType == ArgumentTypes::PROG_LINE) {
			std::vector<std::string> followedStmtList;
			std::vector<std::string> followerStmtList;

			for (const std::pair<int, int>& followsStarPair : PKB::getAllFollowsStarPair()) {
				followedStmtList.push_back(std::to_string(followsStarPair.first));
				followerStmtList.push_back(std::to_string(followsStarPair.second));
			}

			followsStarClauseTable.emplace(firstArg, followedStmtList);
			followsStarClauseTable.emplace(secondArg, followerStmtList);
		}
		else {
			std::vector<std::string> followedStmtList;
			std::vector<std::string> followerStmtList;

			for (const std::string& stmt : getStmtTypeIsFollowsStarList(secondArgType, PKB::getAllFollowerStmt())) {
				for (const int& followed : PKB::getFollowedListOfStmt(std::stoi(stmt))) {
					followedStmtList.push_back(std::to_string(followed));
					followerStmtList.push_back(stmt);
				}
			}

			followsStarClauseTable.emplace(firstArg, followedStmtList);
			followsStarClauseTable.emplace(secondArg, followerStmtList);
		}
	}
	else {
		if (secondArgType == ArgumentTypes::ANY) {
			followsStarClauseTable = FollowsEvaluator::evaluateFollowsClause(currSuchThatClause);
		}
		else if (secondArgType == ArgumentTypes::INT) {
			int secondInt = std::stoi(secondArg);

			if (PKB::getAllFollowerStmt().count(secondInt) == 1) {
				std::unordered_set<int> followedStmtSet = PKB::getFollowedListOfStmt(secondInt);
				followsStarClauseTable.emplace(firstArg, getStmtTypeIsFollowsStarList(firstArgType, followedStmtSet));
			}
		}
		else if (secondArgType == ArgumentTypes::STMT || secondArgType == ArgumentTypes::PROG_LINE) {
			std::vector<std::string> followedStmtList;
			std::vector<std::string> followerStmtList;

			for (const std::string& stmt : getStmtTypeIsFollowsStarList(firstArgType, PKB::getAllFollowedStmt())) {
				for (const int& follower : PKB::getFollowerListOfStmt(std::stoi(stmt))) {
					followedStmtList.push_back(stmt);
					followerStmtList.push_back(std::to_string(follower));
				}
			}

			followsStarClauseTable.emplace(firstArg, followedStmtList);
			followsStarClauseTable.emplace(secondArg, followerStmtList);
		}
		else {
			std::pair<std::vector<std::string>, std::vector<std::string>> followsStarListPair = getStmtTypesAreFollowsStarList(firstArgType, secondArgType);
			followsStarClauseTable.emplace(firstArg, followsStarListPair.first);
			followsStarClauseTable.emplace(secondArg, followsStarListPair.second);
		}
	}

	return followsStarClauseTable;
}

std::vector<std::string> FollowsStarEvaluator::getStmtTypeIsFollowsStarList(ArgumentTypes& stmtType, std::unordered_set<int>& followsStarStmtSet) {
	return FollowsEvaluator::getStmtTypeIsFollowsList(stmtType, followsStarStmtSet);
}

std::pair<std::vector<std::string>, std::vector<std::string>> FollowsStarEvaluator::getStmtTypesAreFollowsStarList(ArgumentTypes& firstStmtType, ArgumentTypes& secondStmtType) {
	std::vector<std::string> stmtTypeIsFollowedList;
	std::vector<std::string> stmtTypeIsFollowerList;

	ArgumentTypes nextStmtType;
	std::unordered_set<int> currStmtTypeSet;
	std::unordered_set<int> currFollowsStmtSet;
	std::unordered_set<int> (*getFollowsSetOfStmt)(int);

	std::unordered_set<int> firstStmtTypeSet = QueryEvaluatorUtil::getStmtTypeSet(firstStmtType);
	std::unordered_set<int> secondStmtTypeSet = QueryEvaluatorUtil::getStmtTypeSet(secondStmtType);
	bool isFirstStmtType = firstStmtTypeSet.size() < secondStmtTypeSet.size();

	if (isFirstStmtType) {
		nextStmtType = secondStmtType;
		currStmtTypeSet = firstStmtTypeSet;
		currFollowsStmtSet = PKB::getAllFollowedStmt();
		getFollowsSetOfStmt = &PKB::getFollowerListOfStmt;
	}
	else {
		nextStmtType = firstStmtType;
		currStmtTypeSet = secondStmtTypeSet;
		currFollowsStmtSet = PKB::getAllFollowerStmt();
		getFollowsSetOfStmt = &PKB::getFollowedListOfStmt;
	}

	if (currStmtTypeSet.size() < currFollowsStmtSet.size()) {
		for (const int& stmt : currStmtTypeSet) {
			if (currFollowsStmtSet.count(stmt) == 1) {
				std::unordered_set<int> nextFollowsStmtSet = getFollowsSetOfStmt(stmt);
				std::vector<std::string> nextStmtTypeFollowsList = getStmtTypeIsFollowsStarList(nextStmtType, nextFollowsStmtSet);
				std::string stmtString = std::to_string(stmt);

				for (const std::string& follows : nextStmtTypeFollowsList) {
					if (isFirstStmtType) {
						stmtTypeIsFollowedList.push_back(stmtString);
						stmtTypeIsFollowerList.push_back(follows);
					}
					else {
						stmtTypeIsFollowedList.push_back(follows);
						stmtTypeIsFollowerList.push_back(stmtString);
					}
				}
			}
		}
	}
	else {
		for (const int& stmt : currFollowsStmtSet) {
			if (currStmtTypeSet.count(stmt) == 1) {
				std::unordered_set<int> nextFollowsStmtSet = getFollowsSetOfStmt(stmt);
				std::vector<std::string> nextStmtTypeFollowsList = getStmtTypeIsFollowsStarList(nextStmtType, nextFollowsStmtSet);
				std::string stmtString = std::to_string(stmt);

				for (const std::string& follows : nextStmtTypeFollowsList) {
					if (isFirstStmtType) {
						stmtTypeIsFollowedList.push_back(stmtString);
						stmtTypeIsFollowerList.push_back(follows);
					}
					else {
						stmtTypeIsFollowedList.push_back(follows);
						stmtTypeIsFollowerList.push_back(stmtString);
					}
				}
			}
		}
	}

	return std::make_pair(stmtTypeIsFollowedList, stmtTypeIsFollowerList);
}

int FollowsStarEvaluator::estimateFollowsStarClauseTableSize(QuerySuchThatClause& currSuchThatClause) {
	std::string firstArg = currSuchThatClause.getFirstArgument();
	ArgumentTypes firstArgType = currSuchThatClause.getFirstArgumentType();
	std::string secondArg = currSuchThatClause.getSecondArgument();
	ArgumentTypes secondArgType = currSuchThatClause.getSecondArgumentType();
	int estimatedTableSize = 0;

	if (firstArg == secondArg) {
		return estimatedTableSize;
	}

	if (firstArgType == ArgumentTypes::ANY) {
		estimatedTableSize = FollowsEvaluator::estimateFollowsClauseTableSize(currSuchThatClause);
	}
	else if (firstArgType == ArgumentTypes::INT) {
		if (secondArgType == ArgumentTypes::STMT || secondArgType == ArgumentTypes::PROG_LINE) {
			int firstInt = std::stoi(firstArg);

			if (PKB::getAllFollowedStmt().count(firstInt) == 1) {
				estimatedTableSize = PKB::getFollowerListOfStmt(firstInt).size();
			}
		}
		else {
			int firstInt = std::stoi(firstArg);

			if (PKB::getAllFollowedStmt().count(firstInt) == 1) { // check this
				estimatedTableSize = std::min(QueryEvaluatorUtil::getStmtTypeSet(secondArgType).size(), PKB::getFollowerListOfStmt(firstInt).size());
			}
		}
	}
	else if (firstArgType == ArgumentTypes::STMT || firstArgType == ArgumentTypes::PROG_LINE) {
		if (secondArgType == ArgumentTypes::ANY) {
			estimatedTableSize = FollowsEvaluator::estimateFollowsClauseTableSize(currSuchThatClause);
		}
		else if (secondArgType == ArgumentTypes::INT) {
			int secondInt = std::stoi(secondArg);

			if (PKB::getAllFollowerStmt().count(secondInt) == 1) {
				estimatedTableSize = PKB::getFollowedListOfStmt(secondInt).size();
			}
		}
		else if (secondArgType == ArgumentTypes::STMT || secondArgType == ArgumentTypes::PROG_LINE) {
			estimatedTableSize = PKB::getAllFollowsStarPair().size();
		}
		else {
			for (const std::string& stmt : getStmtTypeIsFollowsStarList(secondArgType, PKB::getAllFollowerStmt())) {
				estimatedTableSize += PKB::getFollowedListOfStmt(std::stoi(stmt)).size();
			}
		}
	}
	else {
		if (secondArgType == ArgumentTypes::ANY) {
			estimatedTableSize = FollowsEvaluator::estimateFollowsClauseTableSize(currSuchThatClause);
		}
		else if (secondArgType == ArgumentTypes::INT) {
			int secondInt = std::stoi(secondArg);

			if (PKB::getAllFollowerStmt().count(secondInt) == 1) { // check this
				estimatedTableSize = std::min(QueryEvaluatorUtil::getStmtTypeSet(firstArgType).size(), PKB::getFollowedListOfStmt(secondInt).size());
			}
		}
		else if (secondArgType == ArgumentTypes::STMT || secondArgType == ArgumentTypes::PROG_LINE) {
			for (const std::string& stmt : getStmtTypeIsFollowsStarList(firstArgType, PKB::getAllFollowedStmt())) {
				estimatedTableSize += PKB::getFollowerListOfStmt(std::stoi(stmt)).size();
			}
		}
		else {
			estimatedTableSize = getStmtTypesAreFollowsStarListSize(firstArgType, secondArgType);
		}
	}

	return estimatedTableSize;
}

int FollowsStarEvaluator::getStmtTypesAreFollowsStarListSize(ArgumentTypes& firstStmtType, ArgumentTypes& secondStmtType) {
	int listSize = 0;

	ArgumentTypes nextStmtType;
	std::unordered_set<int> currStmtTypeSet;
	std::unordered_set<int> currFollowsStmtSet;
	std::unordered_set<int> (*getFollowsSetOfStmt)(int);

	std::unordered_set<int> firstStmtTypeSet = QueryEvaluatorUtil::getStmtTypeSet(firstStmtType);
	std::unordered_set<int> secondStmtTypeSet = QueryEvaluatorUtil::getStmtTypeSet(secondStmtType);
	bool isFirstStmtType = firstStmtTypeSet.size() < secondStmtTypeSet.size();

	if (isFirstStmtType) {
		nextStmtType = secondStmtType;
		currStmtTypeSet = firstStmtTypeSet;
		currFollowsStmtSet = PKB::getAllFollowedStmt();
		getFollowsSetOfStmt = &PKB::getFollowerListOfStmt;
	}
	else {
		nextStmtType = firstStmtType;
		currStmtTypeSet = secondStmtTypeSet;
		currFollowsStmtSet = PKB::getAllFollowerStmt();
		getFollowsSetOfStmt = &PKB::getFollowedListOfStmt;
	}

	if (currStmtTypeSet.size() < currFollowsStmtSet.size()) {
		for (const int& stmt : currStmtTypeSet) {
			if (currFollowsStmtSet.count(stmt) == 1) {
				listSize += std::min(QueryEvaluatorUtil::getStmtTypeSet(nextStmtType).size(), getFollowsSetOfStmt(stmt).size());
			}
		}
	}
	else {
		for (const int& stmt : currFollowsStmtSet) {
			if (currStmtTypeSet.count(stmt) == 1) {
				listSize += std::min(QueryEvaluatorUtil::getStmtTypeSet(nextStmtType).size(), getFollowsSetOfStmt(stmt).size());
			}
		}
	}

	return listSize;
}