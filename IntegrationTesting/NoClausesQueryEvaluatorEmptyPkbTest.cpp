#include "stdafx.h"
#include "CppUnitTest.h"
#include "QueryEvaluator.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace IntegrationTesting
{
	TEST_CLASS(NoClausesQueryEvaluatorEmptyPkbTest)
	{
	public:
		TEST_CLASS_INITIALIZE(ClearPKB)
		{
			PKB pkb = PKB();
			pkb.clear();
		}

		TEST_METHOD(evaluateNoClausesQueryEmptyPkb_STMT)
		{
			std::unordered_set<std::string> expectedOutput;

			Query stmtQuery = Query("s", ArgumentTypes::STMT);
			std::unordered_set<std::string> resultSet = QueryEvaluator::evaluateQuery(stmtQuery);

			Assert::IsTrue(expectedOutput == resultSet);
		}

		TEST_METHOD(evaluateNoClausesQueryEmptyPkb_READ) {
			std::unordered_set<std::string> expectedOutput;

			Query readQuery = Query("r", ArgumentTypes::READ);
			std::unordered_set<std::string> resultSet = QueryEvaluator::evaluateQuery(readQuery);

			Assert::IsTrue(expectedOutput == resultSet);
		}

		TEST_METHOD(evaluateNoClausesQueryEmptyPkb_PRINT) {
			std::unordered_set<std::string> expectedOutput;

			Query printQuery = Query("pr", ArgumentTypes::PRINT);
			std::unordered_set<std::string> resultSet = QueryEvaluator::evaluateQuery(printQuery);

			Assert::IsTrue(expectedOutput == resultSet);
		}

		TEST_METHOD(evaluateNoClausesQueryEmptyPkb_WHILE) {
			std::unordered_set<std::string> expectedOutput;

			Query whileQuery = Query("w", ArgumentTypes::WHILE);
			std::unordered_set<std::string> resultSet = QueryEvaluator::evaluateQuery(whileQuery);

			Assert::IsTrue(expectedOutput == resultSet);
		}

		TEST_METHOD(evaluateNoClausesQueryEmptyPkb_IF) {
			std::unordered_set<std::string> expectedOutput;

			Query ifQuery = Query("ifs", ArgumentTypes::IF);
			std::unordered_set<std::string> resultSet = QueryEvaluator::evaluateQuery(ifQuery);

			Assert::IsTrue(expectedOutput == resultSet);
		}

		TEST_METHOD(evaluateNoClausesQueryEmptyPkb_ASSIGN) {
			std::unordered_set<std::string> expectedOutput;

			Query assignQuery = Query("a", ArgumentTypes::ASSIGN);
			std::unordered_set<std::string> resultSet = QueryEvaluator::evaluateQuery(assignQuery);

			Assert::IsTrue(expectedOutput == resultSet);
		}

		TEST_METHOD(evaluateNoClausesQueryEmptyPkb_VARIABLE) {
			std::unordered_set<std::string> expectedOutput;

			Query variableQuery = Query("v", ArgumentTypes::VARIABLE);
			std::unordered_set<std::string> resultSet = QueryEvaluator::evaluateQuery(variableQuery);

			Assert::IsTrue(expectedOutput == resultSet);
		}

		TEST_METHOD(evaluateNoClausesQueryEmptyPkb_CONSTANT) {
			std::unordered_set<std::string> expectedOutput;

			Query constantQuery = Query("c", ArgumentTypes::CONSTANT);
			std::unordered_set<std::string> resultSet = QueryEvaluator::evaluateQuery(constantQuery);

			Assert::IsTrue(expectedOutput == resultSet);
		}

		TEST_METHOD(evaluateNoClausesQueryEmptyPkb_PROCEDURE) {
			std::unordered_set<std::string> expectedOutput;

			Query procedureQuery = Query("p", ArgumentTypes::PROCEDURE);
			std::unordered_set<std::string> resultSet = QueryEvaluator::evaluateQuery(procedureQuery);

			Assert::IsTrue(expectedOutput == resultSet);
		}
	};
}
