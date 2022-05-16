#include "QueryParserUtil.h"

// trim from start
std::string QueryParserUtil::ltrim(std::string s) {
	if (s.size() == 0) {
		return "";
	}

	int start = 0;
	int curr = 0;
	while (std::isspace(s[curr])) {
		curr++;
	}
	start = curr;


	return s.substr(start);
}

// trim from end (in place)
std::string QueryParserUtil::rtrim(std::string s) {
	if (s.size() == 0) {
		return "";
	}

	int end = s.size() - 1;
	int curr = s.size() - 1;
	while (std::isspace(s[curr])) {
		curr--;
	}
	end = curr;

	return s.substr(0, end + 1);
}

// trim from both ends
std::string QueryParserUtil::trim(std::string s) {
    return rtrim(ltrim(s));
}

std::string QueryParserUtil::getFirstWord(std::string& s, std::string delimiter) {
    size_t space_pos = s.find(delimiter);
    if (space_pos != std::string::npos) {
        return s.substr(0, space_pos);
    }
    return s;
}

AttrName QueryParserUtil::stringToAttribute(std::string attrString) {
	attrString = QueryParserUtil::trim(attrString);
	if (attrString.compare("procName") == 0) {
		return AttrName::PROC_NAME;
	}
	else if (attrString.compare("varName") == 0) {
		return AttrName::VAR_NAME;
	}
	else if (attrString.compare("value") == 0) {
		return AttrName::VALUE;
	}
	else if (attrString.compare("stmt#") == 0) {
		return AttrName::STMT_NUM;
	}
	else {
		std::cout << "Invalid AttrName conversion: Unrecognised attrName\n";
		return AttrName::NONE;
	}
}

std::pair<bool, ArgumentTypes> QueryParserUtil::checkNameExists(std::vector<std::string> synonymNames, std::vector<ArgumentTypes> synonymTypes,
	std::string name) {
	auto namePos = std::find(synonymNames.begin(), synonymNames.end(), name);
	bool doesExist = namePos != synonymNames.end();

	if (doesExist) {
		int typeIndex = namePos - synonymNames.begin();
		ArgumentTypes resultType = synonymTypes[typeIndex];
		return std::pair<bool, ArgumentTypes>{ true, resultType };
	}
	else {
		// if synonym does not already exist, just return a filler ArgumentType
		return std::pair<bool, ArgumentTypes>{ false, ArgumentTypes::ANY };
	}
}

bool QueryParserUtil::validateAttrType(ArgumentTypes synonymType, AttrName attr) {
	switch (attr) {
	case AttrName::PROC_NAME:
		return (synonymType == ArgumentTypes::PROCEDURE || synonymType == ArgumentTypes::CALL);
	case AttrName::VAR_NAME:
		return (synonymType == ArgumentTypes::VARIABLE || synonymType == ArgumentTypes::READ || synonymType == ArgumentTypes::PRINT);
	case AttrName::VALUE:
		return synonymType == ArgumentTypes::CONSTANT;
	case AttrName::STMT_NUM:
		return (synonymType == ArgumentTypes::STMT || synonymType == ArgumentTypes::READ ||
			synonymType == ArgumentTypes::PRINT || synonymType == ArgumentTypes::CALL || synonymType == ArgumentTypes::WHILE ||
			synonymType == ArgumentTypes::IF || synonymType == ArgumentTypes::ASSIGN);
	}
}

bool QueryParserUtil::isSynonym(ArgumentTypes argType) {
	return (argType == ArgumentTypes::STMT || argType == ArgumentTypes::READ || argType == ArgumentTypes::PRINT ||
		argType == ArgumentTypes::WHILE || argType == ArgumentTypes::IF || argType == ArgumentTypes::ASSIGN ||
		argType == ArgumentTypes::VARIABLE || argType == ArgumentTypes::CONSTANT || argType == ArgumentTypes::PROCEDURE ||
		argType == ArgumentTypes::CALL || argType == ArgumentTypes::PROG_LINE);
}

