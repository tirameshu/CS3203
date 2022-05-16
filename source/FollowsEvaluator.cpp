#include "FollowsEvaluator.h"

bool FollowsEvaluator::checkFollowsClauseHolds(QuerySuchThatClause& currSuchThatClause) {
	std::string firstArg = currSuchThatClause.getFirstArgument();
	ArgumentTypes firstArgType = currSuchThatClause.getFirstArgumentType();
	std::string secondArg = currSuchThatClause.getSecondArgument();
	ArgumentTypes secondArgType = currSuchThatClause.getSecondArgumentType();
	bool followsClauseHolds = false;

	if (firstArg == secondArg && firstArgType != ArgumentTypes::ANY) {
		return followsClauseHolds;
	}

	if (firstArgType == ArgumentTypes::ANY || firstArgType == ArgumentTypes::STMT || firstArgType == ArgumentTypes::PROG_LINE) {
		if (secondArgType == ArgumentTypes::ANY || secondArgType == ArgumentTypes::STMT || secondArgType == ArgumentTypes::PROG_LINE) {
			followsClauseHolds = PKB::programHasFollowerStmt();
		}
		else if (secondArgType == ArgumentTypes::INT) {
			followsClauseHolds = PKB::getAllFollowerStmt().count(std::stoi(secondArg)) == 1;
		}
		else {
			followsClauseHolds = checkStmtTypeIsFollows(secondArgType, PKB::getAllFollowerStmt());
		}
	}
	else if (firstArgType == ArgumentTypes::INT) {
		if (secondArgType == ArgumentTypes::ANY || secondArgType == ArgumentTypes::STMT || secondArgType == ArgumentTypes::PROG_LINE) {
			followsClauseHolds = PKB::getAllFollowedStmt().count(std::stoi(firstArg)) == 1;
		}
		else if (secondArgType == ArgumentTypes::INT) {
			int firstInt = std::stoi(firstArg);
			int secondInt = std::stoi(secondArg);

			if (firstInt < secondInt) {
				followsClauseHolds = PKB::getAllFollowsPair().count(std::make_pair(firstInt, secondInt)) == 1;
			}
		}
		else {
			int firstInt = std::stoi(firstArg);

			if (PKB::getAllFollowedStmt().count(firstInt) == 1) {
				std::unordered_set<int> followerStmtSet;
				followerStmtSet.emplace(PKB::getFollowerOfStmt(firstInt));
				followsClauseHolds = checkStmtTypeIsFollows(secondArgType, followerStmtSet);
			}
		}
	}
	else {
		if (secondArgType == ArgumentTypes::ANY || secondArgType == ArgumentTypes::STMT || secondArgType == ArgumentTypes::PROG_LINE) {
			followsClauseHolds = checkStmtTypeIsFollows(firstArgType, PKB::getAllFollowedStmt());
		}
		else if (secondArgType == ArgumentTypes::INT) {
			int secondInt = std::stoi(secondArg);

			if (PKB::getAllFollowerStmt().count(secondInt) == 1) {
				std::unordered_set<int> followedStmtSet;
				followedStmtSet.emplace(PKB::getFollowedOfStmt(secondInt));
				followsClauseHolds = checkStmtTypeIsFollows(firstArgType, followedStmtSet);
			}
		}
		else {
			followsClauseHolds = checkStmtTypesAreFollows(firstArgType, secondArgType);
		}
	}

	return followsClauseHolds;
}

bool FollowsEvaluator::checkStmtTypeIsFollows(ArgumentTypes& stmtType, std::unordered_set<int>& followsStmtSet) {
	bool stmtTypeIsFollows = false;
	std::unordered_set<int> stmtTypeSet = QueryEvaluatorUtil::getStmtTypeSet(stmtType);

	if (stmtTypeSet.size() < followsStmtSet.size()) {
		for (const int& stmt : stmtTypeSet) {
			if (followsStmtSet.count(stmt) == 1) {
				stmtTypeIsFollows = true;
				break;
			}
		}
	}
	else {
		for (const int& stmt : followsStmtSet) {
			if (stmtTypeSet.count(stmt) == 1) {
				stmtTypeIsFollows = true;
				break;
			}
		}
	}

	return stmtTypeIsFollows;
}

