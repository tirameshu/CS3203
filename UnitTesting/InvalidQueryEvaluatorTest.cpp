#include "stdafx.h"
#include "CppUnitTest.h"
#include "QueryEvaluator.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{
	TEST_CLASS(InvalidQueryEvaluatorTest)
	{
	public:
		TEST_METHOD(evaluateInvalidQuery) {
			std::unordered_set<std::string> expectedOutput;

			Query invalidQuery = Query();
			std::unordered_set<std::string> resultSet = QueryEvaluator::evaluateQuery(invalidQuery);

			Assert::IsTrue(expectedOutput == resultSet);
		}
	};
}