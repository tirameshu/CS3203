#include "stdafx.h"
#include "CppUnitTest.h"
#include "ParentStoreTestData.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{
	TEST_CLASS(ParentStoreTest)
	{
	public:

		TEST_CLASS_INITIALIZE(InitializeStore)
		{
			ParentStore pStore;
			pStore.clear(); // need to clear for unit testing, pStore used by multiple test files
			pStore.addParent(3, { 4, 5, 7, 8 });
			pStore.addParent(5, { 6 });
			pStore.addParent(10, { 11 });
			pStore.addAncestors(4, { 3 });
			pStore.addAncestors(5, { 3 });
			pStore.addAncestors(6, { 3, 5 });
			pStore.addAncestors(7, { 3 });
			pStore.addAncestors(8, { 3 });
			pStore.addAncestors(11, { 10 });
			pStore.addDescendants(3, { 4, 5, 6, 7, 8 });
			pStore.addDescendants(5, { 6 });
			pStore.addDescendants(10, { 11 });
		}

		// SETTER METHOD TESTS
		TEST_METHOD(addParent_INPUT_ERROR) {
			Assert::IsFalse(ParentStore().addParent(5, { 3, 4 }));
		}

		TEST_METHOD(addAncestors_WRONG_ORDER) {
			Assert::IsFalse(ParentStore().addAncestors(4, { 1, 2, 3 }));
		}

		TEST_METHOD(addDescendants_WRONG_ORDER) {
			Assert::IsFalse(ParentStore().addDescendants(5, { 7, 8, 9 }));
		}

		// QUERY METHOD TESTS
		TEST_METHOD(stmtHasParent_TRUE) {
			Assert::IsTrue(ParentStore().stmtHasParent(4));
		}

		TEST_METHOD(stmtHasParent_FALSE) {
			Assert::IsFalse(ParentStore().stmtHasParent(1));
		}

		TEST_METHOD(stmtHasChildren_TRUE) {
			Assert::IsTrue(ParentStore().stmtHasChildren(3));
		}

		TEST_METHOD(stmtHasChildren_FALSE) {
			Assert::IsFalse(ParentStore().stmtHasParent(12));
		}

		TEST_METHOD(programHasParentStmt_TRUE) {
			Assert::IsTrue(ParentStore().programHasParentStmt());
		}

		TEST_METHOD(programHasChildStmt_TRUE) {
			Assert::IsTrue(ParentStore().programHasChildStmt());
		}

		// GETTER METHOD TESTS
		TEST_METHOD(getParentOfStmt_VALID) {
			Assert::AreEqual(ParentStore().getParentOfStmt(6), expectedStmtParentMap.at(6).stmtParent);
		}

		TEST_METHOD(getParentOfStmt_INVALID) {
			Assert::AreEqual(ParentStore().getParentOfStmt(9), 0);
		}

		TEST_METHOD(getChildrenListOfStmt_VALID) {
			Assert::IsTrue(ParentStore().getChildrenListOfStmt(3) == expectedStmtParentMap.at(3).listOfChildren);
		}

		TEST_METHOD(getChildrenListOfStmt_INVALID) {
			Assert::IsTrue(ParentStore().getChildrenListOfStmt(2) == std::unordered_set<int>{});
		}

		TEST_METHOD(getAncestorsListOfStmt_VALID) {
			Assert::IsTrue(ParentStore().getAncestorsListOfStmt(6) == expectedStmtParentMap.at(6).listOfAncestors);
		}

		TEST_METHOD(getAncestorsListOfStmt_INVALID) {
			Assert::IsTrue(ParentStore().getAncestorsListOfStmt(3) == expectedStmtParentMap.at(3).listOfAncestors);
		}

		TEST_METHOD(getDescendantsListOfStmt_VALID) {
			Assert::IsTrue(ParentStore().getDescendantsListOfStmt(3) == expectedStmtParentMap.at(3).listOfDescendants);
		}

		TEST_METHOD(getDescendantsListOfStmt_INVALID) {
			Assert::IsTrue(ParentStore().getDescendantsListOfStmt(8) == expectedStmtParentMap.at(8).listOfDescendants);
		}

		TEST_METHOD(getAllParentPair_TRUE) {
			Assert::IsTrue(ParentStore().getAllParentPair() == expectedParentPairList);
		}

		TEST_METHOD(getAllParentStarPair_TRUE) {
			Assert::IsTrue(ParentStore().getAllParentStarPair() == expectedParentStarPairList);
		}

		TEST_METHOD(getAllParentStmt_TRUE) {
			Assert::IsTrue(ParentStore().getAllParentStmt() == expectedParentList);
		}

		TEST_METHOD(getAllChildStmt_TRUE) {
			Assert::IsTrue(ParentStore().getAllChildStmt() == expectedChildList);
		}
	};
}

