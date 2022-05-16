#pragma once

#include "DesignExtractor.h"

/*
	ParentExtractor is responsible for extracting Parent* design abstraction relationship
	information between design entities of the program, and populate the PKB's FollowsStore
	with information extracted.
*/

class ParentExtractor {
public:
	static std::unordered_set<int> computeAllAncestors(int stmt);

	static std::unordered_set<int> computeAllDescendants(int stmt);
};