#pragma once

#include "RelRef.h"
#include "QueryClause.h"

class QuerySuchThatClause : public QueryClause {
public:
	//constructor
	QuerySuchThatClause(RelRef relRef, std::string argument1, std::string argument2,
		ArgumentTypes firstArgumentType, ArgumentTypes secondArgumentType);
	QuerySuchThatClause(); // for syntactically invalid clauses
	QuerySuchThatClause(bool isSemanticallyValid); // for semantically invalid but syntactically valid clauses

	// public API to parse the query from user
	RelRef getRelRef() const;
	std::string printSuchThatClause() const;
	std::string suchThatClauseToString() const;
	size_t getClauseHashValue() const;
	bool operator==(const QuerySuchThatClause &other) const;
	void invalidateSuchThatClause();

private:
	RelRef relRef;
	std::string clauseString;
	size_t clauseHashValue;
};