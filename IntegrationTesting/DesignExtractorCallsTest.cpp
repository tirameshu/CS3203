#include "stdafx.h"
#include "CppUnitTest.h"
#include "DesignExtractor.h"
#include "DesignExtractorCallTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace IntegrationTesting
{
    TEST_CLASS(DesignExtractorCallTest)
    {
    public:
        TEST_CLASS_INITIALIZE(CallsT)
        {
            PKB::clear();

            // add calls, PKB will populate callee: caller map
            PKB::addCall("A", "B", 1);
            PKB::addCall("A", "C", 2);
            PKB::addCall("G", "C", 3);
            PKB::addCall("B", "D", 4);
            PKB::addCall("B", "E", 5);
            PKB::addCall("C", "F", 6);
            PKB::addCall("D", "F", 7);

            std::unordered_set<std::string> leaves = { "F", "E" };
            Assert::IsTrue(PKB::getLeafProcs() == leaves);

            std::unordered_set<std::string> roots = { "A", "G" };
            Assert::IsTrue(PKB::getRootProcs() == roots);

            Assert::IsTrue(DesignExtractor::processProgram());
            
        }

        //TEST_METHOD(computeAllCalleesTest_CalleesOfRoot_ReturnAllCallees)
        //{
        //    std::unordered_set<std::string> expectedResults = allCallees;
        //    std::unordered_set<std::string> actualResults = PKB::getTCalleesOfProc("A");

        //    Assert::IsTrue(expectedResults == actualResults);
        //}

        //TEST_METHOD(computeAllCallersTest_CallersOfLeaf_ReturnAllCallers)
        //{
        //    std::unordered_set<std::string> expectedResults = allCallers;
        //    std::unordered_set<std::string> actualResults = PKB::getTCallersOfProc("F");
        //    /*for (auto& x : actualResults) {
        //        Logger::WriteMessage(x.c_str());
        //    }*/

        //    Assert::IsTrue(expectedResults == actualResults);
        //}

        TEST_CLASS_CLEANUP(ClearPKB) 
        {
            PKB::clear();
        }
    };
}