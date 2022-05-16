#include "stdafx.h"
#include "CppUnitTest.h"
#include "PKBTestData.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{
	TEST_CLASS(PKBTest)
	{
	public:

		TEST_CLASS_INITIALIZE(InitializeStore)
		{
			PKB pkb;
			pkb.clear();
			pkb.addProcedure("testA");
			pkb.addProcedure("testB");
			pkb.addProcedure("testC");
			pkb.addVariable("p");
			pkb.addVariable("a");
			pkb.addVariable("c");
			pkb.addVariable("k");
			pkb.addVariable("pattern");
			pkb.addVariable("ifs");
			pkb.addVariable("w");
			pkb.addConstant(1, "4");
			pkb.addConstant(2, "2");
			pkb.addConstant(5, "0");
			pkb.addConstant(7, "1");
			pkb.addConstant(10, "1");
			pkb.addConstant(11, "5");
			pkb.addStatement("testA", { ASSIGN, 1 });
			pkb.addStatement("testA", { ASSIGN, 2 });
			pkb.addStatement("testA", { IF, 3 });
			pkb.addStatement("testA", { ASSIGN, 4 });
			pkb.addStatement("testA", { WHILE, 5 });
			pkb.addStatement("testA", { READ, 6 });
			pkb.addStatement("testA", { ASSIGN, 7 });
			pkb.addStatement("testA", { PRINT, 8 });
			pkb.addStatement("testA", { CALL, 9 });
			pkb.addStatement("testB", { ASSIGN, 10 });
			pkb.addStatement("testB", { WHILE, 11 });
			pkb.addStatement("testB", { PRINT, 12 });
			pkb.addStatement("testB", { CALL, 13 });
			pkb.addStatement("testC", { ASSIGN, 14 });
			pkb.addIfContainerStmtList(3, 4);
			pkb.addIfContainerStmtList(3, 5);
			pkb.addIfContainerStmtList(3, 7);
			pkb.addIfContainerStmtList(3, 8);
			pkb.addWhileContainerStmtList(5, 6);
			pkb.addWhileContainerStmtList(11, 12);
			//pkb.addCallProcedure(9, "testB");
			//pkb.addCallProcedure(13, "testC");
		}

		// SETTER METHOD TESTS
		TEST_METHOD(addStatement_DUPLICATE) {
			Assert::IsFalse(PKB().addStatement("testA", { 1, 1 }));
		}

		TEST_METHOD(addStatement_INVALID_STMT_TYPE) {
			Assert::IsFalse(PKB().addStatement("testA", { 6, 20 }));
		}

		// GETTER METHOD TESTS
		TEST_METHOD(getTypeOfStmt_TRUE) {
			Assert::AreEqual(PKB().getTypeOfStmt(11), 5);
		}

		TEST_METHOD(getProgramStmtMap_TRUE) {
			Assert::IsTrue(PKB().getProgramStmtMap() == expectedProgramStmtMap);
		}

		TEST_METHOD(getProcedureStmtMap_TRUE) {
			Assert::IsTrue(PKB().getProcedureStmtMap() == expectedProcedureStmtMap);
		}

		TEST_METHOD(getStmtProcedureMap_TRUE) {
			Assert::IsTrue(PKB().getStmtProcedureMap() == expectedStmtProcedureMap);
		}

		TEST_METHOD(getIfStmtMap_TRUE) {
			Assert::IsTrue(PKB().getIfStmtMap() == expectedIfStmtMap);
		}

		TEST_METHOD(getWhileStmtMap_TRUE) {
			Assert::IsTrue(PKB().getWhileStmtMap() == expectedWhileStmtMap);
		}

		TEST_METHOD(getConstantStmtMap_TRUE) {
			Assert::IsTrue(PKB().getConstantStmtMap() == expectedConstantStmtMap);
		}

		TEST_METHOD(getStmtConstantMap_TRUE) {
			Assert::IsTrue(PKB().getStmtConstantMap() == expectedStmtConstantMap);
		}

		//TEST_METHOD(getCallProcedureMap_TRUE) {
			//Assert::IsTrue(PKB().getCallProcedureMap() == expectedCallProcedureMap);
		//}

		TEST_METHOD(getProcedureList_TRUE) {
			Assert::IsTrue(PKB().getProcedureList() == expectedProcedureList);
		}

		TEST_METHOD(getVariableList_TRUE) {
			Assert::IsTrue(PKB().getVariableList() == expectedVariableList);
		}

		TEST_METHOD(getConstantList_TRUE) {
			Assert::IsTrue(PKB().getConstantList() == expectedConstantList);
		}

		TEST_METHOD(getStmtList_TRUE) {
			Assert::IsTrue(PKB().getStmtList() == expectedStatementList);
		}

		TEST_METHOD(getAssignStmtList_TRUE) {
			Assert::IsTrue(PKB().getAssignStmtList() == expectedPKBAssignStmtList);
		}

		TEST_METHOD(getPrintStmtList_TRUE) {
			Assert::IsTrue(PKB().getPrintStmtList() == expectedPKBPrintStmtList);
		}

		TEST_METHOD(getReadStmtList_TRUE) {
			Assert::IsTrue(PKB().getReadStmtList() == expectedPKBReadStmtList);
		}

		TEST_METHOD(getIfStmtList_TRUE) {
			Assert::IsTrue(PKB().getIfStmtList() == expectedIfStmtList);
		}

		TEST_METHOD(getWhileStmtList_TRUE) {
			Assert::IsTrue(PKB().getWhileStmtList() == expectedWhileStmtList);
		}

		TEST_METHOD(getCallStmtList_TRUE) {
			Assert::IsTrue(PKB().getCallStmtList() == expectedCallStmtList);
		}
	};
}

