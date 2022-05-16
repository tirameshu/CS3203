#pragma once

#include "DesignExtractor.h"

/*
	NextExtractor is responsible for extracting Next design abstraction relationship
	information between design entities of the program, and populate the PKB's NextStore
	with information extracted.
*/

class NextExtractor {
public:
	static std::unordered_set<int> computeAllPrevious(int stmt);

	static std::unordered_set<int> computeAllNext(int stmt);
};