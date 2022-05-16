#include "IfStore.h"

std::unordered_map<int, std::unordered_set<std::string>> IfStore::ifStmtUsesMap; // map if stmt to all variables used in the condition
std::unordered_map<std::string, std::unordered_set<int>> IfStore::varToIfStmtMap; // map vars to if statements using 
std::unordered_set<std::string> IfStore::ifUsedVarList; // a list of variables used by if statements
std::unordered_set<int> IfStore::ifUsesStmtList; // a list of if stmt using a variable in its conditional expression
int IfStore::totalIfUsedVars; // total number of control variables used in if stmts

IfStore::IfStore() {}

void IfStore::clear() {
	ifStmtUsesMap.clear();
	varToIfStmtMap.clear();
	ifUsedVarList.clear();
	ifUsesStmtList.clear();
	totalIfUsedVars = 0;
}

// ============ SETTER METHODS ==============
bool IfStore::addIfUses(int stmt, std::unordered_set<std::string> varUsed) {
	if (varUsed.empty()) {
		return false;
	}

	// if an entry is made for the statement already, return false
	if (ifStmtUsesMap.count(stmt) != 0) {
		return false;
	}

	// add to var->if-stmt mapping to varToIfStmtMap
	// add variables used to ifUsedVarList
	for (const std::string& x : varUsed) {
		if (!varToIfStmtMap.emplace(x, std::unordered_set<int>{ stmt }).second) {
			varToIfStmtMap.at(x).emplace(stmt);
		}

		ifUsedVarList.emplace(x);
	}

	totalIfUsedVars += varUsed.size();

	return ifStmtUsesMap.emplace(stmt, varUsed).second && ifUsesStmtList.emplace(stmt).second;
}

// ============ GETTER METHODS ==============
std::unordered_map<int, std::unordered_set<std::string>> IfStore::getIfStmtUsesMap() {
	return ifStmtUsesMap;
}

std::unordered_map<std::string, std::unordered_set<int>> IfStore::getVarToIfStmtMap() {
	return varToIfStmtMap;
}

std::unordered_set<std::string> IfStore::getIfUsedVarList() {
	return ifUsedVarList;
}

std::unordered_set<int> IfStore::getIfUsesStmtList() {
	return ifUsesStmtList;
}

std::unordered_set<std::string> IfStore::getVarsUsedByIf(int stmt) {
	if (ifStmtUsesMap.count(stmt) == 0) {
		return {};
	}
	return ifStmtUsesMap.at(stmt);
}

std::unordered_set<int> IfStore::getIfStmtsUsingVar(std::string var) {
	if (varToIfStmtMap.count(var) == 0) {
		return {};
	}
	return varToIfStmtMap.at(var);
}

int IfStore::getTotalIfUsedVars() {
	return totalIfUsedVars;
}
