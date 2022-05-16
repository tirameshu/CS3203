#include "PatternEvaluator.h"

bool PatternEvaluator::checkPatternClauseHolds(QueryPatternClause& currPatternClause) {
	ArgumentTypes patternType = currPatternClause.getPatternType();
	bool patternClauseHolds = false;

	switch (patternType) {
	case ArgumentTypes::ASSIGN:
		patternClauseHolds = AssignEvaluator::checkAssignClauseHolds(currPatternClause);
		break;
	case ArgumentTypes::WHILE:
		patternClauseHolds = WhileEvaluator::checkWhileClauseHolds(currPatternClause);
		break;
	case ArgumentTypes::IF:
		patternClauseHolds = IfEvaluator::checkIfClauseHolds(currPatternClause);
		break;
	}

	return patternClauseHolds;
}

std::unordered_map<std::string, std::vector<std::string>> PatternEvaluator::evaluatePatternClause(QueryPatternClause& currPatternClause) {
	ArgumentTypes patternType = currPatternClause.getPatternType();
	std::unordered_map<std::string, std::vector<std::string>> patternClauseTable;

	switch (patternType) {
	case ArgumentTypes::ASSIGN:
		patternClauseTable = AssignEvaluator::evaluateAssignClause(currPatternClause);
		break;
	case ArgumentTypes::WHILE:
		patternClauseTable = WhileEvaluator::evaluateWhileClause(currPatternClause);
		break;
	case ArgumentTypes::IF:
		patternClauseTable = IfEvaluator::evaluateIfClause(currPatternClause);
		break;
	}

	return patternClauseTable;
}

int PatternEvaluator::estimatePatternClauseTableSize(QueryPatternClause& currPatternClause) {
	ArgumentTypes patternType = currPatternClause.getPatternType();
	int estimatedTableSize = 0;

	switch (patternType) {
	case ArgumentTypes::ASSIGN:
		estimatedTableSize = AssignEvaluator::estimateAssignClauseTableSize(currPatternClause);
		break;
	case ArgumentTypes::WHILE:
		estimatedTableSize = WhileEvaluator::estimateWhileClauseTableSize(currPatternClause);
		break;
	case ArgumentTypes::IF:
		estimatedTableSize = IfEvaluator::estimateIfClauseTableSize(currPatternClause);
		break;
	}

	return estimatedTableSize;
}