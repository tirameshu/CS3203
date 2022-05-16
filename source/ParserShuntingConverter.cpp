#include <stack>
#include <iostream>

#include "ParserShuntingConverter.h"
#include "LexicalSyntaxCheck.h"
#include "ExpressionOperatorChecker.h"
#include "ConditionalExpressionOperatorChecker.h"
#include "RelationalExpressionOperatorChecker.h"
#include "ParserException.h"
#include "ParserExceptionEnum.h"

std::queue<std::string> ParserShuntingConverter::convert(std::vector<std::string> &vectorOfTokens, std::string &expression, bool isConditional) {
	/*=== EXPRESSION VALIDATOR - ROBY TANAMA ft. Shunting Yard Algorithm ===*/
	// STRONZO

	// 1. Start with initializing a stack for operations, a queue for outputs, and 1 array of tokens (which we already have)
	std::stack<std::string> stackOfOperators;
	std::queue<std::string> queueOfOutputs;

	// To check for existence of relational operator as without relational operator, the if statement is invalid
	bool hasRelationalOperator = false;

	// To check for alternating operator and operand
	bool expectOperand = false;
	bool expectOperator = false;

	// To ensure that (a < b) && (b < d) || (c > d) does not happen (cannot stack conditional operator at the same priority level)
	int priorityLevel = 0;
	bool isFirstConditionalOperator = true;

	// 2. Shunting Yard Algorithm starts here baby YEAHHHHH
	for (int i = 0; i < vectorOfTokens.size(); i++) {

		// If the token is a variable/constant, add the token to the queueOfOutputs
		if (LexicalSyntaxCheck::checkName(vectorOfTokens[i])
			|| LexicalSyntaxCheck::checkInteger(vectorOfTokens[i])) {

			if (expectOperator) {
				throw ParserException("OPERAND " + vectorOfTokens[i] + " AND OPERATOR NEEDS TO ALTERNATE: " + expression, ParserExceptionEnum::CONDITIONAL_EXPRESSION_EXCEPTION);
			}

			queueOfOutputs.push(vectorOfTokens[i]);
			expectOperand = false;
			expectOperator = true;
		}

		// If the token is an expression operator 
		else if (ExpressionOperatorChecker::checkOperator(vectorOfTokens[i])) {

			if (expectOperand) {
				throw ParserException("OPERAND AND OPERATOR " + vectorOfTokens[i] + " NEEDS TO ALTERNATE: " + expression, ParserExceptionEnum::CONDITIONAL_EXPRESSION_EXCEPTION);
			}

			// While there is an operator on the top of the stack with greater precedence, pop operators from stack to output queue
			// Check empty
			while (!stackOfOperators.empty() && ExpressionOperatorChecker::isLeftOperatorHigherThanRight(stackOfOperators.top(), vectorOfTokens[i])) {
				// remove the top element of the stack and add it to the queue
				queueOfOutputs.push(stackOfOperators.top());
				stackOfOperators.pop();
			}

			// push the current operator onto the stack
			stackOfOperators.push(vectorOfTokens[i]);
			expectOperator = false;
			expectOperand = true;
		}

		// If the token is relation operator, just add to stack cause no precedence
		else if (RelationalExpressionOperatorChecker::checkOperator(vectorOfTokens[i])) {

			if (expectOperand) {
				throw ParserException("OPERAND AND OPERATOR " + vectorOfTokens[i] + " NEEDS TO ALTERNATE: " + expression, ParserExceptionEnum::CONDITIONAL_EXPRESSION_EXCEPTION);
			}

			if (!isConditional) {
				throw ParserException("THIS EXPRESSION IS NOT MEANT TO HAVE A RELATIONAL OPERATOR: " + expression, ParserExceptionEnum::STANDARD_EXPRESSION_EXCEPTION);
			}

			// While there is an operator on the top of the stack with greater precedence, pop operators from stack to output queue
			// Check empty
			while (!stackOfOperators.empty() && ExpressionOperatorChecker::isLeftOperatorHigherThanRight(stackOfOperators.top(), vectorOfTokens[i])) {
				// remove the top element of the stack and add it to the queue
				queueOfOutputs.push(stackOfOperators.top());
				stackOfOperators.pop();
			}

			hasRelationalOperator = true;
			stackOfOperators.push(vectorOfTokens[i]);
			expectOperator = false;
			expectOperand = true;
		}

		// If the token is a conditional operator, perform check if next token is open parentheses first before pushing it onto the stack
		else if (ConditionalExpressionOperatorChecker::checkOperator(vectorOfTokens[i])) {

			if (expectOperand) {
				throw ParserException("OPERAND AND OPERATOR " + vectorOfTokens[i] + " NEEDS TO ALTERNATE: " + expression, ParserExceptionEnum::CONDITIONAL_EXPRESSION_EXCEPTION);
			}

			if (!isConditional) {
				throw ParserException("THIS EXPRESSION IS NOT MEANT TO HAVE A CONDITIONAL OPERATOR: " + expression, ParserExceptionEnum::STANDARD_EXPRESSION_EXCEPTION);
			}

			if (!isFirstConditionalOperator && (priorityLevel == 0)) {
				throw ParserException("CANNOT STACK CONDITIONAL OPERATORS AT THE SAME PRIORITY LEVEL: " + expression, ParserExceptionEnum::CONDITIONAL_EXPRESSION_EXCEPTION);
			}

			int nextIndex = i + 1;

			// Checking for out of bounds
			if (nextIndex >= vectorOfTokens.size()) {
				throw ParserException("NO RIGHT SIDE OPERAND FOR THE CONDITIONAL OPERATOR " + vectorOfTokens[i] + " AT " + expression, ParserExceptionEnum::CONDITIONAL_EXPRESSION_EXCEPTION);
			}

			// Checking that the next token is an open parentheses (this is a must)
			if (vectorOfTokens[nextIndex].compare("(") != 0) {
				// If not open parentheses, return error
				throw ParserException("CONDITIONAL OPERATOR " + vectorOfTokens[i] + " HAS TO BE FOLLOWED BY AN OPEN PARENTHESES: "
					+ expression, ParserExceptionEnum::CONDITIONAL_EXPRESSION_EXCEPTION);
			}

			int previousIndex = i - 1;

			// Checking for out of bounds
			if (previousIndex <= 0) {
				throw ParserException("NO LEFT SIDE OPERAND FOR THE CONDITIONAL OPERATOR " + vectorOfTokens[i] + " AT " + expression, ParserExceptionEnum::CONDITIONAL_EXPRESSION_EXCEPTION);
			}
			// Checking that the previous token is a closed parentheses (this is a must)
			if (vectorOfTokens[previousIndex].compare(")") != 0) {
				throw ParserException("CONDITIONAL OPERATOR " + vectorOfTokens[i] + " HAS TO FOLLOW A CLOSED PARENTHESES: " + expression, ParserExceptionEnum::CONDITIONAL_EXPRESSION_EXCEPTION);
			}

			// If all checks pass can add the operator to the stack
			stackOfOperators.push(vectorOfTokens[i]);
			isFirstConditionalOperator = false;
			priorityLevel = 0;
			expectOperator = false;
			expectOperand = true;
		}

		// If the token is a unary ! operator, perform check that the next token is an open parentheses first before pushing it onto the stack
		else if (vectorOfTokens[i].compare("!") == 0) {
			int nextIndex = i + 1;

			// Checking for out of bounds
			if (nextIndex >= vectorOfTokens.size()) {
				throw ParserException("NO RIGHT SIDE OPERAND FOR ! OPERATOR AT " + expression, ParserExceptionEnum::CONDITIONAL_EXPRESSION_EXCEPTION);
			}

			if (!isConditional) {
				throw ParserException("THIS EXPRESSION IS NOT MEANT TO HAVE NEGATION OPERATOR: " + expression, ParserExceptionEnum::STANDARD_EXPRESSION_EXCEPTION);
			}

			// Checking that the next token is an open parentheses (this is a must)
			if (vectorOfTokens[nextIndex].compare("(") != 0) {
				// If not open parentheses, return error
				throw ParserException("CONDITIONAL OPERATOR " + vectorOfTokens[i] + " HAS TO BE FOLLOWED BY AN OPEN PARENTHESES: "
					+ expression, ParserExceptionEnum::CONDITIONAL_EXPRESSION_EXCEPTION);
			}

			// If all checks pass, can add operator to the stack
			stackOfOperators.push(vectorOfTokens[i]);
		}

		// If the token is a left bracket, push it onto the stack
		else if (vectorOfTokens[i].compare("(") == 0) {
			stackOfOperators.push(vectorOfTokens[i]);
			priorityLevel++;
		}

		// If the token is a right bracket
		else if (vectorOfTokens[i].compare(")") == 0) {
			priorityLevel--;

			// While there is not left bracket at the top of the stack
			while (!stackOfOperators.empty() && stackOfOperators.top().compare("(") != 0) {

				// remove the top element of the stack and add it to the queue
				queueOfOutputs.push(stackOfOperators.top());
				stackOfOperators.pop();
			}

			// Pop the left bracket from the stack and discard it
			if (!stackOfOperators.empty()) { // empty check
				stackOfOperators.pop();
			}
			else {
				throw ParserException("CLOSING PARENTHESES ENCOUNTERED IN EXPRESSION: " + expression + " BEFORE OPENING PARENTHESES", ParserExceptionEnum::CONDITIONAL_EXPRESSION_EXCEPTION);
			}
			
		}
		else {
			throw ParserException("UNIDENTIFIED TOKEN " + vectorOfTokens[i] + " AT " + expression, ParserExceptionEnum::CONDITIONAL_EXPRESSION_EXCEPTION);
		}
	}

	// 2.5 Check if there exist a relational operator in the expression
	if (!hasRelationalOperator && isConditional) {
		throw ParserException("THIS EXPRESSION DOES NOT HAVE A RELATIONAL OPERATOR: " + expression, ParserExceptionEnum::CONDITIONAL_EXPRESSION_EXCEPTION);
	}

	// 3. While there are still operators on the stack, pop them to the queue
	while (!stackOfOperators.empty()) {
		// remove the top element of the stack and add it to the queue
		queueOfOutputs.push(stackOfOperators.top());
		stackOfOperators.pop();
	}

	return queueOfOutputs;
}
