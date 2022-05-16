#include "AffectsStarEvaluator.h"

bool AffectsStarEvaluator::checkAffectsStarClauseHolds(QuerySuchThatClause currSuchThatClause) {
	std::string firstArg = currSuchThatClause.getFirstArgument();
	ArgumentTypes firstArgType = currSuchThatClause.getFirstArgumentType();
	std::string secondArg = currSuchThatClause.getSecondArgument();
	ArgumentTypes secondArgType = currSuchThatClause.getSecondArgumentType();
	bool affectsStarClauseHolds = false;

	if (firstArgType == ArgumentTypes::ANY || firstArgType == ArgumentTypes::STMT || firstArgType == ArgumentTypes::PROG_LINE || firstArgType == ArgumentTypes::ASSIGN) {
		if (secondArgType == ArgumentTypes::ANY || secondArgType == ArgumentTypes::STMT || secondArgType == ArgumentTypes::PROG_LINE || secondArgType == ArgumentTypes::ASSIGN) {
			// Affects* (s, s)
			if (firstArg == secondArg && firstArgType != ArgumentTypes::ANY) {
				for (int stmt : PKB::getAffectingList()) {
					if (PKB::getAffectingTOfStmt(stmt).count(stmt) == 1) {
						affectsStarClauseHolds = true;
						break;
					}
				}
			}
			else {
				affectsStarClauseHolds = AffectsEvaluator::checkAffectsClauseHolds(currSuchThatClause);
			}
		}
		else if (secondArgType == ArgumentTypes::INT) {
			affectsStarClauseHolds = AffectsEvaluator::checkAffectsClauseHolds(currSuchThatClause);
		}
	}
	else if (firstArgType == ArgumentTypes::INT) {
		if (secondArgType == ArgumentTypes::ANY || secondArgType == ArgumentTypes::STMT || secondArgType == ArgumentTypes::PROG_LINE || secondArgType == ArgumentTypes::ASSIGN) {
			affectsStarClauseHolds = AffectsEvaluator::checkAffectsClauseHolds(currSuchThatClause);
		}
		else if (secondArgType == ArgumentTypes::INT) {
			int firstInt = std::stoi(firstArg);
			int secondInt = std::stoi(secondArg);

			affectsStarClauseHolds = PKB::getAffectsStarPairList().count(std::make_pair(firstInt, secondInt)) == 1;
		}
	}

	return affectsStarClauseHolds;
}

std::unordered_map<std::string, std::vector<std::string>> AffectsStarEvaluator::evaluateAffectsStarClause(QuerySuchThatClause& currSuchThatClause) {
	std::string firstArg = currSuchThatClause.getFirstArgument();
	ArgumentTypes firstArgType = currSuchThatClause.getFirstArgumentType();
	std::string secondArg = currSuchThatClause.getSecondArgument();
	ArgumentTypes secondArgType = currSuchThatClause.getSecondArgumentType();
	std::unordered_map<std::string, std::vector<std::string>> affectsStarClauseTable;

	if (firstArgType == ArgumentTypes::ANY) {
		affectsStarClauseTable = AffectsEvaluator::evaluateAffectsClause(currSuchThatClause);
	}
	else if (firstArgType == ArgumentTypes::INT) {
		if (secondArgType == ArgumentTypes::STMT || secondArgType == ArgumentTypes::PROG_LINE || secondArgType == ArgumentTypes::ASSIGN) {
			int firstInt = std::stoi(firstArg);

			if (PKB::stmtHasAffecting(firstInt)) {
				std::vector<std::string> affectedStmtList = QueryEvaluatorUtil::convertIntSetToStringList(PKB::getAffectingTOfStmt(firstInt));
				affectsStarClauseTable.emplace(secondArg, affectedStmtList);
			}
		}
	}
	else if (firstArgType == ArgumentTypes::STMT || firstArgType == ArgumentTypes::PROG_LINE || firstArgType == ArgumentTypes::ASSIGN) {
		if (secondArgType == ArgumentTypes::ANY) {
			affectsStarClauseTable = AffectsEvaluator::evaluateAffectsClause(currSuchThatClause);
		}
		else if (secondArgType == ArgumentTypes::INT) {
			int secondInt = std::stoi(secondArg);

			if (PKB::stmtHasAffectedBy(secondInt)) {
				std::vector<std::string> affectsStmtList = QueryEvaluatorUtil::convertIntSetToStringList(PKB::getAffectedByTOfStmt(secondInt));
				affectsStarClauseTable.emplace(firstArg, affectsStmtList);
			}
		}
		else if (secondArgType == ArgumentTypes::STMT || secondArgType == ArgumentTypes::PROG_LINE || secondArgType == ArgumentTypes::ASSIGN) {
			std::vector<std::string> affectsStmtList;
			std::vector<std::string> affectedStmtList;
			bool isSameStmtRef = firstArg == secondArg;

			for (int affecting : PKB::getAffectingList()) {
				std::unordered_set<int>& affectedStmtSet = PKB::getAffectingTOfStmt(affecting);

				if (isSameStmtRef) {
					if (affectedStmtSet.count(affecting) == 1) {
						affectsStmtList.push_back(std::to_string(affecting));
					}
				}
				else {
					for (int affected : affectedStmtSet) {
						affectsStmtList.push_back(std::to_string(affecting));
						affectedStmtList.push_back(std::to_string(affected));
					}
				}
			}

			affectsStarClauseTable.emplace(firstArg, affectsStmtList);

			if (!isSameStmtRef) {
				affectsStarClauseTable.emplace(secondArg, affectedStmtList);
			}
		}
	}

	return affectsStarClauseTable;
}

