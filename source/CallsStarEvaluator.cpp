#include "CallsStarEvaluator.h"

bool CallsStarEvaluator::checkCallsStarClauseHolds(QuerySuchThatClause& currSuchThatClause) {
	std::string firstArg = currSuchThatClause.getFirstArgument();
	ArgumentTypes firstArgType = currSuchThatClause.getFirstArgumentType();
	std::string secondArg = currSuchThatClause.getSecondArgument();
	ArgumentTypes secondArgType = currSuchThatClause.getSecondArgumentType();
	bool callsStarClauseHolds = false;

	// Recursion is not allowed
	if (firstArg == secondArg && firstArgType != ArgumentTypes::ANY) {
		return callsStarClauseHolds;
	}

	if (firstArgType == ArgumentTypes::ANY || firstArgType == ArgumentTypes::PROCEDURE) {
		callsStarClauseHolds = CallsEvaluator::checkCallsClauseHolds(currSuchThatClause);
	}
	else if (firstArgType == ArgumentTypes::STRING) {
		if (secondArgType == ArgumentTypes::ANY || secondArgType == ArgumentTypes::PROCEDURE) {
			callsStarClauseHolds = CallsEvaluator::checkCallsClauseHolds(currSuchThatClause);
		}
		else if (secondArgType == ArgumentTypes::STRING) {
			callsStarClauseHolds = PKB::CallsTransitive(firstArg, secondArg);
		}
	}

	return callsStarClauseHolds;
}

std::unordered_map<std::string, std::vector<std::string>> CallsStarEvaluator::evaluateCallsStarClause(QuerySuchThatClause& currSuchThatClause) {
	std::string firstArg = currSuchThatClause.getFirstArgument();
	ArgumentTypes firstArgType = currSuchThatClause.getFirstArgumentType();
	std::string secondArg = currSuchThatClause.getSecondArgument();
	ArgumentTypes secondArgType = currSuchThatClause.getSecondArgumentType();
	std::unordered_map<std::string, std::vector<std::string>> callsStarClauseTable;

	// Recursion is not allowed
	if (firstArg == secondArg) {
		return callsStarClauseTable;
	}

	if (firstArgType == ArgumentTypes::ANY) {
		callsStarClauseTable = CallsEvaluator::evaluateCallsClause(currSuchThatClause);
	}
	else if (firstArgType == ArgumentTypes::STRING) {
		if (secondArgType == ArgumentTypes::PROCEDURE) {
			if (PKB::hasCallee(firstArg)) {
				std::vector<std::string> calledProcList = QueryEvaluatorUtil::convertStringSetToStringList(PKB::getTCalleesOfProc(firstArg));
				callsStarClauseTable.emplace(secondArg, calledProcList);
			}
		}
	}
	else if (firstArgType == ArgumentTypes::PROCEDURE) {
		if (secondArgType == ArgumentTypes::ANY) {
			callsStarClauseTable = CallsEvaluator::evaluateCallsClause(currSuchThatClause);
		}
		else if (secondArgType == ArgumentTypes::STRING) {
			if (PKB::hasCaller(secondArg)) {
				std::vector<std::string> callerProcList = QueryEvaluatorUtil::convertStringSetToStringList(PKB::getTCallersOfProc(secondArg));
				callsStarClauseTable.emplace(firstArg, callerProcList);
			}
		}
		else if (secondArgType == ArgumentTypes::PROCEDURE) {
			std::vector<std::string> callerProcList;
			std::vector<std::string> calledProcList;

			for (const std::string& caller : PKB::getListOfAllCallers()) {
				for (const std::string& callee : PKB::getTCalleesOfProc(caller)) {
					callerProcList.push_back(caller);
					calledProcList.push_back(callee);
				}
			}

			callsStarClauseTable.emplace(firstArg, callerProcList);
			callsStarClauseTable.emplace(secondArg, calledProcList);
		}
	}

	return callsStarClauseTable;
}

int CallsStarEvaluator::estimateCallsStarClauseTableSize(QuerySuchThatClause& currSuchThatClause) {
	std::string firstArg = currSuchThatClause.getFirstArgument();
	ArgumentTypes firstArgType = currSuchThatClause.getFirstArgumentType();
	std::string secondArg = currSuchThatClause.getSecondArgument();
	ArgumentTypes secondArgType = currSuchThatClause.getSecondArgumentType();
	int estimatedTableSize = 0;

	// Recursion is not allowed
	if (firstArg == secondArg) {
		return estimatedTableSize;
	}

	if (firstArgType == ArgumentTypes::ANY) {
		estimatedTableSize = CallsEvaluator::estimateCallsClauseTableSize(currSuchThatClause);
	}
	else if (firstArgType == ArgumentTypes::STRING) {
		if (secondArgType == ArgumentTypes::PROCEDURE) {
			if (PKB::hasCallee(firstArg)) {
				estimatedTableSize = PKB::getTCalleesOfProc(firstArg).size();
			}
		}
	}
	else if (firstArgType == ArgumentTypes::PROCEDURE) {
		if (secondArgType == ArgumentTypes::ANY) {
			estimatedTableSize = CallsEvaluator::estimateCallsClauseTableSize(currSuchThatClause);
		}
		else if (secondArgType == ArgumentTypes::STRING) {
			if (PKB::hasCaller(secondArg)) {
				estimatedTableSize = PKB::getTCallersOfProc(secondArg).size();
			}
		}
		else if (secondArgType == ArgumentTypes::PROCEDURE) {
			estimatedTableSize = PKB::getCallsStarPairList().size();
		}
	}

	return estimatedTableSize;
}