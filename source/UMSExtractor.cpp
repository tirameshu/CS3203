#include "UMSExtractor.h"

bool UMSExtractor::updateAncestorsUse(std::string procedure) {
    // 5. Update ancestors on use
    //    5.1 Get all the vars used by a stmt
    //    5.2 Access stmt ancestors to add vars to UsesStore for ancestors
    std::unordered_map<int, std::unordered_set<std::string>> stmtToVarsUsed = PKB::getStmtUsesMap();

    for (auto& stmt : stmtToVarsUsed) {
        int stmtNo = stmt.first;
        std::unordered_set<std::string> varsUsed = stmt.second;
        DesignExtractorUtil::updateAncestorsOfStmt(stmtNo, varsUsed, "Uses");
    }

    return true;
}

// Updates ancestors with vars modified by children
bool UMSExtractor::updateAncestorsModify(std::string procedure) {
    // 6. Update ancestors on modify
    //    6.1 Get all the vars modified by a stmt
    //    6.2 Access stmt ancestors to add vars to ModifiesStore for ancestors
    std::unordered_map<int, std::unordered_set<std::string>> stmtToVarsModified = PKB::getStmtModifiesMap();

    for (auto& stmt : stmtToVarsModified) {
        int stmtNo = stmt.first;
        std::unordered_set<std::string> varsModified = stmt.second;
        DesignExtractorUtil::updateAncestorsOfStmt(stmtNo, varsModified, "Modifies");
    }

    return true;
}