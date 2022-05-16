#include "stdafx.h"
#include "CppUnitTest.h"
#include "DesignExtractor.h"
#include "DesignExtractorAffectsTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace IntegrationTesting
{
	TEST_CLASS(DesignExtractorAffectsTest)
	{
	public:
		TEST_CLASS_INITIALIZE(InitializePKB) {
			PKB::clear();

			// TODO: add another procedure

			/*
			* procedure main {
			1	read a;
			2	sum = a + b;
			3	print sum;
			4	subtract = a - b;
			5	if (subtract > b) then {
			6		sum = sum - subtract;
				} else {
			7		sum = sum + subtract;
				}
			8	if (sum < subtract) then {
			9		sum = subtract + sum;
			10		print subtract;
				} else {
			11		subtract = subtract - sum;
				}
			12	while (subtract < sum) {
			13      	sum = sum + 2;
			14      	if (use14 == 4) {
			15        		print subtract;
					} else {
			16       		sum = sum - 1;
					}
				}
			17	if (a < b) then {
			18		if (b < c) then {
			19			addr = register + x / y - z;
					else {
			20			register = addr - z * y;
					}
			21		addr = addr + register;
				} else {
			22		while (z != y) {
			23			register = y + z;
					}
				}
			24	while (x != y) {
			25		call B;
			26		register = register + 2;
			27		while (z > 3) {
			28			register = addr + z / x - register;
			29			addr = register - addr + sum;
					}
			30		register = x;
				}
			}

			procedure B {
			31	read addr;
			}
			*/

			PKB::addStatement("main", Statement(3, 1)); // 1: assign, 2: print, 3: read, 4: if, 5: while, 6: else, 7: call
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

			PKB::addStatement("main", Statement(4, 17));
			PKB::addStatement("main", Statement(4, 18));
			PKB::addStatement("main", Statement(1, 19));
			PKB::addStatement("main", Statement(1, 20));
			PKB::addStatement("main", Statement(1, 21));
			PKB::addStatement("main", Statement(5, 22));
			PKB::addStatement("main", Statement(1, 23));
			PKB::addStatement("main", Statement(5, 24));
			PKB::addStatement("main", Statement(7, 25));
			PKB::addStatement("main", Statement(1, 26));
			PKB::addStatement("main", Statement(5, 27));
			PKB::addStatement("main", Statement(1, 28));
			PKB::addStatement("main", Statement(1, 29));
			PKB::addStatement("main", Statement(1, 30));

			PKB::addStatement("B", Statement(3, 31));

			// Follows
			PKB::addFollows(1, 2);
			PKB::addFollows(2, 3);
			PKB::addFollows(3, 4);
			PKB::addFollows(4, 5);
			PKB::addFollows(5, 8);
			PKB::addFollows(8, 12);
			PKB::addFollows(9, 10);
			PKB::addFollows(12, 17);
			PKB::addFollows(13, 14);
			PKB::addFollows(18, 21);
			PKB::addFollows(17, 24);
			PKB::addFollows(25, 26);
			PKB::addFollows(26, 27);
			PKB::addFollows(28, 29);
			PKB::addFollows(24, 30);

			// Parent
			std::unordered_set<int> children1({ 6, 7 });
			PKB::addParent(5, children1);
			std::unordered_set<int> children2({ 9, 10, 11 });
			PKB::addParent(8, children2);
			std::unordered_set<int> children3({ 13, 14 });
			PKB::addParent(12, children3);
			std::unordered_set<int> children4({ 15, 16 });
			PKB::addParent(14, children4);

			PKB::addParent(17, {18, 21, 22});
			PKB::addParent(18, { 19, 20 });
			PKB::addParent(22, { 23 });
			PKB::addParent(24, { 25, 26, 27, 30 });
			PKB::addParent(27, { 28, 29 });

			// Next
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

			PKB::addNext(17, 18);
			PKB::addNext(18, 19);
			PKB::addNext(18, 20);
			PKB::addNext(19, 21);
			PKB::addNext(20, 21);
			PKB::addNext(17, 22);
			PKB::addNext(21, 24);
			PKB::addNext(22, 23);
			PKB::addNext(23, 22);
			PKB::addNext(22, 24);

			PKB::addNext(24, 25);
			PKB::addNext(25, 26);
			PKB::addNext(26, 27);
			PKB::addNext(27, 28);
			PKB::addNext(27, 30);
			PKB::addNext(28, 29);
			PKB::addNext(29, 27);
			PKB::addNext(30, 24);

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

			std::string modS9 = "sum";
			PKB::addModifies(9, 1, modS9);
			std::unordered_set<std::string> useS9({ "sum", "subtract" });
			PKB::addUses(9, 1, useS9);

			std::unordered_set<std::string> useS10({ "subtract" });
			PKB::addUses(10, 2, useS10);

			std::string modS11 = "subtract";
			PKB::addModifies(11, 1, modS11);
			std::unordered_set<std::string> useS11({ "sum", "subtract" });
			PKB::addUses(11, 1, useS11);

			std::unordered_set<std::string> useS12({ "subtract", "sum" });
			PKB::addUses(12, 5, useS12);

			std::string modS13 = "sum";
			PKB::addModifies(13, 1, modS13);
			std::unordered_set<std::string> useS13({ "sum" });
			PKB::addUses(13, 1, useS13);

			std::unordered_set<std::string> useS14({ "useS14" });
			PKB::addUses(14, 4, useS14);

			std::unordered_set<std::string> useS15({ "subtract" });
			PKB::addUses(15, 2, useS15);

			std::string modS16 = "sum";
			PKB::addModifies(16, 1, modS16);
			std::unordered_set<std::string> useS16({ "sum" });
			PKB::addUses(16, 1, useS16);

			PKB::addUses(17, 4, {"a", "b"});
			PKB::addUses(18, 4, { "b", "c" });

			PKB::addModifies(19, 1, "addr");
			PKB::addUses(19, 1, { "register", "x", "y", "z" });

			PKB::addModifies(20, 1, "register");
			PKB::addUses(20, 1, { "addr", "y", "z" });

			PKB::addModifies(21, 1, "addr");
			PKB::addUses(21, 1, { "register", "addr" });

			PKB::addUses(22, 5, { "z", "y" });

			PKB::addModifies(23, 1, "sum");
			PKB::addUses(23, 1, { "sum" });

			PKB::addUses(24, 5, { "x", "y" });

			// 25 is a call stmt, will be added transitively
			PKB::addCall("main", "B", 25);

			PKB::addModifies(26, 1, "register");
			PKB::addUses(26, 1, { "register" });

			PKB::addUses(27, 5, { "z" });

			PKB::addModifies(28, 1, "register");
			PKB::addUses(28, 1, { "addr", "z", "x", "register" });

			PKB::addModifies(29, 1, "addr");
			PKB::addUses(29, 1, { "register", "addr", "sum" });

			PKB::addModifies(30, 1, "register");
			PKB::addUses(30, 1, { "x" });
			
			// B
			PKB::addModifies(31, 3, "addr");
			PKB::addModifiedVarToProc("B", { "addr" });

			validProgramFlag = DesignExtractor::validateProcedures();
			processProgramFlag = DesignExtractor::processProgram();

		}

		TEST_METHOD(initTest) {
			Assert::IsTrue(validProgramFlag);
			Assert::IsTrue(processProgramFlag);
		}

		TEST_METHOD(affectsOfNonAssignStmt_assertNoAffectsRs) {
			int readStmt, printStmt;

			readStmt = 1;
			printStmt = 3;

			Assert::IsTrue(PKB::getAffectedByOfStmt(readStmt).empty());
			Assert::IsTrue(PKB::getAffectingOfStmt(readStmt).empty());
			Assert::IsTrue(PKB::getAffectedByOfStmt(printStmt).empty());
			Assert::IsTrue(PKB::getAffectingOfStmt(printStmt).empty());
		}

		TEST_METHOD(affectsOfFirstAssignStmt_assertNoAffectedBy) {
			int stmt = 2;

			Assert::IsTrue(PKB::getAffectedByOfStmt(stmt).empty());
		}

		TEST_METHOD(affectsOfAssignStmtInIfElse_assertSameAffectedBy) {
			int stmt1 = 6;
			int stmt2 = 7;

			std::unordered_set<int> actualResult1 = PKB::getAffectedByOfStmt(stmt1);
			std::unordered_set<int> actualResult2 = PKB::getAffectedByOfStmt(stmt2);
			
			std::unordered_set<int> expectedResult = { 2, 4 };

			//for (auto& x : actualResult1) {
			//	Logger::WriteMessage(std::to_string(x).c_str());
			//}

			Assert::IsTrue(actualResult1 == expectedResult);
			Assert::IsTrue(actualResult2 == expectedResult);

		}

		TEST_METHOD(affectingOfAssignStmt_returnMultipleStmtsInIfElse) {
			int stmt = 2;
			std::unordered_set<int> actualResult = PKB::getAffectingOfStmt(stmt);
			std::unordered_set<int> expectedResult = { 6, 7 };

			Assert::IsTrue(actualResult == expectedResult);
		}

		TEST_METHOD(affectedBy_stmtInWhileAffectedByIfElse) {
			int stmt = 13;
			std::unordered_set<int> actualResult = PKB::getAffectedByOfStmt(stmt);
			std::unordered_set<int> expectedResult = { 9, 6, 7, 16, 13 };

			Assert::IsTrue(actualResult == expectedResult);
		}

		TEST_METHOD(affectedBy_stmtInElseInWhile) {
			int stmt = 16;
			std::unordered_set<int> actualResult = PKB::getAffectedByOfStmt(stmt);
			std::unordered_set<int> expectedResult = { 13 };

			Assert::IsTrue(actualResult == expectedResult);
		}

		TEST_METHOD(affects_affectedByStmtInNestedIfElse_affectingStmtInsideInWhile) {
			int stmt = 23;
			std::unordered_set<int> actualResult = PKB::getAffectedByOfStmt(stmt);
			std::unordered_set<int> expectedResult = { 6, 7, 9, 13, 16, 23 };

			Assert::IsTrue(actualResult == expectedResult);
		}

		TEST_METHOD(affects_affectedByStmtInNestedIfElse_affectingStmtInsideInIfElse) {
			int stmt = 20;
			std::unordered_set<int> actualResult = PKB::getAffectedByOfStmt(stmt);
			std::unordered_set<int> expectedResult = { };

			Assert::IsTrue(actualResult == expectedResult);
		}

		TEST_METHOD(affects_stmtInNestedWhile_onlyAffectedByPrecedingStmtAndItself) {
			int stmt = 29;
			std::unordered_set<int> actualResult = PKB::getAffectedByOfStmt(stmt);
			std::unordered_set<int> expectedResult = { 6, 7, 9, 13, 16, 23, 28, 29 };

			Assert::IsTrue(actualResult == expectedResult);
		}

		TEST_METHOD(affects_affectedByStmtInNestedIfElse_affectingStmtOutsideInWhile) {
			int stmt = 26;
			std::unordered_set<int> actualResult = PKB::getAffectedByOfStmt(stmt);
			std::unordered_set<int> expectedResult = { 30, 20 };

			Assert::IsTrue(actualResult == expectedResult);
		}

		TEST_METHOD(affects_stmtInNestWhile) {
			int stmt = 28;
			std::unordered_set<int> actualResult = PKB::getAffectedByOfStmt(stmt);
			std::unordered_set<int> expectedResult = { 28, 29, 26 };

			Assert::IsTrue(actualResult == expectedResult);
		}

		TEST_METHOD(affecting_stmtInIfElseAffecting) {
			int stmt = 6;
			std::unordered_set<int> actualResult = PKB::getAffectingOfStmt(stmt);
			std::unordered_set<int> expectedResult = { 9, 11, 13, 23, 29 };

			Assert::IsTrue(actualResult == expectedResult);
		}

		TEST_METHOD(affectedByT_stmtInElseInWhile) {
			int stmt = 16;
			std::unordered_set<int> actualResult = PKB::getAffectedByTOfStmt(stmt);
			std::unordered_set<int> expectedResult = { 2, 4, 6, 7, 9, 13, 16 };

			//for (auto& x : actualResult) {
			//	Logger::WriteMessage(std::to_string(x).c_str());
			//}

			Assert::IsTrue(actualResult == expectedResult);
		}

		TEST_METHOD(affectedByT_stmtInNestedWhile) {
			int stmt = 29;
			std::unordered_set<int> actualResult = PKB::getAffectedByTOfStmt(stmt);
			std::unordered_set<int> expectedResult = {28, 26, 30, 29, 23, 20, 16, 13, 9, 7, 6, 4, 2};

			Assert::IsTrue(actualResult == expectedResult);
		}

		TEST_METHOD(affectingT_secondAssignStmt) {
			int stmt = 4;
			std::unordered_set<int> actualResult = PKB::getAffectingTOfStmt(stmt);
			std::unordered_set<int> expectedResult = { 6, 7, 9, 11, 13, 16, 23, 29, 28 };

			//for (auto& x : actualResult) {
			//	Logger::WriteMessage(std::to_string(x).c_str());
			//}

			Assert::IsTrue(actualResult == expectedResult);
		}

		TEST_CLASS_CLEANUP(ClearPKB) {
			PKB::clear();
		}
	};
}