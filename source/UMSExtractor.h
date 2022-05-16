#pragma once

#include "DesignExtractor.h"

/*
	UMPExtractor is responsible for extracting UsesS/ModifiesS design abstraction relationship
	information of the program regarding container statements and call statements
	and populate the PKB's UsesStore and ModifiesStore with information extracted.
*/

class UMSExtractor {
public:
	static bool updateAncestorsUse(std::string procedure);

	static bool updateAncestorsModify(std::string procedure);
};