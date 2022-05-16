#include "AssignStore.h"

std::unordered_set<int> AssignStore::assignStmtList; // a list of all assign statements
std::unordered_map<int, assignStruct> AssignStore::stmtAssignMap; // a map containing the assignStruct property regarding a specific statment
std::unordered_map<std::pair<std::string, std::string>, std::unordered_set<int>, StringPairHasher> AssignStore::varExpStmtList; // a map of pair(var, exp) to set of stmtNo
std::unordered_map <std::string, std::unordered_set<std::string>> AssignStore::varToExpMap; // a map of variable(LHS) to list of expressions(RHS)
std::unordered_map<std::string, std::unordered_set<int>> AssignStore::varAssignStmtList; // a map of variable to list of assign statements
std::unordered_map<std::string, std::unordered_set<int>> AssignStore::expAssignStmtList; // a map of expression to list of assign statements

AssignStore::AssignStore() {}

void AssignStore::clear() {
	assignStmtList.clear();
	stmtAssignMap.clear();
	varExpStmtList.clear();
	varToExpMap.clear();
	varAssignStmtList.clear();
	expAssignStmtList.clear();
}

// ============ SETTER METHODS ==============
bool AssignStore::addAssignStmt(int stmt) {
	return assignStmtList.emplace(stmt).second;
}

bool AssignStore::addAssign(int stmt, std::string var, std::string exp) {
	if (!stmtAssignMap.emplace(stmt, assignStruct{ var, exp }).second) {
		return false;
	}
	
	// add pair(var, exp) into varExpStmtList, and at the relevant stmt to the stmtList
	std::pair<std::string, std::string> varExp = std::make_pair(var, exp);
	if (!varExpStmtList.emplace(varExp, std::unordered_set<int>{ stmt }).second) {
		varExpStmtList.at(varExp).emplace(stmt);
	}

	// add mapping from variable to list of expressions
	if (!varToExpMap.emplace(var, std::unordered_set<std::string>{exp}).second) {
		varToExpMap.at(var).emplace(exp);
	}

	// add mapping from variable to list of stmtNo 
	if (!varAssignStmtList.emplace(var, std::unordered_set<int>{stmt}).second) {
		varAssignStmtList.at(var).emplace(stmt);
	}

	// add mapping from expression to list of stmtNo 
	if (!expAssignStmtList.emplace(exp, std::unordered_set<int>{stmt}).second) {
		expAssignStmtList.at(exp).emplace(stmt);
	}

	return 1;
}

// ============ GETTER METHODS ==============
std::unordered_set<int> AssignStore::getAssignStmtList() {
	return assignStmtList;
}

std::string AssignStore::getVariableOfStmt(int stmt) {
	if (stmtAssignMap.find(stmt) != stmtAssignMap.end()) {
		return stmtAssignMap.at(stmt).variable;
	}
	return "";
}

std::string AssignStore::getExpressionOfStmt(int stmt) {
	if (stmtAssignMap.find(stmt) != stmtAssignMap.end()) {
		return stmtAssignMap.at(stmt).expression;
	}
	return "";
}

std::unordered_map<std::pair<std::string, std::string>, std::unordered_set<int>, StringPairHasher> AssignStore::getVarExpStmtList() {
	return varExpStmtList;
}

std::unordered_map <std::string, std::unordered_set<std::string>> AssignStore::getVarToExpMap() {
	return varToExpMap;
}

std::unordered_map<std::string, std::unordered_set<int>> AssignStore::getVarAssignStmtList() {
	return varAssignStmtList;
}

std::unordered_map<std::string, std::unordered_set<int>> AssignStore::getExpAssignStmtList() {
	return expAssignStmtList;
}
