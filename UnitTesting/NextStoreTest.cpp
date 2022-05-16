#include "stdafx.h"
#include "CppUnitTest.h"
#include "NextStoreTestData.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{
	TEST_CLASS(NextStoreTest)
	{
	public:

		TEST_CLASS_INITIALIZE(InitializeStore)
		{
			NextStore::clear();
			NextStore::addNext(1, 2);
			NextStore::addNext(2, 3);
			NextStore::addNext(3, 4);
			NextStore::addNext(4, 5);
			NextStore::addNext(5, 6);
			NextStore::addNext(5, 7);
			NextStore::addNext(6, 8);
			NextStore::addNext(7, 8);
			NextStore::addNext(8, 3);
			NextStore::addNext(3, 9);
			NextStore::addNext(10, 11);
			NextStore::addNext(11, 12);
			NextStore::addNext(12, 11);

			NextStore::addAllPrevious(2, { 1 });
			NextStore::addAllPrevious(3, { 1, 2, 3, 4, 5, 6, 7, 8 });
			NextStore::addAllPrevious(4, { 1, 2, 3, 4, 5, 6, 7, 8 });
			NextStore::addAllPrevious(5, { 1, 2, 3, 4, 5, 6, 7, 8 });
			NextStore::addAllPrevious(6, { 1, 2, 3, 4, 5, 6, 7, 8 });
			NextStore::addAllPrevious(7, { 1, 2, 3, 4, 5, 6, 7, 8 });
			NextStore::addAllPrevious(8, { 1, 2, 3, 4, 5, 6, 7, 8 });
			NextStore::addAllPrevious(9, { 1, 2, 3, 4, 5, 6, 7, 8 });
			NextStore::addAllPrevious(11, { 10, 11, 12 });
			NextStore::addAllPrevious(12, { 10, 11, 12 });

			NextStore::addAllNext(1, { 2, 3, 4, 5, 6, 7, 8, 9 });
			NextStore::addAllNext(2, { 3, 4, 5, 6, 7, 8, 9 });
			NextStore::addAllNext(3, { 3, 4, 5, 6, 7, 8, 9 });
			NextStore::addAllNext(4, { 3, 4, 5, 6, 7, 8, 9 });
			NextStore::addAllNext(5, { 3, 4, 5, 6, 7, 8, 9 });
			NextStore::addAllNext(6, { 3, 4, 5, 6, 7, 8, 9 });
			NextStore::addAllNext(7, { 3, 4, 5, 6, 7, 8, 9 });
			NextStore::addAllNext(8, { 3, 4, 5, 6, 7, 8, 9 });
			NextStore::addAllNext(10, { 11, 12 });
			NextStore::addAllNext(11, { 11, 12 });
			NextStore::addAllNext(12, { 11, 12 });
		}

		// SETTER METHOD TESTS
		TEST_METHOD(addNext_DUPLICATE) {
			Assert::IsFalse(NextStore::addNext(1, 2));
		}

		TEST_METHOD(addAllPrevious_INVALID_STMT_ROOT_NODE) {
			Assert::IsFalse(NextStore::addAllPrevious(1, { 0, 2, 3, 4 }));
		}

		TEST_METHOD(addAllPrevious_NON_EXISTENT_STMT) {
			Assert::IsFalse(NextStore::addAllPrevious(15, { 0, 2, 3, 4 }));
		}

		TEST_METHOD(addAllNext_INVALID_STMT_ROOT_NODE) {
			Assert::IsFalse(NextStore::addAllNext(9, { 0, 2, 3, 4 }));
		}

		TEST_METHOD(addAllNext_NON_EXISTENT_STMT) {
			Assert::IsFalse(NextStore::addAllNext(15, { 0, 2, 3, 4 }));
		}

		// QUERY METHOD TESTS
		TEST_METHOD(stmtHasPrev_SUCCESS_STMT_HAS_PREV) {
			Assert::IsTrue(NextStore::stmtHasPrev(3));
		}

		TEST_METHOD(stmtHasPrev_SUCCESS_STMT_DOES_NOT_HAVE_PREV) {
			Assert::IsFalse(NextStore::stmtHasPrev(1));
		}

		TEST_METHOD(stmtHasPrev_UNSUCCESSFUL_NON_EXISTENT_STMT) {
			Assert::IsFalse(NextStore::stmtHasPrev(15));
		}

		TEST_METHOD(stmtHasNext_SUCCESS_STMT_HAS_NEXT) {
			Assert::IsTrue(NextStore::stmtHasNext(8));
		}

		TEST_METHOD(stmtHasNext_SUCCESS_STMT_DOES_NOT_HAVE_Next) {
			Assert::IsFalse(NextStore::stmtHasNext(9));
		}

		TEST_METHOD(stmtHasNext_UNSUCCESSFUL_NON_EXISTENT_STMT) {
			Assert::IsFalse(NextStore::stmtHasNext(15));
		}

		TEST_METHOD(programHasPrevStmt_SUCCESS) {
			Assert::IsTrue(NextStore::programHasPrevStmt());
		}

		TEST_METHOD(programHasNextStmt_SUCCESS) {
			Assert::IsTrue(NextStore::programHasNextStmt());
		}

		// GETTER METHOD TESTS
		TEST_METHOD(getPrevOfStmt_SUCCESS_STMT_HAS_PREV) {
			Assert::IsTrue(NextStore::getPrevOfStmt(8) == expectedStmtNextMap.at(8).prevStmts);
		}

		TEST_METHOD(getPrevOfStmt_SUCCESS_STMT_DOES_NOT_HAVE_PREV) {
			Assert::IsTrue(NextStore::getPrevOfStmt(1) == expectedStmtNextMap.at(1).prevStmts);
		}

		TEST_METHOD(getPrevOfStmt_SUCCESS_NON_EXISTENT_STMT) {
			Assert::IsTrue(NextStore::getPrevOfStmt(15) == std::unordered_set<int>{});
		}

		TEST_METHOD(getNextOfStmt_SUCCESS_STMT_HAS_NEXT) {
			Assert::IsTrue(NextStore::getNextOfStmt(5) == expectedStmtNextMap.at(5).nextStmts);
		}

		TEST_METHOD(getNextOfStmt_SUCCESS_STMT_DOES_NOT_HAVE_NEXT) {
			Assert::IsTrue(NextStore::getNextOfStmt(9) == expectedStmtNextMap.at(9).nextStmts);
		}

		TEST_METHOD(getNextOfStmt_SUCCESS_NON_EXISTENT_STMT) {
			Assert::IsTrue(NextStore::getNextOfStmt(15) == std::unordered_set<int>{});
		}

		TEST_METHOD(getPrevStarListOfStmt_SUCCESS_STMT_HAS_PREV) {
			Assert::IsTrue(NextStore::getPrevStarListOfStmt(8) == expectedStmtNextMap.at(8).listOfPrevStarStmt);
		}

		TEST_METHOD(getPrevStarListOfStmt_SUCCESS_STMT_DOES_NOT_HAVE_PREV) {
			Assert::IsTrue(NextStore::getPrevStarListOfStmt(1) == expectedStmtNextMap.at(1).listOfPrevStarStmt);
		}

		TEST_METHOD(getPrevStarListOfStmt_SUCCESS_NON_EXISTENT_STMT) {
			Assert::IsTrue(NextStore::getPrevStarListOfStmt(15) == std::unordered_set<int>{});
		}

		TEST_METHOD(getNextStarListOfStmt_SUCCESS_STMT_HAS_Next) {
			Assert::IsTrue(NextStore::getNextStarListOfStmt(5) == expectedStmtNextMap.at(5).listOfNextStarStmt);
		}

		TEST_METHOD(getNextStarListOfStmt_SUCCESS_STMT_DOES_NOT_HAVE_NEXT) {
			Assert::IsTrue(NextStore::getNextStarListOfStmt(9) == expectedStmtNextMap.at(9).listOfNextStarStmt);
		}

		TEST_METHOD(getNextStarListOfStmt_SUCCESS_NON_EXISTENT_STMT) {
			Assert::IsTrue(NextStore::getNextStarListOfStmt(15) == std::unordered_set<int>{});
		}

		TEST_METHOD(getAllNextPair_SUCCESS) {
			Assert::IsTrue(NextStore::getAllNextPair() == expectedNextPairList);
		}

		TEST_METHOD(getAllNextStarPair_SUCCESS) {
			Assert::IsTrue(NextStore::getAllNextStarPair() == expectedNextStarPairList);
		}

		TEST_METHOD(getAllPreviousStmt_SUCCESS) {
			Assert::IsTrue(NextStore::getAllPreviousStmt() == expectedPrevList);
		}

		TEST_METHOD(getAllNextStmt_SUCCESS) {
			Assert::IsTrue(NextStore::getAllNextStmt() == expectedNextList);
		}
	};
}

