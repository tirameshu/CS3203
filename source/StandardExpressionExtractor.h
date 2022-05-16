#pragma once
#include <vector>
#include <unordered_set>
#include <string>
#include <tuple>

class StandardExpressionExtractor {
public:
	static std::vector<std::unordered_set<std::string>> extractVariablesAndConstants(std::string &expression);
};
