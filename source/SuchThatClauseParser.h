#pragma once

#include <stdio.h>
#include <string>
#include <vector>

#include "QuerySuchThatClause.h"

/*
	SuchThatCluaseParser is the class responsible for processing a suchthat-cl to be returned in the final query, based on information
	extracted by the QueryParser. Each unique RelRef has its own method to validate arguments and process the clause. Variations of the
	same RelRef (eg. UsesS and UsesP, Next and NextT and NextBip and NextBipT) are processed by the same method.
*/

class SuchThatClauseParser {
public:
	// UsesS
	QuerySuchThatClause parseUses(RelRef relRef, std::string firstArg, std::string secondArg,
		std::vector<std::string> synonymNames, std::vector<ArgumentTypes> synonymTypes);

	// ModifiesS
	QuerySuchThatClause parseModifies(RelRef relRef, std::string firstArg, std::string secondArg,
		std::vector<std::string> synonymNames, std::vector<ArgumentTypes> synonymTypes);

	// Parent and ParentT
	QuerySuchThatClause parseParent(RelRef relRef, std::string firstArg, std::string secondArg,
		std::vector<std::string> synonymNames, std::vector<ArgumentTypes> synonymTypes);

	//Follows and FollowsT
	QuerySuchThatClause parseFollows(RelRef relRef, std::string firstArg, std::string secondArg,
		std::vector<std::string> synonymNames, std::vector<ArgumentTypes> synonymTypes);

	//Calls and Calls
	QuerySuchThatClause parseCalls(RelRef relRef, std::string firstArg, std::string secondArg,
		std::vector<std::string> synonymNames, std::vector<ArgumentTypes> synonymTypes);

	//Next and NextT
	QuerySuchThatClause parseNext(RelRef relRef, std::string firstArg, std::string secondArg,
		std::vector<std::string> synonymNames, std::vector<ArgumentTypes> synonymTypes);

	//Affects and AffectsT
	QuerySuchThatClause parseAffects(RelRef relRef, std::string firstArg, std::string secondArg,
		std::vector<std::string> synonymNames, std::vector<ArgumentTypes> synonymTypes);
};