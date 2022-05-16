#include "NextStore.h"
#include <iostream>

std::unordered_map<int, nextStruct> NextStore::stmtNextMap; // a map containing the Next properties regarding a specific statement
std::unordered_set<std::pair<int, int>, IntPairHasher> NextStore::nextPairList; // stores all pairs <s1, s2> where Next(s1, s2)
std::unordered_set<std::pair<int, int>, IntPairHasher> NextStore::nextStarPairList; // stores all pairs <s1, s2> where Next*(s1, s2)
std::unordered_set<int> NextStore::prevList; // a set of all statements that comes before a node in CFG
std::unordered_set<int> NextStore::nextList; // a set of all statements that comes after a node in CFG

NextStore::NextStore() {}

void NextStore::clear() {
	stmtNextMap.clear();
	nextPairList.clear();
	nextStarPairList.clear();
	prevList.clear();
	nextList.clear();
}

// ============ SETTER METHODS ==============
bool NextStore::addNext(int stmt1, int stmt2) {
	// adding or updating of nextStruct for stmt1
	if (!stmtNextMap.emplace(stmt1, nextStruct{ {}, std::unordered_set<int>{stmt2}, {}, {} }).second) {
		// if nextStruct for stmt1 already exists, add stmt2 to list of next stmts
		stmtNextMap.at(stmt1).nextStmts.emplace(stmt2);
	}
	// adding or updating of nextStruct for stmt2
	if (!stmtNextMap.emplace(stmt2, nextStruct{ std::unordered_set<int>{stmt1}, {}, {}, {} }).second) {
		// if nextStruct for stmt2 already exists, add stmt1 to list of prev stmts
		stmtNextMap.at(stmt2).prevStmts.emplace(stmt1);
	}

	prevList.emplace(stmt1);
	nextList.emplace(stmt2);

	return nextPairList.emplace(std::make_pair(stmt1, stmt2)).second;
}

// called by DE
bool NextStore::addAllPrevious(int stmt, std::unordered_set<int> listOfPrevStarStmt) {
	// should not add to non-existent statements
	if (stmtNextMap.count(stmt) == 0) {
		return false;
	}
	
	// should not add to root nodes 
	if (stmtNextMap.at(stmt).prevStmts.empty()) {
		return false;
	}
	

	for (const int& prev : listOfPrevStarStmt) {
		// add prev to statement's nextStruct listOfPrevStarStmt
		stmtNextMap.at(stmt).listOfPrevStarStmt.emplace(prev);
		stmtNextMap.at(prev).listOfNextStarStmt.emplace(stmt);

		// add pairings to nextStarPairList
		nextStarPairList.emplace(std::make_pair(prev, stmt));
	}

	return true;
}

// called by DE
bool NextStore::addAllNext(int stmt, std::unordered_set<int> listOfNextStarStmt) {
	// should not add to non-existent statements
	if (stmtNextMap.count(stmt) == 0) {
		return false;
	}

	// should not add to leaf nodes 
	if (stmtNextMap.at(stmt).nextStmts.empty()) {
		return false;
	}

	for (const int& next : listOfNextStarStmt) {
		// add next to statement's nextStruct listOfNextStarStmt
		stmtNextMap.at(stmt).listOfNextStarStmt.emplace(next);
		stmtNextMap.at(next).listOfPrevStarStmt.emplace(stmt);

		// add pairings to nextStarPairList
		nextStarPairList.emplace(std::make_pair(stmt, next));
	}

	return true;
}

// ============ QUERY METHODS ==============
bool NextStore::stmtHasPrev(int stmt) {
	return (nextList.find(stmt) != nextList.end());
}

bool NextStore::stmtHasNext(int stmt) {
	return (prevList.find(stmt) != prevList.end());
}

bool NextStore::programHasPrevStmt() {
	return !prevList.empty();
}

bool NextStore::programHasNextStmt() {
	return !nextList.empty();
}

// ============ GETTER METHODS ==============
std::unordered_set<int> NextStore::getPrevOfStmt(int stmt) {
	if (stmtNextMap.find(stmt) != stmtNextMap.end()) {
		return stmtNextMap.at(stmt).prevStmts;
	}
	return {};
}

std::unordered_set<int> NextStore::getNextOfStmt(int stmt) {
	if (stmtNextMap.find(stmt) != stmtNextMap.end()) {
		return stmtNextMap.at(stmt).nextStmts;
	}
	return {};
}

std::unordered_set<int> NextStore::getPrevStarListOfStmt(int stmt) {
	if (stmtNextMap.find(stmt) != stmtNextMap.end()) {
		return stmtNextMap.at(stmt).listOfPrevStarStmt;
	}
	return {};
}

std::unordered_set<int> NextStore::getNextStarListOfStmt(int stmt) {
	if (stmtNextMap.find(stmt) != stmtNextMap.end()) {
		return stmtNextMap.at(stmt).listOfNextStarStmt;
	}
	return {};
}

std::unordered_set<std::pair<int, int>, IntPairHasher> NextStore::getAllNextPair() {
	return nextPairList;
}

std::unordered_set<std::pair<int, int>, IntPairHasher> NextStore::getAllNextStarPair() {
	return nextStarPairList;
}

std::unordered_set<int> NextStore::getAllPreviousStmt() {
	return prevList;
}

std::unordered_set<int> NextStore::getAllNextStmt() {
	return nextList;
}
