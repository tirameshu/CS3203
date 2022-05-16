#include "ParentEvaluator.h"

bool ParentEvaluator::checkParentClauseHolds(QuerySuchThatClause currSuchThatClause) {
	std::string firstArg = currSuchThatClause.getFirstArgument();
	ArgumentTypes firstArgType = currSuchThatClause.getFirstArgumentType();
	std::string secondArg = currSuchThatClause.getSecondArgument();
	ArgumentTypes secondArgType = currSuchThatClause.getSecondArgumentType();
	bool parentClauseHolds = false;

	if (firstArg == secondArg && firstArgType != ArgumentTypes::ANY) {
		return parentClauseHolds;
	}

	if (firstArgType == ArgumentTypes::ANY || firstArgType == ArgumentTypes::STMT || firstArgType == ArgumentTypes::PROG_LINE) {
		if (secondArgType == ArgumentTypes::ANY || secondArgType == ArgumentTypes::STMT || secondArgType == ArgumentTypes::PROG_LINE) {
			parentClauseHolds = PKB::programHasChildStmt();
		}
		else if (secondArgType == ArgumentTypes::INT) {
			parentClauseHolds = PKB::getAllChildStmt().count(std::stoi(secondArg)) == 1;
		}
		else {
			parentClauseHolds = checkStmtTypeIsParent(secondArgType, PKB::getAllChildStmt());
		}
	}
	else if (firstArgType == ArgumentTypes::INT) {
		if (secondArgType == ArgumentTypes::ANY || secondArgType == ArgumentTypes::STMT || secondArgType == ArgumentTypes::PROG_LINE) {
			parentClauseHolds = PKB::getAllParentStmt().count(std::stoi(firstArg)) == 1;
		}
		else if (secondArgType == ArgumentTypes::INT) {
			int firstInt = std::stoi(firstArg);
			int secondInt = std::stoi(secondArg);

			if (firstInt < secondInt) {
				parentClauseHolds = PKB::getAllParentPair().count(std::make_pair(firstInt, secondInt)) == 1;
			}
		}
		else {
			int firstInt = std::stoi(firstArg);

			if (PKB::getAllParentStmt().count(firstInt) == 1) {
				std::unordered_set<int> childStmtSet = PKB::getChildrenListOfStmt(firstInt);
				parentClauseHolds = checkStmtTypeIsParent(secondArgType, childStmtSet);
			}
		}
	}
	else {
		if (secondArgType == ArgumentTypes::ANY || secondArgType == ArgumentTypes::STMT || secondArgType == ArgumentTypes::PROG_LINE) {
			parentClauseHolds = checkStmtTypeIsParent(firstArgType, PKB::getAllParentStmt());
		}
		else if (secondArgType == ArgumentTypes::INT) {
			int secondInt = std::stoi(secondArg);

			if (PKB::getAllChildStmt().count(secondInt) == 1) {
				std::unordered_set<int> parentStmtSet;
				parentStmtSet.emplace(PKB::getParentOfStmt(secondInt));
				parentClauseHolds = checkStmtTypeIsParent(firstArgType, parentStmtSet);
			}
		}
		else {
			parentClauseHolds = checkStmtTypesAreParent(firstArgType, secondArgType);
		}
	}

	return parentClauseHolds;
}

bool ParentEvaluator::checkStmtTypeIsParent(ArgumentTypes& stmtType, std::unordered_set<int>& parentStmtSet) {
	bool stmtTypeIsParent = false;
	std::unordered_set<int> stmtTypeSet = QueryEvaluatorUtil::getStmtTypeSet(stmtType);

	if (stmtTypeSet.size() < parentStmtSet.size()) {
		for (const int& stmt : stmtTypeSet) {
			if (parentStmtSet.count(stmt) == 1) {
				stmtTypeIsParent = true;
				break;
			}
		}
	}
	else {
		for (const int& stmt : parentStmtSet) {
			if (stmtTypeSet.count(stmt) == 1) {
				stmtTypeIsParent = true;
				break;
			}
		}
	}

	return stmtTypeIsParent;
}

