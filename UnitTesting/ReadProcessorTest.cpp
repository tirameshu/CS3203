#include "stdafx.h"
#include "CppUnitTest.h"
#include "ReadProcessor.h"
#include "ParserException.h"
#include "PKB.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
	TEST_CLASS(ReadProcessorTest) {
	public:
		std::string PROCEDURE_NAME = "ProcedureName";
		int STATEMENT_NUMBER = 1;

		TEST_CLASS_INITIALIZE(readProcessorTest_initializePkb) {
			PKB::clear();
		}

		TEST_METHOD(processReadStatement_valid) {

			std::string VALID_READ_STATEMENT_1 = "read nowthisiscool";
			std::string VALID_READ_STATEMENT_2 = "read epicV23alid";
			Assert::IsTrue(ReadProcessor::processRead(VALID_READ_STATEMENT_1, PROCEDURE_NAME, STATEMENT_NUMBER));
			Assert::IsTrue(ReadProcessor::processRead(VALID_READ_STATEMENT_2, PROCEDURE_NAME, STATEMENT_NUMBER));
		}

		TEST_METHOD(processReadStatement_invalid_invalidVariableName) {
			std::string INVALID_READ_STATEMENT_INVALID_VARIABLE_NAME = "read 123";
			auto invalidVariableNameFunction = [&] {
				ReadProcessor::processRead(INVALID_READ_STATEMENT_INVALID_VARIABLE_NAME, PROCEDURE_NAME, STATEMENT_NUMBER);
			};
			Assert::ExpectException<ParserException>(invalidVariableNameFunction);
		}

		TEST_METHOD(processReadStatement_invalid_emptyVariable) {
			std::string INVALID_READ_STATEMENT_EMPTY_VARIABLE = "read";
			auto emptyVariableFunction = [&] {
				ReadProcessor::processRead(INVALID_READ_STATEMENT_EMPTY_VARIABLE, PROCEDURE_NAME, STATEMENT_NUMBER);
			};
			Assert::ExpectException<ParserException>(emptyVariableFunction);
		}

		TEST_METHOD(processReadStatement_invalid_tooManyVariables) {
			std::string INVALID_READ_STATEMENT_TOO_MANY_VARIABLES = "read var1 var2";
			auto tooManyVariablesFunction = [&] {
				ReadProcessor::processRead(INVALID_READ_STATEMENT_TOO_MANY_VARIABLES, PROCEDURE_NAME, STATEMENT_NUMBER);
			};
			Assert::ExpectException<ParserException>(tooManyVariablesFunction);
		}
	};
}