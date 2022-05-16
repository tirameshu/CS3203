#include "PrintProcessor.h"
#include <vector>
#include <sstream>
#include <iterator>

#include "ParserException.h"
#include "Statement.h"
#include "StatementTypeEnum.h"
#include "ParserExceptionEnum.h"
#include "LexicalSyntaxCheck.h"

// Passing PKB using pass-by reference to reduce the load on memory
bool PrintProcessor::processPrint(std::string &inputLine, std::string &proc, int &statementNumber) {
	// 0 is keyword, 1 is varname
	std::vector<std::string> entities;
	std::istringstream streamIterator(inputLine);
	std::copy(std::istream_iterator<std::string>(streamIterator),
		std::istream_iterator<std::string>(),
		std::back_inserter(entities));

	// There should only be keyword 'print' and var_name, anything more means invalid statement
	if (entities.size() != 2) {
		throw ParserException("INVALID VARIABLE COUNT FOR PRINT STATEMENT: " + inputLine, ParserExceptionEnum::PRINT_EXCEPTION);
	}

	std::string var_name = entities[1];

	if (var_name.empty()) {
		throw ParserException("VARIABLE CANNOT BE FOUND", ParserExceptionEnum::PRINT_EXCEPTION);
	}

	// Checking var_name validity
	if (!LexicalSyntaxCheck::checkName(var_name)) { // if var_name is not valid name
		throw ParserException("INVALID VAR NAME AT PRINT CASE: " + inputLine, ParserExceptionEnum::PRINT_EXCEPTION);
	}
	
	// Creating unordered set to fit UsesStore API
	std::unordered_set<std::string> listOfVariables;
	listOfVariables.insert(var_name);

	// Creating the statement object
	Statement outputStatement = Statement(StatementTypeEnum::PRINT, statementNumber);

	// Calling PKB methods to add the statements
	PKB::addStatement(proc, outputStatement);
	PKB::addVariable(var_name);
	
	// Calling PKB methods to add to the UsesStore
	if (!listOfVariables.empty()) {
		PKB::addUses(statementNumber, StatementTypeEnum::PRINT, listOfVariables);
	}
	
	std::unordered_set<std::string> setOfUsedVars;
	setOfUsedVars.insert(var_name);

	if (!setOfUsedVars.empty()) {
		PKB::addProcUses(proc, setOfUsedVars);
	}

	return true;
}
