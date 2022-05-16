#include <vector>
#include <unordered_set>
#include <queue>

#include "stdafx.h"
#include "CppUnitTest.h"
#include "ConditionalExpressionExtractor.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{
	TEST_CLASS(ConditionalExpressionExtractorTest)
	{
	public:
		
		TEST_METHOD(extractConditional_valid1) {
			std::string CONDITIONAL_EXPRESSION = "NOtHs < 2";
			std::vector<std::unordered_set<std::string>> variableAndConstant = ConditionalExpressionExtractor::extractVariablesAndConstants(CONDITIONAL_EXPRESSION);

			std::unordered_set<std::string> expectedVariables;
			expectedVariables.insert("NOtHs");

			std::unordered_set<std::string> expectedConstants;
			expectedConstants.insert("2");

			Assert::IsTrue(variableAndConstant[0] == expectedVariables);
			Assert::IsTrue(variableAndConstant[1] == expectedConstants);
		}

		TEST_METHOD(extractConditional_valid2) {
			std::string CONDITIONAL_EXPRESSION2 = "!(a < 2) && (then == 3)";
			std::vector<std::unordered_set<std::string>> variableAndConstant = ConditionalExpressionExtractor::extractVariablesAndConstants(CONDITIONAL_EXPRESSION2);

			std::unordered_set<std::string> expectedVariables;
			expectedVariables.insert("a");
			expectedVariables.insert("then");

			std::unordered_set<std::string> expectedConstants;
			expectedConstants.insert("2");
			expectedConstants.insert("3");

			Assert::IsTrue(variableAndConstant[0] == expectedVariables);
			Assert::IsTrue(variableAndConstant[1] == expectedConstants);
		}

		TEST_METHOD(extractConditional_valid3) {
			std::string CONDITIONAL_EXPRESSION3 = "a < b + 4";
			std::vector<std::unordered_set<std::string>> variableAndConstant = ConditionalExpressionExtractor::extractVariablesAndConstants(CONDITIONAL_EXPRESSION3);

			std::unordered_set<std::string> expectedVariables;
			expectedVariables.insert("a");
			expectedVariables.insert("b");

			std::unordered_set<std::string> expectedConstants;
			expectedConstants.insert("4");

			Assert::IsTrue(variableAndConstant[0] == expectedVariables);
			Assert::IsTrue(variableAndConstant[1] == expectedConstants);
		}

		TEST_METHOD(extractConditional_valid4) {
			std::string CONDITIONAL_EXPRESSION4 = "2 > 5 + D";
			std::vector<std::unordered_set<std::string>> variableAndConstant = ConditionalExpressionExtractor::extractVariablesAndConstants(CONDITIONAL_EXPRESSION4);

			std::unordered_set<std::string> expectedVariables;
			expectedVariables.insert("D");

			std::unordered_set<std::string> expectedConstants;
			expectedConstants.insert("2");
			expectedConstants.insert("5");

			Assert::IsTrue(variableAndConstant[0] == expectedVariables);
			Assert::IsTrue(variableAndConstant[1] == expectedConstants);
		}
	};
}
