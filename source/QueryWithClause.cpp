#include "QueryWithClause.h"
#include "QueryParserUtil.h"

QueryWithClause::QueryWithClause(std::string argument1, std::string argument2, ArgumentTypes firstArgumentType, ArgumentTypes secondArgumentType,
	AttrName firstAttrName, AttrName secondAttrName) {
	std::pair<std::string, std::string> argPair = std::make_pair(argument1, argument2);
	this->isSemanticallyValid = true;
	this->isSyntacticallyValid = true;
	this->arguments = argPair;
	this->firstArgumentType = firstArgumentType;
	this->secondArgumentType = secondArgumentType;
	this->firstAttrName = firstAttrName;
	this->secondAttrName = secondAttrName;
	clauseString = getFirstArgument() + " " + QueryParserUtil::attributeToString(firstAttrName) + " " + getSecondArgument() + " " + QueryParserUtil::attributeToString(secondAttrName);
	clauseHashValue = std::hash<std::string>()(clauseString);
}

QueryWithClause::QueryWithClause(bool isSemanticallyValid) {
	this->isSyntacticallyValid = true;
	this->isSemanticallyValid = false;

	// arbitrary initialisation
	std::pair<std::string, std::string> argPair = std::make_pair("", "");
	this->arguments = argPair;
	this->firstArgumentType = ArgumentTypes::ANY;
	this->secondArgumentType = ArgumentTypes::ANY;
	this->firstAttrName = AttrName::NONE;
	this->secondAttrName = AttrName::NONE;
}

QueryWithClause::QueryWithClause() {
	this->isSyntacticallyValid = false;
	this->isSemanticallyValid = false;

	// arbitrary initialisation
	std::pair<std::string, std::string> argPair = std::make_pair("", "");
	this->arguments = argPair;
	this->firstArgumentType = ArgumentTypes::ANY;
	this->secondArgumentType = ArgumentTypes::ANY;
	this->firstAttrName = AttrName::NONE;
	this->secondAttrName = AttrName::NONE;
}

AttrName QueryWithClause::getFirstAttrName() const {
	return this->firstAttrName;
}

AttrName QueryWithClause::getSecondAttrName() const {
	return this->secondAttrName;
}

bool QueryWithClause::operator==(const QueryWithClause& other) const {
	return (other.getSyntacticValidity() == getSyntacticValidity()) && (other.getSemanticValidity() == getSemanticValidity())
		&& (other.getArguments() == getArguments()) && (other.getFirstArgumentType() == getFirstArgumentType())
		&& (other.getSecondArgumentType() == getSecondArgumentType() && (other.getFirstAttrName() == getFirstAttrName())
		&& (other.getSecondAttrName() == getSecondAttrName()));
}

std::string QueryWithClause::printWithClause() {
	std::string result;
	result += "With clause:\n";
	if (!this->getSyntacticValidity()) {
		result += "Syntactically invalid\n";
		return result;
	}
	else if (!this->getSemanticValidity()) {
		result += "Semantically invalid\n";
	}
	result += "First ref: " + getFirstArgument() + ", Type: " + QueryParserUtil::argumentTypesToString(this->getFirstArgumentType())
		+ ", Attr: " + QueryParserUtil::attributeToString(this->getFirstAttrName());
	result += "\nSecond ref: " + getSecondArgument() + ", Type: " + QueryParserUtil::argumentTypesToString(this->getSecondArgumentType())
		+ ", Attr: " + QueryParserUtil::attributeToString(this->getSecondAttrName());
	result += "\n";
	return result;
}

std::string QueryWithClause::withClauseToString() const {
	return clauseString;
}

size_t QueryWithClause::getClauseHashValue() const {
	return clauseHashValue;
}