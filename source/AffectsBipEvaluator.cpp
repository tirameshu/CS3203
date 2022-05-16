#include "AffectsBipEvaluator.h"

bool AffectsBipEvaluator::checkAffectsBipClauseHolds(QuerySuchThatClause currSuchThatClause) {
	std::string firstArg = currSuchThatClause.getFirstArgument();
	ArgumentTypes firstArgType = currSuchThatClause.getFirstArgumentType();
	std::string secondArg = currSuchThatClause.getSecondArgument();
	ArgumentTypes secondArgType = currSuchThatClause.getSecondArgumentType();
	bool affectsBipClauseHolds = false;

	if (firstArgType == ArgumentTypes::ANY || firstArgType == ArgumentTypes::STMT || firstArgType == ArgumentTypes::PROG_LINE || firstArgType == ArgumentTypes::ASSIGN) {
		if (secondArgType == ArgumentTypes::ANY || secondArgType == ArgumentTypes::STMT || secondArgType == ArgumentTypes::PROG_LINE || secondArgType == ArgumentTypes::ASSIGN) {
			// AffectsBip (s, s)
			if (firstArg == secondArg && firstArgType != ArgumentTypes::ANY) {
				for (int stmt : PKB::getAffectingBipList()) {
					if (PKB::getAffectingBipOfStmt(stmt).count(stmt) == 1) {
						affectsBipClauseHolds = true;
						break;
					}
				}
			}
			else {
				affectsBipClauseHolds = PKB::programHasAffectedBip();
			}
		}
		else if (secondArgType == ArgumentTypes::INT) {
			affectsBipClauseHolds = PKB::stmtHasAffectedBipBy(std::stoi(secondArg));
		}
	}
	else if (firstArgType == ArgumentTypes::INT) {
		if (secondArgType == ArgumentTypes::ANY || secondArgType == ArgumentTypes::STMT || secondArgType == ArgumentTypes::PROG_LINE || secondArgType == ArgumentTypes::ASSIGN) {
			affectsBipClauseHolds = PKB::stmtHasAffectingBip(std::stoi(firstArg));
		}
		else if (secondArgType == ArgumentTypes::INT) {
			int firstInt = std::stoi(firstArg);
			int secondInt = std::stoi(secondArg);

			affectsBipClauseHolds = PKB::getAffectsBipPairList().count(std::make_pair(firstInt, secondInt)) == 1;
		}
	}

	return affectsBipClauseHolds;
}

