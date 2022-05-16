#include "stdafx.h"
#include "CppUnitTest.h"
#include "AffectsStoreTestData.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{
	TEST_CLASS(AffectsStoreTest)
	{
	public:

		TEST_CLASS_INITIALIZE(InitializeStore)
		{
			AffectsStore::clear();
			AffectsStore::addAffects(1, 2);
			AffectsStore::addAffects(6, 4);
			AffectsStore::addAffects(2, 4);
			AffectsStore::addAffects(8, 4);

			AffectsStore::addAffectingT(1, { 2, 4 });
			AffectsStore::addAffectedByT(4, { 1, 2, 6, 8});
		}

		TEST_METHOD(TestStmtHasAffectedBy_True) {
			Assert::IsTrue(AffectsStore::stmtHasAffectedBy(2));
		}

		TEST_METHOD(TestStmtHasAffectedBy_False) {
			Assert::IsFalse(AffectsStore::stmtHasAffectedBy(1));
		}

		TEST_METHOD(TestStmtHasAffecting_True) {
			Assert::IsTrue(AffectsStore::stmtHasAffecting(6));
		}

		TEST_METHOD(TestStmtHasAffecting_False) {
			Assert::IsFalse(AffectsStore::stmtHasAffecting(4));
		}

		TEST_METHOD(TestProgramHasAffected_True) {
			Assert::IsTrue(AffectsStore::programHasAffected());
		}

		TEST_METHOD(TestProgramHasAffecting_True) {
			Assert::IsTrue(AffectsStore::programHasAffecting());
		}

		/*TEST_METHOD(TestGetStmtAffectsMap) {
			Assert::IsTrue(AffectsStore::getStmtAffectsMap() == expectedStmtAffectsMap);
		}*/

		TEST_METHOD(TestGetAffectsPairList) {
			std::unordered_set<std::pair<int, int>, IntPairHasher> result = AffectsStore::getAffectsPairList();

			Assert::IsTrue(result == expectedAffectsPairList);
		}

		TEST_METHOD(TestGetAffectsStarPairList) {
			std::unordered_set<std::pair<int, int>, IntPairHasher> result = AffectsStore::getAffectsStarPairList();

			Assert::IsTrue(result == expectedAffectsStarPairList);
		}

		TEST_METHOD(TestGetAffectedList) {
			std::unordered_set<int> result = AffectsStore::getAffectedList();

			Assert::IsTrue(result == expectedAffectedList);
		}

		TEST_METHOD(TestGetAffectingList) {
			std::unordered_set<int> result = AffectsStore::getAffectingList();

			Assert::IsTrue(result == expectedAffectingList);
		}

		TEST_METHOD(TestGetAffectedByofStmt_NonEmpty_InStore) {
			std::unordered_set<int> result = AffectsStore::getAffectedByOfStmt(4);
			std::unordered_set<int> expected = { 2, 6, 8 };
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(TestGetAffectedByOfStmt_Empty_InStore) {
			std::unordered_set<int> result = AffectsStore::getAffectedByOfStmt(1);
			std::unordered_set<int> expected = {};

			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(TestGetAffectingByOfStmt_Empty_NotInStore) {
			std::unordered_set<int> result = AffectsStore::getAffectedByOfStmt(10);
			std::unordered_set<int> expected = {};

			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(TestGetAffectingOfStmt_NotEmpty) {
			std::unordered_set<int> result = AffectsStore::getAffectingOfStmt(1);
			std::unordered_set<int> expected = {2};
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(TestGetAffectingOfStmt_Empty_InStore) {
			std::unordered_set<int> result = AffectsStore::getAffectingOfStmt(4);
			std::unordered_set<int> expected = {};

			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(TestGetAffectingOfStmt_Empty_NotInStore) {
			std::unordered_set<int> result = AffectsStore::getAffectedByOfStmt(10);
			std::unordered_set<int> expected = {};

			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(TestGetAffectedByTOfStmt_NotEmpty) {
			std::unordered_set<int> result = AffectsStore::getAffectedByTOfStmt(4);
			std::unordered_set<int> expected = {1, 2, 6, 8};

			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(TestGetAffectedByTOfStmt_Empty_InStore) {
			std::unordered_set<int> result = AffectsStore::getAffectedByTOfStmt(8);
			std::unordered_set<int> expected = {};

			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(TestGetAffectedByTOfStmt_Empty_NotIntStore) {
			std::unordered_set<int> result = AffectsStore::getAffectedByTOfStmt(3);
			std::unordered_set<int> expected = {};

			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(TestGetAffectingTOfStmt_NotEmpty) {
			std::unordered_set<int> result = AffectsStore::getAffectingTOfStmt(1);
			std::unordered_set<int> expected = {2, 4};

			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(TestGetAffectingTOfStmt_Empty_InStore) {
			std::unordered_set<int> result = AffectsStore::getAffectingTOfStmt(4);
			std::unordered_set<int> expected = {};

			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(TestGetAffectingTOfStmt_Empty_NotInStore) {
			std::unordered_set<int> result = AffectsStore::getAffectingTOfStmt(11);
			std::unordered_set<int> expected = {};

			Assert::IsTrue(result == expected);
		}

	};

}