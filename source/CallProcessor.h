#pragma once
#include <string>
#include "PKB.h"

/*
	CallProcessor is responsbile for verifying the validity of call statements identified
	by the Parser (to ensure that no SIMPLE grammar violations are made), and to tokenize
	and extract key information regarding call statements to populate the PKB
*/

class CallProcessor {
public:
	static std::string processCall(std::string& inputLine, std::string& proc, int& statementNumber);
};
