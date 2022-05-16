#include <stdio.h>
#include <iostream>
#include <string>

#include "Query.h"

/*
	QueryParser is the main class responsible for parsing the query statements passed
	into the Static Program Analyzer.
*/

class QueryParser {
public:
	// Constructor
	QueryParser();

	// public API to parse the query from user
	Query parseQuery(std::string userQuery);

private:
	QuerySuchThatClause parseSuchThat(RelRef relRef, std::string firstArg, std::string secondArg,
		std::vector<std::string> synonymNames, std::vector<ArgumentTypes> synonymTypes);
	QueryPatternClause parsePattern(std::string patternName, ArgumentTypes patternType, std::string firstArg, std::string secondArg,
		std::vector<std::string> synonymNames, std::vector<ArgumentTypes> synonymTypes);
	QueryWithClause parseWith(std::string firstRef, std::string secondRef,
		std::vector<std::string> synonymNames, std::vector<ArgumentTypes> synonymTypes);
};