#include "stdafx.h"
#include "CppUnitTest.h"
#include "StatementTypeEnum.h"
#include "CallProcessor.h"
#include "ParserException.h"
#include "PKB.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace IntegrationTesting
{
	TEST_CLASS(CallProcessorIT)
	{
	public:
		std::string procedureName = "procedsaioueg123087dtasf0we3";
		std::string variableName = "asfioyug2459t87tgbnr";
		int statementNumber = 1;

		TEST_METHOD_INITIALIZE(clearPkbBefore) {
			PKB::clear();
		}

		TEST_METHOD_CLEANUP(clearPkbAfter) {
			PKB::clear();
		}

		TEST_METHOD(processCall_ValidInput_ExistInPkb) {
			std::string input = "call " + variableName;
			CallProcessor::processCall(input, procedureName, statementNumber);
			std::unordered_set<int> setOfCallStmtNo = { statementNumber };
			std::unordered_set<std::string> setOfCaller = { procedureName };
			std::unordered_set<std::string> setOfCallee = { variableName };
			Assert::IsTrue(PKB::getCalleesOfProc(procedureName) == setOfCallee);
			Assert::IsTrue(PKB::getCallersOfProc(variableName) == setOfCaller);
			Assert::IsTrue(PKB::getCallerStmts(variableName) == setOfCallStmtNo);
		}

		TEST_METHOD(processCall_InvalidVariableName_NotStoredInPkb) {
			std::string input = "call 12084791264c5b1ncvbasd";

			auto exception = [&] {
				CallProcessor::processCall(input, procedureName, statementNumber);
			};
			
			Assert::ExpectException<ParserException>(exception);

			Assert::IsTrue(PKB::getCallStmtList().empty());
		}

		TEST_METHOD(processCall_TooManyVariables_NotStoredInPkb) {
			std::string input = "call " + variableName + " " + variableName;

			auto exception = [&] {
				CallProcessor::processCall(input, procedureName, statementNumber);
			};

			Assert::ExpectException<ParserException>(exception);

			Assert::IsTrue(PKB::getCallStmtList().empty());
		}
	};
}
