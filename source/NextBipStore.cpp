#include "NextBipStore.h"
#include <iostream>

std::unordered_map<int, nextBipStruct> NextBipStore::stmtNextBipMap; // a map containing the Next properties regarding a specific statement
std::unordered_set<std::pair<int, int>, IntPairHasher> NextBipStore::nextBipPairList; // stores all pairs <s1, s2> where NextBip(s1, s2)
std::unordered_set<std::pair<int, int>, IntPairHasher> NextBipStore::nextBipStarPairList; // stores all pairs <s1, s2> where NextBip*(s1, s2)
std::unordered_set<int> NextBipStore::prevBipList; // a set of all statements that comes before a node in NextBip CFG
std::unordered_set<int> NextBipStore::nextBipList; // a set of all statements that comes after a node in NextBip CFG

NextBipStore::NextBipStore() {}

void NextBipStore::clear() {
	stmtNextBipMap.clear();
	nextBipPairList.clear();
	nextBipStarPairList.clear();
	prevBipList.clear();
	nextBipList.clear();
}

// ============ SETTER METHODS ==============
bool NextBipStore::addNextBip(int stmt1, int stmt2, int caller) {
	// only if caller != 0, then add into list of incoming and outgoing sets for stmt2 and stmt1 respectively
	// caller would usually be 0, unless interprocedural edges are added
	if (caller != 0) {
		// adding or updating of nextBipStruct for stmt1
		// add caller as outgoing edge from stmt1
		if (!stmtNextBipMap.emplace(stmt1, nextBipStruct{ {}, {stmt2}, {}, {}, {}, {caller} }).second) {
			// if nextBipStruct for stmt1 already exists, add stmt2 to list of next bip stmts
			stmtNextBipMap.at(stmt1).nextBipStmts.emplace(stmt2);
			stmtNextBipMap.at(stmt1).listOfOutgoing.emplace(caller);
		}

		// add caller as incoming edge to stmt2
		if (!stmtNextBipMap.emplace(stmt2, nextBipStruct{ {stmt1}, {}, {}, {}, {caller}, {} }).second) {
			// if nextBipStruct for stmt2 already exists, add stmt1 to list of prev bip stmts
			stmtNextBipMap.at(stmt2).prevBipStmts.emplace(stmt1);
			stmtNextBipMap.at(stmt2).listOfIncoming.emplace(caller);
		}
	}
	// if callers == 0, don't add to incoming or outgoing list
	else {
		if (!stmtNextBipMap.emplace(stmt1, nextBipStruct{ {}, {stmt2}, {}, {}, {}, {} }).second) {
			// if nextBipStruct for stmt1 already exists, add stmt2 to list of next bip stmts
			stmtNextBipMap.at(stmt1).nextBipStmts.emplace(stmt2);
		}
		if (!stmtNextBipMap.emplace(stmt2, nextBipStruct{ std::unordered_set<int>{stmt1}, {}, {}, {}, {}}).second) {
			// if nextBipStruct for stmt2 already exists, add stmt1 to list of prev bip stmts
			stmtNextBipMap.at(stmt2).prevBipStmts.emplace(stmt1);
		}
	}

	prevBipList.emplace(stmt1);
	nextBipList.emplace(stmt2);

	return nextBipPairList.emplace(std::make_pair(stmt1, stmt2)).second;
}

// called by DE
bool NextBipStore::addAllPreviousBip(int stmt, std::unordered_set<int> listOfPrevBipStarStmt) {
	// should not add to non-existent statements
	if (stmtNextBipMap.count(stmt) == 0) {
		return false;
	}

	// should not add to root nodes 
	if (stmtNextBipMap.at(stmt).prevBipStmts.empty()) {
		return false;
	}


	for (const int& prev : listOfPrevBipStarStmt) {
		// add prev to statement's nextBipStruct listOfPrevBipStarStmt
		stmtNextBipMap.at(stmt).listOfPrevBipStarStmt.emplace(prev);
		stmtNextBipMap.at(prev).listOfNextBipStarStmt.emplace(stmt);

		// add pairings to nextBipStarPairList
		nextBipStarPairList.emplace(std::make_pair(prev, stmt));
	}

	return true;
}

