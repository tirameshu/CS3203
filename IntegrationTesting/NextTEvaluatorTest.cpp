#include "stdafx.h"
#include "CppUnitTest.h"
#include "NextTEvaluatorTestData.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace IntegrationTesting
{
	TEST_CLASS(NextTEvaluatorTest)
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

		TEST_METHOD(NextTStmtStmtInClause_ReturnAllPrevStmt) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(NextTStmtStmtInClause);
			Assert::IsTrue(result == nextTPrevList);
		}

		TEST_METHOD(NextTStmtStmtNotInClause_ReturnAllSmt) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(NextTStmtStmtNotInClause);
			Assert::IsTrue(result == nextTAllStmt);
		}

		TEST_METHOD(NextTStmtProgLineInClause_ReturnAllPrevStmt) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(NextTStmtProgLineInClause);
			Assert::IsTrue(result == nextTPrevList);
		}

		TEST_METHOD(NextTProgLineStmtInClause_ReturnAllNextTStmt) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(NextTProgLineStmtInClause);
			Assert::IsTrue(result == nextTNextList);
		}

		TEST_METHOD(NextTStmtAssign_ReturnAllPrevStmtOfAssign) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(NextTStmtAssign);
			std::unordered_set<std::string> expected = { "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "15" };
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(NextTStmtPrint_ReturnAllPrevStmtOfPrint) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(NextTStmtPrint);
			std::unordered_set<std::string> expected = { "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13" };
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(NextTStmtRead_ReturnAllPrevStmtOfRead) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(NextTStmtRead);
			std::unordered_set<std::string> expected = { "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12" };
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(NextTProgLineIf_ReturnAllPrevStmtOfIf) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(NextTProgLineIf);
			std::unordered_set<std::string> expected = { "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11" };
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(NextTProgLineWhile_ReturnAllPrevStmtOfWhile) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(NextTProgLineWhile);
			std::unordered_set<std::string> expected = { "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11" };
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(NextTProgLineCall_ReturnAllPrevStmtOfCall) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(NextTProgLineCall);
			std::unordered_set<std::string> expected = { "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11" };
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(NextTStmtIntTrue_ReturnAllPrevStmtOfStmt) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(NextTStmtIntTrue);
			std::unordered_set<std::string> expected = { "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11" };
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(NextTProgLineIntFalse_ReturnNothing) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(NextTProgLineIntFalse);
			std::unordered_set<std::string> expected = {};
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(NextTStmtAny_ReturnAllPrevStmt) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(NextTStmtAny);
			Assert::IsTrue(result == nextTPrevList);
		}

		TEST_METHOD(NextTProgLineAny_ReturnAllPrevStmt) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(NextTProgLineAny);
			Assert::IsTrue(result == nextTPrevList);
		}

		TEST_METHOD(NextTAssignProgLine_ReturnAllNextTOfAssign) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(NextTAssignProgLine);
			std::unordered_set<std::string> expected = { "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15", "16" };
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(NextTPrintProgLine_ReturnAllNextTOfPrint) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(NextTPrintProgLine);
			std::unordered_set<std::string> expected = { "16" };
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(NextTReadProgLine_ReturnAllNextTOfRead) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(NextTReadProgLine);
			std::unordered_set<std::string> expected = { "5", "6", "7", "8", "9", "10", "11", "12", "13", "14" };
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(NextTIfStmt_ReturnAllNextTOfIf) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(NextTIfStmt);
			std::unordered_set<std::string> expected = { "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15", "16" };
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(NextTWhileStmt_ReturnAllNextTOfWhile) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(NextTWhileStmt);
			std::unordered_set<std::string> expected = { "5", "6", "7", "8", "9", "10", "11", "12", "13", "14" };
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(NextTCallStmt_ReturnAllNextTOfCall) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(NextTCallStmt);
			std::unordered_set<std::string> expected = { "5", "6", "7", "8", "9", "10", "11", "12", "13", "14" };
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(NextTAssignAssign_ReturnAssignPrevStmtsAssign) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(NextTAssignAssign);
			std::unordered_set<std::string> expected = { "1", "2", "4", "9", "11" };
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(NextTAssignPrint_ReturnPrintPrevStmtsAssign) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(NextTAssignPrint);
			std::unordered_set<std::string> expected = { "1", "2", "4", "9", "11", "12" };
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(NextTAssignRead_ReturnReadPrevStmtsAssign) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(NextTAssignRead);
			std::unordered_set<std::string> expected = { "1", "2", "4", "9", "11", "12" };
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(NextTAssignCall_ReturnCallPrevStmtsAssign) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(NextTAssignCall);
			std::unordered_set<std::string> expected = { "1", "2", "4", "9", "11" };
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(NextTPrintAssign_ReturnAssignPrevStmtsPrint) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(NextTPrintAssign);
			std::unordered_set<std::string> expected = { "15" };
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(NextTPrintPrint_ReturnPrintPrevStmtsPrint) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(NextTPrintPrint);
			std::unordered_set<std::string> expected = {};
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(NextTPrintRead_ReturnReadPrevStmtsPrint) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(NextTPrintRead);
			std::unordered_set<std::string> expected = {};
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(NextTPrintIf_ReturnIfPrevStmtsPrint) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(NextTPrintIf);
			std::unordered_set<std::string> expected = {};
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(NextTPrintWhile_ReturnWhilePrevStmtsPrint) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(NextTPrintWhile);
			std::unordered_set<std::string> expected = {};
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(NextTPrintCall_ReturnCallPrevStmtsPrint) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(NextTPrintCall);
			std::unordered_set<std::string> expected = {};
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(NextTReadAssign_ReturnAssignPrevStmtsRead) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(NextTReadAssign);
			std::unordered_set<std::string> expected = { "6", "10" };
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(NextTReadPrint_ReturnPrintPrevStmtsRead) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(NextTReadPrint);
			std::unordered_set<std::string> expected = { "6", "10", "13" };
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(NextTReadRead_ReturnReadPrevStmtsRead) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(NextTReadRead);
			std::unordered_set<std::string> expected = { "6", "10" };
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(NextTReadIf_ReturnIfPrevStmtsRead) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(NextTReadIf);
			std::unordered_set<std::string> expected = { "6", "10" };
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(NextTReadWhile_ReturnWhilePrevStmtsRead) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(NextTReadWhile);
			std::unordered_set<std::string> expected = { "6", "10" };
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(NextTReadCall_ReturnCallPrevStmtsRead) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(NextTReadCall);
			std::unordered_set<std::string> expected = { "6", "10" };
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(NextTIfAssign_ReturnAssignPrevStmtsIf) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(NextTIfAssign);
			std::unordered_set<std::string> expected = { "3", "7" };
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(NextTIfPrint_ReturnPrintPrevStmtsIf) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(NextTIfPrint);
			std::unordered_set<std::string> expected = { "3", "7" };
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(NextTIfRead_ReturnReadPrevStmtsIf) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(NextTIfRead);
			std::unordered_set<std::string> expected = { "3", "7" };
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(NextTIfIf_ReturnIfPrevStmtsIf) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(NextTIfIf);
			std::unordered_set<std::string> expected = { "3", "7" };
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(NextTIfWhile_ReturnWhilePrevStmtsIf) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(NextTIfWhile);
			std::unordered_set<std::string> expected = { "3", "7" };
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(NextTIfCall_ReturnCallPrevStmtsIf) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(NextTIfCall);
			std::unordered_set<std::string> expected = { "3", "7" };
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(NextTWhileAssign_ReturnAssignPrevStmtsWhile) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(NextTWhileAssign);
			std::unordered_set<std::string> expected = { "5" };
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(NextTWhilePrint_ReturnPrintPrevStmtsWhile) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(NextTWhilePrint);
			std::unordered_set<std::string> expected = { "5" };
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(NextTWhileRead_ReturnReadPrevStmtsWhile) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(NextTWhileRead);
			std::unordered_set<std::string> expected = { "5" };
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(NextTWhileIf_ReturnIfPrevStmtsWhile) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(NextTWhileIf);
			std::unordered_set<std::string> expected = { "5" };
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(NextTWhileWhile_ReturnWhilePrevStmtsWhile) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(NextTWhileWhile);
			std::unordered_set<std::string> expected = { "5" };
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(NextTWhileCall_ReturnCallPrevStmtsWhile) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(NextTWhileCall);
			std::unordered_set<std::string> expected = { "5" };
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(NextTCallAssign_ReturnAssignPrevStmtsCall) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(NextTCallAssign);
			std::unordered_set<std::string> expected = { "8" };
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(NextTCallPrint_ReturnPrintPrevStmtsCall) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(NextTCallPrint);
			std::unordered_set<std::string> expected = {"8"};
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(NextTCallRead_ReturnReadPrevStmtsCall) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(NextTCallRead);
			std::unordered_set<std::string> expected = { "8" };
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(NextTCallIf_ReturnIfPrevStmtsCall) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(NextTCallIf);
			std::unordered_set<std::string> expected = { "8" };
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(NextTCallWhile_ReturnWhilePrevStmtsCall) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(NextTCallWhile);
			std::unordered_set<std::string> expected = { "8" };
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(NextTCallCall_ReturnCallPrevStmtsCall) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(NextTCallCall);
			std::unordered_set<std::string> expected = { "8" };
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(NextTIntStmtTrue_ReturnNextTStmtsOfStmt) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(NextTIntStmtTrue);
			std::unordered_set<std::string> expected = { "5", "6", "7", "8", "9", "10", "11", "12", "13", "14" };
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(NextTIntProgLineFalse_ReturnNothing) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(NextTIntProgLineFalse);
			std::unordered_set<std::string> expected = {};
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(NextTIntAssignTrue_ReturnAllStmts) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(NextTIntAssignTrue);
			Assert::IsTrue(result == nextTAllStmt);
		}

		TEST_METHOD(NextTIntPrintTrue_ReturnAllStmts) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(NextTIntPrintTrue);
			Assert::IsTrue(result == nextTAllStmt);
		}

		TEST_METHOD(NextTIntReadTrue_ReturnAllStmts) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(NextTIntReadTrue);
			Assert::IsTrue(result == nextTAllStmt);
		}

		TEST_METHOD(NextTIntIfTrue_ReturnAllStmts) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(NextTIntIfTrue);
			Assert::IsTrue(result == nextTAllStmt);
		}

		TEST_METHOD(NextTIntIfFalse_ReturnNothing) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(NextTIntIfFalse);
			std::unordered_set<std::string> expected = {};
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(NextTIntWhileTrue_ReturnAllStmts) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(NextTIntWhileTrue);
			Assert::IsTrue(result == nextTAllStmt);
		}

		TEST_METHOD(NextTIntWhileFalse_ReturnNothing) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(NextTIntWhileFalse);
			std::unordered_set<std::string> expected = {};
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(NextTIntCallTrue_ReturnAllStmts) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(NextTIntCallTrue);
			Assert::IsTrue(result == nextTAllStmt);
		}

		TEST_METHOD(NextTIntCallFalse_ReturnNothing) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(NextTIntCallFalse);
			std::unordered_set<std::string> expected = {};
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(NextTIntIntTrue_ReturnAllStmts) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(NextTIntIntTrue);
			Assert::IsTrue(result == nextTAllStmt);
		}

		TEST_METHOD(NextTIntIntFalse_ReturnNothing) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(NextTIntIntFalse);
			std::unordered_set<std::string> expected = {};
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(NextTIntAnyTrue_ReturnAllStmts) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(NextTIntAnyTrue);
			Assert::IsTrue(result == nextTAllStmt);
		}

		TEST_METHOD(NextTIntAnyFalse_ReturnNothing) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(NextTIntAnyFalse);
			std::unordered_set<std::string> expected = {};
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(NextTAnyProgLineTrue_ReturnAllStmts) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(NextTAnyProgLineTrue);
			Assert::IsTrue(result == nextTNextList);
		}

		TEST_METHOD(NextTAnyStmtTrue_ReturnAllStmts) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(NextTAnyStmtTrue);
			Assert::IsTrue(result == nextTNextList);
		}

		TEST_METHOD(NextTAnyAssign_ReturnAllStmts) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(NextTAnyAssign);
			Assert::IsTrue(result == nextTAllStmt);
		}

		TEST_METHOD(NextTAnyPrint_ReturnAllStmts) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(NextTAnyPrint);
			Assert::IsTrue(result == nextTAllStmt);
		}

		TEST_METHOD(NextTAnyRead_ReturnAllStmts) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(NextTAnyRead);
			Assert::IsTrue(result == nextTAllStmt);
		}

		TEST_METHOD(NextTAnyIf_ReturnAllStmts) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(NextTAnyIf);
			Assert::IsTrue(result == nextTAllStmt);
		}

		TEST_METHOD(NextTAnyWhile_ReturnAllStmts) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(NextTAnyWhile);
			Assert::IsTrue(result == nextTAllStmt);
		}

		TEST_METHOD(NextTAnyCall_ReturnAllStmts) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(NextTAnyCall);
			Assert::IsTrue(result == nextTAllStmt);
		}

		TEST_METHOD(NextTAnyIntTrue_ReturnAllStmts) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(NextTAnyIntTrue);
			Assert::IsTrue(result == nextTAllStmt);
		}

		TEST_METHOD(NextTAnyIntFalse_ReturnNothing) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(NextTAnyIntFalse);
			std::unordered_set<std::string> expected = {};
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(NextTAnyAny_ReturnAllStmts) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(NextTAnyAny);
			Assert::IsTrue(result == nextTAllStmt);
		}

		TEST_METHOD(NextTEdgeCase1_ReturnNothing) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(NextTEdgeCase1);
			std::unordered_set<std::string> expected = {};
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(NextTEdgeCase2_ReturnNothing) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(NextTEdgeCase2);
			Assert::IsTrue(result == nextTAllStmt);
		}
	};
}