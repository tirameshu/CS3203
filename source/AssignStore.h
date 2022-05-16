#pragma once

#include <unordered_set>
#include <unordered_map>
#include "HasherFunctions.h"

/*
	The AssignStore is a PKB component that stores information regarding assign statements, particularly
	the variable modified (LHS of assign) and the expression of the assignment in postfix form (RHS of assign). 
	These information are essential for the query evaluator to evaluate pattern clauses regarding assign statements.
	It offers the neccessary setter methods for populating its data structures, query methods and getter methods
	for accessing neccessary information.
*/

/*
	A structure to contain the key information of an Assign Statement:
	1) variable
	- The variable being modified, on LHS of assignment
	2) affecting
	- The expression stored in postfix form, on RHS of assignment
*/
struct assignStruct {
	std::string variable; 
	std::string expression;
};

class AssignStore {
private:
	// A list of all assign statements in the program
	static std::unordered_set<int> assignStmtList; 

	// A map containing the assignStruct property regarding a specific assign statment
	static std::unordered_map<int, assignStruct> stmtAssignMap; 

	// A map of pair(variable, expression) to a set of assign statement numbers that share the same LHS and RHS of the assignment
	static std::unordered_map<std::pair<std::string, std::string>, std::unordered_set<int>, StringPairHasher> varExpStmtList;

	// A map of variable(LHS) to a list of expressions(RHS)
	static std::unordered_map <std::string, std::unordered_set<std::string>> varToExpMap; 

	// A map of variable(LHS) to a list of assign statements
	static std::unordered_map<std::string, std::unordered_set<int>> varAssignStmtList; 

	// A map of expression(RHS) to a list of assign statements
	static std::unordered_map<std::string, std::unordered_set<int>> expAssignStmtList; 

public:
	AssignStore();
	
	// clears all data structures
	static void clear();

	// ============ SETTER METHODS ==============
	// adds and store assign statement in assignStmtList
	static bool addAssignStmt(int stmt);

	// adds and updates properties of an assign statement with its variable(LHS) and expression(RHS)
	static bool addAssign(int stmt, std::string var, std::string exp);

	// ============ GETTER METHODS ==============
	// gets the list of assignment statement numbers
	static std::unordered_set<int> getAssignStmtList();

	// gets the 'variable' of an assign stmt
	static std::string getVariableOfStmt(int stmt);

	// gets the 'expression' of an assign stmt
	static std::string getExpressionOfStmt(int stmt);

	// gets the map of pair<exp, var> to its corresponding list of assign statements matched
	static std::unordered_map<std::pair<std::string, std::string>, std::unordered_set<int>, StringPairHasher> getVarExpStmtList();

	// gets the map of variable to list of expressions
	static std::unordered_map <std::string, std::unordered_set<std::string>> getVarToExpMap();

	// gets the map of variable to list of assign statements matched
	static std::unordered_map<std::string, std::unordered_set<int>> getVarAssignStmtList();

	// gets the map of expression to list of assign statements matched
	static std::unordered_map<std::string, std::unordered_set<int>> getExpAssignStmtList();
};
