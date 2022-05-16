#include "IfEvaluator.h"

bool IfEvaluator::checkIfClauseHolds(QueryPatternClause& currPatternClause) {
	std::string firstArg = currPatternClause.getFirstArgument();
	ArgumentTypes firstArgType = currPatternClause.getFirstArgumentType();
	bool ifClauseHolds = false;

	if (firstArgType == ArgumentTypes::ANY || firstArgType == ArgumentTypes::VARIABLE) {
		ifClauseHolds = !PKB::getIfUsesStmtList().empty();
	}
	else if (firstArgType == ArgumentTypes::STRING) {
		ifClauseHolds = PKB::getIfUsedVarList().count(firstArg) == 1;
	}

	return ifClauseHolds;
}

std::unordered_map<std::string, std::vector<std::string>> IfEvaluator::evaluateIfClause(QueryPatternClause& currPatternClause) {
	std::string patternName = currPatternClause.getPatternName();
	std::string firstArg = currPatternClause.getFirstArgument();
	ArgumentTypes firstArgType = currPatternClause.getFirstArgumentType();
	std::unordered_map<std::string, std::vector<std::string>> patternClauseTable;

	if (firstArgType == ArgumentTypes::ANY) {
		patternClauseTable.emplace(patternName, QueryEvaluatorUtil::convertIntSetToStringList(PKB::getIfUsesStmtList()));
	}
	else if (firstArgType == ArgumentTypes::STRING) {
		if (PKB::getVarToIfStmtMap().count(firstArg) == 1) {
			patternClauseTable.emplace(patternName, QueryEvaluatorUtil::convertIntSetToStringList(PKB::getIfStmtsUsingVar(firstArg)));
		}
	}
	else if (firstArgType == ArgumentTypes::VARIABLE) {
		std::vector<std::string> ifStmtList;
		std::vector<std::string> ifControlVarList;

		for (const auto& pair : PKB::getIfStmtUsesMap()) {
			int stmt = pair.first;
			std::unordered_set<std::string> controlVars = pair.second;

			for (const std::string& var : controlVars) {
				ifStmtList.push_back(std::to_string(stmt));
				ifControlVarList.push_back(var);
			}
		}

		patternClauseTable.emplace(patternName, ifStmtList);
		patternClauseTable.emplace(firstArg, ifControlVarList);
	}

	return patternClauseTable;
}

int IfEvaluator::estimateIfClauseTableSize(QueryPatternClause& currPatternClause) {
	std::string patternName = currPatternClause.getPatternName();
	std::string firstArg = currPatternClause.getFirstArgument();
	ArgumentTypes firstArgType = currPatternClause.getFirstArgumentType();
	int estimatedTableSize = 0;

	if (firstArgType == ArgumentTypes::ANY) {
		estimatedTableSize = PKB::getIfUsesStmtList().size();
	}
	else if (firstArgType == ArgumentTypes::STRING) {
		if (PKB::getVarToIfStmtMap().count(firstArg) == 1) {
			estimatedTableSize = PKB::getIfStmtsUsingVar(firstArg).size();
		}
	}
	else if (firstArgType == ArgumentTypes::VARIABLE) {
		estimatedTableSize = PKB::getTotalIfUsedVars();
	}

	return estimatedTableSize;
}