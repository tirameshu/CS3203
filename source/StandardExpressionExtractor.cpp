#include <queue>
#include <iostream>

#include "StandardExpressionExtractor.h"
#include "ExpressionTokenizer.h"
#include "ParserException.h"
#include "ParserExceptionEnum.h"
#include "ParserShuntingConverter.h"
#include "StandardPostfixEvaluator.h"
#include "LexicalSyntaxCheck.h"

std::vector<std::unordered_set<std::string>> StandardExpressionExtractor::extractVariablesAndConstants(std::string &expression) {
	
	/*=== TOKENIZE ===*/
	std::vector<std::string> vectorOfTokens = ExpressionTokenizer::tokenize(expression);

	if (vectorOfTokens.size() <= 2) {
		throw ParserException("EMPTY ASSIGN EXPRESSION: " + expression, ParserExceptionEnum::ASSIGN_EXCEPTION);
	}

	/*=== VALIDATE ===*/
	if (vectorOfTokens[1].compare("=") != 0) { // If the 2nd item is not "="
		throw ParserException("INVALID ASSIGNMENT FORMAT: " + expression, ParserExceptionEnum::ASSIGN_EXCEPTION);
	}

	// Creating sub token of just expressions
	// Take the 2nd item onwards after "="
	std::vector<std::string> expressionTokens;
	for (int i = 2; i < vectorOfTokens.size(); i++) {
		expressionTokens.push_back(vectorOfTokens[i]);
	}

	// Get postfix of the expression
	std::queue<std::string> queueOfOutputs = ParserShuntingConverter::convert(expressionTokens, expression, false);

	// Validate the correctness of the postfix
	bool isValid = StandardPostfixEvaluator::evaluateStandardPostfix(queueOfOutputs);

	if (!isValid) {
		throw ParserException("INVALID EXPRESSION FORMAT: " + expression, ParserExceptionEnum::STANDARD_EXPRESSION_EXCEPTION);
	}

	/*=== EXTRACTING VARIABLES OR CONSTANTS ===*/
	std::unordered_set<std::string> listOfVariables;
	std::unordered_set<std::string> listOfConstants;

	while (!queueOfOutputs.empty()) {
		std::string currentToken = queueOfOutputs.front();
		queueOfOutputs.pop();

		if (LexicalSyntaxCheck::checkName(currentToken)) {
			listOfVariables.insert(currentToken);
		}
		else if (LexicalSyntaxCheck::checkInteger(currentToken)) {
			listOfConstants.insert(currentToken);
		}
		// Already valid so no need to validate anymore
		else {
			continue;
		}
	}

	std::vector<std::unordered_set<std::string>> output;
	output.push_back(listOfVariables);
	output.push_back(listOfConstants);
	
	return output;
}
