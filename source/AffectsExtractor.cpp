#include "AffectsExtractor.h"
#include "StatementTypeEnum.h"

// class variables
std::unordered_map<std::string, std::unordered_set<int>> AffectsExtractor::lastModifiedTable;
std::stack< std::unordered_map<std::string, std::unordered_set<int>> > AffectsExtractor::lastModifiedStack;

/*
* int current: stmt to start from
* int control: int to stop at
*/

// - starts computing from this stmt
// - good for branching
// - called recursively by IF and WHILE segments

// ASSUMPTION:
// - UsesS/ ModifiesS computed
// - UsesP/ ModifiesP computed
// current valid, even if is 0
bool AffectsExtractor::computeAffects(int current, int control) {

	while (current != control) {
		std::unordered_set<int> toAdd;
		std::unordered_set<std::string> modifiedVars;
		std::unordered_set<std::string> usedVars;
		std::unordered_map<std::string, std::unordered_set<int>> beforeLMT;
		std::unordered_map<std::string, std::unordered_set<int>> LMTif;
		std::unordered_map<std::string, std::unordered_set<int>> LMTelse;

		int stmtType = PKB::getTypeOfStmt(current); // 1: assign, 2: print, 3: read, 4: if, 5: while, 6: else, 7: call

		switch (stmtType) {
		case StatementTypeEnum::ASSIGN:

			modifiedVars = PKB::getModifiedVarsInStmt(current);
			usedVars = PKB::getUsedVarsInStmt(current);

			// check if any of the vars used was modified
			for (auto& var : usedVars) {
				if (lastModifiedTable.find(var) != lastModifiedTable.end()) {
					// var was modified
					std::unordered_set<int> affectedBy = lastModifiedTable.at(var);

					for (auto& stmt : affectedBy) {
						PKB::addAffects(stmt, current);
					}

				}
			}

			assert(modifiedVars.size() == 1); // should only modify 1 var

			for (auto& var : modifiedVars) {
				if (lastModifiedTable.find(var) != lastModifiedTable.end()) {
					// update var
					lastModifiedTable.at(var) = { current };
				}
				else {
					// new var
					toAdd = { current };
					lastModifiedTable.emplace(var, toAdd);
				}
			}

			break;

		case StatementTypeEnum::IF:
			LMTif = lastModifiedTable;
			LMTelse = lastModifiedTable;

			// if branch
			computeAffectsForIf(current);
			LMTif = lastModifiedTable; // saving results from if  branch

			// else branch
			lastModifiedTable = LMTelse;
			computeAffectsForElse(current);
			lastModifiedTable = DesignExtractorUtil::merge(LMTif, lastModifiedTable);

			if (DesignExtractorUtil::getOutsideIfElse(current) == 0) { // must check, otherwise can end up in situation like computeAffects(0, 17)
				control = 0;
			}
			return computeAffects(DesignExtractorUtil::getOutsideIfElse(current), control);

			break;

		case StatementTypeEnum::WHILE:
			beforeLMT = lastModifiedTable; // copy
			lastModifiedStack.push(beforeLMT);
			return computeAffectsForWhile(current, control);
			break;

		default:
			// if any other statement modifies a var, erase from map
			modifiedVars = PKB::getModifiedVarsInStmt(current);
			for (auto& var : modifiedVars) {
				if (lastModifiedTable.find(var) != lastModifiedTable.end()) {
					// var no longer last modified by assign stmt
					// always remove
					lastModifiedTable.erase(var);
				}
			}

			break;
		}

		// only if, while stmts can have > 1 directNext
		// all others should only have <= 1
		std::unordered_set<int> directNext = PKB::getNextOfStmt(current);

		assert(directNext.size() <= 1);
		if (directNext.size() == 1) {
			current = *PKB::getNextOfStmt(current).begin();
		}
		else {
			current = 0; // terminate for this instance
			return true;
		}
	}

	return true;
}

bool AffectsExtractor::computeAffectsForIf(int ifStmt) {
	std::unordered_set<int> directNext = PKB::getNextOfStmt(ifStmt);
	assert(directNext.size() == 2);

	std::unordered_set<int>::iterator it = directNext.begin();
	std::advance(it, 1);

	int startOfIf = *directNext.begin();
	int startOfElse = *it;

	for (int stmt : directNext) {
		if (stmt < startOfIf) {
			startOfElse = startOfIf;
			startOfIf = stmt;
		}
		else if (stmt > startOfIf) {
			startOfElse = stmt;
		}
	}

	int outsideIfElse = DesignExtractorUtil::getOutsideIfElse(ifStmt);

	return computeAffects(startOfIf, outsideIfElse);
}