std::string QueryParserUtil::argumentTypesToString(ArgumentTypes argType) {
	switch (argType) {
	case ArgumentTypes::STMT:
		return "STMT";
	case ArgumentTypes::PRINT:
		return "PRINT";
	case ArgumentTypes::READ:
		return "READ";
	case ArgumentTypes::WHILE:
		return "WHILE";
	case ArgumentTypes::IF:
		return "IF";
	case ArgumentTypes::ASSIGN:
		return "ASSIGN";
	case ArgumentTypes::CALL:
		return "CALL";
	case ArgumentTypes::VARIABLE:
		return "VARIABLE";
	case ArgumentTypes::PROG_LINE:
		return "PROG_LINE";
	case ArgumentTypes::CONSTANT:
		return "CONSTANT";
	case ArgumentTypes::PROCEDURE:
		return "PROCEDURE";
	case ArgumentTypes::INT:
		return "INT";
	case ArgumentTypes::STRING:
		return "STRING";
	case ArgumentTypes::BOOLEAN:
		return "BOOLEAN";
	case ArgumentTypes::ANY:
		return "ANY";
	default:
		return "NONE";
	}
}

ArgumentTypes QueryParserUtil::stringToArgumentTypes(std::string token) {
	if (token.compare("stmt") == 0) {
		return ArgumentTypes::STMT;
	}
	else if (token.compare("read") == 0) {
		return ArgumentTypes::READ;
	}
	else if (token.compare("print") == 0) {
		return ArgumentTypes::PRINT;
	}
	else if (token.compare("while") == 0) {
		return ArgumentTypes::WHILE;
	}
	else if (token.compare("if") == 0) {
		return ArgumentTypes::IF;
	}
	else if (token.compare("assign") == 0) {
		return ArgumentTypes::ASSIGN;
	}
	else if (token.compare("variable") == 0) {
		return ArgumentTypes::VARIABLE;
	}
	else if (token.compare("constant") == 0) {
		return ArgumentTypes::CONSTANT;
	}
	else if (token.compare("procedure") == 0) {
		return ArgumentTypes::PROCEDURE;
	}
	else if (token.compare("call") == 0) {
		return ArgumentTypes::CALL;
	}
	else if (token.compare("prog_line") == 0) {
		return ArgumentTypes::PROG_LINE;
	}
	else {
		return ArgumentTypes::NONE;
	}
}

std::string  QueryParserUtil::attributeToString(AttrName attr) {
	switch (attr) {
	case AttrName::PROC_NAME:
		return "PROC_NAME";
	case AttrName::STMT_NUM:
		return "STMT_NUM";
	case AttrName::VALUE:
		return "VALUE";
	case AttrName::VAR_NAME:
		return "VAR_NAME";
	default:
		return "NONE";
	}
}

RelRef QueryParserUtil::stringToRelRef(std::string relation) {
	if (relation.compare("Modifies") == 0) {
		return RelRef::MODIFIES_S;
	}
	else if (relation.compare("Uses") == 0) {
		return RelRef::USES_S;
	}
	else if (relation.compare("Parent") == 0) {
		return RelRef::PARENT;
	}
	else if (relation.compare("Parent*") == 0) {
		return RelRef::PARENT_T;
	}
	else if (relation.compare("Follows") == 0) {
		return RelRef::FOLLOWS;
	}
	else if (relation.compare("Follows*") == 0) {
		return RelRef::FOLLOWS_T;
	}
	else if (relation.compare("Calls") == 0) {
		return RelRef::CALLS;
	}
	else if (relation.compare("Calls*") == 0) {
		return RelRef::CALLS_T;
	}
	else if (relation.compare("Next") == 0) {
		return RelRef::NEXT;
	}
	else if (relation.compare("Next*") == 0) {
		return RelRef::NEXT_T;
	}
	else if (relation.compare("NextBip") == 0) {
		return RelRef::NEXT_BIP;
	}
	else if (relation.compare("NextBip*") == 0) {
		return RelRef::NEXT_BIP_T;
	}
	else if (relation.compare("Affects") == 0) {
		return RelRef::AFFECTS;
	}
	else if (relation.compare("Affects*") == 0) {
		return RelRef::AFFECTS_T;
	}
	else if (relation.compare("AffectsBip") == 0) {
		return RelRef::AFFECTS_BIP;
	}
	else if (relation.compare("AffectsBip*") == 0) {
		return RelRef::AFFECTS_BIP_T;
	}
	else {
		std::cout << relation << std::endl; // debug print
		std::cout << "Invalid syntax: Unrecognised RelRef\n";
		return RelRef::NONE;
	}
}

