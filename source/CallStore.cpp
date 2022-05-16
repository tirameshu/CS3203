#include "CallStore.h"

std::unordered_map<std::string, callStruct> CallStore::procCallsMap;
std::unordered_set<std::string> CallStore::listOfRootProcs;
std::unordered_set<std::string> CallStore::listOfLeafProcs;
std::unordered_set<std::string> CallStore::listOfAllCallers;
std::unordered_set<std::string> CallStore::listOfAllCallees;
std::unordered_map<int, std::string> CallStore::callerStmtToProcMap;
std::unordered_map<std::string, std::unordered_set<int>> CallStore::procToCallerStmtsMap;
std::unordered_map<std::string, std::unordered_set<int>> CallStore::procedureToCallStmtMap;
std::unordered_map<int, std::string> CallStore::callStmtToProcedureMap;
std::unordered_set<std::pair<std::string, std::string>, StringPairHasher> CallStore::callsPairList;
std::unordered_set<std::pair<std::string, std::string>, StringPairHasher> CallStore::callsStarPairList;


CallStore::CallStore() {}

void CallStore::clear()
{
	procCallsMap.clear();
	listOfAllCallees.clear();
	listOfAllCallers.clear();
	listOfLeafProcs.clear();
	listOfRootProcs.clear();
	callerStmtToProcMap.clear();
	procToCallerStmtsMap.clear();
	procedureToCallStmtMap.clear();
	callStmtToProcedureMap.clear();
	callsPairList.clear();
	callsStarPairList.clear();
}

// ============ SETTER METHODS ==============
bool CallStore::addCall(std::string caller, std::string callee, int stmtNo)
{

	if (!procCallsMap.emplace(caller, callStruct{ {}, {callee}, {}, {} }).second) {
		//if caller alr exists, add callee in the list of callees for caller
		procCallsMap.at(caller).listOfCallees.emplace(callee);
	}

	if (!procCallsMap.emplace(callee, callStruct{ {caller}, {}, {}, {} }).second) {
		procCallsMap.at(callee).listOfCallers.emplace(caller);
	}

	//if caller is not a callee yet
	if (listOfAllCallees.find(caller) == listOfAllCallees.end()) {
		listOfRootProcs.emplace(caller);
	}
	//if callee is not a caller yet
	if (listOfAllCallers.find(callee) == listOfAllCallers.end()) {
		listOfLeafProcs.emplace(callee);
	}

	//callee cannot possibly be a root proc anymore, remove if in list
	listOfRootProcs.erase(callee);
	//caller cannot possibly be a leaf proc anymore, remove if in list
	listOfLeafProcs.erase(caller);

	//adding stmtNos
	callerStmtToProcMap.emplace(stmtNo, callee);

	if (!procToCallerStmtsMap.emplace(callee, std::unordered_set<int> {stmtNo}).second) {
		procToCallerStmtsMap.at(callee).insert(stmtNo);
	}

	if (!procedureToCallStmtMap.emplace(caller, std::unordered_set<int> {stmtNo}).second) {
		procedureToCallStmtMap.at(caller).insert(stmtNo);
	}

	callStmtToProcedureMap.emplace(stmtNo, caller);


	listOfAllCallees.emplace(callee);
	callsPairList.emplace(std::make_pair(caller, callee));
	return listOfAllCallers.emplace(caller).second;
}

bool CallStore::addCallerT(std::string calleeP, std::unordered_set<std::string> callerP)
{
	for (const std::string& caller : callerP) {
		procCallsMap.at(calleeP).listOfTCallers.emplace(caller);
		procCallsMap.at(caller).listOfTCallees.emplace(calleeP);

		// add pairings to callsStarPairList
		callsStarPairList.emplace(std::make_pair(caller, calleeP));
	}

	return true;
}

bool CallStore::addCalleeT(std::string callerP, std::unordered_set<std::string> calleeP)
{
	for (const std::string& callee : calleeP) {
		procCallsMap.at(callerP).listOfTCallees.emplace(callee);
		procCallsMap.at(callee).listOfTCallers.emplace(callerP);

		// add pairings to callsStarPairList
		callsStarPairList.emplace(std::make_pair(callerP, callee));
	}
	return true;
}

