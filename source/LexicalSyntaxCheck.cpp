#include <ctype.h>

#include "LexicalSyntaxCheck.h"

bool LexicalSyntaxCheck::checkName(std::string &name) {

	// The case that the string is empty, return false
	if (name.empty()) {
		return false;
	}

	// Checking if the first character is a LETTER
	if (!isalpha(name[0])) {
		return false; // return false if the first character is not a letter
	}

	// All good time to iterate
	
	// iterate through each character
	for (std::string::size_type index = 1; index < name.size(); index++) { // Start from 1 cause 0 is checked
		// Just need to verify that each character is alphanumeric
		if (!isalnum(name[index])) {
			return false; // return false if any of the character is not alphanumeric
		}
	}

	return true;
}

bool LexicalSyntaxCheck::checkInteger(std::string &number) {

	// The case the that string is empty, return false
	if (number.empty()) {
		return false;
	}

	// If number starts with 0 and not the only digit
	if (number[0] == '0' && number.size() > 1) {
		return false;
	}
	
	// iterate through all characters
	for (std::string::size_type index = 0; index < number.size(); index++) {
		if (!isdigit(number[index])) {
			return false; // return false if any of the character is not a digit
		}
	}

	return true;
}

bool LexicalSyntaxCheck::isVariableOrConstant(std::string &input) {
	return (LexicalSyntaxCheck::checkInteger(input) || LexicalSyntaxCheck::checkName(input));
}
