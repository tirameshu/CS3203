#include "WhileEvaluator.h"

bool WhileEvaluator::checkWhileClauseHolds(QueryPatternClause& currPatternClause) {
	std::string firstArg = currPatternClause.getFirstArgument();
	ArgumentTypes firstArgType = currPatternClause.getFirstArgumentType();
	bool whileClauseHolds = false;

	if (firstArgType == ArgumentTypes::ANY || firstArgType == ArgumentTypes::VARIABLE) {
		whileClauseHolds = !PKB::getWhileUsesStmtList().empty();
	}
	else if (firstArgType == ArgumentTypes::STRING) {
		whileClauseHolds = PKB::getWhileUsedVarList().count(firstArg) == 1;
	}

	return whileClauseHolds;
}

std::unordered_map<std::string, std::vector<std::string>> WhileEvaluator::evaluateWhileClause(QueryPatternClause& currPatternClause) {
	std::string patternName = currPatternClause.getPatternName();
	std::string firstArg = currPatternClause.getFirstArgument();
	ArgumentTypes firstArgType = currPatternClause.getFirstArgumentType();
	std::unordered_map<std::string, std::vector<std::string>> patternClauseTable;

	if (firstArgType == ArgumentTypes::ANY) {
		patternClauseTable.emplace(patternName, QueryEvaluatorUtil::convertIntSetToStringList(PKB::getWhileUsesStmtList()));
	}
	else if (firstArgType == ArgumentTypes::STRING) {
		if (PKB::getVarToWhileStmtMap().count(firstArg) == 1) {
			patternClauseTable.emplace(patternName, QueryEvaluatorUtil::convertIntSetToStringList(PKB::getWhileStmtsUsingVar(firstArg)));
		}
	}
	else if (firstArgType == ArgumentTypes::VARIABLE) {
		std::vector<std::string> whileStmtList;
		std::vector<std::string> whileControlVarList;

		for (const auto& pair : PKB::getWhileStmtUsesMap()) {
			int stmt = pair.first;
			std::unordered_set<std::string> controlVars = pair.second;

			for (const std::string& var : controlVars) {
				whileStmtList.push_back(std::to_string(stmt));
				whileControlVarList.push_back(var);
			}
		}

		patternClauseTable.emplace(patternName, whileStmtList);
		patternClauseTable.emplace(firstArg, whileControlVarList);
	}

	return patternClauseTable;
}

int WhileEvaluator::estimateWhileClauseTableSize(QueryPatternClause& currPatternClause) {
	std::string patternName = currPatternClause.getPatternName();
	std::string firstArg = currPatternClause.getFirstArgument();
	ArgumentTypes firstArgType = currPatternClause.getFirstArgumentType();
	int estimatedTableSize = 0;

	if (firstArgType == ArgumentTypes::ANY) {
		estimatedTableSize = PKB::getWhileUsesStmtList().size();
	}
	else if (firstArgType == ArgumentTypes::STRING) {
		if (PKB::getVarToWhileStmtMap().count(firstArg) == 1) {
			estimatedTableSize = PKB::getWhileStmtsUsingVar(firstArg).size();
		}
	}
	else if (firstArgType == ArgumentTypes::VARIABLE) {
		estimatedTableSize = PKB::getTotalWhileUsedVars();
	}

	return estimatedTableSize;
}