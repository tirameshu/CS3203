#pragma once

#include <stdio.h>
#include <iostream>
#include <string>
#include <list>
#include <unordered_map>

#include "PKB.h"

/*
	ParserReader is the main class responsible for parsing the SIMPLE source code passed
	into the Static Program Analyzer. 
	
	The following is how the Parser works:
	1. Parse the source code line by line 
	2. Identify the type of each line with the use of StatementTypeIdentifier
	3. Each statement would then be processed by the corresponding Processor depending on their statement type
	4. Each Processor would check for validity of the statement (if there are any SIMPLE grammar violations), if 
	   none are found, the Processor tokenizes the statement to extract key information regarding design entities
	   and immediate design abstractions and populates the PKB by calling the relevant setter methods provided in 
	   PKB
*/

class ParserReader {
public:
	// Constructor
	ParserReader();

	// public API to parse the program
	bool parseProgram(std::string &filename);

private:

	// ATTRIBUTES
	int currentNestingLevel;
	std::string currentProcedureName;

	// Parent relationship
	std::unordered_map<int, int> currentNestingLevelToContainerStatementNumberMap; // Keeps track of nesting level -> container statement number
	std::unordered_map<int, int> currentNestingLevelToContainerStatementTypeMap; // Keeps track of nesting level -> container statement type
	std::unordered_map<int, std::unordered_set<int>> currentNestingLevelToListOfStatements; // Keeps track of nesting level -> list of statement numbers

	// Follow relationship
	std::unordered_map<int, int> currentNestingLevelToStatementNumber; // Keeps track of nesting level -> previous statement number

	// Next relationship
	std::unordered_map<int, std::vector<int>> currentNestingLevelToSetOfLatestNode; // Keeps track of latest node to be added
	
	// Top-level validation 
	std::unordered_map<int, int> currentNestingLevelToNumberOfElseNeeded; // Keeps track of nesting level -> number of else needed
	std::unordered_map<int, int> currentNestingLevelToNumberOfStatements; // Keeps track of nesting level -> number of statements
	std::vector<std::string> listOfProcedures; // Keeps track of list of procedures so far
	std::vector<std::string> listOfCalledProcedures; // Keeps track of all the procedures that are called
};
