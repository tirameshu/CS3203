#include "UMPExtractor.h"

bool UMPExtractor::updateProcedureUses() {

    std::unordered_set<std::string> leafProcedures = PKB::getLeafProcs();

    for (auto& procedure : leafProcedures) {
        assert(PKB::hasCaller(procedure));

        std::unordered_set<std::string> varsUsedByProc = PKB::getUsedVarsInProc(procedure);
        DesignExtractorUtil::updateTCallers(procedure, varsUsedByProc, "Uses");
    }

    return true;
}

bool UMPExtractor::updateProcedureModifies() {

    std::unordered_set<std::string> leafProcedures = PKB::getLeafProcs();

    for (auto& procedure : leafProcedures) {
        assert(PKB::hasCaller(procedure));

        std::unordered_set<std::string> varsModifiedByProc = PKB::getModifiedVarsInProc(procedure);
        DesignExtractorUtil::updateTCallers(procedure, varsModifiedByProc, "Modifies");
    }

    return true;
}