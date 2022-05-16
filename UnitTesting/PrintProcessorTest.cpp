#include "stdafx.h"
#include "CppUnitTest.h"
#include "StatementTypeEnum.h"
#include "PrintProcessor.h"
#include "ParserException.h"
#include "PKB.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{
	TEST_CLASS(PrintProcessorTest)
	{
	public:
		std::string PROCEDURE_NAME = "procedureSample";
		int STATEMENT_NUMBER = 1;

		TEST_CLASS_INITIALIZE(printProcessorTest_initializePkb) {
			PKB::clear();
		}
		
		PKB pkb = PKB();
		TEST_METHOD(processPrintStatement_valid) {
			std::string VALID_PRINT_STATEMENT_1 = "print varName1";
			std::string VALID_PRINT_STATEMENT_2 = "print         VARNAME";
			bool isSuccess1 = PrintProcessor::processPrint(VALID_PRINT_STATEMENT_1, PROCEDURE_NAME, STATEMENT_NUMBER);
			bool isSuccess2 = PrintProcessor::processPrint(VALID_PRINT_STATEMENT_2, PROCEDURE_NAME, STATEMENT_NUMBER);

			Assert::IsTrue(isSuccess1);
			Assert::IsTrue(isSuccess2);
		}

		TEST_METHOD(processPrintStatement_invalid_invalidVarname) {
			std::string INVALID_PRINT_STATEMENT_INVALID_VARIABLE_NAME_1 = "print 12sd";
			std::string INVALID_PRINT_STATEMENT_INVALID_VARIABLE_NAME_2 = "print 2";

			auto exceptionFunction1 = [&] {
				PrintProcessor::processPrint(INVALID_PRINT_STATEMENT_INVALID_VARIABLE_NAME_1, PROCEDURE_NAME, STATEMENT_NUMBER);
			};

			auto exceptionFunction2 = [&] {
				PrintProcessor::processPrint(INVALID_PRINT_STATEMENT_INVALID_VARIABLE_NAME_2, PROCEDURE_NAME, STATEMENT_NUMBER);
			};

			Assert::ExpectException<ParserException>(exceptionFunction1);
			Assert::ExpectException<ParserException>(exceptionFunction2);
		}

		TEST_METHOD(processPrintStatement_invalid_tooManyVariables) {
			std::string INVALID_PRINT_STATEMENT_TOO_MANY_VARIABLES = "print var1 var2";
			auto exceptionFunction3 = [&] {
				PrintProcessor::processPrint(INVALID_PRINT_STATEMENT_TOO_MANY_VARIABLES, PROCEDURE_NAME, STATEMENT_NUMBER);
			};

			Assert::ExpectException<ParserException>(exceptionFunction3);
		}

		TEST_METHOD(processPrintStatement_invalid_emptyVariable) {
			std::string INVALID_PRINT_STATEMENT_EMPTY_VARIABLE_NAME = "print";
			auto exceptionFunction4 = [&] {
				PrintProcessor::processPrint(INVALID_PRINT_STATEMENT_EMPTY_VARIABLE_NAME, PROCEDURE_NAME, STATEMENT_NUMBER);
			};
			
			Assert::ExpectException<ParserException>(exceptionFunction4);
		}
	};
}
