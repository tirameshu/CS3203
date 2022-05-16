#include <vector>
#include <unordered_set>
#include <queue>

#include "stdafx.h"
#include "CppUnitTest.h"
#include "StandardExpressionExtractor.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{
	TEST_CLASS(StandardExpressionExtractorTest)
	{
	public:

		TEST_METHOD(extractExpression_valid_standard) {
			std::string EXPRESSION = "a = a + c + d + sd - 23";
			std::vector<std::unordered_set<std::string>> variablesAndConstants 
				= StandardExpressionExtractor::extractVariablesAndConstants(EXPRESSION);

			std::unordered_set<std::string> setOfConstants;
			std::unordered_set<std::string> setOfVariables;
			setOfVariables.insert("a");
			setOfVariables.insert("c");
			setOfVariables.insert("d");
			setOfVariables.insert("sd");
			setOfConstants.insert("23");

			Assert::IsTrue(setOfVariables == variablesAndConstants[0]);
			Assert::IsTrue(setOfConstants == variablesAndConstants[1]);
		}

		TEST_METHOD(extractExpression_valid_noSpace) {
			std::string EXPRESSION2 = "b=asa-23";
			std::vector<std::unordered_set<std::string>> variablesAndConstants
				= StandardExpressionExtractor::extractVariablesAndConstants(EXPRESSION2);

			std::unordered_set<std::string> setOfConstants;
			std::unordered_set<std::string> setOfVariables;
			setOfVariables.insert("asa");
			setOfConstants.insert("23");

			Assert::IsTrue(setOfVariables == variablesAndConstants[0]);
			Assert::IsTrue(setOfConstants == variablesAndConstants[1]);
		}

		TEST_METHOD(extractExpression_valid_onlyConstants) {
			std::string EXPRESSION3 = "d = 1 * 23 - 24";
			std::vector<std::unordered_set<std::string>> variablesAndConstants
				= StandardExpressionExtractor::extractVariablesAndConstants(EXPRESSION3);

			std::unordered_set<std::string> setOfConstants;
			std::unordered_set<std::string> setOfVariables;
			setOfConstants.insert("1");
			setOfConstants.insert("23");
			setOfConstants.insert("24");

			Assert::IsTrue(setOfVariables == variablesAndConstants[0]);
			Assert::IsTrue(setOfConstants == variablesAndConstants[1]);
		}

		TEST_METHOD(extractExpression_valid_allSameVariable) {
			std::string EXPRESSION4 = "e = a + a + a + a";
			std::vector<std::unordered_set<std::string>> variablesAndConstants
				= StandardExpressionExtractor::extractVariablesAndConstants(EXPRESSION4);

			std::unordered_set<std::string> setOfConstants;
			std::unordered_set<std::string> setOfVariables;
			setOfVariables.insert("a");

			Assert::IsTrue(setOfVariables == variablesAndConstants[0]);
			Assert::IsTrue(setOfConstants == variablesAndConstants[1]);
		}

		TEST_METHOD(extractExpression_valid_onlyOneOperand) {
			std::string EXPRESSION5 = "asds = superCaliFragilisticExpiallydocious";
			std::vector<std::unordered_set<std::string>> variablesAndConstants
				= StandardExpressionExtractor::extractVariablesAndConstants(EXPRESSION5);

			std::unordered_set<std::string> setOfConstants;
			std::unordered_set<std::string> setOfVariables;
			setOfVariables.insert("superCaliFragilisticExpiallydocious");

			Assert::IsTrue(setOfVariables == variablesAndConstants[0]);
			Assert::IsTrue(setOfConstants == variablesAndConstants[1]);
		}
	};
}
