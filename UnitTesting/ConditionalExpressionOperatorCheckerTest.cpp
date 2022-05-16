#include "stdafx.h"
#include "CppUnitTest.h"
#include "ConditionalExpressionOperatorChecker.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{
	TEST_CLASS(ConditionalExpressionOperatorCheckerTest)
	{
	public:
		TEST_METHOD(checkOperator_valid) {
			std::string VALID_OPERATOR_1 = "&&";
			std::string VALID_OPERATOR_2 = "||";
			Assert::IsTrue(ConditionalExpressionOperatorChecker::checkOperator(VALID_OPERATOR_1));
			Assert::IsTrue(ConditionalExpressionOperatorChecker::checkOperator(VALID_OPERATOR_2));
		}

		TEST_METHOD(checkOperator_invalid_invalidOperator) {
			std::string INVALID_OPERATOR_1 = "+";
			std::string INVALID_OPERATOR_2 = "--";
			Assert::IsFalse(ConditionalExpressionOperatorChecker::checkOperator(INVALID_OPERATOR_1));
			Assert::IsFalse(ConditionalExpressionOperatorChecker::checkOperator(INVALID_OPERATOR_2));
		}

		TEST_METHOD(checkOperator_invalid_emptyOperator) {
			std::string INVALID_OPERATOR_EMPTY = "";
			Assert::IsFalse(ConditionalExpressionOperatorChecker::checkOperator(INVALID_OPERATOR_EMPTY));
		}
		
	};
}
