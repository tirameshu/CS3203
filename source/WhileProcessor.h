#pragma once
#include <string>

#include "PKB.h"

/*
	WhileProcessor is responsbile for verifying the validity of while statements identified
	by the Parser (to ensure that no SIMPLE grammar violations are made), and to tokenize
	and extract key information regarding while statements to populate the PKB
*/

class WhileProcessor {
public:
	static bool processWhile(std::string &inputLine, std::string &proc, int &statementNumber);
};
