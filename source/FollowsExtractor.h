#pragma once

#include "DesignExtractor.h"

/*
	FollowsExtractor is responsible for extracting Follows* design abstraction relationship
	information between design entities of the program, and populate the PKB's FollowsStore
	with information extracted.
*/

class FollowsExtractor {
public:
	static std::unordered_set<int> computeAllFollowed(int stmt);

	static std::unordered_set<int> computeAllFollowers(int stmt);
};