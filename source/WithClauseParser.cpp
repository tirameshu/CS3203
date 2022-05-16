#include "WithClauseParser.h"
#include "LexicalSyntaxCheck.h"
#include "QueryParserUtil.h"

QueryWithClause WithClauseParser::parseWith(std::string firstRef, std::string secondRef,
	std::vector<std::string> synonymNames, std::vector<ArgumentTypes> synonymTypes) {

	// validate that both return the same type (INT, STRING, SYNONYM)
	ArgumentTypes firstType = checkType(firstRef, synonymNames, synonymTypes);
	ArgumentTypes secondType = checkType(secondRef, synonymNames, synonymTypes);

	if (firstType == ArgumentTypes::NONE || secondType == ArgumentTypes::NONE) {
		std::cout << "Invalid with-cl: Invalid types for attrCompare" << std::endl;
		return QueryWithClause(false);
	}
	else if (!((firstType == ArgumentTypes::PROG_LINE && secondType == ArgumentTypes::INT) ||
		(secondType == ArgumentTypes::PROG_LINE && firstType == ArgumentTypes::INT) ||
		QueryParserUtil::isSynonym(firstType) && QueryParserUtil::isSynonym(secondType) || firstType == secondType)) {
		std::cout << "Invalid with-cl: Mismatching types for attrCompare" << std::endl;
		return QueryWithClause(false);
	}

	// extract synonym type if it is of the form 'synonym . attrName'
	// else argumentType is equivalent to the type already extracted
	ArgumentTypes firstArgumentType {};
	firstArgumentType = isAttrRef(firstRef) ? parseSynonymType(firstRef, synonymNames, synonymTypes) : firstType;
	ArgumentTypes secondArgumentType {};
	secondArgumentType = isAttrRef(secondRef) ? parseSynonymType(secondRef, synonymNames, synonymTypes) : secondType;

	AttrName firstAttrName = parseAttrName(firstRef);
	AttrName secondAttrName = parseAttrName(secondRef);

	// remove quotation marks for STRING
	if (firstArgumentType == ArgumentTypes::STRING) {
		firstRef = firstRef.substr(1, firstRef.size() - 2);
	}
	if (secondArgumentType == ArgumentTypes::STRING) {
		secondRef = secondRef.substr(1, secondRef.size() - 2);
	}

	// trim arguments
	std::size_t dotPos = firstRef.find(".");
	if (dotPos != std::string::npos) {
		std::string synonym = firstRef.substr(0, dotPos);
		firstRef = QueryParserUtil::trim(synonym);
	}
	dotPos = secondRef.find(".");
	if (dotPos != std::string::npos) {
		std::string synonym = secondRef.substr(0, dotPos);
		secondRef = QueryParserUtil::trim(synonym);
	}

	return QueryWithClause(firstRef, secondRef, firstArgumentType, secondArgumentType, firstAttrName, secondAttrName);

}

