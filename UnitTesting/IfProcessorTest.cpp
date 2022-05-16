#include "stdafx.h"
#include "CppUnitTest.h"
#include "StatementTypeEnum.h"
#include "IfProcessor.h"
#include "ParserException.h"
#include "PKB.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{
	TEST_CLASS(IfProcessorTest)
	{
	public:
		std::string PROCEDURE_NAME = "procedureSample";
		int STATEMENT_NUMBER = 1;

		TEST_CLASS_INITIALIZE(ifProcessorTest_initializePkb) {
			PKB::clear();
		}
		
		TEST_METHOD(processIf_relationalExpressionNormalParenthesesValidThenConsistentWhitespace_valid) {
			std::string VALID_IF_STATEMENT = "if (a < b + c) then";

			Assert::IsTrue(IfProcessor::processIf(VALID_IF_STATEMENT, PROCEDURE_NAME, STATEMENT_NUMBER));
		}

		TEST_METHOD(processIf_relationalExpressionNormalParenthesesValidThenInconsistentWhitespace_valid) {
			std::string VALID_IF_STATEMENT = "if(         a+c <               b+ c)then           ";

			Assert::IsTrue(IfProcessor::processIf(VALID_IF_STATEMENT, PROCEDURE_NAME, STATEMENT_NUMBER));
		}

		TEST_METHOD(processIf_relationalExpressionDeepParenthesesValidThenConsistentWhitespace_valid) {
			std::string VALID_IF_STATEMENT = "if (((((((((((((((((a + f)))))))) < ((((((((b)))))))) + c))))))))) then";

			Assert::IsTrue(IfProcessor::processIf(VALID_IF_STATEMENT, PROCEDURE_NAME, STATEMENT_NUMBER));
		}

		TEST_METHOD(processIf_relationalExpressionDeepParenthesesValidThenInconsistentWhitespace_valid) {
			std::string VALID_IF_STATEMENT = "if(                  ((((			(((((   (((    ((((a+ f)		 ))))    )))   <    ((((    (( ( ( b		)))			)	)))) +		c	)))))	)	)	))			 then  ";		

			Assert::IsTrue(IfProcessor::processIf(VALID_IF_STATEMENT, PROCEDURE_NAME, STATEMENT_NUMBER));
		}

		TEST_METHOD(processIf_conditionalExpressionNormalParenthesesValidThenConsistentWhitespace_valid) {
			std::string VALID_IF_STATEMENT = "if ((superFreak < 2 + upper) && (nowThis - isEpic < 23)) then";

			Assert::IsTrue(IfProcessor::processIf(VALID_IF_STATEMENT, PROCEDURE_NAME, STATEMENT_NUMBER));
		}

		TEST_METHOD(processIf_conditionalExpressionNormalParenthesesValidThenInconsistentWhitespace_valid) {
			std::string VALID_IF_STATEMENT = "if(     (      superFreak< 2          +          upper)&&    (         nowThis    -      isEpic<      23)                 )          then           ";

			Assert::IsTrue(IfProcessor::processIf(VALID_IF_STATEMENT, PROCEDURE_NAME, STATEMENT_NUMBER));
		}

		TEST_METHOD(processIf_conditionalExpressionDeepParenthesesValidThenConsistentWhitespace_valid) {
			std::string VALID_IF_STATEMENT = "if ((((((((((superFreak)))))))) < 2 + upper) && (((((((((nowThis - isEpic < 23)))))))))) then";

			Assert::IsTrue(IfProcessor::processIf(VALID_IF_STATEMENT, PROCEDURE_NAME, STATEMENT_NUMBER));
		}

		TEST_METHOD(processIf_conditionalExpressionDeepParenthesesValidThenInconsistentWhitespace_valid) {
			std::string VALID_IF_STATEMENT = "if (((((((((              (superFreak))))                   )))) <2 +              upper)          && ((              (((((((nowThis         - isEpic< 23)))))))))) then";

			Assert::IsTrue(IfProcessor::processIf(VALID_IF_STATEMENT, PROCEDURE_NAME, STATEMENT_NUMBER));
		}

		TEST_METHOD(processIf_missingThen_invalid) {
			std::string INVALID_IF_STATEMENT = "if (a < b)";
			auto exceptionFunction = [&] {
				IfProcessor::processIf(INVALID_IF_STATEMENT, PROCEDURE_NAME, STATEMENT_NUMBER);
			};

			Assert::ExpectException<ParserException>(exceptionFunction);
		}

		TEST_METHOD(processIf_mispelledThen_invalid) {
			std::string INVALID_IF_STATEMENT = "if (a < b) ther";
			auto exceptionFunction = [&] {
				IfProcessor::processIf(INVALID_IF_STATEMENT, PROCEDURE_NAME, STATEMENT_NUMBER);
			};

			Assert::ExpectException<ParserException>(exceptionFunction);
		}

		TEST_METHOD(processIf_extraWordsBeforeThen_invalid) {
			std::string INVALID_IF_STATEMENT = "if (a < b) extra words then";
			auto exceptionFunction = [&] {
				IfProcessor::processIf(INVALID_IF_STATEMENT, PROCEDURE_NAME, STATEMENT_NUMBER);
			};

			Assert::ExpectException<ParserException>(exceptionFunction);
		}

		TEST_METHOD(processIf_thenWrongWhitespace_invalid) {
			std::string INVALID_IF_STATEMENT = "if (a < b) th en";
			auto exceptionFunction = [&] {
				IfProcessor::processIf(INVALID_IF_STATEMENT, PROCEDURE_NAME, STATEMENT_NUMBER);
			};

			Assert::ExpectException<ParserException>(exceptionFunction);
		}

		TEST_METHOD(processIf_leftParenthesesBeforeRight_invalid) {
			std::string INVALID_IF_STATEMENT = "if ()a + b( < 2) then";
			auto exceptionFunction = [&] {
				IfProcessor::processIf(INVALID_IF_STATEMENT, PROCEDURE_NAME, STATEMENT_NUMBER);
			};

			Assert::ExpectException<ParserException>(exceptionFunction);
		}

		TEST_METHOD(processIf_missingParentheses_invalid) {
			std::string INVALID_IF_STATEMENT = "if a < b then";
			auto exceptionFunction = [&] {
				IfProcessor::processIf(INVALID_IF_STATEMENT, PROCEDURE_NAME, STATEMENT_NUMBER);
			};

			Assert::ExpectException<ParserException>(exceptionFunction);
		}

		TEST_METHOD(processIf_mismatchedParentheses_invalid) {
			std::string INVALID_IF_STATEMENT = "if (a < b )) then";
			auto exceptionFunction = [&] {
				IfProcessor::processIf(INVALID_IF_STATEMENT, PROCEDURE_NAME, STATEMENT_NUMBER);
			};

			Assert::ExpectException<ParserException>(exceptionFunction);
		}

		TEST_METHOD(processIf_missingExpression_invalid) {
			std::string INVALID_IF_STATEMENT = "if () then";
			auto exceptionFunction = [&] {
				IfProcessor::processIf(INVALID_IF_STATEMENT, PROCEDURE_NAME, STATEMENT_NUMBER);
			};

			Assert::ExpectException<ParserException>(exceptionFunction);
		}

		TEST_METHOD(processIf_invalidConditionalExpression_invalid) {
			std::string INVALID_IF_STATEMENT = "if ((a < b) && (a <= b) || (b - c < 4)) then";
			auto exceptionFunction = [&] {
				IfProcessor::processIf(INVALID_IF_STATEMENT, PROCEDURE_NAME, STATEMENT_NUMBER);
			};

			Assert::ExpectException<ParserException>(exceptionFunction);
		}

		TEST_METHOD(processIf_invalidRelationalExpression_invalid) {
			std::string INVALID_IF_STATEMENT = "if (a + b + c === 23) then";
			auto exceptionFunction = [&] {
				IfProcessor::processIf(INVALID_IF_STATEMENT, PROCEDURE_NAME, STATEMENT_NUMBER);
			};

			Assert::ExpectException<ParserException>(exceptionFunction);
		}

		TEST_METHOD(processIf_standardExpression_invalid) {
			std::string INVALID_IF_STATEMENT = "if (2 * variable + supercool) then";
			auto exceptionFunction = [&] {
				IfProcessor::processIf(INVALID_IF_STATEMENT, PROCEDURE_NAME, STATEMENT_NUMBER);
			};

			Assert::ExpectException<ParserException>(exceptionFunction);
		}
	};
}
