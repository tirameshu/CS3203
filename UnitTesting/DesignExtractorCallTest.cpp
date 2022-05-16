#include "stdafx.h"
#include "CppUnitTest.h"
#include "DesignExtractorTestData.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{
    TEST_CLASS(DesignExtractorCallTest)
    {
    public:
        TEST_CLASS_INITIALIZE(CallsT)
        {
            // callers
            std::unordered_set<std::string> callersA = {};
            std::unordered_set<std::string> callersB = { "A" };
            std::unordered_set<std::string> callersC = { "A", "G" };
            std::unordered_set<std::string> callersD = { "B" };
            std::unordered_set<std::string> callersE = { "B" };
            std::unordered_set<std::string> callersF = { "C", "D" };
            std::unordered_set<std::string> callersG = {};

            DesignExtractorTestClass::procToCallers.emplace("A", callersA);
            DesignExtractorTestClass::procToCallers.emplace("B", callersB);
            DesignExtractorTestClass::procToCallers.emplace("C", callersC);
            DesignExtractorTestClass::procToCallers.emplace("D", callersD);
            DesignExtractorTestClass::procToCallers.emplace("E", callersE);
            DesignExtractorTestClass::procToCallers.emplace("F", callersF);
            DesignExtractorTestClass::procToCallers.emplace("G", callersG);

            // callees
            std::unordered_set<std::string> directCalleesA = { "B", "C" };
            std::unordered_set<std::string> directCalleesB = { "D", "E" };
            std::unordered_set<std::string> directCalleesC = { "F" };
            std::unordered_set<std::string> directCalleesD = { "F" };
            std::unordered_set<std::string> directCalleesE = {};
            std::unordered_set<std::string> directCalleesF = {};
            std::unordered_set<std::string> directCalleesG = { "C" };

            DesignExtractorTestClass::procToCallees.emplace("A", directCalleesA);
            DesignExtractorTestClass::procToCallees.emplace("B", directCalleesB);
            DesignExtractorTestClass::procToCallees.emplace("C", directCalleesC);
            DesignExtractorTestClass::procToCallees.emplace("D", directCalleesD);
            DesignExtractorTestClass::procToCallees.emplace("E", directCalleesE);
            DesignExtractorTestClass::procToCallees.emplace("F", directCalleesF);
            DesignExtractorTestClass::procToCallees.emplace("G", directCalleesG);

            // all callees
            std::unordered_set<std::string> emptySet = {};

            DesignExtractorTestClass::procToAllCallees.emplace("A", emptySet);
            DesignExtractorTestClass::procToAllCallees.emplace("B", emptySet);
            DesignExtractorTestClass::procToAllCallees.emplace("C", emptySet);
            DesignExtractorTestClass::procToAllCallees.emplace("D", emptySet);
            DesignExtractorTestClass::procToAllCallees.emplace("E", emptySet);
            DesignExtractorTestClass::procToAllCallees.emplace("F", emptySet);
            DesignExtractorTestClass::procToAllCallees.emplace("G", emptySet);

            // all callers
            DesignExtractorTestClass::procToAllCallers.emplace("A", emptySet);
            DesignExtractorTestClass::procToAllCallers.emplace("B", emptySet);
            DesignExtractorTestClass::procToAllCallers.emplace("C", emptySet);
            DesignExtractorTestClass::procToAllCallers.emplace("D", emptySet);
            DesignExtractorTestClass::procToAllCallers.emplace("E", emptySet);
            DesignExtractorTestClass::procToAllCallers.emplace("F", emptySet);
            DesignExtractorTestClass::procToAllCallers.emplace("G", emptySet);
            
            DesignExtractorTestClass::computeAllCallees("F");
            DesignExtractorTestClass::computeAllCallers("A");
        }

        TEST_METHOD(computeAllCalleesTest_CalleesOfRoot_ReturnAllCallees)
        {
            std::unordered_set<std::string> expectedResults = DesignExtractorTestClass::allCallees;
            std::unordered_set<std::string> actualResults = DesignExtractorTestClass::procToAllCallees.at("A");
            //for (auto& x : actualResults) {
            //    Logger::WriteMessage(x.c_str());
            //}
            
            Assert::IsTrue(expectedResults == actualResults);
        }

        TEST_METHOD(computeAllCallersTest_CallersOfLeaf_ReturnAllCallers)
        {
            std::unordered_set<std::string> expectedResults = DesignExtractorTestClass::allCallers;
            std::unordered_set<std::string> actualResults = DesignExtractorTestClass::procToAllCallers.at("F");
            //for (auto& x : actualResults) {
            //    Logger::WriteMessage(x.c_str());
            //}

            Assert::IsTrue(expectedResults == actualResults);
        }
    };
}