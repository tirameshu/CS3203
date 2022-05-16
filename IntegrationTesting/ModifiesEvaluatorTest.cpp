#include "stdafx.h"
#include "CppUnitTest.h"
#include "ModifiesEvaluatorTestData.h"
#include "assert.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace IntegrationTesting
{
	TEST_CLASS(ModifiesEvaluatorTest)
	{
	public:
		TEST_CLASS_INITIALIZE(Initialization) {

			PKB pkb;
			pkb.clear();
			//modifies stmts: 1, 3, 4, 5
			pkb.addProcedure("testM");
			pkb.addProcedure("testM2");
			pkb.addProcedure("testM3");
			pkb.addStatement("testM", Statement(1, 1));
			pkb.addStatement("testM", Statement(1, 2));
			pkb.addStatement("testM", Statement(4, 3));
			pkb.addStatement("testM", Statement(1, 4));
			pkb.addStatement("testM", Statement(5, 5));
			pkb.addStatement("testM", Statement(3, 6));
			pkb.addStatement("testM", Statement(4, 7));
			pkb.addStatement("testM", Statement(5, 8));
			pkb.addStatement("testM", Statement(1, 9));
			pkb.addStatement("testM", Statement(3, 10));
			pkb.addStatement("testM", Statement(1, 11));
			pkb.addStatement("testM", Statement(1, 12));
			pkb.addStatement("testM", Statement(2, 13));
			pkb.addStatement("testM", Statement(4, 14));
			pkb.addStatement("testM", Statement(1, 15));
			pkb.addStatement("testM", Statement(1, 16));
			pkb.addStatement("testM", Statement(CALL, 17));
			pkb.addStatement("testM2", Statement(1, 18));
			pkb.addStatement("testM2", Statement(CALL, 19));
			pkb.addStatement("testM3", Statement(PRINT, 20));

			pkb.addModifies(1, 1, "p");
			pkb.addModifies(2, 1, "a");
			pkb.addModifies(4, 1, "pattern");
			pkb.addModifies(9, 1, "a");
			pkb.addModifies(11, 1, "ifs");
			pkb.addModifies(12, 1, "a");
			pkb.addModifies(15, 1, "q");
			pkb.addModifies(16, 1, "a");
			pkb.addModifies(6, 3, "r");
			pkb.addModifies(10, 3, "b");
			pkb.addModifies(18, ASSIGN, "a");
		
			pkb.addModifiesToContainer(3, 4, std::unordered_set<std::string> {"pattern"});
			pkb.addModifiesToContainer(7, 4, std::unordered_set<std::string> {"a", "b", "ifs"});
			pkb.addModifiesToContainer(8, 5, std::unordered_set<std::string> {"a", "b", "ifs"});
			pkb.addModifiesToContainer(14, 4, std::unordered_set<std::string> {"q", "a"});
			pkb.addModifiesToContainer(17, CALL, std::unordered_set<std::string> {"a"});

			pkb.addModifiedVarToProc("testM", allModifiedVarList);
			pkb.addModifiedVarToProc("testM2", { "a" });

			pkb.addAssign(1, "p", "4");
			pkb.addAssign(2, "a", "p2*");
			pkb.addAssign(4, "pattern", "pa*a*pifs*+pw*+");
			pkb.addAssign(9, "a", "a1-");
			pkb.addAssign(11, "ifs", "ifs1-");
			pkb.addAssign(12, "a", "10");
			pkb.addAssign(15, "q", "pa-");
			pkb.addAssign(16, "a", "10");
			pkb.addAssign(18, "7", "2");
		}

		//Test checkModifiesClauseHolds, true
		TEST_METHOD(modifiesAnyAny_ClauseHoldsTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(ModifiesAnyAny);

			Assert::IsTrue(result.empty());
		}

		TEST_METHOD(modifiesStmtAny_ResultSynonymInClause) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(ModifiesStmtAnyInClause);
			Assert::IsTrue(result == modifyingStmtList);
		}

		TEST_METHOD(modifiesStmtAny_ResultSynonymNotInClause) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(ModifiesStmtAnyNotInClause);
			Assert::IsTrue(result == allProgStmt);
		}

		TEST_METHOD(modifiesAnyVars_ResultsSynonymInClause) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(ModifiesAnyVarsInClause);
			Assert::IsTrue(result.empty());
		}

		TEST_METHOD(modifiesAnyVars_ResultSynonymNotInClause) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(ModifiesAnyVarsNotInClause);
			Assert::IsTrue(result.empty());
		}

		TEST_METHOD(modifiesStmtVar_ResultSynonymInClause) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(ModifiesStmtVarInClause);
			Assert::IsTrue(result == modifyingStmtList);
		}

		TEST_METHOD(modifiesStmtVar_ResultSynonymNotInClause) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(ModifiesStmtVarNotInClause);
			Assert::IsTrue(result == allProgStmt);
		}

		TEST_METHOD(modifiesStmtStr_ResultSynonymInClause) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(ModifiesStmtStrInClause);
			std::unordered_set<std::string> expected = {"2", "9", "12", "16", "7", "8", "14", "17", "18"};
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(modifiesStmtStr_ResultSynonymNotInClause) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(ModifiesStmtVarNotInClause);
			Assert::IsTrue(result == allProgStmt);
		}

		TEST_METHOD(modifiesAnyStr_True) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(ModifiesAnyStrTrue);
			Assert::IsTrue(result.empty());
		}

		TEST_METHOD(modifiesAnyStr_False) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(ModifiesAnyStrFalse);
			Assert::IsTrue(result.empty());
		}

		TEST_METHOD(modifiesIntAny_True) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(ModifiesIntAny_True);
			Assert::IsTrue(result == allProgStmt);
		}

		TEST_METHOD(modifiesIntAny_False) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(ModifiesIntAny_False);
			Assert::IsTrue(result.empty());
		}

		TEST_METHOD(modifiesIntVar_ResultSynonymInClause) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(ModifiesIntVarInClause);
			std::unordered_set<std::string> expected = { "p" };
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(modifiesIntVar_ResultSynonymNotInClause) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(ModifiesIntVarNotInClause);
			Assert::IsTrue(result == allProgStmt);
		}

		TEST_METHOD(modifiesIntString_True) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(ModifiesIntString_True);
			Assert::IsTrue(result == allProgStmt);
		}

		TEST_METHOD(modifiesIntString_False) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(ModifiesIntString_False);
			Assert::IsTrue(result.empty());
		}

		TEST_METHOD(modifiesIntString_False1) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(ModifiesIntString_False1);
			Assert::IsTrue(result.empty());
		}

		TEST_METHOD(modifiesIntString_False2) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(ModifiesIntString_False2);
			Assert::IsTrue(result.empty());
		}

		TEST_METHOD(modifiesIfAny_ResultSynonymInClause) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(ModifiesIfAnyInClause);
			Assert::IsTrue(result == ifModifyingList);
		}

		TEST_METHOD(modifiesIfAny_ResultSynonymNotInClause) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(ModifiesIfAnyNotInClause);
			Assert::IsTrue(result == allProgStmt);
		}

		TEST_METHOD(modifiesWhileAny_ResultSynonymInClause) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(ModifiesWhileAnyInClause);
			Assert::IsTrue(result == whileModifyingList);
		}

		TEST_METHOD(modifiesWhileAny_ResultSynonymNotInClause) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(ModifiesWhileAnyNotInClause);
			Assert::IsTrue(result == allProgStmt);
		}

		TEST_METHOD(modifiesReadAny_ResultSynonymInClause) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(ModifiesReadAnyInClause);
			Assert::IsTrue(result == readModifyingList);
		}

		TEST_METHOD(modifiesReadAny_ResultSynonymNotInClause) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(ModifiesReadAnyNotInClause);
			Assert::IsTrue(result == allProgStmt);
		}

		TEST_METHOD(modifiesIfVar_ResultSynonymInClause) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(ModifiesIfVarInClause);
			Assert::IsTrue(result == ifModifyingList);
		}

		TEST_METHOD(modifiesIfVar_ResultSynonymNotInClause) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(ModifiesIfVarNotInClause);
			Assert::IsTrue(result == allProgStmt);
		}
		
		TEST_METHOD(modifiesWhileVar_ResultSynonymInClause) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(ModifiesWhileVarInClause);
			Assert::IsTrue(result == whileModifyingList);
		}

		TEST_METHOD(modifiesWhileVar_ResultSynonymNotInClause) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(ModifiesWhileVarNotInClause);
			Assert::IsTrue(result == allProgStmt);
		}

		TEST_METHOD(modifiesReadVar_ResultSynonymInClause) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(ModifiesReadVarInClause);
			Assert::IsTrue(result == readModifyingList);
		}

		TEST_METHOD(modifiesReadVar_ResultSynonymNotInClause) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(ModifiesReadVarNotInClause);
			Assert::IsTrue(result == allProgStmt);
		}

		TEST_METHOD(modifiesIfStr_ResultSynonymInClause) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(ModifiesIfStrInClause);
			std::unordered_set<std::string> expected = {"7", "14"};
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(modifiesIfStr_ResultSynonymNotInClause) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(ModifiesIfStrNotInClause);
			Assert::IsTrue(result == allProgStmt);
		}

		TEST_METHOD(modifiesWhileString_ResultSynonymInClause) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(ModifiesWhileStrInClause);
			std::unordered_set<std::string> expected = { "8" };
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(modifiesWhileStr_ResultSynonymNotInClause) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(ModifiesWhileStrNotInClause);
			Assert::IsTrue(result == allProgStmt);
		}

		TEST_METHOD(modifiesReadStr_ResultSynonymInClause) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(ModifiesReadStrInClause);
			std::unordered_set<std::string> expected = {"6"};

			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(modifiesReadStr_ResultSynonymNotInClause) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(ModifiesReadStrNotInClause);
			Assert::IsTrue(result == allProgStmt);
		}

		// ITERATION 1 BUG REPORT ADDITIONS
		TEST_METHOD(WhileSynonymNotInClause_Return_All_WhileStatements) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(WhileSynonymNotInClause);
			Assert::IsTrue(result == std::unordered_set<std::string>{"5", "8"});
		}

		TEST_METHOD(WhileSynonymNotInClauseVarNameDoesNotExist_Return_Empty) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(WhileSynonymNotInClauseVarNameDoesNotExist);
			Assert::IsTrue(result == std::unordered_set<std::string>{});
		}

		TEST_METHOD(AssignSynonymInClause_Return_All_AssignStatements) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(AssignSynonymInClause);
			Assert::IsTrue(result == std::unordered_set<std::string>{"2", "9", "12", "16"});
		}

		TEST_METHOD(AssignSynonymInClauseVarNameDoesNotExist_Return_Empty) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(AssignSynonymInClauseVarNameDoesNotExist);
			Assert::IsTrue(result == std::unordered_set<std::string>{});
		}

		TEST_METHOD(CallSynonymInClause_ReturnModifyinCallStmt) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(CallSynonymInClause);

			Assert::IsTrue(result == callModifyingStmtList);
		}

		TEST_METHOD(CallSynonymNotInClause_ReturnAllCallStmt) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(CallSynonymNotInClause);
			std::unordered_set<std::string> expected = { "17", "19" };
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(CallSynonymStringInClause_ReturnCallStmtModifyingA) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(CallSynonymStringInClause);
			Assert::IsTrue(result == callModifyingStmtList);
		}

		TEST_METHOD(CallSynonymStringInClause2_ReturnNothing) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(CallSynonymStringInClause2);
			Assert::IsTrue(result == std::unordered_set<std::string>{});
		}

		TEST_METHOD(CallSynonymAnyInClause_ReturnCallStmtModifying) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(CallSynonymAnyInClause);
			Assert::IsTrue(result == callModifyingStmtList);
		}

		TEST_METHOD(CallSynonymAnyNotInClause_ReturnAllCallStmts) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(CallSynonymAnyNotInClause);
			std::unordered_set<std::string> expected = { "17", "19" };
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(ProgLineInClause_ReturnProgLineModifyingA) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(ProgLineInClause);
			std::unordered_set<std::string> expected = { "2", "9", "12", "16", "7", "8", "14", "17", "18" };
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(ProgLineNotInClause_ReturnAllProgLine) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(ProgLineNotInClause);
			Assert::IsTrue(result == allProgStmt);
		}

		/*
		ModfifiesP
		*/

		TEST_METHOD(ProcVarInClause_ReturnProcModifying) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(ProcVarInClause);
			std::unordered_set<std::string> expected = { "testM", "testM2" };
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(ProcVarNotInClause_ReturnAllProc) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(ProcVarNotInClause);
			std::unordered_set<std::string> expected = { "testM", "testM2", "testM3" };
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(ProcStrInClause_ReturnProcModifyingA) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(ProcStrInClause);
			std::unordered_set<std::string> expected = { "testM", "testM2"};
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(ProcStrNotInClause_ReturnAllProc) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(ProcStrNotInClause);
			std::unordered_set<std::string> expected = { "testM", "testM2", "testM3" };
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(ProcAnyInClause_ReturnProcModifyingA) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(ProcStrInClause);
			std::unordered_set<std::string> expected = { "testM", "testM2" };
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(ProcAnyNotInClause_ReturnAllProc) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(ProcStrNotInClause);
			std::unordered_set<std::string> expected = { "testM", "testM2", "testM3" };
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(StrVarInClauseTrue_ReturnAllProc) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(StrVarInClauseTrue);
			std::unordered_set<std::string> expected = { "testM", "testM2", "testM3" };
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(StrVarInClauseFalse_ReturnNothing) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(StrVarInClauseFalse);
			std::unordered_set<std::string> expected = {};
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(StrStrInClauseTrue_ReturnAllProc) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(StrStrInClauseTrue);
			std::unordered_set<std::string> expected = { "testM", "testM2", "testM3" };
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(StrStrInClauseFalse_ReturnNothing) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(StrStrInClauseFalse);
			std::unordered_set<std::string> expected = {};
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(StrAnyInClauseTrue_ReturnAllProc) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(StrStrInClauseTrue);
			std::unordered_set<std::string> expected = { "testM", "testM2", "testM3" };
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(StrAnyInClauseFalse_ReturnNothing) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(StrStrInClauseFalse);
			std::unordered_set<std::string> expected = {};
			Assert::IsTrue(result == expected);
		}
	};

}