bool FollowsEvaluator::checkStmtTypesAreFollows(ArgumentTypes& firstStmtType, ArgumentTypes& secondStmtType) {
	bool stmtTypesAreFollows = false;

	ArgumentTypes nextStmtType;
	std::unordered_set<int> currStmtTypeSet;
	std::unordered_set<int> currFollowsStmtSet;
	int (*getFollowsOfStmt)(int);

	std::unordered_set<int> firstStmtTypeSet = QueryEvaluatorUtil::getStmtTypeSet(firstStmtType);
	std::unordered_set<int> secondStmtTypeSet = QueryEvaluatorUtil::getStmtTypeSet(secondStmtType);

	if (firstStmtTypeSet.size() < secondStmtTypeSet.size()) {
		nextStmtType = secondStmtType;
		currStmtTypeSet = firstStmtTypeSet;
		currFollowsStmtSet = PKB::getAllFollowedStmt();
		getFollowsOfStmt = &PKB::getFollowerOfStmt;
	}
	else {
		nextStmtType = firstStmtType;
		currStmtTypeSet = secondStmtTypeSet;
		currFollowsStmtSet = PKB::getAllFollowerStmt();
		getFollowsOfStmt = &PKB::getFollowedOfStmt;
	}

	if (currStmtTypeSet.size() < currFollowsStmtSet.size()) {
		for (const int& stmt : currStmtTypeSet) {
			if (currFollowsStmtSet.count(stmt) == 1) {
				std::unordered_set<int> nextFollowsStmtSet;
				nextFollowsStmtSet.emplace(getFollowsOfStmt(stmt));

				if (checkStmtTypeIsFollows(nextStmtType, nextFollowsStmtSet)) {
					stmtTypesAreFollows = true;
					break;
				}
			}
		}
	}
	else {
		for (const int& stmt : currFollowsStmtSet) {
			if (currStmtTypeSet.count(stmt) == 1) {
				std::unordered_set<int> nextFollowsStmtSet;
				nextFollowsStmtSet.emplace(getFollowsOfStmt(stmt));

				if (checkStmtTypeIsFollows(nextStmtType, nextFollowsStmtSet)) {
					stmtTypesAreFollows = true;
					break;
				}
			}
		}
	}

	return stmtTypesAreFollows;
}

