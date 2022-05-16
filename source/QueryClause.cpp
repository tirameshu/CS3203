#include "QueryClause.h"

QueryClause::QueryClause(std::string argument1, std::string argument2, ArgumentTypes firstArgumentType, ArgumentTypes secondArgumentType) {
	std::pair<std::string, std::string> argPair = std::make_pair(argument1, argument2);
	this->arguments = argPair;
	this->firstArgumentType = firstArgumentType;
	this->secondArgumentType = secondArgumentType;
};

QueryClause::QueryClause() {
	this->isSyntacticallyValid = false;
	this->isSemanticallyValid = false;
	std::pair<std::string, std::string> argPair = std::make_pair("", "");
	this->arguments = argPair;
	this->firstArgumentType = ArgumentTypes::ANY;
	this->secondArgumentType = ArgumentTypes::ANY;
}


void QueryClause::invalidateClause() {
	this->isSyntacticallyValid = false;
	this->isSemanticallyValid = false;
}

bool QueryClause::getValidity() const {
	return this->isSyntacticallyValid && this->isSemanticallyValid;
};

bool QueryClause::getSyntacticValidity() const {
	return this->isSyntacticallyValid;
}

bool QueryClause::getSemanticValidity() const {
	return this->isSemanticallyValid;
}

std::pair<std::string, std::string> QueryClause::getArguments() const {
	return this->arguments;
};

std::string QueryClause::getFirstArgument() const {
	return this->arguments.first;
};

std::string QueryClause::getSecondArgument() const {
	return this->arguments.second;
};

ArgumentTypes QueryClause::getFirstArgumentType() const {
	return this->firstArgumentType;
};

ArgumentTypes QueryClause::getSecondArgumentType() const {
	return this->secondArgumentType;
};

bool QueryClause::operator==(const QueryClause& other) {
	return (other.getValidity() == this->getValidity()) && (other.getArguments() == this->getArguments())
		&& (other.getFirstArgumentType() == this->getFirstArgumentType()) && (other.getSecondArgumentType() == this->getSecondArgumentType());
}