bool AffectsExtractor::computeAffectsForElse(int ifStmt) {
	std::unordered_set<int> directNext = PKB::getNextOfStmt(ifStmt);
	assert(directNext.size() == 2);

	std::unordered_set<int>::iterator it = directNext.begin();
	std::advance(it, 1);

	int startOfIf = *directNext.begin();
	int startOfElse = *it;

	for (int stmt : directNext) {
		if (stmt < startOfIf) {
			startOfElse = startOfIf;
			startOfIf = stmt;
		}
		else if (stmt > startOfIf) {
			startOfElse = stmt;
		}
	}

	int outsideIfElse = DesignExtractorUtil::getOutsideIfElse(ifStmt);

	return computeAffects(startOfElse, outsideIfElse);
}

bool AffectsExtractor::computeAffectsForWhile(int whileStmt, int control) {
	std::unordered_set<int> directNext = PKB::getNextOfStmt(whileStmt);
	assert(directNext.size() == 1 || directNext.size() == 2);

	std::unordered_set<int>::iterator it = directNext.begin();
	std::advance(it, 1);

	int startOfWhile = *directNext.begin();
	int outsideWhile;

	if (directNext.size() == 1) {
		outsideWhile = 0;
	}
	else {
		outsideWhile = *it;
	}

	if (directNext.size() == 2) {
		int smaller = startOfWhile;
		int bigger = outsideWhile;
		for (int stmt : directNext) {
			if (stmt < smaller) {
				bigger = smaller;
				smaller = stmt;
			}
			else if (stmt > smaller) {
				bigger = stmt;
			}
		}

		if (smaller < whileStmt) {
			outsideWhile = smaller;
			startOfWhile = bigger;
		}
		else {
			// cannot be equal
			outsideWhile = bigger;
			startOfWhile = smaller;
		}
	}
	else if (directNext.size() == 1) { // proc ends with while
		startOfWhile = *directNext.begin();
		outsideWhile = 0;
	}

	computeAffects(startOfWhile, whileStmt); // the control becomes non-zero

	std::unordered_map<std::string, std::unordered_set<int>> beforeLMT = lastModifiedStack.top(); // does not remove
	lastModifiedStack.pop();

	if (beforeLMT != lastModifiedTable) {
		std::unordered_map<std::string, std::unordered_set<int>> mergedTable = DesignExtractorUtil::merge(beforeLMT, lastModifiedTable);
		if (mergedTable == beforeLMT) {
			// only happens if program is in second run
			// or  LMT = beforeLMT with some vars erased: this is the effect of call/ read stmts, shld not change in second run
			// terminate
			if (outsideWhile == 0) { // must check, otherwise can end up in situation like computeAffects(0, 17)
				control = 0;
			}
			lastModifiedTable = mergedTable;
			return computeAffects(outsideWhile, control);
		}
		computeAffects(startOfWhile, whileStmt);

	}

	lastModifiedTable = DesignExtractorUtil::merge(beforeLMT, lastModifiedTable); // update LMT with merged info
	// while loop done, only run max twice
	// skips to outside while loop and continue down the proc
	return computeAffects(outsideWhile, control);
}

bool AffectsExtractor::computeAffectsT(std::vector<int> stmtList) {
	bool resultFlag = true;

	for (int stmt : PKB::getAffectingList()) {
		std::unordered_set<int> visitedStmts;
		DFSUtil(stmt, stmt, visitedStmts);
	}

	return resultFlag;
}

void AffectsExtractor::DFSUtil(int srcStmt, int currStmt, std::unordered_set<int>& visitedStmts) {
	visitedStmts.emplace(currStmt);
	auto& affectedStmts = PKB::getAffectingOfStmt(currStmt);

	for (int affectedStmt : affectedStmts) {
		PKB::addAffectingT(srcStmt, { affectedStmt });
		PKB::addAffectedByT(affectedStmt, { srcStmt });

		if (visitedStmts.count(affectedStmt) == 0) {
			DFSUtil(srcStmt, affectedStmt, visitedStmts);
		}
	}
}

std::unordered_set<int> AffectsExtractor::computeAllAffectedBy(int stmt) {
	std::unordered_set<int> allAffectedBy;

	std::unordered_set<int> directAffectedBy = PKB::getAffectedByOfStmt(stmt);
	for (auto& affectedBy : directAffectedBy) {
		std::unordered_set<int> temp = PKB::getAffectedByTOfStmt(affectedBy);
		allAffectedBy.insert(temp.begin(), temp.end());
		allAffectedBy.insert(affectedBy);
	}

	return allAffectedBy;
}

std::unordered_set<int> AffectsExtractor::computeAllAffecting(int stmt) {
	std::unordered_set<int> allAffecting;

	std::unordered_set<int> directAffecting = PKB::getAffectingOfStmt(stmt);
	for (auto& affecting : directAffecting) {
		std::unordered_set<int> temp = PKB::getAffectingTOfStmt(affecting);
		allAffecting.insert(temp.begin(), temp.end());
		allAffecting.insert(affecting);
	}

	return allAffecting;
}

void AffectsExtractor::clearAffectsData() {
	lastModifiedTable.clear();

	while (!lastModifiedStack.empty()) {
		lastModifiedStack.pop();
	}
}