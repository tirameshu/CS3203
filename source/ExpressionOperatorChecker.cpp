#include "ExpressionOperatorChecker.h"
#include <iostream>

bool ExpressionOperatorChecker::checkOperator(std::string &expressionOperator) {
	if (expressionOperator.compare("+") == 0
		|| expressionOperator.compare("-") == 0
		|| expressionOperator.compare("*") == 0
		|| expressionOperator.compare("/") == 0
		|| expressionOperator.compare("%") == 0) {
		return true;
	}
	else {
		return false;
	}
}

bool ExpressionOperatorChecker::isLeftOperatorHigherThanRight(std::string &leftOperator, std::string &rightOperator) {
	int precedenceOfLeftOperator = ExpressionOperatorChecker::getPrecendence(leftOperator);
	int precedenceOfRightOperator = ExpressionOperatorChecker::getPrecendence(rightOperator);

	return precedenceOfLeftOperator >= precedenceOfRightOperator;
}

int ExpressionOperatorChecker::getPrecendence(std::string &expressionOperator) {
	if (expressionOperator.compare("*") == 0
		|| expressionOperator.compare("/") == 0
		|| expressionOperator.compare("%") == 0) {
		return 4;
	}
	else if (
		expressionOperator.compare("+") == 0
		|| expressionOperator.compare("-") == 0) {
		return 3;
	}
	else if (
		expressionOperator.compare(">=") == 0
		|| expressionOperator.compare(">") == 0
		|| expressionOperator.compare("<=") == 0
		|| expressionOperator.compare("<") == 0
		) {
		return 2;
	}
	else if (
		expressionOperator.compare("==") == 0
		|| expressionOperator.compare("!=") == 0
		) {
		return 1;
	}
	else {
		return 0;
	}
}
