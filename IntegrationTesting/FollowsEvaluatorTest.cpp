#include "stdafx.h"
#include "CppUnitTest.h"
#include "FollowsEvaluatorTestData.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace IntegrationTesting
{
	TEST_CLASS(FollowsEvaluatorTest)
	{
	public:
		TEST_CLASS_INITIALIZE(Initialization) {
			PKB pkb;
			pkb.clear();
			pkb.addStatement("testA", { 1, 1 });
			pkb.addStatement("testA", { 1, 2 });
			pkb.addStatement("testA", { 4, 3 });
			pkb.addStatement("testA", { 1, 4 });
			pkb.addStatement("testA", { 5, 5 });
			pkb.addStatement("testA", { 3, 6 });
			pkb.addStatement("testA", { 4, 7 });
			pkb.addStatement("testA", { CALL, 8 });
			pkb.addStatement("testA", { 1, 9 });
			pkb.addStatement("testA", { 3, 10 });
			pkb.addStatement("testA", { 1, 11 });
			pkb.addStatement("testA", { 1, 12 });
			pkb.addStatement("testA", { 3, 13 });
			pkb.addStatement("testA", { 2, 14 });
			pkb.addStatement("testA", { 2, 15 });
			pkb.addStatement("testA", { 1, 16 });
			pkb.addStatement("testB", { ASSIGN, 17 });
			pkb.addFollows(1, 2);
			pkb.addFollows(2, 3);
			pkb.addFollows(4, 5);
			pkb.addFollows(6, 7);
			pkb.addFollows(8, 9);
			pkb.addFollows(7, 11);
			pkb.addFollows(5, 12);
			pkb.addFollows(12, 13);
			pkb.addFollows(13, 14);
			pkb.addFollows(15, 16);
		}

		TEST_METHOD(followsAnyAny_ClauseHoldsTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(FollowsAnyAny);
			Assert::IsTrue(result == followsAllStmt);
		}

		TEST_METHOD(followsAnyStmt_ResultSynonymInClause) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(FollowsAnyStmt);
			std::unordered_set<std::string> expected = { "2", "3", "5", "7", "9", "11", "12", "13", "14", "16" };
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(followsAnyStmt_ResultSynonymNotInClause) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(InvalidFollowsAnyStmt);
			Assert::IsTrue(result == followsAllStmt);
		}

		TEST_METHOD(followsStmtAny_ResultSynonymInClause) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(FollowsStmtAny);
			std::unordered_set<std::string> expected = { "1", "2", "4", "6", "8", "7", "5", "12", "13", "15" };
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(followsStmtAny_ResultSynonymNotInClause) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(InvalidFollowsStmtAny);
			Assert::IsTrue(result == followsAllStmt);
		}

		TEST_METHOD(followsStmtStmt_ResultSynonymInClause) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(FollowsStmtStmt);
			std::unordered_set<std::string> expected = { "1", "2", "4", "6", "8", "7", "5", "12", "13", "15" };
			Assert::IsTrue(result == expected);
		}

		// Follows(s, s) - return empty set
		TEST_METHOD(followsStmtStmt_ResultSynonymRepeatedInClause) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(InvalidFollowsSameStmt);
			Assert::IsTrue(result.empty());
		}

		TEST_METHOD(followsStmtStmt_ResultSynonymNotInClause) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(InvalidFollowsStmtStmt);
			Assert::IsTrue(result == followsAllStmt);
		}

		TEST_METHOD(followsAnyInt_ClauseHoldsTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(FollowsAnyInt);
			Assert::IsTrue(result == followsAllStmt);
		}

		TEST_METHOD(followsAnyInt_ClauseNotTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(InvalidFollowsAnyInt);
			Assert::IsTrue(result.empty());
		}

		TEST_METHOD(followsStmtInt_ResultSynonymInClause) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(FollowsStmtInt);
			std::unordered_set<std::string> expected = { "7" };
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(followsStmtInt_ResultSynonymNotInClause) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(InvalidFollowsStmtInt);
			Assert::IsTrue(result == followsAllStmt);
		}

		TEST_METHOD(followsAnyAssign_ClauseHoldsTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(FollowsAnyAssign);
			Assert::IsTrue(result == followsAllStmt);
		}

		TEST_METHOD(followsAnyPrint_ClauseHoldsTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(FollowsAnyPrint);
			Assert::IsTrue(result == followsAllStmt);
		}

		TEST_METHOD(followsAnyRead_ClauseHoldsTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(FollowsAnyRead);
			Assert::IsTrue(result == followsAllStmt);
		}

		TEST_METHOD(followsAnyIf_ClauseHoldsTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(FollowsAnyIf);
			Assert::IsTrue(result == followsAllStmt);
		}

		TEST_METHOD(followsAnyWhile_ClauseHoldsTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(FollowsAnyWhile);
			Assert::IsTrue(result == followsAllStmt);
		}

		TEST_METHOD(followsStmtAssign_ResultSynonymInClause) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(FollowsStmtAssign);
			std::unordered_set<std::string> expected = { "1", "5", "7", "8", "15" };
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(followsStmtAssign_ResultSynonymNotInClause) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(InvalidFollowsStmtAssign);
			Assert::IsTrue(result == followsAllStmt);
		}

		TEST_METHOD(followsStmtPrint_ResultSynonymInClause) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(FollowsStmtPrint);
			std::unordered_set<std::string> expected = {"13"};
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(followsStmtPrint_ResultSynonymNotInClause) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(InvalidFollowsStmtPrint);
			Assert::IsTrue(result == followsAllStmt);
		}

		TEST_METHOD(followsStmtRead_ResultSynonymInClause) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(FollowsStmtRead);
			std::unordered_set<std::string> expected = { "12" };
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(followsStmtRead_ResultSynonymNotInClause) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(InvalidFollowsStmtRead);
			Assert::IsTrue(result == followsAllStmt);
		}

		TEST_METHOD(followsStmtIf_ResultSynonymInClause) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(FollowsStmtIf);
			std::unordered_set<std::string> expected = { "2", "6" };
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(followsStmtIf_ResultSynonymNotInClause) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(InvalidFollowsStmtIf);
			Assert::IsTrue(result == followsAllStmt);
		}

		TEST_METHOD(followsStmtWhile_ResultSynonymInClause) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(FollowsStmtWhile);
			std::unordered_set<std::string> expected = { "4" };
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(followsStmtWhile_ResultSynonymNotInClause) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(InvalidFollowsStmtWhile);
			Assert::IsTrue(result == followsAllStmt);
		}

		TEST_METHOD(followsIntAny_ClauseHoldsTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(FollowsIntAny);
			Assert::IsTrue(result == followsAllStmt);
		}

		TEST_METHOD(followsIntAny_ClauseNotTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(InvalidFollowsIntAny);
			Assert::IsTrue(result.empty());
		}

		TEST_METHOD(followsIntStmt_ResultSynonymInClause_ClauseHoldsTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(FollowsIntStmt);
			std::unordered_set<std::string> expected = { "13" };
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(followsIntStmt_ResultSynonymInClause_ClauseNotTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(InvalidFollowsIntStmt1);
			Assert::IsTrue(result.empty());
		}

		TEST_METHOD(followsIntStmt_ResultSynonymNotInClause_ClauseHoldsTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(InvalidFollowsIntStmt2);
			Assert::IsTrue(result == followsAllStmt);
		}

		TEST_METHOD(followsIntStmt_NonExistingStmtNo1) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(InvalidFollowsIntStmt3);
			Assert::IsTrue(result.empty());
		}

		TEST_METHOD(followsIntStmt_NonExistingStmtNo2) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(InvalidFollowsIntStmt4);
			Assert::IsTrue(result.empty());
		}

		TEST_METHOD(followsIntInt_ClauseHoldsTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(FollowsIntInt);
			Assert::IsTrue(result == followsAllStmt);
		}

		TEST_METHOD(followsIntInt_LeftBigger_ClauseNotTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(InvalidFollowsIntInt1);
			Assert::IsTrue(result.empty());
		}

		TEST_METHOD(followsIntInt_SameInt_ClauseNotTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(InvalidFollowsIntInt2);
			Assert::IsTrue(result.empty());
		}

		TEST_METHOD(followsIntAssign_ClauseHoldsTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(FollowsIntAssign);
			Assert::IsTrue(result == followsAllStmt);
		}

		TEST_METHOD(followsIntAssign_ClauseNotTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(InvalidFollowsIntAssign);
			Assert::IsTrue(result.empty());
		}

		TEST_METHOD(followsIntPrint_ClauseHoldsTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(FollowsIntPrint);
			Assert::IsTrue(result == followsAllStmt);
		}

		TEST_METHOD(followsIntPrint_ClauseNotTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(InvalidFollowsIntPrint);
			Assert::IsTrue(result.empty());
		}

		TEST_METHOD(followsIntRead_ClauseHoldsTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(FollowsIntRead);
			Assert::IsTrue(result == followsAllStmt);
		}

		TEST_METHOD(followsIntRead_ClauseNotTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(InvalidFollowsIntRead);
			Assert::IsTrue(result.empty());
		}

		TEST_METHOD(followsIntIf_ClauseHoldsTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(FollowsIntIf);
			Assert::IsTrue(result == followsAllStmt);
		}

		TEST_METHOD(followsIntIf_ClauseNotTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(InvalidFollowsIntIf);
			Assert::IsTrue(result.empty());
		}

		TEST_METHOD(followsIntWhile_ClauseHoldsTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(FollowsIntWhile);
			Assert::IsTrue(result == followsAllStmt);
		}

		TEST_METHOD(followsIntWhile_ClauseNotTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(InvalidFollowsIntWhile);
			Assert::IsTrue(result.empty());
		}

		TEST_METHOD(followsAssignAny_ClauseHoldsTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(FollowsAssignAny);
			Assert::IsTrue(result == followsAllStmt);
		}

		TEST_METHOD(followsPrintAny_ClauseHoldsTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(FollowsPrintAny);
			Assert::IsTrue(result == followsAllStmt);
		}

		TEST_METHOD(followsReadAny_ClauseHoldsTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(FollowsReadAny);
			Assert::IsTrue(result == followsAllStmt);
		}

		TEST_METHOD(followsIfAny_ClauseHoldsTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(FollowsIfAny);
			Assert::IsTrue(result == followsAllStmt);
		}

		TEST_METHOD(followsWhileAny_ClauseHoldsTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(FollowsWhileAny);
			Assert::IsTrue(result == followsAllStmt);
		}

		TEST_METHOD(followsAssignStmt_ResultSynonymInClause) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(FollowsAssignStmt);
			std::unordered_set<std::string> expected = { "2", "3", "5", "13" };
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(followsAssignStmt_ResultSynonymNotInClause) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(InvalidFollowsAssignStmt);
			Assert::IsTrue(result == followsAllStmt);
		}

		TEST_METHOD(followsPrintStmt_ResultSynonymInClause) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(FollowsPrintStmt);
			std::unordered_set<std::string> expected = { "16" };
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(followsPrintStmt_ResultSynonymNotInClause) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(InvalidFollowsPrintStmt);
			Assert::IsTrue(result == followsAllStmt);
		}

		TEST_METHOD(followsReadStmt_ResultSynonymInClause) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(FollowsReadStmt);
			std::unordered_set<std::string> expected = { "7", "14" };
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(followsReadStmt_ResultSynonymNotInClause) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(InvalidFollowsReadStmt);
			Assert::IsTrue(result == followsAllStmt);
		}

		TEST_METHOD(followsIfStmt_ResultSynonymInClause) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(FollowsIfStmt);
			std::unordered_set<std::string> expected = { "11" };
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(followsIfStmt_ResultSynonymNotInClause) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(InvalidFollowsIfStmt);
			Assert::IsTrue(result == followsAllStmt);
		}

		TEST_METHOD(followsWhileStmt_ResultSynonymInClause) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(FollowsWhileStmt);
			std::unordered_set<std::string> expected = { "12" };
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(followsWhileStmt_ResultSynonymNotInClause) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(InvalidFollowsWhileStmt);
			Assert::IsTrue(result == followsAllStmt);
		}

		TEST_METHOD(followsAssignInt_ClauseHoldsTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(FollowsAssignInt);
			Assert::IsTrue(result == followsAllStmt);
		}

		TEST_METHOD(followsAssignInt_ClauseNotTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(InvalidFollowsAssignInt);
			Assert::IsTrue(result.empty());
		}

		TEST_METHOD(followsPrintInt_ClauseHoldsTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(FollowsPrintInt);
			Assert::IsTrue(result == std::unordered_set<std::string>{});
		}

		TEST_METHOD(followsPrintInt_ClauseNotTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(InvalidFollowsPrintInt);
			Assert::IsTrue(result.empty());
		}

		TEST_METHOD(followsReadInt_ClauseHoldsTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(FollowsReadInt);
			Assert::IsTrue(result == followsAllStmt);
		}

		TEST_METHOD(followsReadInt_ClauseNotTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(InvalidFollowsReadInt);
			Assert::IsTrue(result.empty());
		}

		TEST_METHOD(followsIfInt_ClauseHoldsTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(FollowsIfInt);
			Assert::IsTrue(result == followsAllStmt);
		}

		TEST_METHOD(followsIfInt_ClauseNotTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(InvalidFollowsIfInt);
			Assert::IsTrue(result.empty());
		}
		TEST_METHOD(followsWhileInt_ClauseHoldsTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(FollowsWhileInt);
			Assert::IsTrue(result == followsAllStmt);
		}

		TEST_METHOD(followsWhileInt_ClauseNotTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(InvalidFollowsWhileInt);
			Assert::IsTrue(result.empty());
		}

		TEST_METHOD(followsAssignAssign_ClauseHoldsTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(FollowsAssignAssign);
			Assert::IsTrue(result == followsAllStmt);
		}

		TEST_METHOD(followsAssignAssign_ClauseNotTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(InvalidFollowsAssignAssign);
			Assert::IsTrue(result.empty());
		}

		TEST_METHOD(followsAssignPrint_ClauseNotTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(FollowsAssignPrint);
			Assert::IsTrue(result.empty());
		}

		TEST_METHOD(followsAssignRead_ClauseHoldsTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(FollowsAssignRead);
			Assert::IsTrue(result == followsAllStmt);
		}

		TEST_METHOD(followsAssignIf_ClauseHoldsTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(FollowsAssignIf);
			Assert::IsTrue(result == followsAllStmt);
		}

		TEST_METHOD(followsAssignWhile_ClauseHoldsTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(FollowsAssignWhile);
			Assert::IsTrue(result == followsAllStmt);
		}

		TEST_METHOD(followsPrintPrint_ClauseNotTrue) {
			std::unordered_set<std::string> result1 = QueryEvaluator::evaluateQuery(FollowsPrintPrint);
			std::unordered_set<std::string> result2 = QueryEvaluator::evaluateQuery(InvalidFollowsPrintPrint);
			Assert::IsTrue(result1.empty());
			Assert::IsTrue(result2.empty());
		}

		TEST_METHOD(followsPrintAssign_ClauseHoldsTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(FollowsPrintAssign);
			Assert::IsTrue(result == followsAllStmt);
		}

		TEST_METHOD(followsPrintRead_ClauseNotTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(FollowsPrintRead);
			Assert::IsTrue(result.empty());
		}

		TEST_METHOD(followsPrintIf_ClauseNotTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(FollowsPrintIf);
			Assert::IsTrue(result.empty());
		}

		TEST_METHOD(followsPrintWhile_ClauseNotTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(FollowsPrintWhile);
			Assert::IsTrue(result.empty());
		}

		TEST_METHOD(followsReadRead_ClauseNotTrue) {
			std::unordered_set<std::string> result1 = QueryEvaluator::evaluateQuery(FollowsReadRead);
			std::unordered_set<std::string> result2 = QueryEvaluator::evaluateQuery(InvalidFollowsReadRead);
			Assert::IsTrue(result1.empty());
			Assert::IsTrue(result2.empty());
		}

		TEST_METHOD(followsReadAssign_ClauseNotTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(FollowsReadAssign);
			Assert::IsTrue(result.empty());
		}

		TEST_METHOD(followsReadPrint_ClauseHoldsTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(FollowsReadPrint);
			Assert::IsTrue(result == followsAllStmt);
		}

		TEST_METHOD(followsReadIf_ClauseHoldsTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(FollowsReadIf);
			Assert::IsTrue(result == followsAllStmt);
		}

		TEST_METHOD(followsReadWhile_ClauseNotTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(FollowsReadWhile);
			Assert::IsTrue(result.empty());
		}

		TEST_METHOD(followsIfIf_ClauseNotTrue) {
			std::unordered_set<std::string> result1 = QueryEvaluator::evaluateQuery(FollowsIfIf);
			std::unordered_set<std::string> result2 = QueryEvaluator::evaluateQuery(InvalidFollowsIfIf);
			Assert::IsTrue(result1.empty());
			Assert::IsTrue(result2.empty());
		}

		TEST_METHOD(followsIfAssign_ClauseHoldsTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(FollowsIfAssign);
			Assert::IsTrue(result == followsAllStmt);
		}

		TEST_METHOD(followsIfPrint_ClauseNotTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(FollowsIfPrint);
			Assert::IsTrue(result.empty());
		}

		TEST_METHOD(followsIfRead_ClauseNotTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(FollowsIfRead);
			Assert::IsTrue(result.empty());
		}

		TEST_METHOD(followsIfWhile_ClauseNotTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(FollowsIfPrint);
			Assert::IsTrue(result.empty());
		}

		TEST_METHOD(followsWhileWhile_ClauseNotTrue) {
			std::unordered_set<std::string> result1 = QueryEvaluator::evaluateQuery(FollowsWhileWhile);
			std::unordered_set<std::string> result2 = QueryEvaluator::evaluateQuery(InvalidFollowsWhileWhile);
			Assert::IsTrue(result1.empty());
			Assert::IsTrue(result2.empty());
		}

		TEST_METHOD(followsWhileAssign_ClauseHoldsTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(FollowsWhileAssign);
			Assert::IsTrue(result == followsAllStmt);
		}

		TEST_METHOD(followsWhilePrint_ClauseNotTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(FollowsWhilePrint);
			Assert::IsTrue(result.empty());
		}

		TEST_METHOD(followsWhileRead_ClauseNotTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(FollowsWhileRead);
			Assert::IsTrue(result.empty());
		}

		TEST_METHOD(followsWhileIf_ClauseNotTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(FollowsWhileIf);
			Assert::IsTrue(result.empty());
		}

		/*
		Prog_line and call stmt
		*/
		TEST_METHOD(FollowsProgLineStmtInClause_ReturnAllParents) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(FollowsProgLineStmtInClause);
			Assert::IsTrue(result == followsAllFollowed);
		}

		TEST_METHOD(FollowsProgLineStmtNotInClause_ReturnAllFollowed) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(FollowsProgLineStmtNotInClause);
			Assert::IsTrue(result == followsAllStmt);
		}

		TEST_METHOD(FollowsStmtCallInClause_ReturnNothing) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(FollowsStmtCallInClause);
			std::unordered_set<std::string> expected = {};
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(FollowsStmtCallNotInClause_ReturnNothing) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(FollowsStmtCallNotInClause);
			std::unordered_set<std::string> expected = {};
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(FollowsStmtProgLineInClause_ReturnAllParents) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(FollowsStmtProgLineInClause);
			Assert::IsTrue(result == followsAllFollowed);
		}

		TEST_METHOD(FollowsStmtProgLineNotInClause_ReturnNothing) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(FollowsStmtProgLineNotInClause);
			std::unordered_set<std::string> expected = {};
			Assert::IsTrue(result == expected);
		}
	};
}