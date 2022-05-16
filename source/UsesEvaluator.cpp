#include "UsesEvaluator.h"

bool UsesEvaluator::checkUsesClauseHolds(QuerySuchThatClause& currSuchThatClause) {
	std::string firstArg = currSuchThatClause.getFirstArgument();
	ArgumentTypes firstArgType = currSuchThatClause.getFirstArgumentType();
	std::string secondArg = currSuchThatClause.getSecondArgument();
	ArgumentTypes secondArgType = currSuchThatClause.getSecondArgumentType();
	bool usesClauseHolds = false;

	if (firstArgType == ArgumentTypes::STMT || firstArgType == ArgumentTypes::PROG_LINE) {
		if (secondArgType == ArgumentTypes::ANY || secondArgType == ArgumentTypes::VARIABLE) {
			usesClauseHolds = PKB::hasUses();
		}
		else if (secondArgType == ArgumentTypes::STRING) {
			usesClauseHolds = PKB::isVarUsed(secondArg);
		}
	}
	else if (firstArgType == ArgumentTypes::PROCEDURE) {
		if (secondArgType == ArgumentTypes::ANY || secondArgType == ArgumentTypes::VARIABLE) {
			usesClauseHolds = PKB::hasProcUses();
		}
		else if (secondArgType == ArgumentTypes::STRING) {
			usesClauseHolds = PKB::isVarUsed(secondArg);
		}
	}
	else if (firstArgType == ArgumentTypes::INT) {
		if (secondArgType == ArgumentTypes::ANY || secondArgType == ArgumentTypes::VARIABLE) {
			usesClauseHolds = PKB::isStmtUses(std::stoi(firstArg));
		}
		else if (secondArgType == ArgumentTypes::STRING) {
			int firstInt = std::stoi(firstArg);
			usesClauseHolds = PKB::isVarUsedInStmt(firstInt, secondArg);
		}
	}
	else if (firstArgType == ArgumentTypes::STRING) {
		if (secondArgType == ArgumentTypes::ANY || secondArgType == ArgumentTypes::VARIABLE) {
			usesClauseHolds = PKB::procUses(firstArg);
		}
		else if (secondArgType == ArgumentTypes::STRING) {
			usesClauseHolds = PKB::isVarUsedInProc(firstArg, secondArg);
		}
	}
	else {
		if (secondArgType == ArgumentTypes::ANY || secondArgType == ArgumentTypes::VARIABLE) {
			usesClauseHolds = checkStmtTypeIsUses(firstArgType);
		}
		else if (secondArgType == ArgumentTypes::STRING) {
			if (PKB::isVarUsed(secondArg)) {
				usesClauseHolds = checkStmtTypeIsUses(firstArgType, PKB::getStmtsUsingVar(secondArg));
			}
		}
	}

	return usesClauseHolds;
}

bool UsesEvaluator::checkStmtTypeIsUses(ArgumentTypes& stmtType) {
	bool stmtTypeIsUses = false;

	switch (stmtType) {
	case ArgumentTypes::PRINT:
		stmtTypeIsUses = !PKB::getPrintStmtList().empty();
		break;
	case ArgumentTypes::WHILE:
		stmtTypeIsUses = PKB::hasWhileUses();
		break;
	case ArgumentTypes::IF:
		stmtTypeIsUses = PKB::hasIfUses();
		break;
	case ArgumentTypes::ASSIGN:
		stmtTypeIsUses = PKB::hasAssignUses();
		break;
	case ArgumentTypes::CALL:
		stmtTypeIsUses = !PKB::getAllUsesCallStmts().empty();
		break;
	}

	return stmtTypeIsUses;
}