ArgumentTypes WithClauseParser::checkType(std::string ref, std::vector<std::string> synonymNames, std::vector<ArgumentTypes> synonymTypes) {
	std::size_t dotPos = ref.find(".");
	if (dotPos != std::string::npos) {
		// attrRef
		std::string synonym = ref.substr(0, dotPos);
		std::string attr = ref.substr(dotPos + 1);
		synonym = QueryParserUtil::trim(synonym);
		attr = QueryParserUtil::trim(attr);
		
		// get ArgumentTypes of synonym
		
		std::pair<bool, ArgumentTypes> checkRef = QueryParserUtil::checkNameExists(synonymNames, synonymTypes, synonym);
		ArgumentTypes argType = checkRef.second;
		if (!checkRef.first) {
			std::cout << "Invalid with-cl: Synonym does not exist" << std::endl;
			return ArgumentTypes::NONE;
		}

		// get AttrName
		AttrName attrName = QueryParserUtil::stringToAttribute(attr);
		if (attrName == AttrName::NONE) {
			std::cout << "Invalid with-cl: Unrecognised attrName" << std::endl;
			return ArgumentTypes::NONE;
		}

		// return appropriate argument type
		if (QueryParserUtil::validateAttrType(argType, attrName)) {
			if (attrName == AttrName::VALUE || attrName == AttrName::STMT_NUM) {
				return ArgumentTypes::INT;	
			}
			else if (attrName == AttrName::PROC_NAME || attrName == AttrName::VAR_NAME) {
				return ArgumentTypes::STRING;
			}
			else {
				std::cout << "Invalid with-cl: Error parsing attrName" << std::endl;
				return ArgumentTypes::NONE;
			}
		}
		else {
			std::cout << "Invalid with-cl: Invalid synonym-attrName pair" << std::endl;
			return ArgumentTypes::NONE;
		}
	}
	else {
		// ‘”’ IDENT ‘”’ | INTEGER | synonym
		if (LexicalSyntaxCheck::checkInteger(ref)) {
			return ArgumentTypes::INT;
		}
		else if (LexicalSyntaxCheck::checkName(ref)) {
			std::pair<bool, ArgumentTypes> checkRef = QueryParserUtil::checkNameExists(synonymNames, synonymTypes, ref);
			if (!checkRef.first) {
				std::cout << "Invalid with-cl: Synonym does not exist" << std::endl;
				return ArgumentTypes::NONE;
			}
			else {
				if (checkRef.second != ArgumentTypes::PROG_LINE) {
					std::cout << "Invalid with-cl: Synonym ref is not prog_line" << std::endl;
					return ArgumentTypes::NONE;
				}
				return checkRef.second;
			}
		}
		else {
			// check IDENT (quotation marks surrounding checkName
			if (ref[0] == '\"' && ref[ref.size() - 1] == '\"') {
				ref = ref.substr(1, ref.size() - 2); // exclude quotation marks
				if (LexicalSyntaxCheck::checkName(ref)) {
					return ArgumentTypes::STRING;
				}
				else {
					std::cout << "Invalid with-cl: Unrecognised ref type" << std::endl;
					return ArgumentTypes::NONE;
				}
			}
			else {
				std::cout << "Invalid with-cl: Unrecognised ref type" << std::endl;
				return ArgumentTypes::NONE;
			}
		}
	}
}

bool WithClauseParser::isAttrRef(std::string ref) {
	std::size_t dotPos = ref.find(".");
	if (dotPos != std::string::npos) {
		std::string synonym = ref.substr(0, dotPos);
		std::string attr = ref.substr(dotPos + 1);
		synonym = QueryParserUtil::trim(synonym);
		attr = QueryParserUtil::trim(attr);

		if (!LexicalSyntaxCheck::checkName(synonym)) {
			return false;
		}
		if (QueryParserUtil::stringToAttribute(attr) == AttrName::NONE) {
			return false;
		}

		return true;
	}

	return false;
}

// get synonym type for ref that is of the form 'synonym . AttrName'
ArgumentTypes WithClauseParser::parseSynonymType(std::string ref, std::vector<std::string> synonymNames, std::vector<ArgumentTypes> synonymTypes) {
	if (isAttrRef(ref)) {
		std::size_t dotPos = ref.find(".");
		std::string synonym = ref.substr(0, dotPos);
		synonym = QueryParserUtil::trim(synonym);

		std::pair<bool, ArgumentTypes> checkRef = QueryParserUtil::checkNameExists(synonymNames, synonymTypes, synonym);
		if (!checkRef.first) {
			std::cout << "Invalid with-cl: Synonym does not exist" << std::endl;
			return ArgumentTypes::NONE;
		}
		else {
			return checkRef.second;
		}
	}
	else {
		return ArgumentTypes::NONE;
	}
}

AttrName WithClauseParser::parseAttrName(std::string ref) {
	if (isAttrRef(ref)) {
		std::size_t dotPos = ref.find(".");
		std::string attr = ref.substr(dotPos + 1);
		attr = QueryParserUtil::trim(attr);
		return QueryParserUtil::stringToAttribute(attr);
	}
	else {
		return AttrName::NONE;
	}
}