#pragma once
#include <unordered_set>
#include <vector>
#include <string>

class ConditionalExpressionExtractor {
public:
	static std::vector<std::unordered_set<std::string>> extractVariablesAndConstants(std::string& expression); // [0] is variable, [1] is constants
};
