#include "stdafx.h"
#include "CppUnitTest.h"
#include "ParentEvaluatorTestData.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace IntegrationTesting
{
	TEST_CLASS(ParentEvaluatorTest)
	{
	public:
		TEST_CLASS_INITIALIZE(Initialization) {
			PKB pkb;
			pkb.clear();
			pkb.addStatement("main", { 5, 1 }); // 1:assign, 2:print, 3:read, 4:if, 5:while
			pkb.addStatement("main", { 5, 2 });
			pkb.addStatement("main", { 3, 3 });
			pkb.addStatement("main", { 1, 4 });
			pkb.addStatement("main", { 4, 5 });
			pkb.addStatement("main", { 1, 6 });
			pkb.addStatement("main", { CALL, 7 });
			pkb.addStatement("main", { 4, 8 });
			pkb.addStatement("main", { 1, 9 });
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
			pkb.addStatement("main2", { ASSIGN, 20 });
			pkb.addParent(1, { 2 });
			pkb.addParent(2, { 3 });
			pkb.addParent(5, { 6, 7 });
			pkb.addParent(8, { 9, 10, 11 });
			pkb.addParent(12, { 13, 14 });
			pkb.addParent(14, { 15, 16 });
			pkb.addParent(17, { 18 });
			pkb.addParent(18, { 19 });
		}

		TEST_METHOD(ParentAnyAny_ClauseHoldsTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(ParentAnyAny);
			std::unordered_set<std::string> expected = allStmts;
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(ParentAnyStmt_ResultSynonymInClause) {
			PKB pkb = PKB();
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(ParentAnyStmt);
			std::unordered_set<std::string> expected = allChildren;
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(ParentAnyStmt_ResultSynonymNotInClause) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(argNotInSelectParentAnyStmt);
			std::unordered_set<std::string> expected = allStmts;
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(ParentStmtAny_ResultSynonymInClause) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(ParentStmtAny);
			std::unordered_set<std::string> expected = allParents;
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(ParentStmtAny_ResultSynonymNotInClause) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(argNotInSelectParentStmtAny);
			std::unordered_set<std::string> expected = allStmts;
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(ParentStmtStmt_ResultSynonymInClause) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(ParentStmtStmt);
			std::unordered_set<std::string> expected = allParents;
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(ParentStmtStmt_edgeCaseParentSameStmt) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(edgeCaseParentSameStmt);
			Assert::IsTrue(result.empty());
		}

		TEST_METHOD(ParentStmtStmt_ResultSynonymNotInClause) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(argNotInSelectParentStmtStmt);
			std::unordered_set<std::string> expected = allStmts;
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(ParentAnyInt_ClauseHoldsTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(ParentAnyInt);
			std::unordered_set<std::string> expected = allStmts;
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(ParentAnyInt_ClauseNotTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(argNotInSelectParentAnyInt);
			Assert::IsTrue(result.empty());
		}

		TEST_METHOD(ParentAnyInt_ClauseNotTrue1) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(invalidStmtNoParentAnyInt1);
			Assert::IsTrue(result.empty());
		}

		TEST_METHOD(ParentAnyInt_ClauseNotTrue2) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(invalidStmtNoParentAnyInt2);
			Assert::IsTrue(result.empty());
		}

		TEST_METHOD(ParentStmtInt_ResultSynonymInClause) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(ParentStmtInt);
			std::unordered_set<std::string> expected = { "14" };
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(ParentStmtInt_ResultSynonymNotInClause) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(argNotInSelectParentStmtInt);
			std::unordered_set<std::string> expected = allStmts;
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(ParentAnyAssign_ClauseHoldsTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(ParentAnyAssign);
			std::unordered_set<std::string> expected = allStmts;
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(ParentAnyPrint_ClauseHoldsTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(ParentAnyPrint);
			std::unordered_set<std::string> expected = allStmts;
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(ParentAnyRead_ClauseHoldsTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(ParentAnyRead);
			std::unordered_set<std::string> expected = allStmts;
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(ParentAnyIf_ClauseHoldsTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(ParentAnyIf);
			std::unordered_set<std::string> expected = allStmts;
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(ParentAnyWhile_ClauseHoldsTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(ParentAnyWhile);
			std::unordered_set<std::string> expected = allStmts;
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(ParentStmtAssign_ResultSynonymInClause) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(ParentStmtAssign);
			std::unordered_set<std::string> expected = parentsOfAssign;
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(ParentStmtAssign_ResultSynonymNotInClause) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(argNotInSelectParentStmtAssign);
			std::unordered_set<std::string> expected = allStmts;
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(ParentStmtPrint_ResultSynonymInClause) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(ParentStmtPrint);
			std::unordered_set<std::string> expected = parentsOfPrint;
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(ParentStmtPrint_ResultSynonymNotInClause) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(argNotInSelectParentStmtPrint);
			std::unordered_set<std::string> expected = allStmts;
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(ParentStmtRead_ResultSynonymInClause) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(ParentStmtRead);
			std::unordered_set<std::string> expected = parentsOfRead;
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(ParentStmtRead_ResultSynonymNotInClause) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(argNotInSelectParentStmtRead);
			std::unordered_set<std::string> expected = allStmts;
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(ParentStmtIf_ResultSynonymInClause) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(ParentStmtIf);
			std::unordered_set<std::string> expected = parentsOfIf;
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(ParentStmtIf_ResultSynonymNotInClause) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(argNotInSelectParentStmtIf);
			std::unordered_set<std::string> expected = allStmts;
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(ParentStmtWhile_ResultSynonymInClause) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(ParentStmtWhile);
			std::unordered_set<std::string> expected = parentsOfWhile;
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(ParentStmtWhile_ResultSynonymNotInClause) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(argNotInSelectParentStmtWhile);
			std::unordered_set<std::string> expected = allStmts;
			Assert::IsTrue(result == expected);
		}

		/* ===== Select s such that Parent(INT, *) ===== */

		TEST_METHOD(ParentIntAny_ClauseHoldsTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(ParentIntAnyHasResult);
			std::unordered_set<std::string> expected = allStmts;
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(ParentIntAny_ClauseNotTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(ParentIntAnyNoResult);
			Assert::IsTrue(result.empty());
		}

		TEST_METHOD(ParentIntAny_ClauseNotTrue1) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(ParentIntAnyInvalidStmtNo1);
			Assert::IsTrue(result.empty());
		}

		TEST_METHOD(ParentIntAny_ClauseNotTrue2) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(ParentIntAnyInvalidStmtNo1);
			Assert::IsTrue(result.empty());
		}

		TEST_METHOD(ParentIntStmt_ResultSynonymInClause_ClauseHoldsTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(ParentIntStmtHasResult);
			std::unordered_set<std::string> expected = { "9", "10", "11" };
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(ParentIntStmt_ResultSynonymInClause_ClauseNotTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(ParentIntStmtNoResult);
			std::unordered_set<std::string> expected = { "15", "16" };
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(ParentIntStmt_ResultSynonymNotInClause_ClauseHoldsTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(argNotInSelectParentIntStmt);
			std::unordered_set<std::string> expected = allStmts;
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(ParentIntInt_ClauseHoldsTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(ParentIntIntTrue);
			std::unordered_set<std::string> expected = allStmts;
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(ParentIntInt_ClauseNotTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(ParentIntIntFalse);
			Assert::IsTrue(result.empty());
		}

		TEST_METHOD(ParentIntInt_NotDirectParent_ClauseNotTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(ParentIntIntNotDirectParent);
			Assert::IsTrue(result.empty());
		}

		TEST_METHOD(ParentIntInt_LeftBigger_ClauseNotTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(edgeCaseParentIntIntBiggerSmaller);
			Assert::IsTrue(result.empty());
		}

		TEST_METHOD(ParentIntInt_SameInt_ClauseNotTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(edgeCaseParentIntIntSame);
			Assert::IsTrue(result.empty());
		}

		/* ===== Select s such that Parent(INT, *) ===== */

		TEST_METHOD(ParentIntAssign_ClauseHoldsTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(ParentIntAssignHasResult);
			std::unordered_set<std::string> expected = allStmts;
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(ParentIntAssign_ClauseNotTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(ParentIntAssignNoResult);
			Assert::IsTrue(result.empty());
		}

		TEST_METHOD(ParentIntPrint_ClauseHoldsTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(ParentIntPrintHasResult);
			std::unordered_set<std::string> expected = allStmts;
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(ParentIntPrint_ClauseNotTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(ParentIntPrintNoResult);
			Assert::IsTrue(result.empty());
		}

		TEST_METHOD(ParentIntRead_ClauseHoldsTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(ParentIntReadHasResult);
			std::unordered_set<std::string> expected = allStmts;
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(ParentIntRead_ClauseNotTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(ParentIntReadNoResult);
			Assert::IsTrue(result.empty());
		}

		TEST_METHOD(ParentIntIf_ClauseHoldsTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(ParentIntIfHasResult);
			std::unordered_set<std::string> expected = allStmts;
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(ParentIntIf_ClauseNotTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(ParentIntIfNoResult);
			Assert::IsTrue(result.empty());
		}

		TEST_METHOD(ParentIntWhile_ClauseHoldsTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(ParentIntWhileHasResult);
			std::unordered_set<std::string> expected = allStmts;
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(ParentIntWhile_ClauseNotTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(ParentIntWhileNoResult);
			Assert::IsTrue(result.empty());
		}

		/* ===== Select s such that Parent(*, _) ===== */

		TEST_METHOD(ParentAssignAny_ClauseNotTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(ParentAssignAny);
			Assert::IsTrue(result.empty());
		}

		TEST_METHOD(ParentPrintAny_ClauseNotTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(ParentPrintAny);
			Assert::IsTrue(result.empty());
		}

		TEST_METHOD(ParentReadAny_ClauseNotTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(ParentReadAny);
			Assert::IsTrue(result.empty());
		}

		TEST_METHOD(ParentIfAny_ClauseHoldsTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(ParentIfAny);
			std::unordered_set<std::string> expected = allStmts;
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(ParentWhileAny_ClauseHoldsTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(ParentWhileAny);
			std::unordered_set<std::string> expected = allStmts;
			Assert::IsTrue(result == expected);
		}

		/* ===== Select s such that Parent(*, s) ===== */

		TEST_METHOD(ParentAssignStmt_ResultSynonymInClause) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(ParentAssignStmt);
			Assert::IsTrue(result.empty());
		}

		TEST_METHOD(ParentAssignStmt_ResultSynonymNotInClause) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(argNotInSelectParentAssignStmt);
			Assert::IsTrue(result.empty());
		}

		TEST_METHOD(ParentPrintStmt_ResultSynonymInClause) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(ParentPrintStmt);
			Assert::IsTrue(result.empty());
		}

		TEST_METHOD(ParentPrintStmt_ResultSynonymNotInClause) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(argNotInSelectParentPrintStmt);
			Assert::IsTrue(result.empty());
		}

		TEST_METHOD(ParentReadStmt_ResultSynonymInClause) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(ParentReadStmt);
			Assert::IsTrue(result.empty());
		}

		TEST_METHOD(ParentReadStmt_ResultSynonymNotInClause) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(argNotInSelectParentReadStmt);
			Assert::IsTrue(result.empty());
		}

		TEST_METHOD(ParentIfStmt_ResultSynonymInClause) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(ParentIfStmt);
			std::unordered_set<std::string> expected = childrenOfIf;
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(ParentIfStmt_ResultSynonymNotInClause) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(argNotInSelectParentIfStmt);
			std::unordered_set<std::string> expected = allStmts;
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(ParentWhileStmt_ResultSynonymInClause) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(ParentWhileStmt);
			std::unordered_set<std::string> expected = childrenOfWhile;
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(ParentWhileStmt_ResultSynonymNotInClause) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(argNotInSelectParentWhileStmt);
			std::unordered_set<std::string> expected = allStmts;
			Assert::IsTrue(result == expected);
		}

		/* ===== Select s such that Parent(*, INT) ===== */

		TEST_METHOD(ParentAssignInt_ClauseNotTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(ParentAssignInt);
			Assert::IsTrue(result.empty());
		}

		TEST_METHOD(ParentPrintInt_ClauseNotTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(ParentPrintInt);
			Assert::IsTrue(result.empty());
		}

		TEST_METHOD(ParentReadInt_ClauseNotTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(ParentReadInt);
			Assert::IsTrue(result.empty());
		}

		TEST_METHOD(ParentIfInt_ClauseHoldsTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(ParentIfIntHasResult);
			std::unordered_set<std::string> expected = allStmts;
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(ParentIfInt_ClauseNotTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(ParentIfIntNoResult);
			Assert::IsTrue(result.empty());
		}

		TEST_METHOD(ParentWhileInt_ClauseHoldsTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(ParentWhileIntHasResult);
			std::unordered_set<std::string> expected = allStmts;
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(ParentWhileInt_ClauseNotTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(ParentWhileIntNoResult);
			Assert::IsTrue(result.empty());
		}

		/* ===== Select s such that Parent(a, *) ===== */

		TEST_METHOD(ParentAssignAssign_ClauseNotTrue) {
			std::unordered_set<std::string> result1 = QueryEvaluator::evaluateQuery(ParentAssignAssign);
			std::unordered_set<std::string> result2 = QueryEvaluator::evaluateQuery(edgeCaseParentAssignAssignSame);
			Assert::IsTrue(result1.empty());
			Assert::IsTrue(result2.empty());
		}

		TEST_METHOD(ParentAssignPrint_ClauseNotTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(ParentAssignPrint);
			Assert::IsTrue(result.empty());
		}

		TEST_METHOD(ParentAssignRead_ClauseNotTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(ParentAssignRead);
			Assert::IsTrue(result.empty());
		}

		TEST_METHOD(ParentAssignIf_ClauseNotTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(ParentAssignIf);
			Assert::IsTrue(result.empty());
		}

		TEST_METHOD(ParentAssignWhile_ClauseNotTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(ParentAssignWhile);
			Assert::IsTrue(result.empty());
		}

		/* ===== Select s such that Parent(p, *) ===== */

		TEST_METHOD(ParentPrintPrint_ClauseNotTrue) {
			std::unordered_set<std::string> result1 = QueryEvaluator::evaluateQuery(ParentPrintPrint);
			std::unordered_set<std::string> result2 = QueryEvaluator::evaluateQuery(edgeCaseParentPrintPrintSame);
			Assert::IsTrue(result1.empty());
			Assert::IsTrue(result2.empty());
		}

		TEST_METHOD(ParentPrintAssign_ClauseNotTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(ParentPrintAssign);
			Assert::IsTrue(result.empty());
		}

		TEST_METHOD(ParentPrintRead_ClauseNotTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(ParentPrintRead);
			Assert::IsTrue(result.empty());
		}

		TEST_METHOD(ParentPrintIf_ClauseNotTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(ParentPrintIf);
			Assert::IsTrue(result.empty());
		}

		TEST_METHOD(ParentPrintWhile_ClauseNotTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(ParentPrintWhile);
			Assert::IsTrue(result.empty());
		}

		/* ===== Select s such that Parent(r, *) ===== */

		TEST_METHOD(ParentReadRead_ClauseNotTrue) {
			std::unordered_set<std::string> result1 = QueryEvaluator::evaluateQuery(ParentReadRead);
			std::unordered_set<std::string> result2 = QueryEvaluator::evaluateQuery(edgeCaseParentReadReadSame);
			Assert::IsTrue(result1.empty());
			Assert::IsTrue(result2.empty());
		}

		TEST_METHOD(ParentReadAssign_ClauseNotTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(ParentReadAssign);
			Assert::IsTrue(result.empty());
		}

		TEST_METHOD(ParentReadPrint_ClauseNotTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(ParentReadPrint);
			Assert::IsTrue(result.empty());
		}

		TEST_METHOD(ParentReadIf_ClauseNotTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(ParentReadIf);
			Assert::IsTrue(result.empty());
		}

		TEST_METHOD(ParentReadWhile_ClauseNotTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(ParentReadWhile);
			Assert::IsTrue(result.empty());
		}

		/* ===== Select s such that Parent(i, *) ===== */

		TEST_METHOD(ParentIfIf_ClauseHoldsTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(ParentIfIf);
			std::unordered_set<std::string> expected = allStmts;
			Assert::IsTrue(result == expected);
		}
		TEST_METHOD(ParentIfIf_ClauseNotTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(edgeCaseParentIfIfSame);
			Assert::IsTrue(result.empty());
		}

		TEST_METHOD(ParentIfAssign_ClauseHoldsTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(ParentIfAssign);
			std::unordered_set<std::string> expected = allStmts;
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(ParentIfPrint_ClauseHoldsTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(ParentIfPrint);
			std::unordered_set<std::string> expected = allStmts;
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(ParentIfRead_ClauseHoldsTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(ParentIfRead);
			std::unordered_set<std::string> expected = allStmts;
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(ParentIfWhile_ClauseNotTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(ParentIfWhile);
			Assert::IsTrue(result.empty());
		}

		/* ===== Select s such that Parent(w, *) ===== */

		TEST_METHOD(ParentWhileWhile_ClauseHoldsTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(ParentWhileWhile);
			std::unordered_set<std::string> expected = allStmts;
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(ParentWhileWhile_ClauseNotTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(edgeCaseParentWhileWhileSame);
			Assert::IsTrue(result.empty());
		}

		TEST_METHOD(ParentWhileAssign_ClauseHoldsTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(ParentWhileAssign);
			std::unordered_set<std::string> expected = allStmts;
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(ParentWhilePrint_ClauseNotTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(ParentWhilePrint);
			Assert::IsTrue(result.empty());
		}

		TEST_METHOD(ParentWhileRead_ClauseHoldsTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(ParentWhileRead);
			std::unordered_set<std::string> expected = allStmts;
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(ParentWhileIf_ClauseHoldsTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(ParentWhileIf);
			std::unordered_set<std::string> expected = allStmts;
			Assert::IsTrue(result == expected);
		}

		/*
		Prog_line and call stmt
		*/
		TEST_METHOD(ParentProgLineStmtInClause_ReturnAllParents) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(ParentProgLineStmtInClause);
			Assert::IsTrue(result == allParents);
		}

		TEST_METHOD(ParentProgLineStmtNotInClause_ReturnAllParents) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(ParentProgLineStmtNotInClause);
			Assert::IsTrue(result == allStmts);
		}

		TEST_METHOD(ParentStmtCallInClause_ReturnAllParentOfCall) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(ParentStmtCallInClause);
			Assert::IsTrue(result == parentsOfCall);
		}

		TEST_METHOD(ParentStmtCallNotInClause_ReturnAllStmts) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(ParentStmtCallNotInClause);
			Assert::IsTrue(result == allStmts);
		}

		TEST_METHOD(ParentStmtProgLineInClause_ReturnAllParents) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(ParentStmtProgLineInClause);
			Assert::IsTrue(result == allParents);
		}

		TEST_METHOD(ParentStmtProgLineNotInClause_ReturnAllStmts) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(ParentStmtProgLineNotInClause);
			Assert::IsTrue(result == allStmts);
		}
	};
}