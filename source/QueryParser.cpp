#pragma once
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include <sstream>
#include <algorithm>
#include <locale>

#include "QueryParser.h"
#include "QueryParserUtil.h"
#include "QueryResultClauseParser.h"
#include "SuchThatClauseParser.h"
#include "PatternClauseParser.h"
#include "WithClauseParser.h"
#include "LexicalSyntaxCheck.h"

// Constructor
QueryParser::QueryParser() {}

// Main API
Query QueryParser::parseQuery(std::string userQuery) {
	std::cout << "\nQUERY PARSER TEST\n";

    // final query obj to be returned
    Query parsedQuery;

    /*=== Split raw query by declaration and select statements ===*/

    // find all occurrences of "Select"
    // note: could be a synonym name
    size_t selectPos = userQuery.find("Select");
    std::vector<size_t> occurrences;
    while (userQuery.find("Select", selectPos) != std::string::npos) {
        selectPos = userQuery.find("Select", selectPos);
        occurrences.push_back(selectPos);
        selectPos++;
    }

    if (occurrences.size() == 0) {
        std::cout << "Invalid syntax: No select-cl found\n";
        return Query();
    }

    bool finishedDeclaration = false;
    bool startLine = true;
    bool firstChar = true;
    bool isDeclarationSemanticallyValid = true;
    std::string declaration;
    std::string selection;

    int startSelect = -1;

    for (int i = 0; i < (int) userQuery.size(); i++) {
        char& c = userQuery.at(i);
        // the Select is only a functional keyword if it is at the start of a line
        // if there are multiple occurrence of "Select" at the start of a line, it is invalid
        if (std::find(occurrences.begin(), occurrences.end(), i) != occurrences.end()) {
            if (startSelect >= 0 && (startLine || firstChar)) {
                std::cout << "Invalid syntax: More than one select-cl\n";
                return Query();
            }
            if (startLine || firstChar) {
                startSelect = i;
                finishedDeclaration = true;
                selection += c;
                startLine = false;
                firstChar = false;
                continue;
            }
        }

        firstChar = false;

        if (finishedDeclaration) {
            if (c == ';') {
                std::cout << "Invalid syntax: Extra semicolon at end of select-cl\n";
                return Query();
            }
            else {
                selection += c;
            }
        }
        else {
            if (!std::isspace(c)) {
                startLine = c == ';';
            }
            declaration += c;
        }
    }

    // valid query must have a select-cl
    if (selection.size() == 0) {
        std::cout << "Invalid query: No select-cl\n";
        return Query();
    }

    /*=== PARSE DECLARATIONS ===*/
    std::istringstream declarationStream(declaration);
    std::string line;

    // store declarations
    std::vector<std::string> synonymNames;
    std::vector<ArgumentTypes> synonymTypes;

    while (std::getline(declarationStream, line, ';')) {
        if (line.size() == 0) {
            std::cout << "Invalid syntax: Extra semicolons\n";
            return Query();
        }

        line = QueryParserUtil::trim(line);
        bool lookingForName = false;

        std::vector<std::string> declarationToken = QueryParserUtil::tokenizeDeclaration(line);
        for (int i = 0; i < declarationToken.size(); i++) {
            std::string token = declarationToken[i];
            token = QueryParserUtil::trim(token);

            if (lookingForName) {
                std::pair<bool, ArgumentTypes> checkSynonym = QueryParserUtil::checkNameExists(synonymNames, synonymTypes, token);
                if (checkSynonym.first) {
                    std::cout << "Invalid declaration: Repeated synonym names\n";
                    isDeclarationSemanticallyValid = false;
                }

                if (!LexicalSyntaxCheck::checkName(token)) {
                    std::cout << token << std::endl; // debug print
                    std::cout << "Invalid declaration: Invalid synonym name" << std::endl;
                    return Query();
                }

                // if declared name is unqiue and a valid IDENT
                synonymNames.push_back(token);
                // sanity check
                if (synonymTypes.empty()) {
                    std::cout << "Invalid declaration: Error parsing" << std::endl;
                    return Query();
                }

                // check if there are multiple synonym names declared (ie. whether next token is a comma)
                bool hasMultiple = (i + 1 < declarationToken.size()) && declarationToken[i + 1].compare(",") == 0;
                if (hasMultiple) {
                    synonymTypes.push_back(synonymTypes.back());
                    lookingForName = true; // continue parsing for synonym name
                    i++; // skip the comma
                }
                else {
                    lookingForName = false;
                }
            }
            else {
                if (token.empty()) {
                    // reached EOL
                    break;
                }

                ArgumentTypes arg = QueryParserUtil::stringToArgumentTypes(token);
                if (arg == ArgumentTypes::NONE) {
                    std::cout << token << std::endl;
                    std::cout << "Invalid syntax: No such design entity\n";
                    return Query();
                }
                synonymTypes.push_back(arg);

                // set flag when it is a valid design entity declaration
                lookingForName = true;
            }
        }

        if (lookingForName) {
            std::cout << "Invalid syntax: Missing synonym name\n";
            return Query();
        }
    }

    /*=== PARSE SELECT-CL ===*/

    selection = QueryParserUtil::trim(selection);

    bool findResult = true;
    bool parsingCondition = false;
    bool parsingSuchThat = false;
    bool parsingPattern = false;
    bool parsingWith = false;
    int lastParsedClause = 0;
    // 0 = no clause, 1 = suchthat, 2 = pattern, 3 = with
    // for parsing 'and' keyword

    // query parameters
    bool isBooleanResult = false;
    std::vector<QuerySuchThatClause> suchThatClauses = {};
    std::vector<QueryPatternClause> patternClauses = {};
    std::vector<QueryWithClause> withClauses = {};

    std::vector<std::string> resultSynonyms = {};
    std::vector<ArgumentTypes> resultTypes = {};
    std::vector<AttrName> resultAttrNames = {};    

    std::vector<std::string> selectionToken = QueryParserUtil::tokenizeSelection(selection);
    // for debugging
    for (std::string str : selectionToken) {
        std::cout << str << std::endl;
    }

    // sanity check: first token has to be "Select"
    if (selectionToken[0].compare("Select") != 0) {
        std::cout << "Invalid syntax: Unrecognised select-cl\n";
        return Query();
    };

    // sanity check: there should be no semicolons
    if (std::find(selectionToken.begin(), selectionToken.end(), ";") != selectionToken.end()) {
        std::cout << "Invalid syntax: Extra semicolon at end of select-cl\n";
        return Query();
    };

    // begin parsing from the token after the first token "Select"
    for (int i = 1; i < selectionToken.size(); i++) {
        std::string token = selectionToken[i];

        if (findResult) {
            // check whether <tuple>, single synonym or BOOLEAN

            if (token.compare("<") == 0) {
                auto closingPos = std::find(selectionToken.begin(), selectionToken.end(), ">");
                bool hasClosingBracket = closingPos != selectionToken.end();
                if (hasClosingBracket) {
                    int closingIndex = closingPos - selectionToken.begin();

                    // resultTuple extracts the result tuple without the < >
                    std::vector<std::string> resultTuple = {selectionToken.begin() + 2, selectionToken.begin() + closingIndex};
                    std::tuple<bool, std::vector<std::string>, std::vector<ArgumentTypes>,
                        std::vector<AttrName>> parsedTuple = QueryResultClauseParser::parseTuple(resultTuple, synonymNames, synonymTypes);

                    if (std::get<0>(parsedTuple)) {
                        // valid result tuple
                        resultSynonyms = std::get<1>(parsedTuple);
                        resultTypes = std::get<2>(parsedTuple);
                        resultAttrNames = std::get<3>(parsedTuple);
                        i = closingIndex; // skip token to ">"
                        findResult = false;
                    }
                    else {
                        // invalid result tuple
                        std::cout << "Invalid syntax: Error parsing result tuple\n";
                        return Query();
                    }
                }
                else {
                    std::cout << "Invalid syntax: Missing closing bracket for result tuple\n";
                    return Query();
                }
            }
            else if (token.compare("BOOLEAN") == 0) {
                findResult = false; // only one result

                std::pair<bool, ArgumentTypes> checkBoolean = QueryParserUtil::checkNameExists(synonymNames, synonymTypes, token);
                if (!checkBoolean.first) {
                    isBooleanResult = true;
                    resultSynonyms.push_back(token);
                    resultTypes.push_back(ArgumentTypes::BOOLEAN);
                    resultAttrNames.push_back(AttrName::NONE);

                    // note: invalid attrRef like Select BOOLEAN.varName will be caught
                    // because next token '.' will not be recognised
                }
                else {
                    // synonym with name BOOLEAN
                    // parse as regular single synonym

                    // check for attributes
                    std::vector<std::string> resultVector = {};
                    if (i + 1 < selectionToken.size() && selectionToken[i + 1].compare(".") == 0) {
                        if (i + 2 < selectionToken.size()) {
                            resultVector.push_back(token);
                            resultVector.push_back(selectionToken[i + 2]);
                            i += 2;
                        }
                        else {
                            std::cout << "Invalid select: Missing attrName\n";
                            return Query();
                        }
                    }
                    else {
                        // no attribute
                        resultVector.push_back(token);
                    }

                    std::tuple<bool, std::vector<std::string>, std::vector<ArgumentTypes>,
                        std::vector<AttrName>> parsedResultSynonym = QueryResultClauseParser::parseSingleSynonym(resultVector,
                            synonymNames, synonymTypes);

                    if (!std::get<0>(parsedResultSynonym)) {
                        std::cout << "Invalid select: Error parsing result-cl\n";
                        return Query();
                    }

                    resultSynonyms = std::get<1>(parsedResultSynonym);
                    resultTypes = std::get<2>(parsedResultSynonym);
                    resultAttrNames = std::get<3>(parsedResultSynonym);
                }
            }
            // single synonym
            else {
                findResult = false; // only one result

                // check for attributes
                std::vector<std::string> resultVector = {};
                if (i + 1 < selectionToken.size() && selectionToken[i + 1].compare(".") == 0) {
                    if (i + 2 < selectionToken.size()) {
                        resultVector.push_back(token);
                        resultVector.push_back(selectionToken[i + 2]);
                        i += 2;
                    }
                    else {
                        std::cout << "Invalid select: Missing attrName\n";
                        return Query();
                    }
                }
                else {
                    // no attribute
                    resultVector.push_back(token);
                }

                std::tuple<bool, std::vector<std::string>, std::vector<ArgumentTypes>,
                    std::vector<AttrName>> parsedResultSynonym = QueryResultClauseParser::parseSingleSynonym(resultVector,
                        synonymNames, synonymTypes);

                if (!std::get<0>(parsedResultSynonym)) {
                    std::cout << "Invalid select: Error parsing result-cl\n";
                    return Query();
                }

                resultSynonyms = std::get<1>(parsedResultSynonym);
                resultTypes = std::get<2>(parsedResultSynonym);
                resultAttrNames = std::get<3>(parsedResultSynonym);
            }

            parsingCondition = true;

            // stop parsing after result-cl has been parsed, if semantically invalid
            if (!isDeclarationSemanticallyValid) {
                return Query(isBooleanResult); // syntactically valid, semantically invalid
            }
        }

        else if (parsingCondition) {
            if (parsingSuchThat) {
                std::string relation = token;
                RelRef relRef{};
            
                // find relationship
                relRef = QueryParserUtil::stringToRelRef(relation);
                if (relRef == RelRef::NONE) {
                    std::cout << relation << std::endl; // debug print
                    std::cout << "Invalid syntax: Unrecognised suchthat-cl\n";
                    return Query();
                }

                std::string firstArg = "";
                std::string secondArg = "";

                if ((i + 2) < selectionToken.size() && selectionToken[i + 1].compare("(") == 0) {
                    int counter = i + 2;
                    std::string curr = selectionToken[counter];

                    while (curr.compare(",") != 0) {
                        firstArg += curr;
                        counter++;

                        // check bounds
                        if (counter >= selectionToken.size()) {
                            std::cout << "Invalid syntax: Unrecognised suchthat-cl\n";
                            return Query();
                        }
                        else {
                            firstArg += " "; // separate tokens
                            curr = selectionToken[counter];
                        }
                    }
                    firstArg = firstArg.substr(0, firstArg.size() - 1); // remove last space

                    counter++;
                    curr = selectionToken[counter];

                    while (curr.compare(")") != 0) {
                        //if (curr.compare("(")) {
                        //    std::cout << "Invalid syntax: Unrecognised suchthat-cl\n";
                        //    return Query();
                        //}
                        secondArg += curr;
                        counter++;

                        // check bounds
                        if (counter >= selectionToken.size()) {
                            std::cout << "Invalid syntax: Unrecognised suchthat-cl\n";
                            return Query();
                        }
                        else {
                            secondArg += " "; // separate tokens
                            curr = selectionToken[counter];
                        }
                    }
                    secondArg = secondArg.substr(0, secondArg.size() - 1); // remove last space

                    QuerySuchThatClause suchThat = parseSuchThat(relRef, firstArg, secondArg, synonymNames, synonymTypes);
                    suchThatClauses.push_back(suchThat);
                    parsingSuchThat = false;
                    lastParsedClause = 1;
                    i = counter; // skip ")"
                }
                else {
                    std::cout << "Invalid syntax: Unrecognised suchthat-cl\n";
                    return Query();
                }
            }

            else if (parsingPattern) {
                std::string patternName = token;
                ArgumentTypes patternType = {};

                // verify pattern name and type
                auto patternNamePos = std::find(synonymNames.begin(), synonymNames.end(), patternName);
                bool synonymExists = patternNamePos != synonymNames.end();
                if (!synonymExists) {
                    std::cout << patternName << std::endl;
                    std::cout << "Invalid pattern: Pattern name does not exist\n";
                    return Query(isBooleanResult); // semantically invalid clause
                }

                int typeIndex = patternNamePos - synonymNames.begin();
                patternType = synonymTypes[typeIndex];

                if (!(patternType == ArgumentTypes::ASSIGN || patternType == ArgumentTypes::IF || patternType == ArgumentTypes::WHILE)) {
                    std::cout << "Invalid pattern: Invalid pattern type\n";
                    return Query(isBooleanResult); // invalid clause
                }
                else {
                    std::string firstArg = "";
                    std::string secondArg = "";

                    // extract first and second arg from tokens, separated by comma
                    if ((i + 2) < selectionToken.size() && selectionToken[i + 1].compare("(") == 0) {
                        int counter = i + 2;
                        std::string curr = selectionToken[counter];
                        while (curr.compare(",") != 0) {
                            firstArg += curr;
                            counter++;

                            // check bounds
                            if (counter >= selectionToken.size()) {
                                std::cout << "Invalid syntax: Unrecognised pattern-cl\n";
                                return Query();
                            }
                            else {
                                curr = selectionToken[counter];
                            }
                        }

                        counter++;
                        curr = selectionToken[counter];

                        // parse second and third arg for IF pattern
                        if (patternType == ArgumentTypes::IF) {
                            std::string thirdArg;

                            while (curr.compare(",") != 0) {
                                secondArg += curr;
                                counter++;

                                // check bounds
                                if (counter >= selectionToken.size()) {
                                    std::cout << "Invalid syntax: Unrecognised IF pattern-cl\n";
                                    return Query();
                                }
                                else {
                                    curr = selectionToken[counter];
                                }
                            }

                            counter++;
                            curr = selectionToken[counter];

                            while (curr.compare(")") != 0) {
                                thirdArg += curr;
                                counter++;

                                // check bounds
                                if (counter >= selectionToken.size()) {
                                    std::cout << "Invalid syntax: Unrecognised IF pattern-cl\n";
                                    return Query();
                                }
                                else {
                                    curr = selectionToken[counter];
                                }
                            }

                            if (secondArg.compare("_") != 0 || thirdArg.compare("_") != 0) {
                                std::cout << "Invalid syntax: Invalid arguments in IF pattern-cl\n";
                                return Query(isBooleanResult); // semantically invalid
                            }
                        }
                        // else parse second arg only for WHILE and ASSIGN pattern
                        else {
                            while (curr.compare(")") != 0) {
                                secondArg += curr;
                                counter++;

                                // check bounds
                                if (counter >= selectionToken.size()) {
                                    std::cout << "Invalid syntax: Unrecognised pattern-cl\n";
                                    return Query();
                                }
                                else {
                                    curr = selectionToken[counter];
                                }
                            }
                        }

                        QueryPatternClause patternCl = parsePattern(patternName, patternType, firstArg, secondArg, synonymNames, synonymTypes);
                        patternClauses.push_back(patternCl);

                        parsingPattern = false;
                        lastParsedClause = 2;
                        i = counter; // skip ")"
                    }
                    else {
                        std::cout << "Invalid syntax: Unrecognised pattern-cl\n";
                        return Query();
                    }

                }
            }

            else if (parsingWith) {
                std::string firstRef = "";
                std::string secondRef = "";

                // extract first and second ref
                int counter = i;
                std::string curr = selectionToken[counter];
                while (curr.compare("=") != 0) {
                    firstRef += curr;
                    counter++;

                    // check bounds
                    if (counter >= selectionToken.size()) {
                        std::cout << "Invalid syntax: Unrecognised with-cl\n";
                        return Query();
                    }
                    else {
                        firstRef += " "; // separate tokens
                        curr = selectionToken[counter];
                    }
                }
                firstRef = firstRef.substr(0, firstRef.size() - 1); // remove last space

                counter++;
                curr = selectionToken[counter];
                // check bounds
                if (counter >= selectionToken.size()) {
                    std::cout << "Invalid syntax: Unrecognised with-cl\n";
                    return Query();
                }

                secondRef += curr;
                if (counter + 1 < selectionToken.size() && selectionToken[counter + 1].compare(".") == 0) {
                    counter++;
                    secondRef += selectionToken[counter];
                    counter++;
                    if (counter < selectionToken.size()) {
                        secondRef += selectionToken[counter];
                    }
                    else {
                        std::cout << "Invalid syntax: Unrecognised with-cl\n";
                        return Query();
                    }
                }

                QueryWithClause withClause = parseWith(firstRef, secondRef, synonymNames, synonymTypes);
                withClauses.push_back(withClause);

                parsingWith = false;
                lastParsedClause = 3;
                i = counter;
            }
            else {
                // identifying if there are any conditions, and if so, what type of clause

                // suchthat-cl
                if (token.compare("such that") == 0) {
                    std::cout << "suchthat-cl exists" << std::endl; // debug print
                    parsingWith = false;
                    parsingPattern = false;
                    parsingSuchThat = true;
                }
                // pattern-cl
                else if (token.compare("pattern") == 0) {
                    std::cout << "pattern-cl exists" << std::endl; // debug print
                    parsingWith = false;
                    parsingPattern = true;
                    parsingSuchThat = false;
                }
                else if (token.compare("with") == 0) {
                    std::cout << "with-cl exists" << std::endl; // debug print
                    parsingWith = true;
                    parsingPattern = false;
                    parsingSuchThat = false;
                }
                else if (token.compare("and") == 0) {
                    // check last parsed condition
                    std::cout << "" << std::endl; // debug print
                    switch (lastParsedClause) {
                    case 1:
                        std::cout << "suchthat-cl exists" << std::endl; // debug print
                        parsingWith = false;
                        parsingPattern = false;
                        parsingSuchThat = true;
                        break;
                    case 2:
                        std::cout << "pattern-cl exists" << std::endl; // debug print
                        parsingWith = false;
                        parsingPattern = true;
                        parsingSuchThat = false;
                        break;
                    case 3:
                        std::cout << "with-cl exists" << std::endl; // debug print
                        parsingWith = true;
                        parsingPattern = false;
                        parsingSuchThat = false;
                        break;
                    default:
                        std::cout << "Invalid syntax: No clause before 'and' keyword" << std::endl;
                        return Query();
                    }
                }
                else {
                    std::cout << token << std::endl; // debug print
                    std::cout << "Invalid syntax: Unrecognised token in select-cl\n";
                    return Query();
                }
            }
        }
        else {
            std::cout << token << std::endl; // debug print
            std::cout << "Invalid syntax: Unrecognised token in select-cl\n";
            return Query();
        }
    }

    for (std::string name : resultSynonyms) {
        if (!LexicalSyntaxCheck::checkName(name)) {
            std::cout << "Invalid select: Result name wrong syntax\n";
            return Query();
        }
    }

    // syntax: check if there is result-cl
    if (resultSynonyms.size() == 0) {
        std::cout << "Invalid syntax: Missing result-cl" << std::endl;
        return Query();
    }

    // construct Query to be returned
    parsedQuery = Query(resultSynonyms, resultTypes, resultAttrNames,
        suchThatClauses, patternClauses, withClauses, isBooleanResult);

    return parsedQuery;
}

