#include "stdafx.h"
#include "CppUnitTest.h"
#include "DesignExtractorTestData.h"

std::unordered_map<std::string, std::unordered_set<std::string>> DesignExtractorTestClass::procToCallers;
std::unordered_map<std::string, std::unordered_set<std::string>> DesignExtractorTestClass::procToCallees;
std::unordered_map<std::string, std::unordered_set<std::string>> DesignExtractorTestClass::procToAllCallees;
std::unordered_map<std::string, std::unordered_set<std::string>> DesignExtractorTestClass::procToAllCallers;

std::unordered_set<std::string> DesignExtractorTestClass::allCallees = { "B", "C", "D", "E", "F" };
std::unordered_set<std::string> DesignExtractorTestClass::allCallers = { "A", "B", "C", "D", "G" };

void DesignExtractorTestClass::computeAllCallees(std::string procedure) {
    // A -> B -> E
    // A -> B -> D -> F
    // A -> C -> F
    // G -> C -> F

    // Starting at F
    std::unordered_set<std::string> allCallees;

    std::unordered_set<std::string> directCallees = procToCallees.at(procedure);
    std::unordered_set<std::string> directCallers = procToCallers.at(procedure);

    for (auto& callee : directCallees) {
        std::unordered_set<std::string> calleesOfCallee = procToAllCallees.at(callee);
        allCallees.insert(calleesOfCallee.begin(), calleesOfCallee.end()); // "union": range insert
        allCallees.insert(callee);
    }
    procToAllCallees.at(procedure).insert(allCallees.begin(), allCallees.end());

    if (directCallers.size() != 0) {
        for (auto& caller : directCallers) {
            DesignExtractorTestClass::computeAllCallees(caller);
        }
    }

}

void DesignExtractorTestClass::computeAllCallers(std::string procedure) {
    // A -> B -> E
    // A -> B -> D -> F
    // A -> C -> F
    // G -> C -> F

    // Starting at A
    std::unordered_set<std::string> allCallers;

    std::unordered_set<std::string> directCallers = procToCallers.at(procedure);
    std::unordered_set<std::string> directCallees = procToCallees.at(procedure);

    for (auto& caller : directCallers) {
        std::unordered_set<std::string> callersOfCaller = procToAllCallers.at(caller);
        allCallers.insert(callersOfCaller.begin(), callersOfCaller.end()); // "union": range insert
        allCallers.insert(caller);
    }
    procToAllCallers.at(procedure).insert(allCallers.begin(), allCallers.end());

    if (directCallees.size() != 0) {
        for (auto& callee : directCallees) {
            DesignExtractorTestClass::computeAllCallers(callee);
        }
    }

}