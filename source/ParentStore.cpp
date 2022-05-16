# include "ParentStore.h"

std::unordered_map<int, parentStruct> ParentStore::stmtParentMap; // a map containing the parent properties regarding a specific statement
std::unordered_set<std::pair<int, int>, IntPairHasher> ParentStore::parentPairList; // stores all pairs <s1, s2> where Parent(s1, s2)
std::unordered_set<std::pair<int, int>, IntPairHasher> ParentStore::parentStarPairList; // stores all pairs <s1, s2> where Parent*(s1, s2)
std::unordered_set<int> ParentStore::parentList; // a set of all statements that are parent
std::unordered_set<int> ParentStore::childList; // a set of all statements that are child

ParentStore::ParentStore() {}

void ParentStore::clear() {
	stmtParentMap.clear();
	parentPairList.clear();
	parentStarPairList.clear();
	parentList.clear();
	childList.clear();
}

// ============ SETTER METHODS ==============
bool ParentStore::addParent(int parent, std::unordered_set<int> childrenList) {
	// adding or updating of parentStruct for parent stmt
	if (!stmtParentMap.emplace(parent, parentStruct{ 0, childrenList, {}, {} }).second) {
		// if parentStruct for parent already exists
		stmtParentMap.at(parent).listOfChildren = childrenList;
	}
	parentList.emplace(parent);

	bool result_flag = true;
	// adding or updating of parentStruct for each child stmt
	for (const int& child : childrenList) {
		if (child < parent) {
			stmtParentMap.erase(parent);
			return false;
		}
		if (!stmtParentMap.emplace(child, parentStruct{ parent, {}, {}, {} }).second) {
			stmtParentMap.at(child).stmtParent = parent;
		}
		childList.emplace(child);

		// add pairings to parentPairList
		result_flag = result_flag && parentPairList.emplace(std::make_pair(parent, child)).second;
	}

	return result_flag;
}

bool ParentStore::addAncestors(int stmt, std::unordered_set<int> ancestorList) {
	if (stmtParentMap.at(stmt).listOfAncestors.size() != 0) {
		return false;
	}

	for (const int& ancestor : ancestorList) {
		// add pairings to parentStarPairList
		parentStarPairList.emplace(std::make_pair(ancestor, stmt));
	}

	stmtParentMap.at(stmt).listOfAncestors = ancestorList;
	return true;
}

bool ParentStore::addDescendants(int stmt, std::unordered_set<int> descendantList) {
	if (stmtParentMap.at(stmt).listOfDescendants.size() != 0) {
		return false;
	}

	for (const int& descendant : descendantList) {
		// add pairings to parentStarPairList
		parentStarPairList.emplace(std::make_pair(stmt, descendant));
	}

	stmtParentMap.at(stmt).listOfDescendants = descendantList;
	return true;
}

// ============ QUERY METHODS ==============
bool ParentStore::stmtHasParent(int stmt) {
	return (childList.find(stmt) != childList.end());
}

bool ParentStore::stmtHasChildren(int stmt) {
	return (parentList.find(stmt) != parentList.end());
}

bool ParentStore::programHasParentStmt() {
	return !parentList.empty();
}

bool ParentStore::programHasChildStmt() {
	return !childList.empty();
}

// ============ GETTER METHODS ==============
int ParentStore::getParentOfStmt(int stmt) {
	if (stmtParentMap.find(stmt) != stmtParentMap.end()) {
		return stmtParentMap.at(stmt).stmtParent;
	}
	return 0;
}

std::unordered_set<int> ParentStore::getChildrenListOfStmt(int stmt) {
	if (stmtParentMap.find(stmt) != stmtParentMap.end()) {
		return stmtParentMap.at(stmt).listOfChildren;
	}
	return {};
}

std::unordered_set<int> ParentStore::getAncestorsListOfStmt(int stmt) {
	if (stmtParentMap.find(stmt) != stmtParentMap.end()) {
		return stmtParentMap.at(stmt).listOfAncestors;
	}
	return {};
}

std::unordered_set<int> ParentStore::getDescendantsListOfStmt(int stmt) {
	if (stmtParentMap.find(stmt) != stmtParentMap.end()) {
		return stmtParentMap.at(stmt).listOfDescendants;
	}
	return {};
}

std::unordered_set<std::pair<int, int>, IntPairHasher> ParentStore::getAllParentPair() {
	return parentPairList;
}

std::unordered_set<std::pair<int, int>, IntPairHasher> ParentStore::getAllParentStarPair() {
	return parentStarPairList;
}

std::unordered_set<int> ParentStore::getAllParentStmt() {
	return parentList;
}

std::unordered_set<int> ParentStore::getAllChildStmt() {
	return childList;
}
