#pragma once
#include "PKB.h"

/*
	ChildProcessor is responsbile for verifying the validity of nested statements enclosed by
	container statements as identified by the Parser (to ensure that no SIMPLE grammar violations
	are made), and to tokenize and extract key information regarding the statements to populate the PKB
*/

class ChildProcessor {
public:
	static bool processChild(int &parentStatementNumber, int &parentStatementType, int &childStatementNumber);
};
