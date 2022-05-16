#pragma once

#include <unordered_set>
#include <unordered_map>
#include "HasherFunctions.h"

/*
	The ParentStore is a PKB component that stores information between statements. This component
	is responsible for storing the information of the Parent/Parent* design abstraction. It
	offers the neccessary setter methods for populating its data structures, query methods and
	getter methods for accessing neccessary information.
*/

/*
	A structure to contain Parent/Parent* properties of a Statement:
	1) stmtParent
	- The statement number of the immediate (direct) parent statement enclosing this statement
	2) listOfChildren
	- The list of immediate (direct) children statements in the statement list of this statement
	3) listOfAncestors
	- The list of transitive ancestor statements that encloses this statement at different nesting levels
	4) listOfDescendants
	- The list of transitive descendant statements that are enclosed by this statement at different nesting levels
*/

struct parentStruct {
	int stmtParent; 
	std::unordered_set<int> listOfChildren; 
	std::unordered_set<int> listOfAncestors; 
	std::unordered_set<int> listOfDescendants;
};

class ParentStore {
private:
	// A map containing the parent properties regarding a specific statement
	static std::unordered_map<int, parentStruct> stmtParentMap; 

	// A list to store all pairs <s1, s2> where Parent(s1, s2) holds true
	static std::unordered_set<std::pair<int, int>, IntPairHasher> parentPairList;

	// A list to store all pairs <s1, s2> where Parent * (s1, s2) holds true
	static std::unordered_set<std::pair<int, int>, IntPairHasher> parentStarPairList; 

	// A list of all statements that are parent in the program
	static std::unordered_set<int> parentList;

	// A list of all statements that are child in the program
	static std::unordered_set<int> childList; 

public:
	ParentStore();

	// clears all data structures
	static void clear();

	// ============ SETTER METHODS ==============
	// adds and updates 'stmtParent' and 'listOfChildren' properties of parentStmt and individual childStmt
	static bool addParent(int parent, std::unordered_set<int> childrenList); 

	// adds and updates 'listOfAncestors' property of stmt
	static bool addAncestors(int stmt, std::unordered_set<int> ancestorList);

	// adds and updates 'listOfDescendants' property of stmt
	static bool addDescendants(int stmt, std::unordered_set<int> descendantList);

	// ============ QUERY METHODS ==============
	// checks if a statement has a parent statement associated
	static bool stmtHasParent(int stmt);

	// checks if a statement has any children statements associated
	static bool stmtHasChildren(int stmt);

	// checks if the program has any parent statement
	static bool programHasParentStmt();

	// checks if the program has any child statement
	static bool programHasChildStmt();

	// ============ GETTER METHODS ==============
	// gets the 'parent statement' of a stmt
	static int getParentOfStmt(int stmt);

	// gets the 'listOfChildren' of a stmt
	static std::unordered_set<int> getChildrenListOfStmt(int stmt);

	// gets the 'listOfAncestors' of a stmt
	static std::unordered_set<int> getAncestorsListOfStmt(int stmt);

	// gets the 'listOfDescendants' of a stmt
	static std::unordered_set<int> getDescendantsListOfStmt(int stmt);

	// gets the list of all Parent pairs
	static std::unordered_set<std::pair<int, int>, IntPairHasher> getAllParentPair();

	// gets the list of all ParentStar pairs
	static std::unordered_set<std::pair<int, int>, IntPairHasher> getAllParentStarPair(); 

	// gets the list of all statements that are parent
	static std::unordered_set<int> getAllParentStmt(); 

	// gets the list of all statements that are child
	static std::unordered_set<int> getAllChildStmt(); 
};
