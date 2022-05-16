#pragma once
#include <string>

#include "PKB.h"

/*
	IfProcessor is responsbile for verifying the validity of if statements identified
	by the Parser (to ensure that no SIMPLE grammar violations are made), and to tokenize
	and extract key information regarding if statements to populate the PKB
*/

class IfProcessor {
public:
	static bool processIf(std::string &inputLine, std::string &proc, int &statementNumber);
};
