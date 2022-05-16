#pragma once

#include <string>
#include "PKB.h"

/*
	ProcedureProcessor is responsbile for verifying the validity of procedure declarations identified
	by the Parser (to ensure that no SIMPLE grammar violations are made), and to tokenize
	and extract key information regarding the declaration to populate the PKB
*/

class ProcedureProcessor {
public:
	// Check validity and call PKB API
	static std::string processProcedure(std::string &inputLine);
};
