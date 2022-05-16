#pragma once
#include <string>

class LexicalSyntaxCheck {
public:
	static bool checkName(std::string &name);
	static bool checkInteger(std::string &number);
	static bool isVariableOrConstant(std::string &input);
};
