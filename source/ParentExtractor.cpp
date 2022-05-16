#include "ParentExtractor.h"

std::unordered_set<int> ParentExtractor::computeAllAncestors(int stmt) {
    // 1. Current stmt pulls the "ancestors" from direct parent stmt and add parent to it.
    // 2. If current stmt does not have a parent stmt, then it leaves allAncestors list as empty.

    int parent = 0; // default no parent
    std::unordered_set<int> allAncestors;

    if (PKB::stmtHasParent(stmt)) {
        parent = PKB::getParentOfStmt(stmt);
        allAncestors = PKB::getAncestorsListOfStmt(parent); // list of parent's ancestors

        // add current parent
        allAncestors.insert(parent); // last added element is the stmt directly followed

        return allAncestors;

    }
    else {
        // no parent
        // ensure this is empty for induction to be true
        assert(PKB::getAncestorsListOfStmt(stmt).size() == 0);

        return PKB::getAncestorsListOfStmt(stmt); // by default empty set
    }
}

std::unordered_set<int> ParentExtractor::computeAllDescendants(int stmt) {
    // 1. Current stmt pulls the "descendents" from *every direct child* and adds all children to it.
    //        NOTE: this is different from all other relations! stmt can have multiple direct children!
    // 2. If current stmt does not have children, then it leaves allDescendents list as empty.

    std::unordered_set<int> children;
    std::unordered_set<int> allDescendents;

    if (PKB::stmtHasChildren(stmt)) {
        children = PKB::getChildrenListOfStmt(stmt);

        for (auto& child : children) {
            std::unordered_set<int> descendantsOfChild = PKB::getDescendantsListOfStmt(child);
            allDescendents.insert(descendantsOfChild.begin(), descendantsOfChild.end()); // "union": range insert
            allDescendents.insert(child);
        }

        return allDescendents;

    }
    else {
        // no children
        // ensure this is empty for induction to be true
        assert(PKB::getDescendantsListOfStmt(stmt).size() == 0);

        return PKB::getDescendantsListOfStmt(stmt); // by default empty set
    }
}
