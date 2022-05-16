#include <stdio.h>
#include <iostream>
#include <string>
#include <utility>

#include "QueryPatternClause.h"
#include "QueryParserUtil.h"

QueryPatternClause::QueryPatternClause(std::string patternName, std::string argument1, std::string argument2,
	ArgumentTypes patternType, ArgumentTypes firstArgumentType, ArgumentTypes secondArgumentType) {
	std::pair<std::string, std::string> argPair = std::make_pair(argument1, argument2);
	this->arguments = argPair;
	this->patternName = patternName;
	this->firstArgumentType = firstArgumentType;
	this->secondArgumentType = secondArgumentType;
	this->patternType = patternType;
	this->isSyntacticallyValid = true;
	this->isSemanticallyValid = true;
	clauseString = patternName + " " + getFirstArgument() + " " + getSecondArgument();
	clauseHashValue = std::hash<std::string>()(clauseString);
};

QueryPatternClause::QueryPatternClause(std::string patternName, std::string argument1, std::string argument2,
	ArgumentTypes patternType, ArgumentTypes firstArgumentType, ArgumentTypes secondArgumentType, bool isExact) {
	std::pair<std::string, std::string> argPair = std::make_pair(argument1, argument2);
	this->arguments = argPair;
	this->patternName = patternName;
	this->firstArgumentType = firstArgumentType;
	this->secondArgumentType = secondArgumentType;
	this->patternType = patternType;
	this->isExact = isExact;
	this->isSyntacticallyValid = true;
	this->isSemanticallyValid = true;
	std::string exact = isExact ? "T" : "F";
	clauseString = patternName + " " + getFirstArgument() + " " + getSecondArgument() + " " + exact;
	clauseHashValue = std::hash<std::string>()(clauseString);
};

QueryPatternClause::QueryPatternClause(bool isSemanticallyValid) {
	this->isSyntacticallyValid = true;
	this->isSemanticallyValid = false;
	std::pair<std::string, std::string> argPair = std::make_pair("", "");
	this->arguments = argPair;
	this->patternName = "";
	this->firstArgumentType = ArgumentTypes::ANY;
	this->secondArgumentType = ArgumentTypes::ANY;
	this->patternType = ArgumentTypes::ANY;
}

QueryPatternClause::QueryPatternClause() {
	this->isSyntacticallyValid = false;
	this->isSemanticallyValid = false;
	std::pair<std::string, std::string> argPair = std::make_pair("", "");
	this->arguments = argPair;
	this->patternName = "";
	this->firstArgumentType = ArgumentTypes::ANY;
	this->secondArgumentType = ArgumentTypes::ANY;
	this->patternType = ArgumentTypes::ANY;
}

std::string QueryPatternClause::getPatternName() const {
	return this->patternName;
};

ArgumentTypes QueryPatternClause::getPatternType() const {
	return this->patternType;
};

bool  QueryPatternClause::checkExact() const {
	return this->isExact;
}

void QueryPatternClause::invalidatePatternClause() {
	this->invalidateClause();
};

void QueryPatternClause::setExact(bool isExact) {
	this->isExact = isExact;
}

bool QueryPatternClause::operator==(const QueryPatternClause &other) const {
	return (other.getValidity() == getValidity()) && (other.getArguments() == getArguments())
		&& (other.getFirstArgumentType() == getFirstArgumentType()) && (other.getSecondArgumentType() == getSecondArgumentType())
		&& (other.getPatternName().compare(getPatternName()) == 0) && (other.getPatternType() == getPatternType()
		&& (other.checkExact() == checkExact()));
}

std::string QueryPatternClause::printPatternClause() const {
	std::string result;
	result += "Pattern name: " + this->getPatternName() + "<end\n";
	result += "Pattern type: " + QueryParserUtil::argumentTypesToString(this->getPatternType());

	result += "\nFirst arg: " + getFirstArgument() + "<end\n";
	result += "First Arg Type: " + QueryParserUtil::argumentTypesToString(this->getFirstArgumentType());
	
	result += "\nSecond arg: " + getSecondArgument() + "<end\n";
	result += "Second Arg Type: " + QueryParserUtil::argumentTypesToString(this->getSecondArgumentType());
	
	std::string exact = checkExact() ? "TRUE" : "FALSE";
	result += "\nIs Exact: " + exact;

	result += "\n\n";
	return result;
};

std::string QueryPatternClause::patternClauseToString() const {
	return clauseString;
}

size_t QueryPatternClause::getClauseHashValue() const {
	return clauseHashValue;
}