#include <iostream>

#include "SuchThatClauseParser.h"
#include "QueryParserUtil.h"
#include "LexicalSyntaxCheck.h"

// Uses
QuerySuchThatClause SuchThatClauseParser::parseUses(RelRef relRef, std::string firstArg, std::string secondArg,
    std::vector<std::string> synonymNames, std::vector<ArgumentTypes> synonymTypes) {
    if (relRef != RelRef::USES_S) {
        std::cout << "suchthat-cl Parsing Error: Invalid RelRef" << std::endl;
        return QuerySuchThatClause();
    }

    ArgumentTypes firstType{};
    ArgumentTypes secondType{};

    if (LexicalSyntaxCheck::checkInteger(firstArg)) {
        if (firstArg.compare("0") == 0) {
            return QuerySuchThatClause(false); // code line less than 1
        }
        firstType = ArgumentTypes::INT;
    }
    else if (firstArg.compare("_") == 0) {
        std::cout << "Invalid suchthat: Uses cannot have _ as first argument\n";
        return QuerySuchThatClause(false); // semantically invalid clause
    }
    // ident
    else if (firstArg[0] == '"' && firstArg[firstArg.size() - 1] == '"') {
        relRef = RelRef::USES_P;
        std::string trimmedFirstArg = firstArg.substr(1, firstArg.size() - 2);
        trimmedFirstArg = QueryParserUtil::trim(trimmedFirstArg);
        if (LexicalSyntaxCheck::checkName(trimmedFirstArg)) {
            firstType = ArgumentTypes::STRING;
            firstArg = firstArg.substr(1, firstArg.size() - 2); // remove quotation marks
            firstArg = QueryParserUtil::trim(firstArg);
        }
        else {
            std::cout << "Invalid suchthat: Syntactically invalid ident\n";
            return QuerySuchThatClause(); // invalid clause
        }
    }
    // synonym
    else {
        auto namePos = std::find(synonymNames.begin(), synonymNames.end(), firstArg);
        bool synonymExists = namePos != synonymNames.end();
        if (!synonymExists) {
            std::cout << firstArg << std::endl; // debug print
            std::cout << "Invalid suchthat: Synonym does not exist\n";
            return QuerySuchThatClause(false); // semantically invalid clause
        }

        int typeIndex = namePos - synonymNames.begin();
        firstType = synonymTypes[typeIndex];

        if (firstType == ArgumentTypes::PROCEDURE) {
            relRef = RelRef::USES_P;
        }
        // check for semantics
        else if (QueryParserUtil::isSynonym(firstType)) {
            if (!(firstType == ArgumentTypes::STMT || firstType == ArgumentTypes::WHILE ||
                firstType == ArgumentTypes::IF || firstType == ArgumentTypes::ASSIGN || firstType == ArgumentTypes::CONSTANT ||
                firstType == ArgumentTypes::PRINT || firstType == ArgumentTypes::CALL || firstType == ArgumentTypes::PROG_LINE)) {
                std::cout << "Invalid suchthat: Semantically invalid type\n";
                return QuerySuchThatClause(false); // invalid clause
            }

        }
    }

    if (secondArg[0] == '"' && secondArg[secondArg.size() - 1] == '"') {
        std::string trimmedSecondArg = secondArg.substr(1, secondArg.size() - 2);
        trimmedSecondArg = QueryParserUtil::trim(trimmedSecondArg);
        if (LexicalSyntaxCheck::checkName(trimmedSecondArg)) {
            secondType = ArgumentTypes::STRING;
            secondArg = secondArg.substr(1, secondArg.size() - 2); // remove quotation marks
            secondArg = QueryParserUtil::trim(secondArg);
        }
        else {
            std::cout << "Invalid pattern: Syntactically invalid type\n";
            return QuerySuchThatClause(); // invalid clause
        }
    }
    else if (secondArg.compare("_") == 0) {
        secondType = ArgumentTypes::ANY;
    }
    else {
        // synonym
        auto namePos = std::find(synonymNames.begin(), synonymNames.end(), secondArg);
        bool synonymExists = namePos != synonymNames.end();
        if (!synonymExists) {
            std::cout << secondArg << std::endl; // debug print
            std::cout << "Invalid suchthat: Synonym does not exist\n";
            return QuerySuchThatClause(false); // semantically invalid clause
        }

        int typeIndex = namePos - synonymNames.begin();
        secondType = synonymTypes[typeIndex];
        if (secondType != ArgumentTypes::VARIABLE) {
            std::cout << "Invalid suchthat: Semantically invalid type\n";
            return QuerySuchThatClause(false); // invalid clause
        }
    }

    return QuerySuchThatClause(relRef, firstArg, secondArg, firstType, secondType);
}

