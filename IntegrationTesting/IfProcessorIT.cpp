#include "stdafx.h"
#include "CppUnitTest.h"
#include "StatementTypeEnum.h"
#include "IfProcessor.h"
#include "ParserException.h"
#include "PKB.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace IntegrationTesting
{
	TEST_CLASS(IfProcessorIT)
	{
	public:
		std::string ifKeyword = "if";
		std::string relationalExpression = "(a <= b - casdsah + 65)";
		std::string conditionalExpression = "((this <= means) && (then ==what))";
		std::string thenKeyword = "then";
		std::unordered_set<std::string> relationalVariables = { "a", "b", "casdsah" };
		std::unordered_set<std::string> conditionalVariables = { "this", "means", "then", "what" };
		std::string procedureName = "asdiasgbdqw234";
		int statementNumber = 1;
		std::unordered_set<int> inputStatements = { statementNumber };

		TEST_METHOD_INITIALIZE(clearPkbBefore) {
			PKB::clear();
		}

		TEST_METHOD_CLEANUP(clearPkbAfter) {
			PKB::clear();
		}

		TEST_METHOD(processWhile_ValidRelational_ExistInPkb) {
			std::string input = ifKeyword + relationalExpression + thenKeyword;
			IfProcessor::processIf(input, procedureName, statementNumber);
			Assert::IsFalse(PKB::getIfStmtList().empty());
			Assert::IsTrue(PKB::getIfStmtList() == inputStatements);
			Assert::IsTrue(PKB::getIfUsedVarList() == relationalVariables);
		}

		TEST_METHOD(processWhile_ValidConditional_ExistInPkb) {
			std::string input = ifKeyword + conditionalExpression + thenKeyword;
			IfProcessor::processIf(input, procedureName, statementNumber);
			Assert::IsFalse(PKB::getIfStmtList().empty());
			Assert::IsTrue(PKB::getIfStmtList() == inputStatements);
			Assert::IsTrue(PKB::getIfUsedVarList() == conditionalVariables);
		}

		TEST_METHOD(processWhile_InvalidExpression_NotExistInPkb) {
			std::string input = ifKeyword + "5f97r6ftbc02368tt09 5 962334q553" + thenKeyword;
			auto exception = [&] {
				IfProcessor::processIf(input, procedureName, statementNumber);
			};
			Assert::IsTrue(PKB::getIfStmtList().empty());
		}

		TEST_METHOD(processWhile_InvalidIfKeyword_NotExistInPkb) {
			std::string input = "asdsa " + relationalExpression + thenKeyword;
			auto exception = [&] {
				IfProcessor::processIf(input, procedureName, statementNumber);
			};
			Assert::IsTrue(PKB::getIfStmtList().empty());
		}

		TEST_METHOD(processWhile_InvalidThenKeyword_NotExistInPkb) {
			std::string input = ifKeyword + relationalExpression + "th en";
			auto exception = [&] {
				IfProcessor::processIf(input, procedureName, statementNumber);
			};
			Assert::IsTrue(PKB::getIfStmtList().empty());
		}

		TEST_METHOD(processWhile_MissingThenKeyword_NotExistInPkb) {
			std::string input = ifKeyword + relationalExpression;
			auto exception = [&] {
				IfProcessor::processIf(input, procedureName, statementNumber);
			};
			Assert::IsTrue(PKB::getIfStmtList().empty());
			PKB::clear();
		}

		TEST_METHOD(processWhile_EmptyInput_NotExistInPkb) {
			std::string input = "";
			auto exception = [&] {
				IfProcessor::processIf(input, procedureName, statementNumber);
			};
			Assert::IsTrue(PKB::getIfStmtList().empty());
		}
	};
}
