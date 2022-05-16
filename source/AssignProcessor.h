#pragma once
#include <string>
#include "PKB.h"

/*
	AssignProcessor is responsbile for verifying the validity of assign statements identified
	by the Parser (to ensure that no SIMPLE grammar violations are made), and to tokenize
	and extract key information regarding assign statements to populate the PKB
*/

class AssignProcessor {
public:
	static bool processAssign(std::string &inputLine, std::string &proc, int &statementNumber);
};
