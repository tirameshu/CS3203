#include "stdafx.h"
#include "CppUnitTest.h"
#include "StatementTypeEnum.h"
#include "AssignProcessor.h"
#include "ParserException.h"
#include "PKB.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{
	TEST_CLASS(AssignProcessorTest)
	{
	public:
		std::string PROCEDURE_NAME = "procedureSample";
		int STATEMENT_NUMBER = 1;

		TEST_CLASS_INITIALIZE(assignProcessorTest_initializePkb) {
			PKB::clear();
		}

		TEST_METHOD(processAssign_valid_standard) {
			std::string input = "a = b + c";

			Assert::IsTrue(AssignProcessor::processAssign(input, PROCEDURE_NAME, STATEMENT_NUMBER));
		}

		TEST_METHOD(processAssign_valid_nospace) {
			std::string input = "a=b*c/2";

			Assert::IsTrue(AssignProcessor::processAssign(input, PROCEDURE_NAME, STATEMENT_NUMBER));
		}

		TEST_METHOD(processAssign_valid_multipleLetterOperand) {
			std::string input = "aasdagd=128736127386-127362173+asdgasd-1236*q837";

			Assert::IsTrue(AssignProcessor::processAssign(input, PROCEDURE_NAME, STATEMENT_NUMBER));
		}

		TEST_METHOD(processAssign_valid_onlyConstants) {
			std::string input = "asd=123-123+23-52";

			Assert::IsTrue(AssignProcessor::processAssign(input, PROCEDURE_NAME, STATEMENT_NUMBER));
		}

		TEST_METHOD(processAssign_valid_onlyOneVariable) {
			std::string input = "asd=a";

			Assert::IsTrue(AssignProcessor::processAssign(input, PROCEDURE_NAME, STATEMENT_NUMBER));
		}

		TEST_METHOD(processAssign_valid_parentheses) {
			std::string input = "asd=(((((d)))))* (23 + 25)";

			Assert::IsTrue(AssignProcessor::processAssign(input, PROCEDURE_NAME, STATEMENT_NUMBER));
		}

		TEST_METHOD(processAssign_valid_onlyOneConstant) {
			std::string input = "b=3";

			Assert::IsTrue(AssignProcessor::processAssign(input, PROCEDURE_NAME, STATEMENT_NUMBER));
		}

		TEST_METHOD(processAssign_valid_trailingWhiteSpace) {
			std::string input = "b=3                              ";

			Assert::IsTrue(AssignProcessor::processAssign(input, PROCEDURE_NAME, STATEMENT_NUMBER));
		}

		TEST_METHOD(processAssign_valid_leadingWhiteSpace) {
			std::string input = "                            b=3";

			Assert::IsTrue(AssignProcessor::processAssign(input, PROCEDURE_NAME, STATEMENT_NUMBER));
		}

		TEST_METHOD(processAssign_valid_extraWhiteSpaces) {
			std::string input = "                            b               =               3                  ";

			Assert::IsTrue(AssignProcessor::processAssign(input, PROCEDURE_NAME, STATEMENT_NUMBER));
		}

		TEST_METHOD(processAssign_invalid_noExpression) {
			std::string input = "basd =";

			auto exception = [&] {
				AssignProcessor::processAssign(input, PROCEDURE_NAME, STATEMENT_NUMBER);
			};

			Assert::ExpectException<ParserException>(exception);
		}

		TEST_METHOD(processAssign_invalid_mismatchedParentheses) {
			std::string input = "basd = ((adsadh - 23)";

			auto exception = [&] {
				AssignProcessor::processAssign(input, PROCEDURE_NAME, STATEMENT_NUMBER);
			};

			Assert::ExpectException<ParserException>(exception);
		}

		TEST_METHOD(processAssign_invalid_invalidParentheses) {
			std::string input = "basd = )super - (2 + 3)(";

			auto exception = [&] {
				AssignProcessor::processAssign(input, PROCEDURE_NAME, STATEMENT_NUMBER);
			};

			Assert::ExpectException<ParserException>(exception);
		}

		TEST_METHOD(processAssign_invalid_multipleModified) {
			std::string input = "a + d = super * epic";

			auto exception = [&] {
				AssignProcessor::processAssign(input, PROCEDURE_NAME, STATEMENT_NUMBER);
			};

			Assert::ExpectException<ParserException>(exception);
		}

		TEST_METHOD(processAssign_invalid_relationalExpressionExist) {
			std::string input = "a = b < d";

			auto exception = [&] {
				AssignProcessor::processAssign(input, PROCEDURE_NAME, STATEMENT_NUMBER);
			};

			Assert::ExpectException<ParserException>(exception);
		}

		TEST_METHOD(processAssign_invalid_conditionalExpressionExist) {
			std::string input = "b = (a) && (b)";

			auto exception = [&] {
				AssignProcessor::processAssign(input, PROCEDURE_NAME, STATEMENT_NUMBER);
			};

			Assert::ExpectException<ParserException>(exception);
		}
	};
}
