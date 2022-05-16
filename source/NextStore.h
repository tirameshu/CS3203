#pragma once

#include <unordered_set>
#include <unordered_map>
#include "HasherFunctions.h"

/*
	The NextStore is a PKB component that stores information between statements regarding the
	Next/ Next* design abstraction relationship. It offers the neccessary setter methods for
	populating its data structures, query methods and getter methods for accessing neccessary information.
*/

/*
	A structure to contain the Next/Next* property of a Statement:
	1) prevStmts
	- The list of all statements that comes immediately before this statement in the CFG
	2) nextStmts
	- The list of all statements that comes immediately after this statement in the CFG
	3) listOfPrevStarStmt
	- The list of all statements that comes before this statement in the CFG
	4) listOfNextStarStmt
	- The list of all statements that comes after this statement in the CFG
*/
struct nextStruct {
	std::unordered_set<int> prevStmts; 
	std::unordered_set<int> nextStmts; 
	std::unordered_set<int> listOfPrevStarStmt;
	std::unordered_set<int> listOfNextStarStmt;
};

class NextStore {
private:
	// A map containing the Next properties regarding a specific statement
	static std::unordered_map<int, nextStruct> stmtNextMap;

	// A list to store all pairs <s1, s2> where Next(s1, s2) holds true
	static std::unordered_set<std::pair<int, int>, IntPairHasher> nextPairList; 

	// A list to store all pairs <s1, s2> where Next*(s1, s2) holds true
	static std::unordered_set<std::pair<int, int>, IntPairHasher> nextStarPairList; 

	// A list of all statements that comes before a node in the CFG
	static std::unordered_set<int> prevList; 

	// A list of all statements that comes after a node in the CFG
	static std::unordered_set<int> nextList; 

public:
	NextStore();

	// clears all data structures
	static void clear();

	// ============ SETTER METHODS ==============
	// adds and updates 'prevStmts' and 'nextStmts' properties of stmt1 and stmt2 respectively
	static bool addNext(int stmt1, int stmt2);

	// adds and updates 'listOfPrevStarStmt' property of stmt
	static bool addAllPrevious(int stmt, std::unordered_set<int> listOfPrevStarStmt);

	// adds and updates 'listOfNextStarStmt' property of stmt
	static bool addAllNext(int stmt, std::unordered_set<int> listOfNextStarStmt);

	// ============ QUERY METHODS ==============
	// checks if a statement has a previous statement in CFG
	static bool stmtHasPrev(int stmt);

	// checks if a statement has a next statement in CFG
	static bool stmtHasNext(int stmt);

	// checks if the program has any previous statement
	static bool programHasPrevStmt();

	// checks if the program has any next statement
	static bool programHasNextStmt();

	// ============ GETTER METHODS ==============
	// gets the set of 'immediate previous statements' of a stmt
	static std::unordered_set<int> getPrevOfStmt(int stmt);

	// gets the set of 'immediate next statements' of a stmt
	static std::unordered_set<int> getNextOfStmt(int stmt);

	// gets the 'all previous statements' of a stmt
	static std::unordered_set<int> getPrevStarListOfStmt(int stmt);

	// gets the 'all next statements' of a stmt
	static std::unordered_set<int> getNextStarListOfStmt(int stmt);

	// gets the list of all Next pairs
	static std::unordered_set<std::pair<int, int>, IntPairHasher> getAllNextPair();

	// gets the list of all NextStar pairs
	static std::unordered_set<std::pair<int, int>, IntPairHasher> getAllNextStarPair();

	// gets the list of all statements that come before a node in CFG
	static std::unordered_set<int> getAllPreviousStmt();

	// gets the list of all statements that come after a node in CFG
	static std::unordered_set<int> getAllNextStmt();
};
