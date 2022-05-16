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
    TEST_CLASS(DERecursionTest2)
    {
    public:
        TEST_CLASS_INITIALIZE(hasRecursionTest2)
        {
            PKB::clear();

            PKB::addCall("A", "B", 3);
            PKB::addCall("A", "C", 2);
            PKB::addCall("B", "D", 6);
            PKB::addCall("B", "A", 7);
            PKB::addCall("D", "F", 8);
            PKB::addCall("G", "C", 11);
            PKB::addCall("C", "F", 17);

        }

        TEST_METHOD(recursionTest_hasRecursion_ValidateReturnsFalse)
        {
            auto exceptionFunction = [&] {
                DesignExtractor::validateProcedures();
            };
            Assert::ExpectException<ParserException>(exceptionFunction);
        }

        TEST_CLASS_CLEANUP(ClearPKB) {
            PKB::clear();
        }
    };
}