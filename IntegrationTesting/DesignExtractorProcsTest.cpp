#include "stdafx.h"
#include "CppUnitTest.h"
#include "DesignExtractor.h"
#include "DesignExtractorProcsTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace IntegrationTesting
{
	TEST_CLASS(DesignExtractorProcsTest)
	{
	public:
		TEST_CLASS_INITIALIZE(InitializePKB) {
			PKB::clear();

			/*
			* procedure A {
			1	read modA;
			2	call C;
			3	call B;
			}

			procedure B {
			4	modB1 = usesB1 - usesB2
			5	if (usesB1 > usesB3) then {
			6		call D;
				} else {
			7		call E;
				}
			}

			procedure D {
			8	call F;
			}

			procedure F {
			9	modF1 = usesF1 + modF1;
			10	print modF1;
			}

			procedure G {
			11	call C;
			}

			procedure C {
			12	while (usesC1 < usesC2) {
			13    modC1 = usesC1 + 2;
			14    if (usesC3 == 4) {
			15       print usesC3;
				  } else {
			16       modC2 = usesC3 - 1;
				  }
				}
			17	call F;
			}

			procedure E {
			18 modE1 = usesE1 - modE1;
			}
			*/

			PKB::addStatement("A", Statement(3, 1)); // 1:assign, 2:print, 3:read, 4:if, 5:while, 6:else, 7:call
			PKB::addStatement("A", Statement(7, 2));
			PKB::addStatement("A", Statement(7, 3));

			PKB::addStatement("B", Statement(1, 4));
			PKB::addStatement("B", Statement(4, 5));
			PKB::addStatement("B", Statement(7, 6));
			PKB::addStatement("B", Statement(7, 7));

			PKB::addStatement("C", Statement(5, 12));
			PKB::addStatement("C", Statement(1, 13));
			PKB::addStatement("C", Statement(4, 14));
			PKB::addStatement("C", Statement(2, 15));
			PKB::addStatement("C", Statement(1, 16));
			PKB::addStatement("C", Statement(7, 17));

			PKB::addStatement("D", Statement(7, 8));

			PKB::addStatement("E", Statement(1, 18));

			PKB::addStatement("F", Statement(1, 9));
			PKB::addStatement("F", Statement(2, 10));

			PKB::addStatement("G", Statement(7, 11));

			PKB::addCall("A", "B", 3);
			PKB::addCall("A", "C", 2);
			PKB::addCall("B", "D", 6);
			PKB::addCall("B", "E", 7);
			PKB::addCall("D", "F", 8);
			PKB::addCall("G", "C", 11);
			PKB::addCall("C", "F", 17);

			// Follows
			PKB::addFollows(1, 2);
			PKB::addFollows(2, 3);
			PKB::addFollows(4, 5);
			PKB::addFollows(9, 10);
			PKB::addFollows(12, 17);
			PKB::addFollows(13, 14);

			// Next
			PKB::addNext(1, 2);
			PKB::addNext(2, 3);
			PKB::addNext(4, 5);
			PKB::addNext(5, 6);
			PKB::addNext(5, 7);
			PKB::addNext(9, 10);
			PKB::addNext(12, 17);
			PKB::addNext(12, 13);
			PKB::addNext(13, 14);
			PKB::addNext(14, 15);
			PKB::addNext(14, 16);
			PKB::addNext(15, 12);
			PKB::addNext(16, 12);

			// Parent
			std::unordered_set<int> children1({ 6, 7 });
			PKB::addParent(5, children1);
			std::unordered_set<int> children2({ 13, 14 });
			PKB::addParent(12, children2);
			std::unordered_set<int> children3({ 15, 16 });
			PKB::addParent(14, children3);

			// A
			std::string modS1 = "modA";
			PKB::addModifies(1, 3, modS1); // stmtNo, stmtType, str

			// add to proc
			PKB::addModifiedVarToProc("A", { modS1 });
			
			// B
			std::string modS4 = "modB1";
			PKB::addModifies(4, 1, modS4);
			std::unordered_set<std::string> useS4({ "usesB1", "usesB2" });
			PKB::addUses(4, 1, useS4);

			std::unordered_set<std::string> useS5({ "usesB1", "usesB3" });
			PKB::addUses(5, 4, useS5);

			// add to proc
			PKB::addProcUses("B", useS4);
			PKB::addProcUses("B", useS5);
			PKB::addModifiedVarToProc("B", { "modB1" });

			// C
			std::unordered_set<std::string> useS12({ "usesC1", "usesC2" });
			PKB::addUses(12, 5, useS12);

			std::string modS13 = "modC1";
			PKB::addModifies(13, 1, modS13);
			std::unordered_set<std::string> useS13({ "usesC1" });
			PKB::addUses(13, 1, useS13);

			std::unordered_set<std::string> useS14({ "usesC3" });
			PKB::addUses(14, 4, useS14);

			std::unordered_set<std::string> useS15({ "usesC3" });
			PKB::addUses(15, 2, useS15);

			std::string modS16 = "modC2";
			PKB::addModifies(16, 1, modS16);
			std::unordered_set<std::string> useS16({ "usesC3" });
			PKB::addUses(16, 1, useS16);

			// add to proc
			PKB::addProcUses("C", useS12);
			PKB::addProcUses("C", useS13);
			PKB::addProcUses("C", useS14);
			PKB::addProcUses("C", useS15);
			PKB::addProcUses("C", useS16);
			PKB::addModifiedVarToProc("C", { "modC1", "modC2" });

			// D

			// E
			std::string modS18 = "modE1";
			PKB::addModifies(18, 1, modS18);
			std::unordered_set<std::string> useS18({ "usesE1", "modE1" });
			PKB::addUses(18, 1, useS18);

			// add to proc
			PKB::addProcUses("E", useS18);
			PKB::addModifiedVarToProc("C", { "modE1" });

			// F
			std::string modS9 = "modF1";
			PKB::addModifies(9, 1, modS9);
			std::unordered_set<std::string> useS9({ "usesF1", "modF1" });
			PKB::addUses(9, 1, useS9);

			std::unordered_set<std::string> useS10({ "modF1" });
			PKB::addUses(10, 2, useS10);

			// add to proc
			PKB::addProcUses("F", useS9);
			PKB::addProcUses("F", useS10);
			PKB::addModifiedVarToProc("F", { "modF1" });

			// G

			DesignExtractor::validateProcedures();
            DesignExtractor::processProgram();
            
        }

		TEST_METHOD(initTests_returnTrue)
		{
			std::unordered_set<std::string> leaves = { "F", "E" };
			Assert::IsTrue(PKB::getLeafProcs() == leaves);

			std::unordered_set<std::string> roots = { "A", "G" };
			Assert::IsTrue(PKB::getRootProcs() == roots);

			Assert::IsTrue(DesignExtractor::validateProcedures());
		}
		
		TEST_METHOD(computeAllCalleesTest_CalleesOfRoot_ReturnAllCallees)
		{
			std::unordered_set<std::string> expectedResults = allCallees;
			std::unordered_set<std::string> actualResults = PKB::getTCalleesOfProc("A");

			Assert::IsTrue(expectedResults == actualResults);
		}

		TEST_METHOD(computeAllCallersTest_CallersOfLeaf_ReturnAllCallers)
		{
			std::unordered_set<std::string> expectedResults = allCallers;
			std::unordered_set<std::string> actualResults = PKB::getTCallersOfProc("F");

			Assert::IsTrue(expectedResults == actualResults);
		}

		TEST_METHOD(computeAllCallersTest_intermediateProcE_ReturnAB)
		{
			std::unordered_set<std::string> expectedResults = { "A", "B" };
			std::unordered_set<std::string> actualResults = PKB::getTCallersOfProc("E");

			Assert::IsTrue(expectedResults == actualResults);
		}

        TEST_METHOD(updateProcedureUsesTest_rootProc_returnAllVarsUsed)
        {
            std::unordered_set<std::string> expectedResults = allVarsUsed;
            std::unordered_set<std::string> actualResults = PKB::getUsedVarsInProc("A");

            Assert::IsTrue(expectedResults == actualResults);
        }

		TEST_METHOD(updateProcedureModifiedTest_rootProc_returnAllVarsUsed)
		{
			std::unordered_set<std::string> expectedResults = allVarsModified;
			std::unordered_set<std::string> actualResults = PKB::getModifiedVarsInProc("A");

			Assert::IsTrue(expectedResults == actualResults);
		}

		TEST_METHOD(updateProcedureUsesTest_intermediateProcG_returnVarsUsedByCF)
		{
			std::unordered_set<std::string> varsUsedByG = PKB::getUsedVarsInProc("C");
			std::unordered_set<std::string> varsUsedByF = PKB::getUsedVarsInProc("F");
			varsUsedByG.insert(varsUsedByF.begin(), varsUsedByF.end());

			std::unordered_set<std::string> expectedResults = varsUsedByG;
			std::unordered_set<std::string> actualResults = PKB::getUsedVarsInProc("G");
			//for (auto& x : actualResults) {
			//	Logger::WriteMessage(x.c_str());
			//}
			Assert::IsTrue(expectedResults == actualResults);
		}

		TEST_METHOD(updateProcedureUsesTest_intermediateProcB_returnVarsUsedByBDEF)
		{
			std::unordered_set<std::string> varsUsedByB = PKB::getUsedVarsInProc("B");
			std::unordered_set<std::string> varsUsedByD = PKB::getUsedVarsInProc("D");
			std::unordered_set<std::string> varsUsedByE = PKB::getUsedVarsInProc("E");
			std::unordered_set<std::string> varsUsedByF = PKB::getUsedVarsInProc("F");
			varsUsedByB.insert(varsUsedByD.begin(), varsUsedByD.end());
			varsUsedByB.insert(varsUsedByE.begin(), varsUsedByE.end());
			varsUsedByB.insert(varsUsedByF.begin(), varsUsedByF.end());

			std::unordered_set<std::string> expectedResults = varsUsedByB;
			std::unordered_set<std::string> actualResults = PKB::getUsedVarsInProc("B");

			Assert::IsTrue(expectedResults == actualResults);
		}

		TEST_METHOD(updateProcedureModifiesTest_rootProc_returnAllVarsModified)
		{
			std::unordered_set<std::string> expectedResults = allVarsModified;
			std::unordered_set<std::string> actualResults = PKB::getModifiedVarsInProc("A");

			Assert::IsTrue(expectedResults == actualResults);
		}

		// Regression Testing
		TEST_METHOD(computeAllFollowed_consecutiveCallStmts_returnPreviousStmt)
		{
			int stmt = 3;

			std::unordered_set<int> expectedResult = { 1, 2 };
			std::unordered_set<int> actualResult = DesignExtractor::computeAllFollowed(stmt);

			Assert::IsTrue(expectedResult == actualResult);
		}

		TEST_METHOD(computeAllFollowed_nestedStmtWithOneFollowed_returnOneStmt)
		{
			int stmt = 14;

			std::unordered_set<int> expectedResult = { 13 };
			std::unordered_set<int> actualResult = DesignExtractor::computeAllFollowed(stmt);

			Assert::IsTrue(expectedResult == actualResult);
		}

		TEST_METHOD(computeAllFollowers_containerStmtWithNoFollower_returnEmptySet)
		{
			int stmt = 5;

			std::unordered_set<int> expectedResult = emptyIntSet;
			std::unordered_set<int> actualResult = DesignExtractor::computeAllFollowers(stmt);

			Assert::IsTrue(expectedResult == actualResult);
		}

		TEST_METHOD(computeAllFollowers_containerStmtWithOneFollower_returnOneStmt)
		{
			int stmt = 12;

			std::unordered_set<int> expectedResult = {17};
			std::unordered_set<int> actualResult = DesignExtractor::computeAllFollowers(stmt);

			Assert::IsTrue(expectedResult == actualResult);
		}

		TEST_METHOD(computeAllAncestors_nestedStmtWithOneParent_returnOneStmt)
		{
			int stmt = 7;

			std::unordered_set<int> expectedResult = { 5 };
			std::unordered_set<int> actualResult = DesignExtractor::computeAllAncestors(stmt);

			Assert::IsTrue(expectedResult == actualResult);
		}

		TEST_METHOD(computeAllAncestors_nestedStmtWithMultipleAncestors_returnMultiple)
		{
			int stmt = 15;

			std::unordered_set<int> expectedResult = { 12, 14 };
			std::unordered_set<int> actualResult = DesignExtractor::computeAllAncestors(stmt);

			Assert::IsTrue(expectedResult == actualResult);
		}

		TEST_METHOD(computeAllDescendants_containerStmtWithMultipleDescendantes_returnMultiple)
		{
			int stmt = 12;

			std::unordered_set<int> expectedResult = { 13, 14, 15, 16 };
			std::unordered_set<int> actualResult = DesignExtractor::computeAllDescendants(stmt);

			Assert::IsTrue(expectedResult == actualResult);
		}
		
		TEST_CLASS_CLEANUP(ClearPKB) {
			PKB::clear();
		}
	};
}