#include "stdafx.h"
#include "CppUnitTest.h"
#include "AffectsBipStoreTestData.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{
	TEST_CLASS(AffectsBipStoreTest)
	{
	public:

		TEST_CLASS_INITIALIZE(InitializeStore)
		{
			AffectsBipStore::clear();
			AffectsBipStore::addAffectsBip(1, 2);
			AffectsBipStore::addAffectsBip(1, 4);
			AffectsBipStore::addAffectsBip(1, 10);
			AffectsBipStore::addAffectsBip(2, 4);
			AffectsBipStore::addAffectsBip(2, 10);
			AffectsBipStore::addAffectsBip(2, 13);
			AffectsBipStore::addAffectsBip(6, 4);
			AffectsBipStore::addAffectsBip(8, 4);
			AffectsBipStore::addAffectsBip(10, 8);
			AffectsBipStore::addAffectsBip(13, 4);
			AffectsBipStore::addAffectsBip(13, 10);

			AffectsBipStore::addAffectedBipByT(2, { 1 });
			AffectsBipStore::addAffectedBipByT(4, { 1, 2, 6, 8, 10, 13 });
			AffectsBipStore::addAffectedBipByT(8, { 2, 10, 13 });
			AffectsBipStore::addAffectedBipByT(10, { 1, 2, 13 });
			AffectsBipStore::addAffectedBipByT(13, { 2 });

			AffectsBipStore::addAffectingBipT(1, { 2, 4, 10, 13 });
			AffectsBipStore::addAffectingBipT(2, { 4, 10, 13, 8 });
			AffectsBipStore::addAffectingBipT(6, { 4 });
			AffectsBipStore::addAffectingBipT(8, { 4 });
			AffectsBipStore::addAffectingBipT(10, { 4, 8 });
			AffectsBipStore::addAffectingBipT(13, { 4, 8, 10 });
		}


		TEST_METHOD(TestStmtHasAffectedBipBy_True) {
			Assert::IsTrue(AffectsBipStore::stmtHasAffectedBipBy(4));
		}

		TEST_METHOD(TestStmtHasAffectedBipBy_False_InStore) {
			Assert::IsFalse(AffectsBipStore::stmtHasAffectedBipBy(1));
		}

		TEST_METHOD(TestStmtHasAffectedBipBy_False_NotInStore) {
			Assert::IsFalse(AffectsBipStore::stmtHasAffectedBipBy(14));
		}

		TEST_METHOD(TestStmtHasAffectingBip_True) {
			Assert::IsTrue(AffectsBipStore::stmtHasAffectingBip(1));
		}

		TEST_METHOD(TestStmtHasAffectingBip_False_InStore) {
			Assert::IsFalse(AffectsBipStore::stmtHasAffectingBip(4));
		}

		TEST_METHOD(TestStmtHasAffectingBip_False_NotInStore) {
			Assert::IsFalse(AffectsBipStore::stmtHasAffectingBip(15));
		}

		TEST_METHOD(TestProgramHasAffectingBip_True) {
			Assert::IsTrue(AffectsBipStore::programHasAffectingBip());
		}

		TEST_METHOD(TestProgramHasAffectedBip_True) {
			Assert::IsTrue(AffectsBipStore::programHasAffectedBip());
		}

		/*TEST_METHOD(TestGetStmtAffectsBipMap) {
			std::unordered_map<int, affectsBipStruct> result = AffectsBipStore::getStmtAffectsBipMap();
			Assert::IsTrue(result == expectedStmtAffectsBipMap);
		}*/

		TEST_METHOD(TestGetAffectsBipPairList) {
			std::unordered_set<std::pair<int, int>, IntPairHasher> result = AffectsBipStore::getAffectsBipPairList();
			Assert::IsTrue(result == expectedAffectsBipPairList);
		}

		TEST_METHOD(TestGetAffectsBipStarPairList) {
			std::unordered_set<std::pair<int, int>, IntPairHasher> result = AffectsBipStore::getAffectsBipStarPairList();
			Assert::IsTrue(result == expectedAffectsBipStarPairList);
		}

		TEST_METHOD(TestGetAffectedBipList) {
			std::unordered_set<int> result = AffectsBipStore::getAffectedBipList();
			Assert::IsTrue(result == expectedAffectedBipList);
		}

		TEST_METHOD(TestGetAffectingBipList) {
			std::unordered_set<int> result = AffectsBipStore::getAffectingBipList();
			Assert::IsTrue(result == expectedAffectingBipList);
		}

		TEST_METHOD(TestGetAffectedBipByOfStmt_NonEmpty) {
			std::unordered_set<int> result = AffectsBipStore::getAffectedBipByOfStmt(4);
			std::unordered_set<int> expected = {1, 2, 6, 8, 13};
			//for (auto& x : result) {
			//	Logger::WriteMessage(std::to_string(x).c_str());
			//}
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(TestGetAffectedBipByOfStmt_Empty) {
			std::unordered_set<int> result = AffectsBipStore::getAffectedBipByOfStmt(1);
			std::unordered_set<int> expected = {};

			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(TestGetAffectingBipOfStmt_NonEmpty) {
			std::unordered_set<int> result = AffectsBipStore::getAffectingBipOfStmt(2);
			std::unordered_set<int> expected = { 4, 10, 13 };

			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(TestGetAffectingBipOfStmt_Empty) {
			std::unordered_set<int> result = AffectsBipStore::getAffectingBipOfStmt(4);
			std::unordered_set<int> expected = { };

			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(TestGetAffectedBipByTOfStmt_NonEmpty) {
			std::unordered_set<int> result = AffectsBipStore::getAffectedBipByTOfStmt(4);
			std::unordered_set<int> expected = { 1, 2, 6, 8, 10, 13 };

			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(TestGetAffectedBipByTOfStmt_Empty) {
			std::unordered_set<int> result = AffectsBipStore::getAffectedBipByTOfStmt(1);
			std::unordered_set<int> expected = { };

			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(TestGetAffectingBipTOfStmt_NonEmpty) {
			std::unordered_set<int> result = AffectsBipStore::getAffectingBipTOfStmt(13);
			std::unordered_set<int> expected = { 4, 8, 10 };

			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(TestGetAffectingBipTOfStmt_Empty) {
			std::unordered_set<int> result = AffectsBipStore::getAffectingBipTOfStmt(4);
			std::unordered_set<int> expected = { };

			Assert::IsTrue(result == expected);
		}
	};
}