#include "stdafx.h"
#include "CppUnitTest.h"
#include "CallsEvaluatorTestData.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace IntegrationTesting
{
	TEST_CLASS(CallsEvaluatorTest)
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
		}

		TEST_METHOD(CallsProcProcInClause_ReturnAllCallerProc) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(CallsProcProcInClause);
			std::unordered_set<std::string> expected = { "testA", "testB" };
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(CallsProcProcNotInClause_ReturnAllProc) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(CallsProcProcNotInClause);
			Assert::IsTrue(result == callsAllProc);
		}

		TEST_METHOD(CallsProcStringInClauseTrue_ReturnAllCallerOfTestC) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(CallsProcStringInClauseTrue);
			std::unordered_set<std::string> expected = { "testB" };
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(CallsProcStringInClauseFalse_ReturnNothing) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(CallsProcStringInClauseFalse);
			std::unordered_set<std::string> expected = {};
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(CallsProcStringNotInClauseTrue_ReturnAllProc) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(CallsProcStringNotInClauseTrue);
			Assert::IsTrue(result == callsAllProc);
		}

		TEST_METHOD(CallsProcStringNotInClauseFalse_ReturnNothing) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(CallsProcStringNotInClauseFalse);
			std::unordered_set<std::string> expected = {};
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(CallsProcAnyInClause_ReturnAllCallerProc) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(CallsProcAnyInClause);
			std::unordered_set<std::string> expected = { "testA", "testB" };
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(CallsProcAnyNotInClause_ReturnAllProc) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(CallsProcAnyNotInClause);
			Assert::IsTrue(result == callsAllProc);
		}

		TEST_METHOD(CallsStringProcInClauseTrue_ReturnAllCalleeOfTestA) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(CallsStringProcInClauseTrue);
			std::unordered_set<std::string> expected = {"testB" };
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(CallsStringProcInClauseFalse_ReturnNothing) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(CallsStringProcInClauseFalse);
			std::unordered_set<std::string> expected = {};
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(CallsStringProcNotInClauseTrue_ReturnAllProc) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(CallsStringProcNotInClauseTrue);
			Assert::IsTrue(result == callsAllProc);
		}

		TEST_METHOD(CallsStringProcNotInClauseFalse_ReturnNothing) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(CallsStringProcNotInClauseFalse);
			std::unordered_set<std::string> expected = {};
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(CallsStringStringTrue_ReturnAllProc) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(CallsStringStringTrue);
			Assert::IsTrue(result == callsAllProc);
		}

		TEST_METHOD(CallsStringStringFalse_ReturnNothing) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(CallsStringStringFalse);
			std::unordered_set<std::string> expected = {};
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(CallsStringAnyTrue_ReturnAllProc) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(CallsStringAnyTrue);
			Assert::IsTrue(result == callsAllProc);
		}

		TEST_METHOD(CallsStringAnyFalse_ReturnNothing) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(CallsStringAnyFalse);
			std::unordered_set<std::string> expected = {};
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(CallsAnyProcInClause_ReturnAllCalleeProc) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(CallsAnyProcInClause);
			std::unordered_set<std::string> expected = {"testB", "testC"};
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(CallsAnyProcNotInClause_ReturnAllProc) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(CallsAnyProcNotInClause);
			Assert::IsTrue(result == callsAllProc);
		}

		TEST_METHOD(CallsAnyStringTrue_ReturnAllProc) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(CallsAnyStringTrue);
			Assert::IsTrue(result == callsAllProc);
		}

		TEST_METHOD(CallsAnyStringFalse_ReturnNothing) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(CallsAnyStringFalse);
			std::unordered_set<std::string> expected = {};
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(CallsAnyAnyTrue_ReturnAllProc) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(CallsAnyAnyTrue);
			Assert::IsTrue(result == callsAllProc);
		}

		TEST_METHOD(CallsEdgeCase1_ReturnNothing) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(CallsEdgeCase1);
			std::unordered_set<std::string> expected = {};
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(CallsEdgeCase2_ReturnNothing) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(CallsEdgeCase2);
			std::unordered_set<std::string> expected = {};
			Assert::IsTrue(result == expected);
		}
	};
}