std::unordered_map<std::string, std::vector<std::string>> FollowsEvaluator::evaluateFollowsClause(QuerySuchThatClause& currSuchThatClause) {
	std::string firstArg = currSuchThatClause.getFirstArgument();
	ArgumentTypes firstArgType = currSuchThatClause.getFirstArgumentType();
	std::string secondArg = currSuchThatClause.getSecondArgument();
	ArgumentTypes secondArgType = currSuchThatClause.getSecondArgumentType();
	std::unordered_map<std::string, std::vector<std::string>> followsClauseTable;

	if (firstArg == secondArg) {
		return followsClauseTable;
	}

	if (firstArgType == ArgumentTypes::ANY) {
		if (secondArgType == ArgumentTypes::STMT || secondArgType == ArgumentTypes::PROG_LINE) {
			followsClauseTable.emplace(secondArg, QueryEvaluatorUtil::convertIntSetToStringList(PKB::getAllFollowerStmt()));
		}
		else {
			followsClauseTable.emplace(secondArg, getStmtTypeIsFollowsList(secondArgType, PKB::getAllFollowerStmt()));
		}
	}
	else if (firstArgType == ArgumentTypes::INT) {
		if (secondArgType == ArgumentTypes::STMT || secondArgType == ArgumentTypes::PROG_LINE) {
			int firstInt = std::stoi(firstArg);

			if (PKB::getAllFollowedStmt().count(firstInt) == 1) {
				std::vector<std::string> followerStmtList;
				followerStmtList.push_back(std::to_string(PKB::getFollowerOfStmt(firstInt)));
				followsClauseTable.emplace(secondArg, followerStmtList);
			}
		}
		else {
			int firstInt = std::stoi(firstArg);

			if (PKB::getAllFollowedStmt().count(firstInt) == 1) {
				std::unordered_set<int> followerStmtSet;
				followerStmtSet.emplace(PKB::getFollowerOfStmt(firstInt));
				followsClauseTable.emplace(secondArg, getStmtTypeIsFollowsList(secondArgType, followerStmtSet));
			}
		}
	}
	else if (firstArgType == ArgumentTypes::STMT || firstArgType == ArgumentTypes::PROG_LINE) {
		if (secondArgType == ArgumentTypes::ANY) {
			followsClauseTable.emplace(firstArg, QueryEvaluatorUtil::convertIntSetToStringList(PKB::getAllFollowedStmt()));
		}
		else if (secondArgType == ArgumentTypes::INT) {
			int secondInt = std::stoi(secondArg);

			if (PKB::getAllFollowerStmt().count(secondInt) == 1) {
				std::vector<std::string> followedStmtList;
				followedStmtList.push_back(std::to_string(PKB::getFollowedOfStmt(secondInt)));
				followsClauseTable.emplace(firstArg, followedStmtList);
			}
		}
		else if (secondArgType == ArgumentTypes::STMT || secondArgType == ArgumentTypes::PROG_LINE) {
			std::vector<std::string> followedStmtList;
			std::vector<std::string> followerStmtList;

			for (const std::pair<int, int>& followsPair : PKB::getAllFollowsPair()) {
				followedStmtList.push_back(std::to_string(followsPair.first));
				followerStmtList.push_back(std::to_string(followsPair.second));
			}

			followsClauseTable.emplace(firstArg, followedStmtList);
			followsClauseTable.emplace(secondArg, followerStmtList);
		}
		else {
			std::vector<std::string> followedStmtList;
			std::vector<std::string> followerStmtList = getStmtTypeIsFollowsList(secondArgType, PKB::getAllFollowerStmt());
			
			for (const std::string& stmt : followerStmtList) {
				followedStmtList.push_back(std::to_string(PKB::getFollowedOfStmt(std::stoi(stmt))));
			}

			followsClauseTable.emplace(firstArg, followedStmtList);
			followsClauseTable.emplace(secondArg, followerStmtList);
		}
	}
	else {
		if (secondArgType == ArgumentTypes::ANY) {
			followsClauseTable.emplace(firstArg, getStmtTypeIsFollowsList(firstArgType, PKB::getAllFollowedStmt()));
		}
		else if (secondArgType == ArgumentTypes::INT) {
			int secondInt = std::stoi(secondArg);

			if (PKB::getAllFollowerStmt().count(secondInt) == 1) {
				std::unordered_set<int> followedStmtSet;
				followedStmtSet.emplace(PKB::getFollowedOfStmt(secondInt));
				followsClauseTable.emplace(firstArg, getStmtTypeIsFollowsList(firstArgType, followedStmtSet));
			}
		}
		else if (secondArgType == ArgumentTypes::STMT || secondArgType == ArgumentTypes::PROG_LINE) {
			std::vector<std::string> followedStmtList = getStmtTypeIsFollowsList(firstArgType, PKB::getAllFollowedStmt());
			std::vector<std::string> followerStmtList;

			for (const std::string& stmt : followedStmtList) {
				followerStmtList.push_back(std::to_string(PKB::getFollowerOfStmt(std::stoi(stmt))));
			}

			followsClauseTable.emplace(firstArg, followedStmtList);
			followsClauseTable.emplace(secondArg, followerStmtList);
		}
		else {
			std::pair<std::vector<std::string>, std::vector<std::string>> followsListPair = getStmtTypesAreFollowsList(firstArgType, secondArgType);
			followsClauseTable.emplace(firstArg, followsListPair.first);
			followsClauseTable.emplace(secondArg, followsListPair.second);
		}
	}

	return followsClauseTable;
}

std::vector<std::string> FollowsEvaluator::getStmtTypeIsFollowsList(ArgumentTypes& stmtType, std::unordered_set<int>& followsStmtSet) {
	std::vector<std::string> stmtTypeIsFollowsList;
	std::unordered_set<int> stmtTypeSet = QueryEvaluatorUtil::getStmtTypeSet(stmtType);

	if (stmtTypeSet.size() < followsStmtSet.size()) {
		for (const int& stmt : stmtTypeSet) {
			if (followsStmtSet.count(stmt) == 1) {
				stmtTypeIsFollowsList.push_back(std::to_string(stmt));
			}
		}
	}
	else {
		for (const int& stmt : followsStmtSet) {
			if (stmtTypeSet.count(stmt) == 1) {
				stmtTypeIsFollowsList.push_back(std::to_string(stmt));
			}
		}
	}

	return stmtTypeIsFollowsList;
}

