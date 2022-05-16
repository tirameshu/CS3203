#include "ExpressionParser.h"

int ExpressionParser::getPrecedence(char c) {
	if (c == '*' || c == '/' || c == '%') {
		return 2;
	}
	else if (c == '+' || c == '-') {
		return 1;
	}

	return 0;
}

std::string ExpressionParser::getPostfixFromInfix(std::string infix_expression) {
	std::stack<char> terminalStack;
	int infix_expr_len = infix_expression.length();
	std::string postfix_expr;

	for (int i = 0; i < infix_expr_len; i++) {
		char curr = infix_expression[i];
		
		if (isalnum(curr)) {
			postfix_expr += curr;
		}
		else if (curr == '(') {
			terminalStack.push(curr);
		}
		else if (curr == ')') {
			while (terminalStack.top() != '(') {
				char op = terminalStack.top();
				terminalStack.pop();
				postfix_expr += op;
			}

			terminalStack.pop();
		}
		else {
			while (!terminalStack.empty() && getPrecedence(curr) <= getPrecedence(terminalStack.top())) {
				char op = terminalStack.top();
				terminalStack.pop();
				postfix_expr += op;
			}

			terminalStack.push(curr);
		}
	}

	while (!terminalStack.empty()) {
		char op = terminalStack.top();
		terminalStack.pop();
		postfix_expr += op;
	}

	return postfix_expr;
}
