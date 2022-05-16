#include <stack>

#include "ConditionalPostfixEvaluator.h"
#include "ExpressionTypeEnum.h"
#include "LexicalSyntaxCheck.h"
#include "ExpressionOperatorChecker.h"
#include "ParserException.h"
#include "ParserExceptionEnum.h"
#include "RelationalExpressionOperatorChecker.h"
#include "ConditionalExpressionOperatorChecker.h"

bool ConditionalPostfixEvaluator::evaluateConditionalPostfix(std::queue<std::string> queueOfTokens) {
	
	/*=== INITIALIZATION ===*/
	std::stack<int> tokenStack; // Checking for grammar rule

	/*=== ITERATE ===*/
	while (!queueOfTokens.empty()) {

		std::string currentToken = queueOfTokens.front();
		queueOfTokens.pop();

		/* If a variable or constant is encountered
		*	1. Add to tokenStack as FACTOR
		*	2. Move on
		*/

		if (LexicalSyntaxCheck::isVariableOrConstant(currentToken)) {
			tokenStack.push(ExpressionTypeEnum::FACTOR);
		}

		/* If a standard expression operator is encountered
		*	1. Check tokenStack for 2 of either FACTOR or STANDARD_EXPRESSION (pop both)
		*	2. Push result back to stack as STANDARD_EXPRESSION
		*/

		else if (ExpressionOperatorChecker::checkOperator(currentToken)) {

			/*=== LEFT OPERAND ===*/
			if (tokenStack.empty()) {
				throw ParserException("EXPRESSION OPERATOR " + currentToken + " HAS NO SUITABLE OPERANDS", ParserExceptionEnum::CONDITIONAL_EXPRESSION_EXCEPTION);
			}

			int leftOperand = tokenStack.top();
			tokenStack.pop();

			// Check that left operand is either factor or standard expression
			if (!(leftOperand == ExpressionTypeEnum::FACTOR) && !(leftOperand == ExpressionTypeEnum::STANDARD_EXPRESSION)) {
				throw ParserException("EXPRESSION OPERATOR " + currentToken + " IS BEING FED THE WRONG LEFT OPERAND TYPE", ParserExceptionEnum::CONDITIONAL_EXPRESSION_EXCEPTION);
			}

			/*=== RIGHT OPERAND ===*/
			if (tokenStack.empty()) {
				throw ParserException("EXPRESSION OPERATOR" + currentToken + " IS MISSING ONE MORE SUITABLE OPERAND", ParserExceptionEnum::CONDITIONAL_EXPRESSION_EXCEPTION);
			}

			int rightOperand = tokenStack.top();
			tokenStack.pop();

			if (!(rightOperand == ExpressionTypeEnum::FACTOR) && !(rightOperand == ExpressionTypeEnum::STANDARD_EXPRESSION)) {
				throw ParserException("EXPRESSION OPERATOR " + currentToken + " IS BEING FED THE WRONG RIGHT OPERAND TYPE", ParserExceptionEnum::CONDITIONAL_EXPRESSION_EXCEPTION);
			}

			/*=== RESULT ===*/
			tokenStack.push(ExpressionTypeEnum::STANDARD_EXPRESSION);
		}

		/* If a relational expression operator is encountered
		*	1. Check tokenstack for 2 of either FACTOR or STANDARD_EXPRESSION (pop both)
		*	2. Push result back to stack as RELATIONAL_EXPRESSION
		*/

		else if (RelationalExpressionOperatorChecker::checkOperator(currentToken)) {

			/*=== LEFT OPERAND ===*/
			if (tokenStack.empty()) {
				throw ParserException("RELATIONAL OPERATOR " + currentToken + " HAS NO SUITABLE OPERANDS", ParserExceptionEnum::CONDITIONAL_EXPRESSION_EXCEPTION);
			}

			int leftOperand = tokenStack.top();
			tokenStack.pop();

			// Check that left operand is either factor or standard expression
			if (!(leftOperand == ExpressionTypeEnum::FACTOR) && !(leftOperand == ExpressionTypeEnum::STANDARD_EXPRESSION)) {
				throw ParserException("RELATIONAL OPERATOR " + currentToken + " IS BEING FED THE LEFT WRONG OPERAND TYPE", ParserExceptionEnum::CONDITIONAL_EXPRESSION_EXCEPTION);
			}

			/*=== RIGHT OPERAND ===*/
			if (tokenStack.empty()) {
				throw ParserException("RELATIONAL OPERATOR" + currentToken + " IS MISSING ONE MORE SUITABLE OPERAND", ParserExceptionEnum::CONDITIONAL_EXPRESSION_EXCEPTION);
			}

			int rightOperand = tokenStack.top();
			tokenStack.pop();

			if (!(rightOperand == ExpressionTypeEnum::FACTOR) && !(rightOperand == ExpressionTypeEnum::STANDARD_EXPRESSION)) {
				throw ParserException("RELATIONAL OPERATOR " + currentToken + " IS BEING FED THE RIGHT WRONG OPERAND TYPE", ParserExceptionEnum::CONDITIONAL_EXPRESSION_EXCEPTION);
			}

			/*=== RESULT ===*/
			tokenStack.push(ExpressionTypeEnum::RELATIONAL_EXPRESSION);
		}

		/* If a conditional expressino operator is encountered
		*	1. Check tokenstack for 2 of either RELATIONAL_EXPRESSION or CONDITIONAL_EXPRESSION
		*	2. Push result back to stack as CONDITIONAL_EXPRESSION
		*/

		else if (ConditionalExpressionOperatorChecker::checkOperator(currentToken)) {

			/*=== LEFT OPERAND ===*/
			if (tokenStack.empty()) {
				throw ParserException("CONDITIONAL OPERATOR " + currentToken + " HAS NO SUITABLE OPERANDS", ParserExceptionEnum::CONDITIONAL_EXPRESSION_EXCEPTION);
			}

			int leftOperand = tokenStack.top();
			tokenStack.pop();

			// Check that left operand is either factor or standard expression
			if (!(leftOperand == ExpressionTypeEnum::RELATIONAL_EXPRESSION) && !(leftOperand == ExpressionTypeEnum::CONDITIONAL_EXPRESSION)) {
				throw ParserException("CONDITIONAL OPERATOR " + currentToken + " IS BEING FED THE WRONG LEFT OPERAND TYPE", ParserExceptionEnum::CONDITIONAL_EXPRESSION_EXCEPTION);
			}

			/*=== RIGHT OPERAND ===*/
			if (tokenStack.empty()) {
				throw ParserException("CONDITIONAL OPERATOR" + currentToken + " IS MISSING ONE MORE SUITABLE OPERAND", ParserExceptionEnum::CONDITIONAL_EXPRESSION_EXCEPTION);
			}

			int rightOperand = tokenStack.top();
			tokenStack.pop();

			if (!(rightOperand == ExpressionTypeEnum::RELATIONAL_EXPRESSION) && !(rightOperand == ExpressionTypeEnum::CONDITIONAL_EXPRESSION)) {
				throw ParserException("CONDITIONAL OPERATOR " + currentToken + " IS BEING FED THE WRONG RIGHT OPERAND TYPE", ParserExceptionEnum::CONDITIONAL_EXPRESSION_EXCEPTION);
			}

			/*=== RESULT ===*/
			tokenStack.push(ExpressionTypeEnum::CONDITIONAL_EXPRESSION);
		}

		/* If ! is encountered
		*	1. Just skip lol
		*/
		else if (currentToken.compare("!") == 0) {
			continue;
		}

		/* Anything else would be invalid */

		else {
			throw ParserException("INVALID TOKEN " + currentToken, ParserExceptionEnum::CONDITIONAL_EXPRESSION_EXCEPTION);
		}
	}

	/* Check that the last tokenStack after the loop ended is either
	*	1. Relational expression
	*	2. or Conditional expression
	* 
	*	Throw error if got more / less than 1 result
	*/
	if (tokenStack.empty()) {
		throw ParserException("DETECTED AN EXPRESSION THAT EVALUATES TO NOTHING", ParserExceptionEnum::CONDITIONAL_EXPRESSION_EXCEPTION);
	}

	int result = tokenStack.top();
	tokenStack.pop();

	if (!tokenStack.empty()) {
		throw ParserException("THIS EXPRESSION EVALUATES TO MORE THAN 1 RESULT", ParserExceptionEnum::CONDITIONAL_EXPRESSION_EXCEPTION);
	}

	if (!(result == ExpressionTypeEnum::CONDITIONAL_EXPRESSION) && !(result == ExpressionTypeEnum::RELATIONAL_EXPRESSION)) {
		throw ParserException("THIS EXPRESSION DOES NOT EVALUATE TO A CONDITIONAL EXCEPTION", ParserExceptionEnum::CONDITIONAL_EXPRESSION_EXCEPTION);
	}

	return true;
}
