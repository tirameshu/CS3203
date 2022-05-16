#pragma once
#include <string>

class ExpressionOperatorChecker {
public:
	static bool checkOperator(std::string &expressionOperator);
	static bool isLeftOperatorHigherThanRight(std::string &leftOperator, std::string &rightOperator);

private: 
	static int getPrecendence(std::string &expressionOperator);
};
