#pragma once
#include <stdio.h>
#include <string>
#include <utility>

#include "ArgumentTypes.h"

/*
	QueryClause is the base class that all other Query Clause-related classes inherit from.
	It contains attributes common to all Query clauses.
*/

class QueryClause {
public:
	//constructors
	QueryClause(std::string argument1, std::string argument2, ArgumentTypes firstArgumentType, ArgumentTypes secondArgumentType);
	QueryClause();

	//API to access attributes
	bool getValidity() const;
	bool getSyntacticValidity() const;
	bool getSemanticValidity() const;
	std::pair<std::string, std::string> getArguments() const;
	std::string getFirstArgument() const;
	std::string getSecondArgument() const;
	ArgumentTypes getFirstArgumentType() const;
	ArgumentTypes getSecondArgumentType() const;
	bool operator==(const QueryClause& other);
	void invalidateClause();

protected:
	bool isSyntacticallyValid = true;
	bool isSemanticallyValid = true;
	ArgumentTypes firstArgumentType;
	ArgumentTypes secondArgumentType;
	std::pair<std::string, std::string> arguments;
};