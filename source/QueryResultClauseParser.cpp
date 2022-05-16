#include "QueryResultClauseParser.h"
#include "QueryParserUtil.h"

std::tuple<bool, std::vector<std::string>, std::vector<ArgumentTypes>, std::vector<AttrName>> QueryResultClauseParser::parseTuple(std::vector<std::string> selectionToken,
    std::vector<std::string> synonymNames, std::vector<ArgumentTypes> synonymTypes) {

    std::vector<std::string> resultSynonyms = {};
    std::vector<ArgumentTypes> resultTypes = {};
    std::vector<AttrName> resultAttrNames = {};

    // to be returned if invalid syntax
    std::tuple<bool, std::vector<std::string>, std::vector<ArgumentTypes>, std::vector<AttrName>> invalid = std::make_tuple(false,
        resultSynonyms, resultTypes, resultAttrNames);

    for (int j = 0; j < selectionToken.size(); j++) {
        std::vector<std::string> elem = {};
        std::string name = selectionToken[j];
        elem.push_back(name);

        if (j + 1 < selectionToken.size() && selectionToken[j + 1].compare(".") == 0) {
            if (j + 2 < selectionToken.size()) {
                elem.push_back(selectionToken[j + 2]);
                j += 2;
            }
            else {
                std::cout << "Invalid select: Missing attrName\n";
                return invalid;
            }
        }

        // extract indiv synonym and parse each as single synonym
        std::tuple<bool, std::vector<std::string>, std::vector<ArgumentTypes>,
            std::vector<AttrName>> curr = parseSingleSynonym(elem, synonymNames, synonymTypes);

        if (!std::get<0>(curr)) {
            std::cout << "Invalid select: Error parsing tuple result-cl\n";
            return invalid;
        }
        else {
            resultSynonyms.push_back(std::get<1>(curr)[0]);
            resultTypes.push_back(std::get<2>(curr)[0]);
            resultAttrNames.push_back(std::get<3>(curr)[0]);
        }

        // check syntactic validity: ensure there is a comma between each elem in tuple
        if (j + 1 < selectionToken.size()) {
            if (selectionToken[j + 1].compare(",") != 0) {
                std::cout << "Invalid select: Missing comma in tuple\n";
                return invalid;
            }
            j++;
        }
    }

    return std::make_tuple(true, resultSynonyms, resultTypes, resultAttrNames);
}

std::tuple<bool, std::vector<std::string>, std::vector<ArgumentTypes>, std::vector<AttrName>> QueryResultClauseParser::parseSingleSynonym(std::vector<std::string> selectionToken,
    std::vector<std::string> synonymNames, std::vector<ArgumentTypes> synonymTypes) {
    
    std::vector<std::string> resultSynonyms = {};
    std::vector<ArgumentTypes> resultTypes = {};
    std::vector<AttrName> resultAttrNames = {};

    // to be returned if invalid syntax
    std::tuple<bool, std::vector<std::string>, std::vector<ArgumentTypes>, std::vector<AttrName>> invalid = std::make_tuple(false,
        resultSynonyms, resultTypes, resultAttrNames);

    std::string name = selectionToken[0];
    std::pair<bool, ArgumentTypes> checkSynonym = QueryParserUtil::checkNameExists(synonymNames, synonymTypes, name);
    if (!checkSynonym.first) {
        std::cout << "Invalid select: Synonym does not exist\n";
        return invalid;
    }

    ArgumentTypes synonymType = checkSynonym.second;
    resultSynonyms.push_back(name);
    resultTypes.push_back(synonymType);

    // check for attributes
    if (selectionToken.size() == 2) {
        std::string attrString = selectionToken[1];
        AttrName attr = QueryParserUtil::stringToAttribute(attrString);

        if (attr == AttrName::NONE) {
            std::cout << "Invalid select: Unrecognised attrName\n";
            return invalid;
        }

        bool isValidAttrType = QueryParserUtil::validateAttrType(synonymType, attr);
        if (!isValidAttrType) {
            std::cout << "Invalid select: Invalid attrRef\n";
            return invalid;
        }
        else {
            resultAttrNames.push_back(attr);
        }
    }
    else if (selectionToken.size() == 1) {
        // no attribute
        resultAttrNames.push_back(AttrName::NONE);
    }
    else {
        std::cout << "Bug: Error in result subvector\n";
    }

    return std::make_tuple(true, resultSynonyms, resultTypes, resultAttrNames);
}