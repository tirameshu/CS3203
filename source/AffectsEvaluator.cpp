#include "AffectsEvaluator.h"

bool AffectsEvaluator::checkAffectsClauseHolds(QuerySuchThatClause currSuchThatClause) {
	std::string firstArg = currSuchThatClause.getFirstArgument();
	ArgumentTypes firstArgType = currSuchThatClause.getFirstArgumentType();
	std::string secondArg = currSuchThatClause.getSecondArgument();
	ArgumentTypes secondArgType = currSuchThatClause.getSecondArgumentType();
	bool affectsClauseHolds = false;

	if (firstArgType == ArgumentTypes::ANY || firstArgType == ArgumentTypes::STMT || firstArgType == ArgumentTypes::PROG_LINE || firstArgType == ArgumentTypes::ASSIGN) {
		if (secondArgType == ArgumentTypes::ANY || secondArgType == ArgumentTypes::STMT || secondArgType == ArgumentTypes::PROG_LINE || secondArgType == ArgumentTypes::ASSIGN) {
			// Affects (s, s)
			if (firstArg == secondArg && firstArgType != ArgumentTypes::ANY) {
				for (int stmt : PKB::getAffectingList()) {
					if (PKB::getAffectingOfStmt(stmt).count(stmt) == 1) {
						affectsClauseHolds = true;
						break;
					}
				}
			}
			else {
				affectsClauseHolds = PKB::programHasAffected();
			}
		}
		else if (secondArgType == ArgumentTypes::INT) {
			affectsClauseHolds = PKB::stmtHasAffectedBy(std::stoi(secondArg));
		}
	}
	else if (firstArgType == ArgumentTypes::INT) {
		if (secondArgType == ArgumentTypes::ANY || secondArgType == ArgumentTypes::STMT || secondArgType == ArgumentTypes::PROG_LINE || secondArgType == ArgumentTypes::ASSIGN) {
			affectsClauseHolds = PKB::stmtHasAffecting(std::stoi(firstArg));
		}
		else if (secondArgType == ArgumentTypes::INT) {
			int firstInt = std::stoi(firstArg);
			int secondInt = std::stoi(secondArg);
			
			affectsClauseHolds = PKB::getAffectsPairList().count(std::make_pair(firstInt, secondInt)) == 1;
		}
	}

	return affectsClauseHolds;
}

std::unordered_map<std::string, std::vector<std::string>> AffectsEvaluator::evaluateAffectsClause(QuerySuchThatClause& currSuchThatClause) {
	std::string firstArg = currSuchThatClause.getFirstArgument();
	ArgumentTypes firstArgType = currSuchThatClause.getFirstArgumentType();
	std::string secondArg = currSuchThatClause.getSecondArgument();
	ArgumentTypes secondArgType = currSuchThatClause.getSecondArgumentType();
	std::unordered_map<std::string, std::vector<std::string>> affectsClauseTable;

	if (firstArgType == ArgumentTypes::ANY) {
		if (secondArgType == ArgumentTypes::STMT || secondArgType == ArgumentTypes::PROG_LINE || secondArgType == ArgumentTypes::ASSIGN) {
			affectsClauseTable.emplace(secondArg, QueryEvaluatorUtil::convertIntSetToStringList(PKB::getAffectedList()));
		}
	}
	else if (firstArgType == ArgumentTypes::INT) {
		if (secondArgType == ArgumentTypes::STMT || secondArgType == ArgumentTypes::PROG_LINE || secondArgType == ArgumentTypes::ASSIGN) {
			int firstInt = std::stoi(firstArg);

			if (PKB::stmtHasAffecting(firstInt)) {
				std::vector<std::string> affectedStmtList = QueryEvaluatorUtil::convertIntSetToStringList(PKB::getAffectingOfStmt(firstInt));
				affectsClauseTable.emplace(secondArg, affectedStmtList);
			}
		}
	}
	else if (firstArgType == ArgumentTypes::STMT || firstArgType == ArgumentTypes::PROG_LINE || firstArgType == ArgumentTypes::ASSIGN) {
		if (secondArgType == ArgumentTypes::ANY) {
			affectsClauseTable.emplace(firstArg, QueryEvaluatorUtil::convertIntSetToStringList(PKB::getAffectingList()));
		}
		else if (secondArgType == ArgumentTypes::INT) {
			int secondInt = std::stoi(secondArg);

			if (PKB::stmtHasAffectedBy(secondInt)) {
				std::vector<std::string> affectsStmtList = QueryEvaluatorUtil::convertIntSetToStringList(PKB::getAffectedByOfStmt(secondInt));
				affectsClauseTable.emplace(firstArg, affectsStmtList);
			}
		}
		else if (secondArgType == ArgumentTypes::STMT || secondArgType == ArgumentTypes::PROG_LINE || secondArgType == ArgumentTypes::ASSIGN) {
			std::vector<std::string> affectsStmtList;
			std::vector<std::string> affectedStmtList;
			bool isSameStmtRef = firstArg == secondArg;

			for (int affecting : PKB::getAffectingList()) {
				std::unordered_set<int>& affectedStmtSet = PKB::getAffectingOfStmt(affecting);

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

			affectsClauseTable.emplace(firstArg, affectsStmtList);

			if (!isSameStmtRef) {
				affectsClauseTable.emplace(secondArg, affectedStmtList);
			}			
		}
	}

	return affectsClauseTable;
}

int AffectsEvaluator::estimateAffectsClauseTableSize(QuerySuchThatClause& currSuchThatClause) {
	std::string firstArg = currSuchThatClause.getFirstArgument();
	ArgumentTypes firstArgType = currSuchThatClause.getFirstArgumentType();
	std::string secondArg = currSuchThatClause.getSecondArgument();
	ArgumentTypes secondArgType = currSuchThatClause.getSecondArgumentType();
	int estimatedTableSize = 0;

	if (firstArgType == ArgumentTypes::ANY) {
		if (secondArgType == ArgumentTypes::STMT || secondArgType == ArgumentTypes::PROG_LINE || secondArgType == ArgumentTypes::ASSIGN) {
			estimatedTableSize = PKB::getAffectedList().size();
		}
	}
	else if (firstArgType == ArgumentTypes::INT) {
		if (secondArgType == ArgumentTypes::STMT || secondArgType == ArgumentTypes::PROG_LINE || secondArgType == ArgumentTypes::ASSIGN) {
			int firstInt = std::stoi(firstArg);

			if (PKB::stmtHasAffecting(firstInt)) {
				estimatedTableSize = PKB::getAffectingOfStmt(firstInt).size();
			}
		}
	}
	else if (firstArgType == ArgumentTypes::STMT || firstArgType == ArgumentTypes::PROG_LINE || firstArgType == ArgumentTypes::ASSIGN) {
		if (secondArgType == ArgumentTypes::ANY) {
			estimatedTableSize = PKB::getAffectingList().size();
		}
		else if (secondArgType == ArgumentTypes::INT) {
			int secondInt = std::stoi(secondArg);

			if (PKB::stmtHasAffectedBy(secondInt)) {
				estimatedTableSize = PKB::getAffectedByOfStmt(secondInt).size();
			}
		}
		else if (secondArgType == ArgumentTypes::STMT || secondArgType == ArgumentTypes::PROG_LINE || secondArgType == ArgumentTypes::ASSIGN) { // check this first
			std::vector<std::string> affectsStmtList;
			std::vector<std::string> affectedStmtList;
			bool isSameStmtRef = firstArg == secondArg;

			for (int affecting : PKB::getAffectingList()) {
				std::unordered_set<int>& affectedStmtSet = PKB::getAffectingOfStmt(affecting);

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