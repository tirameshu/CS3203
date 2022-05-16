#include "stdafx.h"
#include "CppUnitTest.h"
#include "DesignExtractor.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace IntegrationTesting
{
	TEST_CLASS(DesignExtractorSingleProcTest)
	{
	public:
		TEST_CLASS_INITIALIZE(InitializePKB) {
			PKB::clear();

			/*
			* procedure main {
			1	read a;
			2	sum = a + b;
			3	print sum;
			4	subtract = a - b;
			5	if (subtract > b) then {
			6			sum = sum - subtract;
				} else {
			7			sum = sum + subtract;
				}
			8	if (sum < subtract) then {
			9		subtract = subtract + sum;
			10		print subtract;
				} else {
			11		subtract = subtract - sum;
				}
			12	while (subtract < sum) {
			13    mod13 = use13 + 2;
			14    if (use14 == 4) {
			15       print use15;
			      } else {
			16       mod16 = use16 - 1;
				  }
				}
			17	print subtract;
			} 
			*/
			PKB::addStatement("main", Statement(3, 1)); // 1:assign, 2:print, 3:read, 4:if, 5:while
			PKB::addStatement("main", Statement(1, 2));
			PKB::addStatement("main", Statement(2, 3));
			PKB::addStatement("main", Statement(1, 4));
			PKB::addStatement("main", Statement(4, 5));
			PKB::addStatement("main", Statement(1, 6));
			PKB::addStatement("main", Statement(1, 7));
			PKB::addStatement("main", Statement(4, 8));
			PKB::addStatement("main", Statement(1, 9));
			PKB::addStatement("main", Statement(2, 10));
			PKB::addStatement("main", Statement(1, 11));
			PKB::addStatement("main", Statement(5, 12));
			PKB::addStatement("main", Statement(1, 13));
			PKB::addStatement("main", Statement(4, 14));
			PKB::addStatement("main", Statement(2, 15));
			PKB::addStatement("main", Statement(1, 16));
			PKB::addStatement("main", Statement(2, 17));

			// FollowsT
			PKB::addFollows(1, 2);
			PKB::addFollows(2, 3);
			PKB::addFollows(3, 4);
			PKB::addFollows(4, 5);
			PKB::addFollows(5, 8);
			PKB::addFollows(8, 12);
			PKB::addFollows(12, 17);
			PKB::addFollows(13, 14);
			PKB::addFollows(9, 10);
			
			// ParentT
			std::unordered_set<int> children1({ 6, 7 });
			PKB::addParent(5, children1);
			std::unordered_set<int> children2({ 9, 10, 11 });
			PKB::addParent(8, children2);
			std::unordered_set<int> children3({ 13, 14 });
			PKB::addParent(12, children3);
			std::unordered_set<int> children4({ 15, 16 });
			PKB::addParent(14, children4);

			// NextT
			PKB::addNext(1, 2);
			PKB::addNext(2, 3);
			PKB::addNext(3, 4);
			PKB::addNext(4, 5);
			PKB::addNext(5, 6);
			PKB::addNext(5, 7);
			PKB::addNext(6, 8);
			PKB::addNext(7, 8);
			PKB::addNext(8, 9);
			PKB::addNext(9, 10);
			PKB::addNext(8, 11);
			PKB::addNext(10, 12);
			PKB::addNext(11, 12);
			PKB::addNext(12, 13);
			PKB::addNext(13, 14);
			PKB::addNext(14, 15);
			PKB::addNext(14, 16);
			PKB::addNext(15, 12);
			PKB::addNext(16, 12);
			PKB::addNext(12, 17);

			std::string modS1 = "a";
			PKB::addModifies(1, 3, modS1); // stmtNo, stmtType, str

			std::string modS2 = "sum";
			PKB::addModifies(2, 1, modS2);
			std::unordered_set<std::string> useS2({ "a", "b" });
			PKB::addUses(2, 1, useS2); // stmtNo, stmtType, set

			std::unordered_set<std::string> useS3({ "sum" });
			PKB::addUses(3, 2, useS3);

			std::string modS4 = "subtract";
			PKB::addModifies(4, 1, modS4);
			std::unordered_set<std::string> useS4({ "a", "b" });
			PKB::addUses(4, 1, useS4);

			std::unordered_set<std::string> useS5({ "subtract", "b" });
			PKB::addUses(5, 4, useS5);

			std::string modS6 = "sum";
			PKB::addModifies(6, 1, modS6);
			std::unordered_set<std::string> useS6({ "sum", "subtract" });
			PKB::addUses(6, 1, useS6);

			std::string modS7 = "sum";
			PKB::addModifies(7, 1, modS7);
			std::unordered_set<std::string> useS7({ "sum", "subtract" });
			PKB::addUses(7, 1, useS7);

			std::unordered_set<std::string> useS8({ "sum", "subtract" });
			PKB::addUses(8, 4, useS8);

			std::string modS9 = "subtract";
			PKB::addModifies(9, 1, modS9);
			std::unordered_set<std::string> useS9({ "sum", "subtract" });
			PKB::addUses(9, 1, useS9);

			std::unordered_set<std::string> useS10({ "subtract" });
			PKB::addUses(10, 2, useS10);

			std::string modS11 = "subtract";
			PKB::addModifies(11, 1, modS11);
			std::unordered_set<std::string> useS11({ "sum", "subtract" });
			PKB::addUses(11, 1, useS11);

			std::unordered_set<std::string> useS12({ "sum", "subtract" });
			PKB::addUses(12, 5, useS12);

			std::string modS13 = "modS13";
			PKB::addModifies(13, 1, modS13);
			std::unordered_set<std::string> useS13({ "useS13" });
			PKB::addUses(13, 1, useS13);

			std::unordered_set<std::string> useS14({ "useS14" });
			PKB::addUses(14, 4, useS14);
			
			std::unordered_set<std::string> useS15({ "useS15" });
			PKB::addUses(15, 2, useS15);
    
			std::string modS16 = "modS16";
			PKB::addModifies(16, 1, modS16);
			std::unordered_set<std::string> useS16({ "useS16" });
			PKB::addUses(16, 1, useS16);
    
			std::unordered_set<std::string> useS17({ "subtract" });
			PKB::addUses(17, 2, useS17);

			DesignExtractor::processProgram();
		}

		// Followed
		TEST_METHOD(computeAllFollowedTest_noFollowed_returnEmptySet)
		{
			int stmt = 1;
			std::unordered_set<int> actualResult = DesignExtractor::computeAllFollowed(stmt);

			Assert::IsTrue(actualResult.size() == 0);
		}

		TEST_METHOD(computeAllFollowedTest_multipleFollowed_returnPopulatedSet)
		{
			int stmt = 17;
			std::unordered_set<int> expectedResult = { 1, 2, 3, 4, 5, 8, 12 };
			std::unordered_set<int> actualResult = DesignExtractor::computeAllFollowed(stmt);
			Assert::IsTrue(expectedResult == actualResult);
		}
		
		// Follower
		TEST_METHOD(computeAllFollowersTest_noFollowerInNested_returnEmptySet)
		{
		  int stmt = 6;
		  std::unordered_set<int> actualResult = DesignExtractor::computeAllFollowers(stmt);

		  Assert::IsTrue(actualResult.size() == 0);
		}

		TEST_METHOD(computeAllFollowersTest_multipleFollowers_returnPopulatedSet)
		{
			int stmt = 4;
			std::unordered_set<int> expectedResult = { 5, 8, 12, 17 };
			std::unordered_set<int> actualResult = DesignExtractor::computeAllFollowers(stmt);

			Assert::IsTrue(expectedResult == actualResult);
		}
  
		TEST_METHOD(computeAllFollowersTest_oneFollowerInNested_returnPopulatedSet)
		{
			int stmt = 9;
			std::unordered_set<int> expectedResult = { 10 };
			std::unordered_set<int> actualResult = DesignExtractor::computeAllFollowers(stmt);
      
			Assert::IsTrue(expectedResult == actualResult);
		}
    
		// Ancestors
		TEST_METHOD(computeAllAncestorsTest_noAncestor_returnEmptySet)
		{
			int stmt = 3;
			std::unordered_set<int> actualResult = DesignExtractor::computeAllAncestors(stmt);
			Assert::IsTrue(actualResult.size() == 0);
		}
    
		TEST_METHOD(computeAllAncestorsTest_multipleAncestors_returnPopulatedSet)
		{
			PKB pkb = PKB();
			int stmt = 16;
			std::unordered_set<int> expectedResult = { 12, 14 };
			std::unordered_set<int> actualResult = DesignExtractor::computeAllAncestors(stmt);
      
			Assert::IsTrue(expectedResult == actualResult);
			Assert::IsTrue(expectedResult == PKB::getAncestorsListOfStmt(stmt));
		}

		// Descendants
		TEST_METHOD(computeAllDescendantsTest_noDescendant_returnEmptySet)
		{
			int stmt = 2;
			std::unordered_set<int> actualResult = DesignExtractor::computeAllDescendants(stmt);
			Assert::IsTrue(actualResult.size() == 0);
		}
		
		TEST_METHOD(computeAllDescendantsTest_multipleDescendants_returnPopulatedSet)
		{
			int stmt = 8;
			std::unordered_set<int> expectedResult = { 9, 10, 11 };
			std::unordered_set<int> actualResult = DesignExtractor::computeAllDescendants(stmt);

			Assert::IsTrue(expectedResult == actualResult);
		}
  
		TEST_METHOD(computeAllDescendantsTest_multipleDescendantsInNested_returnPopulatedSet)
		{
			int stmt = 12;
			std::unordered_set<int> expectedResult = { 13, 14, 15, 16 };
			std::unordered_set<int> actualResult = DesignExtractor::computeAllDescendants(stmt);
      
			Assert::IsTrue(expectedResult == actualResult);
		}

		// Uses
		TEST_METHOD(updateAncestorsUseTest_noTransitiveUsage_returnStmtUseVars)
		{
			int stmt = 7;
			std::unordered_set<std::string> expectedResult = { "sum", "subtract" };
			std::unordered_set<std::string> actualResult = PKB::getUsedVarsInStmt(stmt);

			Assert::IsTrue(expectedResult == actualResult);
		}

		TEST_METHOD(updateAncestorsUseTest_multipleTransitiveUsage_returnPopulatedSet)
		{
			int stmt = 12;
			std::unordered_set<std::string> expectedResult = { "sum", "subtract", "useS13", "useS14", "useS15", "useS16" };
			std::unordered_set<std::string> actualResult = PKB::getUsedVarsInStmt(stmt);

			Assert::IsTrue(expectedResult == actualResult);
		}

		// Modifies
		TEST_METHOD(updateAncestorsModifyTest_noTransitiveModify_returnStmtModifiedVars)
		{
			int stmt = 1;
			std::unordered_set<std::string> expectedResult = { "a" };
			std::unordered_set<std::string> actualResult = PKB::getModifiedVarsInStmt(stmt);

			Assert::IsTrue(expectedResult == actualResult);
		}

		TEST_METHOD(updateAncestorsModifyTest_multipleTransitiveModify_returnPopulatedSet)
		{
			int stmt = 12;
			std::unordered_set<std::string> expectedResult = { "modS13", "modS16" };
			std::unordered_set<std::string> actualResult = PKB::getModifiedVarsInStmt(stmt);

			Assert::IsTrue(expectedResult == actualResult);
		}

		TEST_METHOD(computeAllPrevious_lastStmtOfNonNestedProc_returnMultiple)
		{
			int stmt = 3;

			std::unordered_set<int> expectedResult = { 1, 2 };
			std::unordered_set<int> actualResult = DesignExtractor::computeAllPrevious(stmt);

			//for (auto& x : actualResult) {
			//	Logger::WriteMessage(std::to_string(x).c_str());
			//}

			Assert::IsTrue(expectedResult == actualResult);
		}

		TEST_METHOD(computeAllPrevious_stmtInIfElseClause_returnMultiple)
		{
			int stmt = 7;

			std::unordered_set<int> expectedResult = { 1, 2, 3, 4, 5 };
			std::unordered_set<int> actualResult = DesignExtractor::computeAllPrevious(stmt);

			Assert::IsTrue(expectedResult == actualResult);
		}

		TEST_METHOD(computeAllPrevious_whileStmt_returnAllInStmtList)
		{
			int stmt = 12;

			std::unordered_set<int> expectedResult = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 };
			std::unordered_set<int> actualResult = DesignExtractor::computeAllPrevious(stmt);

			//for (auto& x : actualResult) {
			//	Logger::WriteMessage(std::to_string(x).c_str());
			//}

			Assert::IsTrue(expectedResult == actualResult);
		}

		TEST_METHOD(computeAllPrevious_lastStmtOfNestedProc_returnMultiple)
		{
			int stmt = 17;

			std::unordered_set<int> expectedResult = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 };
			std::unordered_set<int> actualResult = DesignExtractor::computeAllPrevious(stmt);

			Assert::IsTrue(expectedResult == actualResult);
		}

		TEST_METHOD(computeAllNext_whileStmt_returnAllStmtsAfter)
		{
			int stmt = 12;

			std::unordered_set<int> expectedResult = { 12, 13, 14, 15, 16, 17 };
			std::unordered_set<int> actualResult = DesignExtractor::computeAllNext(stmt);

			/*for (auto& x : actualResult) {
				Logger::WriteMessage(std::to_string(x).c_str());
			}*/

			Assert::IsTrue(expectedResult == actualResult);
		}

		TEST_METHOD(computeAllNext_firstStmt_returnAllOtherStmts)
		{
			int stmt = 1;

			std::unordered_set<int> allOtherStmts = { 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17 };
			std::unordered_set<int> expectedResult = allOtherStmts;
			std::unordered_set<int> actualResult = DesignExtractor::computeAllNext(stmt);

			Assert::IsTrue(expectedResult == actualResult);
		}

		TEST_METHOD(computeAllNext_elseStmt_returnAllNext)
		{
			int stmt = 11;

			std::unordered_set<int> allOtherStmts = { 12, 13, 14, 15, 16, 17 };
			std::unordered_set<int> expectedResult = allOtherStmts;
			std::unordered_set<int> actualResult = DesignExtractor::computeAllNext(stmt);

			Assert::IsTrue(expectedResult == actualResult);
		}

		TEST_CLASS_CLEANUP(ClearPKB)
		{
			PKB::clear();
		}
	};
}