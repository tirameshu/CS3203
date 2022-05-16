#include "stdafx.h"
#include "CppUnitTest.h"
#include "FollowsTEvaluatorTestData.h"
#include "assert.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace IntegrationTesting
{
	TEST_CLASS(FollowsTEvaluatorTest)
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
			pkb.addStatement("testA", { 5, 6 });
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

			pkb.addAllFollower(1, { 2, 3 });
			pkb.addAllFollower(2, { 3 });
			pkb.addAllFollower(4, { 5, 12, 13, 14 });
			pkb.addAllFollower(5, { 12, 13, 14 });
			pkb.addAllFollower(6, { 7, 11 });
			pkb.addAllFollower(7, { 11 });
			pkb.addAllFollower(8, {9});
			pkb.addAllFollower(12, { 13, 14 });
			pkb.addAllFollower(13, { 14 });
			pkb.addAllFollower(15, { 16 });

			pkb.addAllFollowed(2, { 1 });
			pkb.addAllFollowed(3, { 1, 2 });
			pkb.addAllFollowed(5, { 4 });
			pkb.addAllFollowed(7, { 6 });
			pkb.addAllFollowed(9, { 8 });
			pkb.addAllFollowed(11, { 6, 7 });
			pkb.addAllFollowed(12, { 4, 5 });
			pkb.addAllFollowed(13, { 4, 5, 12 });
			pkb.addAllFollowed(14, { 4, 5, 12, 13 });
			pkb.addAllFollowed(16, { 15 });
		}

		// any, any
		TEST_METHOD(followsTAnyAny_ClauseHoldsTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(FollowsTAnyAny);
			Assert::IsTrue(result == allProgStmts);
		}

		// any, stmt
		TEST_METHOD(followsTAnyStmt_ResultSynonymInClause) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(FollowsTAnyStmtInClause);
			Assert::IsTrue(result == allFollowerStmt);
		}

		TEST_METHOD(followsTAnyStmt_ResultSynonymNotInClause) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(FollowsTAnyStmtNotInClause);
			Assert::IsTrue(result == allProgStmts);
		}

		// stmt, any
		TEST_METHOD(followsTStmtAny_ResultSynonymInClause) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(FollowsTStmtAnyInClause);
			Assert::IsTrue(result == allFollowedStmt);
		}

		TEST_METHOD(followsTStmtAny_ResultSynonymNotInClause) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(FollowsTStmtAnyNotInClause);
			Assert::IsTrue(result == allProgStmts);
		}

		// stmt, stmt
		TEST_METHOD(followsTStmtStmtFollowed_ResultSynonymInClause) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(FollowsTStmtStmtFollowed);
			Assert::IsTrue(result == allFollowedStmt);
		}

		TEST_METHOD(followsTStmtStmtFollower_ResultSynonymInClause) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(FollowsTStmtStmtFollower);
			Assert::IsTrue(result == allFollowerStmt);
		}

		TEST_METHOD(followsTStmtStmt_ResultSynonymNotInClause) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(FollowsTStmtStmtNotInClause);
			Assert::IsTrue(result == allProgStmts);
		}

		// any, int
		TEST_METHOD(followsTAnyInt_ClauseHoldsTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(FollowsTAnyInt_True);
			Assert::IsTrue(result == allProgStmts);
		}

		TEST_METHOD(followsTAnyInt_ClauseNotTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(FollowsTAnyInt_False);
			Assert::IsTrue(result.empty());
		}

		// stmt, int
		TEST_METHOD(followsTStmtInt_ResultSynonymInClause) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(FollowsTStmtIntInClause);
			std::unordered_set<std::string> expected = { "1", "2" };
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(followsTStmtInt_ResultSynonymNotInClause) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(FollowsTStmtIntNotInClause);
			Assert::IsTrue(result == allProgStmts);
		}

		TEST_METHOD(followsTStmtInt_ClauseNotTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(FollowsTStmtInt_False);
			Assert::IsTrue(result.empty());
		}

		// any, syn
		TEST_METHOD(followsTAnyAssign_ResultSynonymInClause) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(FollowsTAnyAssignInClause);
			std::unordered_set<std::string> expected = { "2", "9", "11", "12", "16" };
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(followsTAnyAssign_ResultSynonymNotInClause) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(FollowsTAnyAssignNotInClause);
			Assert::IsTrue(result == allProgStmts);
		}

		TEST_METHOD(followsTAnyPrint_ResultSynonymNotInClause) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(FollowsTAnyPrintNotInClause);
			Assert::IsTrue(result == allProgStmts);
		}

		TEST_METHOD(followsTAnyRead_ResultSynonymNotInClause) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(FollowsTAnyReadNotInClause);
			Assert::IsTrue(result == allProgStmts);
		}

		TEST_METHOD(followsTAnyIf_ResultSynonymInClause) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(FollowsTAnyIfInClause);
			std::unordered_set<std::string> expected = { "3", "7" };
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(followsTAnyIf_ResultSynonymNotInClause) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(FollowsTAnyIfNotInClause);
			Assert::IsTrue(result == allProgStmts);
		}

		TEST_METHOD(followsTAnyWhile_ResultSynonymInClause) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(FollowsTAnyWhileInClause);
			std::unordered_set<std::string> expected = { "5" };
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(followsTAnyWhile_ResultSynonymNotInClause) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(FollowsTAnyWhileNotInClause);
			Assert::IsTrue(result == allProgStmts);
		}

		// stmt, syn
		TEST_METHOD(followsTStmtIf_ResultSynonymInClause) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(FollowsTStmtIfInClause);
			std::unordered_set<std::string> expected = { "1", "2", "6" };
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(followsTStmtIf_ResultSynonymNotInClause) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(FollowsTStmtIfNotInClause);
			Assert::IsTrue(result == allProgStmts);
		}

		TEST_METHOD(followsTStmtWhile_ResultSynonymInClause) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(FollowsTStmtWhileInClause);
			std::unordered_set<std::string> expected = { "4" };
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(followsTStmtWhile_ResultSynonymNotInClause) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(FollowsTStmtWhileNotInClause);
			Assert::IsTrue(result == allProgStmts);
		}
		
		// int, any
		TEST_METHOD(followsTIntAny_ClauseHoldsTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(FollowsTIntAny_True);
			Assert::IsTrue(result == allProgStmts);
		}

		TEST_METHOD(followsTIntAny_ClauseNotTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(FollowsTIntAny_False);
			Assert::IsTrue(result.empty());
		}

		// int, stmt
		TEST_METHOD(followsTIntStmt_ResultSynonymInClause) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(FollowsTIntStmtInClause);
			std::unordered_set<std::string> expected = { "5", "12", "13", "14"};
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(followsTIntStmt_ResultSynonymNotInClause) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(FollowsTIntStmtNotInClause);
			Assert::IsTrue(result == allProgStmts);
		}

		TEST_METHOD(followsTIntStmt_ClauseNotTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(FollowsTIntStmt_False);
			Assert::IsTrue(result.empty());
		}

		// int, int
		TEST_METHOD(followsTIntInt_ClauseHoldsTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(FollowsTIntInt_True);
			Assert::IsTrue(result == allProgStmts);
		}

		TEST_METHOD(followsTIntInt_ClauseNotTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(FollowsTIntInt_False);
			Assert::IsTrue(result.empty());
		}

		// int, syn
		TEST_METHOD(followsTIntIf_ResultSynonymInClause) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(FollowsTIntIfInClause);
			std::unordered_set<std::string> expected = { "3" };
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(followsTIntIf_ResultSynonymNotInClause) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(FollowsTIntIfNotInClause);
			Assert::IsTrue(result == allProgStmts);
		}

		TEST_METHOD(followsTIntIf_ClauseNotTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(FollowsTIntIf_False);
			Assert::IsTrue(result.empty());
		}

		TEST_METHOD(followsTIntWhile_ResultSynonymInClause) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(FollowsTIntWhileInClause);
			std::unordered_set<std::string> expected = { "5" };
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(followsTIntWhile_ResultSynonymNotInClause) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(FollowsTIntWhileNotInClause);
			Assert::IsTrue(result == allProgStmts);
		}

		TEST_METHOD(followsTIntWhile_ClauseNotTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(FollowsTIntWhile_False);
			Assert::IsTrue(result.empty());
		}

		// syn, any
		TEST_METHOD(followsTIfAny_ResultSynonymInClause) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(FollowsTIfAnyInClause);
			std::unordered_set<std::string> expected = { "7" };
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(followsTIfAny_ResultSynonymNotInClause) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(FollowsTIfAnyNotInClause);
			Assert::IsTrue(result == allProgStmts);
		}

		TEST_METHOD(followsTWhileAny_ResultSynonymInClause) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(FollowsTWhileAnyInClause);
			std::unordered_set<std::string> expected = { "5", "6" };
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(followsTWhileAny_ResultSynonymNotInClause) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(FollowsTWhileAnyNotInClause);
			Assert::IsTrue(result == allProgStmts);
		}

		TEST_METHOD(FollowsTIfStmt_ResultSynonymInClause) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(FollowsTIfStmtInClause);
			std::unordered_set<std::string> expected = { "7" };
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(FollowsTIfStmt_ResultSynonymNotInClause) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(FollowsTIfStmtNotInClause);
			Assert::IsTrue(result == allProgStmts);
		}

		TEST_METHOD(FollowsTWhileStmt_ResultSynonymInClause) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(FollowsTWhileStmtInClause);
			std::unordered_set<std::string> expected = { "5", "6" };

			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(FollowsTWhileStmt_ResultSynonymNotInClause) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(FollowsTWhileStmtNotInClause);
			Assert::IsTrue(result == allProgStmts);
		}


		//syn, int
		TEST_METHOD(FollowsTIfInt_ResultSynonymInClause) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(FollowsTIfIntInClause);
			std::unordered_set<std::string> expected = { "7" };
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(FollowsTIfInt_ResultSynonymNotInClause) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(FollowsTIfIntNotInClause);
			Assert::IsTrue(result == allProgStmts);
		}

		TEST_METHOD(FollowsTIfInt_ResultSynonymNotInClause_False) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(FollowsTIfIntNotInClause_False);
			Assert::IsTrue(result.empty());
		}

		TEST_METHOD(FollowsTWHileInt_ResultSynonymInClause) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(FollowsTWhileIntInClause);
			std::unordered_set<std::string> expected = { "5" };
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(FollowsTWhileInt_ResultSynonymNotInClause) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(FollowsTWhileIntNotInClause);
			Assert::IsTrue(result == allProgStmts);
		}
		
		TEST_METHOD(FollowsTWhileInt_ResultSynonymNotInClause_False) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(FollowsTWhileIntNotInClause_False);
			Assert::IsTrue(result.empty());
		}

		// syn, syn
		TEST_METHOD(FollowsTWhileIf_ResultSynonymInClause) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(FollowsTWhileIfInClause);
			std::unordered_set<std::string> expected = { "7" };
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(FollowsTWhileIf_ResultSynonymNotInClause) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(FollowsTWhileIfNotInClause);
			Assert::IsTrue(result == allProgStmts);
		}

		/*
		Prog_line and call stmt
		*/
		TEST_METHOD(FollowsTProgLineStmtInClause_ReturnAllParents) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(FollowsTProgLineStmtInClause);
			Assert::IsTrue(result == allFollowedStmt);
		}

		TEST_METHOD(FollowsTProgLineStmtNotInClause_ReturnAllFollowed) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(FollowsTProgLineStmtNotInClause);
			Assert::IsTrue(result == allProgStmts);
		}

		TEST_METHOD(FollowsTStmtCallInClause_ReturnFollowsTOfCall) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(FollowsTStmtCallInClause);
			std::unordered_set<std::string> expected = {};
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(FollowsTStmtCallNotInClause_ReturnNothing) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(FollowsTStmtCallNotInClause);
			std::unordered_set<std::string> expected = {};
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(FollowsTStmtProgLineInClause_ReturnAllParents) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(FollowsTStmtProgLineInClause);
			Assert::IsTrue(result == allFollowedStmt);
		}

		TEST_METHOD(FollowsTStmtProgLineNotInClause_ReturnNothing) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(FollowsTStmtProgLineNotInClause);
			std::unordered_set<std::string> expected = {};
			Assert::IsTrue(result == expected);
		}
	};
}