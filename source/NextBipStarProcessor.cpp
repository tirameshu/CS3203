#include <stack>
#include <queue>
#include "PKB.h"
#include "NextBipStarProcessor.h"

std::unordered_set<int> getReturnOfCall(int callStmt) {
	std::unordered_set<int> output;
	std::string procName = PKB::getCallerStmtToProcMap().at(callStmt);
	std::vector<int> procStmtList = PKB::getProcedureStmtMap().at(procName);
	for (int i = 0; i < procStmtList.size(); i++) {
		auto& curr = PKB::getAncestorsListOfStmt(procStmtList[i]);
		bool doesNotHaveWhileAncestor = true;

		for (int stmt : curr) {
			if (PKB::getWhileStmtList().count(stmt) == 1) {
				doesNotHaveWhileAncestor = false;
				break;
			}
		}

		if (!PKB::stmtHasNext(procStmtList[i]) || (PKB::getWhileStmtList().count(procStmtList[i]) && !PKB::stmtHasFollower(procStmtList[i]) && doesNotHaveWhileAncestor)) {
			
			// If the stmt is a call stmt, call recursively
			if (PKB::getCallStmtList().count(procStmtList[i])) {
				
				// Get all the leaf nodes of the caller stmt
				for (int calledNode : getReturnOfCall(procStmtList[i])) {
					output.emplace(calledNode);
				}
			}
			// Else not a call stmt
			else {
				output.emplace(procStmtList[i]);
			}
		}
	}

	return output;
}

void pairAllCalledStmt(int originalStmt, int callStmt) {
	// Getting the procedure's statements
	std::string procName = PKB::getCallerStmtToProcMap().at(callStmt);
	std::vector<int> procStmtList = PKB::getProcedureStmtMap().at(procName);
	// iterating through all the stmt in the stmtlist
	for (int i = 0; i < procStmtList.size(); i++) {
		// Pair the original stmt with the call stmt first
		PKB::addAllNextBip(originalStmt, {procStmtList[i]});
		//std::cout << "CALL NEXTBIPSTAR: " + std::to_string(originalStmt) + " ===> " + std::to_string(procStmtList[i]) << "\n";

		// If the stmt is call stmt, call recursively
		if (PKB::getCallStmtList().count(procStmtList[i])) {
			pairAllCalledStmt(originalStmt, procStmtList[i]);
		}
	}
}

void NextBipStarProcessor::calculateNextBipStar() {
	// Iterate through all statements
	for (int stmt : PKB::getStmtList()) { 
		std::stack<int> frontier;
		std::unordered_set<int> visited;

		if (PKB::getCallStmtList().count(stmt)) {
			// Adding all the stmts recursively from the call stmt
			pairAllCalledStmt(stmt, stmt);

			if (PKB::stmtHasNext(stmt)) { // If not last call statement
				// Should only be one element
				for (int next : PKB::getNextOfStmt(stmt)) {
					frontier.push(next);
				}
			}
			// Else if the call statement is the last statement in the procedure
			else { 
				// Get all statements that call this statement
				std::string currentProcName = PKB::getCallStmtToProcedureMap().at(stmt);
				// Check if the procedure has anything that calls it
				if (PKB::getListOfAllCallees().count(currentProcName)) {
					std::unordered_set<int> callerStmtOfCurrProc = PKB::getProcToCallerStmtsMap().at(currentProcName);
					
					// Get all return nodes
					std::unordered_set<int> returnNodes = getReturnOfCall(stmt);
					for (int returnNode : returnNodes) {
						for (int nextBipOfReturnNode : PKB::getNextBipOfStmt(returnNode)) {
							for (int nextBipIncoming : PKB::getNextBipListOfIncomingOfStmt(nextBipOfReturnNode)) {
								if (callerStmtOfCurrProc.count(nextBipIncoming)) {
									frontier.push(nextBipOfReturnNode);
									break;
								}
							}
						}
					}
				}
			}
		}
		else {
			// Add all next to frontier
			for (int next : PKB::getNextBipOfStmt(stmt)) {
				frontier.push(next);
			}
		}

		/*=== ITERATE ===*/
		while (!frontier.empty()) {

			int top = frontier.top();
			frontier.pop();

			// If call stmt
			if (PKB::getCallStmtList().count(top)) {
				// Adding all the stmts recursively from the call stmt
				pairAllCalledStmt(stmt, top);
				
				if (PKB::stmtHasNext(top)) { // If not last call statement
				// Should only be one element
					for (int next : PKB::getNextOfStmt(top)) {
						frontier.push(next);
					}
				}
				// Else if the call statement is the last statement in the procedure
				else {
					// Get all statements that call this statement
					std::string currentProcName = PKB::getCallStmtToProcedureMap().at(top);
					// Check if the procedure has anything that calls it
					if (PKB::getListOfAllCallees().count(currentProcName)) {
						std::unordered_set<int> callerStmtOfCurrProc = PKB::getProcToCallerStmtsMap().at(currentProcName);

						// Get all return nodes
						std::unordered_set<int> returnNodes = getReturnOfCall(top);
						for (int returnNode : returnNodes) {
							for (int nextBipOfReturnNode : PKB::getNextBipOfStmt(returnNode)) {
								for (int nextBipIncoming : PKB::getNextBipListOfIncomingOfStmt(nextBipOfReturnNode)) {
									if (callerStmtOfCurrProc.count(nextBipIncoming)) {
										frontier.push(nextBipOfReturnNode);
										break;
									}
								}
							}
						}
					}
				}
			}

			// Else normal statement
			else {
				for (int next : PKB::getNextBipOfStmt(top)) {
					// push to frontier
					if (!visited.count(next)) {
						frontier.push(next);
					}
				}
			}

			visited.emplace(top);
			PKB::addAllNextBip(stmt, { top });
			//std::cout << "NEXTBIPSTAR: " + std::to_string(stmt) + " ===> " + std::to_string(top) << "\n";
		}
	}
}
