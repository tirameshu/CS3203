#pragma once
#include <vector>
#include <string>

class ExpressionTokenizer {
public:
	static std::vector<std::string> tokenize(std::string &expression);
};