// called by DE
bool NextBipStore::addAllNextBip(int stmt, std::unordered_set<int> listOfNextBipStarStmt) {
	// should not add to non-existent statements
	if (stmtNextBipMap.count(stmt) == 0) {
		return false;
	}

	// should not add to leaf nodes 
	if (stmtNextBipMap.at(stmt).nextBipStmts.empty()) {
		return false;
	}

	for (const int& next : listOfNextBipStarStmt) {
		// add next to statement's nextBipStruct listOfNextBipStarStmt
		stmtNextBipMap.at(stmt).listOfNextBipStarStmt.emplace(next);
		stmtNextBipMap.at(next).listOfPrevBipStarStmt.emplace(stmt);

		// add pairings to nextBipStarPairList
		nextBipStarPairList.emplace(std::make_pair(stmt, next));
	}

	return true;
}

// ============ QUERY METHODS ==============
bool NextBipStore::stmtHasPrevBip(int stmt) {
	return (nextBipList.find(stmt) != nextBipList.end());
}

bool NextBipStore::stmtHasNextBip(int stmt) {
	return (prevBipList.find(stmt) != prevBipList.end());
}

bool NextBipStore::programHasPrevBipStmt() {
	return !prevBipList.empty();
}

bool NextBipStore::programHasNextBipStmt() {
	return !nextBipList.empty();
}

// ============ GETTER METHODS ==============
std::unordered_set<int> NextBipStore::getPrevBipOfStmt(int stmt) {
	if (stmtNextBipMap.find(stmt) != stmtNextBipMap.end()) {
		return stmtNextBipMap.at(stmt).prevBipStmts;
	}
	return {};
}

std::unordered_set<int> NextBipStore::getNextBipOfStmt(int stmt) {
	if (stmtNextBipMap.find(stmt) != stmtNextBipMap.end()) {
		return stmtNextBipMap.at(stmt).nextBipStmts;
	}
	return {};
}

std::unordered_set<int> NextBipStore::getPrevBipStarListOfStmt(int stmt) {
	if (stmtNextBipMap.find(stmt) != stmtNextBipMap.end()) {
		return stmtNextBipMap.at(stmt).listOfPrevBipStarStmt;
	}
	return {};
}

std::unordered_set<int> NextBipStore::getNextBipStarListOfStmt(int stmt) {
	if (stmtNextBipMap.find(stmt) != stmtNextBipMap.end()) {
		return stmtNextBipMap.at(stmt).listOfNextBipStarStmt;
	}
	return {};
}

std::unordered_set<int> NextBipStore::getNextBipListOfIncomingOfStmt(int stmt) {
	if (stmtNextBipMap.find(stmt) != stmtNextBipMap.end()) {
		return stmtNextBipMap.at(stmt).listOfIncoming;
	}
	return {};
}

std::unordered_set<int> NextBipStore::getNextBipListOfOutgoingOfStmt(int stmt) {
	if (stmtNextBipMap.find(stmt) != stmtNextBipMap.end()) {
		return stmtNextBipMap.at(stmt).listOfOutgoing;
	}
	return {};
}

std::unordered_set<std::pair<int, int>, IntPairHasher> NextBipStore::getAllNextBipPair() {
	return nextBipPairList;
}

std::unordered_set<std::pair<int, int>, IntPairHasher> NextBipStore::getAllNextBipStarPair() {
	return nextBipStarPairList;
}

std::unordered_set<int> NextBipStore::getAllPreviousBipStmt() {
	return prevBipList;
}

std::unordered_set<int> NextBipStore::getAllNextBipStmt() {
	return nextBipList;
}
