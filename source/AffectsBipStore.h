#pragma once

#include <unordered_set>
#include <unordered_map>
#include "HasherFunctions.h"

/*
	The AffectsBipStore is a PKB component that stores information between assign statements regarding the 
	AffectsBip/ AffectsBip* design abstraction relationship. It offers the neccessary setter methods for 
	populating its data structures, query methods and getter methods for accessing neccessary information.
*/

/*
	A structure to contain the AffectsBip property of an Assign Statement:	
	1) affectedBipBy
	- The list of previous <declaration stmt> affecting the current <using stmt>
	2) affectingBip
	- The list of statements affected by the current <declaration stmt>
	3) affectedBipByT
	- The list of transitive previous <declaration stmt> affecting the current <using stmt>
	4) affectingBipT
	- The list of statements affected by transitive by the current <declaration stmt>
*/
struct affectsBipStruct {
	std::unordered_set<int> affectedBipBy; 
	std::unordered_set<int> affectingBip; 
	std::unordered_set<int> affectedBipByT; 
	std::unordered_set<int> affectingBipT; 
};

class AffectsBipStore {
private:
	// A map containing the affectsBipStruct property regarding a specific assignment statement
	static std::unordered_map<int, affectsBipStruct> stmtAffectsBipMap; 

	// A list to store all pairs <a1, a2> where AffectsBip(a1, a2)
	static std::unordered_set<std::pair<int, int>, IntPairHasher> affectsBipPairList;

	// A list to store all pairs <a1, a2> where AffectsBip*(a1, a2) holds true
	static std::unordered_set<std::pair<int, int>, IntPairHasher> affectsBipStarPairList; 

	// A list of assign statements affected by another assign statement
	static std::unordered_set<int> affectedBipList; 

	// A list of assign statements affecting another assign statement
	static std::unordered_set<int> affectingBipList; 

public:
	AffectsBipStore();

	// clears all data structures
	static void clear();

	// ============ SETTER METHODS ==============
	//  adds and updates the affectsBip properties of an assign statement
	static bool addAffectsBip(int stmt, int affected);

	//  adds and updates the affectedByT list of an assign statement
	static bool addAffectedBipByT(int stmt, std::unordered_set<int> listOfAffectors);

	//  adds and updates the affectingT list of an assign statement
	static bool addAffectingBipT(int stmt, std::unordered_set<int> listOfAffecting);

	// ============ QUERY METHODS ==============
	// checks if a specific assign statement is affected by another assign statement
	static bool stmtHasAffectedBipBy(int stmt);

	// checks if a specific assign statement is affecting another assign statement
	static bool stmtHasAffectingBip(int stmt);

	// checks if the program has an assign statement being affected
	static bool programHasAffectedBip();

	// checks if the program has an assign statement affecting another assign statement
	static bool programHasAffectingBip();

	// ============ GETTER METHODS ==============
	// gets the stmtAffectsBipMap
	static std::unordered_map<int, affectsBipStruct> getStmtAffectsBipMap();

	// gets the list of all AffectsBip pair
	static std::unordered_set<std::pair<int, int>, IntPairHasher> getAffectsBipPairList();

	// gets the list of all AffectsBipT pair
	static std::unordered_set<std::pair<int, int>, IntPairHasher> getAffectsBipStarPairList();

	// gets the list of assign statements affected by another assign statement
	static std::unordered_set<int> getAffectedBipList();

	// gets the list of assign statements affecting another assign statement
	static std::unordered_set<int> getAffectingBipList();

	// gets the list of previous <declaration stmt> affecting specified stmt
	static std::unordered_set<int> getAffectedBipByOfStmt(int stmt);

	// gets the list of statements affected by the specified stmt
	static std::unordered_set<int> getAffectingBipOfStmt(int stmt);

	// gets the list of transitive previous <declaration stmt> affecting the specified stmt
	static std::unordered_set<int> getAffectedBipByTOfStmt(int stmt);

	// gets the list of statements affected by transitive by the specified stmt
	static std::unordered_set<int> getAffectingBipTOfStmt(int stmt);
};
