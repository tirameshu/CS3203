#pragma once
#include <string>
#include "PKB.h"

/*
	ReadProcessor is responsbile for verifying the validity of read statements identified
	by the Parser (to ensure that no SIMPLE grammar violations are made), and to tokenize
	and extract key information regarding read statements to populate the PKB
*/

class ReadProcessor {
public:
	// Check validity and call PKB API to store print and all relevant relationship 
	static bool processRead(std::string &inputLine, std::string &proc, int &statementNumber);
};
