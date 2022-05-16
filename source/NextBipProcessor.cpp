#include "NextBipProcessor.h"
#include "PKB.h"

std::unordered_set<int> getLastStatement(std::string proc) {
	std::unordered_set<int> output;
	for (int i = 0; i < PKB::getProcedureStmtMap().at(proc).size(); i ++) {
		int stmt = PKB::getProcedureStmtMap().at(proc)[i];
		// Is last statement
		auto& curr = PKB::getAncestorsListOfStmt(stmt);
		bool doesNotHaveWhileAncestor = true;

		for (int stmt : curr) {
			if (PKB::getWhileStmtList().count(stmt) == 1) {
				doesNotHaveWhileAncestor = false;
				break;
			}
		}

		if (!PKB::stmtHasNext(stmt) || (PKB::getWhileStmtList().count(stmt) && !PKB::stmtHasFollower(stmt) && doesNotHaveWhileAncestor)) {
			// if is call statement
			if (PKB::getCallStmtList().count(stmt)) {
				std::string calledProc = PKB::getCallerStmtToProcMap().at(stmt);
				std::unordered_set<int> intermediate = getLastStatement(calledProc);
				for (int intermediateElem : intermediate) {
					output.emplace(intermediateElem);
				}
			}
			else {
				output.emplace(stmt);
			}
		}
	}

	return output;
}

void NextBipProcessor::calculateNextBip() {

	std::unordered_map<int, std::string> callToProc = PKB::getCallerStmtToProcMap();
	std::unordered_map<std::string, std::vector<int>> procToStmtList = PKB::getProcedureStmtMap();

	// Iterate through all the statement lists
	for (const int& currentStatementNumber : PKB::getStmtList()) {

		// if the statement is a call statement
		// Check by seeing if it exists in the stmtlist of call statements
		if (PKB::getCallStmtList().count(currentStatementNumber)) {
			std::string calledProc = callToProc[currentStatementNumber];

			// Connecting caller stmt to first statement of callee
			int firstStmtInCalledProc = procToStmtList[calledProc].front();
			PKB::addNextBip(currentStatementNumber, firstStmtInCalledProc, currentStatementNumber);
			//std::cout << std::to_string(currentStatementNumber) + "====>" + std::to_string(firstStmtInCalledProc) + " CALLED BY: " + std::to_string(currentStatementNumber) << std::endl;

			// Connecting last statement of callee to next statement of caller (if exist)
			std::unordered_set<int> lastStmtsInProc = getLastStatement(calledProc);

			// Check if the call statement has a next, otherwise ignore this step
			if (PKB::stmtHasNext(currentStatementNumber)) {
				// Finding the next statement after the call statement
				std::unordered_set<int> nextStmtOfCurrent = PKB::getNextOfStmt(currentStatementNumber); // By right, should only contain 1 element because call statements don't branch

				for (const int& elem : nextStmtOfCurrent) { // Set of 1 element
					for (const int& innerElem : lastStmtsInProc) { // May contain multiple element
						PKB::addNextBip(innerElem, elem, currentStatementNumber);
						//std::cout << std::to_string(innerElem) + "====>" + std::to_string(elem) + " CALLED BY: " + std::to_string(currentStatementNumber) << std::endl;
					}
				}
			}

			continue;
		}

		// If the statement does not have a next
		if (!PKB::stmtHasNext(currentStatementNumber)) {
			continue;
		}
		else {
			// If the statement has a next, just do as per normal
			for (const int& elem : PKB::getNextOfStmt(currentStatementNumber)) {
				PKB::addNextBip(currentStatementNumber, elem, 0);
				//std::cout << std::to_string(currentStatementNumber) + "====>" + std::to_string(elem) + " NOT INTERPROCEDURAL" << std::endl;
			}
		}
	}
}
