#include "stdafx.h"
#include "CppUnitTest.h"
#include "UsesEvaluatorTestData.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace IntegrationTesting
{
	TEST_CLASS(UsesEvaluatorTest)
	{
	public:

		TEST_CLASS_INITIALIZE(Initialization) {
			PKB pkb;
			pkb.clear();
			pkb.addProcedure("testA");
			pkb.addProcedure("testB");
			pkb.addProcedure("testC");
			pkb.addStatement("testA", { 1, 1 });
			pkb.addStatement("testA", { 1, 2 });
			pkb.addStatement("testA", { 4, 3 });
			pkb.addStatement("testA", { 1, 4 });
			pkb.addStatement("testA", { 5, 5 });
			pkb.addStatement("testA", { 3, 6 });
			pkb.addStatement("testA", { 4, 7 });
			pkb.addStatement("testA", { 2, 8 });
			pkb.addStatement("testA", { 1, 9 });
			pkb.addStatement("testA", { 3, 10 });
			pkb.addStatement("testA", { 1, 11 });
			pkb.addStatement("testA", { 1, 12 });
			pkb.addStatement("testA", { 3, 13 });
			pkb.addStatement("testA", { 2, 14 });
			pkb.addStatement("testA", { 2, 15 });
			pkb.addStatement("testA", { 1, 16 });
			pkb.addStatement("testA", { CALL, 17 });
			pkb.addStatement("testB", { PRINT, 18 });
			pkb.addStatement("testB", { CALL, 19 });
			pkb.addStatement("testC", { READ, 20 });
			pkb.addVariable("p");
			pkb.addVariable("a");
			pkb.addVariable("c");
			pkb.addVariable("k");
			pkb.addVariable("ifs");
			pkb.addVariable("w");
			pkb.addVariable("pattern");
			pkb.addUses(2, 1, { "p" });
			pkb.addUses(3, 4, { "c", "k" });
			pkb.addUses(4, 1, { "p", "a", "ifs", "w" });
			pkb.addUses(5, 5, { "w" });
			pkb.addUses(7, 4, { "a" });
			pkb.addUses(8, 2, { "a" });
			pkb.addUses(9, 1, { "a" });
			pkb.addUses(11, 1, { "ifs" });
			pkb.addUses(14, 2, { "a" });
			pkb.addUses(15, 2, { "p" });
			pkb.addUses(18, PRINT, { "a" });
			pkb.addUsesToContainer(3, 4, { "c", "k", "p", "a", "ifs", "w" });
			pkb.addUsesToContainer(7, 4, { "a" });
			pkb.addUsesToContainer(5, 5, { "w", "a", "ifs" });
			pkb.addUsesToContainer(17, CALL, {"a"});
			pkb.addProcUses("testA", usedVar);
			pkb.addProcUses("testB", { "a" });
		}

		TEST_METHOD(usesAnyAny_ClauseHoldsTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(UsesAnyAny);
			Assert::IsTrue(result.empty());
		}

		TEST_METHOD(usesAnyVariable_ClauseHoldsTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(UsesAnyVariable);
			Assert::IsTrue(result.empty());
		}

		TEST_METHOD(usesStmtAny_ResultSynonymInClause_ClauseHoldsTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(UsesStmtAny1);
			Assert::IsTrue(result == usesStmt);
		}

		TEST_METHOD(usesStmtAny_ResultSynonymNotInClause_ClauseHoldsTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(UsesStmtAny2);
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(usesStmtVariable_SelectS_ResultSynonymInClause) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(UsesStmtVariableS1);
			Assert::IsTrue(result == usesStmt);
		}

		TEST_METHOD(usesStmtVariable_SelectS_ResultSynonymNotInClause) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(UsesStmtVariableS2);
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(usesStmtVariable_SelectV_ResultSynonymInClause) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(UsesStmtVariableV1);
			Assert::IsTrue(result == usedVar);
		}

		TEST_METHOD(usesStmtVariable_SelectV_ResultSynonymNotInClause) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(UsesStmtVariableV2);
			Assert::IsTrue(result == allVar);
		}

		TEST_METHOD(usesAnyString_ClauseHoldsTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(UsesAnyString);
			Assert::IsTrue(result.empty());
		}

		TEST_METHOD(usesAnyString_ClauseNotTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(InvalidUsesAnyString);
			Assert::IsTrue(result.empty());
		}

		TEST_METHOD(usesStmtString_ResultSynonymInClause_ClauseHoldsTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(UsesStmtStringS);
			std::unordered_set<std::string> expect = {"3", "4", "5", "11" };
			Assert::IsTrue(result == expect);
		}

		TEST_METHOD(usesStmtString_ResultSynonymNotInClause_ClauseHoldsTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(InvalidUsesStmtStringS);
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(usesStmtString_ResultSynonymInClause_ClauseNotTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(UsesStmtStringV);
			Assert::IsTrue(result.empty());
		}

		TEST_METHOD(usesStmtString_ResultSynonymNotInClause_ClauseNotTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(InvalidUsesStmtStringV);
			Assert::IsTrue(result.empty());
		}

		TEST_METHOD(usesIntAny_ClauseHoldsTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(UsesIntAny);
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(usesIntAny_ClauseNotTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(InvalidUsesIntAny);
			Assert::IsTrue(result.empty());
		}

		TEST_METHOD(usesIntVariable_SelectS_ClauseHoldsTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(UsesIntVariableS);
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(usesIntVariable_SelectS_ClauseNotTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(InvalidUsesIntVariableS);
			Assert::IsTrue(result.empty());
		}

		TEST_METHOD(usesIntVariable_SelectV_ResultSynonymInClause_ClauseHoldsTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(UsesIntVariableV);
			std::unordered_set<std::string> expect = { "c", "k", "a", "p", "ifs", "w" };
			Assert::IsTrue(result == expect);
		}

		TEST_METHOD(usesIntVariable_SelectV_ResultSynonymNotInClause_ClauseHoldsTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(InvalidUsesIntVariableV1);
			Assert::IsTrue(result == allVar);
		}

		TEST_METHOD(usesIntVariable_SelectV_ResultSynonymInClause_ClauseNotTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(InvalidUsesIntVariableV2);
			Assert::IsTrue(result.empty());
		}

		TEST_METHOD(usesIntString_ClauseHoldsTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(UsesIntString);
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(usesIntString_ClauseNotTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(InvalidUsesIntString);
			Assert::IsTrue(result.empty());
		}

		TEST_METHOD(usesIntString1_ClauseNotTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(InvalidUsesIntString1);
			Assert::IsTrue(result.empty());
		}

		TEST_METHOD(usesIntString2_ClauseNotTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(InvalidUsesIntString2);
			Assert::IsTrue(result.empty());
		}

		TEST_METHOD(usesAssignAny_ClauseHoldsTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(UsesAssignAny);
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(usesPrintAny_ClauseHoldsTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(UsesPrintAny);
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(usesIfAny_ClauseHoldsTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(UsesIfAny);
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(usesWhileAny_ClauseHoldsTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(UsesWhileAny);
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(usesAssignVariable_SelectS_ClauseHoldsTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(UsesAssignVariableS);
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(usesAssignVariable_SelectV_ClauseHoldsTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(UsesAssignVariableV);
			std::unordered_set<std::string> expect = { "a", "p", "ifs", "w" };
			Assert::IsTrue(result == expect);
		}

		TEST_METHOD(usesPrintVariable_SelectS_ClauseHoldsTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(UsesPrintVariableS);
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(usesPrintVariable_SelectV_ClauseHoldsTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(UsesPrintVariableV);
			std::unordered_set<std::string> expect = { "a", "p" };
			Assert::IsTrue(result == expect);
		}

		TEST_METHOD(usesIfVariable_SelectS_ClauseHoldsTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(UsesIfVariableS);
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(usesIfVariable_SelectV_ClauseHoldsTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(UsesIfVariableV);
			std::unordered_set<std::string> expect = { "a", "p", "c", "k", "ifs", "w"};
			Assert::IsTrue(result == expect);
		}

		TEST_METHOD(usesWhileVariable_SelectS_ClauseHoldsTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(UsesWhileVariableS);
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(usesWhileVariable_SelectV_ClauseHoldsTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(UsesWhileVariableV);
			std::unordered_set<std::string> expect = { "a", "ifs", "w" };
			Assert::IsTrue(result == expect);
		}

		TEST_METHOD(usesAssignString_ClauseHoldsTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(UsesAssignString);
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(usesAssignString_ClauseNotTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(InvalidUsesAssignString);
			Assert::IsTrue(result.empty());
		}

		TEST_METHOD(usesPrintString_ClauseHoldsTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(UsesPrintString);
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(usesPrintString_ClauseNotTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(InvalidUsesPrintString);
			Assert::IsTrue(result.empty());
		}

		TEST_METHOD(usesIfString_ClauseHoldsTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(UsesIfString);
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(usesIfString_ClauseNotTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(InvalidUsesIfString);
			Assert::IsTrue(result.empty());
		}

		TEST_METHOD(usesWhileString_ClauseHoldsTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(UsesWhileString);
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(usesWhileString_ClauseNotTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(InvalidUsesWhileString);
			Assert::IsTrue(result.empty());
		}

		/*
		UsesS for call and prog_line
		*/
		TEST_METHOD(UsesCallSynonymInClause_ReturnCallStmtThatUsesV) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(UsesCallSynonymInClause);
			std::unordered_set<std::string> expected = { "17" };
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(UsesCallSynonymNotInClause_ReturnAllCallStmts) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(UsesCallSynonymNotInClause);
			std::unordered_set<std::string> expected = { "17", "19" };
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(UsesCallSynonymStringInClause_ReturnCallStmtThatUsesVariableString) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(UsesCallSynonymStringInClause);
			std::unordered_set<std::string> expected = { "17" };
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(UsesCallSynonymStringInClause2_ReturnNothing) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(UsesCallSynonymStringInClause2);
			std::unordered_set<std::string> expected = {};
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(UsesCallSynonymAnyInClause_ReturnCallStmtThatUsesSomething) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(UsesCallSynonymAnyInClause);
			std::unordered_set<std::string> expected = {"17"};
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(UsesCallSynonymAnyNotInClause_ReturnAllCallStmts) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(UsesCallSynonymAnyNotInClause);
			std::unordered_set<std::string> expected = { "17", "19" };
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(UsesProgLineInClause_ReturnAllProgLineUsingVariableA) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(UsesProgLineInClause);
			std::unordered_set<std::string> expected = { "3", "4", "5", "7", "8", "9", "14", "17", "18" };
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(UsesProgLineNotInClause_ReturnAllProgLine) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(UsesProgLineNotInClause);
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(UsesProcVarInClause_ReturnAllProgLineUsing) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(UsesProcVarInClause);
			std::unordered_set<std::string> expected = { "testA", "testB" };
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(UsesProcVarNotInClause_ReturnAllProgLine) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(UsesProcVarNotInClause);
			std::unordered_set<std::string> expected = { "testA", "testB", "testC" };
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(UsesProcStrInClause_ReturnAllProgLineUsing) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(UsesProcStrInClause);
			std::unordered_set<std::string> expected = { "testA", "testB" };
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(UsesProcStrNotInClause_ReturnAllProgLine) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(UsesProcStrNotInClause);
			std::unordered_set<std::string> expected = { "testA", "testB", "testC" };
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(UsesProcAnyInClause_ReturnAllProgLineUsing) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(UsesProcAnyInClause);
			std::unordered_set<std::string> expected = { "testA", "testB" };
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(UsesProcAnyNotInClause_ReturnAllProgLine) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(UsesProcAnyNotInClause);
			std::unordered_set<std::string> expected = { "testA", "testB", "testC" };
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(UsesStrVarInClauseTrue_ReturnAllProgLineUsing) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(UsesStrVarInClauseTrue);
			std::unordered_set<std::string> expected = { "testA", "testB", "testC" };
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(UsesStrVarInClauseFalse_ReturnAllProgLine) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(UsesStrVarInClauseFalse);
			std::unordered_set<std::string> expected = {};
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(UsesStrStrInClauseTrue_ReturnAllProgLineUsing) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(UsesStrStrInClauseTrue);
			std::unordered_set<std::string> expected = { "testA", "testB", "testC" };
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(UsesStrStrInClauseFalse_ReturnAllProgLine) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(UsesStrStrInClauseFalse);
			std::unordered_set<std::string> expected = {};
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(UsesStrAnyInClauseTrue_ReturnAllProgLineUsing) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(UsesStrAnyInClauseTrue);
			std::unordered_set<std::string> expected = { "testA", "testB", "testC" };
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(UsesStrAnyInClauseFalse_ReturnAllProgLine) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(UsesStrAnyInClauseFalse);
			std::unordered_set<std::string> expected = {};
			Assert::IsTrue(result == expected);
		}
	};
}