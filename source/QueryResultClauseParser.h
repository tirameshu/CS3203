#pragma once
#include <stdio.h>
#include <iostream>
#include <string>
#include <tuple>
#include <vector>

#include "ArgumentTypes.h"
#include "AttrName.h"

/*
	QueryResultClauseParser is the class responsible for processing what is to be returned in the final query, based on information
	extracted by the QueryParser.
	There are two methods; one to parse tuples (eg. Select <s1, s2>), and another to parse single synonyms (eg. Select s).
*/

class QueryResultClauseParser {
public:
	static std::tuple<bool, std::vector<std::string>, std::vector<ArgumentTypes>, std::vector<AttrName>> parseTuple(std::vector<std::string> selectionToken,
		std::vector<std::string> synonymNames, std::vector<ArgumentTypes> synonymTypes);
	static std::tuple<bool, std::vector<std::string>, std::vector<ArgumentTypes>, std::vector<AttrName>> parseSingleSynonym(std::vector<std::string> selectionToken,
		std::vector<std::string> synonymNames, std::vector<ArgumentTypes> synonymTypes);
};