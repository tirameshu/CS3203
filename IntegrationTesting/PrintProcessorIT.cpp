#include "stdafx.h"
#include "CppUnitTest.h"
#include "StatementTypeEnum.h"
#include "PrintProcessor.h"
#include "ParserException.h"
#include "PKB.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace IntegrationTesting
{
	TEST_CLASS(PrintProcessorIT)
	{
	public:
		std::string PROCEDURE_NAME = "procedureSample";
		std::string VALID_STATEMENT = "print cool4";
		std::string VALID_VARIABLE = "cool4";

		int STATEMENT_NUMBER = 1;

		TEST_METHOD_INITIALIZE(clearPkbBefore) {
			PKB::clear();
		}

		TEST_METHOD_CLEANUP(clearPkbAfter) {
			PKB::clear();
		}

		TEST_METHOD(processPrint_AddStatement_StatementExist) {
			PrintProcessor::processPrint(VALID_STATEMENT, PROCEDURE_NAME, STATEMENT_NUMBER);
			std::unordered_set<int> actualOutput = PKB::getPrintStmtList();

			Assert::IsTrue(!actualOutput.empty());

			for (const int& elem : actualOutput) {
				Assert::AreEqual(elem, STATEMENT_NUMBER);
			}
		}

		TEST_METHOD(processPrint_AddStatement_AllUsedVarExist) {
			PrintProcessor::processPrint(VALID_STATEMENT, PROCEDURE_NAME, STATEMENT_NUMBER);
			std::unordered_set<std::string> actualOutput = PKB::getAllUsedVarList();

			Assert::IsTrue(!actualOutput.empty());

			for (const std::string& elem : actualOutput) {
				Assert::AreEqual(elem, VALID_VARIABLE);
			}
		}

		TEST_METHOD(processPrint_AddStatement_AllPrintUsedVarExist) {
			PrintProcessor::processPrint(VALID_STATEMENT, PROCEDURE_NAME, STATEMENT_NUMBER);
			std::unordered_set<std::string> actualOutput = PKB::getAllPrintUsedVarList();

			Assert::IsTrue(!actualOutput.empty());

			for (const std::string& elem : actualOutput) {
				Assert::AreEqual(elem, VALID_VARIABLE);
			}
		}

		TEST_METHOD(processPrint_AddStatement_VarIsUsedInProc) {
			PrintProcessor::processPrint(VALID_STATEMENT, PROCEDURE_NAME, STATEMENT_NUMBER);

			Assert::IsTrue(PKB::isVarUsedInProc(PROCEDURE_NAME, VALID_VARIABLE));
		}

		TEST_METHOD(processPrint_InvalidStatement_NotStoredInPKB) {
			std::string invalidInput = "print super super";

			auto exception = [&] {
				PrintProcessor::processPrint(invalidInput, PROCEDURE_NAME, STATEMENT_NUMBER);
			};

			Assert::ExpectException<ParserException>(exception);

			Assert::IsTrue(PKB::getAllUsedVarList().empty());
		}
	};
}
