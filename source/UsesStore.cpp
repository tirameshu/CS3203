#include "UsesStore.h"

//UsesS
std::unordered_map<int, std::unordered_set<std::string>> UsesStore::stmtUsesMap; //map stmt to all variables used by stmt
std::unordered_map<std::string, std::unordered_set<int>> UsesStore::varUsedMap; //map all vars to the stmts that uses them
std::unordered_set<std::string> UsesStore::allUsedVarList; //list of vars used throughout
std::unordered_set<std::string> UsesStore::allPrintUsedVarList; //list of vars used throughout print statements only
std::unordered_set<int> UsesStore::allUsesIfStmts; //list of all uses if stmts
std::unordered_set<int> UsesStore::allUsesWhileStmts; //list of all uses while stmts
std::unordered_set<int> UsesStore::allUsesAssignStmts; //list of all uses assign stmts
std::unordered_set<int> UsesStore::printStmtList; //list of all print stmts -> all prints are uses
std::unordered_map<int, std::string> UsesStore::printStmtToVarMap; //a map of print stmt to variable used
std::unordered_map<std::string, std::unordered_set<int>> UsesStore::varToPrintStmtMap; //a map of variable used to list of print stmts using it

//UsesP
std::unordered_map<std::string, std::unordered_set<std::string>> UsesStore::procUsesMap;
std::unordered_map<std::string, std::unordered_set<std::string>> UsesStore::varUsesPMap;
std::unordered_set<std::string> UsesStore::allUsesProc;
std::unordered_set<int> UsesStore::allUsesCallStmts;

int UsesStore::totalVarsUsedForStmts; // total number of used variables for stmts
int UsesStore::totalVarsUsedForPrintStmts; // total number of used variables for print stmts
int UsesStore::totalVarsUsedForAssignStmts; // total number of used variables for assign stmts
int UsesStore::totalVarsUsedForIfStmts; // total number of used variables for if stmts
int UsesStore::totalVarsUsedForWhileStmts; // total number of used variables for while stmts
int UsesStore::totalVarsUsedForCallStmts; // total number of used variables for call stmts
int UsesStore::totalVarsUsedForProcs; // total number of used variables for procedures

UsesStore::UsesStore() {}

void UsesStore::clear() {
	stmtUsesMap.clear();
	varUsedMap.clear();
	allUsedVarList.clear();
	allPrintUsedVarList.clear();
	allUsesIfStmts.clear();
	allUsesWhileStmts.clear();
	allUsesAssignStmts.clear();
	printStmtList.clear();
	allUsesCallStmts.clear();
	procUsesMap.clear();
	varUsesPMap.clear();
	allUsesProc.clear();
	printStmtToVarMap.clear();
	varToPrintStmtMap.clear();
	totalVarsUsedForStmts = 0;
	totalVarsUsedForPrintStmts = 0;
	totalVarsUsedForAssignStmts = 0;
	totalVarsUsedForIfStmts = 0;
	totalVarsUsedForWhileStmts = 0;
	totalVarsUsedForCallStmts = 0;
	totalVarsUsedForProcs = 0;
}

// to be called by Parser, used by all assign/print stmt
// ============ SETTER METHODS ==============
bool UsesStore::addUses(int stmtNo, int stmtType, std::unordered_set<std::string> listOfUsedVars) {
	// not a valid mapping to add
	if (listOfUsedVars.empty()) {
		return false;
	}
	if (!stmtUsesMap.emplace(stmtNo, listOfUsedVars).second) {
		stmtUsesMap.at(stmtNo).insert(listOfUsedVars.begin(), listOfUsedVars.end());
	}

	//add to var->stmt map & add to general vars used list
	for (const std::string& x: listOfUsedVars) {
		if (!varUsedMap.emplace(x, std::unordered_set<int>{ stmtNo }).second) {
			varUsedMap.at(x).emplace(stmtNo);
		}

		if (stmtType == PRINT) {
			allPrintUsedVarList.emplace(x);
			printStmtToVarMap.emplace(stmtNo, x);
			if (!varToPrintStmtMap.emplace(x, std::unordered_set<int>{stmtNo}).second) {
				varToPrintStmtMap.at(x).emplace(stmtNo);
			}
		}

		allUsedVarList.emplace(x);
	}

	switch (stmtType) {
	case ASSIGN:
		addAssignStmt(stmtNo);
		totalVarsUsedForAssignStmts += listOfUsedVars.size();
		break;
	case PRINT:
		addPrintStmt(stmtNo);
		totalVarsUsedForPrintStmts += listOfUsedVars.size();
		break;
	case IF:
		addIfUses(stmtNo);
		totalVarsUsedForIfStmts += listOfUsedVars.size();
		break;
	case WHILE:
		addWhileUses(stmtNo);
		totalVarsUsedForWhileStmts += listOfUsedVars.size();
		break;
	}

	totalVarsUsedForStmts += listOfUsedVars.size();

	return 1;
}

