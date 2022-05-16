#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>

#include "Query.h"
#include "QueryParserUtil.h"

// constructors

// default constructor
// for queries that are immediately invalid (eg. no corresponding declarations)
Query::Query() {
	this->isSyntacticallyValid = false;
	this->isSemanticallyValid = false;
}

Query::Query(std::vector<std::string> resultSynonyms, std::vector<ArgumentTypes> resultTypes, std::vector<AttrName> resultAttrNames,
	std::vector<QuerySuchThatClause> suchThatClauses, std::vector<QueryPatternClause> patternClauses, std::vector<QueryWithClause> withClauses,
	bool isBooleanResult) {

	this->isSyntacticallyValid = true;
	this->isSemanticallyValid = true;

	// Check validity of all clauses
	// As long as any one of the clauses is syntatically/semantically invalid, the whole Query is syntatically/semantically invalid
	for (QuerySuchThatClause suchthat : suchThatClauses) {
		if (!suchthat.getSyntacticValidity()) {
			this->isSyntacticallyValid = false;
			this->isSemanticallyValid = false;
		}
		else if (!suchthat.getSemanticValidity()) {
			this->isSemanticallyValid = false;
		}
	}

	for (QueryPatternClause pattern : patternClauses) {
		if (!pattern.getSyntacticValidity()) {
			this->isSyntacticallyValid = false;
			this->isSemanticallyValid = false;
		}
		else if (!pattern.getSemanticValidity()) {
			this->isSemanticallyValid = false;
		}
	}

	for (QueryWithClause with : withClauses) {
		if (!with.getSyntacticValidity()) {
			this->isSyntacticallyValid = false;
			this->isSemanticallyValid = false;
		}
		else if (!with.getSemanticValidity()) {
			this->isSemanticallyValid = false;
		}
	}

	this->resultSynonyms = resultSynonyms;
	this->resultTypes = resultTypes;
	this->resultAttrNames = resultAttrNames;
	this->suchThatClauses = suchThatClauses;
	this->patternClauses = patternClauses;
	this->withClauses = withClauses;
	this->isBooleanResult = isBooleanResult;
}

Query::Query(bool isBooleanResult) {
	this->isSyntacticallyValid = true;
	this->isSemanticallyValid = false;
	this->isBooleanResult = isBooleanResult;
}

// iteration 1 constructors
Query::Query(std::string resultSynonym, ArgumentTypes resultType, QuerySuchThatClause suchThatClause, QueryPatternClause patternClause) {
	this->resultSynonyms.push_back(resultSynonym);
	this->resultTypes.push_back(resultType);
	this->resultAttrNames.push_back(AttrName::NONE);
	this->suchThatClauses.push_back(suchThatClause);
	this->patternClauses.push_back(patternClause);

	// query is only valid when all clauses are also valid
	this->isSyntacticallyValid = suchThatClause.getValidity() && patternClause.getValidity();
	this->isSemanticallyValid = suchThatClause.getValidity() && patternClause.getValidity();
};

Query::Query(std::string resultSynonym, ArgumentTypes resultType) {
	this->resultSynonyms.push_back(resultSynonym);
	this->resultTypes.push_back(resultType);
	this->resultAttrNames.push_back(AttrName::NONE);
	// valid by default
};

Query::Query(std::string resultSynonym, ArgumentTypes resultType, QuerySuchThatClause suchThatClause) {
	this->resultSynonyms.push_back(resultSynonym);
	this->resultTypes.push_back(resultType);
	this->resultAttrNames.push_back(AttrName::NONE);
	this->suchThatClauses.push_back(suchThatClause);

	// query only valid will all clauses are also valid
	this->isSyntacticallyValid = suchThatClause.getValidity();
	this->isSemanticallyValid = suchThatClause.getValidity();
};

Query::Query(std::string resultSynonym, ArgumentTypes resultType, QueryPatternClause patternClause) {
	this->resultSynonyms.push_back(resultSynonym);
	this->resultTypes.push_back(resultType);
	this->resultAttrNames.push_back(AttrName::NONE);
	this->patternClauses.push_back(patternClause);

	// query only valid will all clauses are also valid
	this->isSyntacticallyValid = patternClause.getValidity();
	this->isSemanticallyValid = patternClause.getValidity();
};

