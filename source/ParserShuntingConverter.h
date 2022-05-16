#pragma once

#include <string>
#include <vector>
#include <queue>

class ParserShuntingConverter {
public:
	static std::queue<std::string> convert(std::vector<std::string> &vectorOfTokens, std::string &expression, bool isConditional);
};
