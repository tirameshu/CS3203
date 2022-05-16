#include "AffectsStore.h"

std::unordered_map<int, affectsStruct> AffectsStore::stmtAffectsMap; // a map containing the affectsStruct property regarding a specific assignment statement
std::unordered_set<std::pair<int, int>, IntPairHasher>  AffectsStore::affectsPairList; // stores all pairs <s1, s2> where Affects(s1, s2)
std::unordered_set<std::pair<int, int>, IntPairHasher>  AffectsStore::affectsStarPairList; // stores all pairs <s1, s2> where Affects*(s1, s2)
std::unordered_set<int>  AffectsStore::affectedList; // list of assign statements affected by another assign statement
std::unordered_set<int>  AffectsStore::affectingList; // list of assign statements affecting another assign statement

AffectsStore::AffectsStore() {}

void AffectsStore::clear() {
	stmtAffectsMap.clear();
	affectsPairList.clear();
	affectsStarPairList.clear();
	affectedList.clear();
	affectingList.clear();
}

// ============ SETTER METHODS ==============
bool AffectsStore::addAffects(int stmt, int affected) {
	// add stmt affectsStruct
	if (!stmtAffectsMap.emplace(stmt, affectsStruct{ {}, std::unordered_set<int>{affected}, {}, {} }).second) {
		// if stmt already has an affectsStruct in stmtAffectsMap
		stmtAffectsMap.at(stmt).affecting.emplace(affected);
	}

	// add affected affectsStruct
	if (!stmtAffectsMap.emplace(affected, affectsStruct{ std::unordered_set<int>{stmt}, {}, {}, {} }).second) {
		// if stmt already has an affectsStruct in stmtAffectsMap
		stmtAffectsMap.at(affected).affectedBy.emplace(stmt);
	}

	affectingList.emplace(stmt);
	affectedList.emplace(affected);
	
	return affectsPairList.emplace(std::make_pair(stmt, affected)).second;
}

bool AffectsStore::addAffectedByT(int stmt, std::unordered_set<int> listOfAffectors) {
	// should not add to non-existent statements
	if (stmtAffectsMap.count(stmt) == 0) {
		return false;
	}
	
	// for QE, Affects*(affector, stmt)
	for (const int& affector : listOfAffectors) {
		stmtAffectsMap.at(stmt).affectedByT.emplace(affector);
		stmtAffectsMap.at(affector).affectingT.emplace(stmt);
		affectsStarPairList.emplace(std::make_pair(affector, stmt));
	}

	return true;
}

bool AffectsStore::addAffectingT(int stmt, std::unordered_set<int> listOfAffecting) {
	// should not add to non-existent statements
	if (stmtAffectsMap.count(stmt) == 0) {
		return false;
	}

	// for QE, Affects*(stmt, affecting)
	for (const int& affecting : listOfAffecting) {
		stmtAffectsMap.at(stmt).affectingT.emplace(affecting);
		stmtAffectsMap.at(affecting).affectedByT.emplace(stmt);
		affectsStarPairList.emplace(std::make_pair(stmt, affecting));
	}

	return true;
}

// ============ QUERY METHODS ==============
bool AffectsStore::stmtHasAffectedBy(int stmt) {
	// if stmt is affectBy another stmt, it will exist in the affectedList
	return (affectedList.find(stmt) != affectedList.end());
}

bool AffectsStore::stmtHasAffecting(int stmt) {
	// if stmt is affecting another stmt, it will exist in the affectingList
	return (affectingList.find(stmt) != affectingList.end());
}

bool AffectsStore::programHasAffected() {
	return !affectedList.empty();
}

bool AffectsStore::programHasAffecting() {
	return !affectingList.empty();
}

// ============ GETTER METHODS ==============

std::unordered_map<int, affectsStruct> AffectsStore::getStmtAffectsMap() {
	return stmtAffectsMap;
}

std::unordered_set<std::pair<int, int>, IntPairHasher> AffectsStore::getAffectsPairList() {
	return affectsPairList;
}

std::unordered_set<std::pair<int, int>, IntPairHasher> AffectsStore::getAffectsStarPairList() {
	return affectsStarPairList;
}

std::unordered_set<int> AffectsStore::getAffectedList() {
	return affectedList;
}

std::unordered_set<int> AffectsStore::getAffectingList() {
	return affectingList;
}

std::unordered_set<int> AffectsStore::getAffectedByOfStmt(int stmt) {
	if (stmtAffectsMap.find(stmt) != stmtAffectsMap.end()) {
		return stmtAffectsMap.at(stmt).affectedBy;
	}
	return {};
}

std::unordered_set<int> AffectsStore::getAffectingOfStmt(int stmt) {
	if (stmtAffectsMap.find(stmt) != stmtAffectsMap.end()) {
		return stmtAffectsMap.at(stmt).affecting;
	}
	return {};
}

std::unordered_set<int> AffectsStore::getAffectedByTOfStmt(int stmt) {
	if (stmtAffectsMap.find(stmt) != stmtAffectsMap.end()) {
		return stmtAffectsMap.at(stmt).affectedByT;
	}
	return {};
}

std::unordered_set<int> AffectsStore::getAffectingTOfStmt(int stmt) {
	if (stmtAffectsMap.find(stmt) != stmtAffectsMap.end()) {
		return stmtAffectsMap.at(stmt).affectingT;
	}
	return {};
}
