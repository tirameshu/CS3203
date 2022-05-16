#pragma once

#include <unordered_set>
#include <unordered_map>
#include "HasherFunctions.h"

/*
	The CallStore is a PKB component that stores information between procedures, it is populated whenever
	a call statement is encountered, where the caller (enclosing procedure of call statement) and the 
	callee (procedure called by call statement) are stored. This component is responsible for storing the 
	information of the Calls/Calls* design abstraction. It offers the neccessary setter methods for
	populating its data structures, query methods and getter methods for accessing neccessary information.
*/

/*
	A structure to contain Calls properties of a procedure:
	1) listOfCallers
	- The list of immediate (direct) caller procedures of this procedure
	2) listOfCallees
	- The list of immediate (direct) callee procedures called by this procedure
	3) listOfTCallers
	- The list of transitive caller procedures of this procedure
	4) listOfTCallees
	- The list of transitive callee procedures of this procedure
*/
struct callStruct {
	std::unordered_set<std::string> listOfCallers;
	std::unordered_set<std::string> listOfCallees;
	std::unordered_set<std::string> listOfTCallers;
	std::unordered_set<std::string> listOfTCallees;
};

class CallStore {
private:
	// A map to store all procedures involved in a call statement (both caller and callee)
	static std::unordered_map<std::string, callStruct> procCallsMap;

	// A list of all procedures that calls another procedure, but are not called by any procedure
	static std::unordered_set<std::string> listOfRootProcs;

	// A list of all procedures that are called by another procedure, but do not call any procedure
	static std::unordered_set<std::string> listOfLeafProcs;

	// A list of all procedures that call some procedure
	static std::unordered_set<std::string> listOfAllCallers;

	// A list of all procedures that are called by some procedure
	static std::unordered_set<std::string> listOfAllCallees;

	// A map call statement numbers to the procedure name called
	static std::unordered_map<int, std::string> callerStmtToProcMap;

	// A map of procedure name to a list of call stmts calling them
	static std::unordered_map<std::string, std::unordered_set<int>> procToCallerStmtsMap;

	// A map procedure name to all call statements in the procedure
	static std::unordered_map<std::string, std::unordered_set<int>> procedureToCallStmtMap;
	
	// A map of call statement to its enclosing procedure
	static std::unordered_map<int, std::string> callStmtToProcedureMap;

	// A list to store all pairs <p1, p2> where Calls(p1, p2) holds true
	static std::unordered_set<std::pair<std::string, std::string>, StringPairHasher> callsPairList;

	// A list to store all pairs <p1, p2> where Calls*(p1, p2) holds true
	static std::unordered_set<std::pair<std::string, std::string>, StringPairHasher> callsStarPairList; 

public:
	CallStore();

	// clears all data structures
	static void clear();

	// ============ SETTER METHODS ==============
	// for parser -> Call(p1, p2)
	//adds immediate call relationships into callstore
	//also adds the stmt no of the call stmt into the mappings in call store
	static bool addCall(std::string callerP, std::string calleeP, int stmtNo);

	// for DE -> Call*(p1, p2)
	//adds transitive callers into callstore
	static bool addCallerT(std::string calleeP, std::unordered_set<std::string> callerP);

	// for DE 
	//adds transitve callees into callstore
	static bool addCalleeT(std::string callerP, std::unordered_set<std::string> calleeP);

	// ============ QUERY METHODS ==============
	
	//evaluates Calls(_, p)
	static bool hasCaller(std::string p);

	//evaluates Calls(p, _)
	static bool hasCallee(std::string p);

	//evaluates Calls(caller, callee)
	static bool Calls(std::string caller, std::string callee);

	//evaluates Calls*(caller, callee)
	static bool CallsTransitive(std::string caller, std::string calle);

	//evaluates Calls(_, _)
	static bool hasCall();

	// ============ GETTER METHODS ==============
	//gets list of immediate calles of proc p
	static std::unordered_set<std::string> getCalleesOfProc(std::string p);

	//gets list of immediate callers of proc p
	static std::unordered_set<std::string> getCallersOfProc(std::string p);

	//gets transitive list of callers of proc p
	static std::unordered_set<std::string> getTCallersOfProc(std::string p);

	//gets transitive list of callees of proc p
	static std::unordered_set<std::string> getTCalleesOfProc(std::string p);

	//gets all caller procedures without any callers for themselves
	static std::unordered_set<std::string> getRootProcs();

	//gets all callee procedures without any callees for themselves
	static std::unordered_set<std::string> getLeafProcs();

	//get mapping of call stmtNo -> procedure being called in stmt
	static std::unordered_map<int, std::string> getCallerStmtToProcMap();

	//get mapping of procedure -> caller stmts calling them
	static std::unordered_map<std::string, std::unordered_set<int>> getProcToCallerStmtsMap();

	//get mapping of procName -> call stmtNo in the procedure
	static std::unordered_map<std::string, std::unordered_set<int>> getProcedureToCallStmtMap();

	//get mapping of call stmtNo -> procedure they belong in 
	static std::unordered_map<int, std::string> getCallStmtToProcedureMap();

	//get all stmtNo calling proc
	static std::unordered_set<int> getCallerStmts(std::string proc);

	//get all call stmtNo in proc
	static std::unordered_set<int> getAllCallStmtsInProc(std::string proc);

	// get all caller stmtNo
	static std::unordered_set<std::string> getListOfAllCallers();

	// get all callee stmtNo
	static std::unordered_set<std::string> getListOfAllCallees();

	//get all procs involved in calls
	static std::unordered_set<std::string> getAllCallProcs();

	// get all calls pairs
	static std::unordered_set<std::pair<std::string, std::string>, StringPairHasher> getCallsPairList();

	// get all callsStar pairs
	static std::unordered_set<std::pair<std::string, std::string>, StringPairHasher> getCallsStarPairList();
};
