#include "stdafx.h"
#include "CppUnitTest.h"
#include "../source/ModifiesStore.h"
#include "ModifiesStoreTestData.h"
#include "cassert"
#include <string>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{
	TEST_CLASS(TestModifiesStore)
	{
	public:
		
		TEST_CLASS_INITIALIZE(initializeStore) {
			ModifiesStore::clear();

			ModifiesStore::addModifies(1, 3, "p");
			ModifiesStore::addModifies(2, 1, "a");
			ModifiesStore::addModifies(4, 1, "pattern");
			ModifiesStore::addModifies(6, 1, "w");
			ModifiesStore::addModifies(9, 1, "ifs");
			ModifiesStore::addModifies(12, 1, "a");
			ModifiesStore::addModifies(14, 1, "p");
			ModifiesStore::addModifies(15, 1, "x");



			ModifiesStore::addModifiesToContainer(3, 4, { "pattern", "w", "ifs", "a", "p", "x" });
			ModifiesStore::addModifiesToContainer(5, 5, { "w" });
			ModifiesStore::addModifiesToContainer(7, 7, { "ifs", "a", "p", "x" });
			ModifiesStore::addModifiesToContainer(8, 7, { "a", "p", "x" });
			ModifiesStore::addModifiesToContainer(10, 5, { "a", "p", "x" });
			ModifiesStore::addModifiesToContainer(11, 7, { "a", "p", "x" });
			ModifiesStore::addModifiesToContainer(13, 7, { "p", "x" });

			ModifiesStore::addModifiedVarToProc("testA", { "p", "a", "pattern", "w", "ifs", "x" });
			ModifiesStore::addModifiedVarToProc("testB", { "ifs", "a", "p", "x" });
			ModifiesStore::addModifiedVarToProc("testC", { "a", "p", "x" });
			ModifiesStore::addModifiedVarToProc("testD", { "p", "x" });

		}
		//Test AddModifies, readstmt
		//TEST_METHOD(TestAddModifies_ReadStmt)
		//{
		//	int stmtNo = 1;
		//	int stmtType = 3;
		//	std::string var = "p";

		//	Assert::IsTrue(ModifiesStore::addModifies(stmtNo, stmtType, var));
		//}

		////Test AddModifiesToContainer, if stmt
		//TEST_METHOD(TestAddModifiesToContainer_IfStmt) {
		//	int stmtNo = 1;
		//	int stmtType = 4;
		//	std::unordered_set<std::string> varList = { "pattern", "w", "ifs", "a", "p", "x" };

		//	ModifiesStore::addModifiesToContainer(stmtNo, stmtType, varList);

		//	Assert::IsTrue(ModifiesStore::hasIfModifies());
		//}

		////Test AddModifiesToContainer, while stmt
		//TEST_METHOD(TestAddModifiesToContainer_WhileStmt) {
		//	ModifiesStore ModifiesStore:: = ModifiesStore();
		//	int stmtNo = 1;
		//	int stmtType = 5;
		//	std::unordered_set<std::string> varList = { "a", "b", "c" };

		//	ModifiesStore::.addModifiesToContainer(stmtNo, stmtType, varList);

		//	Assert::IsTrue(ModifiesStore::hasWhileModifies());
		//}

		//Test isVarModifiesInStmt, true
		TEST_METHOD(TestIsVarModifiedInStmt_True) {
			Assert::IsTrue(ModifiesStore::isVarModifiedInStmt("a", 2));
		}

		//Test isVarModifiedInStmt, False, non-empty ModifiesStore
		TEST_METHOD(TestIsVarModifiedInStmt_False) {
			Assert::IsFalse(ModifiesStore::isVarModifiedInStmt("b", 2));
		}

		//Test isVarModified, true
		TEST_METHOD(TestIsVarModified_True) {
			Assert::IsTrue(ModifiesStore::isVarModified("x"));
		}

		//Test isVarModified, false
		TEST_METHOD(TestIsVarModified_False) {
			Assert::IsFalse(ModifiesStore::isVarModified("y"));
		}


		//Test isStmtModifies, true
		TEST_METHOD(TestIsStmtModifies_True) {
			Assert::IsTrue(ModifiesStore::isStmtModifies(3));
		}

		//Test isStmtModifies, false
		TEST_METHOD(TestIsStmtModifies_False) {
			Assert::IsFalse(ModifiesStore::isStmtModifies(16));
		}

		//Test hasIfModifies, true
		TEST_METHOD(TestHasIfModifies_True) {
			Assert::IsTrue(ModifiesStore::hasIfModifies());

		}

		//Test hasWhileModifies, true
		TEST_METHOD(TestHasWhileModifies_True) {
			Assert::IsTrue(ModifiesStore::hasWhileModifies());
		}

		//Test hasModifies, true
		TEST_METHOD(TestHasModifies_True) {
			Assert::IsTrue(ModifiesStore::hasModifies());
		}

		//Test getStmtModifiesMap, non empty
		TEST_METHOD(TestGetStmtModifiesMap_NonEmpty) {
			Assert::IsTrue(ModifiesStore::getStmtModifiesMap() == expectedStmtModifiesMap);
		}

		//Test getVarModifiedMap, non empty
		TEST_METHOD(TestGetVarModifiedMap_NonEmpty) {
			std::unordered_map<std::string, std::unordered_set<int>>result = ModifiesStore::getVarModifiedMap();
			Assert::IsTrue(ModifiesStore::getVarModifiedMap() == expectedVarModifiedMap);
		}

		//Test getAllModifiedVars, non empty
		TEST_METHOD(TestGetAllModifiedVars_NonEmpty) {
			Assert::IsTrue(ModifiesStore::getAllModifiedVars() == expectedAllModifiedVarList);
		}

		//Test getModifyingStmtList non empty
		TEST_METHOD(TestGetModifyingStmtList_NonEmpty) {
			Assert::IsTrue(ModifiesStore::getModifyingStmtList() == expectedModifyingStmtList);
		}

		//Test getReadStmtList Non_empty
		TEST_METHOD(TestGetReadStmtList_NonEmpty) {
			Assert::IsTrue(ModifiesStore::getReadStmtList() == expectedReadStmtList);
		}

		//Test getModifyingIfStmtList non empty
		TEST_METHOD(TestGetModifyingIfStmtList_NonEmpty) {
			Assert::IsTrue(ModifiesStore::getModifyingIfStmtList() == expectedModifyingIfStmtList);
		}

		//Test getModifyingWhileStmtsList, non empty
		TEST_METHOD(TestGetModifyingWhileStmtList_NonEmpty) {
			Assert::IsTrue(ModifiesStore::getModifyingWhileStmtsList() == expectedModifyingWhileStmtList);
		}

		//Test getStmtsModifyingVar, non empty
		TEST_METHOD(TestGetStmtsModifyingVar_NonEmpty) {
			std::unordered_set<int> expected = { 2, 3, 7, 8, 10, 11, 12 };
			Assert::IsTrue(ModifiesStore::getStmtsModifyingVar("a") == expected);
		}

		//Test getModifiedVarsInStmt, non empty
		TEST_METHOD(TestGetModifiedVarsInStmt_NonEmpty) {
			std::unordered_set<std::string> expected = { "pattern", "w", "ifs", "a", "p", "x" };
			Assert::IsTrue(ModifiesStore::getModifiedVarsInStmt(3) == expected);
		}

		//Test getReadVars, non empty
		TEST_METHOD(TestGetReadVars_NonEmpty) {
			std::unordered_set<std::string> expected = { "p" };
			Assert::IsTrue(ModifiesStore::getReadVars() == expected);
		}

		//Test getVarsModifiedInWhile, non empty return
		TEST_METHOD(TestGetVarsModifiedInWhile_NonEmpty) {
			std::unordered_set<std::string> resultList = { "w", "a", "p", "x" };

			Assert::IsTrue(ModifiesStore::getVarsModifiedInWhile() == resultList);
		}

		//Test getVarsModifiedInIf, non empty return
		TEST_METHOD(TestGetVarsModifiedInIf_NonEmpty) {
			std::unordered_set<std::string> resultList = { "pattern", "w", "ifs", "a", "p", "x" };

			Assert::IsTrue(ModifiesStore::getVarsModifiedInIf() == resultList);
		}

		//modifiesP tests

		TEST_METHOD(TestIsVarModifiedInProc_True) {
			Assert::IsTrue(ModifiesStore::isVarModifiedInProc("a", "testA"));
		}

		TEST_METHOD(TestIsVarModifiedInProc_False_ProcExists) {
			Assert::IsFalse(ModifiesStore::isVarModifiedInProc("a", "testD"));
		}

		TEST_METHOD(TestIsVarModifiedInProc_False_ProcNotExists) {
			Assert::IsFalse(ModifiesStore::isVarModifiedInProc("a", "testF"));
		}

		TEST_METHOD(TestProcHasModifies_True) {
			Assert::IsTrue(ModifiesStore::procHasModifies("testA"));
		}

		TEST_METHOD(TestProcHasModifies_False) {
			Assert::IsFalse(ModifiesStore::procHasModifies("testF"));
		}

		TEST_METHOD(TestHasModifiesP_True) {
			Assert::IsTrue(ModifiesStore::hasModifiesP());
		}

		TEST_METHOD(TestGetModifyingProcs) {
			std::unordered_set<std::string> result = ModifiesStore::getModifiyingProcs();
			Assert::IsTrue(result == expectedAllModifiyingProcs);
		}

		TEST_METHOD(TestGetModifiedVarsInProc_NonEmpty) {
			std::unordered_set<std::string> result = ModifiesStore::getModifiedVarsInProc("testA");
			std::unordered_set<std::string> expected = { "p", "a", "pattern", "w", "ifs", "x" };

			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(TestGetModifiedVarsInProc_Empty) {
			std::unordered_set<std::string> result = ModifiesStore::getModifiedVarsInProc("testF");
			std::unordered_set<std::string> expected = {};

			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(TestGetProcModifyingVar_NonEmpty) {
			std::unordered_set<std::string> result = ModifiesStore::getProcModifyingVar("a");
			std::unordered_set<std::string> expected = { "testA", "testB", "testC" };

			Assert::IsTrue(result == expected);
		}
		
		TEST_METHOD(TestGetProcModifyingVar_Empty) {
			std::unordered_set<std::string> result = ModifiesStore::getProcModifyingVar("z");
			std::unordered_set<std::string> expected = {};

			Assert::IsTrue(result == expected);
		}
		
		TEST_METHOD(getReadStmtToVarMap_SUCCESS) {
			Assert::IsTrue(ModifiesStore::getReadStmtToVarMap() == expectedReadStmtToVarMap);
		}

		TEST_METHOD(getVarToReadStmtMap_SUCCESS) {
			Assert::IsTrue(ModifiesStore::getVarToReadStmtMap() == expectedVarToReadStmtMap);
		}
	};


}