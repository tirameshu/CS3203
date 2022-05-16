#include <iostream>

#include "QuerySuchThatClause.h"
#include "QueryParserUtil.h"

// Constructors
QuerySuchThatClause::QuerySuchThatClause(RelRef relRef, std::string argument1, std::string argument2,
	ArgumentTypes firstArgumentType, ArgumentTypes secondArgumentType) {
	this->isSyntacticallyValid = true;
	this->isSemanticallyValid = true;
	this->relRef = relRef;
	std::pair<std::string, std::string> argPair = std::make_pair(argument1, argument2);
	this->arguments = argPair;
	this->firstArgumentType = firstArgumentType;
	this->secondArgumentType = secondArgumentType;
	clauseString = QueryParserUtil::relRefToString(relRef) + " " + getFirstArgument() + " " + getSecondArgument();
	clauseHashValue = std::hash<std::string>()(clauseString);
};

QuerySuchThatClause::QuerySuchThatClause(bool isSemanticallyValid) {
	this->isSyntacticallyValid = true;
	this->isSemanticallyValid = false;
	this->relRef = RelRef::FOLLOWS;
	std::pair<std::string, std::string> argPair = std::make_pair("", "");
	this->arguments = argPair;
	this->firstArgumentType = ArgumentTypes::ANY;
	this->secondArgumentType = ArgumentTypes::ANY;
}

QuerySuchThatClause::QuerySuchThatClause() {
	this->isSyntacticallyValid = false;
	this->isSemanticallyValid = false;
	this->relRef = RelRef::FOLLOWS;
	std::pair<std::string, std::string> argPair = std::make_pair("", "");
	this->arguments = argPair;
	this->firstArgumentType = ArgumentTypes::ANY;
	this->secondArgumentType = ArgumentTypes::ANY;
}

void QuerySuchThatClause::invalidateSuchThatClause() {
	this->invalidateClause();
}

RelRef QuerySuchThatClause::getRelRef() const {
	return this->relRef;
};

bool QuerySuchThatClause::operator==(const QuerySuchThatClause &other) const {
	return (other.getSyntacticValidity() == getSyntacticValidity()) && (other.getSemanticValidity() == getSemanticValidity())
		&& (other.getArguments() == getArguments()) && (other.getFirstArgumentType() == getFirstArgumentType())
		&& (other.getSecondArgumentType() == getSecondArgumentType()) && (other.getRelRef() == getRelRef());
}

std::string QuerySuchThatClause::printSuchThatClause() const {
	std::string result;
	result += "suchthat-cl RelRef: ";
	if (!this->isSyntacticallyValid) {
		result += "SYNTACTICALLY INVALID\n";
		return result;
	}

	if (!this->isSemanticallyValid) {
		result += "SEMANTICALLY INVALID\n";
		return result;
	}

	result += QueryParserUtil::relRefToString(this->getRelRef());

	result += "\nFirst arg: " + getFirstArgument() + "\n";
	result += "First Arg Type: ";
	result += QueryParserUtil::argumentTypesToString(this->getFirstArgumentType());

	result += "\nSecond arg: " + getSecondArgument() + "\n";
	result += "Second Arg Type: ";
	result += QueryParserUtil::argumentTypesToString(this->getSecondArgumentType());
	result += "\n\n";
	return result;
};

std::string QuerySuchThatClause::suchThatClauseToString() const {
	return clauseString;
}

size_t QuerySuchThatClause::getClauseHashValue() const {
	return clauseHashValue;
}