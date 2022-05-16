#include "stdafx.h"
#include "CppUnitTest.h"
#include "IfStoreTestData.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{
	TEST_CLASS(IfStoreTest)
	{
	public:

		TEST_CLASS_INITIALIZE(InitializeStore)
		{
			IfStore::clear();
			IfStore::addIfUses(5, { "c", "k" });
			IfStore::addIfUses(11, { "a" });
			IfStore::addIfUses(13, { "a", "b" });
		}

		// SETTER METHOD TESTS
		TEST_METHOD(addIfUses_FAIL_EMPTY_VARLIST) {
			Assert::IsFalse(IfStore::addIfUses(8, {}));
		}

		TEST_METHOD(addIfUses_FAIL_DUPLICATE) {
			Assert::IsFalse(IfStore::addIfUses(5, { "PPAP" }));
		}

		// GETTER METHOD TESTS
		TEST_METHOD(getIfStmtUsesMap_SUCCESS) {
			Assert::IsTrue(IfStore::getIfStmtUsesMap() == expectedIfStmtUsesMap);
		}

		TEST_METHOD(getVarToIfStmtMap_SUCCESS) {
			Assert::IsTrue(IfStore::getVarToIfStmtMap() == expectedVarToIfStmtMap);
		}

		TEST_METHOD(getIfUsedVarList_SUCCESS) {
			Assert::IsTrue(IfStore::getIfUsedVarList() == expectedIfUsedVarList);
		}

		TEST_METHOD(getIfUsesStmtList_SUCCESS) {
			Assert::IsTrue(IfStore::getIfUsesStmtList() == expectedIfUsesStmtList);
		}

		TEST_METHOD(getVarUsedByIf_SUCCESS_VALID_WHILE_STMT) {
			Assert::IsTrue(IfStore::getVarsUsedByIf(5) == expectedIfStmtUsesMap.at(5));
		}

		TEST_METHOD(getVarUsedByIf_FAIL_INVALID_WHILE_STMT) {
			Assert::IsTrue(IfStore::getVarsUsedByIf(8) == std::unordered_set <std::string>{});
		}

		TEST_METHOD(getVarUsedByIf_FAIL_NON_EXISTENT_STMT) {
			Assert::IsTrue(IfStore::getVarsUsedByIf(20) == std::unordered_set <std::string>{});
		}

		TEST_METHOD(getIfStmtsUsingVar_SUCCESS) {
			Assert::IsTrue(IfStore::getIfStmtsUsingVar("a") == expectedVarToIfStmtMap.at("a"));
		}

		TEST_METHOD(getIfStmtsUsingVar_FAIL_NON_EXISTENT_VAR) {
			Assert::IsTrue(IfStore::getIfStmtsUsingVar("PPAP") == std::unordered_set<int>{});
		}
	};
}
