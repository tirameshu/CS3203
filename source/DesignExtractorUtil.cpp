#include "DesignExtractorUtil.h"

bool DesignExtractorUtil::anyAncestorOfType(int stmt, int type) {
	std::unordered_set<int> ancestors = PKB::getAncestorsListOfStmt(stmt);
	for (auto& ancestor : ancestors) {
		if (PKB::getTypeOfStmt(ancestor) == type) {
			return true;
		}
	}
	return false;
}

bool DesignExtractorUtil::anyDescendantOfType(int stmt, int type) {
	std::unordered_set<int> descendants = PKB::getDescendantsListOfStmt(stmt);
	for (auto& descendant : descendants) {
		if (PKB::getTypeOfStmt(descendant) == type) {
			return true;
		}
	}
	return false;
}


bool DesignExtractorUtil::hasRecursion(std::string currentProc, std::unordered_map<std::string, bool> visited, std::unordered_map<std::string, bool> inRecStack)
{
    auto visitedIt = visited.find(currentProc);
    auto recStackIt = inRecStack.find(currentProc);
    assert(visitedIt != visited.end() && recStackIt != inRecStack.end());

    if (visitedIt->second == false)
    {
        // Mark the current node as visited and part of recursion stack 
        visitedIt->second = true;
        recStackIt->second = true;

        // Recurse on direct callees
        std::unordered_set<std::string> directCallees = PKB::getCalleesOfProc(currentProc);
        for (auto i = directCallees.begin(); i != directCallees.end(); ++i)
        {

            // recursing on callees
            if (!visited.at(*i) && hasRecursion(*i, visited, inRecStack)) {
                return true;
            }

            else if (inRecStack.at(*i)) {
                // this is the actual clause detecting the recursion
                throw ParserException("INVALID PROGRAM: CYCLIC CALL DETECTED BETWEEN PROCEDURES " + currentProc + ", " + (*i), ParserExceptionEnum::PROCEDURE_EXCEPTION);
                return true;
            }
        }
    }
    recStackIt->second = false;  // remove the vertex from recursion stack 
    return false;
}

bool DesignExtractorUtil::updateAncestorsOfStmt(int stmt, std::unordered_set<std::string> vars, std::string rel) {

    std::unordered_set<int> ancestors = PKB::getAncestorsListOfStmt(stmt);

    if (!vars.empty()) {
        for (auto& ancestor : ancestors) {
            // adding the transitive use to ancestor
            // automatically updates for the specific container type as well

            if (rel == "Uses") {
                if (!PKB::addUsesToContainer(ancestor, PKB::getTypeOfStmt(ancestor), vars)) {
                    return false;
                }
            }
            else if (rel == "Modifies") {
                if (!PKB::addModifiesToContainer(ancestor, PKB::getTypeOfStmt(ancestor), vars)) {
                    return false;
                }
            }
            else {
                return false;
            }

        }
    }

    return true;
}