bool UsesEvaluator::checkStmtTypeIsUses(ArgumentTypes& stmtType, std::unordered_set<int>& usesStmtSet) {
	bool stmtTypeIsUses = false;
	std::unordered_set<int> stmtTypeSet = getStmtTypeIsUsesSet(stmtType);

	if (stmtTypeSet.size() < usesStmtSet.size()) {
		for (const int& stmt : stmtTypeSet) {
			if (usesStmtSet.count(stmt) == 1) {
				stmtTypeIsUses = true;
				break;
			}
		}
	}
	else {
		for (const int& stmt : usesStmtSet) {
			if (stmtTypeSet.count(stmt) == 1) {
				stmtTypeIsUses = true;
				break;
			}
		}
	}

	return stmtTypeIsUses;
}

std::unordered_map<std::string, std::vector<std::string>> UsesEvaluator::evaluateUsesClause(QuerySuchThatClause& currSuchThatClause) {
	std::string firstArg = currSuchThatClause.getFirstArgument();
	ArgumentTypes firstArgType = currSuchThatClause.getFirstArgumentType();
	std::string secondArg = currSuchThatClause.getSecondArgument();
	ArgumentTypes secondArgType = currSuchThatClause.getSecondArgumentType();
	std::unordered_map<std::string, std::vector<std::string>> usesClauseTable;

	if (firstArgType == ArgumentTypes::INT) {
		if (secondArgType == ArgumentTypes::VARIABLE) {
			int firstInt = std::stoi(firstArg);

			if (PKB::isStmtUses(firstInt)) {
				std::vector<std::string> usedVarList = QueryEvaluatorUtil::convertStringSetToStringList(PKB::getUsedVarsInStmt(firstInt));
				usesClauseTable.emplace(secondArg, usedVarList);
			}
		}
	}
	else if (firstArgType == ArgumentTypes::STRING) {
		if (secondArgType == ArgumentTypes::VARIABLE) {
			if (PKB::procUses(firstArg)) {
				std::vector<std::string> usedVarList = QueryEvaluatorUtil::convertStringSetToStringList(PKB::getUsedVarsInProc(firstArg));
				usesClauseTable.emplace(secondArg, usedVarList);
			}
		}
	}
	else if (firstArgType == ArgumentTypes::STMT || firstArgType == ArgumentTypes::PROG_LINE) {
		if (secondArgType == ArgumentTypes::ANY) {
			usesClauseTable.emplace(firstArg, getUsesStmtList(PKB::getStmtUsesMap()));
		}
		else if (secondArgType == ArgumentTypes::STRING) {
			if (PKB::isVarUsed(secondArg)) {
				std::vector<std::string> usesStmtList = QueryEvaluatorUtil::convertIntSetToStringList(PKB::getStmtsUsingVar(secondArg));
				usesClauseTable.emplace(firstArg, usesStmtList);
			}
		}
		else if (secondArgType == ArgumentTypes::VARIABLE) {
			std::vector<std::string> usesStmtList;
			std::vector<std::string> usedVarList;

			for (const std::string& stmt : getUsesStmtList(PKB::getStmtUsesMap())) {
				for (const std::string& var : PKB::getUsedVarsInStmt(std::stoi(stmt))) {
					usesStmtList.push_back(stmt);
					usedVarList.push_back(var);
				}
			}

			usesClauseTable.emplace(firstArg, usesStmtList);
			usesClauseTable.emplace(secondArg, usedVarList);
		}
	}
	else if (firstArgType == ArgumentTypes::PROCEDURE) {
		if (secondArgType == ArgumentTypes::ANY) {
			usesClauseTable.emplace(firstArg, QueryEvaluatorUtil::convertStringSetToStringList(PKB::getAllUsesProcs()));
		}
		else if (secondArgType == ArgumentTypes::STRING) {
			if (PKB::isVarUsed(secondArg)) {
				std::vector<std::string> usesProcList = QueryEvaluatorUtil::convertStringSetToStringList(PKB::getProcsUsingVar(secondArg));
				usesClauseTable.emplace(firstArg, usesProcList);
			}
		}
		else if (secondArgType == ArgumentTypes::VARIABLE) {
			std::vector<std::string> usesProcList;
			std::vector<std::string> usedVarList;

			for (const std::string& proc : PKB::getAllUsesProcs()) {
				for (const std::string& var : PKB::getUsedVarsInProc(proc)) {
					usesProcList.push_back(proc);
					usedVarList.push_back(var);
				}
			}

			usesClauseTable.emplace(firstArg, usesProcList);
			usesClauseTable.emplace(secondArg, usedVarList);
		}
	}
	else {
		if (secondArgType == ArgumentTypes::ANY) {
			std::vector<std::string> usesStmtList = QueryEvaluatorUtil::convertIntSetToStringList(getStmtTypeIsUsesSet(firstArgType));
			usesClauseTable.emplace(firstArg, usesStmtList);
		}
		else if (secondArgType == ArgumentTypes::STRING) {
			if (PKB::isVarUsed(secondArg)) {
				std::unordered_set<int> usesStmtSet = PKB::getStmtsUsingVar(secondArg);
				usesClauseTable.emplace(firstArg, getStmtTypeIsUsesList(firstArgType, usesStmtSet));
			}
		}
		else if (secondArgType == ArgumentTypes::VARIABLE) {
			std::vector<std::string> usesStmtList;
			std::vector<std::string> usedVarList;

			for (const int& stmt : getStmtTypeIsUsesSet(firstArgType)) {
				std::string stmtString = std::to_string(stmt);

				for (const std::string& var : PKB::getUsedVarsInStmt(stmt)) {
					usesStmtList.push_back(stmtString);
					usedVarList.push_back(var);
				}
			}

			usesClauseTable.emplace(firstArg, usesStmtList);
			usesClauseTable.emplace(secondArg, usedVarList);
		}
	}

	return usesClauseTable;
}

