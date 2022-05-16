#include "CallsExtractor.h"

// CallsT
bool CallsExtractor::computeAllCallees(std::string procedure) {
    // 1. Start at leaf. Get all callees of proc. Add proc to list.
    // 2. For each caller of proc, add all calleesTto it.
    // 3. For each caller, call this method if it too has a caller.
    //  3.1 Each callee updates its caller, order of updating should not matter

    // Cannot use DFS for this because we do want multiple leaf nodes to update the same caller node if needed

    std::unordered_set<std::string> allCallees = PKB::getTCalleesOfProc(procedure);

    std::unordered_set<std::string> directCallees = PKB::getCalleesOfProc(procedure);
    std::unordered_set<std::string> directCallers = PKB::getCallersOfProc(procedure);

    if (!PKB::hasCallee(procedure)) {
        assert(directCallees.empty() && allCallees.empty());
    }

    for (auto& callee : directCallees) {
        std::unordered_set<std::string> calleesT = PKB::getTCalleesOfProc(callee); // get all transitive callees
        allCallees.insert(calleesT.begin(), calleesT.end()); // "union": range insert
        allCallees.insert(callee);
    }

    if (!PKB::addCalleeT(procedure, allCallees)) {
        return false;
    }

    for (auto& caller : directCallers) {
        computeAllCallees(caller);
    }

    return true;
}


bool CallsExtractor::computeAllCallers(std::string procedure) {
    // 1. Start at root. Get all callers of root.
    // 2. For each callee of proc, add all callersT to it.
    // 3. For each callee, call this method if it too has a callee.

    std::unordered_set<std::string> allCallers = PKB::getTCallersOfProc(procedure);

    std::unordered_set<std::string> directCallers = PKB::getCallersOfProc(procedure);
    std::unordered_set<std::string> directCallees = PKB::getCalleesOfProc(procedure);

    if (!PKB::hasCaller(procedure)) {
        assert(directCallers.empty() && allCallers.empty());
    }
    else {
        assert(!directCallers.empty());
    }

    for (auto& caller : directCallers) {
        std::unordered_set<std::string> callersT = PKB::getTCallersOfProc(caller); // get all transitive callers
        allCallers.insert(callersT.begin(), callersT.end()); // "union": range insert
        allCallers.insert(caller);
    }

    if (!PKB::addCallerT(procedure, allCallers)) {
        return false;
    }

    for (auto& callee : directCallees) {
        computeAllCallers(callee);
    }

    return true;

}