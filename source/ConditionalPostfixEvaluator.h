#pragma once
#include <string>
#include <queue>

class ConditionalPostfixEvaluator {
public:
	static bool evaluateConditionalPostfix(std::queue<std::string> queueOfTokens);
};
