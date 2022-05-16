#pragma once

#include "DesignExtractor.h"

/*
	CallsExtractor is responsible for extracting Calls* design abstraction relationship
	information between design entities of the program, and populate the PKB's CallStore
	with information extracted.
*/

class CallsExtractor {
public:
	static bool computeAllCallees(std::string procedure);

	static bool computeAllCallers(std::string procedure);
};