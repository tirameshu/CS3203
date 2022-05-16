#include <stdio.h>
#include <string>
#include <sstream>
#include <iterator>
#include <vector>
#include <fstream>

#include "StatementTypeIdentifier.h"
#include "StatementTypeEnum.h"
#include "ParserException.h"
#include "ParserExceptionEnum.h"
#include "ExpressionTokenizer.h"

// Public API
int StatementTypeIdentifier::identifyStatement(std::string &inputLine) {

	// Trivial error cases
	if (inputLine.empty()) { // If double semicolon or curly bracket
		throw ParserException("EMPTY STATEMENT DETECTED, CHECK FOR DOUBLE SEMICOLON/CURLY BRACKET?", ParserExceptionEnum::EMPTY_EXCEPTION);
	}
	
	std::vector<std::string> vectorOfTokens = ExpressionTokenizer::tokenize(inputLine);

	if (vectorOfTokens.size() == 1) {
		if (vectorOfTokens[0].compare("else") == 0) {
			return StatementTypeEnum::ELSE;
		}
		else {
			throw ParserException("INVALID ELSE SYNTAX: " + inputLine, ParserExceptionEnum::UNIDENTIFIED_EXCEPTION);
		}
	}

	if (vectorOfTokens.size() < 2) {
		throw ParserException("INVALID STATEMENT LINE: " + inputLine, ParserExceptionEnum::UNIDENTIFIED_EXCEPTION);
	}

	if (vectorOfTokens[1].compare("=") == 0) {
		return StatementTypeEnum::ASSIGN;
	}
	else if (vectorOfTokens[1].compare("(") == 0) {
		if (vectorOfTokens[0].compare("if") == 0) {
			return StatementTypeEnum::IF;
		}
		else if (vectorOfTokens[0].compare("while") == 0) {
			return StatementTypeEnum::WHILE;
		}
		else {
			throw ParserException("UNIDENTIFIED KEYWORD: " + vectorOfTokens[0], ParserExceptionEnum::UNIDENTIFIED_EXCEPTION);
		}
	}
	else if (vectorOfTokens[0].compare("print") == 0) {
		return StatementTypeEnum::PRINT;
	}
	else if (vectorOfTokens[0].compare("read") == 0) {
		return StatementTypeEnum::READ;
	}
	else if (vectorOfTokens[0].compare("call") == 0) {
		return StatementTypeEnum::CALL;
	}
	else {
		throw ParserException("UNIDENTIFIED STATEMENT LINE: " + inputLine, ParserExceptionEnum::UNIDENTIFIED_EXCEPTION);
	}
}
