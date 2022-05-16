#include "DesignExtractor.h"
#include "NextBipProcessor.h"
#include "NextBipStarProcessor.h"

#include "CallsExtractor.h"
#include "FollowsExtractor.h"
#include "NextExtractor.h"
#include "ParentExtractor.h"
#include "UMSExtractor.h"
#include "UMPExtractor.h"
#include "AffectsExtractor.h"
#include "AffectsBipProcessor.h"

bool DesignExtractor::processProgram() {
    // process procedures first, establish call relationships for Uses/Modifies updating
    if (!processProcedures()) {
        return false;
    }

    // < proc_name: <stmtlist>, proc_name: <stmtlist>, ... >
    std::unordered_map<std::string, std::vector<int>> procedureStmtMap = PKB::getProcedureStmtMap();
  
    for (auto& procedure : procedureStmtMap) {
        std::vector<int> stmtList = procedure.second;
    
        if (!processStatements(procedure.first, stmtList)) { // if any fails
            std::cout << "procedure " << procedure.first << " failed\n";
            return false;
        }
    }

    if (!updateProcedureUsesAndModifies()) {
        return false;
    }

    for (auto& procedure : procedureStmtMap) {
        std::vector<int> stmtList = procedure.second;
        int current = *stmtList.begin();

        computeAffects(current, 0);
        computeAffectsT(stmtList);
        clearAffectsData();
    }

    // Roby's methods
    //NextBipProcessor::calculateNextBip();
    //NextBipStarProcessor::calculateNextBipStar();
    //AffectsBipProcessor::calculateAffectsBip();
    
    return true;
  
}

bool DesignExtractor::processProcedures() {
    // 1. Validate first: check for cyclical calls with dfs.
    // 2. For each leaf procedure, dfs to update its callers with transitive callees

    // Validation
    try {
        validateProcedures();
    }
    catch (ParserException& e) {
        std::cout << e.getExceptionMessage();
        std::exit(0);
    }

    bool calleeFlag = true;
    bool callerFlag = true;

    std::unordered_set<std::string> leafProcedures = PKB::getLeafProcs();

    for (std::string procedure : leafProcedures) {
        calleeFlag &= computeAllCallees(procedure);
    }

    std::unordered_set<std::string> rootProcedures = PKB::getRootProcs();

    for (std::string procedure : rootProcedures) {
        callerFlag &= computeAllCallers(procedure);
    }

    return (calleeFlag && callerFlag);
}

// either returns True for valid program
// or throw Exception for cyclic
bool DesignExtractor::validateProcedures() {
    // assumes callsT relationships have been established

    if (PKB::hasCall()) {

        // 1. there are calling procs, but no root/ leaf -> cyclic
        if (PKB::getLeafProcs().empty() && PKB::getRootProcs().empty()) {
            throw ParserException("INVALID PROGRAM: NO ROOT AND NO LEAF", ParserExceptionEnum::PROCEDURE_EXCEPTION);
        }

        // has both leaves and roots
        else if (!PKB::getLeafProcs().empty() && !PKB::getRootProcs().empty()) {
            std::unordered_set<std::string> allCallProcs = PKB::getAllCallProcs();
            // init
            std::unordered_map<std::string, bool> visited;
            std::unordered_map<std::string, bool> inRecStack;
            for (auto& procedure : allCallProcs) {
                visited.emplace(procedure, false);
                inRecStack.emplace(procedure, false);
            }

            // recursive check on all call procs
            for (auto& procedure : allCallProcs) {
                // either false or exception
                DesignExtractorUtil::hasRecursion(procedure, visited, inRecStack);
            }
        }
        else {
            // 2. only leaves exists, or only roots exists -> cyclic
            throw ParserException("INVALID PROGRAM: NOT BOTH ROOT AND LEAVES EXIST", ParserExceptionEnum::PROCEDURE_EXCEPTION);
        }
    }

    // no calls -> no cyclic
    // !hasRecursion -> no cyclic
    return true;
}

