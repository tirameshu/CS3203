#include "stdafx.h"
#include "CppUnitTest.h"
#include "StatementTypeEnum.h"
#include "AssignProcessor.h"
#include "ParserException.h"
#include "PKB.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace IntegrationTesting
{
	TEST_CLASS(AssignProcessorIT)
	{
	public:
		std::string procedureName = "procedsaioueg123087dtasf0we3";
		std::string modifiedVarname = "adfioug32d413bn965ehnfwugf1";
		std::string expression = "asdbn2379rthf + 3087 - ajkdb134hnc6f * fsdouih3d4b / 98 + ((2 * abs) * 23) + (superduper - superdupernot) - 2";
		std::string postfixExpression = "asdbn2379rthf 3087 + ajkdb134hnc6f fsdouih3d4b * 98 / - 2 abs * 23 * + superduper superdupernot - + 2 -";
		int statementNumber = 1;

		TEST_METHOD_INITIALIZE(clearPkbBefore) {
			PKB::clear();
		}

		TEST_METHOD_CLEANUP(clearPkbAfter) {
			PKB::clear();
		}

		TEST_METHOD(processAssign_ValidInput_ExistInPkb) {
			std::string input = modifiedVarname + "= " + expression;
			AssignProcessor::processAssign(input, procedureName, statementNumber);
			std::unordered_set<int> setOfAssign = { statementNumber };
			std::unordered_set<std::string> setOfModVar = { modifiedVarname };
			std::unordered_set<std::string> setOfUsedVar = {
				"asdbn2379rthf", "ajkdb134hnc6f", "fsdouih3d4b", "abs", "superduper", "superdupernot"
			};
			std::unordered_map < std::string, std::unordered_set<int>> inputMap = {{postfixExpression, setOfAssign}};
			Assert::IsTrue(PKB::getAllUsesAssignStmts() == setOfAssign);
			Assert::IsTrue(PKB::getVarsUsedInAssign() == setOfUsedVar);
			Assert::IsTrue(PKB::hasAssignUses());
			Assert::IsTrue(PKB::getAssignStmtList() == setOfAssign);
			Assert::IsTrue(PKB::getExpAssignStmtList() == inputMap);
			Assert::IsTrue(PKB::getAllModifiedVars() == setOfModVar);
		}

		TEST_METHOD(processAssign_InvalidExpression_NotExistInPkb) {
			std::string invalidInput = modifiedVarname + " = " + "anyhow - super && epic";
			auto exception = [&] {
				AssignProcessor::processAssign(invalidInput, procedureName, statementNumber);
			};
			Assert::ExpectException<ParserException>(exception);
			Assert::IsTrue(PKB::getAssignStmtList().empty());
		}

		TEST_METHOD(processAssign_InvalidModified_NotExistInPkb) {
			std::string invalidInput = "45987q5t6fgebdhfcvb24 = " + expression;
			auto exception = [&] {
				AssignProcessor::processAssign(invalidInput, procedureName, statementNumber);
			};
			Assert::ExpectException<ParserException>(exception);
			Assert::IsTrue(PKB::getAssignStmtList().empty());
		}

		TEST_METHOD(processAssign_LackOfEqualSign_NotExistInPkb) {
			std::string invalidInput = expression;
			auto exception = [&] {
				AssignProcessor::processAssign(invalidInput, procedureName, statementNumber);
			};
			Assert::ExpectException<ParserException>(exception);
			Assert::IsTrue(PKB::getAssignStmtList().empty());
		}

		TEST_METHOD(processAssign_Empty_NotExistInPkb) {
			std::string invalidInput = "";
				auto exception = [&] {
				AssignProcessor::processAssign(invalidInput, procedureName, statementNumber);
			};
			Assert::ExpectException<ParserException>(exception);
			Assert::IsTrue(PKB::getAssignStmtList().empty());
		}
	};
}
