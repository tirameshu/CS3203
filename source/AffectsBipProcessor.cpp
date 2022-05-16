#include "AffectsBipProcessor.h"
#include "DesignExtractorUtil.h"
#include "PKB.h"

void AffectsBipProcessor::calculateAffectsBip() {
	/*=== ITERATE PROCEDURE ===*/
	for (std::string proc : PKB::getProcedureList()) {
		int root = PKB::getProcedureStmtMap().at(proc).front();

		/*=== INITIATE DATA STRUCTURES ===*/
		std::stack<std::unordered_map<std::string, std::unordered_set<int>>> LMS;
		std::unordered_map<std::string, std::unordered_set<int>> LMT;

		calculateAffect(root, LMT);
	}
}

void AffectsBipProcessor::calculateAffect(int stmt, std::unordered_map<std::string, std::unordered_set<int>>& LMT) {

	/*=== ASSIGN STMT ===*/
	if (PKB::getAssignStmtList().count(stmt)) {
		AffectsBipProcessor::handleAssign(stmt, LMT);
	}

	/*=== WHILE STMT ===*/
	else if (PKB::getWhileStmtList().count(stmt)) {
		AffectsBipProcessor::handleWhile(stmt, LMT);
	}

	else if (PKB::getCallStmtList().count(stmt)) {
		AffectsBipProcessor::handleCall(stmt, LMT);
	}

	/*=== IF STMT ===*/
	else if (PKB::getIfStmtList().count(stmt)) {
		AffectsBipProcessor::handleIf(stmt, LMT);
	}

	/*=== OTHER TYPES OF STMT ===*/
	else {
		AffectsBipProcessor::handleOther(stmt, LMT);
	}

	// Update frontier
	if (PKB::stmtHasFollower(stmt)) {
		int follower = PKB::getFollowerOfStmt(stmt);
		calculateAffect(follower, LMT);
	}
}

void AffectsBipProcessor::handleCall(int stmt, std::unordered_map<std::string, std::unordered_set<int>>& LMT) {
	std::string calledProcedure = PKB::getCallerStmtToProcMap().at(stmt);
	int front = PKB::getProcedureStmtMap().at(calledProcedure).front();

	AffectsBipProcessor::calculateAffect(front, LMT);
}

void AffectsBipProcessor::handleIf(int stmt, std::unordered_map<std::string, std::unordered_set<int>>& LMT) {
	std::unordered_map<std::string, std::unordered_set<int>> LMTIf = LMT;
	std::unordered_map<std::string, std::unordered_set<int>> LMTElse = LMT;	

	int ifFront = 0;
	int elseFront = 0;

	for (int next : PKB::getNextOfStmt(stmt)) {
		if (next == stmt + 1) {
			ifFront = next;
		}
		else {
			elseFront = next;
		}
	}

	calculateAffect(ifFront, LMTIf); // if branch
	calculateAffect(elseFront, LMTElse); // else branch

	// Merge if with else
	LMT = DesignExtractorUtil::merge(LMTIf, LMTElse);
}


void AffectsBipProcessor::handleOther(int stmt, std::unordered_map<std::string, std::unordered_set<int>>& LMT) {
	std::unordered_set<std::string> modvars;
	for (std::string modvar : PKB::getModifiedVarsInStmt(stmt)) { // should only have 1 or 0 item
		modvars.insert(modvar);
	}

	// Read stmt
	if (!modvars.empty()) {
		for (std::string modvar : modvars) {
			if (LMT.count(modvar)) {
				LMT.erase(modvar);
			}
		}
	}
}

void AffectsBipProcessor::handleAssign(int stmt, std::unordered_map<std::string, std::unordered_set<int>>& LMT) {
	// 1. modvar = getModifies(a)
	// Assign statement can only have one modified var
	std::string modvar = "";
	for (std::string modvarelem : PKB::getModifiedVarsInStmt(stmt)) {
		modvar = modvarelem;
	}

	// 2. usedvar = getUses(a)
	std::unordered_set<std::string> usedvars = PKB::getUsedVarsInStmt(stmt);

	// 3. for each u in usedvars
	for (std::string u : usedvars) {
		// If u exist in lmt
		if (LMT.count(u)) {
			std::unordered_set<int> lastmodstmts = LMT.at(u);
			for (int lastmodstmt : lastmodstmts) {
				PKB::addAffectsBip(lastmodstmt, stmt);
				//std::cout << "AffectsBip: " + std::to_string(lastmodstmt) + " ===> " + std::to_string(stmt) << "\n";
			}
		}
	}

	// 4. Update LMT with last mod
	if (LMT.count(modvar)) {
		// update var
		LMT.at(modvar) = { stmt };
	}
	else {
		// new var
		std::unordered_set<int> temp = { stmt };
		LMT.emplace(modvar, temp);
	}
}

void AffectsBipProcessor::handleWhile(int stmt, std::unordered_map<std::string, std::unordered_set<int>>& LMT) {
	std::unordered_map<std::string, std::unordered_set<int>> beforeLMT = LMT;
	int stmtAfterWhile = stmt + 1;
	
	AffectsBipProcessor::calculateAffect(stmtAfterWhile, LMT);

	// End of the while loop                          
	if (beforeLMT != LMT) {
		LMT = DesignExtractorUtil::merge(beforeLMT, LMT);

		// repeat the algo if needed
		if (beforeLMT != LMT) {
			handleWhile(stmt, LMT);
		}
	}
}