// For UsesP, ModifiesP
// 1. Start at root.
// 2. Get all callerStmts of root.
// 3. Add vars used by root proc to callerStmts. Add to its ancestors as well.
// 4. CallerStmts will add these vars to its proc.
// 5. Recurse.
bool DesignExtractorUtil::updateTCallers(std::string procedure, std::unordered_set<std::string> varsToUpdate, std::string rel) {

    // direct call stmts
    std::unordered_set<int> callerStmts = PKB::getCallerStmts(procedure);

    // direct callers
    std::unordered_set<std::string> directCallerProcs = PKB::getCallersOfProc(procedure);

    if (!varsToUpdate.empty()) {
        for (auto& callStmt : callerStmts) {

            if (rel == "Uses") {
                // add to call stmt
                if (!PKB::addUsesToContainer(callStmt, PKB::getTypeOfStmt(callStmt), varsToUpdate)) {
                    return false;
                }

                // add to ancestors of call stmt
                if (!DesignExtractorUtil::updateAncestorsOfStmt(callStmt, varsToUpdate, "Uses")) {
                    return false;
                }
            }
            else if (rel == "Modifies") {
                // add to call stmt
                if (!PKB::addModifiesToContainer(callStmt, PKB::getTypeOfStmt(callStmt), varsToUpdate)) {
                    return false;
                }

                // add to ancestors of call stmt
                if (!DesignExtractorUtil::updateAncestorsOfStmt(callStmt, varsToUpdate, "Modifies")) {
                    return false;
                }
            }
        }
    }


    for (auto& caller : directCallerProcs) {
        std::unordered_set<std::string> cache = varsToUpdate;
        // add to caller procUsesMap
        if (rel == "Uses") {
            if (!varsToUpdate.empty()) {
                PKB::addProcUses(caller, varsToUpdate);
            }
            std::unordered_set<std::string> varsUsedByProc = PKB::getUsedVarsInProc(caller);
            varsToUpdate.insert(varsUsedByProc.begin(), varsUsedByProc.end());
        }
        else if (rel == "Modifies") {
            if (!varsToUpdate.empty()) {
                PKB::addModifiedVarToProc(caller, varsToUpdate);
            }
            std::unordered_set<std::string> varsModifiedByProc = PKB::getModifiedVarsInProc(caller);
            varsToUpdate.insert(varsModifiedByProc.begin(), varsModifiedByProc.end());
        }
        updateTCallers(caller, varsToUpdate, rel);
        varsToUpdate = cache;
    }

    return true;
}

int DesignExtractorUtil::getOutsideIfElse(int ifStmt) {
    int outsideIfElse = 0;

    // compute stmt outside if-else only if it exists
    if (PKB::getFollowerOfStmt(ifStmt) != 0) {
        std::unordered_set<int> stmtsInIfElse = PKB::getDescendantsListOfStmt(ifStmt);
        for (auto& stmt : stmtsInIfElse) {
            if (stmt > outsideIfElse) {
                outsideIfElse = stmt;
            }
        }
        outsideIfElse += 1;
    }
    else {
        if (!PKB::stmtHasParent(ifStmt)) {
            outsideIfElse = 0;
        }
        else {
            int parent = PKB::getParentOfStmt(ifStmt);
            if (PKB::getTypeOfStmt(parent) == StatementTypeEnum::IF) {
                outsideIfElse = getOutsideIfElse(parent);
            }
            else if (PKB::getTypeOfStmt(parent) == StatementTypeEnum::WHILE) {
                outsideIfElse = parent;
            }
        }
    }

    return outsideIfElse;
}

/*
* update beforeLMT for subsequent usage
*/
std::unordered_map<std::string, std::unordered_set<int>> DesignExtractorUtil::merge(std::unordered_map<std::string, std::unordered_set<int>> beforeLMT, std::unordered_map<std::string, std::unordered_set<int>> afterLMT) {
    // combine keys from both maps
    // if key in both maps but value differ, combine
    // if key absent in beforeLMT, add from LMT
    // if key absent in LMT, DO NOT erase from beforeLMT
    // - to preserve the cfg that does not pass through the while loop
    std::unordered_set<std::string> allVars;

    for (auto& var: beforeLMT) {
        allVars.insert(var.first);
    }

    for (auto& var: afterLMT) {
        allVars.insert(var.first);
    }

    for (auto& var : allVars) {
        if (beforeLMT.find(var) == beforeLMT.end()) {
            // new modified var in while
            beforeLMT.emplace(var, afterLMT.at(var));
        }
        else if (afterLMT.find(var) == afterLMT.end()) {
            continue;
        }
        else {
            // only other case is when both have the var
            std::unordered_set<int> modifyingStmts;
            modifyingStmts.insert(beforeLMT.at(var).begin(), beforeLMT.at(var).end());
            modifyingStmts.insert(afterLMT.at(var).begin(), afterLMT.at(var).end());

            // add to beforeLMT
            beforeLMT.at(var) = modifyingStmts;
        }
    }

    return beforeLMT;
}