std::string QueryParserUtil::relRefToString(RelRef relref) {
	switch (relref) {
	case RelRef::FOLLOWS:
		return "FOLLOWS";
	case RelRef::FOLLOWS_T:
		return "FOLLOWS_T";
	case RelRef::MODIFIES_S:
		return "MODIFIES_S";
	case RelRef::MODIFIES_P:
		return "MODIFIES_P";
	case RelRef::PARENT:
		return "PARENT";
	case RelRef::PARENT_T:
		return "PARENT_T";
	case RelRef::USES_S:
		return "USES_S";
	case RelRef::USES_P:
		return "USES_P";
	case RelRef::AFFECTS:
		return "AFFECTS";
	case RelRef::AFFECTS_T:
		return "AFFECTS_T";
	case RelRef::AFFECTS_BIP:
		return "AFFECTS_BIP";
	case RelRef::AFFECTS_BIP_T:
		return "AFFECTS_BIP_T";
	case RelRef::NEXT:
		return "NEXT";
	case RelRef::NEXT_T:
		return "NEXT_T";
	case RelRef::NEXT_BIP:
		return "NEXT_BIP";
	case RelRef::NEXT_BIP_T:
		return "NEXT_BIP_T";
	case RelRef::CALLS:
		return "CALLS";
	case RelRef::CALLS_T:
		return "CALLS_T";
	case RelRef::NONE:
		return "NONE";
	}
}

std::vector<std::string> QueryParserUtil::tokenizeDeclaration(std::string declaration) {
	/*=== Initialization ===*/
	std::vector<std::string> outputTokens;
	std::string token; // Current token

	// Useful boolean to know if the recursion is currently building a variable/constant
	bool isBuilding = false;

	/*=== TRIVIAL CASE EMPTY EXPRESSION ===*/
	if (declaration.empty()) {
		return outputTokens;
	}

	/*=== Iterating through the expression ===*/
	for (std::string::size_type i = 0; i < declaration.size(); i++) {

		// If encounter an alphanumeric, add the token into the built string
		if (std::isalnum(declaration[i])) {

			if (!isBuilding) {
				if (!token.empty()) {
					outputTokens.push_back(token);
					token.clear();
					isBuilding = false;
				}
			}

			token += declaration[i];
			isBuilding = true;
		}

		// If encounter whitespace, cut the string
		else {

			if (std::isspace(declaration[i])) {

				if (!token.empty()) {
					outputTokens.push_back(token);
					token.clear();
					isBuilding = false;
				}
			}

			else if (token.compare("prog") == 0 && declaration[i] == '_'
				&& i + 5 < declaration.size() && declaration.substr(i, 5).compare("_line") == 0) {
				token += "_line";
				i += 4;
				continue;
			}

			else {

				if (isBuilding) {
					outputTokens.push_back(token);
					token.clear();
					isBuilding = false;
				}

				isBuilding = false;
				token += declaration[i];
			}
		}
	}

	if (!token.empty()) {
		outputTokens.push_back(token);
		token.clear();
	}

	return outputTokens;
}

