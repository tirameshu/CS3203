#include "stdafx.h"
#include "CppUnitTest.h"
#include "ParentStarEvaluatorTestData.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace IntegrationTesting
{
	TEST_CLASS(ParentStarEvaluatorTest)
	{
	public:
		TEST_CLASS_INITIALIZE(Initialization) {
			PKB pkb;
			pkb.clear();
			pkb.addStatement("main", { 5, 1 }); // 1:assign, 2:print, 3:read, 4:if, 5:while
			pkb.addStatement("main", { 5, 2 });
			pkb.addStatement("main", { 3, 3 });
			pkb.addStatement("main", { 4, 4 });
			pkb.addStatement("main", { 5, 5 });
			pkb.addStatement("main", { 1, 6 });
			pkb.addStatement("main", { 2, 7 });
			pkb.addStatement("main", { 3, 8 });
			pkb.addStatement("main", { 3, 9 });
			pkb.addStatement("main", { 2, 10 });
			pkb.addStatement("main", { 1, 11 });
			pkb.addStatement("main", { 5, 12 });
			pkb.addStatement("main", { 1, 13 });
			pkb.addStatement("main", { 4, 14 });
			pkb.addStatement("main", { 2, 15 });
			pkb.addStatement("main", { 3, 16 });
			pkb.addStatement("main", { 4, 17 });
			pkb.addStatement("main", { 4, 18 });
			pkb.addStatement("main", { 2, 19 });

			pkb.addParent(1, { 2, 4 });
			pkb.addParent(2, { 3 });
			pkb.addParent(4, { 5, 9, 10, 11 });
			pkb.addParent(5, { 6, 7, 8 });
			pkb.addParent(12, { 13, 14 });
			pkb.addParent(14, { 15, 16 });
			pkb.addParent(17, { 18 });
			pkb.addParent(18, { 19 });

			pkb.addAncestors(2, { 1 });
			pkb.addAncestors(3, { 1, 2 });
			pkb.addAncestors(4, { 1 });
			pkb.addAncestors(5, { 1, 4 });
			pkb.addAncestors(6, { 1, 4, 5 });
			pkb.addAncestors(7, { 1, 4, 5 });
			pkb.addAncestors(8, { 1, 4, 5 });
			pkb.addAncestors(9, { 1, 4 });
			pkb.addAncestors(10, { 1, 4 });
			pkb.addAncestors(11, { 1, 4 });
			pkb.addAncestors(13, { 12 });
			pkb.addAncestors(14, { 12 });
			pkb.addAncestors(15, { 12, 14 });
			pkb.addAncestors(16, { 12, 14 });
			pkb.addAncestors(18, { 17 });
			pkb.addAncestors(19, { 17, 18 });

			pkb.addDescendants(1, { 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 });
			pkb.addDescendants(2, { 3 });
			pkb.addDescendants(4, { 5, 6, 7, 8, 9, 10, 11 });
			pkb.addDescendants(5, { 6, 7, 8 });
			pkb.addDescendants(12, { 13, 14, 15, 16 });
			pkb.addDescendants(14, { 15, 16 });
			pkb.addDescendants(17, { 18, 19 });
			pkb.addDescendants(18, { 19 });
		}

		TEST_METHOD(ParentStarAnyAny_ClauseHoldsTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(ParentStarAnyAny);
			std::unordered_set<std::string> expected = allStmtsParentStar;
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(ParentStarAnyStmt_ResultSynonymInClause) {
			PKB pkb = PKB();
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(ParentStarAnyStmt);
			std::unordered_set<std::string> expected = allDescendants;
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(ParentStarAnyStmt_ResultSynonymNotInClause) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(argNotInSelectParentStarAnyStmt);
			std::unordered_set<std::string> expected = allStmtsParentStar;
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(ParentStarStmtAny_ResultSynonymInClause) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(ParentStarStmtAny);
			std::unordered_set<std::string> expected = allAncestors;
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(ParentStarStmtAny_ResultSynonymNotInClause) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(argNotInSelectParentStarStmtAny);
			std::unordered_set<std::string> expected = allStmtsParentStar;
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(ParentStarStmtStmt_ResultSynonymInClause) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(ParentStarStmtStmt);
			std::unordered_set<std::string> expected = allAncestors;
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(ParentStarStmtStmt_edgeCaseParentStarSameStmt) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(edgeCaseParentStarSameStmt);
			Assert::IsTrue(result.empty());
		}

		TEST_METHOD(ParentStarStmtStmt_ResultSynonymNotInClause) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(argNotInSelectParentStarStmtStmt);
			std::unordered_set<std::string> expected = allStmtsParentStar;
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(ParentStarAnyInt_ClauseHoldsTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(ParentStarAnyInt);
			std::unordered_set<std::string> expected = allStmtsParentStar;
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(ParentStarAnyInt_ClauseNotTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(argNotInSelectParentStarAnyInt);
			Assert::IsTrue(result.empty());
		}

		TEST_METHOD(ParentStarStmtInt_ResultSynonymInClause) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(ParentStarStmtInt);
			std::unordered_set<std::string> expected = { "12", "14" };
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(ParentStarStmtInt_ResultSynonymNotInClause) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(argNotInSelectParentStarStmtInt);
			std::unordered_set<std::string> expected = allStmtsParentStar;
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(ParentStarAnyAssign_ClauseHoldsTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(ParentStarAnyAssign);
			std::unordered_set<std::string> expected = allStmtsParentStar;
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(ParentStarAnyPrint_ClauseHoldsTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(ParentStarAnyPrint);
			std::unordered_set<std::string> expected = allStmtsParentStar;
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(ParentStarAnyRead_ClauseHoldsTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(ParentStarAnyRead);
			std::unordered_set<std::string> expected = allStmtsParentStar;
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(ParentStarAnyIf_ClauseHoldsTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(ParentStarAnyIf);
			std::unordered_set<std::string> expected = allStmtsParentStar;
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(ParentStarAnyWhile_ClauseHoldsTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(ParentStarAnyWhile);
			std::unordered_set<std::string> expected = allStmtsParentStar;
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(ParentStarStmtAssign_ResultSynonymInClause) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(ParentStarStmtAssign);
			std::unordered_set<std::string> expected = ancestorsOfAssign;
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(ParentStarStmtAssign_ResultSynonymNotInClause) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(argNotInSelectParentStarStmtAssign);
			std::unordered_set<std::string> expected = allStmtsParentStar;
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(ParentStarStmtPrint_ResultSynonymInClause) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(ParentStarStmtPrint);
			std::unordered_set<std::string> expected = ancestorsOfPrint;
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(ParentStarStmtPrint_ResultSynonymNotInClause) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(argNotInSelectParentStarStmtPrint);
			std::unordered_set<std::string> expected = allStmtsParentStar;
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(ParentStarStmtRead_ResultSynonymInClause) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(ParentStarStmtRead);
			std::unordered_set<std::string> expected = ancestorsOfRead;
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(ParentStarStmtRead_ResultSynonymNotInClause) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(argNotInSelectParentStarStmtRead);
			std::unordered_set<std::string> expected = allStmtsParentStar;
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(ParentStarStmtIf_ResultSynonymInClause) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(ParentStarStmtIf);
			std::unordered_set<std::string> expected = ancestorsOfIf;
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(ParentStarStmtIf_ResultSynonymNotInClause) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(argNotInSelectParentStarStmtIf);
			std::unordered_set<std::string> expected = allStmtsParentStar;
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(ParentStarStmtWhile_ResultSynonymInClause) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(ParentStarStmtWhile);
			std::unordered_set<std::string> expected = ancestorsOfWhile;
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(ParentStarStmtWhile_ResultSynonymNotInClause) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(argNotInSelectParentStarStmtWhile);
			std::unordered_set<std::string> expected = allStmtsParentStar;
			Assert::IsTrue(result == expected);
		}

		/* ===== Select s such that ParentStar(INT, *) ===== */

		TEST_METHOD(ParentStarIntAny_ClauseHoldsTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(ParentStarIntAnyHasResult);
			std::unordered_set<std::string> expected = allStmtsParentStar;
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(ParentStarIntAny_ClauseNotTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(ParentStarIntAnyNoResult);
			Assert::IsTrue(result.empty());
		}

		TEST_METHOD(ParentStarIntStmt_ResultSynonymInClause_ClauseHoldsTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(ParentStarIntStmtHasResult);
			std::unordered_set<std::string> expected = { "2", "3", "4", "5", "6", "7", "8", "9", "10", "11" };
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(ParentStarIntStmt_ResultSynonymInClause_ClauseNotTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(ParentStarIntStmtNoResult);
			Assert::IsTrue(result.empty());
		}

		TEST_METHOD(ParentStarIntStmt_ResultSynonymNotInClause_ClauseHoldsTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(argNotInSelectParentStarIntStmt);
			std::unordered_set<std::string> expected = allStmtsParentStar;
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(ParentStarIntInt_ClauseHoldsTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(ParentStarIntIntTrue);
			std::unordered_set<std::string> expected = allStmtsParentStar;
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(ParentStarIntInt_ClauseNotTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(ParentStarIntIntFalse);
			Assert::IsTrue(result.empty());
		}

		TEST_METHOD(ParentStarIntInt_NotDirectParentStar_ClauseNotTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(ParentStarIntIntNotDirectParentStar);
			Assert::IsTrue(result.empty());
		}

		TEST_METHOD(ParentStarIntInt_LeftBigger_ClauseNotTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(edgeCaseParentStarIntIntBiggerSmaller);
			Assert::IsTrue(result.empty());
		}

		TEST_METHOD(ParentStarIntInt_SameInt_ClauseNotTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(edgeCaseParentStarIntIntSame);
			Assert::IsTrue(result.empty());
		}

		/* ===== Select s such that ParentStar(INT, *) ===== */

		TEST_METHOD(ParentStarIntAssign_ClauseHoldsTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(ParentStarIntAssignHasResult);
			std::unordered_set<std::string> expected = allStmtsParentStar;
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(ParentStarIntAssign_ClauseNotTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(ParentStarIntAssignNoResult);
			Assert::IsTrue(result.empty());
		}

		TEST_METHOD(ParentStarIntPrint_ClauseHoldsTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(ParentStarIntPrintHasResult);
			std::unordered_set<std::string> expected = allStmtsParentStar;
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(ParentStarIntPrint_ClauseNotTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(ParentStarIntPrintNoResult);
			Assert::IsTrue(result.empty());
		}

		TEST_METHOD(ParentStarIntRead_ClauseHoldsTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(ParentStarIntReadHasResult);
			std::unordered_set<std::string> expected = allStmtsParentStar;
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(ParentStarIntRead_ClauseNotTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(ParentStarIntReadNoResult);
			Assert::IsTrue(result.empty());
		}

		TEST_METHOD(ParentStarIntIf_ClauseHoldsTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(ParentStarIntIfHasResult);
			std::unordered_set<std::string> expected = allStmtsParentStar;
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(ParentStarIntIf_ClauseNotTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(ParentStarIntIfNoResult);
			Assert::IsTrue(result.empty());
		}

		TEST_METHOD(ParentStarIntWhile_ClauseHoldsTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(ParentStarIntWhileHasResult);
			std::unordered_set<std::string> expected = allStmtsParentStar;
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(ParentStarIntWhile_ClauseNotTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(ParentStarIntWhileNoResult);
			Assert::IsTrue(result.empty());
		}

		/* ===== Select s such that ParentStar(*, _) ===== */

		TEST_METHOD(ParentStarAssignAny_ClauseNotTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(ParentStarAssignAny);
			Assert::IsTrue(result.empty());
		}

		TEST_METHOD(ParentStarPrintAny_ClauseNotTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(ParentStarPrintAny);
			Assert::IsTrue(result.empty());
		}

		TEST_METHOD(ParentStarReadAny_ClauseNotTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(ParentStarReadAny);
			Assert::IsTrue(result.empty());
		}

		TEST_METHOD(ParentStarIfAny_ClauseHoldsTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(ParentStarIfAny);
			std::unordered_set<std::string> expected = allStmtsParentStar;
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(ParentStarWhileAny_ClauseHoldsTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(ParentStarWhileAny);
			std::unordered_set<std::string> expected = allStmtsParentStar;
			Assert::IsTrue(result == expected);
		}

		/* ===== Select s such that ParentStar(*, s) ===== */

		TEST_METHOD(ParentStarAssignStmt_ResultSynonymInClause) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(ParentStarAssignStmt);
			Assert::IsTrue(result.empty());
		}

		TEST_METHOD(ParentStarAssignStmt_ResultSynonymNotInClause) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(argNotInSelectParentStarAssignStmt);
			Assert::IsTrue(result.empty());
		}

		TEST_METHOD(ParentStarPrintStmt_ResultSynonymInClause) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(ParentStarPrintStmt);
			Assert::IsTrue(result.empty());
		}

		TEST_METHOD(ParentStarPrintStmt_ResultSynonymNotInClause) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(argNotInSelectParentStarPrintStmt);
			Assert::IsTrue(result.empty());
		}

		TEST_METHOD(ParentStarReadStmt_ResultSynonymInClause) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(ParentStarReadStmt);
			Assert::IsTrue(result.empty());
		}

		TEST_METHOD(ParentStarReadStmt_ResultSynonymNotInClause) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(argNotInSelectParentStarReadStmt);
			Assert::IsTrue(result.empty());
		}

		TEST_METHOD(ParentStarIfStmt_ResultSynonymInClause) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(ParentStarIfStmt);
			std::unordered_set<std::string> expected = descendantsOfIf;
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(ParentStarIfStmt_ResultSynonymNotInClause) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(argNotInSelectParentStarIfStmt);
			std::unordered_set<std::string> expected = allStmtsParentStar;
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(ParentStarWhileStmt_ResultSynonymInClause) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(ParentStarWhileStmt);
			std::unordered_set<std::string> expected = descendantsOfWhile;
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(ParentStarWhileStmt_ResultSynonymNotInClause) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(argNotInSelectParentStarWhileStmt);
			std::unordered_set<std::string> expected = allStmtsParentStar;
			Assert::IsTrue(result == expected);
		}

		/* ===== Select s such that ParentStar(*, INT) ===== */

		TEST_METHOD(ParentStarAssignInt_ClauseNotTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(ParentStarAssignInt);
			Assert::IsTrue(result.empty());
		}

		TEST_METHOD(ParentStarPrintInt_ClauseNotTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(ParentStarPrintInt);
			Assert::IsTrue(result.empty());
		}

		TEST_METHOD(ParentStarReadInt_ClauseNotTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(ParentStarReadInt);
			Assert::IsTrue(result.empty());
		}

		TEST_METHOD(ParentStarIfInt_ClauseHoldsTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(ParentStarIfIntHasResult);
			std::unordered_set<std::string> expected = allStmtsParentStar;
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(ParentStarIfInt_ClauseNotTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(ParentStarIfIntNoResult);
			Assert::IsTrue(result.empty());
		}

		TEST_METHOD(ParentStarWhileInt_ClauseHoldsTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(ParentStarWhileIntHasResult);
			std::unordered_set<std::string> expected = allStmtsParentStar;
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(ParentStarWhileInt_ClauseNotTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(ParentStarWhileIntNoResult);
			Assert::IsTrue(result.empty());
		}

		/* ===== Select s such that ParentStar(a, *) ===== */

		TEST_METHOD(ParentStarAssignAssign_ClauseNotTrue) {
			std::unordered_set<std::string> result1 = QueryEvaluator::evaluateQuery(ParentStarAssignAssign);
			std::unordered_set<std::string> result2 = QueryEvaluator::evaluateQuery(edgeCaseParentStarAssignAssignSame);
			Assert::IsTrue(result1.empty());
			Assert::IsTrue(result2.empty());
		}

		TEST_METHOD(ParentStarAssignPrint_ClauseNotTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(ParentStarAssignPrint);
			Assert::IsTrue(result.empty());
		}

		TEST_METHOD(ParentStarAssignRead_ClauseNotTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(ParentStarAssignRead);
			Assert::IsTrue(result.empty());
		}

		TEST_METHOD(ParentStarAssignIf_ClauseNotTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(ParentStarAssignIf);
			Assert::IsTrue(result.empty());
		}

		TEST_METHOD(ParentStarAssignWhile_ClauseNotTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(ParentStarAssignWhile);
			Assert::IsTrue(result.empty());
		}

		/* ===== Select s such that ParentStar(p, *) ===== */

		TEST_METHOD(ParentStarPrintPrint_ClauseNotTrue) {
			std::unordered_set<std::string> result1 = QueryEvaluator::evaluateQuery(ParentStarPrintPrint);
			std::unordered_set<std::string> result2 = QueryEvaluator::evaluateQuery(edgeCaseParentStarPrintPrintSame);
			Assert::IsTrue(result1.empty());
			Assert::IsTrue(result2.empty());
		}

		TEST_METHOD(ParentStarPrintAssign_ClauseNotTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(ParentStarPrintAssign);
			Assert::IsTrue(result.empty());
		}

		TEST_METHOD(ParentStarPrintRead_ClauseNotTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(ParentStarPrintRead);
			Assert::IsTrue(result.empty());
		}

		TEST_METHOD(ParentStarPrintIf_ClauseNotTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(ParentStarPrintIf);
			Assert::IsTrue(result.empty());
		}

		TEST_METHOD(ParentStarPrintWhile_ClauseNotTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(ParentStarPrintWhile);
			Assert::IsTrue(result.empty());
		}

		/* ===== Select s such that ParentStar(r, *) ===== */

		TEST_METHOD(ParentStarReadRead_ClauseNotTrue) {
			std::unordered_set<std::string> result1 = QueryEvaluator::evaluateQuery(ParentStarReadRead);
			std::unordered_set<std::string> result2 = QueryEvaluator::evaluateQuery(edgeCaseParentStarReadReadSame);
			Assert::IsTrue(result1.empty());
			Assert::IsTrue(result2.empty());
		}

		TEST_METHOD(ParentStarReadAssign_ClauseNotTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(ParentStarReadAssign);
			Assert::IsTrue(result.empty());
		}

		TEST_METHOD(ParentStarReadPrint_ClauseNotTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(ParentStarReadPrint);
			Assert::IsTrue(result.empty());
		}

		TEST_METHOD(ParentStarReadIf_ClauseNotTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(ParentStarReadIf);
			Assert::IsTrue(result.empty());
		}

		TEST_METHOD(ParentStarReadWhile_ClauseNotTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(ParentStarReadWhile);
			Assert::IsTrue(result.empty());
		}

		/* ===== Select s such that ParentStar(i, *) ===== */

		TEST_METHOD(ParentStarIfIf_ClauseHoldsTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(ParentStarIfIf);
			std::unordered_set<std::string> expected = allStmtsParentStar;
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(ParentStarIfIf_ClauseNotTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(edgeCaseParentStarIfIfSame);
			Assert::IsTrue(result.empty());
		}

		TEST_METHOD(ParentStarIfAssign_ClauseHoldsTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(ParentStarIfAssign);
			std::unordered_set<std::string> expected = allStmtsParentStar;
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(ParentStarIfPrint_ClauseHoldsTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(ParentStarIfPrint);
			std::unordered_set<std::string> expected = allStmtsParentStar;
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(ParentStarIfRead_ClauseHoldsTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(ParentStarIfRead);
			std::unordered_set<std::string> expected = allStmtsParentStar;
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(ParentStarIfWhile_ClauseNotTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(ParentStarIfPrint);
			std::unordered_set<std::string> expected = allStmtsParentStar;
			Assert::IsTrue(result == expected);
		}

		/* ===== Select s such that ParentStar(w, *) ===== */

		TEST_METHOD(ParentStarWhileWhile_ClauseHoldsTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(ParentStarWhileWhile);
			std::unordered_set<std::string> expected = allStmtsParentStar;
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(ParentStarWhileWhile_ClauseNotTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(edgeCaseParentStarWhileWhileSame);
			Assert::IsTrue(result.empty());
		}

		TEST_METHOD(ParentStarWhileAssign_ClauseHoldsTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(ParentStarWhileAssign);
			std::unordered_set<std::string> expected = allStmtsParentStar;
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(ParentStarWhilePrint_ClauseNotTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(ParentStarWhilePrint);
			std::unordered_set<std::string> expected = allStmtsParentStar;
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(ParentStarWhileRead_ClauseHoldsTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(ParentStarWhileRead);
			std::unordered_set<std::string> expected = allStmtsParentStar;
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(ParentStarWhileIf_ClauseHoldsTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(ParentStarWhileIf);
			std::unordered_set<std::string> expected = allStmtsParentStar;
			Assert::IsTrue(result == expected);
		}

		TEST_CLASS_CLEANUP(ClearPKB) {
			PKB pkb = PKB();
			pkb.clear();
		}
	};
}