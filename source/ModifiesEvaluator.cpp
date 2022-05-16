#include "ModifiesEvaluator.h"

bool ModifiesEvaluator::checkModifiesClauseHolds(QuerySuchThatClause& currSuchThatClause) {
	std::string firstArg = currSuchThatClause.getFirstArgument();
	ArgumentTypes firstArgType = currSuchThatClause.getFirstArgumentType();
	std::string secondArg = currSuchThatClause.getSecondArgument();
	ArgumentTypes secondArgType = currSuchThatClause.getSecondArgumentType();
	bool modifiesClauseHolds = false;

	if (firstArgType == ArgumentTypes::STMT || firstArgType == ArgumentTypes::PROG_LINE) {
		if (secondArgType == ArgumentTypes::ANY || secondArgType == ArgumentTypes::VARIABLE) {
			modifiesClauseHolds = PKB::hasModifies();
		}
		else if (secondArgType == ArgumentTypes::STRING) {
			modifiesClauseHolds = PKB::isVarModified(secondArg);
		}
	}
	else if (firstArgType == ArgumentTypes::PROCEDURE) {
		if (secondArgType == ArgumentTypes::ANY || secondArgType == ArgumentTypes::VARIABLE) {
			modifiesClauseHolds = PKB::hasModifiesP();
		}
		else if (secondArgType == ArgumentTypes::STRING) {
			modifiesClauseHolds = PKB::isVarModified(secondArg);
		}
	}
	else if (firstArgType == ArgumentTypes::INT) {
		if (secondArgType == ArgumentTypes::ANY || secondArgType == ArgumentTypes::VARIABLE) {
			modifiesClauseHolds = PKB::isStmtModifies(std::stoi(firstArg));
		}
		else if (secondArgType == ArgumentTypes::STRING) {
			int firstInt = std::stoi(firstArg);
			modifiesClauseHolds = PKB::isVarModifiedInStmt(secondArg, firstInt);
		}
	}
	else if (firstArgType == ArgumentTypes::STRING) {
		if (secondArgType == ArgumentTypes::ANY || secondArgType == ArgumentTypes::VARIABLE) {
			modifiesClauseHolds = PKB::procHasModifies(firstArg);
		}
		else if (secondArgType == ArgumentTypes::STRING) {
			modifiesClauseHolds = PKB::isVarModifiedInProc(secondArg, firstArg);
		}
	}
	else {
		if (secondArgType == ArgumentTypes::ANY || secondArgType == ArgumentTypes::VARIABLE) {
			modifiesClauseHolds = checkStmtTypeIsModifies(firstArgType);
		}
		else if (secondArgType == ArgumentTypes::STRING) {
			if (PKB::isVarModified(secondArg)) {
				if (firstArgType == ArgumentTypes::ASSIGN) {
					modifiesClauseHolds = PKB::getVarAssignStmtList().count(secondArg) == 1;
				}
				else {
					modifiesClauseHolds = checkStmtTypeIsModifies(firstArgType, PKB::getStmtsModifyingVar(secondArg));
				}				
			}
		}
	}

	return modifiesClauseHolds;
}

bool ModifiesEvaluator::checkStmtTypeIsModifies(ArgumentTypes& stmtType) {
	bool stmtTypeIsModifies = false;

	switch (stmtType) {
	case ArgumentTypes::READ:
		stmtTypeIsModifies = !PKB::getReadStmtList().empty();
		break;
	case ArgumentTypes::WHILE:
		stmtTypeIsModifies = PKB::hasWhileModifies();
		break;
	case ArgumentTypes::IF:
		stmtTypeIsModifies = PKB::hasIfModifies();
		break;
	case ArgumentTypes::ASSIGN:
		stmtTypeIsModifies = !PKB::getAssignStmtList().empty();
		break;
	case ArgumentTypes::CALL:
		stmtTypeIsModifies = !PKB::getModifyingCallStmtList().empty();
		break;
	}

	return stmtTypeIsModifies;
}

