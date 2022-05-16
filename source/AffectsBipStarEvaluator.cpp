#include "AffectsBipStarEvaluator.h"

bool AffectsBipStarEvaluator::checkAffectsBipStarClauseHolds(QuerySuchThatClause currSuchThatClause) {
	std::string firstArg = currSuchThatClause.getFirstArgument();
	ArgumentTypes firstArgType = currSuchThatClause.getFirstArgumentType();
	std::string secondArg = currSuchThatClause.getSecondArgument();
	ArgumentTypes secondArgType = currSuchThatClause.getSecondArgumentType();
	bool affectsBipStarClauseHolds = false;

	if (firstArgType == ArgumentTypes::ANY || firstArgType == ArgumentTypes::STMT || firstArgType == ArgumentTypes::PROG_LINE || firstArgType == ArgumentTypes::ASSIGN) {
		if (secondArgType == ArgumentTypes::ANY || secondArgType == ArgumentTypes::STMT || secondArgType == ArgumentTypes::PROG_LINE || secondArgType == ArgumentTypes::ASSIGN) {
			// AffectsBip* (s, s)
			if (firstArg == secondArg && firstArgType != ArgumentTypes::ANY) {
				for (int stmt : PKB::getAffectingBipList()) {
					if (PKB::getAffectingBipTOfStmt(stmt).count(stmt) == 1) {
						affectsBipStarClauseHolds = true;
						break;
					}
				}
			}
			else {
				affectsBipStarClauseHolds = AffectsBipEvaluator::checkAffectsBipClauseHolds(currSuchThatClause);
			}
		}
		else if (secondArgType == ArgumentTypes::INT) {
			affectsBipStarClauseHolds = AffectsBipEvaluator::checkAffectsBipClauseHolds(currSuchThatClause);
		}
	}
	else if (firstArgType == ArgumentTypes::INT) {
		if (secondArgType == ArgumentTypes::ANY || secondArgType == ArgumentTypes::STMT || secondArgType == ArgumentTypes::PROG_LINE || secondArgType == ArgumentTypes::ASSIGN) {
			affectsBipStarClauseHolds = AffectsBipEvaluator::checkAffectsBipClauseHolds(currSuchThatClause);
		}
		else if (secondArgType == ArgumentTypes::INT) {
			int firstInt = std::stoi(firstArg);
			int secondInt = std::stoi(secondArg);

			affectsBipStarClauseHolds = PKB::getAffectsBipStarPairList().count(std::make_pair(firstInt, secondInt)) == 1;
		}
	}

	return affectsBipStarClauseHolds;
}

std::unordered_map<std::string, std::vector<std::string>> AffectsBipStarEvaluator::evaluateAffectsBipStarClause(QuerySuchThatClause& currSuchThatClause) {
	std::string firstArg = currSuchThatClause.getFirstArgument();
	ArgumentTypes firstArgType = currSuchThatClause.getFirstArgumentType();
	std::string secondArg = currSuchThatClause.getSecondArgument();
	ArgumentTypes secondArgType = currSuchThatClause.getSecondArgumentType();
	std::unordered_map<std::string, std::vector<std::string>> affectsBipStarClauseTable;

	if (firstArgType == ArgumentTypes::ANY) {
		affectsBipStarClauseTable = AffectsBipEvaluator::evaluateAffectsBipClause(currSuchThatClause);
	}
	else if (firstArgType == ArgumentTypes::INT) {
		if (secondArgType == ArgumentTypes::STMT || secondArgType == ArgumentTypes::PROG_LINE || secondArgType == ArgumentTypes::ASSIGN) {
			int firstInt = std::stoi(firstArg);

			if (PKB::stmtHasAffectingBip(firstInt)) {
				std::vector<std::string> affectedBipStmtList = QueryEvaluatorUtil::convertIntSetToStringList(PKB::getAffectingBipTOfStmt(firstInt));
				affectsBipStarClauseTable.emplace(secondArg, affectedBipStmtList);
			}
		}
	}
	else if (firstArgType == ArgumentTypes::STMT || firstArgType == ArgumentTypes::PROG_LINE || firstArgType == ArgumentTypes::ASSIGN) {
		if (secondArgType == ArgumentTypes::ANY) {
			affectsBipStarClauseTable = AffectsBipEvaluator::evaluateAffectsBipClause(currSuchThatClause);
		}
		else if (secondArgType == ArgumentTypes::INT) {
			int secondInt = std::stoi(secondArg);

			if (PKB::stmtHasAffectedBipBy(secondInt)) {
				std::vector<std::string> affectsBipStmtList = QueryEvaluatorUtil::convertIntSetToStringList(PKB::getAffectedBipByTOfStmt(secondInt));
				affectsBipStarClauseTable.emplace(firstArg, affectsBipStmtList);
			}
		}
		else if (secondArgType == ArgumentTypes::STMT || secondArgType == ArgumentTypes::PROG_LINE || secondArgType == ArgumentTypes::ASSIGN) {
			std::vector<std::string> affectsBipStmtList;
			std::vector<std::string> affectedBipStmtList;
			bool isSameStmtRef = firstArg == secondArg;

			for (int affecting : PKB::getAffectingBipList()) {
				std::unordered_set<int>& affectedBipStmtSet = PKB::getAffectingBipTOfStmt(affecting);

				if (isSameStmtRef) {
					if (affectedBipStmtSet.count(affecting) == 1) {
						affectsBipStmtList.push_back(std::to_string(affecting));
					}
				}
				else {
					for (int affected : affectedBipStmtSet) {
						affectsBipStmtList.push_back(std::to_string(affecting));
						affectedBipStmtList.push_back(std::to_string(affected));
					}
				}
			}

			affectsBipStarClauseTable.emplace(firstArg, affectsBipStmtList);

			if (!isSameStmtRef) {
				affectsBipStarClauseTable.emplace(secondArg, affectedBipStmtList);
			}
		}
	}

	return affectsBipStarClauseTable;
}

