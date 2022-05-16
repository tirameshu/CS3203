#include "stdafx.h"
#include "CppUnitTest.h"
#include "NextBipStoreTestData.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{
	TEST_CLASS(NextBipStoreTest)
	{
	public:

		TEST_CLASS_INITIALIZE(InitializeStore)
		{
			NextBipStore::clear();
			NextBipStore::addNextBip(1, 2, 0);
			NextBipStore::addNextBip(2, 3, 0);
			NextBipStore::addNextBip(3, 4, 0);
			NextBipStore::addNextBip(3, 9, 0);
			NextBipStore::addNextBip(4, 5, 0);
			NextBipStore::addNextBip(5, 6, 0);
			NextBipStore::addNextBip(5, 7, 0);
			NextBipStore::addNextBip(6, 8, 0);
			NextBipStore::addNextBip(7, 10, 7);
			NextBipStore::addNextBip(8, 3, 0);
			NextBipStore::addNextBip(10, 11, 0);
			NextBipStore::addNextBip(11, 8, 7);
			NextBipStore::addNextBip(11, 12, 0);
			NextBipStore::addNextBip(12, 13, 12);
			NextBipStore::addNextBip(13, 11, 0);

			NextBipStore::addAllPreviousBip(2, { 1 });
			NextBipStore::addAllPreviousBip(3, { 1, 2, 3, 4, 5, 6, 7, 8, 10, 11, 12, 13 });
			NextBipStore::addAllPreviousBip(4, { 1, 2, 3, 4, 5, 6, 7, 8, 10, 11, 12, 13 });
			NextBipStore::addAllPreviousBip(5, { 1, 2, 3, 4, 5, 6, 7, 8, 10, 11, 12, 13 });
			NextBipStore::addAllPreviousBip(6, { 1, 2, 3, 4, 5, 6, 7, 8, 10, 11, 12, 13 });
			NextBipStore::addAllPreviousBip(7, { 1, 2, 3, 4, 5, 6, 7, 8, 10, 11, 12, 13 });
			NextBipStore::addAllPreviousBip(8, { 1, 2, 3, 4, 5, 6, 7, 8, 10, 11, 12, 13 });
			NextBipStore::addAllPreviousBip(9, { 1, 2, 3, 4, 5, 6, 7, 8, 10, 11, 12, 13 });
			NextBipStore::addAllPreviousBip(10, { 1, 2, 3, 4, 5, 6, 7, 8, 10, 11, 12, 13 });
			NextBipStore::addAllPreviousBip(11, { 1, 2, 3, 4, 5, 6, 7, 8, 10, 11, 12, 13 });
			NextBipStore::addAllPreviousBip(12, { 1, 2, 3, 4, 5, 6, 7, 8, 10, 11, 12, 13 });
			NextBipStore::addAllPreviousBip(13, { 1, 2, 3, 4, 5, 6, 7, 8, 10, 11, 12, 13 });

			NextBipStore::addAllNextBip(1, { 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13 });
			NextBipStore::addAllNextBip(2, { 3, 4, 5, 6, 7, 8, 9 , 10, 11, 12, 13});
			NextBipStore::addAllNextBip(3, { 3, 4, 5, 6, 7, 8, 9 , 10, 11, 12, 13 });
			NextBipStore::addAllNextBip(4, { 3, 4, 5, 6, 7, 8, 9 , 10, 11, 12, 13 });
			NextBipStore::addAllNextBip(5, { 3, 4, 5, 6, 7, 8, 9 , 10, 11, 12, 13 });
			NextBipStore::addAllNextBip(6, { 3, 4, 5, 6, 7, 8, 9 , 10, 11, 12, 13 });
			NextBipStore::addAllNextBip(7, { 3, 4, 5, 6, 7, 8, 9 , 10, 11, 12, 13 });
			NextBipStore::addAllNextBip(8, { 3, 4, 5, 6, 7, 8, 9 , 10, 11, 12, 13 });
			NextBipStore::addAllNextBip(10, { 3, 4, 5, 6, 7, 8, 9 , 10, 11, 12, 13 });
			NextBipStore::addAllNextBip(11, { 3, 4, 5, 6, 7, 8, 9 , 10, 11, 12, 13 });
			NextBipStore::addAllNextBip(12, { 3, 4, 5, 6, 7, 8, 9 , 10, 11, 12, 13 });
			NextBipStore::addAllNextBip(13, { 3, 4, 5, 6, 7, 8, 9 , 10, 11, 12, 13 });
		}

		// SETTER METHOD TESTS
		TEST_METHOD(addNextBip_DUPLICATE) {
			Assert::IsFalse(NextBipStore::addNextBip(1, 2, 0));
		}

		TEST_METHOD(addAllPreviousBip_INVALID_STMT_ROOT_NODE) {
			Assert::IsFalse(NextBipStore::addAllPreviousBip(1, { 0, 2, 3, 4 }));
		}

		TEST_METHOD(addAllPreviousBip_NON_EXISTENT_STMT) {
			Assert::IsFalse(NextBipStore::addAllPreviousBip(15, { 0, 2, 3, 4 }));
		}

		TEST_METHOD(addAllNextBip_INVALID_STMT_ROOT_NODE) {
			Assert::IsFalse(NextBipStore::addAllNextBip(9, { 0, 2, 3, 4 }));
		}

		TEST_METHOD(addAllNextBip_NON_EXISTENT_STMT) {
			Assert::IsFalse(NextBipStore::addAllNextBip(15, { 0, 2, 3, 4 }));
		}

		// QUERY METHOD TESTS
		TEST_METHOD(stmtHasPrev_SUCCESS_STMT_HAS_PREV) {
			Assert::IsTrue(NextBipStore::stmtHasPrevBip(3));
		}

		TEST_METHOD(stmtHasPrev_SUCCESS_STMT_DOES_NOT_HAVE_PREV) {
			Assert::IsFalse(NextBipStore::stmtHasPrevBip(1));
		}

		TEST_METHOD(stmtHasPrev_UNSUCCESSFUL_NON_EXISTENT_STMT) {
			Assert::IsFalse(NextBipStore::stmtHasPrevBip(15));
		}

		TEST_METHOD(stmtHasNextBip_SUCCESS_STMT_HAS_NEXT) {
			Assert::IsTrue(NextBipStore::stmtHasNextBip(8));
		}

		TEST_METHOD(stmtHasNextBip_SUCCESS_STMT_DOES_NOT_HAVE_NextBip) {
			Assert::IsFalse(NextBipStore::stmtHasNextBip(9));
		}

		TEST_METHOD(stmtHasNextBip_UNSUCCESSFUL_NON_EXISTENT_STMT) {
			Assert::IsFalse(NextBipStore::stmtHasNextBip(15));
		}

		TEST_METHOD(programHasPrevStmt_SUCCESS) {
			Assert::IsTrue(NextBipStore::programHasPrevBipStmt());
		}

		TEST_METHOD(programHasNextBipStmt_SUCCESS) {
			Assert::IsTrue(NextBipStore::programHasNextBipStmt());
		}

		// GETTER METHOD TESTS
		TEST_METHOD(getPrevOfStmt_SUCCESS_STMT_HAS_PREV) {
			Assert::IsTrue(NextBipStore::getPrevBipOfStmt(8) == expectedStmtNextBipMap.at(8).prevBipStmts);
		}

		TEST_METHOD(getPrevOfStmt_SUCCESS_STMT_DOES_NOT_HAVE_PREV) {
			Assert::IsTrue(NextBipStore::getPrevBipOfStmt(1) == expectedStmtNextBipMap.at(1).prevBipStmts);
		}

		TEST_METHOD(getPrevOfStmt_SUCCESS_NON_EXISTENT_STMT) {
			Assert::IsTrue(NextBipStore::getPrevBipOfStmt(15) == std::unordered_set<int>{});
		}

		TEST_METHOD(getNextBipOfStmt_SUCCESS_STMT_HAS_NEXT) {
			Assert::IsTrue(NextBipStore::getNextBipOfStmt(5) == expectedStmtNextBipMap.at(5).nextBipStmts);
		}

		TEST_METHOD(getNextBipOfStmt_SUCCESS_STMT_DOES_NOT_HAVE_NEXT) {
			Assert::IsTrue(NextBipStore::getNextBipOfStmt(9) == expectedStmtNextBipMap.at(9).nextBipStmts);
		}

		TEST_METHOD(getNextBipOfStmt_SUCCESS_NON_EXISTENT_STMT) {
			Assert::IsTrue(NextBipStore::getNextBipOfStmt(15) == std::unordered_set<int>{});
		}

		TEST_METHOD(getPrevStarListOfStmt_SUCCESS_STMT_HAS_PREV) {
			Assert::IsTrue(NextBipStore::getPrevBipStarListOfStmt(8) == expectedStmtNextBipMap.at(8).listOfPrevBipStarStmt);
		}

		TEST_METHOD(getPrevStarListOfStmt_SUCCESS_STMT_DOES_NOT_HAVE_PREV) {
			Assert::IsTrue(NextBipStore::getPrevBipStarListOfStmt(1) == expectedStmtNextBipMap.at(1).listOfPrevBipStarStmt);
		}

		TEST_METHOD(getPrevStarListOfStmt_SUCCESS_NON_EXISTENT_STMT) {
			Assert::IsTrue(NextBipStore::getPrevBipStarListOfStmt(15) == std::unordered_set<int>{});
		}

		TEST_METHOD(getNextBipStarListOfStmt_SUCCESS_STMT_HAS_NextBip) {
			Assert::IsTrue(NextBipStore::getNextBipStarListOfStmt(5) == expectedStmtNextBipMap.at(5).listOfNextBipStarStmt);
		}

		TEST_METHOD(getNextBipStarListOfStmt_SUCCESS_STMT_DOES_NOT_HAVE_NEXT) {
			Assert::IsTrue(NextBipStore::getNextBipStarListOfStmt(9) == expectedStmtNextBipMap.at(9).listOfNextBipStarStmt);
		}

		TEST_METHOD(getNextBipStarListOfStmt_SUCCESS_NON_EXISTENT_STMT) {
			Assert::IsTrue(NextBipStore::getNextBipStarListOfStmt(15) == std::unordered_set<int>{});
		}

		TEST_METHOD(getNextBipListOfCallersOfStmt_SUCCESS_HAS_CALLER) {
			Assert::IsTrue(NextBipStore::getNextBipListOfIncomingOfStmt(10) == std::unordered_set<int>{7});
		}

		TEST_METHOD(getNextBipListOfCallersOfStmt_SUCCESS_HAS_NO_CALLER) {
			Assert::IsTrue(NextBipStore::getNextBipListOfIncomingOfStmt(11) == std::unordered_set<int>{});
		}

		TEST_METHOD(getNextBipListOfCallersOfStmt_FALSE_HAVE_CALLER) {
			Assert::IsTrue(NextBipStore::getNextBipListOfIncomingOfStmt(8) == std::unordered_set<int>{7});
		}

		TEST_METHOD(getAllNextBipPair_SUCCESS) {
			Assert::IsTrue(NextBipStore::getAllNextBipPair() == expectedNextBipPairList);
		}

		TEST_METHOD(getAllNextBipStarPair_SUCCESS) {
			Assert::IsTrue(NextBipStore::getAllNextBipStarPair() == expectedNextBipStarPairList);
		}

		TEST_METHOD(getAllPreviousBipStmt_SUCCESS) {
			Assert::IsTrue(NextBipStore::getAllPreviousBipStmt() == expectedPrevBipList);
		}

		TEST_METHOD(getAllNextBipStmt_SUCCESS) {
			Assert::IsTrue(NextBipStore::getAllNextBipStmt() == expectedNextBipList);
		}
	};
}