std::pair<std::vector<std::string>, std::vector<std::string>> FollowsEvaluator::getStmtTypesAreFollowsList(ArgumentTypes& firstStmtType, ArgumentTypes& secondStmtType) {
	std::vector<std::string> stmtTypeIsFollowedList;
	std::vector<std::string> stmtTypeIsFollowerList;
	
	ArgumentTypes nextStmtType;
	std::unordered_set<int> currStmtTypeSet;
	std::unordered_set<int> currFollowsStmtSet;
	int (*getFollowsOfStmt)(int);

	std::unordered_set<int> firstStmtTypeSet = QueryEvaluatorUtil::getStmtTypeSet(firstStmtType);
	std::unordered_set<int> secondStmtTypeSet = QueryEvaluatorUtil::getStmtTypeSet(secondStmtType);
	bool isFirstStmtType = firstStmtTypeSet.size() < secondStmtTypeSet.size();

	if (isFirstStmtType) {
		nextStmtType = secondStmtType;
		currStmtTypeSet = firstStmtTypeSet;
		currFollowsStmtSet = PKB::getAllFollowedStmt();
		getFollowsOfStmt = &PKB::getFollowerOfStmt;
	}
	else {
		nextStmtType = firstStmtType;
		currStmtTypeSet = secondStmtTypeSet;
		currFollowsStmtSet = PKB::getAllFollowerStmt();
		getFollowsOfStmt = &PKB::getFollowedOfStmt;
	}

	if (currStmtTypeSet.size() < currFollowsStmtSet.size()) {
		for (const int& stmt : currStmtTypeSet) {
			if (currFollowsStmtSet.count(stmt) == 1) {
				std::unordered_set<int> nextFollowsStmtSet;
				nextFollowsStmtSet.emplace(getFollowsOfStmt(stmt));

				std::vector<std::string> nextStmtTypeFollowsList = getStmtTypeIsFollowsList(nextStmtType, nextFollowsStmtSet);

				if (!nextStmtTypeFollowsList.empty()) {
					if (isFirstStmtType) {
						stmtTypeIsFollowedList.push_back(std::to_string(stmt));
						stmtTypeIsFollowerList.push_back(nextStmtTypeFollowsList.front());
					}
					else {
						stmtTypeIsFollowedList.push_back(nextStmtTypeFollowsList.front());
						stmtTypeIsFollowerList.push_back(std::to_string(stmt));
					}
				}
			}
		}
	}
	else {
		for (const int& stmt : currFollowsStmtSet) {
			if (currStmtTypeSet.count(stmt) == 1) {
				std::unordered_set<int> nextFollowsStmtSet;
				nextFollowsStmtSet.emplace(getFollowsOfStmt(stmt));

				std::vector<std::string> nextStmtTypeFollowsList = getStmtTypeIsFollowsList(nextStmtType, nextFollowsStmtSet);

				if (!nextStmtTypeFollowsList.empty()) {
					if (isFirstStmtType) {
						stmtTypeIsFollowedList.push_back(std::to_string(stmt));
						stmtTypeIsFollowerList.push_back(nextStmtTypeFollowsList.front());
					}
					else {
						stmtTypeIsFollowedList.push_back(nextStmtTypeFollowsList.front());
						stmtTypeIsFollowerList.push_back(std::to_string(stmt));
					}
				}
			}
		}
	}

	return std::make_pair(stmtTypeIsFollowedList, stmtTypeIsFollowerList);
}