bool ModifiesEvaluator::checkStmtTypeIsModifies(ArgumentTypes& stmtType, std::unordered_set<int>& modifiesStmtSet) {
	bool stmtTypeIsModifies = false;
	std::unordered_set<int> stmtTypeSet = getStmtTypeIsModifiesSet(stmtType);

	if (stmtTypeSet.size() < modifiesStmtSet.size()) {
		for (const int& stmt : stmtTypeSet) {
			if (modifiesStmtSet.count(stmt) == 1) {
				stmtTypeIsModifies = true;
				break;
			}
		}
	}
	else {
		for (const int& stmt : modifiesStmtSet) {
			if (stmtTypeSet.count(stmt) == 1) {
				stmtTypeIsModifies = true;
				break;
			}
		}
	}

	return stmtTypeIsModifies;
}

std::unordered_map<std::string, std::vector<std::string>> ModifiesEvaluator::evaluateModifiesClause(QuerySuchThatClause& currSuchThatClause) {
	std::string firstArg = currSuchThatClause.getFirstArgument();
	ArgumentTypes firstArgType = currSuchThatClause.getFirstArgumentType();
	std::string secondArg = currSuchThatClause.getSecondArgument();
	ArgumentTypes secondArgType = currSuchThatClause.getSecondArgumentType();
	std::unordered_map<std::string, std::vector<std::string>> modifiesClauseTable;

	if (firstArgType == ArgumentTypes::INT) {
		if (secondArgType == ArgumentTypes::VARIABLE) {
			int firstInt = std::stoi(firstArg);

			if (PKB::isStmtModifies(firstInt)) {
				std::vector<std::string> modifiedVarList = QueryEvaluatorUtil::convertStringSetToStringList(PKB::getModifiedVarsInStmt(firstInt));
				modifiesClauseTable.emplace(secondArg, modifiedVarList);
			}
		}
	}
	else if (firstArgType == ArgumentTypes::STRING) {
		if (secondArgType == ArgumentTypes::VARIABLE) {
			if (PKB::procHasModifies(firstArg)) {
				std::vector<std::string> modifiedVarList = QueryEvaluatorUtil::convertStringSetToStringList(PKB::getModifiedVarsInProc(firstArg));
				modifiesClauseTable.emplace(secondArg, modifiedVarList);
			}
		}
	}
	else if (firstArgType == ArgumentTypes::STMT || firstArgType == ArgumentTypes::PROG_LINE) {
		if (secondArgType == ArgumentTypes::ANY) {
			modifiesClauseTable.emplace(firstArg, QueryEvaluatorUtil::convertIntSetToStringList(PKB::getModifyingStmtList()));
		}
		else if (secondArgType == ArgumentTypes::STRING) {
			if (PKB::isVarModified(secondArg)) {
				std::vector<std::string> modifiesStmtList = QueryEvaluatorUtil::convertIntSetToStringList(PKB::getStmtsModifyingVar(secondArg));
				modifiesClauseTable.emplace(firstArg, modifiesStmtList);
			}
		}
		else if (secondArgType == ArgumentTypes::VARIABLE) {
			std::vector<std::string> modifiesStmtList;
			std::vector<std::string> modifiedVarList;

			for (const int& stmt : PKB::getModifyingStmtList()) {
				for (const std::string& var : PKB::getModifiedVarsInStmt(stmt)) {
					modifiesStmtList.push_back(std::to_string(stmt));
					modifiedVarList.push_back(var);
				}
			}

			modifiesClauseTable.emplace(firstArg, modifiesStmtList);
			modifiesClauseTable.emplace(secondArg, modifiedVarList);
		}
	}
	else if (firstArgType == ArgumentTypes::PROCEDURE) {
		if (secondArgType == ArgumentTypes::ANY) {
			modifiesClauseTable.emplace(firstArg, QueryEvaluatorUtil::convertStringSetToStringList(PKB::getModifiyingProcs()));
		}
		else if (secondArgType == ArgumentTypes::STRING) {
			if (PKB::isVarModified(secondArg)) {
				std::vector<std::string> modifiesProcList = QueryEvaluatorUtil::convertStringSetToStringList(PKB::getProcModifyingVar(secondArg));
				modifiesClauseTable.emplace(firstArg, modifiesProcList);
			}
		}
		else if (secondArgType == ArgumentTypes::VARIABLE) {
			std::vector<std::string> modifiesProcList;
			std::vector<std::string> modifiedVarList;

			for (const std::string& proc : PKB::getModifiyingProcs()) {
				for (const std::string& var : PKB::getModifiedVarsInProc(proc)) {
					modifiesProcList.push_back(proc);
					modifiedVarList.push_back(var);
				}
			}

			modifiesClauseTable.emplace(firstArg, modifiesProcList);
			modifiesClauseTable.emplace(secondArg, modifiedVarList);
		}
	}
	else {
		if (secondArgType == ArgumentTypes::ANY) {
			std::vector<std::string> modifiesStmtList = QueryEvaluatorUtil::convertIntSetToStringList(getStmtTypeIsModifiesSet(firstArgType));
			modifiesClauseTable.emplace(firstArg, modifiesStmtList);
		}
		else if (secondArgType == ArgumentTypes::STRING) {
			if (PKB::isVarModified(secondArg)) {
				if (firstArgType == ArgumentTypes::ASSIGN) {
					auto curr = PKB::getVarAssignStmtList();

					if (curr.count(secondArg) == 1) {
						modifiesClauseTable.emplace(firstArg, QueryEvaluatorUtil::convertIntSetToStringList(curr.at(secondArg)));
					}
				}
				else {
					std::unordered_set<int> modifiesStmtSet = PKB::getStmtsModifyingVar(secondArg);
					modifiesClauseTable.emplace(firstArg, getStmtTypeIsModifiesList(firstArgType, modifiesStmtSet));
				}
			}
		}
		else if (secondArgType == ArgumentTypes::VARIABLE) {
			std::vector<std::string> modifiesStmtList;
			std::vector<std::string> modifiedVarList;

			for (const int& stmt : getStmtTypeIsModifiesSet(firstArgType)) {
				for (const std::string& var : PKB::getModifiedVarsInStmt(stmt)) {
					modifiesStmtList.push_back(std::to_string(stmt));
					modifiedVarList.push_back(var);
				}
			}

			modifiesClauseTable.emplace(firstArg, modifiesStmtList);
			modifiesClauseTable.emplace(secondArg, modifiedVarList);
		}
	}

	return modifiesClauseTable;
}