// Modifies
QuerySuchThatClause SuchThatClauseParser::parseModifies(RelRef relRef, std::string firstArg, std::string secondArg,
    std::vector<std::string> synonymNames, std::vector<ArgumentTypes> synonymTypes) {
    if (relRef != RelRef::MODIFIES_S) {
        std::cout << "suchthat-cl Parsing Error: Invalid RelRef" << std::endl;
        return QuerySuchThatClause();
    }

    ArgumentTypes firstType{};
    ArgumentTypes secondType{};

    if (LexicalSyntaxCheck::checkInteger(firstArg)) {
        if (firstArg.compare("0") == 0) {
            return QuerySuchThatClause(false); // code line less than 1
        }
        firstType = ArgumentTypes::INT;
    }
    else if (firstArg.compare("_") == 0) {
        std::cout << "Invalid suchthat: Modifies cannot have _ as first argument\n";
        return QuerySuchThatClause(false); // semantically invalid clause
    }
    // ident
    else if (firstArg[0] == '"' && firstArg[firstArg.size() - 1] == '"') {
        relRef = RelRef::MODIFIES_P;
        std::string trimmedFirstArg = firstArg.substr(1, firstArg.size() - 2);
        trimmedFirstArg = QueryParserUtil::trim(trimmedFirstArg);
        if (LexicalSyntaxCheck::checkName(trimmedFirstArg)) {
            firstType = ArgumentTypes::STRING;
            firstArg = firstArg.substr(1, firstArg.size() - 2); // remove quotation marks
            firstArg = QueryParserUtil::trim(firstArg);
        }
        else {
            std::cout << "Invalid suchthat: Semantically invalid ident\n";
            return QuerySuchThatClause(); // invalid clause
        }
    }
    // synonym
    else {
        auto namePos = std::find(synonymNames.begin(), synonymNames.end(), firstArg);
        bool synonymExists = namePos != synonymNames.end();
        if (!synonymExists) {
            std::cout << firstArg << std::endl; // debug print
            std::cout << "Invalid suchthat: Synonym does not exist\n";
            return QuerySuchThatClause(); // invalid clause
        }

        int typeIndex = namePos - synonymNames.begin();
        firstType = synonymTypes[typeIndex];

        if (firstType == ArgumentTypes::PROCEDURE) {
            relRef = RelRef::MODIFIES_P;
        }
        // check for semantics
        else if (!(firstType == ArgumentTypes::STMT || firstType == ArgumentTypes::WHILE ||
            firstType == ArgumentTypes::IF || firstType == ArgumentTypes::ASSIGN || firstType == ArgumentTypes::CONSTANT ||
            firstType == ArgumentTypes::READ || firstType == ArgumentTypes::CALL || firstType == ArgumentTypes::PROG_LINE)) {
            std::cout << "Invalid suchthat: Semantically invalid type\n";
            return QuerySuchThatClause(false); // invalid clause
        }
    }

    if (secondArg[0] == '"' && secondArg[secondArg.size() - 1] == '"') {
        std::string trimmedSecondArg = secondArg.substr(1, secondArg.size() - 2);
        trimmedSecondArg = QueryParserUtil::trim(trimmedSecondArg);
        if (LexicalSyntaxCheck::checkName(trimmedSecondArg)) {
            secondType = ArgumentTypes::STRING;
            secondArg = secondArg.substr(1, secondArg.size() - 2); // remove quotation marks
            secondArg = QueryParserUtil::trim(secondArg);
        }
        else {
            std::cout << "Invalid pattern: Semantically invalid type\n";
            return QuerySuchThatClause(); // invalid clause
        }
    }
    else if (secondArg.compare("_") == 0) {
        secondType = ArgumentTypes::ANY;
    }
    else {
        // synonym
        auto namePos = std::find(synonymNames.begin(), synonymNames.end(), secondArg);
        bool synonymExists = namePos != synonymNames.end();
        if (!synonymExists) {
            std::cout << secondArg << std::endl; // debug print
            std::cout << "Invalid suchthat: Synonym does not exist\n";
            return QuerySuchThatClause(false); // invalid clause
        }

        int typeIndex = namePos - synonymNames.begin();
        secondType = synonymTypes[typeIndex];
        if (secondType != ArgumentTypes::VARIABLE) {
            std::cout << "Invalid suchthat: Semantically invalid type\n";
            return QuerySuchThatClause(false); // invalid clause
        }
    }

    return QuerySuchThatClause(relRef, firstArg, secondArg, firstType, secondType);
}

