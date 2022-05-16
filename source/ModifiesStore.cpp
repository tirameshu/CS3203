#include "ModifiesStore.h"
#include <string>
#include <assert.h>

//ModifiesS
std::unordered_map<int, std::unordered_set<std::string>> ModifiesStore::stmtModifiesMap; // a map from stmtNo to set of variables modified by stmt
std::unordered_map<std::string, std::unordered_set<int>> ModifiesStore::varModifiedMap; // a map from variable to set of stmtNo that modifies it
std::unordered_set<std::string> ModifiesStore::allModifiedVarList; // a list of all modified variables
std::unordered_set<std::string> ModifiesStore::allReadModifiedVarList; // a list of all modified variables by read statements
std::unordered_set<int> ModifiesStore::modifyingStmtList; // a list of all modifying statements stmtNo (contains all assign stmt, read stmt, and if/while stmt that has modifies)
std::unordered_set<int> ModifiesStore::readStmtList; // a list of all read statements stmtNo
std::unordered_set<int> ModifiesStore::modifyingIfStmtList; // a list of all if modifying statements stmtNo
std::unordered_set<int> ModifiesStore::modifyingWhileStmtList; // a list of all while modifying statements stmtNo

 // read statement mappings
std::unordered_map<int, std::string> ModifiesStore::readStmtToVarMap; // a map of read stmt to variable used
std::unordered_map < std::string, std::unordered_set<int>> ModifiesStore::varToReadStmtMap; // a map of variable used to a list of read stmts using it

//ModifiesP
std::unordered_map<std::string, std::unordered_set<std::string>> ModifiesStore::procModifiesMap; //a map of all proc name to variables modified in proc
std::unordered_map<std::string, std::unordered_set<std::string>> ModifiesStore::varModifiedProcMap; //a map of all vars to the procs modifiying them
std::unordered_set<std::string> ModifiesStore::allModifiyingProcs; //a list of all modifiying proc names

std::unordered_set<int> ModifiesStore::modifyingCallStmtList; //a list of all call statements that modify some var

int ModifiesStore::totalVarsModifiedForStmts; // total number of modified variables for stmts
int ModifiesStore::totalVarsModifiedForReadStmts; // total number of modified variables for read stmts
int ModifiesStore::totalVarsModifiedForAssignStmts; // total number of modified variables for assign stmts
int ModifiesStore::totalVarsModifiedForIfStmts; // total number of modified variables for if stmts
int ModifiesStore::totalVarsModifiedForWhileStmts; // total number of modified variables for while stmts
int ModifiesStore::totalVarsModifiedForCallStmts; // total number of modified variables for call stmts
int ModifiesStore::totalVarsModifiedForProcs; // total number of modified variables for procedures

ModifiesStore::ModifiesStore() {}

void ModifiesStore::clear() {
	stmtModifiesMap.clear();
	varModifiedMap.clear();
	allModifiedVarList.clear();
	allReadModifiedVarList.clear();
	modifyingStmtList.clear();
	readStmtList.clear();
	modifyingIfStmtList.clear();
	modifyingWhileStmtList.clear();
	procModifiesMap.clear();
	varModifiedProcMap.clear();
	allModifiyingProcs.clear();
	modifyingCallStmtList.clear();
	readStmtToVarMap.clear();
	varToReadStmtMap.clear();
	totalVarsModifiedForStmts = 0;
	totalVarsModifiedForReadStmts = 0;
	totalVarsModifiedForAssignStmts = 0;
	totalVarsModifiedForIfStmts = 0;
	totalVarsModifiedForWhileStmts = 0;
	totalVarsModifiedForCallStmts = 0;
	totalVarsModifiedForProcs = 0;
}

// ============ SETTER METHODS ==============
// to be called by Parser, used by all assign/read stmt
bool ModifiesStore::addModifies(int stmtNo, int stmtType, std::string var) {
	//add stmtNo and var to stmt -> var map
	if (!stmtModifiesMap.emplace(stmtNo, std::unordered_set<std::string>{ var }).second) {
		stmtModifiesMap.at(stmtNo).emplace(var);
	}
	
	//add var and stmtNo to var -> stmtNo
	if (!varModifiedMap.emplace(var, std::unordered_set<int>{ stmtNo }).second) {
		varModifiedMap.at(var).emplace(stmtNo);
	}

	if (stmtType == READ) {
		addReadStmt(stmtNo);
		readStmtToVarMap.emplace(stmtNo, var);
		if (!varToReadStmtMap.emplace(var, std::unordered_set<int>{stmtNo}).second) {
			varToReadStmtMap.at(var).emplace(stmtNo);
		}
		allReadModifiedVarList.emplace(var);

		totalVarsModifiedForReadStmts += 1;
	}
	else if (stmtType == ASSIGN) {
		totalVarsModifiedForAssignStmts += 1;
	}

	modifyingStmtList.emplace(stmtNo);

	totalVarsModifiedForStmts += 1;

	return allModifiedVarList.emplace(var).second;
}

