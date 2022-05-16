#pragma once

#include <exception>
#include <string>

class ParserException : std::exception {
public:
	// Constructor
	ParserException(std::string codeLine, int errorType);

	// Throw exception
	std::string getExceptionMessage();

private:
	std::string codeLine;
	std::string errorType;
};
