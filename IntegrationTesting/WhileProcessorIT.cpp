#include "stdafx.h"
#include "CppUnitTest.h"
#include "StatementTypeEnum.h"
#include "WhileProcessor.h"
#include "ParserException.h"
#include "PKB.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace IntegrationTesting
{
	TEST_CLASS(WhileProcessorIT)
	{
	public:
		std::string whileKeyword = "while";
		std::string relationalExpression = "(a <= b - casdsah + 65)";
		std::string conditionalExpression = "((this <= means) && (then ==what))";
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

		TEST_METHOD(processWhile_ValidRelationalInput_ExistInPkb) {
			std::string input = whileKeyword + " " + relationalExpression;
			WhileProcessor::processWhile(input, procedureName, statementNumber);
			Assert::IsTrue(PKB::getWhileStmtList().size() == 1);
			Assert::IsTrue(PKB::getWhileStmtList() == inputStatements);
			Assert::IsTrue(PKB::getVarsUsedByWhile(statementNumber) == relationalVariables);
			Assert::IsTrue(PKB::getVarsUsedInWhile() == relationalVariables);
		}

		TEST_METHOD(processWhile_ValidConditionalInput_ExistInPkb) {
			std::string input = whileKeyword + conditionalExpression;
			WhileProcessor::processWhile(input, procedureName, statementNumber);
			Assert::IsTrue(PKB::getWhileStmtList() == inputStatements);
			Assert::IsTrue(PKB::getVarsUsedByWhile(statementNumber) == conditionalVariables);
			Assert::IsTrue(PKB::getVarsUsedInWhile() == conditionalVariables);
		}

		TEST_METHOD(processWhile_InvalidExpression_NotExistInPkb) {
			std::string input = whileKeyword + "(faur23ttdf23 55 3258ry guwb 358vy 3tg e)";
			auto exception = [&] {
				WhileProcessor::processWhile(input, procedureName, statementNumber);
			};
			Assert::IsTrue(PKB::getWhileStmtList().empty());
		}

		TEST_METHOD(processWhile_WrongKeyword_NotExistInPkb) {
			std::string input = "whie" + relationalExpression;
			auto exception = [&] {
				WhileProcessor::processWhile(input, procedureName, statementNumber);
			};
			Assert::IsTrue(PKB::getWhileStmtList().empty());
		}

		TEST_METHOD(processWhile_Empty_NotExistInPkb) {
			std::string input = "";
			auto exception = [&] {
				WhileProcessor::processWhile(input, procedureName, statementNumber);
			};
			Assert::IsTrue(PKB::getWhileStmtList().empty());
		}
	};
}
