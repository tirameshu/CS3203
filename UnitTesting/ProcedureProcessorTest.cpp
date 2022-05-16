#include "stdafx.h"
#include "CppUnitTest.h"
#include "StatementTypeEnum.h"
#include "ProcedureProcessor.h"
#include "ParserException.h"
#include "PKB.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{
	TEST_CLASS(ProcedureProcessorTest)
	{
	public:

		TEST_CLASS_INITIALIZE(procedureProcessorTest_initialize_clearPkb) {
			PKB::clear();
		}

		TEST_METHOD(processProcedure_valid_standard) {
			std::string input = "procedure Epic";
			std::string procname = "Epic";

			Assert::IsTrue(ProcedureProcessor::processProcedure(input).compare(procname) == 0);
		}

		TEST_METHOD(processProcedure_valid_varNameWithNumber) {
			std::string input = "procedure Epic4";
			std::string procname = "Epic4";

			Assert::IsTrue(ProcedureProcessor::processProcedure(input).compare(procname) == 0);
		}

		TEST_METHOD(processProcedure_valid_whitespace) {
			std::string input = "procedure                                                     Epic4";
			std::string procname = "Epic4";

			Assert::IsTrue(ProcedureProcessor::processProcedure(input).compare(procname) == 0);
		}

		TEST_METHOD(processProcedure_valid_leadingWhitespace) {
			std::string input = "                       procedure                                                     Epic4";
			std::string procname = "Epic4";

			Assert::IsTrue(ProcedureProcessor::processProcedure(input).compare(procname) == 0);
		}

		TEST_METHOD(processProcedure_valid_trailingWhitespace) {
			std::string input = "procedure                    Epic4                                                            ";
			std::string procname = "Epic4";

			Assert::IsTrue(ProcedureProcessor::processProcedure(input).compare(procname) == 0);
		}

		TEST_METHOD(processProcedure_valid_procedureNameSameAsKeyword) {
			std::string input = "procedure procedure";
			std::string procname = "procedure";

			Assert::IsTrue(ProcedureProcessor::processProcedure(input).compare(procname) == 0);
		}

		TEST_METHOD(processProcedure_invalid_invalidKeyword) {
			std::string input = "proc Rpiv";
			auto exception = [&] {
				ProcedureProcessor::processProcedure(input);
			};

			Assert::ExpectException<ParserException>(exception);
		}

		TEST_METHOD(processProcedure_invalid_invalidVarName) {
			std::string input = "procedure 63asd";
			auto exception = [&] {
				ProcedureProcessor::processProcedure(input);
			};

			Assert::ExpectException<ParserException>(exception);
		}

		TEST_METHOD(processProcedure_invalid_invalidVarName2) {
			std::string input = "procedure 632232";
			auto exception = [&] {
				ProcedureProcessor::processProcedure(input);
			};

			Assert::ExpectException<ParserException>(exception);
		}

		TEST_METHOD(processProcedure_invalid_tooManyVar) {
			std::string input = "procedure var1 var2";
			auto exception = [&] {
				ProcedureProcessor::processProcedure(input);
			};

			Assert::ExpectException<ParserException>(exception);
		}

		TEST_METHOD(processProcedure_invalid_tooManyKeywords) {
			std::string input = "procedure procedure procedureName";
			auto exception = [&] {
				ProcedureProcessor::processProcedure(input);
			};

			Assert::ExpectException<ParserException>(exception);
		}

		TEST_METHOD(processProcedure_invalid_noVariable) {
			std::string input = "procedure";
			auto exception = [&] {
				ProcedureProcessor::processProcedure(input);
			};

			Assert::ExpectException<ParserException>(exception);
		}

		TEST_METHOD(processProcedure_invalid) {
			std::string input = "";
			auto exception = [&] {
				ProcedureProcessor::processProcedure(input);
			};

			Assert::ExpectException<ParserException>(exception);
		}
	};
}
