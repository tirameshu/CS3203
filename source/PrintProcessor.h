#pragma once
#include <string>
#include "PKB.h"

/*
	PrintProcessor is responsbile for verifying the validity of print statements identified
	by the Parser (to ensure that no SIMPLE grammar violations are made), and to tokenize
	and extract key information regarding print statements to populate the PKB
*/

class PrintProcessor {
public:
	// Check validity and call PKB API to store print and all relevant relationship 
	static bool processPrint(std::string &inputLine, std::string &proc, int &statementNumber);
};