// to be called by DesignExtractor
bool ModifiesStore::addModifiesToContainer(int stmtNo, int stmtType, std::unordered_set<std::string> listOfVar) {
	// update the list of variables mapped to if/while stmtNo (to contain all variables used by its children stmts)
	int numVarsModified = 0;

	if (stmtModifiesMap.count(stmtNo) == 0) {
		numVarsModified += listOfVar.size();
	}
	else {
		auto& curr = stmtModifiesMap.at(stmtNo);

		for (const std::string& var : listOfVar) {
			if (curr.count(var) == 0) {
				numVarsModified += 1;
			}
		}
	}

	totalVarsModifiedForStmts += numVarsModified;

	if (!stmtModifiesMap.emplace(stmtNo,listOfVar).second) {
		stmtModifiesMap.at(stmtNo).insert(listOfVar.begin(), listOfVar.end());
	}	

	//add to varModifiedMap 
	for (auto& x : listOfVar) {
		if (!varModifiedMap.emplace(x, std::unordered_set<int>{ stmtNo }).second) {
			varModifiedMap.at(x).insert(stmtNo);
		}
		allModifiedVarList.emplace(x);
	}

	switch (stmtType) {
	case IF:
		addModifyingIfStmt(stmtNo);
		totalVarsModifiedForIfStmts += numVarsModified;
		break;
	case WHILE:
	 	addModifyingWhileStmt(stmtNo);
		totalVarsModifiedForWhileStmts += numVarsModified;	
		break;
	case CALL:
		addModifyingCallStmt(stmtNo);
		totalVarsModifiedForCallStmts += numVarsModified;
	}

	modifyingStmtList.emplace(stmtNo);

	return 1;
}

bool ModifiesStore::addModifyingIfStmt(int stmtNo) {
	return modifyingIfStmtList.emplace(stmtNo).second;
}


bool ModifiesStore::addModifyingWhileStmt(int stmtNo) {
	return modifyingWhileStmtList.emplace(stmtNo).second;
}

bool ModifiesStore::addReadStmt(int stmtNo) {
	return readStmtList.emplace(stmtNo).second;
}

bool ModifiesStore::addModifyingCallStmt(int stmtNo)
{
	return modifyingCallStmtList.emplace(stmtNo).second;
}

bool ModifiesStore::addModifiedVarToProc(std::string procName, std::unordered_set<std::string> listOfVars)
{
	if (procModifiesMap.count(procName) == 0) {
		totalVarsModifiedForProcs += listOfVars.size();
	}
	else {
		auto& curr = procModifiesMap.at(procName);

		for (const std::string& var : listOfVars) {
			if (curr.count(var) == 0) {
				totalVarsModifiedForProcs += 1;
			}
		}
	}

	if (!procModifiesMap.emplace(procName, listOfVars).second) {
		procModifiesMap.at(procName).insert(listOfVars.begin(), listOfVars.end());
	}

	for (auto& x : listOfVars) {
		if (!varModifiedProcMap.emplace(x, std::unordered_set<std::string>{procName}).second) {
			varModifiedProcMap.at(x).emplace(procName);
		}
	}

	return allModifiyingProcs.emplace(procName).second;
}

// ============ QUERY METHODS ==============

bool ModifiesStore::isVarModifiedInStmt(std::string var, int stmtNo) {
	if (stmtModifiesMap.find(stmtNo) != stmtModifiesMap.end()) {
		std::unordered_map<int, std::unordered_set<std::string>>::const_iterator it = stmtModifiesMap.find(stmtNo);
		std::unordered_set<std::string>::const_iterator it2 = (it->second).find(var);

		return (it != stmtModifiesMap.end()) && (it2 != (it->second).end());
	}
	
	return false;
}

bool ModifiesStore::isVarModifiedInProc(std::string var, std::string procName)
{
	if (procModifiesMap.find(procName) != procModifiesMap.end()) {
		std::unordered_map<std::string, std::unordered_set<std::string>>::const_iterator it = procModifiesMap.find(procName);
		std::unordered_set<std::string>::const_iterator it2 = (it->second).find(var);

		return it2 != (it->second).end();
	}
	return false;
	
}

bool ModifiesStore::isVarModified(std::string var) {
	return allModifiedVarList.find(var) != allModifiedVarList.end();
}

bool ModifiesStore::isStmtModifies(int stmtNo) {
	return stmtModifiesMap.find(stmtNo) != stmtModifiesMap.end();
}

bool ModifiesStore::procHasModifies(std::string procName)
{
	return procModifiesMap.find(procName) != procModifiesMap.end();
}

