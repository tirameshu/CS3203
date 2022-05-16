#pragma once

#include <stdio.h>
#include <string>
#include <vector>

#include "QueryPatternClause.h"

/*
	PatternCluaseParser is the class responsible for processing a pattern-cl to be returned in the final query, based on information
	extracted by the QueryParser.
*/

class PatternClauseParser {
public:
	QueryPatternClause parsePattern(std::string patternName, ArgumentTypes patternType, std::string firstArg, std::string secondArg,
		std::vector<std::string> synonymNames, std::vector<ArgumentTypes> synonymTypes);
};