bool ParentEvaluator::checkStmtTypesAreParent(ArgumentTypes& firstStmtType, ArgumentTypes& secondStmtType) {
	bool stmtTypesAreParent = false;

	ArgumentTypes nextStmtType;
	std::unordered_set<int> currStmtTypeSet;
	std::unordered_set<int> currParentStmtSet;
	std::unordered_set<int> (*getParentSetOfStmt)(int);

	std::unordered_set<int> firstStmtTypeSet = QueryEvaluatorUtil::getStmtTypeSet(firstStmtType);
	std::unordered_set<int> secondStmtTypeSet = QueryEvaluatorUtil::getStmtTypeSet(secondStmtType);

	if (firstStmtTypeSet.size() < secondStmtTypeSet.size()) {
		nextStmtType = secondStmtType;
		currStmtTypeSet = firstStmtTypeSet;
		currParentStmtSet = PKB::getAllParentStmt();
		getParentSetOfStmt = &PKB::getChildrenListOfStmt;
	}
	else {
		nextStmtType = firstStmtType;
		currStmtTypeSet = secondStmtTypeSet;
		currParentStmtSet = PKB::getAllChildStmt();
		getParentSetOfStmt = [](int stmt) {
			std::unordered_set<int> temp;
			temp.emplace(PKB::getParentOfStmt(stmt));
			return temp;
		};
	}

	if (currStmtTypeSet.size() < currParentStmtSet.size()) {
		for (const int& stmt : currStmtTypeSet) {
			if (currParentStmtSet.count(stmt) == 1) {
				std::unordered_set<int> nextParentStmtSet = getParentSetOfStmt(stmt);

				if (checkStmtTypeIsParent(nextStmtType, nextParentStmtSet)) {
					stmtTypesAreParent = true;
					break;
				}
			}
		}
	}
	else {
		for (const int& stmt : currParentStmtSet) {
			if (currStmtTypeSet.count(stmt) == 1) {
				std::unordered_set<int> nextParentStmtSet = getParentSetOfStmt(stmt);

				if (checkStmtTypeIsParent(nextStmtType, nextParentStmtSet)) {
					stmtTypesAreParent = true;
					break;
				}
			}
		}
	}

	return stmtTypesAreParent;
}