std::unordered_map<std::string, std::vector<std::string>> AffectsBipEvaluator::evaluateAffectsBipClause(QuerySuchThatClause& currSuchThatClause) {
	std::string firstArg = currSuchThatClause.getFirstArgument();
	ArgumentTypes firstArgType = currSuchThatClause.getFirstArgumentType();
	std::string secondArg = currSuchThatClause.getSecondArgument();
	ArgumentTypes secondArgType = currSuchThatClause.getSecondArgumentType();
	std::unordered_map<std::string, std::vector<std::string>> affectsBipClauseTable;

	if (firstArgType == ArgumentTypes::ANY) {
		if (secondArgType == ArgumentTypes::STMT || secondArgType == ArgumentTypes::PROG_LINE || secondArgType == ArgumentTypes::ASSIGN) {
			affectsBipClauseTable.emplace(secondArg, QueryEvaluatorUtil::convertIntSetToStringList(PKB::getAffectedBipList()));
		}
	}
	else if (firstArgType == ArgumentTypes::INT) {
		if (secondArgType == ArgumentTypes::STMT || secondArgType == ArgumentTypes::PROG_LINE || secondArgType == ArgumentTypes::ASSIGN) {
			int firstInt = std::stoi(firstArg);

			if (PKB::stmtHasAffectingBip(firstInt)) {
				std::vector<std::string> affectedBipStmtList = QueryEvaluatorUtil::convertIntSetToStringList(PKB::getAffectingBipOfStmt(firstInt));
				affectsBipClauseTable.emplace(secondArg, affectedBipStmtList);
			}
		}
	}
	else if (firstArgType == ArgumentTypes::STMT || firstArgType == ArgumentTypes::PROG_LINE || firstArgType == ArgumentTypes::ASSIGN) {
		if (secondArgType == ArgumentTypes::ANY) {
			affectsBipClauseTable.emplace(firstArg, QueryEvaluatorUtil::convertIntSetToStringList(PKB::getAffectingBipList()));
		}
		else if (secondArgType == ArgumentTypes::INT) {
			int secondInt = std::stoi(secondArg);

			if (PKB::stmtHasAffectedBipBy(secondInt)) {
				std::vector<std::string> affectsBipStmtList = QueryEvaluatorUtil::convertIntSetToStringList(PKB::getAffectedBipByOfStmt(secondInt));
				affectsBipClauseTable.emplace(firstArg, affectsBipStmtList);
			}
		}
		else if (secondArgType == ArgumentTypes::STMT || secondArgType == ArgumentTypes::PROG_LINE || secondArgType == ArgumentTypes::ASSIGN) {
			std::vector<std::string> affectsBipStmtList;
			std::vector<std::string> affectedBipStmtList;
			bool isSameStmtRef = firstArg == secondArg;

			for (int affecting : PKB::getAffectingBipList()) {
				std::unordered_set<int>& affectedBipStmtSet = PKB::getAffectingBipOfStmt(affecting);

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

			affectsBipClauseTable.emplace(firstArg, affectsBipStmtList);

			if (!isSameStmtRef) {
				affectsBipClauseTable.emplace(secondArg, affectedBipStmtList);
			}
		}
	}

	return affectsBipClauseTable;
}

int AffectsBipEvaluator::estimateAffectsBipClauseTableSize(QuerySuchThatClause& currSuchThatClause) {
	std::string firstArg = currSuchThatClause.getFirstArgument();
	ArgumentTypes firstArgType = currSuchThatClause.getFirstArgumentType();
	std::string secondArg = currSuchThatClause.getSecondArgument();
	ArgumentTypes secondArgType = currSuchThatClause.getSecondArgumentType();
	int estimatedTableSize = 0;

	if (firstArgType == ArgumentTypes::ANY) {
		if (secondArgType == ArgumentTypes::STMT || secondArgType == ArgumentTypes::PROG_LINE || secondArgType == ArgumentTypes::ASSIGN) {
			estimatedTableSize = PKB::getAffectedBipList().size();
		}
	}
	else if (firstArgType == ArgumentTypes::INT) {
		if (secondArgType == ArgumentTypes::STMT || secondArgType == ArgumentTypes::PROG_LINE || secondArgType == ArgumentTypes::ASSIGN) {
			int firstInt = std::stoi(firstArg);

			if (PKB::stmtHasAffectingBip(firstInt)) {
				estimatedTableSize = PKB::getAffectingBipOfStmt(firstInt).size();
			}
		}
	}
	else if (firstArgType == ArgumentTypes::STMT || firstArgType == ArgumentTypes::PROG_LINE || firstArgType == ArgumentTypes::ASSIGN) {
		if (secondArgType == ArgumentTypes::ANY) {
			estimatedTableSize = PKB::getAffectingBipList().size();
		}
		else if (secondArgType == ArgumentTypes::INT) {
			int secondInt = std::stoi(secondArg);

			if (PKB::stmtHasAffectedBipBy(secondInt)) {
				estimatedTableSize = PKB::getAffectedBipByOfStmt(secondInt).size();
			}
		}
		else if (secondArgType == ArgumentTypes::STMT || secondArgType == ArgumentTypes::PROG_LINE || secondArgType == ArgumentTypes::ASSIGN) {
			std::vector<std::string> affectsBipStmtList;
			std::vector<std::string> affectedBipStmtList;
			bool isSameStmtRef = firstArg == secondArg;

			for (int affecting : PKB::getAffectingBipList()) {
				std::unordered_set<int>& affectedBipStmtSet = PKB::getAffectingBipOfStmt(affecting);

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