// to be called by DesignExtractor
bool UsesStore::addUsesToContainer(int stmtNo, int stmtType, std::unordered_set<std::string> listOfVar) {
	// update the list of variables mapped to if/while stmtNo (to contain all variables used by its children stmts)
	int numVarsUsed = 0;

	if (stmtUsesMap.count(stmtNo) == 0) {
		numVarsUsed += listOfVar.size();
	}
	else {
		auto& curr = stmtUsesMap.at(stmtNo);

		for (const std::string& var : listOfVar) {
			if (curr.count(var) == 0) {
				numVarsUsed += 1;
			}
		}
	}

	totalVarsUsedForStmts += numVarsUsed;
	
	if (!stmtUsesMap.emplace(stmtNo, listOfVar).second) {
		stmtUsesMap.at(stmtNo).insert(listOfVar.begin(), listOfVar.end());
	}

	for (const std::string& x : listOfVar) {
		if (!varUsedMap.emplace(x, std::unordered_set<int>{ stmtNo }).second) {
			varUsedMap.at(x).emplace(stmtNo);
		}

		allUsedVarList.emplace(x);
	}

	switch (stmtType) {
	case IF:
		addIfUses(stmtNo);
		totalVarsUsedForIfStmts += numVarsUsed;
		break;
	case WHILE:
		addWhileUses(stmtNo);
		totalVarsUsedForWhileStmts += numVarsUsed;
		break;
	case CALL:
		addCallUses(stmtNo);
		totalVarsUsedForCallStmts += numVarsUsed;
	}

	return 1;
}

bool UsesStore::addAssignStmt(int stmtNo) {
	return allUsesAssignStmts.emplace(stmtNo).second;
}

bool UsesStore::addPrintStmt(int stmtNo) {
	return printStmtList.emplace(stmtNo).second;
}

bool UsesStore::addIfUses(int stmtNo) {
	return allUsesIfStmts.emplace(stmtNo).second;
}

bool UsesStore::addWhileUses(int stmtNo) {
	return allUsesWhileStmts.emplace(stmtNo).second;
}

bool UsesStore::addCallUses(int stmtNo) {
	return allUsesCallStmts.emplace(stmtNo).second;
}

bool UsesStore::addProcUses(std::string procName, std::unordered_set<std::string> listOfVars)
{
	if (procUsesMap.count(procName) == 0) {
		totalVarsUsedForProcs += listOfVars.size();
	}
	else {
		auto& curr = procUsesMap.at(procName);

		for (const std::string& var : listOfVars) {
			if (curr.count(var) == 0) {
				totalVarsUsedForProcs += 1;
			}
		}
	}

	if (!procUsesMap.emplace(procName, listOfVars).second) {
		procUsesMap.at(procName).insert(listOfVars.begin(), listOfVars.end());
	}

	for (auto& x : listOfVars) {
		if (!varUsesPMap.emplace(x, std::unordered_set<std::string> {procName}).second) {
			varUsesPMap.at(x).emplace(procName);
		}
	}
	return allUsesProc.emplace(procName).second;
}

// ============ QUERY METHODS ==============
bool UsesStore::isVarUsedInStmt(int stmtNo, std::string var) {
	//get all variables used by stmt
	if (stmtUsesMap.count(stmtNo) == 0) {
		return false;
	}

	std::unordered_set<std::string> varListOfStmt = stmtUsesMap.at(stmtNo);
	//check if var exists in the list
	return varListOfStmt.find(var) != varListOfStmt.end();
}

bool UsesStore::isVarUsedInProc(std::string procName, std::string var)
{
	if (procUsesMap.find(procName) != procUsesMap.end()) {
		std::unordered_map<std::string, std::unordered_set<std::string>>::const_iterator it = procUsesMap.find(procName);
		std::unordered_set<std::string>::const_iterator it2 = (it->second).find(var);

		return it2 != (it->second).end();
	}

	return false;
	
}

bool UsesStore::isVarUsed(std::string var) {
	return varUsedMap.find(var) != varUsedMap.end();
}

bool UsesStore::isStmtUses(int stmtNo) {
	return stmtUsesMap.find(stmtNo) != stmtUsesMap.end();
}

bool UsesStore::procUses(std::string procName)
{
	return procUsesMap.find(procName) != procUsesMap.end();
}

bool UsesStore::hasIfUses() {
	return !allUsesIfStmts.empty();
}

bool UsesStore::hasWhileUses() {
	return !allUsesWhileStmts.empty();
}

bool UsesStore::hasAssignUses() {
	return !allUsesAssignStmts.empty();
}

bool UsesStore::hasProcUses()
{
	return !allUsesProc.empty();
}

bool UsesStore::hasUses() {
	return !stmtUsesMap.empty();
}


// ============ GETTER METHODS ==============
std::unordered_map<int, std::unordered_set<std::string>> UsesStore::getStmtUsesMap() {
	return stmtUsesMap;
}

std::unordered_map<std::string, std::unordered_set<int>> UsesStore::getVarUsedMap() {
	return varUsedMap;
}

