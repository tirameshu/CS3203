#include "ParenthesesValidator.h"
#include "ParserException.h"
#include "ParserExceptionEnum.h"

bool ParenthesesValidator::validateParentheses(std::string &input) {
	int noOfOpenParentheses = 0;

	for (std::string::size_type i = 0; i < input.size(); i++) {
		if (input[i] == '(') {
			noOfOpenParentheses++;
		}
		else if (input[i] == ')') {
			noOfOpenParentheses--;

			if (noOfOpenParentheses < 0) {
				return false;
			}
		}
	}

	if (noOfOpenParentheses == 0) {
		return true;
	}
	else {
		return false;
	}
}
