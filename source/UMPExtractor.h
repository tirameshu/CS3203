#pragma once

#include "DesignExtractor.h"

/*
	UMPExtractor is responsible for extracting UsesP/ModifiesP design abstraction relationship
	information of the program, and populate the PKB's UsesStore and ModifiesStore
	with information extracted.
*/

class UMPExtractor {
public:
	static bool updateProcedureUses();

	static bool updateProcedureModifies();
};