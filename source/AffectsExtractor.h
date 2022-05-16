#pragma once

#include "DesignExtractor.h"
#include <stack>

/*
	AffectsExtractor is responsible for extracting Affects design abstraction relationship
	information between design entities of the program, and populate the PKB's AffectsStore
	with information extracted.
*/

class AffectsExtractor {
public:
	static std::unordered_map<std::string, std::unordered_set<int>> lastModifiedTable;
	static std::stack< std::unordered_map<std::string, std::unordered_set<int>> > lastModifiedStack;
	static std::unordered_set<int> processedStmts;

	static bool computeAffects(int current, int control);
	static bool computeAffectsForWhile(int whileStmt, int control);
	static bool computeAffectsForIf(int ifStmt);
	static bool computeAffectsForElse(int ifStmt);

	static bool computeAffectsT(std::vector<int> stmtList);
	static std::unordered_set<int> computeAllAffectedBy(int stmt);
	static std::unordered_set<int> computeAllAffecting(int stmt);

	static void DFSUtil(int srcStmt, int currStmt, std::unordered_set<int>& visitedStmts);
	static void clearAffectsData();
};