#include <vector>
#include <unordered_set>
#include <queue>

#include "AssignProcessor.h"
#include "ParserException.h"
#include "ParserExceptionEnum.h"
#include "StandardExpressionExtractor.h"
#include "ParenthesesValidator.h"
#include "PKB.h"
#include "StatementTypeEnum.h"
#include "LexicalSyntaxCheck.h"
#include "ExpressionTokenizer.h"
#include "ParserShuntingConverter.h"

bool AssignProcessor::processAssign(std::string &inputLine, std::string &proc, int &statementNumber) {
	
	/*=== TRIVIAL CHECKS ===*/
	// Check for empty input
	if (inputLine.empty()) {
		throw ParserException("ASSIGN INPUT LINE CANNOT BE EMPTY", ParserExceptionEnum::ASSIGN_EXCEPTION);
	}

	// Check for invalid parentheses
	if (!ParenthesesValidator::validateParentheses(inputLine)) {
		throw ParserException("INVALID PARENTHESES: " + inputLine, ParserExceptionEnum::ASSIGN_EXCEPTION);
	}
	
	/*=== VALIDATOR AND EXTRACTOR ===*/
	// Validating and extracting modified variable
	std::string modifiedVariable = "";
	int startPosOfExpression = 0;
	for (std::string::size_type i = 0; i < inputLine.size(); i++) {
		if (std::isalnum(inputLine[i])) {
			modifiedVariable += inputLine[i];
		}
		else if (std::isspace(inputLine[i])) {
			continue;
		}
		else {
			startPosOfExpression = i + 1;
			break;
		}
	}

	if (modifiedVariable.empty()) {
		throw ParserException("MODIFIED VARIABLE CANNOT BE FOUND", ParserExceptionEnum::ASSIGN_EXCEPTION);
	}

	// validate modified variable is valid variable name
	if (!LexicalSyntaxCheck::checkName(modifiedVariable)) {
		throw ParserException("INVALID MODIFIED VARIABLE FORMAT: " + modifiedVariable, ParserExceptionEnum::ASSIGN_EXCEPTION);
	}

	// 0 is variables used, 1 is constants used, 2 is variable/constant modified
	std::vector<std::unordered_set<std::string>> setOfVariablesAndConstants = StandardExpressionExtractor::extractVariablesAndConstants(inputLine);

	/*=== STORING ===*/
	// Call PKB methods to store statement
	Statement statement = Statement(StatementTypeEnum::ASSIGN, statementNumber);
	PKB::addStatement(proc, statement);

	std::string expression;
	if (startPosOfExpression < inputLine.size()) {
		expression = inputLine.substr(startPosOfExpression);
	}

	std::vector<std::string> expressionTokens = ExpressionTokenizer::tokenize(expression);
	std::queue<std::string> expressionPostfixQueue = ParserShuntingConverter::convert(expressionTokens, expression, false);

	std::string postfixExpression;
	// no validation needed as it's already done above, just get the string
	while (!expressionPostfixQueue.empty()) {
		postfixExpression += expressionPostfixQueue.front() + " ";
		expressionPostfixQueue.pop();
	}

	int lastPos = postfixExpression.size() - 1;
	for (int i = lastPos; i >= 0; i--) {
		if (!std::isspace(postfixExpression[i])) {
			lastPos = i;
			break;
		}
	}

	postfixExpression = postfixExpression.substr(0, lastPos + 1);

	PKB::addAssign(statementNumber, modifiedVariable, postfixExpression);

	for (const std::string& element : setOfVariablesAndConstants[0]) {
		PKB::addVariable(element);
	}

	PKB::addVariable(modifiedVariable);

	// Call PKB methods to handle relationships
	if (!setOfVariablesAndConstants[0].empty()) {
		PKB::addUses(statementNumber, StatementTypeEnum::ASSIGN, setOfVariablesAndConstants[0]);
		PKB::addProcUses(proc, setOfVariablesAndConstants[0]);
	}

	for (const std::string& element : setOfVariablesAndConstants[1]) {
		PKB::addConstant(statementNumber, element);
	}

	PKB::addModifies(statementNumber, StatementTypeEnum::ASSIGN, modifiedVariable);
	std::unordered_set<std::string> setOfModifiedVars;
	setOfModifiedVars.insert(modifiedVariable);

	if (!setOfModifiedVars.empty()) {
		PKB::addModifiedVarToProc(proc, setOfModifiedVars);
	}
	
	return true;
}
