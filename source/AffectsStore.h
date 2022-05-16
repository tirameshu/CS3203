#pragma once

#include <unordered_set>
#include <unordered_map>
#include "HasherFunctions.h"

/*
	The AffectsStore is a PKB component that stores information between assign statements regarding the
	Affects/ Affects* design abstraction relationship. It offers the neccessary setter methods for
	populating its data structures, query methods and getter methods for accessing neccessary information.
*/

/*
	A structure to contain the Affects property of an Assign Statement:
	1) affectedBy
	- The list of previous <declaration stmt> affecting the current <using stmt>
	2) affecting
	- The list of statements affected by the current <declaration stmt>
	3) affectedByT
	- The list of transitive previous <declaration stmt> affecting the current <using stmt>
	4) affectingT
	- The list of statements affected by transitive by the current <declaration stmt>
*/
struct affectsStruct {
	std::unordered_set<int> affectedBy;
	std::unordered_set<int> affecting; 
	std::unordered_set<int> affectedByT;
	std::unordered_set<int> affectingT;
};

class AffectsStore {
private:
	// A map containing the affectsStruct property regarding a specific assignment statement
	static std::unordered_map<int, affectsStruct> stmtAffectsMap;

	// A list to store all pairs <a1, a2> where Affects(a1, a2) holds true
	static std::unordered_set<std::pair<int, int>, IntPairHasher> affectsPairList; 

	// A list to store all pairs <a1, a2> where Affects*(a1, a2) holds true
	static std::unordered_set<std::pair<int, int>, IntPairHasher> affectsStarPairList; 

	// A list of assign statements affected by another assign statement
	static std::unordered_set<int> affectedList; 

	// A list of assign statements affecting another assign statement
	static std::unordered_set<int> affectingList; 

public:
	AffectsStore();

	// clears all data structures
	static void clear();

	// ============ SETTER METHODS ==============
	//  adds and updates the affects properties of an assign statement
	static bool addAffects(int stmt, int affected);

	//  adds and updates the affectedByT list of an assign statement
	static bool addAffectedByT(int stmt, std::unordered_set<int> listOfAffectors);

	//  adds and updates the affectingT list of an assign statement
	static bool addAffectingT(int stmt, std::unordered_set<int> listOfAffecting);

	// ============ QUERY METHODS ==============
	// checks if a specific assign statement is affected by another assign statement
	static bool stmtHasAffectedBy(int stmt);

	// checks if a specific assign statement is affecting another assign statement
	static bool stmtHasAffecting(int stmt);

	// checks if the program has an assign statement being affected
	static bool programHasAffected();

	// checks if the program has an assign statement affecting another assign statement
	static bool programHasAffecting();

	// ============ GETTER METHODS ==============
	// gets the stmtAffectsMap
	static std::unordered_map<int, affectsStruct> getStmtAffectsMap();

	// gets the list of all Affects pair
	static std::unordered_set<std::pair<int, int>, IntPairHasher> getAffectsPairList();

	// gets the list of all AffectsT pair
	static std::unordered_set<std::pair<int, int>, IntPairHasher> getAffectsStarPairList();

	// gets the list of assign statements affected by another assign statement
	static std::unordered_set<int> getAffectedList();

	// gets the list of assign statements affecting another assign statement
	static std::unordered_set<int> getAffectingList();

	// gets the list of previous <declaration stmt> affecting specified stmt
	static std::unordered_set<int> getAffectedByOfStmt(int stmt);

	// gets the list of statements affected by the specified stmt
	static std::unordered_set<int> getAffectingOfStmt(int stmt);

	// gets the list of transitive previous <declaration stmt> affecting the specified stmt
	static std::unordered_set<int> getAffectedByTOfStmt(int stmt);

	// gets the list of statements affected by transitive by the specified stmt
	static std::unordered_set<int> getAffectingTOfStmt(int stmt);
};