// Parent and ParentT
QuerySuchThatClause SuchThatClauseParser::parseParent(RelRef relRef, std::string firstArg, std::string secondArg,
    std::vector<std::string> synonymNames, std::vector<ArgumentTypes> synonymTypes) {
    if (relRef != RelRef::PARENT && relRef != RelRef::PARENT_T) {
        std::cout << "suchthat-cl Parsing Error: Invalid RelRef" << std::endl;
        return QuerySuchThatClause();
    }

    ArgumentTypes firstType{};
    ArgumentTypes secondType{};

    if (LexicalSyntaxCheck::checkInteger(firstArg)) {
        if (firstArg.compare("0") == 0) {
            return QuerySuchThatClause(false); // code line less than 1
        }
        firstType = ArgumentTypes::INT;
    }
    else if (firstArg.compare("_") == 0) {
        firstType = ArgumentTypes::ANY;
    }
    // synonym
    else {
        auto namePos = std::find(synonymNames.begin(), synonymNames.end(), firstArg);
        bool synonymExists = namePos != synonymNames.end();
        if (!synonymExists) {
            std::cout << "Invalid suchthat: Synonym does not exist\n";
            return QuerySuchThatClause(false); // invalid clause
        }

        int typeIndex = namePos - synonymNames.begin();
        firstType = synonymTypes[typeIndex];
        if (!(firstType == ArgumentTypes::STMT || firstType == ArgumentTypes::WHILE || firstType == ArgumentTypes::IF ||
            firstType == ArgumentTypes::PROG_LINE || firstType == ArgumentTypes::CONSTANT)) {
            std::cout << "Invalid suchthat: Semantically invalid type\n";
            return QuerySuchThatClause(false); // invalid clause
        }
    }

    if (LexicalSyntaxCheck::checkInteger(secondArg)) {
        if (secondArg.compare("0") == 0) {
            return QuerySuchThatClause(false); // code line less than 1
        }
        secondType = ArgumentTypes::INT;
    }
    else if (secondArg.compare("_") == 0) {
        secondType = ArgumentTypes::ANY;
    }
    // synonym
    else {
        auto namePos = std::find(synonymNames.begin(), synonymNames.end(), secondArg);
        bool synonymExists = namePos != synonymNames.end();
        if (!synonymExists) {
            std::cout << "Invalid suchthat: Synonym does not exist\n";
            return QuerySuchThatClause(false); // invalid clause
        }

        int typeIndex = namePos - synonymNames.begin();
        secondType = synonymTypes[typeIndex];
        if (!(secondType == ArgumentTypes::STMT || secondType == ArgumentTypes::READ || secondType == ArgumentTypes::WHILE ||
            secondType == ArgumentTypes::PRINT || secondType == ArgumentTypes::IF || secondType == ArgumentTypes::ASSIGN ||
            secondType == ArgumentTypes::CALL || secondType == ArgumentTypes::PROG_LINE || secondType == ArgumentTypes::CONSTANT)) {
            std::cout << "Invalid suchthat: Semantically invalid type\n";
            return QuerySuchThatClause(false); // invalid clause
        }
    }

    return QuerySuchThatClause(relRef, firstArg, secondArg, firstType, secondType);
}