std::unordered_map<std::string, std::unordered_set<std::string>> UsesStore::getProcUsesMap()
{
	return procUsesMap;
}

std::unordered_set<std::string> UsesStore::getAllUsedVarList() {
	return allUsedVarList;
}

std::unordered_set<std::string> UsesStore::getAllPrintUsedVarList() {
	return allPrintUsedVarList;
}

std::unordered_set<int> UsesStore::getAllUsesIfStmts() {
	return allUsesIfStmts;
}
std::unordered_set<int> UsesStore::getAllUsesWhileStmts() {
	return allUsesWhileStmts;
}
std::unordered_set<int> UsesStore::getAllUsesAssignStmts() {
	return allUsesAssignStmts;
}

std::unordered_set<int> UsesStore::getPrintStmtList() {
	return printStmtList;
}

std::unordered_set<std::string> UsesStore::getAllUsesProcs()
{
	return allUsesProc;
}

std::unordered_set<std::string> UsesStore::getUsedVarsInStmt(int stmtNo) {
	std::unordered_map<int, std::unordered_set<std::string>>::const_iterator it = stmtUsesMap.find(stmtNo);
	if (it == stmtUsesMap.end()) {
		//if stmtNo does not exist in the map then return empty set
		return {};
	}
	else {
		return (it->second);
	}
}

std::unordered_set<int> UsesStore::getStmtsUsingVar(std::string var) {
	std::unordered_map<std::string, std::unordered_set<int>>::const_iterator it = varUsedMap.find(var);
	if (it == varUsedMap.end()) {
		//if var does not exist in map then return empty set
		return {};
	}
	else {
		return (it->second);
	}
}

std::unordered_set<std::string> UsesStore::getUsedVarsInProc(std::string procName)
{
	if (procUsesMap.find(procName) != procUsesMap.end()) {
		return 	procUsesMap.at(procName);
	}
	return {};
}

std::unordered_set<std::string> UsesStore::getProcsUsingVar(std::string var)
{
	if (varUsesPMap.find(var) != varUsesPMap.end()) {
		return varUsesPMap.at(var);
	}
	return {};
}

std::unordered_set<std::string> UsesStore::getPrintVars() {
	std::unordered_set<std::string> allPrintVars;

	for (auto& x : printStmtList) {
		allPrintVars.insert(stmtUsesMap.at(x).begin(), stmtUsesMap.at(x).end());
	}
	return allPrintVars;
}

std::unordered_set<std::string> UsesStore::getVarsUsedInWhile() {
	std::unordered_set<std::string> allWhileVars;
	for (auto& x: allUsesWhileStmts) {
		allWhileVars.insert(stmtUsesMap.at(x).begin(), stmtUsesMap.at(x).end());
	}

	return allWhileVars;
}

std::unordered_set<std::string> UsesStore::getVarsUsedInIf() {
	std::unordered_set<std::string> allIfVars;
	for (auto& x: allUsesIfStmts) {
		allIfVars.insert(stmtUsesMap.at(x).begin(), stmtUsesMap.at(x).end());
	}

	return allIfVars;
}

std::unordered_set<std::string> UsesStore::getVarsUsedInAssign() {
	std::unordered_set<std::string> allAssignVars = {};
	for (auto& x : allUsesAssignStmts) {
		allAssignVars.insert(stmtUsesMap.at(x).begin(), stmtUsesMap.at(x).end());
	}

	return allAssignVars;
}

std::unordered_set<std::string> UsesStore::getVarsUsedInCall()
{
	std::unordered_set<std::string> allCallVars = {};
	for (auto& x : allUsesCallStmts) {
		allCallVars.insert(stmtUsesMap.at(x).begin(), stmtUsesMap.at(x).end());
	}

	return allCallVars;
}

std::unordered_set<int> UsesStore::getAllUsesCallStmts() {
	return allUsesCallStmts;
}

std::unordered_map<int, std::string> UsesStore::getPrintStmtToVarMap() {
	return printStmtToVarMap;
}

std::unordered_map<std::string, std::unordered_set<int>> UsesStore::getVarToPrintStmtMap() {
	return varToPrintStmtMap;
}

int UsesStore::getTotalVarsUsedForStmts() {
	return totalVarsUsedForStmts;
}

int UsesStore::getTotalVarsUsedForPrintStmts() {
	return totalVarsUsedForPrintStmts;
}

int UsesStore::getTotalVarsUsedForAssignStmts() {
	return totalVarsUsedForAssignStmts;
}

int UsesStore::getTotalVarsUsedForIfStmts() {
	return totalVarsUsedForIfStmts;
}

int UsesStore::getTotalVarsUsedForWhileStmts() {
	return totalVarsUsedForWhileStmts;
}

int UsesStore::getTotalVarsUsedForCallStmts() {
	return totalVarsUsedForCallStmts;
}

int UsesStore::getTotalVarsUsedForProcs() {
	return totalVarsUsedForProcs;
}