int AffectsBipStarEvaluator::estimateAffectsBipStarClauseTableSize(QuerySuchThatClause& currSuchThatClause) {
	std::string firstArg = currSuchThatClause.getFirstArgument();
	ArgumentTypes firstArgType = currSuchThatClause.getFirstArgumentType();
	std::string secondArg = currSuchThatClause.getSecondArgument();
	ArgumentTypes secondArgType = currSuchThatClause.getSecondArgumentType();
	int estimatedTableSize = 0;

	if (firstArgType == ArgumentTypes::ANY) {
		estimatedTableSize = AffectsBipEvaluator::estimateAffectsBipClauseTableSize(currSuchThatClause);
	}
	else if (firstArgType == ArgumentTypes::INT) {
		if (secondArgType == ArgumentTypes::STMT || secondArgType == ArgumentTypes::PROG_LINE || secondArgType == ArgumentTypes::ASSIGN) {
			int firstInt = std::stoi(firstArg);

			if (PKB::stmtHasAffectingBip(firstInt)) {
				estimatedTableSize = PKB::getAffectingBipTOfStmt(firstInt).size();
			}
		}
	}
	else if (firstArgType == ArgumentTypes::STMT || firstArgType == ArgumentTypes::PROG_LINE || firstArgType == ArgumentTypes::ASSIGN) {
		if (secondArgType == ArgumentTypes::ANY) {
			estimatedTableSize = AffectsBipEvaluator::estimateAffectsBipClauseTableSize(currSuchThatClause);
		}
		else if (secondArgType == ArgumentTypes::INT) {
			int secondInt = std::stoi(secondArg);

			if (PKB::stmtHasAffectedBipBy(secondInt)) {
				estimatedTableSize = PKB::getAffectedBipByTOfStmt(secondInt).size();
			}
		}
		else if (secondArgType == ArgumentTypes::STMT || secondArgType == ArgumentTypes::PROG_LINE || secondArgType == ArgumentTypes::ASSIGN) {
			std::vector<std::string> affectsBipStmtList;
			std::vector<std::string> affectedBipStmtList;
			bool isSameStmtRef = firstArg == secondArg;

			for (int affecting : PKB::getAffectingBipList()) {
				std::unordered_set<int>& affectedBipStmtSet = PKB::getAffectingBipTOfStmt(affecting);

				if (isSameStmtRef) {
					if (affectedBipStmtSet.count(affecting) == 1) {
						estimatedTableSize += 1;
					}
				}
				else {
					estimatedTableSize += affectedBipStmtSet.size();
				}
			}
		}
	}

	return estimatedTableSize;
}