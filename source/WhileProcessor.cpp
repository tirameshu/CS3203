#include "WhileProcessor.h"
#include "ParserException.h"
#include "ParserExceptionEnum.h"
#include "StatementTypeEnum.h"
#include "Statement.h"
#include "ConditionalExpressionExtractor.h"
#include "PKB.h"
#include "ParenthesesValidator.h"

bool WhileProcessor::processWhile(std::string &inputLine, std::string &proc, int &statementNumber) {

	/*=== Initialization ===*/
	int startPosOfExpression = 0; // To extract the expression
	int endPosOfExpression = 0; // To extract the expression

	/*=== Check Parentheses ===*/
	if (!ParenthesesValidator::validateParentheses(inputLine)) {
		throw ParserException("MISMATCHED PARENTHESES AT WHILE CASE: " + inputLine, ParserExceptionEnum::WHILE_EXCEPTION);
	}

	/*=== Extracting expression ===*/
	std::string expression;

	// Finding position of the first parentheses
	for (std::string::size_type index = 0; index < inputLine.size(); index++) {
		if (inputLine[index] == '(') {
			startPosOfExpression = index + 1; // Next character is the start of the expression

			// Check that the start position of expression is not the end of the inputline
			if (startPosOfExpression >= inputLine.size()) {
				throw ParserException("EXPRESSION IS NOT VALID: " + inputLine, ParserExceptionEnum::WHILE_EXCEPTION);
			}

			break; // Found so break
		}
	}

	// Finding position of the last parentheses
	for (std::string::size_type index = startPosOfExpression; index < inputLine.size(); index++) {
		if (inputLine[index] == ')') {
			endPosOfExpression = index;
		}
	}

	// Getting the expression from the substring
	expression = inputLine.substr(startPosOfExpression, endPosOfExpression - startPosOfExpression);

	// Checking that expression is not empty
	if (expression.empty()) {
		throw ParserException("EXPRESSION IS EMPTY", ParserExceptionEnum::WHILE_EXCEPTION);
	}

	// Checking that the while statement ends after )
	if (endPosOfExpression != inputLine.size() - 1) {
		std::string extraWords;
		for (std::string::size_type index = endPosOfExpression + 1; index < inputLine.size(); index++) {
			if (!std::isspace(inputLine[index])) {
				extraWords += inputLine[index];
			}
		}

		if (!extraWords.empty()) { 
			throw ParserException("WHILE STATEMENT NEEDS TO END AFTER EXPRESSION: " + inputLine, ParserExceptionEnum::WHILE_EXCEPTION);
		}
	}

	// ALL VALIDATIONS PASSED
	// 0 is variables, 1 is constants
	std::vector<std::unordered_set<std::string>> listOfVariablesAndConstantsUsed = ConditionalExpressionExtractor::extractVariablesAndConstants(expression);

	// Call PKB methods to store statement
	Statement statement = Statement(StatementTypeEnum::WHILE, statementNumber);
	PKB::addStatement(proc, statement);

	// Call PKB methods to handle relationships
	if (!listOfVariablesAndConstantsUsed[0].empty()) {
		PKB::addUses(statementNumber, StatementTypeEnum::WHILE, listOfVariablesAndConstantsUsed[0]);
		PKB::addProcUses(proc, listOfVariablesAndConstantsUsed[0]);
		PKB::addWhileUses(statementNumber, listOfVariablesAndConstantsUsed[0]);
	}

	for (const std::string& element : listOfVariablesAndConstantsUsed[0]) {
		PKB::addVariable(element);
	}

	for (const std::string& element : listOfVariablesAndConstantsUsed[1]) {
		PKB::addConstant(statementNumber, element);
	}

	return true;
}
