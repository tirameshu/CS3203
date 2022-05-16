#include "stdafx.h"
#include "CppUnitTest.h"
#include "../source/UsesStore.h"
#include "UsesStoreTestData.h"
#include "cassert"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{
	TEST_CLASS(TestUsesStore)
	{
	public:

		TEST_CLASS_INITIALIZE(InitializeStore) {
			UsesStore::clear();

			UsesStore::addUses(1, 2, { "p" });
			UsesStore::addUses(2, 1, { "p" });
			UsesStore::addUses(4, 1, { "a", "p", "w", "ifs" });
			UsesStore::addUses(6, 1, { "w" });
			UsesStore::addUses(9, 1, { "p", "a" });
			UsesStore::addUses(12, 1, { "a" });
			UsesStore::addUses(15, 1, { "p" });


			UsesStore::addUsesToContainer(3, 4, { "c", "k", "a", "p", "ifs", "w" });
			UsesStore::addUsesToContainer(5, 5, { "w" });
			UsesStore::addUsesToContainer(7,7, { "p", "a" });
			UsesStore::addUsesToContainer(8, 7, { "p", "a" });
			UsesStore::addUsesToContainer(10, 5, { "p", "a" });
			UsesStore::addUsesToContainer(11, 7, { "p", "a" });
			UsesStore::addUsesToContainer(13, 7, { "p" });

			UsesStore::addProcUses("testA", { "a", "c", "p", "k", "ifs", "w" });
			UsesStore::addProcUses("testB", { "a", "p" });
			UsesStore::addProcUses("testC", { "a", "p" });
			UsesStore::addProcUses("testD", { "p" });
		}


		////Tests for addUses()
		//TEST_METHOD(TestAddUses_AssignStmt) {
		//	UsesStore tempUsesStore = UsesStore();
		//	int stmtNo = 1;
		//	int stmtType = 1;
		//	std::unordered_set<std::string> varsUsed = { "a" };

		//	UsesStore::addUses(stmtNo, stmtType, varsUsed);

		//	Assert::IsTrue(UsesStore::hasAssignUses());
		//}

		//
		//TEST_METHOD(TestAddUses_PrintStmt) {
		//	UsesStore tempUsesStore = UsesStore();
		//	int stmtNo = 1;
		//	int stmtType = 2;
		//	std::unordered_set<std::string> varsUsed = { "a" };

		//	UsesStore::addUses(stmtNo, stmtType, varsUsed);

		//	std::unordered_set<int> printStmtList = UsesStore::getPrintStmtList();

		//	Assert::IsTrue(printStmtList.find(stmtNo) != printStmtList.end());
		//}

		//TEST_METHOD(TestAddUses_IfStmt) {
		//	UsesStore tempUsesStore = UsesStore();
		//	int stmtNo = 1;
		//	int stmtType = 4;
		//	std::unordered_set<std::string> varsUsed = { "a" };

		//	UsesStore::addUses(stmtNo, stmtType, varsUsed);

		//	Assert::IsTrue(UsesStore::hasIfUses());
		//}

		//TEST_METHOD(TestAddUses_WhileStmt) {
		//	UsesStore tempUsesStore = UsesStore();
		//	int stmtNo = 1;
		//	int stmtType = 5;
		//	std::unordered_set<std::string> varsUsed = { "a" };

		//	UsesStore::addUses(stmtNo, stmtType, varsUsed);

		//	Assert::IsTrue(UsesStore::hasWhileUses());
		//}


		////Tests for addUsesToContainer()
		//TEST_METHOD(TestAddUsesToContainer_IfStmt) {
		//	UsesStore tempUsesStore = UsesStore();
		//	int stmtNo = 1;
		//	int stmtType = 4;
		//	std::unordered_set<std::string> varList = { "a" };
		//	std::unordered_set<int> ifStmtList = { 1 };

		//	UsesStore::addUsesToContainer(stmtNo, stmtType, varList);

		//	Assert::IsTrue(UsesStore::getAllUsesIfStmts() == ifStmtList);
		//}

		//TEST_METHOD(TestAddUsesToContainer_WhileStmt) {
		//	UsesStore tempUsesStore = UsesStore();
		//	int stmtNo = 1;
		//	int stmtType = 5;
		//	std::unordered_set<std::string> varList = { "a" };
		//	std::unordered_set<int> whileStmtList = { 1 };

		//	UsesStore::addUsesToContainer(stmtNo, stmtType, varList);

		//	Assert::IsTrue(UsesStore::getAllUsesWhileStmts() == whileStmtList);
		//}

		//Test for isVarUsedInStmt(stmtNo, var), true
		TEST_METHOD(TestIsVarUsedInStmt_True) {
			bool result = UsesStore::isVarUsedInStmt(2, "p");
			Assert::IsTrue(result);
		}

		//Test for isVarUsedInStmt(stmtNo, var), false
		TEST_METHOD(TestIsVarUsedInStmt_False) {
			bool result = UsesStore::isVarUsedInStmt(1, "a");
			Assert::IsFalse(result);
		}

		//Test for isVarUsed(), true
		TEST_METHOD(TestIsVarUsed_True) {
			bool result = UsesStore::isVarUsed("a");
			Assert::IsTrue(result);
		}

		//Test for isVarUsed(), false
		TEST_METHOD(TestIsVarUsed_False) {
			bool result = UsesStore::isVarUsed("b");
			Assert::IsFalse(result);
		}

		//Test for isStmtUses(), True
		TEST_METHOD(TestIsStmtUses_True) {
			Assert::IsTrue(UsesStore::isStmtUses(1));
		}

		//Test for isStmtUses(), False
		TEST_METHOD(TestIsStmtUses_False) {
			Assert::IsFalse(UsesStore::isStmtUses(14));
		}


		//Test for hasIfUses, true
		TEST_METHOD(TestHasIfUses_True) {
			Assert::IsTrue(UsesStore::hasIfUses());
		}

		//Test for hasWhileUses, true
		TEST_METHOD(TestHasWhileUses_True) {
			Assert::IsTrue(UsesStore::hasWhileUses());
		}

		//Test for hasAssignUses, true
		TEST_METHOD(TestHasAssignUses_True) {
			Assert::IsTrue(UsesStore::hasAssignUses());
		}

		//Test for hasUses, true 
		TEST_METHOD(TestHasUses_True) {
			Assert::IsTrue(UsesStore::hasUses());
		}

		//Test for getStmtUsingVar, true
		TEST_METHOD(TestGetStmtUsingVar_True) {
			std::unordered_set<int> result = UsesStore::getStmtsUsingVar("a");
			std::unordered_set<int> expected = {3, 4, 7, 8, 9, 10, 11, 12};

			Assert::IsTrue(result == expected);

		}

		//Test for getStmtUsingVar, empty, filled usesStore
		TEST_METHOD(TestGetStmtUsingVar_Empty_FilledUsesStore) {
			std::unordered_set<int> emptyList = {};
			Assert::IsTrue(UsesStore::getStmtsUsingVar("b") == emptyList);
		}

		//Test for getPrintVars, true
		TEST_METHOD(TestGetPrintVars_True) {
			std::unordered_set<std::string> expected = { "p" };
			Assert::IsTrue(UsesStore::getPrintVars() == expected);
		}

		//Test for getVarsUsedInWhile, non_empty
		TEST_METHOD(TestGetVarsUsedInWhile_NonEmpty) {
			std::unordered_set<std::string> expected = { "w", "a", "p" };

			Assert::IsTrue(UsesStore::getVarsUsedInWhile() == expected);
		}

		//Test for getVarsUsedInIf, non empty return
		TEST_METHOD(TestGetVarsUsedInIf_NonEmpty) {
			std::unordered_set<std::string> expected = { "c", "k", "a", "p", "ifs", "w" };
			Assert::IsTrue(UsesStore::getVarsUsedInIf() == expected);
		}


		//Test for getVarsUsedInAssign, non empty return
		TEST_METHOD(TestGetVarsUsedInAssign_NonEmpty) {
			std::unordered_set<std::string> expected = {"p", "a", "ifs", "w"};
			Assert::IsTrue(UsesStore::getVarsUsedInAssign() == expected);
		}

	

		TEST_METHOD(TestIsVarUsedInProc_True) {
			Assert::IsTrue(UsesStore::isVarUsedInProc("testA", "p"));
		}

		TEST_METHOD(TestIsVarUsedInProc_False) {
			Assert::IsFalse(UsesStore::isVarUsedInProc("testD", "a"));
		}

		TEST_METHOD(TestProcUses_True) {
			Assert::IsTrue(UsesStore::procUses("testA"));
		}

		TEST_METHOD(TestProcUses_False) {
			Assert::IsFalse(UsesStore::procUses("testF"));
		}

		TEST_METHOD(TestGetProcUsesMap) {
			std::unordered_map<std::string, std::unordered_set<std::string>> result = UsesStore::getProcUsesMap();
			Assert::IsTrue(result == expectedProcUsesMap);
		}

		TEST_METHOD(TestGetAllUsesProc) {
			std::unordered_set<std::string> result = UsesStore::getAllUsesProcs();
			Assert::IsTrue(result == expectedAllUsesProc);
		}

		TEST_METHOD(TestGetUsedVarsInProc_NonEmpty) {
			std::unordered_set<std::string> result = UsesStore::getUsedVarsInProc("testA");
			std::unordered_set<std::string> expected = { "a", "c", "p", "k", "ifs", "w" };
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(TestGetUsedVarsInProc_Empty) {
			std::unordered_set<std::string> result = UsesStore::getUsedVarsInProc("testF");
			std::unordered_set<std::string> expected = { };
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(TestGetProcUsingVar_NonEmpty) {
			std::unordered_set<std::string> result = UsesStore::getProcsUsingVar("p");
			std::unordered_set<std::string> expected = {"testA", "testB", "testC", "testD" };
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(TestGetProcUsingVar_Empty) {
			std::unordered_set<std::string> result = UsesStore::getProcsUsingVar("e");
			std::unordered_set<std::string> expected = { };
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(TestGetVarsUsedInCall) {
			std::unordered_set<std::string> result = UsesStore::getVarsUsedInCall();
			std::unordered_set<std::string> expected = {"a", "p" };
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(getPrintStmtToVarMap_SUCCESS) {
			Assert::IsTrue(UsesStore::getPrintStmtToVarMap() == expectedPrintStmtToVarMap);
		}

		TEST_METHOD(getVarToPrintStmtMap_SUCCESS) {
			Assert::IsTrue(UsesStore::getVarToPrintStmtMap() == expectedVarToPrintStmtMap);
		}
	};
}