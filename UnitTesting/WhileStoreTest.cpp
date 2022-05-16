#include "stdafx.h"
#include "CppUnitTest.h"
#include "WhileStoreTestData.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{
	TEST_CLASS(WhileStoreTest)
	{
	public:

		TEST_CLASS_INITIALIZE(InitializeStore)
		{
			WhileStore::clear();
			WhileStore::addWhileUses(3, { "w" });
			WhileStore::addWhileUses(11, { "a" });
			WhileStore::addWhileUses(13, { "a", "b" });
		}

		// SETTER METHOD TESTS
		TEST_METHOD(addWhileUses_FAIL_EMPTY_VARLIST) {
			Assert::IsFalse(WhileStore::addWhileUses(5, {}));
		}

		TEST_METHOD(addWhileUses_FAIL_DUPLICATE) {
			Assert::IsFalse(WhileStore::addWhileUses(3, { "PPAP" }));
		}

		// GETTER METHOD TESTS
		TEST_METHOD(getWhileStmtUsesMap_SUCCESS) {
			Assert::IsTrue(WhileStore::getWhileStmtUsesMap() == expectedWhileStmtUsesMap);
		}

		TEST_METHOD(getVarToWhileStmtMap_SUCCESS) {
			Assert::IsTrue(WhileStore::getVarToWhileStmtMap() == expectedVarToWhileStmtMap);
		}

		TEST_METHOD(getWhileUsedVarList_SUCCESS) {
			Assert::IsTrue(WhileStore::getWhileUsedVarList() == expectedWhileUsedVarList);
		}

		TEST_METHOD(getWhileUsesStmtList_SUCCESS) {
			Assert::IsTrue(WhileStore::getWhileUsesStmtList() == expectedWhileUsesStmtList);
		}

		TEST_METHOD(getVarUsedByWhile_SUCCESS_VALID_WHILE_STMT) {
			Assert::IsTrue(WhileStore::getVarsUsedByWhile(13) == expectedWhileStmtUsesMap.at(13));
		}

		TEST_METHOD(getVarUsedByWhile_FAIL_INVALID_WHILE_STMT) {
			Assert::IsTrue(WhileStore::getVarsUsedByWhile(5) == std::unordered_set <std::string>{});
		}

		TEST_METHOD(getVarUsedByWhile_FAIL_NON_EXISTENT_STMT) {
			Assert::IsTrue(WhileStore::getVarsUsedByWhile(20) == std::unordered_set <std::string>{});
		}

		TEST_METHOD(getWhileStmtsUsingVar_SUCCESS) {
			Assert::IsTrue(WhileStore::getWhileStmtsUsingVar("a") == expectedVarToWhileStmtMap.at("a"));
		}

		TEST_METHOD(getWhileStmtsUsingVar_FAIL_NON_EXISTENT_VAR) {
			Assert::IsTrue(WhileStore::getWhileStmtsUsingVar("PPAP") == std::unordered_set<int>{});
		}
	};
}