int FollowsEvaluator::estimateFollowsClauseTableSize(QuerySuchThatClause& currSuchThatClause) {
	std::string firstArg = currSuchThatClause.getFirstArgument();
	ArgumentTypes firstArgType = currSuchThatClause.getFirstArgumentType();
	std::string secondArg = currSuchThatClause.getSecondArgument();
	ArgumentTypes secondArgType = currSuchThatClause.getSecondArgumentType();
	int estimatedTableSize = 0;

	if (firstArg == secondArg) {
		return estimatedTableSize;
	}

	if (firstArgType == ArgumentTypes::ANY) {
		if (secondArgType == ArgumentTypes::STMT || secondArgType == ArgumentTypes::PROG_LINE) {
			estimatedTableSize = PKB::getAllFollowerStmt().size();
		}
		else { // check this
			estimatedTableSize = std::min(QueryEvaluatorUtil::getStmtTypeSet(secondArgType).size(), PKB::getAllFollowerStmt().size());
		}
	}
	else if (firstArgType == ArgumentTypes::INT) {
		if (secondArgType == ArgumentTypes::STMT || secondArgType == ArgumentTypes::PROG_LINE) {
			int firstInt = std::stoi(firstArg);

			if (PKB::getAllFollowedStmt().count(firstInt) == 1) {
				estimatedTableSize = 1;
			}
		}
		else {
			int firstInt = std::stoi(firstArg);

			if (PKB::getAllFollowedStmt().count(firstInt) == 1 && QueryEvaluatorUtil::getStmtTypeSet(secondArgType).count(PKB::getFollowerOfStmt(firstInt)) == 1) {
				estimatedTableSize = 1;
			}
		}
	}
	else if (firstArgType == ArgumentTypes::STMT || firstArgType == ArgumentTypes::PROG_LINE) {
		if (secondArgType == ArgumentTypes::ANY) {
			estimatedTableSize = PKB::getAllFollowedStmt().size();
		}
		else if (secondArgType == ArgumentTypes::INT) {
			int secondInt = std::stoi(secondArg);

			if (PKB::getAllFollowerStmt().count(secondInt) == 1) {
				estimatedTableSize = 1;
			}
		}
		else if (secondArgType == ArgumentTypes::STMT || secondArgType == ArgumentTypes::PROG_LINE) {
			estimatedTableSize = PKB::getAllFollowsPair().size();
		}
		else { // check this
			estimatedTableSize = std::min(QueryEvaluatorUtil::getStmtTypeSet(secondArgType).size(), PKB::getAllFollowerStmt().size());
		}
	}
	else {
		if (secondArgType == ArgumentTypes::ANY) { // check this
			estimatedTableSize = std::min(QueryEvaluatorUtil::getStmtTypeSet(firstArgType).size(), PKB::getAllFollowedStmt().size());
		}
		else if (secondArgType == ArgumentTypes::INT) {
			int secondInt = std::stoi(secondArg);

			if (PKB::getAllFollowerStmt().count(secondInt) == 1 && QueryEvaluatorUtil::getStmtTypeSet(firstArgType).count(PKB::getFollowedOfStmt(secondInt)) == 1) {
				estimatedTableSize = 1;
			}
		}
		else if (secondArgType == ArgumentTypes::STMT || secondArgType == ArgumentTypes::PROG_LINE) { // check this
			estimatedTableSize = std::min(QueryEvaluatorUtil::getStmtTypeSet(firstArgType).size(), PKB::getAllFollowedStmt().size());
		}
		else {
			estimatedTableSize = getStmtTypesAreFollowsListSize(firstArgType, secondArgType);
		}
	}

	return estimatedTableSize;
}

int FollowsEvaluator::getStmtTypesAreFollowsListSize(ArgumentTypes& firstStmtType, ArgumentTypes& secondStmtType) {
	int listSize = 0;

	ArgumentTypes nextStmtType;
	std::unordered_set<int> currStmtTypeSet;
	std::unordered_set<int> currFollowsStmtSet;
	int (*getFollowsOfStmt)(int);

	std::unordered_set<int> firstStmtTypeSet = QueryEvaluatorUtil::getStmtTypeSet(firstStmtType);
	std::unordered_set<int> secondStmtTypeSet = QueryEvaluatorUtil::getStmtTypeSet(secondStmtType);
	bool isFirstStmtType = firstStmtTypeSet.size() < secondStmtTypeSet.size();

	if (isFirstStmtType) {
		nextStmtType = secondStmtType;
		currStmtTypeSet = firstStmtTypeSet;
		currFollowsStmtSet = PKB::getAllFollowedStmt();
		getFollowsOfStmt = &PKB::getFollowerOfStmt;
	}
	else {
		nextStmtType = firstStmtType;
		currStmtTypeSet = secondStmtTypeSet;
		currFollowsStmtSet = PKB::getAllFollowerStmt();
		getFollowsOfStmt = &PKB::getFollowedOfStmt;
	}

	if (currStmtTypeSet.size() < currFollowsStmtSet.size()) {
		for (const int& stmt : currStmtTypeSet) {
			if (currFollowsStmtSet.count(stmt) == 1 && QueryEvaluatorUtil::getStmtTypeSet(nextStmtType).count(getFollowsOfStmt(stmt)) == 1) {
				listSize += 1;
			}
		}
	}
	else {
		for (const int& stmt : currFollowsStmtSet) {
			if (currStmtTypeSet.count(stmt) == 1 && QueryEvaluatorUtil::getStmtTypeSet(nextStmtType).count(getFollowsOfStmt(stmt)) == 1) {
				listSize += 1;
			}
		}
	}

	return listSize;
}