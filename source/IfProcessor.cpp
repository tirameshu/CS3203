#include "IfProcessor.h"
#include "ParserException.h"
#include "ParserExceptionEnum.h"
#include "StatementTypeEnum.h"
#include "Statement.h"
#include "ConditionalExpressionExtractor.h"
#include "ExpressionTokenizer.h"
#include "ParenthesesValidator.h"

bool IfProcessor::processIf(std::string &inputLine, std::string &proc, int &statementNumber) {
	
	/*=== Initialization ===*/
	int startPosOfExpression = 0; // To extract the expression
	int endPosOfExpression = 0; // To extract the expression
	std::string const THEN_KEYWORD = "then";

	/*=== Check Parentheses ===*/
	if (!ParenthesesValidator::validateParentheses(inputLine)) {
		throw ParserException("MISMATCHED PARENTHESES AT IF CASE: " + inputLine, ParserExceptionEnum::IF_EXCEPTION);
	}

	/*=== Extracting expression ===*/
	std::string expression;

	// Finding position of the first parentheses
	for (std::string::size_type index = 0; index < inputLine.size(); index++) {
		if (inputLine[index] == '(') {
			startPosOfExpression = index + 1; // Next character is the start of the expression

			// Check that the start position of expression is not the end of the inputline
			if (startPosOfExpression >= inputLine.size()) {
				throw ParserException("EXPRESSION IS NOT VALID: " + inputLine, ParserExceptionEnum::IF_EXCEPTION);
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
		throw ParserException("EXPRESSION IS EMPTY", ParserExceptionEnum::IF_EXCEPTION);
	}

	std::string keywordCheck = inputLine.substr(endPosOfExpression);

	std::vector<std::string> tokenizedThen = ExpressionTokenizer::tokenize(keywordCheck);

	if (tokenizedThen.size() != 2) {
		throw ParserException("Invalid then keyword at: " + expression, ParserExceptionEnum::IF_EXCEPTION);
	}

	if (tokenizedThen[1].compare(THEN_KEYWORD) != 0) {
		throw ParserException("Invalid then keyword at: " + expression, ParserExceptionEnum::IF_EXCEPTION);
	}

	// ALL VALIDATIONS PASSED
	// 0 is variables, 1 is constants
	std::vector<std::unordered_set<std::string>> listOfVariablesAndConstantsUsed = ConditionalExpressionExtractor::extractVariablesAndConstants(expression);

	// Call PKB methods to store statement
	Statement statement = Statement(StatementTypeEnum::IF, statementNumber);
	PKB::addStatement(proc, statement);

	for (const std::string& element : listOfVariablesAndConstantsUsed[0]) {
		PKB::addVariable(element);
	}

	// Call PKB methods to handle relationships
	if (!listOfVariablesAndConstantsUsed[0].empty()) {
		PKB::addUses(statementNumber, StatementTypeEnum::IF, listOfVariablesAndConstantsUsed[0]);
		PKB::addProcUses(proc, listOfVariablesAndConstantsUsed[0]);
		PKB::addIfUses(statementNumber, listOfVariablesAndConstantsUsed[0]);
	}

	for (const std::string& element : listOfVariablesAndConstantsUsed[1]) {
		PKB::addConstant(statementNumber, element);
	}

	return true;
}
