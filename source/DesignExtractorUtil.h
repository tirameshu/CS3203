#pragma once

#include <stdio.h>
#include <algorithm>
#include <assert.h>

#include "PKB.h"
#include "ParserException.h"
#include "ParserExceptionEnum.h"

/*
	DesignExtractorUtil is comprises of utility methods used by the DesignExtractor.
*/

class DesignExtractorUtil {
public:
	static bool anyAncestorOfType(int stmt, int type); // 1: assign, 2: print, 3: read, 4: if, 5: while, 6: else, 7: call

	static bool anyDescendantOfType(int stmt, int type);

	static bool hasRecursion(std::string currentProc, std::unordered_map<std::string, bool> visited, std::unordered_map<std::string, bool> inRecStack);

	static bool updateAncestorsOfStmt(int stmt, std::unordered_set<std::string> vars, std::string rel);

	static bool updateTCallers(std::string procedure, std::unordered_set<std::string> varsToUpdate, std::string rel);

	static int getOutsideIfElse(int ifStmt);

	static std::unordered_map<std::string, std::unordered_set<int>> merge(std::unordered_map<std::string, std::unordered_set<int>> beforeLMT, std::unordered_map<std::string, std::unordered_set<int>> LMT);
};