// Main API
bool Query::getSyntacticValidity() {
	return this->isSyntacticallyValid;
}

bool Query::getSemanticValidity() {
	return this->isSemanticallyValid;
}

bool Query::getValidity() {
	// only returns TRUE if both syntactically and semantically valid
	return this->isSyntacticallyValid && this->isSemanticallyValid;
}

bool Query::checkBooleanResult() {
	return this->isBooleanResult;
}

std::vector<QuerySuchThatClause> Query::getSuchThatClauses() {
	return this->suchThatClauses;
};

std::vector<QueryPatternClause> Query::getPatternClauses() {
	return this->patternClauses;
};

std::vector<QueryWithClause> Query::getWithClauses() {
	return this->withClauses;
}

std::vector<std::string> Query::getResultSynonyms() {
	return this->resultSynonyms;
}

std::vector<ArgumentTypes> Query::getResultTypes() {
	return this->resultTypes;
}

std::vector<AttrName> Query::getResultAttrNames() {
	return this->resultAttrNames;
}

// for testing
bool Query::compare(Query query) {
	// all invalid queries considered equal
	if (!query.getSyntacticValidity() && !this->getSyntacticValidity()) {
		return true;
	}

	if (!query.getSemanticValidity() && !this->getSemanticValidity()) {
		return true;
	} 
	// compare suchThatClauses
	std::vector<QuerySuchThatClause> otherSuchThat = query.getSuchThatClauses();
	std::vector<QuerySuchThatClause> thisSuchThat = this->getSuchThatClauses();
	if (otherSuchThat.size() != thisSuchThat.size()) {
		return false;
	}
	else {
		for (int i = 0; i < otherSuchThat.size(); i++) {
			if (!(otherSuchThat[i] == thisSuchThat[i])) {
				return false;
			}
		}
	}

	// compare patternClauses
	std::vector<QueryPatternClause> otherPattern = query.getPatternClauses();
	std::vector<QueryPatternClause> thisPattern = query.getPatternClauses();
	if (otherPattern.size() != thisPattern.size()) {
		return false;
	}
	else {
		for (int i = 0; i < otherPattern.size(); i++) {
			if (!(otherPattern[i] == thisPattern[i])) {
				return false;
			}
		}
	}

	return (query.getResultSynonyms() == this->getResultSynonyms())
		&& (query.getResultTypes() == this->getResultTypes());
}

// For debugging
std::string Query::printQuery() {
	std::string result = "\n==========BEGIN QUERY PRINT==========\n";

	if (!this->getSyntacticValidity()) {
		result += "Invalid Query: Syntactic\n";
	}

	if (!this->getSemanticValidity()) {
		result += "Invalid Query: Semantic\n";
		std::string isBool = this->checkBooleanResult() ? "TRUE\n" : "FALSE\n";
		result += "Is Boolean Result: " + isBool;
	}
	else {
		result += "Result Synonyms:";

		std::vector<std::string> resultSynonyms = this->getResultSynonyms();
		std::vector<ArgumentTypes> resultTypes = this->getResultTypes();
		std::vector<AttrName> resultAttrNames = this->getResultAttrNames();
		for (int i = 0; i < resultSynonyms.size(); i++) {
			result += "\n" + resultSynonyms[i] + " " + QueryParserUtil::argumentTypesToString(resultTypes[i]);
			switch (resultAttrNames[i]) {
			case AttrName::PROC_NAME:
				result += ".PROC_NAME";
				break;
			case AttrName::STMT_NUM:
				result += ".STMT_NUM";
				break;
			case AttrName::VALUE:
				result += ".VALUE";
				break;
			case AttrName::VAR_NAME:
				result += ".VAR_NAME";
				break;
			default:
				break;
			}
		}
		result += "\n\n";

		if (this->suchThatClauses.size() > 0) {
			for (QuerySuchThatClause cl : this->suchThatClauses) {
				result += cl.printSuchThatClause();
			}
		}

		if (this->patternClauses.size() > 0) {
			for (QueryPatternClause cl : this->patternClauses) {
				result += cl.printPatternClause();
			}
		}

		if (this->withClauses.size() > 0) {
			for (QueryWithClause cl : this->withClauses) {
				result += cl.printWithClause();
			}
		}
	}

	result += "==========END QUERY PRINT==========\n\n";
	
	return result;
}