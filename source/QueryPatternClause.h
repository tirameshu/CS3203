#pragma once

#include "QueryClause.h"

/*
	QueryPatternClause represents a pattern-clause in a given query. Besides common attributes inherited from the
	Query Clause base class, this clause class contains the attributes patternName and patternType to store relevant information
	about the pattern-clause, as well as a Boolean isExact to indicate whether the expression in the second argument of a
	pattern-clause is exact or non-exact (distinguised in a query string by the presence of underscores).
*/

class QueryPatternClause : public QueryClause {
public:
	//constructors
	QueryPatternClause(std::string patternName, std::string argument1, std::string argument2,
		ArgumentTypes patternType, ArgumentTypes firstArgumentType, ArgumentTypes secondArgumentType);
	QueryPatternClause(std::string patternName, std::string argument1, std::string argument2,
		ArgumentTypes patternType, ArgumentTypes firstArgumentType, ArgumentTypes secondArgumentType, bool isExact);
	QueryPatternClause(); // for syntactically invalid pattern
	QueryPatternClause(bool isSemanticallyValid); // for syntactically valid but semantically invalid pattern

	//API to access attributes
	std::string getPatternName() const;
	ArgumentTypes getPatternType() const;
	std::string printPatternClause() const;
	std::string patternClauseToString() const;
	size_t getClauseHashValue() const;
	bool checkExact() const;
	bool operator==(const QueryPatternClause &other) const;
	void invalidatePatternClause();
	void setExact(bool isExact);

private:
	bool isExact = false;
	std::string patternName;
	ArgumentTypes patternType;
	std::string clauseString;
	size_t clauseHashValue;
};