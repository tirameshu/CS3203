#include "stdafx.h"
#include "CppUnitTest.h"
#include "AssignStoreTestData.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{
	TEST_CLASS(AssignStoreTest)
	{
	public:

		TEST_CLASS_INITIALIZE(InitializeStore)
		{
			AssignStore aStore;
			aStore.clear(); // need to clear for unit testing, aStore used by multiple test files
			aStore.addAssignStmt(1);
			aStore.addAssignStmt(2);
			aStore.addAssignStmt(4);
			aStore.addAssignStmt(6);
			aStore.addAssignStmt(7);
			aStore.addAssignStmt(8);
			aStore.addAssignStmt(9);
			aStore.addAssignStmt(12);

			aStore.addAssign(1, "p", "4");
			aStore.addAssign(2, "a", "p2*");
			aStore.addAssign(4, "pattern", "pa*a*pifs*+pw*+");
			aStore.addAssign(6, "p", "p1-");
			aStore.addAssign(7, "ifs", "ifs1-");
			aStore.addAssign(8, "p", "5");
			aStore.addAssign(9, "ifs", "ifs1-");
			aStore.addAssign(12, "pattern", "pa*a*pifs*+pw*+");
		}

		// SETTER METHOD TESTS
		TEST_METHOD(addAssign_DUPLICATE) {
			Assert::IsFalse(AssignStore().addAssign(4, "pattern", "pa*a*pifs*+pw*+"));
		}

		// GETTER METHOD TESTS
		TEST_METHOD(getAssignStmtList_TRUE) {
			Assert::IsTrue(AssignStore().getAssignStmtList() == expectedAssignStmtList);
		}

		TEST_METHOD(getExpressionOfStmt_VALID) {
			Assert::IsTrue(AssignStore().getExpressionOfStmt(4) == expectedStmtAssignMap.at(4).expression);
		}

		TEST_METHOD(getExpressionOfStmt_INVALID) {
			Assert::IsTrue(AssignStore().getExpressionOfStmt(3) == "");
		}

		TEST_METHOD(getVarExpStmtList_TRUE) {
			Assert::IsTrue(AssignStore().getVarExpStmtList() == expectedVarExpStmtList);
		}

		TEST_METHOD(getVarToExpMap_TRUE) {
			Assert::IsTrue(AssignStore().getVarToExpMap() == expectedVarToExpMap);
		}

		TEST_METHOD(getVarAssignStmtList_TRUE) {
			Assert::IsTrue(AssignStore().getVarAssignStmtList() == expectedVarAssignStmtList);
		}

		TEST_METHOD(getExpAssignStmtList_TRUE) {
			Assert::IsTrue(AssignStore().getExpAssignStmtList() == expectedExpAssignStmtList);
		}
	};
}

