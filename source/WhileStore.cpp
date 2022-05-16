#include "WhileStore.h"

std::unordered_map<int, std::unordered_set<std::string>> WhileStore::whileStmtUsesMap; // map while stmt to all variables used in the condition
std::unordered_map<std::string, std::unordered_set<int>> WhileStore::varToWhileStmtMap; // map vars to while statements using 
std::unordered_set<std::string> WhileStore::whileUsedVarList; // a list of variables used by while statements
std::unordered_set<int> WhileStore::whileUsesStmtList; // a list of while stmt using a variable in its conditional expression
int WhileStore::totalWhileUsedVars; // total number of control variables used in while stmts

WhileStore::WhileStore() {}

void WhileStore::clear() {
	whileStmtUsesMap.clear();
	varToWhileStmtMap.clear();
	whileUsedVarList.clear();
	whileUsesStmtList.clear();
	totalWhileUsedVars = 0;
}

// ============ SETTER METHODS ==============
bool WhileStore::addWhileUses(int stmt, std::unordered_set<std::string> varUsed) {
	if (varUsed.empty()) {
		return false;
	}

	// if an entry is made for the statement already, return false
	if (whileStmtUsesMap.count(stmt) != 0) {
		return false;
	}

	// add to var->while-stmt mapping to varToWhileStmtMap
	// add variables used to whileUsedVarList
	for (const std::string& x : varUsed) {
		if (!varToWhileStmtMap.emplace(x, std::unordered_set<int>{ stmt }).second) {
			varToWhileStmtMap.at(x).emplace(stmt);
		}

		whileUsedVarList.emplace(x);
	}

	totalWhileUsedVars += varUsed.size();

	return whileStmtUsesMap.emplace(stmt, varUsed).second && whileUsesStmtList.emplace(stmt).second;
}

// ============ GETTER METHODS ==============
std::unordered_map<int, std::unordered_set<std::string>> WhileStore::getWhileStmtUsesMap() {
	return whileStmtUsesMap;
}

std::unordered_map<std::string, std::unordered_set<int>> WhileStore::getVarToWhileStmtMap() {
	return varToWhileStmtMap;
}

std::unordered_set<std::string> WhileStore::getWhileUsedVarList() {
	return whileUsedVarList;
}

std::unordered_set<int> WhileStore::getWhileUsesStmtList() {
	return whileUsesStmtList;
}

std::unordered_set<std::string> WhileStore::getVarsUsedByWhile(int stmt) {
	if (whileStmtUsesMap.count(stmt) == 0) {
		return {};
	}
	return whileStmtUsesMap.at(stmt);
}

std::unordered_set<int> WhileStore::getWhileStmtsUsingVar(std::string var) {
	if (varToWhileStmtMap.count(var) == 0) {
		return {};
	}
	return varToWhileStmtMap.at(var);
}

int WhileStore::getTotalWhileUsedVars() {
	return totalWhileUsedVars;
}
