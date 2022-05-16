#pragma once
#include <string>
#include <queue>

class StandardPostfixEvaluator {
public:
	static bool evaluateStandardPostfix(std::queue<std::string> queueOfTokens);
};
