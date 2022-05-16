#include "RelationalExpressionOperatorChecker.h"

bool RelationalExpressionOperatorChecker::checkOperator(std::string &relationalOperator) {
	// Compare the string against all the possible conditional operators
	const std::string BIGGER_THAN = ">";
	const std::string BIGGER_THAN_EQUALS = ">=";
	const std::string SMALLER_THAN = "<";
	const std::string SMALLER_THAN_EQUALS = "<=";
	const std::string EQUALS = "==";
	const std::string NOT_EQUALS = "!=";



	if (relationalOperator.compare(BIGGER_THAN) == 0
		|| relationalOperator.compare(BIGGER_THAN_EQUALS) == 0
		|| relationalOperator.compare(SMALLER_THAN) == 0
		|| relationalOperator.compare(SMALLER_THAN_EQUALS) == 0
		|| relationalOperator.compare(EQUALS) == 0
		|| relationalOperator.compare(NOT_EQUALS) == 0)  {
		return true;
	}
	else {
		return false;
	}
}
