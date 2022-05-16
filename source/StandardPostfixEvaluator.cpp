#include <stack>
#include <iostream>

#include "StandardPostfixEvaluator.h"
#include "ExpressionTypeEnum.h"
#include "LexicalSyntaxCheck.h"
#include "ExpressionOperatorChecker.h"
#include "ParserException.h"
#include "ParserExceptionEnum.h"

bool StandardPostfixEvaluator::evaluateStandardPostfix(std::queue<std::string> queueOfTokens) {

	std::stack<int> stackOfOperands;

	while (!queueOfTokens.empty()) {
		
		std::string currentToken = queueOfTokens.front();
		queueOfTokens.pop();

		/* If operand is encountered
		*	1. Add token to stack of operands as FACTOR type
		*/

		if (LexicalSyntaxCheck::isVariableOrConstant(currentToken)) {
			stackOfOperands.push(ExpressionTypeEnum::FACTOR);
		} 

		/* If operator is encountered
		*	1. Take two from stack of operands of either STANDARD_EXPRESSION or FACTOR
		*	2. Push back result as STANDARD_EXPRESSION
		*/

		else if (ExpressionOperatorChecker::checkOperator(currentToken)) {

			// Check empty
			if (stackOfOperands.empty()) {
				throw ParserException("NO SUITABLE OPERAND FOUND FOR " + currentToken, ParserExceptionEnum::STANDARD_EXPRESSION_EXCEPTION);
			}

			int leftOperand = stackOfOperands.top();
			stackOfOperands.pop();

			// Check left operand type
			if (!(leftOperand == ExpressionTypeEnum::FACTOR) && !(leftOperand == ExpressionTypeEnum::STANDARD_EXPRESSION)) {
				throw ParserException("WRONG LEFT OPERAND TYPE FOR " + currentToken, ParserExceptionEnum::STANDARD_EXPRESSION_EXCEPTION);
			}

			// Check empty
			if (stackOfOperands.empty()) {
				throw ParserException("MISSING ONE MORE SUITABLE OPERAND FOR " + currentToken, ParserExceptionEnum::STANDARD_EXPRESSION_EXCEPTION);
			}

			int rightOperand = stackOfOperands.top();
			stackOfOperands.pop();

			if (!(rightOperand == ExpressionTypeEnum::FACTOR) && !(rightOperand == ExpressionTypeEnum::STANDARD_EXPRESSION)) {
				throw ParserException("WRONG RIGHT OPERAND TYPE FOR " + currentToken, ParserExceptionEnum::STANDARD_EXPRESSION_EXCEPTION);
			}

			/*=== CHECK PASSED ===*/
			stackOfOperands.push(ExpressionTypeEnum::STANDARD_EXPRESSION);
		}

		else {
			throw ParserException("INVALID TOKEN FOUND IN ASSIGNMENT EXPRESSION " + currentToken, ParserExceptionEnum::STANDARD_EXPRESSION_EXCEPTION);
		}
	}

	/*=== RESULT CHECKING ===*/
	if (stackOfOperands.empty()) {
		throw ParserException("FOUND EXPRESSION THAT EVALUATES TO NOTHING", ParserExceptionEnum::STANDARD_EXPRESSION_EXCEPTION);
	}

	int result = stackOfOperands.top();
	stackOfOperands.pop();

	if (!stackOfOperands.empty()) {
		throw ParserException("ASSIGNMENT EXPRESSION RETURNING MULTIPLE RESULTS", ParserExceptionEnum::STANDARD_EXPRESSION_EXCEPTION);
	}

	if (!(result == ExpressionTypeEnum::FACTOR) && !(result == ExpressionTypeEnum::STANDARD_EXPRESSION)) {
		throw ParserException("WRONG RESULT OF EXPRESSION FOR ASSIGNMENT STATEMENT", ParserExceptionEnum::STANDARD_EXPRESSION_EXCEPTION);
	}

	return true;
}
