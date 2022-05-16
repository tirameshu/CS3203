#include "stdafx.h"
#include "CppUnitTest.h"
#include "AssignEvaluatorTestData.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace IntegrationTesting
{
	TEST_CLASS(AssignEvaluatorTest)
	{
	public:
		std::unordered_set<std::string> allStmt = { "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15", "16" };
		std::unordered_set<std::string> assignStmt = { "1", "2", "4", "9", "11", "12", "16" };

		TEST_CLASS_INITIALIZE(Initialization) {
			PKB pkb;
			pkb.clear();
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
			pkb.addConstant(1, "4");
			pkb.addConstant(2, "2");
			pkb.addConstant(5, "0");
			pkb.addConstant(7, "10");
			pkb.addConstant(9, "1");
			pkb.addConstant(11, "1");
			pkb.addConstant(12, "10");
			pkb.addConstant(16, "10");
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
			pkb.addAssign(1, "p", "4");
			pkb.addAssign(2, "a", "p2*");
			pkb.addAssign(4, "pattern", "pa*a*pifs*+pw*+");
			pkb.addAssign(9, "a", "a1-");
			pkb.addAssign(11, "ifs", "ifs1-");
			pkb.addAssign(12, "a", "10");
			pkb.addAssign(16, "a", "10");
		}

		TEST_METHOD(assignAnyAny_SelectS_ClauseHoldsTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(AssignAnyAnyS);
			Assert::IsTrue(result == allStmt);
		}

		TEST_METHOD(assignAnyAny_SelectA_ClauseHoldsTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(AssignAnyAnyA);
			Assert::IsTrue(result == assignStmt);
		}

		TEST_METHOD(assignVariableAny_SelectS_ClauseHoldsTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(AssignVariableAnyS);
			Assert::IsTrue(result == allStmt);
		}

		TEST_METHOD(assignVariableAny_SelectA_ClauseHoldsTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(AssignVariableAnyA);
			Assert::IsTrue(result == assignStmt);
		}

		TEST_METHOD(assignVariableAny_SelectV_ClauseHoldsTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(AssignVariableAnyV);
			std::unordered_set<std::string> expected = { "p", "a", "pattern", "ifs"};
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(assignAnyString_ClauseHoldsTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(AssignAnyString);
			Assert::IsTrue(result == allStmt);
		}

		TEST_METHOD(assignAnyString_ClauseNotTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(InvalidAssignAnyString);
			Assert::IsTrue(result.empty());
		}

		TEST_METHOD(assignAnyString_SelectA1_ClauseHoldsTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(AssignAnyStringA1);
			std::unordered_set<std::string> expected = { "4", "11" };
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(assignAnyString_SelectA2_ClauseHoldsTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(AssignAnyStringA2);
			std::unordered_set<std::string> expected = { "12", "16" };
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(assignVariableString_ClauseHoldsTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(AssignVariableString);
			Assert::IsTrue(result == allStmt);
		}

		TEST_METHOD(assignVariableString_ClauseNotTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(InvalidAssignVariableString);
			Assert::IsTrue(result.empty());
		}

		TEST_METHOD(assignVariableString_SelectA_ClauseHoldsTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(AssignVariableStringA);
			std::unordered_set<std::string> expected = { "4", "11" };
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(assignVariableString_SelectV_ClauseHoldsTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(AssignVariableStringV);
			std::unordered_set<std::string> expected = { "pattern", "ifs" };
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(assignStringAny_ClauseHoldsTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(AssignStringAny);
			Assert::IsTrue(result == allStmt);
		}

		TEST_METHOD(assignStringAny_ClauseNotTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(InvalidAssignStringAny);
			Assert::IsTrue(result.empty());
		}

		TEST_METHOD(assignStringAny_SelectA_ClauseHoldsTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(AssignStringAnyA);
			std::unordered_set<std::string> expected = { "2", "9", "12", "16"};
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(assignStringString_ClauseHoldsTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(AssignStringString);
			Assert::IsTrue(result == allStmt);
		}

		TEST_METHOD(assignStringString_ClauseNotTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(InvalidAssignStringString);
			Assert::IsTrue(result.empty());
		}

		TEST_METHOD(assignStringString_SelectA_ClauseHoldsTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(AssignStringStringA);
			
			std::unordered_set<std::string> expected = { "4" };
			Assert::IsTrue(result == expected);
		}

		TEST_METHOD(assignStringString_SelectA_ClauseNotTrue) {
			std::unordered_set<std::string> result = QueryEvaluator::evaluateQuery(InvalidAssignStringStringA);
			Assert::IsTrue(result.empty());
		}
	};
}