std::unordered_set<int> ModifiesEvaluator::getStmtTypeIsModifiesSet(ArgumentTypes& stmtType) {
	std::unordered_set<int> stmtTypeIsModifiesSet;

	switch (stmtType) {
	case ArgumentTypes::READ:
		stmtTypeIsModifiesSet = PKB::getReadStmtList();
		break;
	case ArgumentTypes::WHILE:
		stmtTypeIsModifiesSet = PKB::getModifyingWhileStmtsList();
		break;
	case ArgumentTypes::IF:
		stmtTypeIsModifiesSet = PKB::getModifyingIfStmtList();
		break;
	case ArgumentTypes::ASSIGN:
		stmtTypeIsModifiesSet = PKB::getAssignStmtList();
		break;
	case ArgumentTypes::CALL:
		stmtTypeIsModifiesSet = PKB::getModifyingCallStmtList();
		break;
	}

	return stmtTypeIsModifiesSet;
}

std::vector<std::string> ModifiesEvaluator::getStmtTypeIsModifiesList(ArgumentTypes& stmtType, std::unordered_set<int>& modifiesStmtSet) {
	std::vector<std::string> stmtTypeIsModifiesList;
	std::unordered_set<int> stmtTypeSet = getStmtTypeIsModifiesSet(stmtType);

	if (stmtTypeSet.size() < modifiesStmtSet.size()) {
		for (const int& stmt : stmtTypeSet) {
			if (modifiesStmtSet.count(stmt) == 1) {
				stmtTypeIsModifiesList.push_back(std::to_string(stmt));
			}
		}
	}
	else {
		for (const int& stmt : modifiesStmtSet) {
			if (stmtTypeSet.count(stmt) == 1) {
				stmtTypeIsModifiesList.push_back(std::to_string(stmt));
			}
		}
	}

	return stmtTypeIsModifiesList;
}

