#include <vector>
#include <sstream>
#include <iterator>

#include "CallProcessor.h"
#include "ParserException.h"
#include "ParserExceptionEnum.h"
#include "LexicalSyntaxCheck.h"

std::string CallProcessor::processCall(std::string& inputLine, std::string& proc, int& statementNumber) {
	// Split the input by whitespaces
	std::vector<std::string> entities;
	std::istringstream streamIterator(inputLine);
	std::copy(std::istream_iterator<std::string>(streamIterator),
		std::istream_iterator<std::string>(),
		std::back_inserter(entities));

	// Check that the size is only 2, keyword and parameter
	if (entities.size() != 2) {
		throw ParserException("TOO MANY PARAMETERS: " + inputLine, ParserExceptionEnum::CALL_EXCEPTION);
	}

	// Checking that the procedure name parameter is valid
	if (!LexicalSyntaxCheck::checkName(entities[1])) {
		throw ParserException("INVALID PROCEDURE NAME PARAMETER: " + inputLine, ParserExceptionEnum::CALL_EXCEPTION);
	}

	/*=== STORING TO PKB ===*/
	Statement outputStatement = Statement(StatementTypeEnum::CALL, statementNumber);
	PKB::addStatement(proc, outputStatement);

	PKB::addCall(proc, entities[1], statementNumber);

	return entities[1];
}
