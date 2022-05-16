#pragma once

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>

#include <assert.h>

#include "PKB.h"
#include "DesignExtractorUtil.h"

#include "ParserException.h"
#include "ParserExceptionEnum.h"

/*
	Design Extractor is responsible for design abstraction relationships from the SIMPLE source code 
	parsed by the Static Program Analyzer. It has full access to the PKB information that is populated
	by the Parser initially (with immediate relationships). Design Extractor computes the transitive 
	relationships and more complex relationships between design entities of the program.
	
	Design Extractor computes the following relationships:
	- Follows*
	- Parent*
	- UsesS/ UsesP (for container statements, call statements and procedures)
	- ModifiesS/ ModifiesP (for container statements, call statements and procedures)
	- Call*
	- Next*
	- Affects/ Affects*
	- NextBip/ NextBip*
	- AffectsBip
*/

class DesignExtractor {
public:

	// called by parser to kick start DE
	static bool processProgram();

	// establish Calls* relationship between procedures
	static bool processProcedures();

	// validate procedures
	static bool validateProcedures();

	// iteratively called by processProcedure() to process all stmtList in 1 procedure
	static bool processStatements(std::string procedure, std::vector<int> stmtList);

	// FollowsT
	static std::unordered_set<int> computeAllFollowed(int stmt); //  returns all ancestors

	static std::unordered_set<int> computeAllFollowers(int stmt); // returns all descendents

	// ParentT
	static std::unordered_set<int> computeAllAncestors(int stmt); //  returns all ancestors

	static std::unordered_set<int> computeAllDescendants(int stmt); // returns all descendents

	// CallsT
	static bool computeAllCallees(std::string procedure);

	static bool computeAllCallers(std::string procedure);

	// NextT
	static std::unordered_set<int> computeAllPrevious(int stmt); // returns all stmts executed before current stmt

	static std::unordered_set<int> computeAllNext(int stmt); // returns all stmts executed after current stmt

	// UsesS, ModifiesS
	static bool updateAncestorsUseAndModify(std::string procedure);

	// UsesP, ModifiesP
	static bool updateProcedureUsesAndModifies();

	// Affects
	static bool computeAffects(int current, int control);

	// AffectsT
	static bool computeAffectsT(std::vector<int> stmtList);

	// Clears lastModifiedTable and lastModifiedStack
	static void clearAffectsData();
};
