#pragma once

#include "AttrName.h"
#include "QueryClause.h"

/*
	QueryWithClause represents a with-clause in a given query.
	Attributes unique to this clause class includes the firstAttrName and secondAttrName, which are of enum type AttrName.
	This represents an attribute type attached to an attrRef. If a ref is not an attrRef (ie. either "IDENT", INTEGER, or synonym), then
	attrName would be stored as NONE.
*/

class QueryWithClause : public QueryClause {
public:
	//constructors
	QueryWithClause(std::string argument1, std::string argument2, ArgumentTypes firstArgumentType, ArgumentTypes secondArgumentType,
		AttrName firstAttrName, AttrName secondAttrName);
	QueryWithClause(); // for syntactically invalid
	QueryWithClause(bool isSemanticallyValid); // for syntactically valid but semantically invalid

	AttrName getFirstAttrName() const;
	AttrName getSecondAttrName() const;
	bool operator==(const QueryWithClause& other) const;
	std::string printWithClause();
	std::string withClauseToString() const;
	size_t getClauseHashValue() const;

private:
	// semantically valid argumentType-attrName pairs:
	// procedure.procName, call.procName, variable.varName, read.varName, print.varName
	// constant.value
	// stmt.stmt#, read.stmt#, print.stmt#, call.stmt#, while.stmt#, if.stmt#, assign.stmt#

	// when attrName is not applicable (eg. INTEGER, IDENT), AttrName will be NONE
	AttrName firstAttrName;
	AttrName secondAttrName;
	std::string clauseString;
	size_t clauseHashValue;
};