#include "stdafx.h"

#include <stdio.h>
#include <string>
#include <unordered_map>
#include <unordered_set>

#include "CppUnitTest.h"
#include "DesignExtractor.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace IntegrationTesting
{
    TEST_CLASS(DERecursionTest1)
    {
    public:
        TEST_CLASS_INITIALIZE(hasRecursionTest1)
        {
            PKB::clear();

            PKB::addCall("A", "B", 3);
            PKB::addCall("A", "C", 2);
            PKB::addCall("B", "D", 6);
            PKB::addCall("B", "E", 7);
            PKB::addCall("D", "F", 8);
            PKB::addCall("G", "C", 11);
            PKB::addCall("C", "F", 17);

            
        }

        TEST_METHOD(recursionTest_noRecursion_ValidateReturnsTrue)
        {
            Assert::IsTrue(DesignExtractor::processProcedures());
        }

        TEST_CLASS_CLEANUP(ClearPKB) {
            PKB::clear();
        }
    };
}