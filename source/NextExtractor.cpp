#include "NextExtractor.h"

std::unordered_set<int> NextExtractor::computeAllPrevious(int stmt) {
    // 1. Get statements that are executed before
    //    1.1 Current stmt pulls the "previous" from prev stmt and adds prev stmt to it.
    //    1.2 If current stmt does not have a prev stmt, then it leaves previousList as empty.

    std::unordered_set<int> directPrevious;
    std::unordered_set<int> allPrevious;

    if (PKB::stmtHasPrev(stmt)) {
        directPrevious = PKB::getPrevOfStmt(stmt);

        if (PKB::getTypeOfStmt(stmt) == 5) { // stmt is a while stmt
            allPrevious.insert(stmt);
            std::unordered_set<int> childrenOfWhile = PKB::getDescendantsListOfStmt(stmt);
            allPrevious.insert(childrenOfWhile.begin(), childrenOfWhile.end());
        }

        // it is possible for a stmt to come before itself, eg empty while loop
        for (int previous : directPrevious) {
            if (previous == stmt) {
                continue;
            }
            // previous has while as ancestor: stmt in a while loop
            else if (DesignExtractorUtil::anyAncestorOfType(previous, 5)) {
                // if it has multiple direct prev,
                // all its other direct previous must be in the while loop
                // because the only time a stmt can have multiple previous is when
                // it is in a (possibly nested) if-else clause
                // and if nested, no normal stmt can come in between the if-else layers (because direct)
                // so after adding the stmtList in the while loop, just need add previousStar of the while stmt

                // add all stmts in stmtList
                std::unordered_set<int> childrenOfWhile = PKB::getDescendantsListOfStmt(previous);
                allPrevious.insert(childrenOfWhile.begin(), childrenOfWhile.end());
            }

            std::unordered_set<int> previousStar = PKB::getPrevStarListOfStmt(previous);
            allPrevious.insert(previousStar.begin(), previousStar.end());
            allPrevious.insert(previous);
        }

        return allPrevious;

    }
    else {
        // no stmt comes before
        // ensure this is empty for induction to be true
        assert(PKB::getPrevOfStmt(stmt).empty());

        return PKB::getPrevOfStmt(stmt); // by default empty set
    }
}

std::unordered_set<int> NextExtractor::computeAllNext(int stmt) {
    // 1. Get statements that are executed after
    //    1.1 Current stmt pulls the "next" from next stmt and adds next stmt to it.
    //    1.2 If current stmt does not have a next stmt, then it leaves nextList as empty.

    std::unordered_set<int> directNext;
    std::unordered_set<int> allNext;

    if (PKB::stmtHasNext(stmt)) {
        directNext = PKB::getNextOfStmt(stmt);

        if (PKB::getTypeOfStmt(stmt) == 5) { // stmt is a while stmt
            allNext.insert(stmt);
            std::unordered_set<int> childrenOfWhile = PKB::getDescendantsListOfStmt(stmt);
            allNext.insert(childrenOfWhile.begin(), childrenOfWhile.end());
        }

        // it is possible for a stmt to come before itself, eg empty while loop
        for (int next : directNext) {
            if (next == stmt) {
                allNext.insert(stmt);
                continue;
            }
            // previous has while as ancestor: stmt in a while loop
            else if (DesignExtractorUtil::anyAncestorOfType(next, 5)) {
                // if it has multiple direct prev,
                // all its other direct previous must be in the while loop
                // because the only time a stmt can have multiple previous is when
                // it is in a (possibly nested) if-else clause
                // and if nested, no normal stmt can come in between the if-else layers (because direct)
                // so after adding the stmtList in the while loop, just need add previousStar of the while stmt

                // add all stmts in stmtList
                std::unordered_set<int> childrenOfWhile = PKB::getDescendantsListOfStmt(next);
                allNext.insert(childrenOfWhile.begin(), childrenOfWhile.end());

                // add previousStar of while
                std::unordered_set<int> nextStarListOfStmt = PKB::getNextStarListOfStmt(next);
                allNext.insert(nextStarListOfStmt.begin(), nextStarListOfStmt.end());

                // still need add direct next, could be outside
                allNext.insert(next);
            }
            // can have multiple direct Next even if one of them is a while stmt
            // the while can be nested in the "else" part 
            std::unordered_set<int> nextStarListOfStmt = PKB::getNextStarListOfStmt(next);
            allNext.insert(nextStarListOfStmt.begin(), nextStarListOfStmt.end());
            allNext.insert(next);
        }

        return allNext;

    }
    else {
        // no stmt comes after
        // ensure this is empty for induction to be true
        assert(PKB::getNextOfStmt(stmt).empty());

        return PKB::getNextOfStmt(stmt); // by default empty set
    }
}