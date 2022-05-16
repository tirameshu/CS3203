#pragma once

#include <vector>
#include "QueryWithClause.h"

/*
	WithCluaseParser is the class responsible for processing a with-clause to be returned in the final query, based on information
	extracted by the QueryParser. This class contains one main method, parseWith, to parse the with-clause, and various utility
	methods employed by parseWith, to extract relevant information about the various arguments involved in the with-clause.
*/

class WithClauseParser {
public:
	QueryWithClause parseWith(std::string firstRef, std::string secondRef,
		std::vector<std::string> synonymNames, std::vector<ArgumentTypes> synonymTypes);

private:
	ArgumentTypes checkType(std::string ref, std::vector<std::string> synonymNames, std::vector<ArgumentTypes> synonymTypes);
	bool isAttrRef(std::string ref);
	AttrName parseAttrName(std::string ref);
	ArgumentTypes parseSynonymType(std::string ref, std::vector<std::string> synonymNames, std::vector<ArgumentTypes> synonymTypes);
};