//Follows and FollowsT
QuerySuchThatClause SuchThatClauseParser::parseFollows(RelRef relRef, std::string firstArg, std::string secondArg,
    std::vector<std::string> synonymNames, std::vector<ArgumentTypes> synonymTypes) {
    if (relRef != RelRef::FOLLOWS && relRef != RelRef::FOLLOWS_T) {
        std::cout << "suchthat-cl Parsing Error: Invalid RelRef" << std::endl;
        return QuerySuchThatClause();
    }

    ArgumentTypes firstType{};
    ArgumentTypes secondType{};

    if (LexicalSyntaxCheck::checkInteger(firstArg)) {
        if (firstArg.compare("0") == 0) {
            return QuerySuchThatClause(false); // code line less than 1
        }
        firstType = ArgumentTypes::INT;
    }
    else if (firstArg.compare("_") == 0) {
        firstType = ArgumentTypes::ANY;
    }
    // synonym
    else {
        auto namePos = std::find(synonymNames.begin(), synonymNames.end(), firstArg);
        bool synonymExists = namePos != synonymNames.end();
        if (!synonymExists) {
            std::cout << firstArg << std::endl; // debug print
            std::cout << "Invalid suchthat: Synonym does not exist\n";
            return QuerySuchThatClause(false); // invalid clause
        }

        int typeIndex = namePos - synonymNames.begin();
        firstType = synonymTypes[typeIndex];
        if (!(firstType == ArgumentTypes::STMT || firstType == ArgumentTypes::READ || firstType == ArgumentTypes::WHILE ||
            firstType == ArgumentTypes::PRINT || firstType == ArgumentTypes::IF || firstType == ArgumentTypes::ASSIGN ||
            firstType == ArgumentTypes::CALL || firstType == ArgumentTypes::PROG_LINE || firstType == ArgumentTypes::CONSTANT)) {
            std::cout << "Invalid suchthat: Semantically invalid type\n";
            return QuerySuchThatClause(false); // invalid clause
        }
    }

    if (LexicalSyntaxCheck::checkInteger(secondArg)) {
        if (secondArg.compare("0") == 0) {
            return QuerySuchThatClause(false); // code line less than 1
        }
        secondType = ArgumentTypes::INT;
    }
    else if (secondArg.compare("_") == 0) {
        secondType = ArgumentTypes::ANY;
    }
    // synonym
    else {
        auto namePos = std::find(synonymNames.begin(), synonymNames.end(), secondArg);
        bool synonymExists = namePos != synonymNames.end();
        if (!synonymExists) {
            std::cout << secondArg << std::endl; // debug print
            std::cout << "Invalid suchthat: Synonym does not exist\n";
            return QuerySuchThatClause(false); // invalid clause
        }

        int typeIndex = namePos - synonymNames.begin();
        secondType = synonymTypes[typeIndex];
        if (!(secondType == ArgumentTypes::STMT || secondType == ArgumentTypes::READ || secondType == ArgumentTypes::WHILE ||
            secondType == ArgumentTypes::PRINT || secondType == ArgumentTypes::IF || secondType == ArgumentTypes::ASSIGN ||
            secondType == ArgumentTypes::CALL || secondType == ArgumentTypes::PROG_LINE || secondType == ArgumentTypes::CONSTANT)) {
            std::cout << "Invalid suchthat: Semantically invalid type\n";
            return QuerySuchThatClause(false); // invalid clause
        }
    }

    return QuerySuchThatClause(relRef, firstArg, secondArg, firstType, secondType);
}

