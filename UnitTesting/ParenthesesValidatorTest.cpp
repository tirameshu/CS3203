#include "stdafx.h"
#include "CppUnitTest.h"
#include "ParenthesesValidator.h"
#include "ParserException.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{
	TEST_CLASS(ParenthesesValidatorTest)
	{
	public:

		TEST_METHOD(validateParentheses_valid) {
			std::string VALID_PARENTHESES_1 = "(a * b + c + (b - d))";
			std::string VALID_PARENTHESES_2 = "a + ((c - d))";
			Assert::IsTrue(ParenthesesValidator::validateParentheses(VALID_PARENTHESES_1));
			Assert::IsTrue(ParenthesesValidator::validateParentheses(VALID_PARENTHESES_2));
		}

		TEST_METHOD(validateParentheses_valid_empty) {
			std::string VALID_PARENTHESES_EMPTY = "";
			Assert::IsTrue(ParenthesesValidator::validateParentheses(VALID_PARENTHESES_EMPTY));
		}

		TEST_METHOD(validateParentheses_valid_noParentheses) {
			std::string VALID_PARENTHESES_NO_PARENTHESES = "a + c - d";
			Assert::IsTrue(ParenthesesValidator::validateParentheses(VALID_PARENTHESES_NO_PARENTHESES));
		}

		TEST_METHOD(validateParentheses_invalid) {
			std::string INVALID_PARENTHESES_1 = "a + c)";
			std::string INVALID_PARENTHESES_2 = "(aiusga + s";
			Assert::IsFalse(ParenthesesValidator::validateParentheses(INVALID_PARENTHESES_1));
			Assert::IsFalse(ParenthesesValidator::validateParentheses(INVALID_PARENTHESES_2));
		}
	};
}
