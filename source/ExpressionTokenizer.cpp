#include "ExpressionTokenizer.h"

std::vector<std::string> ExpressionTokenizer::tokenize(std::string &expression) {

	/*=== Initialization ===*/
	std::vector<std::string> outputTokens;
	std::string token; // Current token

	// Useful boolean to know if the recursion is currently building a variable/constant
	bool isBuilding = false;

	/*=== TRIVIAL CASE EMPTY EXPRESSION ===*/
	if (expression.empty()) {
		return outputTokens;
	}
	
	/*=== Iterating through the expression ===*/
	for (std::string::size_type i = 0; i < expression.size(); i++) {

		// If encounter an alphanumeric, add the token into the built string
		if (std::isalnum(expression[i])) {

			if (!isBuilding) {
				if (!token.empty()) {
					outputTokens.push_back(token);
					token.clear();
					isBuilding = false;
				}
			}

			token += expression[i];
			isBuilding = true;
		}

		// If encounter whitespace, cut the string
		else {

			if (expression[i] == '(') {

				if (!token.empty()) {
					outputTokens.push_back(token);
					token.clear();
					isBuilding = false;
				}

				outputTokens.push_back("(");
			}

			else if (expression[i] == ')') {

				if (!token.empty()) {
					outputTokens.push_back(token);
					token.clear();
					isBuilding = false;
				}

				outputTokens.push_back(")");
			}

			else if (std::isspace(expression[i])) {

				if (!token.empty()) {
					outputTokens.push_back(token);
					token.clear();
					isBuilding = false;
				}
			}

			else {

				if (isBuilding) {
					outputTokens.push_back(token);
					token.clear();
					isBuilding = false;
				}

				isBuilding = false;
				token += expression[i];
			}
		}
	}

	if (!token.empty()) {
		outputTokens.push_back(token);
		token.clear();
	}

	return outputTokens;
}
