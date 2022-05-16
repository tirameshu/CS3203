#include "ChildProcessor.h"
#include "StatementTypeEnum.h"
#include "ParserException.h"
#include "ParserExceptionEnum.h"

bool ChildProcessor::processChild(int &parentStatementNumber, int &parentStatementType, int &childStatementNumber) {

	if (parentStatementType == StatementTypeEnum::IF || parentStatementType == StatementTypeEnum::ELSE) {
		PKB::addIfContainerStmtList(parentStatementNumber, childStatementNumber);
	}
	else if (parentStatementType == StatementTypeEnum::WHILE) {
		PKB::addWhileContainerStmtList(parentStatementNumber, childStatementNumber);
	}
	else {
		throw ParserException("UNSUITABLE PARENT STATEMENT NUMBER " + std::to_string(parentStatementNumber)
			+ " FOR THE STATEMENT NUMBER " + std::to_string(childStatementNumber), ParserExceptionEnum::CHILD_STATEMENT_EXCEPTION);
	}

	return true;
}