// ============ QUERY METHODS ==============
bool CallStore::hasCaller(std::string p)
{
	//checks if p has immediate callers
	if (procCallsMap.find(p) != procCallsMap.end()) {
		return !procCallsMap.at(p).listOfCallers.empty();
	}
	return false;
	
}

bool CallStore::hasCallee(std::string p)
{
	//checks if p has immediate callees
	if (procCallsMap.find(p) != procCallsMap.end()) {
		return !procCallsMap.at(p).listOfCallees.empty();
	}
	return false;
}

bool CallStore::Calls(std::string caller, std::string callee)
{
	if (procCallsMap.find(caller) != procCallsMap.end()) {
		return procCallsMap.at(caller).listOfCallees.find(callee) != procCallsMap.at(caller).listOfCallees.end();
	}
	return false;
}

bool CallStore::CallsTransitive(std::string caller, std::string callee)
{
	if (procCallsMap.find(caller) != procCallsMap.end()) {
		return procCallsMap.at(caller).listOfTCallees.find(callee) != procCallsMap.at(caller).listOfTCallees.end();
	}
	return false;
}

bool CallStore::hasCall()
{
	//checks if program has any call statements at all
	return !procCallsMap.empty();
}

// ============ GETTER METHODS ===============
std::unordered_set<std::string> CallStore::getCalleesOfProc(std::string p)
{
	if (procCallsMap.find(p) != procCallsMap.end()) {
		return procCallsMap.at(p).listOfCallees;
	}

	return {};
}

std::unordered_set<std::string> CallStore::getCallersOfProc(std::string p)
{
	if (procCallsMap.find(p) != procCallsMap.end()) {
		return procCallsMap.at(p).listOfCallers;
	}

	return {};
}

std::unordered_set<std::string> CallStore::getTCallersOfProc(std::string p)
{
	if (procCallsMap.find(p) != procCallsMap.end()) {
		return procCallsMap.at(p).listOfTCallers;
	}

	return {};
}

std::unordered_set<std::string> CallStore::getTCalleesOfProc(std::string p)
{
	if (procCallsMap.find(p) != procCallsMap.end()) {
		return procCallsMap.at(p).listOfTCallees;
	}

	return {};	
}

std::unordered_set<std::string> CallStore::getRootProcs()
{
	return listOfRootProcs;
}

std::unordered_set<std::string> CallStore::getLeafProcs()
{
	return listOfLeafProcs;
}

std::unordered_map<int, std::string> CallStore::getCallerStmtToProcMap()
{
	return callerStmtToProcMap;
}

std::unordered_map<std::string, std::unordered_set<int>> CallStore::getProcToCallerStmtsMap()
{
	return procToCallerStmtsMap;
}

std::unordered_map<std::string, std::unordered_set<int>> CallStore::getProcedureToCallStmtMap()
{
	return procedureToCallStmtMap;
}

std::unordered_map<int, std::string> CallStore::getCallStmtToProcedureMap()
{
	return callStmtToProcedureMap;
}

std::unordered_set<int> CallStore::getCallerStmts(std::string proc)
{
	if (procToCallerStmtsMap.find(proc) != procToCallerStmtsMap.end()) {
		return procToCallerStmtsMap.at(proc);
	}
	return {};
}

std::unordered_set<int> CallStore::getAllCallStmtsInProc(std::string proc)
{
	if (procedureToCallStmtMap.find(proc) != procedureToCallStmtMap.end()) {
		return procedureToCallStmtMap.at(proc);
	}
	return {};
}

std::unordered_set<std::string> CallStore::getListOfAllCallers() {
	return listOfAllCallers;
}

std::unordered_set<std::string> CallStore::getListOfAllCallees() {
	return listOfAllCallees;
}

std::unordered_set<std::string> CallStore::getAllCallProcs() {
	std::unordered_set<std::string> allCallProcs = listOfAllCallers;
	allCallProcs.insert(listOfAllCallees.begin(), listOfAllCallees.end());
	return allCallProcs;
}

std::unordered_set<std::pair<std::string, std::string>, StringPairHasher> CallStore::getCallsPairList() {
	return callsPairList;
}

std::unordered_set<std::pair<std::string, std::string>, StringPairHasher> CallStore::getCallsStarPairList() {
	return callsStarPairList;
}
 