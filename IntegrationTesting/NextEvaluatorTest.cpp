#include "stdafx.h"
#include "CppUnitTest.h"
#include "NextEvaluatorTestData.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace IntegrationTesting
{
	TEST_CLASS(NextEvaluatorTest)
	{
	public:
		TEST_CLASS_INITIALIZE(Initialization) {
			PKB pkb;
			pkb.clear();
			pkb.addStatement("testA", { ASSIGN, 1 });
			pkb.addStatement("testA", { ASSIGN, 2 });
			pkb.addStatement("testA", { IF, 3 });
			pkb.addStatement("testA", { ASSIGN, 4 });
			pkb.addStatement("testA", { WHILE, 5 });
			pkb.addStatement("testA", { READ, 6 });
			pkb.addStatement("testA", { IF, 7 });
			pkb.addStatement("testA", { CALL, 8 });
			pkb.addStatement("testA", { ASSIGN, 9 });
			pkb.addStatement("testA", { READ, 10 });
			pkb.addStatement("testA", { ASSIGN, 11 });
			pkb.addStatement("testA", { ASSIGN, 12 });
			pkb.addStatement("testA", { READ, 13 });
			pkb.addStatement("testA", { PRINT, 14 });
			pkb.addStatement("testA", { PRINT, 15 });
			pkb.addStatement("testA", { ASSIGN, 16 });

			pkb.addNext(1, 2);
			pkb.addNext(2, 3);
			pkb.addNext(3, 4);
			pkb.addNext(3, 15);
			pkb.addNext(4, 5);
			pkb.addNext(5, 6);
			pkb.addNext(6, 7);
			pkb.addNext(7, 8);
			pkb.addNext(7, 10);
			pkb.addNext(8, 9);
			pkb.addNext(9, 11);
			pkb.addNext(10, 11);
			pkb.addNext(11, 5);
			pkb.addNext(11, 12);
			pkb.addNext(12, 13);
			pkb.addNext(13, 14);
			pkb.addNext(15, 16);

			pkb.addAllNext(1, { 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 });
			pkb.addAllNext(2, { 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 });
			pkb.addAllNext(3, { 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 });
			pkb.addAllNext(4, { 5, 6, 7, 8, 9, 10, 11, 12, 13, 14 });
			pkb.addAllNext(5, { 5, 6, 7, 8, 9, 10, 11, 12, 13, 14 });
			pkb.addAllNext(6, { 5, 6, 7, 8, 9, 10, 11, 12, 13, 14 });
			pkb.addAllNext(7, { 5, 6, 7, 8, 9, 10, 11, 12, 13, 14 });
			pkb.addAllNext(8, { 5, 6, 7, 8, 9, 10, 11, 12, 13, 14 });
			pkb.addAllNext(9, { 5, 6, 7, 8, 9, 10, 11, 12, 13, 14 });
			pkb.addAllNext(10, { 5, 6, 7, 8, 9, 10, 11, 12, 13, 14 });
			pkb.addAllNext(11, { 5, 6, 7, 8, 9, 10, 11, 12, 13, 14 });
			pkb.addAllNext(12, { 13, 14 });
			pkb.addAllNext(13, { 14 });
			pkb.addAllNext(15, { 16 });

			pkb.addAllPrevious(2, { 1 });
			pkb.addAllPrevious(3, { 1, 2 });
			pkb.addAllPrevious(4, { 1, 2, 3 });
			pkb.addAllPrevious(5, { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 });
			pkb.addAllPrevious(6, { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 });
			pkb.addAllPrevious(7, { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 });
			pkb.addAllPrevious(8, { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 });
			pkb.addAllPrevious(9, { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 });
			pkb.addAllPrevious(10, { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 });
			pkb.addAllPrevious(11, { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 });
			pkb.addAllPrevious(12, { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 });
			pkb.addAllPrevious(13, { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 });
			pkb.addAllPrevious(14, { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13 });
			pkb.addAllPrevious(15, { 1, 2, 3 });
			pkb.addAllPrevious(16, { 1, 2, 3, 15 });
		}

		TEST_METHOD(NextStmtStmtInClause_ReturnAllPrevStmt) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(NextStmtStmtInClause);
			Assert::IsTrue(result == nextPrevList);
		}

		TEST_METHOD(NextStmtStmtNotInClause_ReturnAllSmt) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(NextStmtStmtNotInClause);
			Assert::IsTrue(result == nextAllStmt);
		}
	
		TEST_METHOD(NextStmtProgLineInClause_ReturnAllPrevStmt) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(NextStmtProgLineInClause);
			Assert::IsTrue(result == nextPrevList);
		}

		TEST_METHOD(NextProgLineStmtInClause_ReturnAllNextStmt) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(NextProgLineStmtInClause);
			Assert::IsTrue(result == nextNextList);
		}

		TEST_METHOD(NextStmtAssign_ReturnAllPrevStmtOfAssign) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(NextStmtAssign);
			std::unordered_set<std::string> expected = { "1", "3", "8", "9", "10", "11", "15" };
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(NextStmtPrint_ReturnAllPrevStmtOfPrint) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(NextStmtPrint);
			std::unordered_set<std::string> expected = { "3", "13" };
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(NextStmtRead_ReturnAllPrevStmtOfRead) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(NextStmtRead);
			std::unordered_set<std::string> expected = { "5", "7", "12" };
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(NextProgLineIf_ReturnAllPrevStmtOfIf) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(NextProgLineIf);
			std::unordered_set<std::string> expected = { "2", "6" };
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(NextProgLineWhile_ReturnAllPrevStmtOfWhile) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(NextProgLineWhile);
			std::unordered_set<std::string> expected = { "4", "11" };
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(NextProgLineCall_ReturnAllPrevStmtOfCall) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(NextProgLineCall);
			std::unordered_set<std::string> expected = { "7" };
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(NextStmtIntTrue_ReturnAllPrevStmtOfStmt) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(NextStmtIntTrue);
			std::unordered_set<std::string> expected = { "9", "10" };
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(NextProgLineIntFalse_ReturnNothing) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(NextProgLineIntFalse);
			std::unordered_set<std::string> expected = {};
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(NextStmtAny_ReturnAllPrevStmt) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(NextStmtAny);
			Assert::IsTrue(result == nextPrevList);
		}

		TEST_METHOD(NextProgLineAny_ReturnAllPrevStmt) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(NextProgLineAny);
			Assert::IsTrue(result == nextPrevList);
		}

		TEST_METHOD(NextAssignProgLine_ReturnAllNextOfAssign) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(NextAssignProgLine);
			std::unordered_set<std::string> expected = { "2", "3", "5", "11", "12", "13" };
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(NextPrintProgLine_ReturnAllNextOfPrint) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(NextPrintProgLine);
			std::unordered_set<std::string> expected = { "16" };
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(NextReadProgLine_ReturnAllNextOfRead) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(NextReadProgLine);
			std::unordered_set<std::string> expected = { "7", "11", "14" };
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(NextIfStmt_ReturnAllNextOfIf) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(NextIfStmt);
			std::unordered_set<std::string> expected = { "4", "15", "8", "10" };
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(NextWhileStmt_ReturnAllNextOfWhile) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(NextWhileStmt);
			std::unordered_set<std::string> expected = { "6" };
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(NextCallStmt_ReturnAllNextOfCall) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(NextCallStmt);
			std::unordered_set<std::string> expected = { "9" };
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(NextAssignAssign_ReturnAssignPrevStmtsAssign) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(NextAssignAssign);
			std::unordered_set<std::string> expected = { "1", "9", "11" };
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(NextAssignPrint_ReturnPrintPrevStmtsAssign) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(NextAssignPrint);
			std::unordered_set<std::string> expected = {};
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(NextAssignRead_ReturnReadPrevStmtsAssign) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(NextAssignRead);
			std::unordered_set<std::string> expected = { "12" };
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(NextAssignCall_ReturnCallPrevStmtsAssign) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(NextAssignCall);
			std::unordered_set<std::string> expected = {};
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(NextPrintAssign_ReturnAssignPrevStmtsPrint) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(NextPrintAssign);
			std::unordered_set<std::string> expected = {"15"};
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(NextPrintPrint_ReturnPrintPrevStmtsPrint) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(NextPrintPrint);
			std::unordered_set<std::string> expected = {};
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(NextPrintRead_ReturnReadPrevStmtsPrint) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(NextPrintRead);
			std::unordered_set<std::string> expected = {};
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(NextPrintIf_ReturnIfPrevStmtsPrint) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(NextPrintIf);
			std::unordered_set<std::string> expected = {};
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(NextPrintWhile_ReturnWhilePrevStmtsPrint) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(NextPrintWhile);
			std::unordered_set<std::string> expected = {};
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(NextPrintCall_ReturnCallPrevStmtsPrint) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(NextPrintCall);
			std::unordered_set<std::string> expected = {};
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(NextReadAssign_ReturnAssignPrevStmtsRead) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(NextReadAssign);
			std::unordered_set<std::string> expected = { "10" };
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(NextReadPrint_ReturnPrintPrevStmtsRead) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(NextReadPrint);
			std::unordered_set<std::string> expected = { "13" };
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(NextReadRead_ReturnReadPrevStmtsRead) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(NextReadRead);
			std::unordered_set<std::string> expected = {};
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(NextReadIf_ReturnIfPrevStmtsRead) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(NextReadIf);
			std::unordered_set<std::string> expected = { "6" };
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(NextReadWhile_ReturnWhilePrevStmtsRead) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(NextReadWhile);
			std::unordered_set<std::string> expected = {};
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(NextReadCall_ReturnCallPrevStmtsRead) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(NextReadCall);
			std::unordered_set<std::string> expected = {};
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(NextIfAssign_ReturnAssignPrevStmtsIf) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(NextIfAssign);
			std::unordered_set<std::string> expected = { "3" };
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(NextIfPrint_ReturnPrintPrevStmtsIf) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(NextIfPrint);
			std::unordered_set<std::string> expected = { "3" };
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(NextIfRead_ReturnReadPrevStmtsIf) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(NextIfRead);
			std::unordered_set<std::string> expected = {"7"};
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(NextIfIf_ReturnIfPrevStmtsIf) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(NextIfIf);
			std::unordered_set<std::string> expected = {};
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(NextIfWhile_ReturnWhilePrevStmtsIf) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(NextIfWhile);
			std::unordered_set<std::string> expected = {};
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(NextIfCall_ReturnCallPrevStmtsIf) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(NextIfCall);
			std::unordered_set<std::string> expected = {"7"};
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(NextWhileAssign_ReturnAssignPrevStmtsWhile) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(NextWhileAssign);
			std::unordered_set<std::string> expected = {};
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(NextWhilePrint_ReturnPrintPrevStmtsWhile) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(NextWhilePrint);
			std::unordered_set<std::string> expected = {};
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(NextWhileRead_ReturnReadPrevStmtsWhile) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(NextWhileRead);
			std::unordered_set<std::string> expected = {"5"};
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(NextWhileIf_ReturnIfPrevStmtsWhile) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(NextWhileIf);
			std::unordered_set<std::string> expected = {};
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(NextWhileWhile_ReturnWhilePrevStmtsWhile) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(NextWhileWhile);
			std::unordered_set<std::string> expected = {};
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(NextWhileCall_ReturnCallPrevStmtsWhile) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(NextWhileCall);
			std::unordered_set<std::string> expected = {};
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(NextCallAssign_ReturnAssignPrevStmtsCall) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(NextCallAssign);
			std::unordered_set<std::string> expected = {"8"};
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(NextCallPrint_ReturnPrintPrevStmtsCall) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(NextCallPrint);
			std::unordered_set<std::string> expected = {};
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(NextCallRead_ReturnReadPrevStmtsCall) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(NextCallRead);
			std::unordered_set<std::string> expected = {};
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(NextCallIf_ReturnIfPrevStmtsCall) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(NextCallIf);
			std::unordered_set<std::string> expected = {};
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(NextCallWhile_ReturnWhilePrevStmtsCall) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(NextCallWhile);
			std::unordered_set<std::string> expected = {};
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(NextCallCall_ReturnCallPrevStmtsCall) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(NextCallCall);
			std::unordered_set<std::string> expected = {};
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(NextIntStmtTrue_ReturnNextStmtsOfStmt) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(NextIntStmtTrue);
			std::unordered_set<std::string> expected = {"8", "10"};
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(NextIntProgLineFalse_ReturnNothing) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(NextIntProgLineFalse);
			std::unordered_set<std::string> expected = {};
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(NextIntAssignTrue_ReturnAllStmts) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(NextIntAssignTrue);
			Assert::IsTrue(result == nextAllStmt);
		}

		TEST_METHOD(NextIntAssignFalse_ReturnNothing) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(NextIntAssignFalse);
			std::unordered_set<std::string> expected = {};
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(NextIntPrintTrue_ReturnAllStmts) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(NextIntPrintTrue);
			Assert::IsTrue(result == nextAllStmt);
		}

		TEST_METHOD(NextIntPrintFalse_ReturnNothing) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(NextIntPrintFalse);
			std::unordered_set<std::string> expected = {};
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(NextIntReadTrue_ReturnAllStmts) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(NextIntReadTrue);
			Assert::IsTrue(result == nextAllStmt);
		}

		TEST_METHOD(NextIntReadFalse_ReturnNothing) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(NextIntReadFalse);
			std::unordered_set<std::string> expected = {};
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(NextIntIfTrue_ReturnAllStmts) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(NextIntIfTrue);
			Assert::IsTrue(result == nextAllStmt);
		}

		TEST_METHOD(NextIntIfFalse_ReturnNothing) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(NextIntIfFalse);
			std::unordered_set<std::string> expected = {};
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(NextIntWhileTrue_ReturnAllStmts) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(NextIntWhileTrue);
			Assert::IsTrue(result == nextAllStmt);
		}

		TEST_METHOD(NextIntWhileFalse_ReturnNothing) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(NextIntWhileFalse);
			std::unordered_set<std::string> expected = {};
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(NextIntCallTrue_ReturnAllStmts) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(NextIntCallTrue);
			Assert::IsTrue(result == nextAllStmt);
		}

		TEST_METHOD(NextIntCallFalse_ReturnNothing) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(NextIntCallFalse);
			std::unordered_set<std::string> expected = {};
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(NextIntIntTrue_ReturnAllStmts) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(NextIntIntTrue);
			Assert::IsTrue(result == nextAllStmt);
		}

		TEST_METHOD(NextIntIntFalse_ReturnNothing) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(NextIntIntFalse);
			std::unordered_set<std::string> expected = {};
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(NextIntAnyTrue_ReturnAllStmts) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(NextIntAnyTrue);
			Assert::IsTrue(result == nextAllStmt);
		}

		TEST_METHOD(NextIntAnyFalse_ReturnNothing) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(NextIntAnyFalse);
			std::unordered_set<std::string> expected = {};
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(NextAnyProgLineTrue_ReturnAllStmts) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(NextAnyProgLineTrue);
			Assert::IsTrue(result == nextNextList);
		}

		TEST_METHOD(NextAnyStmtTrue_ReturnAllStmts) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(NextAnyStmtTrue);
			Assert::IsTrue(result == nextNextList);
		}

		TEST_METHOD(NextAnyAssign_ReturnAllStmts) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(NextAnyAssign);
			Assert::IsTrue(result == nextAllStmt);
		}

		TEST_METHOD(NextAnyPrint_ReturnAllStmts) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(NextAnyPrint);
			Assert::IsTrue(result == nextAllStmt);
		}

		TEST_METHOD(NextAnyRead_ReturnAllStmts) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(NextAnyRead);
			Assert::IsTrue(result == nextAllStmt);
		}

		TEST_METHOD(NextAnyIf_ReturnAllStmts) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(NextAnyIf);
			Assert::IsTrue(result == nextAllStmt);
		}

		TEST_METHOD(NextAnyWhile_ReturnAllStmts) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(NextAnyWhile);
			Assert::IsTrue(result == nextAllStmt);
		}

		TEST_METHOD(NextAnyCall_ReturnAllStmts) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(NextAnyCall);
			Assert::IsTrue(result == nextAllStmt);
		}

		TEST_METHOD(NextAnyIntTrue_ReturnAllStmts) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(NextAnyIntTrue);
			Assert::IsTrue(result == nextAllStmt);
		}

		TEST_METHOD(NextAnyIntFalse_ReturnNothing) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(NextAnyIntFalse);
			std::unordered_set<std::string> expected = {};
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(NextAnyAny_ReturnAllStmts) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(NextAnyAny);
			Assert::IsTrue(result == nextAllStmt);
		}

		TEST_METHOD(NextEdgeCase1_ReturnNothing) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(NextEdgeCase1);
			std::unordered_set<std::string> expected = {};
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(NextEdgeCase2_ReturnNothing) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(NextEdgeCase2);
			std::unordered_set<std::string> expected = {};
			Assert::IsTrue(result == expected);
		}
	};
}