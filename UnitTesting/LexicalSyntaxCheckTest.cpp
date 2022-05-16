#include "stdafx.h"
#include "CppUnitTest.h"
#include "LexicalSyntaxCheck.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{
	TEST_CLASS(LexicalSyntaxCheckTest)
	{
	public:

		TEST_METHOD(checkLexicalSyntax_valid_variableName) {
			std::string VALID_VARIABLE_NAME_1 = "varname1";
			std::string VALID_VARIABLE_NAME_2 = "Vasd9uah12";
			Assert::IsTrue(LexicalSyntaxCheck::checkName(VALID_VARIABLE_NAME_1));
			Assert::IsTrue(LexicalSyntaxCheck::checkName(VALID_VARIABLE_NAME_2));
		}

		TEST_METHOD(checkLexicalSyntax_valid_integer) {
			std::string VALID_INTEGER_1 = "2";
			std::string VALID_INTEGER_2 = "0";
			Assert::IsTrue(LexicalSyntaxCheck::checkInteger(VALID_INTEGER_1));
			Assert::IsTrue(LexicalSyntaxCheck::checkInteger(VALID_INTEGER_2));
		}

		TEST_METHOD(checkLexicalSyntax_invalid_variableName) {
			std::string INVALID_VARIABLE_NAME_1 = "9Varname";
			std::string INVALID_VARIABLE_NAME_2 = "2asdas23";
			Assert::IsFalse(LexicalSyntaxCheck::checkName(INVALID_VARIABLE_NAME_1));
			Assert::IsFalse(LexicalSyntaxCheck::checkName(INVALID_VARIABLE_NAME_2));
		}

		TEST_METHOD(checkLexicalSyntax_invalid_integer) {
			std::string INVALID_INTEGER_1 = "A1321";
			std::string INVALID_INTEGER_2 = "0123";
			Assert::IsFalse(LexicalSyntaxCheck::checkInteger(INVALID_INTEGER_1));
			Assert::IsFalse(LexicalSyntaxCheck::checkInteger(INVALID_INTEGER_2));
		}

		TEST_METHOD(checkLexicalSyntaxl_invalid_empty) {
			std::string INVALID_EMPTY = "";
			Assert::IsFalse(LexicalSyntaxCheck::checkName(INVALID_EMPTY));
			Assert::IsFalse(LexicalSyntaxCheck::checkInteger(INVALID_EMPTY));
		}
	};
}