std::unordered_set<int> UsesEvaluator::getStmtTypeIsUsesSet(ArgumentTypes& stmtType) {
	std::unordered_set<int> stmtTypeIsUsesSet;

	switch (stmtType) {
	case ArgumentTypes::PRINT:
		stmtTypeIsUsesSet = PKB::getPrintStmtList();
		break;
	case ArgumentTypes::WHILE:
		stmtTypeIsUsesSet = PKB::getAllUsesWhileStmts();
		break;
	case ArgumentTypes::IF:
		stmtTypeIsUsesSet = PKB::getAllUsesIfStmts();
		break;
	case ArgumentTypes::ASSIGN:
		stmtTypeIsUsesSet = PKB::getAllUsesAssignStmts();
		break;
	case ArgumentTypes::CALL:
		stmtTypeIsUsesSet = PKB::getAllUsesCallStmts();
		break;
	}

	return stmtTypeIsUsesSet;
}

std::vector<std::string> UsesEvaluator::getStmtTypeIsUsesList(ArgumentTypes& stmtType, std::unordered_set<int>& usesStmtSet) {
	std::vector<std::string> stmtTypeIsUsesList;
	std::unordered_set<int> stmtTypeSet = getStmtTypeIsUsesSet(stmtType);

	if (stmtTypeSet.size() < usesStmtSet.size()) {
		for (const int& stmt : stmtTypeSet) {
			if (usesStmtSet.count(stmt) == 1) {
				stmtTypeIsUsesList.push_back(std::to_string(stmt));
			}
		}
	}
	else {
		for (const int& stmt : usesStmtSet) {
			if (stmtTypeSet.count(stmt) == 1) {
				stmtTypeIsUsesList.push_back(std::to_string(stmt));
			}
		}
	}

	return stmtTypeIsUsesList;
}

std::vector<std::string> UsesEvaluator::getUsesStmtList(std::unordered_map<int, std::unordered_set<std::string>>& stmtUsesMap) {
	std::vector<std::string> usesStmtList;

	for (const auto& pair : stmtUsesMap) {
		usesStmtList.push_back(std::to_string(pair.first));
	}

	return usesStmtList;
}

