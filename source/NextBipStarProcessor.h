#pragma once
#include <unordered_set>

/*
	NextBipStarProcessor is responsible for extracting NextBip* design abstraction relationship
	information between design entities of the program, and populate the PKB's NextBipStore
	with information extracted.
*/

class NextBipStarProcessor {
public:
	static void calculateNextBipStar();
};
