#include "stdafx.h"
#include "CppUnitTest.h"
#include "FollowsStoreTestData.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{
	TEST_CLASS(FollowsStoreTest)
	{
	public:
		
		TEST_CLASS_INITIALIZE(InitializeStore)
		{
			FollowsStore fStore;
			fStore.clear(); // need to clear for unit testing, fStore used by multiple test files
			fStore.addFollows(1, 2);
			fStore.addFollows(2, 3);
			fStore.addFollows(4, 5);
			fStore.addFollows(5, 7);
			fStore.addFollows(9, 10);
			fStore.addAllFollower(1, { 2, 3 });
			fStore.addAllFollower(2, { 3 });
			fStore.addAllFollower(4, { 5, 7 });
			fStore.addAllFollower(5, { 7 });
			fStore.addAllFollower(9, { 10 });
			fStore.addAllFollowed(2, { 1 });
			fStore.addAllFollowed(3, { 1, 2 });
			fStore.addAllFollowed(5, { 4 });
			fStore.addAllFollowed(7, { 4, 5 });
			fStore.addAllFollowed(10, { 9 });
		}

		// SETTER METHOD TESTS
		TEST_METHOD(addFollows_INPUT_ERROR) {
			Assert::IsFalse(FollowsStore().addFollows(2,1));
		}

		TEST_METHOD(addFollows_DUPLICATE) {
			Assert::IsFalse(FollowsStore().addFollows(1, 2));
		}

		TEST_METHOD(addAllFollowed_WRONG_ORDER) {
			Assert::IsFalse(FollowsStore().addAllFollowed(3, { 7, 8, 9 }));
		}

		TEST_METHOD(addAllFollower_WRONG_ORDER) {
			Assert::IsFalse(FollowsStore().addAllFollower(2, { 3, 4, 5 }));
		}

		// QUERY METHOD TESTS
		TEST_METHOD(stmtHasFollowed_TRUE) {
			Assert::IsTrue(FollowsStore().stmtHasFollowed(7));
		}

		TEST_METHOD(stmtHasFollowed_FALSE) {
			Assert::IsFalse(FollowsStore().stmtHasFollowed(1));
		}

		TEST_METHOD(stmtHasFollower_TRUE) {
			Assert::IsTrue(FollowsStore().stmtHasFollower(1));
		}

		TEST_METHOD(stmtHasFollower_FALSE) {
			Assert::IsFalse(FollowsStore().stmtHasFollower(8));
		}

		TEST_METHOD(programHasFollowedStmt_TRUE) {
			Assert::IsTrue(FollowsStore().programHasFollowedStmt());
		}

		TEST_METHOD(programHasFollowerStmt_TRUE) {
			Assert::IsTrue(FollowsStore().programHasFollowerStmt());
		}

		// GETTER METHOD TESTS
		TEST_METHOD(getFollowedOfStmt_VALID) {
			Assert::AreEqual(FollowsStore().getFollowedOfStmt(2), expectedStmtFollowsMap.at(2).stmtFollowed);
		}

		TEST_METHOD(getFollowedOfStmt_INVALID) {
			Assert::AreEqual(FollowsStore().getFollowedOfStmt(9), expectedStmtFollowsMap.at(9).stmtFollowed);
		}

		TEST_METHOD(getFollowerOfStmt_VALID) {
			Assert::AreEqual(FollowsStore().getFollowerOfStmt(5), expectedStmtFollowsMap.at(5).stmtFollower);
		}

		TEST_METHOD(getFollowerOfStmt_INVALID) {
			Assert::AreEqual(FollowsStore().getFollowerOfStmt(12), 0);
		}

		TEST_METHOD(getFollowedListOfStmt_VALID) {
			Assert::IsTrue(FollowsStore().getFollowedListOfStmt(3) == expectedStmtFollowsMap.at(3).listOfFollowed);
		}

		TEST_METHOD(getFollowerListOfStmt_VALID) {
			Assert::IsTrue(FollowsStore().getFollowerListOfStmt(5) == expectedStmtFollowsMap.at(5).listOfFollower);
		}
		
		TEST_METHOD(getAllFollowsPair_TRUE) {
			Assert::IsTrue(FollowsStore().getAllFollowsPair() == expectedFollowsPairList);
		}

		TEST_METHOD(getAllFollowsStarPair_TRUE) {
			Assert::IsTrue(FollowsStore().getAllFollowsStarPair() == expectedFollowsStarPairList);
		}
		TEST_METHOD(getAllFollowedStmt_TRUE) {
			Assert::IsTrue(FollowsStore().getAllFollowedStmt() == expectedFollowedList);
		}

		TEST_METHOD(getAllFollowerStmt_TRUE) {
			Assert::IsTrue(FollowsStore().getAllFollowerStmt() == expectedFollowerList);
		}

	};
}

