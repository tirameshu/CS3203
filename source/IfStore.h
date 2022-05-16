#pragma once

#include <unordered_set>
#include <unordered_map>

/*
	The IfStore is a PKB component that stores information regarding if statements, particularly
	the variables used in its conditional expression. These information are essential for the query evaluator
	to evaluate pattern clauses regarding if statements. It offers the neccessary setter methods for populating
	its data structures, query methods and getter methods for accessing neccessary information.
*/

class IfStore {
private:
	// A map of if statement numbers to all variables used in its conditional expression
	static std::unordered_map<int, std::unordered_set<std::string>> ifStmtUsesMap;

	// A map variables to if statements that uses the variable in their conditional expression 
	static std::unordered_map<std::string, std::unordered_set<int>> varToIfStmtMap;

	// A list of variables used by if statements in their conditional expression only
	static std::unordered_set<std::string> ifUsedVarList;

	// A list of all if statements in the program that uses a variable in its conditional expression 
	static std::unordered_set<int> ifUsesStmtList;

	// The total number of control variables used in if stmts, used for Query Optimization purposes
	static int totalIfUsedVars;

public:
	IfStore();

	// clears all data structures
	static void clear();

	// ============ SETTER METHODS ==============
	// stores if statement information in IfStore
	static bool addIfUses(int stmt, std::unordered_set<std::string> varUsed);

	// ============ GETTER METHODS ==============
	// gets the ifStmtUsesMap
	static std::unordered_map<int, std::unordered_set<std::string>> getIfStmtUsesMap();

	// gets the varToIfStmtMap
	static std::unordered_map<std::string, std::unordered_set<int>> getVarToIfStmtMap();

	// gets the ifUsedVarList
	static std::unordered_set<std::string> getIfUsedVarList();

	// gets the ifUsesStmtList
	static std::unordered_set<int> getIfUsesStmtList();

	// gets the list of variables used by a particular if statement
	static std::unordered_set<std::string> getVarsUsedByIf(int stmt);

	// gets the list of if statements using a particular variable 
	static std::unordered_set<int> getIfStmtsUsingVar(std::string var);

	// gets the total number of control variables used in if stmts
	static int getTotalIfUsedVars();
};
