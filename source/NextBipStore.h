#pragma once

#include <unordered_set>
#include <unordered_map>
#include "HasherFunctions.h"

/*
	The NextBipStore is a PKB component that stores information between statements regarding the
	NextBip/ NextBip* design abstraction relationship. It offers the neccessary setter methods for
	populating its data structures, query methods and getter methods for accessing neccessary information.
*/

/*
	A structure to contain the NextBip/NextBip* property of a Statement:
	1) prevBipStmts
	- The list of all statements that comes immediately before this statement in the CFG
	2) nextBipStmts
	- The list of all statements that comes immediately after this statement in the CFG
	3) listOfPrevBipStarStmt
	- The list of all statements that comes before this statement in the CFG
	4) listOfNextBipStarStmt
	- The list of all statements that comes after this statement in the CFG
	5) listOfIncoming
	- The list of incoming interprocedural edges of this statement in the CFG
	6) listOfOutgoing
	- The list of outgoing interprocedural edges of this statement in the CFG
*/
struct nextBipStruct {
	std::unordered_set<int> prevBipStmts;
	std::unordered_set<int> nextBipStmts; 
	std::unordered_set<int> listOfPrevBipStarStmt;
	std::unordered_set<int> listOfNextBipStarStmt;
	std::unordered_set<int> listOfIncoming; 
	std::unordered_set<int> listOfOutgoing; 
};

class NextBipStore {
private:
	// A map containing the NextBip properties regarding a specific statement
	static std::unordered_map<int, nextBipStruct> stmtNextBipMap; 

	// A list to store all pairs <s1, s2> where NextBip(s1, s2) holds true
	static std::unordered_set<std::pair<int, int>, IntPairHasher> nextBipPairList; 

	// A list to store all pairs <s1, s2> where NextBip*(s1, s2) holds true
	static std::unordered_set<std::pair<int, int>, IntPairHasher> nextBipStarPairList;

	// A list of all statements that comes before a node in the CFG
	static std::unordered_set<int> prevBipList; 
	// A list of all statements that comes after a node in the CFG
	static std::unordered_set<int> nextBipList;

public:
	NextBipStore();

	// clears all data structures
	static void clear();

	// ============ SETTER METHODS ==============
	// adds and updates 'prevBipStmts' and 'nextBipStmts' properties of stmt1 and stmt2 respectively, caller > 0 if it is an interprocedural edge in the NextBip CFG
	static bool addNextBip(int stmt1, int stmt2, int caller);

	// adds and updates 'listOfPrevBipStarStmt' property of stmt
	static bool addAllPreviousBip(int stmt, std::unordered_set<int> listOfPrevBipStarStmt);

	// adds and updates 'listOfNextBipStarStmt' property of stmt
	static bool addAllNextBip(int stmt, std::unordered_set<int> listOfNextBipStarStmt);

	// ============ QUERY METHODS ==============
	// checks if a statement has a previous bip statement in CFG
	static bool stmtHasPrevBip(int stmt);

	// checks if a statement has a next bip statement in CFG
	static bool stmtHasNextBip(int stmt);

	// checks if the program has any previous bip statement
	static bool programHasPrevBipStmt();

	// checks if the program has any next bip statement
	static bool programHasNextBipStmt();

	// ============ GETTER METHODS ==============
	// gets the set of 'immediate previous bip statements' of a stmt
	static std::unordered_set<int> getPrevBipOfStmt(int stmt);

	// gets the set of 'immediate next bip statements' of a stmt
	static std::unordered_set<int> getNextBipOfStmt(int stmt);

	// gets the 'all previous bip statements' of a stmt
	static std::unordered_set<int> getPrevBipStarListOfStmt(int stmt);

	// gets the 'all next bip statements' of a stmt
	static std::unordered_set<int> getNextBipStarListOfStmt(int stmt);

	// gets the list of incoming edges of a statement from the NextBip CFG
	static std::unordered_set<int> getNextBipListOfIncomingOfStmt(int stmt);

	// gets the list of outgoing edges of a statement from the NextBip CFG
	static std::unordered_set<int> getNextBipListOfOutgoingOfStmt(int stmt);

	// gets the list of all NextBip pairs
	static std::unordered_set<std::pair<int, int>, IntPairHasher> getAllNextBipPair();

	// gets the list of all NextBipStar pairs
	static std::unordered_set<std::pair<int, int>, IntPairHasher> getAllNextBipStarPair();

	// gets the list of all statements that come before a node in NextBip CFG
	static std::unordered_set<int> getAllPreviousBipStmt();

	// gets the list of all statements that come after a node in NextBip CFG
	static std::unordered_set<int> getAllNextBipStmt();
};
