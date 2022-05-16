#include <ctype.h>
#include <queue>
#include <unordered_set>

#include "ConditionalExpressionExtractor.h"
#include "ParserException.h"
#include "ParserExceptionEnum.h"
#include "ExpressionTokenizer.h"
#include "ParserShuntingConverter.h"
#include "LexicalSyntaxCheck.h"
#include "ConditionalPostfixEvaluator.h"

std::vector<std::unordered_set<std::string>> ConditionalExpressionExtractor::extractVariablesAndConstants(std::string &expression) {


	// trivial error check
	if (expression.empty()) {
		throw ParserException("EMPTY EXPRESSION: " + expression, CONDITIONAL_EXPRESSION_EXCEPTION);
	}

	/*=== TOKENIZE EXPRESSION ===*/
	std::vector<std::string> vectorOfTokens = ExpressionTokenizer::tokenize(expression);

	/*=== CONVERT INFIX TO POSTFIX ===*/
	std::queue<std::string> queueOfOutputs = ParserShuntingConverter::convert(vectorOfTokens, expression, true); // Contains validation

	/*=== INITIALIZING RETURN RESULT ====*/
	// Unordered sets are used to remove duplicates
	std::unordered_set<std::string> listOfVariables;
	std::unordered_set<std::string> listOfConstants;

	/*=== PROCESSING POSTFIX EXPRESSION ===*/
	bool isValidConditional = ConditionalPostfixEvaluator::evaluateConditionalPostfix(queueOfOutputs);

	if (!isValidConditional) {
		throw ParserException("CONDITIONAL EXPRESSION " + expression + " IS NOT VALID", ParserExceptionEnum::CONDITIONAL_EXPRESSION_EXCEPTION);
	}

	/*=== EXTRACTING VARIABLES OR CONSTANTS ===*/
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