std::unordered_map<std::string, std::vector<std::string>> ParentEvaluator::evaluateParentClause(QuerySuchThatClause& currSuchThatClause) {
	std::string firstArg = currSuchThatClause.getFirstArgument();
	ArgumentTypes firstArgType = currSuchThatClause.getFirstArgumentType();
	std::string secondArg = currSuchThatClause.getSecondArgument();
	ArgumentTypes secondArgType = currSuchThatClause.getSecondArgumentType();
	std::unordered_map<std::string, std::vector<std::string>> parentClauseTable;

	if (firstArg == secondArg) {
		return parentClauseTable;
	}

	if (firstArgType == ArgumentTypes::ANY) {
		if (secondArgType == ArgumentTypes::STMT || secondArgType == ArgumentTypes::PROG_LINE) {
			parentClauseTable.emplace(secondArg, QueryEvaluatorUtil::convertIntSetToStringList(PKB::getAllChildStmt()));
		}
		else {
			parentClauseTable.emplace(secondArg, getStmtTypeIsParentList(secondArgType, PKB::getAllChildStmt()));
		}
	}
	else if (firstArgType == ArgumentTypes::INT) {
		if (secondArgType == ArgumentTypes::STMT || secondArgType == ArgumentTypes::PROG_LINE) {
			int firstInt = std::stoi(firstArg);

			if (PKB::getAllParentStmt().count(firstInt) == 1) {
				std::vector<std::string> childStmtList = QueryEvaluatorUtil::convertIntSetToStringList(PKB::getChildrenListOfStmt(firstInt));
				parentClauseTable.emplace(secondArg, childStmtList);
			}
		}
		else {
			int firstInt = std::stoi(firstArg);

			if (PKB::getAllParentStmt().count(firstInt) == 1) {
				std::unordered_set<int> childStmtSet = PKB::getChildrenListOfStmt(firstInt);
				parentClauseTable.emplace(secondArg, getStmtTypeIsParentList(secondArgType, childStmtSet));
			}
		}
	}
	else if (firstArgType == ArgumentTypes::STMT || firstArgType == ArgumentTypes::PROG_LINE) {
		if (secondArgType == ArgumentTypes::ANY) {
			parentClauseTable.emplace(firstArg, QueryEvaluatorUtil::convertIntSetToStringList(PKB::getAllParentStmt()));
		}
		else if (secondArgType == ArgumentTypes::INT) {
			int secondInt = std::stoi(secondArg);

			if (PKB::getAllChildStmt().count(secondInt) == 1) {
				std::vector<std::string> parentStmtList;
				parentStmtList.push_back(std::to_string(PKB::getParentOfStmt(secondInt)));
				parentClauseTable.emplace(firstArg, parentStmtList);
			}
		}
		else if (secondArgType == ArgumentTypes::STMT || secondArgType == ArgumentTypes::PROG_LINE) {
			std::vector<std::string> parentStmtList;
			std::vector<std::string> childStmtList;

			for (const std::pair<int, int>& parentPair : PKB::getAllParentPair()) {
				parentStmtList.push_back(std::to_string(parentPair.first));
				childStmtList.push_back(std::to_string(parentPair.second));
			}

			parentClauseTable.emplace(firstArg, parentStmtList);
			parentClauseTable.emplace(secondArg, childStmtList);
		}
		else {
			std::vector<std::string> parentStmtList;
			std::vector<std::string> childStmtList = getStmtTypeIsParentList(secondArgType, PKB::getAllChildStmt());

			for (const std::string& stmt : childStmtList) {
				parentStmtList.push_back(std::to_string(PKB::getParentOfStmt(std::stoi(stmt))));
			}

			parentClauseTable.emplace(firstArg, parentStmtList);
			parentClauseTable.emplace(secondArg, childStmtList);
		}
	}
	else {
		if (secondArgType == ArgumentTypes::ANY) {
			parentClauseTable.emplace(firstArg, getStmtTypeIsParentList(firstArgType, PKB::getAllParentStmt()));
		}
		else if (secondArgType == ArgumentTypes::INT) {
			int secondInt = std::stoi(secondArg);

			if (PKB::getAllChildStmt().count(secondInt) == 1) {
				std::unordered_set<int> parentStmtSet;
				parentStmtSet.emplace(PKB::getParentOfStmt(secondInt));
				parentClauseTable.emplace(firstArg, getStmtTypeIsParentList(firstArgType, parentStmtSet));
			}
		}
		else if (secondArgType == ArgumentTypes::STMT || secondArgType == ArgumentTypes::PROG_LINE) {
			std::vector<std::string> parentStmtList;
			std::vector<std::string> childStmtList;

			for (const std::string& stmt : getStmtTypeIsParentList(firstArgType, PKB::getAllParentStmt())) {
				for (const int& child : PKB::getChildrenListOfStmt(std::stoi(stmt))) {
					parentStmtList.push_back(stmt);
					childStmtList.push_back(std::to_string(child));
				}
			}

			parentClauseTable.emplace(firstArg, parentStmtList);
			parentClauseTable.emplace(secondArg, childStmtList);
		}
		else {
			std::pair<std::vector<std::string>, std::vector<std::string>> parentListPair = getStmtTypesAreParentList(firstArgType, secondArgType);
			parentClauseTable.emplace(firstArg, parentListPair.first);
			parentClauseTable.emplace(secondArg, parentListPair.second);
		}
	}

	return parentClauseTable;
}

std::vector<std::string> ParentEvaluator::getStmtTypeIsParentList(ArgumentTypes& stmtType, std::unordered_set<int>& parentStmtSet) {
	std::vector<std::string> stmtTypeIsParentList;
	std::unordered_set<int> stmtTypeSet = QueryEvaluatorUtil::getStmtTypeSet(stmtType);

	if (stmtTypeSet.size() < parentStmtSet.size()) {
		for (const int& stmt : stmtTypeSet) {
			if (parentStmtSet.count(stmt) == 1) {
				stmtTypeIsParentList.push_back(std::to_string(stmt));
			}
		}
	}
	else {
		for (const int& stmt : parentStmtSet) {
			if (stmtTypeSet.count(stmt) == 1) {
				stmtTypeIsParentList.push_back(std::to_string(stmt));
			}
		}
	}

	return stmtTypeIsParentList;
}

