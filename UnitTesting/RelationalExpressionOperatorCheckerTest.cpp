#include "stdafx.h"
#include "CppUnitTest.h"
#include "RelationalExpressionOperatorChecker.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{
	TEST_CLASS(RelationalExpressionOperatorCheckerTest)
	{
	public:
		
		TEST_METHOD(checkOperator_valid) {
			std::string VALID_OPERATOR_1 = ">";
			std::string VALID_OPERATOR_2 = "!=";
			std::string VALID_OPERATOR_3 = "==";
			Assert::IsTrue(RelationalExpressionOperatorChecker::checkOperator(VALID_OPERATOR_1));
			Assert::IsTrue(RelationalExpressionOperatorChecker::checkOperator(VALID_OPERATOR_2));
			Assert::IsTrue(RelationalExpressionOperatorChecker::checkOperator(VALID_OPERATOR_3));
		}

		TEST_METHOD(checkOperator_invalid_invalidOperator) {
			std::string INVALID_OPERATOR_1 = "+";
			std::string INVALID_OPERATOR_2 = "--";
			Assert::IsFalse(RelationalExpressionOperatorChecker::checkOperator(INVALID_OPERATOR_1));
			Assert::IsFalse(RelationalExpressionOperatorChecker::checkOperator(INVALID_OPERATOR_2));
		}

		TEST_METHOD(checkOperator_invalid_emptyOperator) {
			std::string INVALID_OPERATOR_EMPTY = "";
			Assert::IsFalse(RelationalExpressionOperatorChecker::checkOperator(INVALID_OPERATOR_EMPTY));
		}

	};
}
