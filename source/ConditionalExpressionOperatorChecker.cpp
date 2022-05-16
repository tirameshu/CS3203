#include "ConditionalExpressionOperatorChecker.h"

bool ConditionalExpressionOperatorChecker::checkOperator(std::string &conditionalOperator) {
	const std::string AND = "&&";
	const std::string OR = "||";

	if (conditionalOperator.compare(AND) == 0
		|| conditionalOperator.compare(OR) == 0) {
		return true;
	}
	else {
		return false;
	}
}