bool ModifiesStore::hasIfModifies() {
	return !modifyingIfStmtList.empty();
}

bool ModifiesStore::hasWhileModifies() {
	return !modifyingWhileStmtList.empty();
}

bool ModifiesStore::hasModifiesP()
{
	return !procModifiesMap.empty();
}

bool ModifiesStore::hasModifies() {
	return !stmtModifiesMap.empty();
}

// ============ GETTER METHODS ==============

std::unordered_map<int, std::unordered_set<std::string>> ModifiesStore::getStmtModifiesMap() {
	return stmtModifiesMap;
}

std::unordered_map<std::string, std::unordered_set<int>> ModifiesStore::getVarModifiedMap() {
	return varModifiedMap;
}

std::unordered_set<std::string> ModifiesStore::getAllModifiedVars() {
	return allModifiedVarList;
}

std::unordered_set<std::string> ModifiesStore::getAllReadModifiedVars() {
	return allReadModifiedVarList;
}

std::unordered_set<int> ModifiesStore::getModifyingStmtList() {
	return modifyingStmtList;
}

std::unordered_set<int>  ModifiesStore::getReadStmtList() {
	return readStmtList;
}

std::unordered_set<int>  ModifiesStore::getModifyingIfStmtList() {
	return modifyingIfStmtList;
}

std::unordered_set<int>  ModifiesStore::getModifyingWhileStmtsList() {
	return modifyingWhileStmtList;
}

std::unordered_set<std::string> ModifiesStore::getModifiyingProcs()
{
	return allModifiyingProcs;
}

std::unordered_set<int> ModifiesStore::getStmtsModifyingVar(std::string var) {
	if (varModifiedMap.find(var) != varModifiedMap.end()) {
		return varModifiedMap.at(var);
	 }
	
	return {};
}

std::unordered_set<std::string> ModifiesStore::getModifiedVarsInStmt(int stmtNo) {
	if (stmtModifiesMap.find(stmtNo) != stmtModifiesMap.end()) {
		return stmtModifiesMap.at(stmtNo);
	}
	return {};
}

std::unordered_set<std::string> ModifiesStore::getModifiedVarsInProc(std::string p)
{
	if (procModifiesMap.find(p) != procModifiesMap.end()) {
		return procModifiesMap.at(p);
	}
	return {};
}

std::unordered_set<std::string> ModifiesStore::getProcModifyingVar(std::string var)
{
	if (varModifiedProcMap.find(var) != varModifiedProcMap.end()) {
		return varModifiedProcMap.at(var);
	}
	return {};
}

std::unordered_set<std::string> ModifiesStore::getReadVars() {
	std::unordered_set<std::string> allReadVars;

	//for every stmtNo, get the var list and add to result set
	for (auto& x: readStmtList) {
		allReadVars.insert(stmtModifiesMap.at(x).begin(), stmtModifiesMap.at(x).end());
	}
	return allReadVars;
}

std::unordered_set<std::string> ModifiesStore::getVarsModifiedInWhile() {
	std::unordered_set<std::string> allWhileVars;

	for (auto& x: modifyingWhileStmtList) {
		allWhileVars.insert(stmtModifiesMap.at(x).begin(), stmtModifiesMap.at(x).end());
	}
	return allWhileVars;
}

std::unordered_set<std::string> ModifiesStore::getVarsModifiedInIf() {
	std::unordered_set<std::string> allIfVars;

	for (auto& x: modifyingIfStmtList) {
		allIfVars.insert(stmtModifiesMap.at(x).begin(), stmtModifiesMap.at(x).end());
	}
	return allIfVars;
}

std::unordered_set<int> ModifiesStore::getModifyingCallStmtList() {
	return modifyingCallStmtList;
}

std::unordered_map<int, std::string> ModifiesStore::getReadStmtToVarMap() {
	return readStmtToVarMap;
}

std::unordered_map<std::string, std::unordered_set<int>> ModifiesStore::getVarToReadStmtMap() {
	return varToReadStmtMap;
}

int ModifiesStore::getTotalVarsModifiedForStmts() {
	return totalVarsModifiedForStmts;
}

int ModifiesStore::getTotalVarsModifiedForReadStmts() {
	return totalVarsModifiedForReadStmts;
}

int ModifiesStore::getTotalVarsModifiedForAssignStmts() {
	return totalVarsModifiedForAssignStmts;
}

int ModifiesStore::getTotalVarsModifiedForIfStmts() {
	return totalVarsModifiedForIfStmts;
}

int ModifiesStore::getTotalVarsModifiedForWhileStmts() {
	return totalVarsModifiedForWhileStmts;
}

int ModifiesStore::getTotalVarsModifiedForCallStmts() {
	return totalVarsModifiedForCallStmts;
}

int ModifiesStore::getTotalVarsModifiedForProcs() {
	return totalVarsModifiedForProcs;
}