std::pair<std::vector<std::string>, std::vector<std::string>> ParentEvaluator::getStmtTypesAreParentList(ArgumentTypes& firstStmtType, ArgumentTypes& secondStmtType) {
	std::vector<std::string> stmtTypeIsParentList;
	std::vector<std::string> stmtTypeIsChildList;

	ArgumentTypes nextStmtType;
	std::unordered_set<int> currStmtTypeSet;
	std::unordered_set<int> currParentStmtSet;
	std::unordered_set<int> (*getParentSetOfStmt)(int);

	std::unordered_set<int> firstStmtTypeSet = QueryEvaluatorUtil::getStmtTypeSet(firstStmtType);
	std::unordered_set<int> secondStmtTypeSet = QueryEvaluatorUtil::getStmtTypeSet(secondStmtType);
	bool isFirstStmtType = firstStmtTypeSet.size() < secondStmtTypeSet.size();

	if (isFirstStmtType) {
		nextStmtType = secondStmtType;
		currStmtTypeSet = firstStmtTypeSet;
		currParentStmtSet = PKB::getAllParentStmt();
		getParentSetOfStmt = &PKB::getChildrenListOfStmt;
	}
	else {
		nextStmtType = firstStmtType;
		currStmtTypeSet = secondStmtTypeSet;
		currParentStmtSet = PKB::getAllChildStmt();
		getParentSetOfStmt = [](int stmt) {
			std::unordered_set<int> temp;
			temp.emplace(PKB::getParentOfStmt(stmt));
			return temp;
		};
	}

	if (currStmtTypeSet.size() < currParentStmtSet.size()) {
		for (const int& stmt : currStmtTypeSet) {
			if (currParentStmtSet.count(stmt) == 1) {
				std::unordered_set<int> nextParentStmtSet = getParentSetOfStmt(stmt);
				std::vector<std::string> nextStmtTypeParentList = getStmtTypeIsParentList(nextStmtType, nextParentStmtSet);

				for (const std::string& curr : nextStmtTypeParentList) {
					if (isFirstStmtType) {
						stmtTypeIsParentList.push_back(std::to_string(stmt));
						stmtTypeIsChildList.push_back(curr);
					}
					else {
						stmtTypeIsParentList.push_back(curr);
						stmtTypeIsChildList.push_back(std::to_string(stmt));
					}
				}
			}
		}
	}
	else {
		for (const int& stmt : currParentStmtSet) {
			if (currStmtTypeSet.count(stmt) == 1) {
				std::unordered_set<int> nextParentStmtSet = getParentSetOfStmt(stmt);
				std::vector<std::string> nextStmtTypeParentList = getStmtTypeIsParentList(nextStmtType, nextParentStmtSet);

				for (const std::string& curr : nextStmtTypeParentList) {
					if (isFirstStmtType) {
						stmtTypeIsParentList.push_back(std::to_string(stmt));
						stmtTypeIsChildList.push_back(curr);
					}
					else {
						stmtTypeIsParentList.push_back(curr);
						stmtTypeIsChildList.push_back(std::to_string(stmt));
					}
				}
			}
		}
	}

	return std::make_pair(stmtTypeIsParentList, stmtTypeIsChildList);
}