bool DesignExtractor::processStatements(std::string procedure, std::vector<int> stmtList) {
    bool first_pass_flag = true;
    bool second_pass_flag = true;
    bool third_pass_flag = true;
    bool fourth_pass_flag = true;

    // First Pass (top down) for Followed, Ancestor
    for (auto& stmt : stmtList) {

        std::unordered_set<int> allFollowed = computeAllFollowed(stmt);
        std::unordered_set<int> allAncestors = computeAllAncestors(stmt);

        // if any error is encountered in storing to PKB
        // it is assumed that the function responsible will
        // print its own error message.

        if (!allAncestors.empty()) {
            first_pass_flag &= PKB::addAncestors(stmt, allAncestors);
        }

        if (!allFollowed.empty()) {
            first_pass_flag &= PKB::addAllFollowed(stmt, allFollowed);
        }
    }

    if (!first_pass_flag) {
        return false;
    }

    // Second Pass (bottom up) for Follows, Descendent
    // using iterator for reverse traversal
    for (std::vector<int>::reverse_iterator i = stmtList.rbegin(); i != stmtList.rend(); i++) {

        std::unordered_set<int> allFollowers = computeAllFollowers(*i); // dereference for value
        std::unordered_set<int> allDescendants = computeAllDescendants(*i);

        if (!allFollowers.empty()) {
            second_pass_flag &= PKB::addAllFollower(*i, allFollowers);
        }

        if (!allDescendants.empty()) {
            second_pass_flag &= PKB::addDescendants(*i, allDescendants);
        }
    }

    if (!second_pass_flag) {
        return false;
    }

    // Third Pass (top down) for Previous
    for (auto& stmt : stmtList) {

        std::unordered_set<int> allPrevious = computeAllPrevious(stmt);

        if (!allPrevious.empty()) {
            third_pass_flag &= PKB::addAllPrevious(stmt, allPrevious);
            assert(third_pass_flag);
        }
    }

    if (!third_pass_flag) {
        return false;
    }

    // Fourth Pass (bottom up) for Next
    for (std::vector<int>::reverse_iterator i = stmtList.rbegin(); i != stmtList.rend(); i++) {

        std::unordered_set<int> allNext = computeAllNext(*i);

        if (!allNext.empty()) {
            fourth_pass_flag &= PKB::addAllNext(*i, allNext);
        }
    }

    if (!fourth_pass_flag) {
        return false;
    }
  
    return updateAncestorsUseAndModify(procedure);
}

// CallsT
bool DesignExtractor::computeAllCallees(std::string procedure) {
    return CallsExtractor::computeAllCallees(procedure);
}

bool DesignExtractor::computeAllCallers(std::string procedure) {
    return CallsExtractor::computeAllCallers(procedure);
}

// FollowsT
std::unordered_set<int> DesignExtractor::computeAllFollowed(int stmt) {
    return FollowsExtractor::computeAllFollowed(stmt);
}

std::unordered_set<int> DesignExtractor::computeAllFollowers(int stmt) {
    return FollowsExtractor::computeAllFollowers(stmt);
}

// NextT
// requires Ancestors, Descendants to be computed
std::unordered_set<int> DesignExtractor::computeAllPrevious(int stmt) {
    return NextExtractor::computeAllPrevious(stmt);
}

std::unordered_set<int> DesignExtractor::computeAllNext(int stmt) {
    return NextExtractor::computeAllNext(stmt);
}

// ParentT
std::unordered_set<int> DesignExtractor::computeAllAncestors(int stmt) {
    return ParentExtractor::computeAllAncestors(stmt);
}

std::unordered_set<int> DesignExtractor::computeAllDescendants(int stmt) {
    return ParentExtractor::computeAllDescendants(stmt);
}

// Update ancestors with vars used and modified by children
// Within 1 procedure
bool DesignExtractor::updateAncestorsUseAndModify(std::string procedure) {
    return UMSExtractor::updateAncestorsUse(procedure) && UMSExtractor::updateAncestorsModify(procedure);
}

// Start from leaf callees
// Update their respective call *stmts*
// These stmts will update their ancestors and the proc
bool DesignExtractor::updateProcedureUsesAndModifies() {
    return UMPExtractor::updateProcedureUses() && UMPExtractor::updateProcedureModifies();
}

bool DesignExtractor::computeAffects(int current, int control) {
    return AffectsExtractor::computeAffects(current, control);
}

bool DesignExtractor::computeAffectsT(std::vector<int> stmtList) {
    return AffectsExtractor::computeAffectsT(stmtList);
}

void DesignExtractor::clearAffectsData() {
    AffectsExtractor::clearAffectsData();
}