//Calls and Calls
QuerySuchThatClause SuchThatClauseParser::parseCalls(RelRef relRef, std::string firstArg, std::string secondArg,
    std::vector<std::string> synonymNames, std::vector<ArgumentTypes> synonymTypes) {
    if (relRef != RelRef::CALLS && relRef != RelRef::CALLS_T) {
        std::cout << "suchthat-cl Parsing Error: Invalid RelRef" << std::endl;
        return QuerySuchThatClause();
    }

    ArgumentTypes firstType{};
    ArgumentTypes secondType{};

    if (firstArg.compare("_") == 0) {
        firstType = ArgumentTypes::ANY;
    }
    // ident
    else if (firstArg[0] == '"' && firstArg[firstArg.size() - 1] == '"') {
        std::string trimmedFirstArg = firstArg.substr(1, firstArg.size() - 2);
        trimmedFirstArg = QueryParserUtil::trim(trimmedFirstArg);
        if (LexicalSyntaxCheck::checkName(trimmedFirstArg)) {
            firstType = ArgumentTypes::STRING;
            firstArg = firstArg.substr(1, firstArg.size() - 2); // remove quotation marks
            firstArg = QueryParserUtil::trim(firstArg);
        }
        else {
            std::cout << "Invalid suchthat: Semantically invalid ident\n";
            return QuerySuchThatClause(); // invalid clause
        }
    }
    // synonym
    else {
        if (!LexicalSyntaxCheck::checkName(firstArg)) {
            return QuerySuchThatClause(); // syntax error
        }

        auto namePos = std::find(synonymNames.begin(), synonymNames.end(), firstArg);
        bool synonymExists = namePos != synonymNames.end();
        if (!synonymExists) {
            std::cout << firstArg << std::endl; // debug print
            std::cout << "Invalid suchthat: Synonym does not exist\n";
            return QuerySuchThatClause(false); // invalid clause
        }

        int typeIndex = namePos - synonymNames.begin();
        firstType = synonymTypes[typeIndex];
        if (firstType != ArgumentTypes::PROCEDURE) {
            std::cout << "Invalid suchthat: Semantically invalid type\n";
            return QuerySuchThatClause(false); // invalid clause
        }
    }

    if (secondArg.compare("_") == 0) {
        secondType = ArgumentTypes::ANY;
    }
    // ident
    else if (secondArg[0] == '"' && secondArg[secondArg.size() - 1] == '"') {
        std::string trimmedFirstArg = secondArg.substr(1, secondArg.size() - 2);
        trimmedFirstArg = QueryParserUtil::trim(trimmedFirstArg);
        if (LexicalSyntaxCheck::checkName(trimmedFirstArg)) {
            secondType = ArgumentTypes::STRING;
            secondArg = secondArg.substr(1, secondArg.size() - 2); // remove quotation marks
            secondArg = QueryParserUtil::trim(secondArg);
        }
        else {
            std::cout << "Invalid suchthat: Semantically invalid ident\n";
            return QuerySuchThatClause(); // invalid clause
        }
    }
    // synonym
    else {
        if (!LexicalSyntaxCheck::checkName(secondArg)) {
            return QuerySuchThatClause(); // syntax error
        }

        auto namePos = std::find(synonymNames.begin(), synonymNames.end(), secondArg);
        bool synonymExists = namePos != synonymNames.end();
        if (!synonymExists) {
            std::cout << secondArg << std::endl; // debug print
            std::cout << "Invalid suchthat: Synonym does not exist\n";
            return QuerySuchThatClause(false); // invalid clause
        }

        int typeIndex = namePos - synonymNames.begin();
        secondType = synonymTypes[typeIndex];
        if (secondType != ArgumentTypes::PROCEDURE) {
            std::cout << "Invalid suchthat: Semantically invalid type\n";
            return QuerySuchThatClause(false); // invalid clause
        }
    }

    return QuerySuchThatClause(relRef, firstArg, secondArg, firstType, secondType);
}

//Next and NextT and NextBip and NextBipT
QuerySuchThatClause SuchThatClauseParser::parseNext(RelRef relRef, std::string firstArg, std::string secondArg,
    std::vector<std::string> synonymNames, std::vector<ArgumentTypes> synonymTypes) {
    if (relRef != RelRef::NEXT && relRef != RelRef::NEXT_T && relRef != RelRef::NEXT_BIP && relRef != RelRef::NEXT_BIP_T) {
        std::cout << "suchthat-cl Parsing Error: Invalid RelRef" << std::endl;
        return QuerySuchThatClause();
    }

    ArgumentTypes firstType{};
    ArgumentTypes secondType{};

    if (LexicalSyntaxCheck::checkInteger(firstArg)) {
        if (firstArg.compare("0") == 0) {
            return QuerySuchThatClause(false); // code line less than 1
        }
        firstType = ArgumentTypes::INT;
    }
    else if (firstArg.compare("_") == 0) {
        firstType = ArgumentTypes::ANY;
    }
    // synonym
    else {
        auto namePos = std::find(synonymNames.begin(), synonymNames.end(), firstArg);
        bool synonymExists = namePos != synonymNames.end();
        if (!synonymExists) {
            std::cout << firstArg << std::endl; // debug print
            std::cout << "Invalid suchthat: Synonym does not exist\n";
            return QuerySuchThatClause(false); // invalid clause
        }

        int typeIndex = namePos - synonymNames.begin();
        firstType = synonymTypes[typeIndex];

        if (!(firstType == ArgumentTypes::STMT || firstType == ArgumentTypes::READ || firstType == ArgumentTypes::WHILE ||
            firstType == ArgumentTypes::PRINT || firstType == ArgumentTypes::IF || firstType == ArgumentTypes::ASSIGN ||
            firstType == ArgumentTypes::PROG_LINE || firstType == ArgumentTypes::CALL || firstType == ArgumentTypes::CONSTANT)) {
            std::cout << QueryParserUtil::argumentTypesToString(firstType) << std::endl; // debug print
            std::cout << "Invalid suchthat: Semantically invalid type\n";
            return QuerySuchThatClause(false); // invalid clause
        }
    }

    if (LexicalSyntaxCheck::checkInteger(secondArg)) {
        if (secondArg.compare("0") == 0) {
            return QuerySuchThatClause(false); // code line less than 1
        }
        secondType = ArgumentTypes::INT;
    }
    else if (secondArg.compare("_") == 0) {
        secondType = ArgumentTypes::ANY;
    }
    // synonym
    else {
        auto namePos = std::find(synonymNames.begin(), synonymNames.end(), secondArg);
        bool synonymExists = namePos != synonymNames.end();
        if (!synonymExists) {
            std::cout << secondArg << std::endl; // debug print
            std::cout << "Invalid suchthat: Synonym does not exist\n";
            return QuerySuchThatClause(false); // invalid clause
        }

        int typeIndex = namePos - synonymNames.begin();
        secondType = synonymTypes[typeIndex];
        if (!(secondType == ArgumentTypes::STMT || secondType == ArgumentTypes::READ || secondType == ArgumentTypes::WHILE ||
            secondType == ArgumentTypes::PRINT || secondType == ArgumentTypes::IF || secondType == ArgumentTypes::ASSIGN ||
            secondType == ArgumentTypes::PROG_LINE || secondType == ArgumentTypes::CALL || secondType == ArgumentTypes::CONSTANT)) {
            std::cout << "Invalid suchthat: Semantically invalid type\n";
            return QuerySuchThatClause(false); // invalid clause
        }
    }

    return QuerySuchThatClause(relRef, firstArg, secondArg, firstType, secondType);
}

