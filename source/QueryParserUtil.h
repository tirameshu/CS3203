#pragma once
#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>

#include "ArgumentTypes.h"
#include "AttrName.h"
#include "RelRef.h"

/*
	QueryParserUtil comprises of utility methods used by the QueryParser.
*/

class QueryParserUtil {
public:
	static std::string ltrim(std::string str);
	static std::string rtrim(std::string str);
	static std::string trim(std::string str);
	static std::string getFirstWord(std::string& s, std::string delimiter);
	static AttrName stringToAttribute(std::string attrString);
	static std::string attributeToString(AttrName attr);
	static ArgumentTypes stringToArgumentTypes(std::string token);
	static std::string argumentTypesToString(ArgumentTypes argType);
	static RelRef stringToRelRef(std::string relation);
	static std::string relRefToString(RelRef relref);

	static std::vector<std::string> tokenizeDeclaration(std::string declaration);
	static std::vector<std::string> tokenizeSelection(std::string selection); // tokenize a line in a query

	// checks if a name has already been declared, if it has, it returns the corresponding ArgumentType
	static std::pair<bool, ArgumentTypes> checkNameExists(std::vector<std::string> synonymNames, std::vector<ArgumentTypes> synonymTypes, std::string name);
	// validates synonym.attrRef pairings
	static bool validateAttrType(ArgumentTypes synonymType, AttrName attr);
	// checks if a given ArgumentType is a synonym
	static bool isSynonym(ArgumentTypes argType);
};