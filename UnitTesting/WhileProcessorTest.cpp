#include "stdafx.h"
#include "CppUnitTest.h"
#include "StatementTypeEnum.h"
#include "WhileProcessor.h"
#include "ParserException.h"
#include "PKB.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{
	TEST_CLASS(WhileProcessorTest)
	{
	public:
		std::string PROCEDURE_NAME = "procedureSample";
		int STATEMENT_NUMBER = 1;

		TEST_CLASS_INITIALIZE(whileProcessorTest_initializePkb) {
			PKB::clear();
		}

		TEST_METHOD(processWhile_valid) {
			std::string VALID_WHILE_STATEMENT = "while(now + this  == epic + lmao)";
			std::string VALID_WHILE_STATEMENT2 = "while (cool < epic)                ";
			std::string VALID_WHILE_STATEMENT3 = "while ((((nice == epic))))";
			std::string VALID_WHILE_STATEMENT4 = "while ((a > b) ||(2 < g))";
			std::string VALID_WHILE_STATEMENT5 = "while (a + b > s)";
			std::string VALID_WHILE_STATEMENT6 = "while ((a + b == 3))";
			std::string VALID_WHILE_STATEMENT7 = "while (asd1231232 + 2131232 == alidhas3)";

			Assert::IsTrue(WhileProcessor::processWhile(VALID_WHILE_STATEMENT, PROCEDURE_NAME, STATEMENT_NUMBER));
			Assert::IsTrue(WhileProcessor::processWhile(VALID_WHILE_STATEMENT2, PROCEDURE_NAME, STATEMENT_NUMBER));
			Assert::IsTrue(WhileProcessor::processWhile(VALID_WHILE_STATEMENT3, PROCEDURE_NAME, STATEMENT_NUMBER));
			Assert::IsTrue(WhileProcessor::processWhile(VALID_WHILE_STATEMENT4, PROCEDURE_NAME, STATEMENT_NUMBER));
			Assert::IsTrue(WhileProcessor::processWhile(VALID_WHILE_STATEMENT5, PROCEDURE_NAME, STATEMENT_NUMBER));
			Assert::IsTrue(WhileProcessor::processWhile(VALID_WHILE_STATEMENT6, PROCEDURE_NAME, STATEMENT_NUMBER));
			Assert::IsTrue(WhileProcessor::processWhile(VALID_WHILE_STATEMENT7, PROCEDURE_NAME, STATEMENT_NUMBER));
		}

		TEST_METHOD(processWhile_invalid_extraKeyword) {
			std::string INVALID_WHILE_STATEMENT_EXTRA_KEYWORD = "while (a < b) a";
			auto exceptionFunction = [&] {
				WhileProcessor::processWhile(INVALID_WHILE_STATEMENT_EXTRA_KEYWORD, PROCEDURE_NAME, STATEMENT_NUMBER);
			};

			Assert::ExpectException<ParserException>(exceptionFunction);
		}

		TEST_METHOD(processWhile_invalid_extraKeyword2) {
			std::string INVALID_WHILE_STATEMENT_EXTRA_KEYWORD2 = "while (as < b)asthen";
			auto exceptionFunction = [&] {
				WhileProcessor::processWhile(INVALID_WHILE_STATEMENT_EXTRA_KEYWORD2, PROCEDURE_NAME, STATEMENT_NUMBER);
			};

			Assert::ExpectException<ParserException>(exceptionFunction);
		}

		TEST_METHOD(processWhile_invalid_invalidExpression) {
			std::string INVALID_WHILE_STATEMENT_INVALID_EXPRESSION = "while (a + b)";
			auto exceptionFunction = [&] {
				WhileProcessor::processWhile(INVALID_WHILE_STATEMENT_INVALID_EXPRESSION, PROCEDURE_NAME, STATEMENT_NUMBER);
			};

			Assert::ExpectException<ParserException>(exceptionFunction);
		}

		TEST_METHOD(processWhile_invalid_invalidExpression2) {
			std::string INVALID_WHILE_STATEMENT_INVALID_EXPRESSION2 = "while (a && b)";
			auto exceptionFunction = [&] {
				WhileProcessor::processWhile(INVALID_WHILE_STATEMENT_INVALID_EXPRESSION2, PROCEDURE_NAME, STATEMENT_NUMBER);
			};

			Assert::ExpectException<ParserException>(exceptionFunction);
		}

		TEST_METHOD(processWhile_invalid_emptyExpression) {
			std::string INVALID_WHILE_STATEMENT_EMPTY_EXPRESSION = "while ()";
			auto exceptionFunction = [&] {
				WhileProcessor::processWhile(INVALID_WHILE_STATEMENT_EMPTY_EXPRESSION, PROCEDURE_NAME, STATEMENT_NUMBER);
			};

			Assert::ExpectException<ParserException>(exceptionFunction);
		}
	};
}