int ModifiesEvaluator::estimateModifiesClauseTableSize(QuerySuchThatClause& currSuchThatClause) {
	std::string firstArg = currSuchThatClause.getFirstArgument();
	ArgumentTypes firstArgType = currSuchThatClause.getFirstArgumentType();
	std::string secondArg = currSuchThatClause.getSecondArgument();
	ArgumentTypes secondArgType = currSuchThatClause.getSecondArgumentType();
	int estimatedTableSize = 0;

	if (firstArgType == ArgumentTypes::INT) {
		if (secondArgType == ArgumentTypes::VARIABLE) {
			int firstInt = std::stoi(firstArg);

			if (PKB::isStmtModifies(firstInt)) {
				estimatedTableSize = PKB::getModifiedVarsInStmt(firstInt).size();
			}
		}
	}
	else if (firstArgType == ArgumentTypes::STRING) {
		if (secondArgType == ArgumentTypes::VARIABLE) {
			if (PKB::procHasModifies(firstArg)) {
				estimatedTableSize = PKB::getModifiedVarsInProc(firstArg).size();
			}
		}
	}
	else if (firstArgType == ArgumentTypes::STMT || firstArgType == ArgumentTypes::PROG_LINE) {
		if (secondArgType == ArgumentTypes::ANY) {
			estimatedTableSize = PKB::getModifyingStmtList().size();
		}
		else if (secondArgType == ArgumentTypes::STRING) {
			if (PKB::isVarModified(secondArg)) {
				estimatedTableSize = PKB::getStmtsModifyingVar(secondArg).size();
			}
		}
		else if (secondArgType == ArgumentTypes::VARIABLE) {
			estimatedTableSize = PKB::getTotalVarsModifiedForStmts();
		}
	}
	else if (firstArgType == ArgumentTypes::PROCEDURE) {
		if (secondArgType == ArgumentTypes::ANY) {
			estimatedTableSize = PKB::getModifiyingProcs().size();
		}
		else if (secondArgType == ArgumentTypes::STRING) {
			if (PKB::isVarModified(secondArg)) {
				estimatedTableSize = PKB::getProcModifyingVar(secondArg).size();
			}
		}
		else if (secondArgType == ArgumentTypes::VARIABLE) {
			estimatedTableSize = PKB::getTotalVarsModifiedForProcs();
		}
	}
	else {
		if (secondArgType == ArgumentTypes::ANY) {
			estimatedTableSize = getStmtTypeIsModifiesSet(firstArgType).size();
		}
		else if (secondArgType == ArgumentTypes::STRING) {
			if (PKB::isVarModified(secondArg)) {
				if (firstArgType == ArgumentTypes::ASSIGN) {
					auto& curr = PKB::getVarAssignStmtList();

					if (curr.count(secondArg) == 1) {
						estimatedTableSize = curr.at(secondArg).size();
					}
				}
				else { // check this
					estimatedTableSize = std::min(getStmtTypeIsModifiesSet(firstArgType).size(), PKB::getStmtsModifyingVar(secondArg).size());
				}
			}
		}
		else if (secondArgType == ArgumentTypes::VARIABLE) {
			estimatedTableSize = getTotalVarsModifiedForStmtType(firstArgType);
		}
	}

	return estimatedTableSize;
}

int ModifiesEvaluator::getTotalVarsModifiedForStmtType(ArgumentTypes& stmtType) {
	int totalVarsModifiedForStmtType = 0;

	switch (stmtType) {
	case ArgumentTypes::READ:
		totalVarsModifiedForStmtType = PKB::getTotalVarsModifiedForReadStmts();
		break;
	case ArgumentTypes::WHILE:
		totalVarsModifiedForStmtType = PKB::getTotalVarsModifiedForWhileStmts();
		break;
	case ArgumentTypes::IF:
		totalVarsModifiedForStmtType = PKB::getTotalVarsModifiedForIfStmts();
		break;
	case ArgumentTypes::ASSIGN:
		totalVarsModifiedForStmtType = PKB::getTotalVarsModifiedForAssignStmts();
		break;
	case ArgumentTypes::CALL:
		totalVarsModifiedForStmtType = PKB::getTotalVarsModifiedForCallStmts();
		break;
	}

	return totalVarsModifiedForStmtType;
}