int UsesEvaluator::estimateUsesClauseTableSize(QuerySuchThatClause& currSuchThatClause) {
	std::string firstArg = currSuchThatClause.getFirstArgument();
	ArgumentTypes firstArgType = currSuchThatClause.getFirstArgumentType();
	std::string secondArg = currSuchThatClause.getSecondArgument();
	ArgumentTypes secondArgType = currSuchThatClause.getSecondArgumentType();
	int estimatedTableSize = 0;

	if (firstArgType == ArgumentTypes::INT) {
		if (secondArgType == ArgumentTypes::VARIABLE) {
			int firstInt = std::stoi(firstArg);

			if (PKB::isStmtUses(firstInt)) {
				estimatedTableSize = PKB::getUsedVarsInStmt(firstInt).size();
			}
		}
	}
	else if (firstArgType == ArgumentTypes::STRING) {
		if (secondArgType == ArgumentTypes::VARIABLE) {
			if (PKB::procUses(firstArg)) {
				estimatedTableSize = PKB::getUsedVarsInProc(firstArg).size();
			}
		}
	}
	else if (firstArgType == ArgumentTypes::STMT || firstArgType == ArgumentTypes::PROG_LINE) {
		if (secondArgType == ArgumentTypes::ANY) {
			estimatedTableSize = PKB::getStmtUsesMap().size();
		}
		else if (secondArgType == ArgumentTypes::STRING) {
			if (PKB::isVarUsed(secondArg)) {
				estimatedTableSize = PKB::getStmtsUsingVar(secondArg).size();
			}
		}
		else if (secondArgType == ArgumentTypes::VARIABLE) {
			estimatedTableSize = PKB::getTotalVarsUsedForStmts();
		}
	}
	else if (firstArgType == ArgumentTypes::PROCEDURE) {
		if (secondArgType == ArgumentTypes::ANY) {
			estimatedTableSize = PKB::getAllUsesProcs().size();
		}
		else if (secondArgType == ArgumentTypes::STRING) {
			if (PKB::isVarUsed(secondArg)) {
				estimatedTableSize = PKB::getProcsUsingVar(secondArg).size();
			}
		}
		else if (secondArgType == ArgumentTypes::VARIABLE) {
			estimatedTableSize = PKB::getTotalVarsUsedForProcs();
		}
	}
	else {
		if (secondArgType == ArgumentTypes::ANY) {
			estimatedTableSize = getStmtTypeIsUsesSet(firstArgType).size();
		}
		else if (secondArgType == ArgumentTypes::STRING) {
			if (PKB::isVarUsed(secondArg)) { // check this
				estimatedTableSize = std::min(getStmtTypeIsUsesSet(firstArgType).size(), PKB::getStmtsUsingVar(secondArg).size());
			}
		}
		else if (secondArgType == ArgumentTypes::VARIABLE) {
			estimatedTableSize = getTotalVarsUsedForStmtType(firstArgType);
		}
	}

	return estimatedTableSize;
}

int UsesEvaluator::getTotalVarsUsedForStmtType(ArgumentTypes& stmtType) {
	int totalVarsUsedForStmtType = 0;

	switch (stmtType) {
	case ArgumentTypes::PRINT:
		totalVarsUsedForStmtType = PKB::getTotalVarsUsedForPrintStmts();
		break;
	case ArgumentTypes::WHILE:
		totalVarsUsedForStmtType = PKB::getTotalVarsUsedForWhileStmts();
		break;
	case ArgumentTypes::IF:
		totalVarsUsedForStmtType = PKB::getTotalVarsUsedForIfStmts();
		break;
	case ArgumentTypes::ASSIGN:
		totalVarsUsedForStmtType = PKB::getTotalVarsUsedForAssignStmts();
		break;
	case ArgumentTypes::CALL:
		totalVarsUsedForStmtType = PKB::getTotalVarsUsedForCallStmts();
		break;
	}

	return totalVarsUsedForStmtType;
}