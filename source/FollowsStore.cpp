#include "FollowsStore.h"

std::unordered_map<int, followsStruct> FollowsStore::stmtFollowsMap; // a map containing the follow properties regarding a specific statement
std::unordered_set<std::pair<int, int>, IntPairHasher> FollowsStore::followsPairList; // stores all pairs <s1, s2> where Follow(s1, s2)
std::unordered_set<std::pair<int, int>, IntPairHasher> FollowsStore::followsStarPairList; // stores all pairs <s1, s2> where Follows*(s1, s2)
std::unordered_set<int> FollowsStore::followedList; // a set of all statements that are followed
std::unordered_set<int> FollowsStore::followerList; // a set of all statements that are followers

FollowsStore::FollowsStore() {}

void FollowsStore::clear() {
	stmtFollowsMap.clear();
	followsPairList.clear();
	followsStarPairList.clear();
	followedList.clear();
	followerList.clear();
}

// ============ SETTER METHODS ==============
bool FollowsStore::addFollows(int stmtNo1, int stmtNo2) {
	if (stmtNo1 > stmtNo2) {
		return false;
	}
	// adding or updating of followStruct for stmt1
	if (!stmtFollowsMap.emplace(stmtNo1, followsStruct{ 0, stmtNo2, {}, {} }).second) {
		// if followStruct for stmt1 already exists, update follower value
		stmtFollowsMap.at(stmtNo1).stmtFollower = stmtNo2;
	}
	// adding or updating of followStruct for stmt2
	if (!stmtFollowsMap.emplace(stmtNo2, followsStruct{ stmtNo1, 0, {}, {} }).second) {
		// if followStruct for stmt2 already exists, update followed value
		stmtFollowsMap.at(stmtNo2).stmtFollowed = stmtNo1;
	}

	followedList.emplace(stmtNo1);
	followerList.emplace(stmtNo2);
	return followsPairList.emplace(std::make_pair(stmtNo1, stmtNo2)).second;
}

bool FollowsStore::addAllFollowed(int stmt, std::unordered_set<int> listOfFollowed) {
	// ensure that listOfFollowed is 0 for proper update of 'followed statements' in a top-down iteration manner
	if (stmtFollowsMap.at(stmt).listOfFollowed.size() != 0) {
		return false;
	}

	for (const int& followed: listOfFollowed) {
		// add pairings to followsStarPairList
		followsStarPairList.emplace(std::make_pair(followed, stmt));
	}
	
	stmtFollowsMap.at(stmt).listOfFollowed = listOfFollowed;
	return true;
}

bool FollowsStore::addAllFollower(int stmt, std::unordered_set<int> listOfFollower) {
	// ensure that listOfFollower is 0 for proper update of 'follower statements' in a bottom-up iteration manner
	if (stmtFollowsMap.at(stmt).listOfFollower.size() != 0) {
		return false;
	}

	for (const int& follower : listOfFollower) {
		// add pairings to followsStarPairList
		followsStarPairList.emplace(std::make_pair(stmt, follower));
	}
	
	stmtFollowsMap.at(stmt).listOfFollower = listOfFollower;
	return true;
}

// ============ QUERY METHODS ==============
bool FollowsStore::stmtHasFollowed(int stmt) {
	return (followerList.find(stmt) != followerList.end());
}

bool FollowsStore::stmtHasFollower(int stmt) {
	return (followedList.find(stmt) != followedList.end());
}

bool FollowsStore::programHasFollowedStmt() {
	return !followedList.empty();
}

bool FollowsStore::programHasFollowerStmt() {
	return !followerList.empty();
}

// ============ GETTER METHODS ==============
int FollowsStore::getFollowedOfStmt(int stmt) {
	if (stmtFollowsMap.find(stmt) != stmtFollowsMap.end()) {
		return stmtFollowsMap.at(stmt).stmtFollowed;
	}
	return 0;
}

int FollowsStore::getFollowerOfStmt(int stmt) {
	if (stmtFollowsMap.find(stmt) != stmtFollowsMap.end()) {
		return stmtFollowsMap.at(stmt).stmtFollower;
	}
	return 0;
}

std::unordered_set<int> FollowsStore::getFollowedListOfStmt(int stmt) {
	if (stmtFollowsMap.find(stmt) != stmtFollowsMap.end()) {
		return stmtFollowsMap.at(stmt).listOfFollowed;
	}
	return {};
}

std::unordered_set<int> FollowsStore::getFollowerListOfStmt(int stmt) {
	if (stmtFollowsMap.find(stmt) != stmtFollowsMap.end()) {
		return stmtFollowsMap.at(stmt).listOfFollower;
	}
	return {};
}

std::unordered_set<std::pair<int, int>, IntPairHasher> FollowsStore::getAllFollowsPair() {
	return followsPairList;
}

std::unordered_set<std::pair<int, int>, IntPairHasher> FollowsStore::getAllFollowsStarPair() {
	return followsStarPairList;
}

std::unordered_set<int> FollowsStore::getAllFollowedStmt() {
	return followedList;
}

std::unordered_set<int> FollowsStore::getAllFollowerStmt() {
	return followerList;
}
