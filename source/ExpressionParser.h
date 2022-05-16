#pragma once

#include <stack>
#include <string>

namespace ExpressionParser {

	// Returns an int representing the order of precedence of an operator
	int getPrecedence(char c);

	// Returns a string representing the postfix notation of a valid infix expression that has whitespaces removed
	std::string getPostfixFromInfix(std::string infix_expression);
}
