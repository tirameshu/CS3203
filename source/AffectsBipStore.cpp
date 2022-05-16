#include "AffectsBipStore.h"

std::unordered_map<int, affectsBipStruct> AffectsBipStore::stmtAffectsBipMap; // a map containing the affectsBipStruct property regarding a specific assignment statement
std::unordered_set<std::pair<int, int>, IntPairHasher>  AffectsBipStore::affectsBipPairList; // stores all pairs <s1, s2> where AffectsBip(s1, s2)
std::unordered_set<std::pair<int, int>, IntPairHasher>  AffectsBipStore::affectsBipStarPairList; // stores all pairs <s1, s2> where AffectsBip*(s1, s2)
std::unordered_set<int>  AffectsBipStore::affectedBipList; // list of assign statements affected by another assign statement
std::unordered_set<int>  AffectsBipStore::affectingBipList; // list of assign statements affecting another assign statement

AffectsBipStore::AffectsBipStore() {}

void AffectsBipStore::clear() {
	stmtAffectsBipMap.clear();
	affectsBipPairList.clear();
	affectsBipStarPairList.clear();
	affectedBipList.clear();
	affectingBipList.clear();
}

// ============ SETTER METHODS ==============
bool AffectsBipStore::addAffectsBip(int stmt, int affected) {
	// add stmt affectsStruct
	if (!stmtAffectsBipMap.emplace(stmt, affectsBipStruct{ {}, std::unordered_set<int>{affected}, {}, {} }).second) {
		// if stmt already has an affectsStruct in stmtAffectsMap
		stmtAffectsBipMap.at(stmt).affectingBip.emplace(affected);
	}

	// add affected affectsBipStruct
	if (!stmtAffectsBipMap.emplace(affected, affectsBipStruct{ std::unordered_set<int>{stmt}, {}, {}, {} }).second) {
		// if stmt already has an affectsStruct in stmtAffectsMap
		stmtAffectsBipMap.at(affected).affectedBipBy.emplace(stmt);
	}

	affectingBipList.emplace(stmt);
	affectedBipList.emplace(affected);

	return affectsBipPairList.emplace(std::make_pair(stmt, affected)).second;
}

bool AffectsBipStore::addAffectedBipByT(int stmt, std::unordered_set<int> listOfAffectors) {
	// should not add to non-existent statements
	if (stmtAffectsBipMap.count(stmt) == 0) {
		return false;
	}

	// for QE, AffectsBip*(affector, stmt)
	for (const int& affector : listOfAffectors) {
		stmtAffectsBipMap.at(stmt).affectedBipByT.emplace(affector);
		stmtAffectsBipMap.at(affector).affectingBipT.emplace(stmt);
		affectsBipStarPairList.emplace(std::make_pair(affector, stmt));
	}

	return true;
}

bool AffectsBipStore::addAffectingBipT(int stmt, std::unordered_set<int> listOfAffecting) {
	// should not add to non-existent statements
	if (stmtAffectsBipMap.count(stmt) == 0) {
		return false;
	}

	// for QE, AffectsBip*(stmt, affecting)
	for (const int& affecting : listOfAffecting) {
		stmtAffectsBipMap.at(stmt).affectingBipT.emplace(affecting);
		stmtAffectsBipMap.at(affecting).affectedBipByT.emplace(stmt);
		affectsBipStarPairList.emplace(std::make_pair(stmt, affecting));
	}

	return true;
}

// ============ QUERY METHODS ==============
bool AffectsBipStore::stmtHasAffectedBipBy(int stmt) {
	// if stmt is affectBy another stmt, it will exist in the affectedList
	return (affectedBipList.find(stmt) != affectedBipList.end());
}

bool AffectsBipStore::stmtHasAffectingBip(int stmt) {
	// if stmt is affecting another stmt, it will exist in the affectingList
	return (affectingBipList.find(stmt) != affectingBipList.end());
}

bool AffectsBipStore::programHasAffectedBip() {
	return !affectedBipList.empty();
}

bool AffectsBipStore::programHasAffectingBip() {
	return !affectingBipList.empty();
}

// ============ GETTER METHODS ==============

std::unordered_map<int, affectsBipStruct> AffectsBipStore::getStmtAffectsBipMap() {
	return stmtAffectsBipMap;
}

std::unordered_set<std::pair<int, int>, IntPairHasher> AffectsBipStore::getAffectsBipPairList() {
	return affectsBipPairList;
}

std::unordered_set<std::pair<int, int>, IntPairHasher> AffectsBipStore::getAffectsBipStarPairList() {
	return affectsBipStarPairList;
}

std::unordered_set<int> AffectsBipStore::getAffectedBipList() {
	return affectedBipList;
}

std::unordered_set<int> AffectsBipStore::getAffectingBipList() {
	return affectingBipList;
}

std::unordered_set<int> AffectsBipStore::getAffectedBipByOfStmt(int stmt) {
	if (stmtAffectsBipMap.find(stmt) != stmtAffectsBipMap.end()) {
		return stmtAffectsBipMap.at(stmt).affectedBipBy;
	}
	return {};
}

std::unordered_set<int> AffectsBipStore::getAffectingBipOfStmt(int stmt) {
	if (stmtAffectsBipMap.find(stmt) != stmtAffectsBipMap.end()) {
		return stmtAffectsBipMap.at(stmt).affectingBip;
	}
	return {};
}

std::unordered_set<int> AffectsBipStore::getAffectedBipByTOfStmt(int stmt) {
	if (stmtAffectsBipMap.find(stmt) != stmtAffectsBipMap.end()) {
		return stmtAffectsBipMap.at(stmt).affectedBipByT;
	}
	return {};
}

std::unordered_set<int> AffectsBipStore::getAffectingBipTOfStmt(int stmt) {
	if (stmtAffectsBipMap.find(stmt) != stmtAffectsBipMap.end()) {
		return stmtAffectsBipMap.at(stmt).affectingBipT;
	}
	return {};
}