int ParentEvaluator::estimateParentClauseTableSize(QuerySuchThatClause& currSuchThatClause) {
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
			estimatedTableSize = PKB::getAllChildStmt().size();
		}
		else { // check this
			estimatedTableSize = std::min(QueryEvaluatorUtil::getStmtTypeSet(secondArgType).size(), PKB::getAllChildStmt().size());
		}
	}
	else if (firstArgType == ArgumentTypes::INT) {
		if (secondArgType == ArgumentTypes::STMT || secondArgType == ArgumentTypes::PROG_LINE) {
			int firstInt = std::stoi(firstArg);

			if (PKB::getAllParentStmt().count(firstInt) == 1) {
				estimatedTableSize = PKB::getChildrenListOfStmt(firstInt).size();
			}
		}
		else { // check this
			int firstInt = std::stoi(firstArg);

			if (PKB::getAllParentStmt().count(firstInt) == 1) {
				estimatedTableSize = std::min(QueryEvaluatorUtil::getStmtTypeSet(secondArgType).size(), PKB::getChildrenListOfStmt(firstInt).size());
			}
		}
	}
	else if (firstArgType == ArgumentTypes::STMT || firstArgType == ArgumentTypes::PROG_LINE) {
		if (secondArgType == ArgumentTypes::ANY) {
			estimatedTableSize = PKB::getAllParentStmt().size();
		}
		else if (secondArgType == ArgumentTypes::INT) {
			int secondInt = std::stoi(secondArg);

			if (PKB::getAllChildStmt().count(secondInt) == 1) {
				estimatedTableSize = 1;
			}
		}
		else if (secondArgType == ArgumentTypes::STMT || secondArgType == ArgumentTypes::PROG_LINE) {
			estimatedTableSize = PKB::getAllParentPair().size();
		}
		else { // check this
			estimatedTableSize = std::min(QueryEvaluatorUtil::getStmtTypeSet(secondArgType).size(), PKB::getAllChildStmt().size());
		}
	}
	else {
		if (secondArgType == ArgumentTypes::ANY) { // check this
			estimatedTableSize = std::min(QueryEvaluatorUtil::getStmtTypeSet(firstArgType).size(), PKB::getAllParentStmt().size());
		}
		else if (secondArgType == ArgumentTypes::INT) {
			int secondInt = std::stoi(secondArg);

			if (PKB::getAllChildStmt().count(secondInt) == 1 && QueryEvaluatorUtil::getStmtTypeSet(firstArgType).count(PKB::getParentOfStmt(secondInt)) == 1) {
				estimatedTableSize = 1;
			}
		}
		else if (secondArgType == ArgumentTypes::STMT || secondArgType == ArgumentTypes::PROG_LINE) {
			std::vector<std::string> parentStmtList;
			std::vector<std::string> childStmtList;

			for (const std::string& stmt : getStmtTypeIsParentList(firstArgType, PKB::getAllParentStmt())) {
				estimatedTableSize += PKB::getChildrenListOfStmt(std::stoi(stmt)).size();
			}
		}
		else {
			estimatedTableSize = getStmtTypesAreParentListSize(firstArgType, secondArgType);
		}
	}

	return estimatedTableSize;
}

int ParentEvaluator::getStmtTypesAreParentListSize(ArgumentTypes& firstStmtType, ArgumentTypes& secondStmtType) {
	int listSize = 0;

	ArgumentTypes nextStmtType;
	std::unordered_set<int> currStmtTypeSet;
	std::unordered_set<int> currParentStmtSet;
	std::unordered_set<int> (*getParentSetOfStmt)(int);

	std::unordered_set<int> firstStmtTypeSet = QueryEvaluatorUtil::getStmtTypeSet(firstStmtType);
	std::unordered_set<int> secondStmtTypeSet = QueryEvaluatorUtil::getStmtTypeSet(secondStmtType);
	bool isFirstStmtType = firstStmtTypeSet.size() < secondStmtTypeSet.size();

	if (isFirstStmtType) {
		nextStmtType = secondStmtType;
		currStmtTypeSet = firstStmtTypeSet;
		currParentStmtSet = PKB::getAllParentStmt();
		getParentSetOfStmt = &PKB::getChildrenListOfStmt;
	}
	else {
		nextStmtType = firstStmtType;
		currStmtTypeSet = secondStmtTypeSet;
		currParentStmtSet = PKB::getAllChildStmt();
		getParentSetOfStmt = [](int stmt) {
			std::unordered_set<int> temp;
			temp.emplace(PKB::getParentOfStmt(stmt));
			return temp;
		};
	}

	if (currStmtTypeSet.size() < currParentStmtSet.size()) {
		for (const int& stmt : currStmtTypeSet) {
			if (currParentStmtSet.count(stmt) == 1) {
				listSize += std::min(QueryEvaluatorUtil::getStmtTypeSet(nextStmtType).size(), getParentSetOfStmt(stmt).size());
			}
		}
	}
	else {
		for (const int& stmt : currParentStmtSet) {
			if (currStmtTypeSet.count(stmt) == 1) {
				listSize += std::min(QueryEvaluatorUtil::getStmtTypeSet(nextStmtType).size(), getParentSetOfStmt(stmt).size());
			}
		}
	}

	return listSize;
}