#include "stdafx.h"
#include "CppUnitTest.h"
#include "QueryEvaluator.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace IntegrationTesting
{
	TEST_CLASS(NoClausesQueryEvaluatorTest)
	{
	public:
		TEST_CLASS_INITIALIZE(InitializePKB)
		{
			PKB pkb = PKB();
			pkb.clear(); // need to clear for unit testing, pkb used by multiple test files
			pkb.addStatement("main", Statement(1, 5)); // 1:assign, 2:print, 3:read, 4:if, 5:while
			pkb.addStatement("first", Statement(1, 7));
			pkb.addStatement("main", Statement(2, 3));
			pkb.addStatement("first", Statement(2, 1));
			pkb.addStatement("second", Statement(3, 9));
			pkb.addStatement("second", Statement(3, 8));
			pkb.addStatement("first", Statement(4, 6));
			pkb.addStatement("second", Statement(4, 4));
			pkb.addStatement("main", Statement(5, 2));
			pkb.addStatement("third", Statement(5, 10));

			pkb.addProcedure("main");
			pkb.addProcedure("first");
			pkb.addProcedure("second");
			pkb.addProcedure("third");

			pkb.addVariable("a");
			pkb.addVariable("Cs3203");
			pkb.addVariable("sPA");

			pkb.addConstant(1, "20");
			pkb.addConstant(2, "3203");
			pkb.addConstant(3, "2021");
		}

		TEST_METHOD(evaluateNoClausesQuery_STMT) {
			std::unordered_set<std::string> expectedOutput;

			for (int i = 1; i <= 10; i++) {
				expectedOutput.emplace(std::to_string(i));
			}

			Query stmtQuery = Query("s", ArgumentTypes::STMT);
			std::unordered_set<std::string> resultSet = QueryEvaluator::evaluateQuery(stmtQuery);

			Assert::IsTrue(expectedOutput == resultSet);
		}

		TEST_METHOD(evaluateNoClausesQuery_READ) {
			std::unordered_set<std::string> expectedOutput = { "8", "9" };

			Query readQuery = Query("r", ArgumentTypes::READ);
			std::unordered_set<std::string> resultSet = QueryEvaluator::evaluateQuery(readQuery);

			Assert::IsTrue(expectedOutput == resultSet);
		}

		TEST_METHOD(evaluateNoClausesQuery_PRINT) {
			std::unordered_set<std::string> expectedOutput = { "1", "3" };

			Query printQuery = Query("pr", ArgumentTypes::PRINT);
			std::unordered_set<std::string> resultSet = QueryEvaluator::evaluateQuery(printQuery);

			Assert::IsTrue(expectedOutput == resultSet);
		}

		TEST_METHOD(evaluateNoClausesQuery_WHILE) {
			std::unordered_set<std::string> expectedOutput = { "2", "10" };

			Query whileQuery = Query("w", ArgumentTypes::WHILE);
			std::unordered_set<std::string> resultSet = QueryEvaluator::evaluateQuery(whileQuery);

			Assert::IsTrue(expectedOutput == resultSet);
		}

		TEST_METHOD(evaluateNoClausesQuery_IF) {
			std::unordered_set<std::string> expectedOutput = { "4", "6" };

			Query ifQuery = Query("ifs", ArgumentTypes::IF);
			std::unordered_set<std::string> resultSet = QueryEvaluator::evaluateQuery(ifQuery);

			Assert::IsTrue(expectedOutput == resultSet);
		}

		TEST_METHOD(evaluateNoClausesQuery_ASSIGN) {
			std::unordered_set<std::string> expectedOutput = { "5", "7" };

			Query assignQuery = Query("a", ArgumentTypes::ASSIGN);
			std::unordered_set<std::string> resultSet = QueryEvaluator::evaluateQuery(assignQuery);

			Assert::IsTrue(expectedOutput == resultSet);
		}

		TEST_METHOD(evaluateNoClausesQuery_VARIABLE) {
			std::unordered_set<std::string> expectedOutput = { "a", "Cs3203", "sPA" };

			Query variableQuery = Query("v", ArgumentTypes::VARIABLE);
			std::unordered_set<std::string> resultSet = QueryEvaluator::evaluateQuery(variableQuery);

			Assert::IsTrue(expectedOutput == resultSet);
		}

		TEST_METHOD(evaluateNoClausesQuery_CONSTANT) {
			std::unordered_set<std::string> expectedOutput = { "20", "3203", "2021" };

			Query constantQuery = Query("c", ArgumentTypes::CONSTANT);
			std::unordered_set<std::string> resultSet = QueryEvaluator::evaluateQuery(constantQuery);

			Assert::IsTrue(expectedOutput == resultSet);
		}

		TEST_METHOD(evaluateNoClausesQuery_PROCEDURE) {
			std::unordered_set<std::string> expectedOutput = { "main", "first", "second", "third" };

			Query procedureQuery = Query("p", ArgumentTypes::PROCEDURE);
			std::unordered_set<std::string> resultSet = QueryEvaluator::evaluateQuery(procedureQuery);

			Assert::IsTrue(expectedOutput == resultSet);
		}
	};
}
