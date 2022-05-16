#include <iostream>
#include <stack>

#include "ExpressionTokenizer.h"
#include "ParserShuntingConverter.h"
#include "StandardPostfixEvaluator.h"
#include "PatternClauseParser.h"
#include "QueryParserUtil.h"
#include "LexicalSyntaxCheck.h"
#include "ParserException.h"

QueryPatternClause PatternClauseParser::parsePattern(std::string patternName, ArgumentTypes patternType, std::string firstArg, std::string secondArg,
    std::vector<std::string> synonymNames, std::vector<ArgumentTypes> synonymTypes) {
    ArgumentTypes firstType{};
    ArgumentTypes secondType{};

    if (patternType == ArgumentTypes::ASSIGN) {
        // PARSE FIRST ARGUMENT
        if (firstArg[0] == '"' && firstArg[firstArg.size() - 1] == '"') {
            std::string trimmedFirstArg = QueryParserUtil::trim(firstArg.substr(1, firstArg.size() - 2));
            if (LexicalSyntaxCheck::checkName(trimmedFirstArg)) {
                firstType = ArgumentTypes::STRING;
                firstArg = QueryParserUtil::trim(firstArg.substr(1, firstArg.size() - 2)); // remove quotation marks
            }
            else {
                std::cout << "Invalid pattern: Syntactically invalid type\n";
                return QueryPatternClause(); // invalid clause
            }
        }
        else if (firstArg.compare("_") == 0) {
            firstType = ArgumentTypes::ANY;
        }
        // synonym
        else {
            auto namePos = std::find(synonymNames.begin(), synonymNames.end(), firstArg);
            bool synonymExists = namePos != synonymNames.end();
            if (!synonymExists) {
                std::cout << firstArg << std::endl;
                std::cout << "Invalid pattern: Synonym does not exist\n";
                return QueryPatternClause(false); // invalid clause
            }

            int typeIndex = namePos - synonymNames.begin();
            firstType = synonymTypes[typeIndex];
            if (firstType != ArgumentTypes::VARIABLE) {
                std::cout << "Invalid pattern: Semantically invalid type\n";
                return QueryPatternClause(false); // invalid clause
            }
        }

        // PARSE SECOND ARGUMENT
        bool isExact = false;
        secondArg = QueryParserUtil::trim(secondArg);
        if (secondArg.compare("_") == 0) {
            secondType = ArgumentTypes::ANY;
        }
        else if (secondArg[0] == '_' && secondArg[secondArg.size() - 1] == '_') {
            isExact = false;
            std::string withoutUnderscoreSecondArg = QueryParserUtil::trim(secondArg.substr(1, secondArg.size() - 2));
            if (withoutUnderscoreSecondArg[0] == '"' && withoutUnderscoreSecondArg[withoutUnderscoreSecondArg.size() - 1] == '"') {
                std::string trimmedSecondArg = QueryParserUtil::trim(withoutUnderscoreSecondArg.substr(1, withoutUnderscoreSecondArg.size() - 2));
                secondArg = trimmedSecondArg;
                secondType = ArgumentTypes::STRING;

                // check whether valid expr

                try {
                    /*=== TOKENIZE ===*/
                    std::vector<std::string> expressionTokens = ExpressionTokenizer::tokenize(secondArg);
                    std::queue<std::string> queueOfOutputs = ParserShuntingConverter::convert(expressionTokens, secondArg, false);

                    // Validate the correctness of the postfix
                    bool isValidExpression = StandardPostfixEvaluator::evaluateStandardPostfix(queueOfOutputs);
                    if (!isValidExpression) {
                        std::cout << secondArg << std::endl; // debug print
                        std::cout << "Invalid assign-pattern: Syntactically invalid expression\n";
                        return QueryPatternClause();
                    } else {
                        // extract the postfix expression with spaces
                        std::string postfixExpression;
                        while (!queueOfOutputs.empty()) {
                            postfixExpression += queueOfOutputs.front() + " ";
                            queueOfOutputs.pop();
                        }
                        secondArg = QueryParserUtil::trim(postfixExpression);
                    }
                }
                catch (ParserException& e) {
                    std::cout << e.getExceptionMessage();
                    std::cout << secondArg << std::endl; // debug print
                    std::cout << "Invalid assign-pattern: Invalid expression\n";
                    return QueryPatternClause();
                }

            }
            else {
                std::cout << "Invalid assign-pattern: Invalid syntax for expression-spec\n";
                return QueryPatternClause();
            }
        }
        else if (secondArg[0] == '"' && secondArg[secondArg.size() - 1] == '"') {
            isExact = true;
            std::string trimmedSecondArg = QueryParserUtil::trim(secondArg.substr(1, secondArg.size() - 2));
            secondArg = trimmedSecondArg;
            secondType = ArgumentTypes::STRING;

            // check whether valid expr
            try {
                /*=== TOKENIZE ===*/
                std::vector<std::string> expressionTokens = ExpressionTokenizer::tokenize(secondArg);
                std::queue<std::string> queueOfOutputs = ParserShuntingConverter::convert(expressionTokens, secondArg, false);

                // Validate the correctness of the postfix
                bool isValidExpression = StandardPostfixEvaluator::evaluateStandardPostfix(queueOfOutputs);
                if (!isValidExpression) {
                    std::cout << secondArg << std::endl; // debug print
                    std::cout << "Invalid assign-pattern: Invalid expression\n";
                    return QueryPatternClause();
                }
                else {
                    // extract the postfix expression with spaces
                    std::string postfixExpression;
                    while (!queueOfOutputs.empty()) {
                        postfixExpression += queueOfOutputs.front() + " ";
                        queueOfOutputs.pop();
                    }
                    secondArg = QueryParserUtil::trim(postfixExpression);
                }
            }
            catch (ParserException& e) {
                std::cout << e.getExceptionMessage();
                std::cout << secondArg << std::endl; // debug print
                std::cout << "Invalid assign-pattern: Invalid expression\n";
                return QueryPatternClause();
            }
        }
        else {
            // invalid
            std::cout << "Invalid pattern: Unrecognised argument\n";
            return QueryPatternClause();
        }

        return QueryPatternClause(patternName, firstArg, secondArg, patternType, firstType, secondType, isExact);
    }
    else if (patternType == ArgumentTypes::WHILE) {
        // PARSE FIRST ARGUMENT
        if (firstArg[0] == '"' && firstArg[firstArg.size() - 1] == '"') {
            std::string trimmedFirstArg = QueryParserUtil::trim(firstArg.substr(1, firstArg.size() - 2));
            if (LexicalSyntaxCheck::checkName(trimmedFirstArg)) {
                firstType = ArgumentTypes::STRING;
                firstArg = QueryParserUtil::trim(firstArg.substr(1, firstArg.size() - 2)); // remove quotation marks
            }
            else {
                std::cout << "Invalid pattern: Syntactically invalid type\n";
                return QueryPatternClause(); // invalid clause
            }
        }
        else if (firstArg.compare("_") == 0) {
            firstType = ArgumentTypes::ANY;
        }
        // synonym
        else {
            auto namePos = std::find(synonymNames.begin(), synonymNames.end(), firstArg);
            bool synonymExists = namePos != synonymNames.end();
            if (!synonymExists) {
                std::cout << firstArg << std::endl;
                std::cout << "Invalid pattern: Synonym does not exist\n";
                return QueryPatternClause(false); // invalid clause
            }

            int typeIndex = namePos - synonymNames.begin();
            firstType = synonymTypes[typeIndex];
            if (firstType != ArgumentTypes::VARIABLE) {
                std::cout << "Invalid pattern: Semantically invalid type\n";
                return QueryPatternClause(false); // invalid clause
            }
        }

        // PARSE SECOND ARGUMENT
        secondArg = QueryParserUtil::trim(secondArg);
        if (secondArg.compare("_") == 0) {
            secondType = ArgumentTypes::ANY;
        }
        else {
            std::cout << "Invalid while-pattern: Syntactically invalid type\n";
            return QueryPatternClause(); // invalid clause
        }

        return QueryPatternClause(patternName, firstArg, secondArg, patternType, firstType, secondType);
    }
    else if (patternType == ArgumentTypes::IF) {
        // PARSE FIRST ARGUMENT
        if (firstArg[0] == '"' && firstArg[firstArg.size() - 1] == '"') {
            std::string trimmedFirstArg = firstArg.substr(1, firstArg.size() - 2);
            trimmedFirstArg = QueryParserUtil::trim(trimmedFirstArg);
            if (LexicalSyntaxCheck::checkName(trimmedFirstArg)) {
                firstType = ArgumentTypes::STRING;
                firstArg = firstArg.substr(1, firstArg.size() - 2); // remove quotation marks
                firstArg = QueryParserUtil::trim(firstArg);
            }
            else {
                std::cout << "Invalid pattern: Syntactically invalid type\n";
                return QueryPatternClause(); // invalid clause
            }
        }
        else if (firstArg.compare("_") == 0) {
            firstType = ArgumentTypes::ANY;
        }
        // synonym
        else {
            auto namePos = std::find(synonymNames.begin(), synonymNames.end(), firstArg);
            bool synonymExists = namePos != synonymNames.end();
            if (!synonymExists) {
                std::cout << firstArg << std::endl;
                std::cout << "Invalid pattern: Synonym does not exist\n";
                return QueryPatternClause(false); // invalid clause
            }

            int typeIndex = namePos - synonymNames.begin();
            firstType = synonymTypes[typeIndex];
            if (firstType != ArgumentTypes::VARIABLE) {
                std::cout << "Invalid pattern: Semantically invalid type\n";
                return QueryPatternClause(false); // invalid clause
            }
        }

        // PARSE SECOND ARGUMENT
        secondArg = QueryParserUtil::trim(secondArg);
        if (secondArg.compare("_") == 0) {
            secondType = ArgumentTypes::ANY;
        }
        else {
            std::cout << "Invalid if-pattern: Syntacticallyu invalid type\n";
            return QueryPatternClause(); // invalid clause
        }

        return QueryPatternClause(patternName, firstArg, secondArg, patternType, firstType, secondType);
    }
    else {
        std::cout << QueryParserUtil::argumentTypesToString(patternType) << std::endl;
        std::cout << "Invalid pattern: Invalid pattern type\n";
        return QueryPatternClause(false); // invalid clause
    }
}