// select-cl tokenizer also tokenizes contiguous whitespaces
std::vector<std::string> QueryParserUtil::tokenizeSelection(std::string selection) {
	/*=== Initialization ===*/
	std::vector<std::string> outputTokens;
	std::string token; // Current token

	// Useful boolean to know if the recursion is currently building a variable/constant
	bool isBuilding = false;
	bool isExpression = false;

	// Boolean to flag that last token is "such"
	bool isSuch = false;

	/*=== TRIVIAL CASE EMPTY EXPRESSION ===*/
	if (selection.empty()) {
		return outputTokens;
	}

	/*=== Iterating through the expression ===*/
	for (std::string::size_type i = 0; i < selection.size(); i++) {
		if (isSuch) {
			if (i + 5 < selection.size() && selection.substr(i, 5).compare(" that") == 0) {
				token += " that";
				i += 4;
				continue;
			}
			else {
				isSuch = false;
			}
		}

		// If encounter an alphanumeric, add the token into the built string
		if (std::isalnum(selection[i])) {

			if (!isBuilding) {
				isSuch = false;
				if (!token.empty()) {
					outputTokens.push_back(token);
					token.clear();
					isBuilding = false;
				}
			}

			token += selection[i];
			isBuilding = true;

			if (token.compare("such") == 0) {
				isSuch = true;
			}
		}

		// If encounter whitespace, cut the string
		else {

			if (isExpression && selection[i] != '"') {
				isSuch = false;
				isExpression = true;
				token += selection[i];
				isBuilding = true;
				continue;
			}

			if (selection[i] == '(') {

				isSuch = false;
				if (!token.empty()) {
					outputTokens.push_back(token);
					token.clear();
					isBuilding = false;
				}

				outputTokens.push_back("(");
			}

			else if (selection[i] == ')') {
				isSuch = false;
				if (!token.empty()) {
					outputTokens.push_back(token);
					token.clear();
					isBuilding = false;
				}

				outputTokens.push_back(")");
			}

			else if (selection[i] == ',') {

				isSuch = false;
				if (!token.empty()) {
					outputTokens.push_back(token);
					token.clear();
					isBuilding = false;
				}

				outputTokens.push_back(",");
			}

			else if (selection[i] == '"') {
				isSuch = false;

				if (isExpression) {
					// end expression
					token += selection[i];
					isBuilding = false;
					outputTokens.push_back(token);
					token.clear();
					isExpression = false;
				}
				else {
					// start expression
					if (!token.empty()) {
						outputTokens.push_back(token);
						token.clear();
					}

					isExpression = true;
					token += selection[i];
					isBuilding = true;
				}
			}

			else if (selection[i] == '#') {
				if (token.compare("stmt") == 0) {
					token += selection[i];
					isBuilding = true;
				}
				else {
					if (!token.empty()) {
						outputTokens.push_back(token);
						token.clear();
					}
					outputTokens.push_back("");
				}
			}

			else if (std::isspace(selection[i])) {

				if (isExpression) {
					token += selection[i];
				}
				else {
					if (!token.empty()) {
						outputTokens.push_back(token);
						token.clear();
						isBuilding = false;
					}
				}
			}

			else if (selection[i] == '*') {
				isSuch = false;
				if (token.compare("Parent") == 0 || token.compare("Follows") == 0 || token.compare("NextBip") == 0 ||
					token.compare("Next") == 0 || token.compare("Affects") == 0 || token.compare("AffectsBip") == 0 || token.compare("Calls") == 0) {
					token += selection[i];
				}
				else {
					if (!token.empty()) {
						outputTokens.push_back(token);
						token.clear();
						isBuilding = false;
					}

					outputTokens.push_back("*");
				}
			}

			else {
				isSuch = false;
				if (isBuilding) {
					outputTokens.push_back(token);
					token.clear();
					isBuilding = false;
				}

				isBuilding = false;
				token += selection[i];
			}
		}
	}

	if (!token.empty()) {
		outputTokens.push_back(token);
		token.clear();
	}

	return outputTokens;
}