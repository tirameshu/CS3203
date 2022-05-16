#include "CallsEvaluator.h"

bool CallsEvaluator::checkCallsClauseHolds(QuerySuchThatClause& currSuchThatClause) {
	std::string firstArg = currSuchThatClause.getFirstArgument();
	ArgumentTypes firstArgType = currSuchThatClause.getFirstArgumentType();
	std::string secondArg = currSuchThatClause.getSecondArgument();
	ArgumentTypes secondArgType = currSuchThatClause.getSecondArgumentType();
	bool callsClauseHolds = false;

	// Recursion is not allowed
	if (firstArg == secondArg && firstArgType != ArgumentTypes::ANY) {
		return callsClauseHolds;
	}

	if (firstArgType == ArgumentTypes::ANY || firstArgType == ArgumentTypes::PROCEDURE) {
		if (secondArgType == ArgumentTypes::ANY || secondArgType == ArgumentTypes::PROCEDURE) {
			callsClauseHolds = PKB::hasCall();
		}
		else if (secondArgType == ArgumentTypes::STRING) {
			callsClauseHolds = PKB::hasCaller(secondArg);
		}
	}
	else if (firstArgType == ArgumentTypes::STRING) {
		if (secondArgType == ArgumentTypes::ANY || secondArgType == ArgumentTypes::PROCEDURE) {
			callsClauseHolds = PKB::hasCallee(firstArg);
		}
		else if (secondArgType == ArgumentTypes::STRING) {
			callsClauseHolds = PKB::Calls(firstArg, secondArg);
		}
	}

	return callsClauseHolds;
}

std::unordered_map<std::string, std::vector<std::string>> CallsEvaluator::evaluateCallsClause(QuerySuchThatClause& currSuchThatClause) {
	std::string firstArg = currSuchThatClause.getFirstArgument();
	ArgumentTypes firstArgType = currSuchThatClause.getFirstArgumentType();
	std::string secondArg = currSuchThatClause.getSecondArgument();
	ArgumentTypes secondArgType = currSuchThatClause.getSecondArgumentType();
	std::unordered_map<std::string, std::vector<std::string>> callsClauseTable;

	// Recursion is not allowed
	if (firstArg == secondArg) {
		return callsClauseTable;
	}

	if (firstArgType == ArgumentTypes::ANY) {
		if (secondArgType == ArgumentTypes::PROCEDURE) {
			callsClauseTable.emplace(secondArg, QueryEvaluatorUtil::convertStringSetToStringList(PKB::getListOfAllCallees()));
		}
	}
	else if (firstArgType == ArgumentTypes::STRING) {
		if (secondArgType == ArgumentTypes::PROCEDURE) {
			if (PKB::hasCallee(firstArg)) {
				std::vector<std::string> calledProcList = QueryEvaluatorUtil::convertStringSetToStringList(PKB::getCalleesOfProc(firstArg));
				callsClauseTable.emplace(secondArg, calledProcList);
			}
		}
	}
	else if (firstArgType == ArgumentTypes::PROCEDURE) {
		if (secondArgType == ArgumentTypes::ANY) {
			callsClauseTable.emplace(firstArg, QueryEvaluatorUtil::convertStringSetToStringList(PKB::getListOfAllCallers()));
		}
		else if (secondArgType == ArgumentTypes::STRING) {
			if (PKB::hasCaller(secondArg)) {
				std::vector<std::string> callerProcList = QueryEvaluatorUtil::convertStringSetToStringList(PKB::getCallersOfProc(secondArg));
				callsClauseTable.emplace(firstArg, callerProcList);
			}
		}
		else if (secondArgType == ArgumentTypes::PROCEDURE) {
			std::vector<std::string> callerProcList;
			std::vector<std::string> calledProcList;

			for (const std::string& caller : PKB::getListOfAllCallers()) {
				for (const std::string& callee : PKB::getCalleesOfProc(caller)) {
					callerProcList.push_back(caller);
					calledProcList.push_back(callee);
				}
			}

			callsClauseTable.emplace(firstArg, callerProcList);
			callsClauseTable.emplace(secondArg, calledProcList);
		}
	}

	return callsClauseTable;
}

int CallsEvaluator::estimateCallsClauseTableSize(QuerySuchThatClause& currSuchThatClause) {
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
		if (secondArgType == ArgumentTypes::PROCEDURE) {
			estimatedTableSize = PKB::getListOfAllCallees().size();
		}
	}
	else if (firstArgType == ArgumentTypes::STRING) {
		if (secondArgType == ArgumentTypes::PROCEDURE) {
			if (PKB::hasCallee(firstArg)) {
				estimatedTableSize = PKB::getCalleesOfProc(firstArg).size();
			}
		}
	}
	else if (firstArgType == ArgumentTypes::PROCEDURE) {
		if (secondArgType == ArgumentTypes::ANY) {
			estimatedTableSize = PKB::getListOfAllCallers().size();
		}
		else if (secondArgType == ArgumentTypes::STRING) {
			if (PKB::hasCaller(secondArg)) {
				estimatedTableSize = PKB::getCallersOfProc(secondArg).size();
			}
		}
		else if (secondArgType == ArgumentTypes::PROCEDURE) {
			estimatedTableSize = PKB::getCallsPairList().size();
		}
	}

	return estimatedTableSize;
}