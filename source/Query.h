#pragma once
#include <stdio.h>
#include <string>
#include <vector>

#include "ArgumentTypes.h"
#include "QueryPatternClause.h"
#include "QuerySuchThatClause.h"
#include "QueryWithClause.h"

/*
	The Query class represents the object which the Query Parser will convert a raw string query into, to be passed to the Query Evaluator.
	It contains all necessary information about a Query, including the synonyms declared, the result clause and vectors of any other clauses
	present in the query. It also consists of Booleans to indicate validity of a Query, as well as whether the result clause is "Select BOOLEAN...".
	All attributes in Query are private and can only be retrieved using getter methods.
*/

class Query {
public:
	//constructors
	Query(std::vector<std::string> resultSynonyms, std::vector<ArgumentTypes> resultTypes, std::vector<AttrName> resultAttrNames,
		std::vector<QuerySuchThatClause> suchThatClauses, std::vector<QueryPatternClause> patternClauses, std::vector<QueryWithClause> withClauses, bool isBooleanResult);
	Query(); // for queries that are immediately syntactically invalid (eg. no corresponding declarations)
	Query(bool isBooleanResult); // for queries that are syntactcally valid but semantically invalid

	// iteration 1 constructors
	Query(std::string resultSynonym, ArgumentTypes resultType);
	Query(std::string resultSynonym, ArgumentTypes resultType, QuerySuchThatClause suchThatClause);
	Query(std::string resultSynonym, ArgumentTypes resultType, QueryPatternClause patternClause);
	Query(std::string resultSynonym, ArgumentTypes resultType, QuerySuchThatClause suchThatClause, QueryPatternClause patternClause);

	// API to access attributes
	bool getValidity();
	bool getSyntacticValidity();
	bool getSemanticValidity();
	bool checkBooleanResult();
	std::vector<QuerySuchThatClause> getSuchThatClauses();
	std::vector<QueryPatternClause> getPatternClauses();
	std::vector<QueryWithClause> getWithClauses();
	std::vector<std::string> getResultSynonyms();
	std::vector<ArgumentTypes> getResultTypes();
	std::vector<AttrName> getResultAttrNames();
	bool compare(Query query); // for testing
	std::string printQuery(); // for debugging

private:
	bool isSyntacticallyValid = true;
	bool isSemanticallyValid = true;
	bool isBooleanResult = false;

	// suchthat-cl and pattern-cl default empty vector
	std::vector<QuerySuchThatClause> suchThatClauses = {};
	std::vector<QueryPatternClause> patternClauses = {};
	std::vector<QueryWithClause> withClauses = {};

	// vectors for result(s)
	std::vector<std::string> resultSynonyms = {};
	std::vector<ArgumentTypes> resultTypes = {};
	std::vector<AttrName> resultAttrNames = {};
};