//Affects and AffectsT and AffectsBip and AffectsBipT
QuerySuchThatClause SuchThatClauseParser::parseAffects(RelRef relRef, std::string firstArg, std::string secondArg,
    std::vector<std::string> synonymNames, std::vector<ArgumentTypes> synonymTypes) {
    if (relRef != RelRef::AFFECTS && relRef != RelRef::AFFECTS_T && relRef != RelRef::AFFECTS_BIP && relRef != RelRef::AFFECTS_BIP_T) {
        std::cout << "suchthat-cl Parsing Error: Invalid RelRef" << std::endl;
        return QuerySuchThatClause();
    }

    ArgumentTypes firstType{};
    ArgumentTypes secondType{};

    if (LexicalSyntaxCheck::checkInteger(firstArg)) {
        if (firstArg.compare("0") == 0) {
            return QuerySuchThatClause(false); // code line less than 1
        }
        firstType = ArgumentTypes::INT;
    }
    else if (firstArg.compare("_") == 0) {
        firstType = ArgumentTypes::ANY;
    }
    // synonym
    else {
        auto namePos = std::find(synonymNames.begin(), synonymNames.end(), firstArg);
        bool synonymExists = namePos != synonymNames.end();
        if (!synonymExists) {
            std::cout << firstArg << std::endl; // debug print
            std::cout << "Invalid suchthat: Synonym does not exist\n";
            return QuerySuchThatClause(false); // invalid clause
        }

        int typeIndex = namePos - synonymNames.begin();
        firstType = synonymTypes[typeIndex];
        if (!(firstType == ArgumentTypes::STMT || firstType == ArgumentTypes::ASSIGN || firstType == ArgumentTypes::PROG_LINE || firstType == ArgumentTypes::CONSTANT)) {
            std::cout << "Invalid suchthat: Semantically invalid type\n";
            return QuerySuchThatClause(false); // invalid clause
        }
    }

    if (LexicalSyntaxCheck::checkInteger(secondArg)) {
        if (secondArg.compare("0") == 0) {
            return QuerySuchThatClause(false); // code line less than 1
        }
        secondType = ArgumentTypes::INT;
    }
    else if (secondArg.compare("_") == 0) {
        secondType = ArgumentTypes::ANY;
    }
    // synonym
    else {
        auto namePos = std::find(synonymNames.begin(), synonymNames.end(), secondArg);
        bool synonymExists = namePos != synonymNames.end();
        if (!synonymExists) {
            std::cout << secondArg << std::endl; // debug print
            std::cout << "Invalid suchthat: Synonym does not exist\n";
            return QuerySuchThatClause(false); // invalid clause
        }

        int typeIndex = namePos - synonymNames.begin();
        secondType = synonymTypes[typeIndex];
        if (!(secondType == ArgumentTypes::STMT || secondType == ArgumentTypes::ASSIGN || secondType == ArgumentTypes::PROG_LINE || secondType == ArgumentTypes::CONSTANT)) {
            std::cout << "Invalid suchthat: Semantically invalid type\n";
            return QuerySuchThatClause(false); // invalid clause
        }
    }

    return QuerySuchThatClause(relRef, firstArg, secondArg, firstType, secondType);
}