// Helper functions
QuerySuchThatClause QueryParser::parseSuchThat(RelRef relRef, std::string firstArg, std::string secondArg,
    std::vector<std::string> synonymNames, std::vector<ArgumentTypes> synonymTypes) {
    SuchThatClauseParser suchThatParser;
    QuerySuchThatClause suchThatClause = QuerySuchThatClause();

    if (relRef == RelRef::USES_S) {
        suchThatClause = suchThatParser.parseUses(relRef, firstArg, secondArg, synonymNames, synonymTypes);
    }
    else if (relRef == RelRef::MODIFIES_S) {
        suchThatClause = suchThatParser.parseModifies(relRef, firstArg, secondArg, synonymNames, synonymTypes);
    }
    else if (relRef == RelRef::FOLLOWS || relRef == RelRef::FOLLOWS_T) {
        suchThatClause = suchThatParser.parseFollows(relRef, firstArg, secondArg, synonymNames, synonymTypes);
    }
    else if (relRef == RelRef::PARENT || relRef == RelRef::PARENT_T) {
        suchThatClause = suchThatParser.parseParent(relRef, firstArg, secondArg, synonymNames, synonymTypes);
    }
    else if (relRef == RelRef::CALLS || relRef == RelRef::CALLS_T) {
        suchThatClause = suchThatParser.parseCalls(relRef, firstArg, secondArg, synonymNames, synonymTypes);
    }
    else if (relRef == RelRef::NEXT || relRef == RelRef::NEXT_T || relRef == RelRef::NEXT_BIP || relRef == RelRef::NEXT_BIP_T) {
        suchThatClause = suchThatParser.parseNext(relRef, firstArg, secondArg, synonymNames, synonymTypes);
    }
    else if (relRef == RelRef::AFFECTS || relRef == RelRef::AFFECTS_T || relRef == RelRef::AFFECTS_BIP || relRef == RelRef::AFFECTS_BIP_T) {
        suchThatClause = suchThatParser.parseAffects(relRef, firstArg, secondArg, synonymNames, synonymTypes);
    }
    else {
        std::cout << "Invalid suchthat: Unrecognised RelRef\n";
        return QuerySuchThatClause(); // invalid clause
    }
    
    std::cout << "Successfully parsed suchthat-cl" << std::endl; // debug print
    return suchThatClause;
}

QueryPatternClause QueryParser::parsePattern(std::string patternName, ArgumentTypes patternType, std::string firstArg, std::string secondArg,
    std::vector<std::string> synonymNames, std::vector<ArgumentTypes> synonymTypes) {
    PatternClauseParser parser;
    QueryPatternClause patternClause = parser.parsePattern(patternName, patternType, firstArg, secondArg, synonymNames, synonymTypes);

    std::cout << "Successfully parsed pattern-cl" << std::endl; // debug print
    return patternClause;
}

QueryWithClause QueryParser::parseWith(std::string firstRef, std::string secondRef,
    std::vector<std::string> synonymNames, std::vector<ArgumentTypes> synonymTypes) {
    WithClauseParser parser;
    QueryWithClause withClause = parser.parseWith(firstRef, secondRef, synonymNames, synonymTypes);

    std::cout << "Successfully parsed with-cl" << std::endl; // debug print
    return withClause;
}