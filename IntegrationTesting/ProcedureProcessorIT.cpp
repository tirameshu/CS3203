#include "stdafx.h"
#include "CppUnitTest.h"
#include "StatementTypeEnum.h"
#include "ProcedureProcessor.h"
#include "ParserException.h"
#include "PKB.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace IntegrationTesting
{
	TEST_CLASS(ProcedureProcessorIT)
	{
	public:

		TEST_METHOD_INITIALIZE(clearPkbBefore) {
			PKB::clear();
		}

		TEST_METHOD_CLEANUP(clearPkbAfter) {
			PKB::clear();
		}

		TEST_METHOD(procedureProcessor_ValidProcedure_ExistInPkb) {
			std::string procedureName = "procedureExample23";
			std::string input = "procedure " + procedureName;
			ProcedureProcessor::processProcedure(input);
			std::unordered_set<std::string> inputProcNames = { procedureName };
			std::unordered_set<std::string> procedureList = PKB::getProcedureList();
			Assert::IsFalse(procedureList.empty());
			Assert::IsTrue(procedureList.size() == 1);
			Assert::IsTrue(inputProcNames == procedureList);
		}

		TEST_METHOD(procedureProcessor_MultipleValidProcedures_ExistInPkb) {	
			std::string procedureName1 = "asuhdgasu1241287tbf";
			std::string procedureName2 = "asdouyg5jhbfgv923452352359275rdfg";
			std::string input1 = "procedure " + procedureName1;
			std::string input2 = "procedure " + procedureName2;
			ProcedureProcessor::processProcedure(input1);
			ProcedureProcessor::processProcedure(input2);
			std::unordered_set<std::string> inputProcNames = { procedureName1, procedureName2 };
			std::unordered_set<std::string> pkbProcNames = PKB::getProcedureList();
			Assert::IsFalse(pkbProcNames.empty());
			Assert::IsTrue(pkbProcNames.size() == 2);
			Assert::IsTrue(inputProcNames == pkbProcNames);
		}

		TEST_METHOD(procedureProcessor_InvalidProcedureName_PkbEmpty) {
			std::string procedureName1 = "983438hsdfasd";
			std::string input1 = "procedure " + procedureName1;

			auto exception = [&] {
				ProcedureProcessor::processProcedure(input1);
			};
			
			Assert::ExpectException<ParserException>(exception);

			std::unordered_set<std::string> pkbProcNames = PKB::getProcedureList();
			Assert::IsTrue(pkbProcNames.empty());
		}

		TEST_METHOD(procedureProcessor_WrongProcedureKeyword_PkbEmpty) {
			std::string procedureName1 = "validProf";
			std::string input1 = "proc " + procedureName1;

			auto exception = [&] {
				ProcedureProcessor::processProcedure(input1);
			};

			Assert::ExpectException<ParserException>(exception);

			std::unordered_set<std::string> pkbProcNames = PKB::getProcedureList();
			Assert::IsTrue(pkbProcNames.empty());
		}
	};
}
