#pragma once

#include <unordered_set>
#include <unordered_map>

/*
	The WhileStore is a PKB component that stores information regarding while statements, particularly
	the variables used in its conditional expression. These information are essential for the query evaluator
	to evaluate pattern clauses regarding while statements. It offers the neccessary setter methods for populating
	its data structures, query methods and getter methods for accessing neccessary information.
*/

class WhileStore {
private:
	// A map of while statement numbers to all variables used in its conditional expression
	static std::unordered_map<int, std::unordered_set<std::string>> whileStmtUsesMap;

	// A map variables to while statements that uses the variable in their conditional expression 
	static std::unordered_map<std::string, std::unordered_set<int>> varToWhileStmtMap;

	// A list of variables used by while statements in their conditional expression only
	static std::unordered_set<std::string> whileUsedVarList;

	// A list of all while statements in the program that uses a variable in its conditional expression 
	static std::unordered_set<int> whileUsesStmtList; 

	// The total number of control variables used in while stmts, used for Query Optimization purposes
	static int totalWhileUsedVars; 

public:
	WhileStore();

	// clears all data structures
	static void clear();

	// ============ SETTER METHODS ==============
	// stores while statement information in WhileStore
	static bool addWhileUses(int stmt, std::unordered_set<std::string> varUsed);

	// ============ GETTER METHODS ==============
	// gets the whileStmtUsesMap
	static std::unordered_map<int, std::unordered_set<std::string>> getWhileStmtUsesMap();

	// gets the varToWhileStmtMap
	static std::unordered_map<std::string, std::unordered_set<int>> getVarToWhileStmtMap();

	// gets the whileUsedVarList
	static std::unordered_set<std::string> getWhileUsedVarList();

	// gets the whileUsesStmtList
	static std::unordered_set<int> getWhileUsesStmtList();

	// gets the list of variables used by a particular while statement
	static std::unordered_set<std::string> getVarsUsedByWhile(int stmt);

	// gets the list of while statements using a particular variable 
	static std::unordered_set<int> getWhileStmtsUsingVar(std::string var);

	// gets the total number of control variables used in while stmts
	static int getTotalWhileUsedVars();
};
