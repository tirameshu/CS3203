#include "stdafx.h"
#include "CppUnitTest.h"
#include "CallsTEvaluatorTestData.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace IntegrationTesting
{
	TEST_CLASS(CallsTEvaluatorTest)
	{
	public:
		TEST_CLASS_INITIALIZE(Initialization) {
			PKB pkb;
			pkb.clear();
			pkb.addProcedure("testA");
			pkb.addProcedure("testB");
			pkb.addProcedure("testC");
			pkb.addStatement("testA", { ASSIGN, 1 });
			pkb.addStatement("testA", { CALL, 2 });
			pkb.addStatement("testB", { ASSIGN, 3 });
			pkb.addStatement("testB", { IF, 4 });
			pkb.addStatement("testB", { CALL, 5 });
			pkb.addStatement("testB", { PRINT, 6 });
			pkb.addStatement("testB", { CALL, 7 });
			pkb.addStatement("testB", { READ, 8 });
			pkb.addStatement("testC", { ASSIGN, 9 });

			pkb.addCall("testA", "testB", 2);
			pkb.addCall("testB", "testC", 5);
			pkb.addCall("testB", "testC", 7);

			pkb.addCallerT("testB", { "testA"});
			pkb.addCallerT("testC", { "testA", "testB"});

			pkb.addCalleeT("testA", { "testB", "testC" });
			pkb.addCalleeT("testB", { "testC" });

		}

		TEST_METHOD(CallsTProcProcInClause_ReturnAllCallerProc) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(CallsTProcProcInClause);
			std::unordered_set<std::string> expected = { "testA", "testB" };
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(CallsTProcProcNotInClause_ReturnAllProc) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(CallsTProcProcNotInClause);
			Assert::IsTrue(result == callsTAllProc);
		}

		TEST_METHOD(CallsTProcStringInClauseTrue_ReturnAllCallerTOfTestC) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(CallsTProcStringInClauseTrue);
			std::unordered_set<std::string> expected = { "testA", "testB" };
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(CallsTProcStringInClauseFalse_ReturnNothing) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(CallsTProcStringInClauseFalse);
			std::unordered_set<std::string> expected = {};
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(CallsTProcStringNotInClauseTrue_ReturnAllProc) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(CallsTProcStringNotInClauseTrue);
			Assert::IsTrue(result == callsTAllProc);
		}

		TEST_METHOD(CallsTProcStringNotInClauseFalse_ReturnNothing) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(CallsTProcStringNotInClauseFalse);
			std::unordered_set<std::string> expected = {};
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(CallsTProcAnyInClause_ReturnAllCallerProc) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(CallsTProcAnyInClause);
			std::unordered_set<std::string> expected = { "testA", "testB" };
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(CallsTProcAnyNotInClause_ReturnAllProc) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(CallsTProcAnyNotInClause);
			Assert::IsTrue(result == callsTAllProc);
		}

		TEST_METHOD(CallsTStringProcInClauseTrue_ReturnAllCalleeTOfTestA) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(CallsTStringProcInClauseTrue);
			std::unordered_set<std::string> expected = { "testB", "testC" };
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(CallsTStringProcInClauseFalse_ReturnNothing) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(CallsTStringProcInClauseFalse);
			std::unordered_set<std::string> expected = {};
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(CallsTStringProcNotInClauseTrue_ReturnAllProc) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(CallsTStringProcNotInClauseTrue);
			Assert::IsTrue(result == callsTAllProc);
		}

		TEST_METHOD(CallsTStringProcNotInClauseFalse_ReturnNothing) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(CallsTStringProcNotInClauseFalse);
			std::unordered_set<std::string> expected = {};
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(CallsTStringStringTrue_ReturnAllProc) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(CallsTStringStringTrue);
			Assert::IsTrue(result == callsTAllProc);
		}

		TEST_METHOD(CallsTStringStringFalse_ReturnNothing) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(CallsTStringStringFalse);
			std::unordered_set<std::string> expected = {};
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(CallsTStringAnyTrue_ReturnAllProc) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(CallsTStringAnyTrue);
			Assert::IsTrue(result == callsTAllProc);
		}

		TEST_METHOD(CallsTStringAnyFalse_ReturnNothing) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(CallsTStringAnyFalse);
			std::unordered_set<std::string> expected = {};
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(CallsTAnyProcInClause_ReturnAllCalleeProc) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(CallsTAnyProcInClause);
			std::unordered_set<std::string> expected = { "testB", "testC" };
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(CallsTAnyProcNotInClause_ReturnAllProc) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(CallsTAnyProcNotInClause);
			Assert::IsTrue(result == callsTAllProc);
		}

		TEST_METHOD(CallsTAnyStringTrue_ReturnAllProc) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(CallsTAnyStringTrue);
			Assert::IsTrue(result == callsTAllProc);
		}

		TEST_METHOD(CallsTAnyStringFalse_ReturnNothing) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(CallsTAnyStringFalse);
			std::unordered_set<std::string> expected = {};
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(CallsTAnyAnyTrue_ReturnAllProc) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(CallsTAnyAnyTrue);
			Assert::IsTrue(result == callsTAllProc);
		}

		TEST_METHOD(CallsTEdgeCase1_ReturnNothing) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(CallsTEdgeCase1);
			std::unordered_set<std::string> expected = {};
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(CallsTEdgeCase2_ReturnNothing) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(CallsTEdgeCase2);
			std::unordered_set<std::string> expected = {};
			Assert::IsTrue(result == expected);
		}
	};
}