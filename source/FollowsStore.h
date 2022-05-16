#pragma once

#include <unordered_set>
#include <unordered_map>
#include "HasherFunctions.h"

/*
	The FollowsStore is a PKB component that stores information between statements. This component
	is responsible for storing the information of the Follows/Follows* design abstraction. It 
	offers the neccessary setter methods for populating its data structures, query methods and
	getter methods for accessing neccessary information.
*/

/*
	A structure to contain Follows/Follows* properties of a Statement:
	1) stmtFollowed
	- The statement number of the immediate (direct) statement before this statement
	2) stmtFollower
	- The statement number of the immediate (direct) statement after this statement
	3) listOfFollowed
	- The list of transitive statements that come before this statement in the statement list (following)
	4) listOfFollower
	- The list of transitive statements that come after this statement in the statement list (followers)
*/
struct followsStruct {
	int stmtFollowed; 
	int stmtFollower;
	std::unordered_set<int> listOfFollowed;
	std::unordered_set<int> listOfFollower; 
};

class FollowsStore {
private: 
	// A map containing the follow properties regarding a specific statement
	static std::unordered_map<int, followsStruct> stmtFollowsMap; 

	// A list to store all pairs <s1, s2> where Follow(s1, s2) holds true
	static std::unordered_set<std::pair<int, int>, IntPairHasher> followsPairList; 

	// A list to store all pairs <s1, s2> where Follow*(s1, s2) holds true
	static std::unordered_set<std::pair<int, int>, IntPairHasher> followsStarPairList; 

	// A list of all statements in the program that are followed
	static std::unordered_set<int> followedList; 

	// A list of all statements in the program that are followers
	static std::unordered_set<int> followerList; 
	
public:
	FollowsStore();

	// clears all data structures
	static void clear();

	// ============ SETTER METHODS ==============
	// adds and updates 'followed' and 'follower' properties of stmt1 and stmt2 respectively
	static bool addFollows(int stmt1, int stmt2);

	// adds and updates 'listOfFollowed' property of stmt
	static bool addAllFollowed(int stmt, std::unordered_set<int> listOfFollowed);

	// adds and updates 'listOfFollower' property of stmt
	static bool addAllFollower(int stmt, std::unordered_set<int> listOfFollower);

	// ============ QUERY METHODS ==============
	// checks if a statement has a followed statement associated
	static bool stmtHasFollowed(int stmt);

	// checks if a statement has a follower statement associated
	static bool stmtHasFollower(int stmt);

	// checks if the program has any followed statement
	static bool programHasFollowedStmt();

	// checks if the program has any follower statement
	static bool programHasFollowerStmt();

	// ============ GETTER METHODS ==============
	// gets the 'followed statement' of a stmt
	static int getFollowedOfStmt(int stmt);

	// gets the 'follower statement' of a stmt
	static int getFollowerOfStmt(int stmt);

	// gets the 'listOfFollowed' of a stmt
	static std::unordered_set<int> getFollowedListOfStmt(int stmt);

	// gets the 'listOfFollower' of a stmt
	static std::unordered_set<int> getFollowerListOfStmt(int stmt);

	// gets the list of all Follows pairs
	static std::unordered_set<std::pair<int, int>, IntPairHasher> getAllFollowsPair();

	// gets the list of all FollowsStar pairs
	static std::unordered_set<std::pair<int, int>, IntPairHasher> getAllFollowsStarPair();

	// gets the list of all statements that are followed
	static std::unordered_set<int> getAllFollowedStmt();

	// gets the list of all statements that are follower
	static std::unordered_set<int> getAllFollowerStmt();
};
