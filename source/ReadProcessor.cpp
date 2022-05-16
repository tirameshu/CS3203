#include "ReadProcessor.h"
#include <vector>
#include <sstream>
#include <iterator>

#include "ParserException.h"
#include "ParserExceptionEnum.h"
#include "StatementTypeEnum.h"
#include "LexicalSyntaxCheck.h"

bool ReadProcessor::processRead(std::string &inputLine, std::string &proc, int &statementNumber) {
	// 0 is keyword, 1 is varname
	std::vector<std::string> entities;
	std::istringstream streamIterator(inputLine);
	std::copy(std::istream_iterator<std::string>(streamIterator),
		std::istream_iterator<std::string>(),
		std::back_inserter(entities));

	// There should only be keyword 'read' and var_name, anything more means invalid statement
	if (entities.size() != 2) {
		throw ParserException("INVALID PRINT FORMAT: " + inputLine, ParserExceptionEnum::READ_EXCEPTION);
	}

	std::string var_name = entities[1];

	if (var_name.empty()) {
		throw ParserException("VARIABLE IS EMPTY", ParserExceptionEnum::READ_EXCEPTION);
	}

	if (!LexicalSyntaxCheck::checkName(var_name)) { // if var_name is not valid
		throw ParserException("INVALID VAR NAME " + var_name + " AT: " + inputLine, ParserExceptionEnum::READ_EXCEPTION);
	}

	// Creating the statement object
	Statement outputStatement = Statement(StatementTypeEnum::READ, statementNumber);

	// Calling PKB methods to add the statements
	PKB::addStatement(proc, outputStatement);
	PKB::addVariable(var_name);

	// Calling PKB methods to add to the UsesStore
	PKB::addModifies(statementNumber, StatementTypeEnum::READ, var_name);
	std::unordered_set<std::string> setOfModifiedVars;
	setOfModifiedVars.insert(var_name);

	if (!setOfModifiedVars.empty()) {
		PKB::addModifiedVarToProc(proc, setOfModifiedVars);
	}

	return true;
}
