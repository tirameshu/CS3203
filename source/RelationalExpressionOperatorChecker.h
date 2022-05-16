#pragma once
#include <string>

class RelationalExpressionOperatorChecker {
public:
	// Returns true if the operator is indeed a relational operator
	static bool checkOperator(std::string &relationalOperator);
};
