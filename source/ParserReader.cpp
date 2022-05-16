#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include <unordered_map>

#include "ParserReader.h"
#include "StatementTypeIdentifier.h"
#include "ParserException.h"
#include "PrintProcessor.h"
#include "ReadProcessor.h"
#include "ProcedureProcessor.h"
#include "StatementTypeEnum.h"
#include "ParserExceptionEnum.h"
#include "IfProcessor.h"
#include "ChildProcessor.h"
#include "WhileProcessor.h"
#include "AssignProcessor.h"
#include "CallProcessor.h"

// Constructor
ParserReader::ParserReader() {
	currentNestingLevel = 0;
}

// Main API
bool ParserReader::parseProgram(std::string &filename) {
	std::ifstream sourceFile(filename);

	if (sourceFile.is_open()) {

		char currentCharacter;
		std::string currentLineOfCode;
		int currentStatementNumber = 0;

		try {
			while (sourceFile >> std::noskipws >> currentCharacter) {
				if (currentCharacter == '{') {

					/*=== PROCEDURE ===*/
					if (currentNestingLevel == 0) { 
						currentProcedureName = ProcedureProcessor::processProcedure(currentLineOfCode);

						// Check that the procedure does not already exist
						if (std::find(listOfProcedures.begin(), listOfProcedures.end(), currentProcedureName) != this->listOfProcedures.end()) {
							throw ParserException("ADDING A PROCEDURE THAT ALREADY EXIST: " + currentProcedureName, ParserExceptionEnum::PROCEDURE_EXCEPTION);
						}

						listOfProcedures.push_back(currentProcedureName);
					}
					else { 
						int statementType = StatementTypeIdentifier::identifyStatement(currentLineOfCode);
						currentStatementNumber++;

						/*=== ELSE ===*/
						if (statementType == StatementTypeEnum::ELSE) {
							currentStatementNumber--; // Else statement does not count towards increasing a statement number
							currentNestingLevelToNumberOfElseNeeded[currentNestingLevel]--; // - 1 else needed (by right should always be 0 

							// Check if the else statement checks out first (after if needs to be an else)
							if (currentNestingLevelToNumberOfElseNeeded[currentNestingLevel] != 0) { // encounter if case without closing else case
								throw ParserException("ELSE STATEMENT BEFORE AN IF STATEMENT", ParserExceptionEnum::IF_EXCEPTION);
							}

							// Next
							currentNestingLevelToSetOfLatestNode[currentNestingLevel + 1].clear();
							currentNestingLevelToSetOfLatestNode[currentNestingLevel + 1].push_back(
								currentNestingLevelToContainerStatementNumberMap[currentNestingLevel]
							);

							currentNestingLevelToContainerStatementTypeMap[currentNestingLevel] = StatementTypeEnum::ELSE;
						}

						else {
							/*=== ELSE VALIDATION ===*/
							if (currentNestingLevelToNumberOfElseNeeded[currentNestingLevel] != 0) { // encounter if case without closing else case
								throw ParserException("IF STATEMENT WITHOUT CORRESPONDING ELSE STATEMENT", ParserExceptionEnum::IF_EXCEPTION);
							}

							/*=== IF ===*/
							if (statementType == StatementTypeEnum::IF) {
								IfProcessor::processIf(currentLineOfCode, currentProcedureName, currentStatementNumber);
								currentNestingLevelToNumberOfElseNeeded[currentNestingLevel]++; // Track the number of else needed (always needs to be 0)
							}

							/*=== WHILE ===*/
							else if (statementType == StatementTypeEnum::WHILE) {
								WhileProcessor::processWhile(currentLineOfCode, currentProcedureName, currentStatementNumber);
							}

							currentNestingLevelToContainerStatementNumberMap[currentNestingLevel] = currentStatementNumber; // Keeps track of parent's statement number
							currentNestingLevelToContainerStatementTypeMap[currentNestingLevel] = statementType; // Keeps track of parent's statement type
							currentNestingLevelToNumberOfStatements[currentNestingLevel]++; // Keeps track of number of statements in the statement list 

							/*=== HANDLE RELATIONSHIP ===*/
							// Follows
							if (currentNestingLevelToStatementNumber[currentNestingLevel] != 0) {
								PKB::addFollows(currentNestingLevelToStatementNumber[currentNestingLevel], currentStatementNumber);
							}
							currentNestingLevelToStatementNumber[currentNestingLevel] = currentStatementNumber;

							// Next
							if (!currentNestingLevelToSetOfLatestNode[currentNestingLevel].empty()) {
								for (int node : currentNestingLevelToSetOfLatestNode[currentNestingLevel]) {
									PKB::addNext(node, currentStatementNumber);
								}
								currentNestingLevelToSetOfLatestNode[currentNestingLevel].clear();
							}
							currentNestingLevelToSetOfLatestNode[currentNestingLevel + 1].push_back(currentStatementNumber); // connect to next node

							// Child -> Parent
							if (currentNestingLevel > 1) { // above 1 means child statement
								ChildProcessor::processChild(
									currentNestingLevelToContainerStatementNumberMap[currentNestingLevel - 1],
									currentNestingLevelToContainerStatementTypeMap[currentNestingLevel - 1],
									currentStatementNumber
								);
							}
							// Parent -> List of child statements
							currentNestingLevelToListOfStatements[currentNestingLevel].insert(currentStatementNumber);
						}
					}

					currentNestingLevel++;

					/*=== RESET ===*/
					currentLineOfCode = "";
				}
				else if (currentCharacter == ';') {

					if (currentNestingLevelToNumberOfElseNeeded[currentNestingLevel] != 0) { // Check that an if statement is immediately followed by an else statement
						throw ParserException("IF STATEMENT NOT FOLLOWED BY ELSE STATEMENT BUT INSTEAD FOLLOWED BY: " + currentLineOfCode, ParserExceptionEnum::IF_EXCEPTION);
					}

					int statementType = StatementTypeIdentifier::identifyStatement(currentLineOfCode);
					currentStatementNumber++;

					std::string calledProcedure; // To check for procedure that is called if this statement is a call statement

					switch (statementType) {
					/*=== ASSIGN ===*/
					case StatementTypeEnum::ASSIGN:
						AssignProcessor::processAssign(currentLineOfCode, currentProcedureName, currentStatementNumber);
						break;

					/*=== PRINT ===*/
					case StatementTypeEnum::PRINT:
						PrintProcessor::processPrint(currentLineOfCode, currentProcedureName, currentStatementNumber);
						break;

					/*=== READ ===*/
					case StatementTypeEnum::READ:
						ReadProcessor::processRead(currentLineOfCode, currentProcedureName, currentStatementNumber);
						break;

					/*=== CALL ===*/
					case StatementTypeEnum::CALL:
						calledProcedure = CallProcessor::processCall(currentLineOfCode, currentProcedureName, currentStatementNumber);
						listOfCalledProcedures.push_back(calledProcedure);
						break;

					default:
						throw ParserException("UNABLE TO IDENTIFY STATEMENT TYPE THAT ENDS WITH SEMICOLON: " + currentLineOfCode, ParserExceptionEnum::UNIDENTIFIED_EXCEPTION); // Can't find the type
					}

					currentNestingLevelToNumberOfStatements[currentNestingLevel]++;

					/*=== HANDLE RELATIONSHIP ===*/
					// Follows
					if (currentNestingLevelToStatementNumber[currentNestingLevel] != 0) {
						PKB::addFollows(currentNestingLevelToStatementNumber[currentNestingLevel], currentStatementNumber);
					}
					currentNestingLevelToStatementNumber[currentNestingLevel] = currentStatementNumber;

					// Next
					if (!currentNestingLevelToSetOfLatestNode[currentNestingLevel].empty()) {
						for (int node : currentNestingLevelToSetOfLatestNode[currentNestingLevel]) {
							PKB::addNext(node, currentStatementNumber);
						}
						currentNestingLevelToSetOfLatestNode[currentNestingLevel].clear();
					}
					currentNestingLevelToSetOfLatestNode[currentNestingLevel].push_back(currentStatementNumber);

					// Child -> Parent
					if (currentNestingLevel > 1) { // above 1 means child statement
						ChildProcessor::processChild(
							currentNestingLevelToContainerStatementNumberMap[currentNestingLevel - 1],
							currentNestingLevelToContainerStatementTypeMap[currentNestingLevel - 1],
							currentStatementNumber
						);
					}

					// Parent -> List of child statements
					currentNestingLevelToListOfStatements[currentNestingLevel].insert(currentStatementNumber);

					/*=== RESET ===*/
					currentLineOfCode = ""; 
				}
				else if (currentCharacter == '}') {
					if (currentNestingLevelToNumberOfElseNeeded[currentNestingLevel] != 0) { // Check that an if statement is immediately followed by an else statement
						throw ParserException("IF STATEMENT WITHOUT CORRESPONDING ELSE STATEMENT", ParserExceptionEnum::IF_EXCEPTION);
					}

					if (currentNestingLevelToNumberOfStatements[currentNestingLevel] < 1) { // Check that currest nesting level has at least one stmt in the stmtlist
						throw ParserException("STMTLIST CANNOT BE EMPTY, CHECK FOR DOUBLE CLOSING CURLY BRACKETS?", ParserExceptionEnum::CHILD_STATEMENT_EXCEPTION);
					}

					/*=== RELATIONSHIP ===*/
					// Add parent relationship
					if (!currentNestingLevelToListOfStatements[currentNestingLevel].empty() // Check that list of statements not empty
						// Check that the parent is not a procedure
						&& currentNestingLevelToContainerStatementTypeMap[currentNestingLevel - 1] != 0
						// Check that the parent is also in an else statement
						&& currentNestingLevelToNumberOfElseNeeded[currentNestingLevel - 1] == 0) {
							PKB::addParent(
								currentNestingLevelToContainerStatementNumberMap[currentNestingLevel - 1], // The parent statement number
								currentNestingLevelToListOfStatements[currentNestingLevel] // The statement list
							);
					}

					// Next relationship
					// if the parent is an if or else, transfer set of latest node
					if (currentNestingLevelToContainerStatementTypeMap[currentNestingLevel - 1] == StatementTypeEnum::IF
						|| currentNestingLevelToContainerStatementTypeMap[currentNestingLevel - 1] == StatementTypeEnum::ELSE) {

						for (int node : currentNestingLevelToSetOfLatestNode[currentNestingLevel]) {
							currentNestingLevelToSetOfLatestNode[currentNestingLevel - 1].push_back(node);
						}

						currentNestingLevelToSetOfLatestNode[currentNestingLevel].clear();
					}

					// If the parent is a while statement, clear latest node and set latest to while statement number
					if (currentNestingLevelToContainerStatementTypeMap[currentNestingLevel - 1] == StatementTypeEnum::WHILE) {
						for (int node : currentNestingLevelToSetOfLatestNode[currentNestingLevel]) {
							PKB::addNext(node, currentNestingLevelToContainerStatementNumberMap[currentNestingLevel - 1]);
						}
						currentNestingLevelToSetOfLatestNode[currentNestingLevel].clear();

						currentNestingLevelToSetOfLatestNode[currentNestingLevel - 1].push_back(
							currentNestingLevelToContainerStatementNumberMap[currentNestingLevel - 1]
						);
					}
					

					/*=== RESET DATA ===*/
					// Check that it does not end with if to include the else statement's children as well
					// Affects parent relationship (not follow relationship)
					if (currentNestingLevelToNumberOfElseNeeded[currentNestingLevel - 1] == 0) {
						currentNestingLevelToListOfStatements[currentNestingLevel].clear();
					}	

					// Next relationship reset if new procedure
					if (currentNestingLevel == 1) {
						currentNestingLevelToSetOfLatestNode[currentNestingLevel].clear();
					}

					currentNestingLevelToStatementNumber[currentNestingLevel] = 0; // Reset previous statement to close the follow list

					currentNestingLevelToNumberOfStatements[currentNestingLevel] = 0; // Reset number of statements in a statement list

					currentNestingLevel--;

					/*=== RESET ===*/
					currentLineOfCode = "";
				}
				else {
					currentLineOfCode += currentCharacter;
				}
			}

			// Check that the { and } corresponds (no mismatched number)
			if (currentNestingLevel != 0) {
				throw ParserException("MISMATCHED CURLY BRACKETS", ParserExceptionEnum::PROCEDURE_EXCEPTION);
			}

			// Check that the call statements has not made any calls to a procedure that does not exist
			for (std::string calledProcedureName : listOfCalledProcedures) {
				// Check that the called procedure exist in the list of procedures
				if (!(std::find(listOfProcedures.begin(), listOfProcedures.end(), calledProcedureName) != listOfProcedures.end())) {
					throw ParserException("CALLING A PROCEDURE THAT DOES NOT EXIST: " + calledProcedureName, ParserExceptionEnum::PROCEDURE_EXCEPTION);
				}
			}
		}
		catch (ParserException& e) {
			// Stop the while loop
			std::cout << e.getExceptionMessage();
			std::exit(0);
		}
	}
	else {
		throw ParserException("UNABLE TO OPEN SOURCE FILE", ParserExceptionEnum::READER_EXCEPTION);
	}
	return true;
}
