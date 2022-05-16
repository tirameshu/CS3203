#include "ParserException.h"
#include "ParserExceptionEnum.h"

ParserException::ParserException(std::string codeLine, int errorType) {
	this->codeLine = codeLine;
	switch (errorType) {
	case ParserExceptionEnum::ASSIGN_EXCEPTION:
		this->errorType = "ASSIGN";
		break;
	case ParserExceptionEnum::UNIDENTIFIED_EXCEPTION:
		this->errorType = "UNIDENTIFIED STATEMENT";
		break;
	case ParserExceptionEnum::EMPTY_EXCEPTION:
		this->errorType = "EMPTY";
		break;
	case ParserExceptionEnum::IF_EXCEPTION:
		this->errorType = "IF";
		break;
	case ParserExceptionEnum::PRINT_EXCEPTION:
		this->errorType = "PRINT";
		break;
	case ParserExceptionEnum::PROCEDURE_EXCEPTION:
		this->errorType = "PROCEDURE";
		break;
	case ParserExceptionEnum::READ_EXCEPTION:
		this->errorType = "READ";
		break;
	case ParserExceptionEnum::WHILE_EXCEPTION:
		this->errorType = "WHILE";
		break;
	case ParserExceptionEnum::CALL_EXCEPTION:
		this->errorType = "CALL";
		break;
	case ParserExceptionEnum::CONDITIONAL_EXPRESSION_EXCEPTION:
		this->errorType = "CONDITIONAL EXPRESSION";
		break;
	case ParserExceptionEnum::STANDARD_EXPRESSION_EXCEPTION:
		this->errorType = "STANDARD EXPRESSION";
		break;
	case ParserExceptionEnum::CHILD_STATEMENT_EXCEPTION:
		this->errorType = "CHILD STATEMENT";
		break;
	case ParserExceptionEnum::READER_EXCEPTION:
		this->errorType = "READER EXCEPTION";
		break;
	default:
		this->errorType = "UNKNOWN";
	}
}

// Throw exception
std::string ParserException::getExceptionMessage() {
	return "\n" + this->errorType + " EXCEPTION AT THE CODE: " + this->codeLine + "\n";
}