int AffectsStarEvaluator::estimateAffectsStarClauseTableSize(QuerySuchThatClause& currSuchThatClause) {
	std::string firstArg = currSuchThatClause.getFirstArgument();
	ArgumentTypes firstArgType = currSuchThatClause.getFirstArgumentType();
	std::string secondArg = currSuchThatClause.getSecondArgument();
	ArgumentTypes secondArgType = currSuchThatClause.getSecondArgumentType();
	int estimatedTableSize = 0;

	if (firstArgType == ArgumentTypes::ANY) {
		estimatedTableSize = AffectsEvaluator::estimateAffectsClauseTableSize(currSuchThatClause);
	}
	else if (firstArgType == ArgumentTypes::INT) {
		if (secondArgType == ArgumentTypes::STMT || secondArgType == ArgumentTypes::PROG_LINE || secondArgType == ArgumentTypes::ASSIGN) {
			int firstInt = std::stoi(firstArg);

			if (PKB::stmtHasAffecting(firstInt)) {
				estimatedTableSize = PKB::getAffectingTOfStmt(firstInt).size();
			}
		}
	}
	else if (firstArgType == ArgumentTypes::STMT || firstArgType == ArgumentTypes::PROG_LINE || firstArgType == ArgumentTypes::ASSIGN) {
		if (secondArgType == ArgumentTypes::ANY) {
			estimatedTableSize = AffectsEvaluator::estimateAffectsClauseTableSize(currSuchThatClause);
		}
		else if (secondArgType == ArgumentTypes::INT) {
			int secondInt = std::stoi(secondArg);

			if (PKB::stmtHasAffectedBy(secondInt)) {
				estimatedTableSize = PKB::getAffectedByTOfStmt(secondInt).size();
			}
		}
		else if (secondArgType == ArgumentTypes::STMT || secondArgType == ArgumentTypes::PROG_LINE || secondArgType == ArgumentTypes::ASSIGN) {
			std::vector<std::string> affectsStmtList;
			std::vector<std::string> affectedStmtList;
			bool isSameStmtRef = firstArg == secondArg;

			for (int affecting : PKB::getAffectingList()) {
				std::unordered_set<int>& affectedStmtSet = PKB::getAffectingTOfStmt(affecting);

				if (isSameStmtRef) {
					if (affectedStmtSet.count(affecting) == 1) {
						estimatedTableSize += 1;
					}
				}
				else {
					estimatedTableSize += affectedStmtSet.size();
				}
			}
		}
	}

	return estimatedTableSize;
}