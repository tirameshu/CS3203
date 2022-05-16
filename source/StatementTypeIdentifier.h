#pragma once

#include <stdio.h>
#include <iostream>
#include <string>
#include <list>
#include "Statement.h"

/*
	StatementTypeIdentifier is a class that parses the a given statement string and
	returns the integer corresponding to the statement type that was identified
*/

class StatementTypeIdentifier {
public: 
	// General validator that can just call
	// Returns the statement type based on PKB's enum
	static int identifyStatement(std::string &inputLine);
};
