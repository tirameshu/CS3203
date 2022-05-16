#pragma once

#include <stdio.h>
#include <string>

/*
	NextBipProcessor is responsible for extracting NextBip design abstraction relationship
	information between design entities of the program, and populate the PKB's NextBipStore
	with information extracted.
*/

class NextBipProcessor {
public:
	static void calculateNextBip();
};
