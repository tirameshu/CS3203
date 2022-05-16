#include "stdafx.h"
#include "CppUnitTest.h"
#include "StatementTypeEnum.h"
#include "ReadProcessor.h"
#include "ParserException.h"
#include "PKB.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace IntegrationTesting
{
	TEST_CLASS(ReadProcessorIT)
	{
	public:
		std::string PROCEDURE_NAME = "procedureSample";
		std::string VALID_VARIABLE = "nowthisis2epic";
		std::string VALID_STATEMENT = "read " + VALID_VARIABLE;

		int STATEMENT_NUMBER = 1;

		TEST_METHOD_INITIALIZE(clearPkbBefore) {
			PKB::clear();
		}

		TEST_METHOD_CLEANUP(clearPkbAfter) {
			PKB::clear();
		}

		TEST_METHOD(processRead_Valid_StatementExist) {
			ReadProcessor::processRead(VALID_STATEMENT, PROCEDURE_NAME, STATEMENT_NUMBER);
			std::unordered_set<int> actualOutput = PKB::getReadStmtList();

			Assert::IsFalse(actualOutput.empty());

			for (const int& elem : actualOutput) {
				Assert::AreEqual(STATEMENT_NUMBER, elem);
			}
		}

		TEST_METHOD(processRead_Valid_ModifiedVariableExist) {
			ReadProcessor::processRead(VALID_STATEMENT, PROCEDURE_NAME, STATEMENT_NUMBER);
			std::unordered_set<std::string> actualOutput = PKB::getAllModifiedVars();

			Assert::IsFalse(actualOutput.empty());

			for (const std::string& elem : actualOutput) {
				Assert::AreEqual(VALID_VARIABLE, elem);
			}
		}

		TEST_METHOD(processRead_Valid_ModifiedReadVariableExist) {
			ReadProcessor::processRead(VALID_STATEMENT, PROCEDURE_NAME, STATEMENT_NUMBER);
			std::unordered_set<std::string> actualOutput = PKB::getAllReadModifiedVars();

			Assert::IsFalse(actualOutput.empty());
			Assert::IsTrue(PKB::isVarModified(VALID_VARIABLE));

			for (const std::string& elem : actualOutput) {
				Assert::AreEqual(VALID_VARIABLE, elem);
			}
		}

		TEST_METHOD(processRead_Valid_ProcModifyExist) {
			ReadProcessor::processRead(VALID_STATEMENT, PROCEDURE_NAME, STATEMENT_NUMBER);
			Assert::IsTrue(PKB::isVarModifiedInProc(VALID_VARIABLE, PROCEDURE_NAME));
		}

		TEST_METHOD(processRead_Invalid_NotStoredInPKB) {
			std::string input = "read not not not";

			auto exception = [&] {
				ReadProcessor::processRead(input, PROCEDURE_NAME, STATEMENT_NUMBER);
			};
			
			Assert::ExpectException<ParserException>(exception);

			Assert::IsTrue(PKB::getAllModifiedVars().empty());
		}
	};
}
