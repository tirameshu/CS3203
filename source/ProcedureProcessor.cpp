#include <sstream>
#include <iterator>
#include <vector>

#include "ProcedureProcessor.h"
#include "ParserException.h"
#include "ParserExceptionEnum.h"
#include "LexicalSyntaxCheck.h"

// INCOMPLETE
std::string ProcedureProcessor::processProcedure(std::string &inputLine) {
	// Split into tokens
	std::vector<std::string> procedureEntities;
	std::istringstream streamIterator(inputLine);
	std::copy(std::istream_iterator<std::string>(streamIterator),
		std::istream_iterator<std::string>(),
		std::back_inserter(procedureEntities));

	if (procedureEntities.empty()) {
		throw ParserException("NO PROCEDURE DETECTED", ParserExceptionEnum::PROCEDURE_EXCEPTION);
	}

	// Trivial validation
	// If the procedure keyword is wrong
	if (procedureEntities[0].compare("procedure") != 0) {
		throw ParserException("WRONG PROCEDURE KEYWORD: " + procedureEntities[0], ParserExceptionEnum::PROCEDURE_EXCEPTION);
	}
	else if (procedureEntities.size() != 2) { // Should only be procedure keyword and procedure name
		throw ParserException("INVALID PROCEDURE FORMAT: " + inputLine, ParserExceptionEnum::PROCEDURE_EXCEPTION);
	}

	// Check for procedure name
	if (!LexicalSyntaxCheck::checkName(procedureEntities[1])) {
		throw ParserException("INVALID PROCEDURE NAME: " + procedureEntities[1], ParserExceptionEnum::PROCEDURE_EXCEPTION);
	}

	PKB::addProcedure(procedureEntities[1]);

	// Return the procedure name
	return procedureEntities[1];
}
