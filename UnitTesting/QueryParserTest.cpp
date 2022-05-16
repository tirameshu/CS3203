#include "stdafx.h"
#include "CppUnitTest.h"
#include "QueryParserTestData.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{
	TEST_CLASS(QueryParserTest)
	{
	public:
		// valid
		TEST_METHOD(onlySelect_VALID) {
			Query expectedOutput = Query("s", ArgumentTypes::STMT);
			Query resultQuery = QueryParser().parseQuery(onlySelect);

			Assert::IsTrue(expectedOutput.compare(resultQuery));
		}
		TEST_METHOD(onlySelect_multipleDeclarations_VALID) {
			Query expectedOutput = Query("s1", ArgumentTypes::STMT);
			Query resultQuery = QueryParser().parseQuery(onlySelect_multipleDeclarations);

			Assert::IsTrue(expectedOutput.compare(resultQuery));
		}
		TEST_METHOD(onlySelect_multipleDeclarationTypes_VALID) {
			Query expectedOutput = Query("pn", ArgumentTypes::PRINT);
			Query resultQuery = QueryParser().parseQuery(onlySelect_multipleDeclarationTypes);

			Assert::IsTrue(expectedOutput.compare(resultQuery));
		}
		TEST_METHOD(selectProcedure_VALID) {
			Query expectedOutput = Query("p", ArgumentTypes::PROCEDURE);
			Query resultQuery = QueryParser().parseQuery(selectProcedure);

			Assert::IsTrue(expectedOutput.compare(resultQuery));
		}
		TEST_METHOD(selectConstant_VALID) {
			Query expectedOutput = Query("c", ArgumentTypes::CONSTANT);
			Query resultQuery = QueryParser().parseQuery(selectConstant);

			Assert::IsTrue(expectedOutput.compare(resultQuery));
		}
		TEST_METHOD(selectCall_VALID) {
			Query expectedOutput = Query("c", ArgumentTypes::CALL);
			Query resultQuery = QueryParser().parseQuery(selectCall);

			Assert::IsTrue(expectedOutput.compare(resultQuery));
		}
		TEST_METHOD(selectProgLine_VALID) {
			Query expectedOutput = Query("n", ArgumentTypes::PROG_LINE);
			Query resultQuery = QueryParser().parseQuery(selectProgLine);

			Assert::IsTrue(expectedOutput.compare(resultQuery));
		}
		TEST_METHOD(patternPostfixExact_VALID) {
			QueryPatternClause expectedPattern = QueryPatternClause("a", "_", "bc ad +",
				ArgumentTypes::ASSIGN, ArgumentTypes::ANY, ArgumentTypes::STRING, false);
			Query expectedOutput = Query("a", ArgumentTypes::ASSIGN, expectedPattern);
			Query resultQuery = QueryParser().parseQuery(patternPostfixExact);

			Assert::IsTrue(expectedOutput.compare(resultQuery));
		}
		TEST_METHOD(patternPostfix_VALID) {
			QueryPatternClause expectedPattern = QueryPatternClause("a", "_", "bc ad +",
				ArgumentTypes::ASSIGN, ArgumentTypes::ANY, ArgumentTypes::STRING, true);
			Query expectedOutput = Query("a", ArgumentTypes::ASSIGN, expectedPattern);
			Query resultQuery = QueryParser().parseQuery(patternPostfix);

			Assert::IsTrue(expectedOutput.compare(resultQuery));
		}
		TEST_METHOD(extraWhitespace_VALID) {
			Query expectedOutput = Query("abc", ArgumentTypes::STMT);
			Query resultQuery = QueryParser().parseQuery(extraWhitespace);

			Assert::IsTrue(expectedOutput.compare(resultQuery));
		}
		TEST_METHOD(patternExtraWhitespace_VALID) {
			QueryPatternClause expectedPattern = QueryPatternClause("a", "_", "answer",
				ArgumentTypes::ASSIGN, ArgumentTypes::ANY, ArgumentTypes::STRING);
			Query expectedOutput = Query("c", ArgumentTypes::CONSTANT, expectedPattern);
			Query resultQuery = QueryParser().parseQuery(patternExtraWhitespace);

			Assert::IsTrue(expectedOutput.compare(resultQuery));
		}
		TEST_METHOD(patternIdentExprExact_VALID) {
			QueryPatternClause expectedPattern = QueryPatternClause("a", "x", "answer",
				ArgumentTypes::ASSIGN, ArgumentTypes::STRING, ArgumentTypes::STRING);
			expectedPattern.setExact(true);
			Query expectedOutput = Query("a", ArgumentTypes::ASSIGN, expectedPattern);
			Query resultQuery = QueryParser().parseQuery(patternIdentExprExact);

			Assert::IsTrue(expectedOutput.compare(resultQuery));
		}
		TEST_METHOD(patternLeftWhitespace_VALID) {
			QueryPatternClause expectedPattern = QueryPatternClause("a", "_", "answer",
				ArgumentTypes::ASSIGN, ArgumentTypes::ANY, ArgumentTypes::STRING);
			Query expectedOutput = Query("c", ArgumentTypes::CONSTANT, expectedPattern);
			Query resultQuery = QueryParser().parseQuery(patternLeftWhitespace);

			Assert::IsTrue(expectedOutput.compare(resultQuery));
		}
		TEST_METHOD(clauseNamedSelect_VALID) {
			Query expectedOutput = Query("Select", ArgumentTypes::STMT);
			Query resultQuery = QueryParser().parseQuery(clauseNamedSelect);

			Assert::IsTrue(expectedOutput.compare(resultQuery));
		}
		TEST_METHOD(declarationWhitespace_VALID) {
			Query expectedOutput = Query("b", ArgumentTypes::STMT);
			Query resultQuery = QueryParser().parseQuery(declarationWhitespace);

			Assert::IsTrue(expectedOutput.compare(resultQuery));
		}
		TEST_METHOD(selectModifies_VALID) {
			QuerySuchThatClause expectedSuchThat = QuerySuchThatClause(RelRef::MODIFIES_S, "3", "v", ArgumentTypes::INT, ArgumentTypes::VARIABLE);
			Query expectedOutput = Query("v", ArgumentTypes::VARIABLE, expectedSuchThat);
			Query resultQuery = QueryParser().parseQuery(selectModifies);

			Assert::IsTrue(expectedOutput.compare(resultQuery));
		}
		TEST_METHOD(selectUses_VALID) {
			QuerySuchThatClause expectedSuchThat = QuerySuchThatClause(RelRef::USES_S, "ifs", "x", ArgumentTypes::IF, ArgumentTypes::STRING);
			Query expectedOutput = Query("ifs", ArgumentTypes::IF, expectedSuchThat);
			Query resultQuery = QueryParser().parseQuery(selectUses);

			Assert::IsTrue(expectedOutput.compare(resultQuery));
		}
		TEST_METHOD(selectParent_VALID) {
			QuerySuchThatClause expectedSuchThat = QuerySuchThatClause(RelRef::PARENT, "ifs", "s", ArgumentTypes::IF, ArgumentTypes::STMT);
			Query expectedOutput = Query("s", ArgumentTypes::STMT, expectedSuchThat);
			Query resultQuery = QueryParser().parseQuery(selectParent);

			Assert::IsTrue(expectedOutput.compare(resultQuery));
		}
		TEST_METHOD(selectParentT_VALID) {
			QuerySuchThatClause expectedSuchThat = QuerySuchThatClause(RelRef::PARENT_T, "5", "a", ArgumentTypes::INT, ArgumentTypes::ASSIGN);
			Query expectedOutput = Query("a", ArgumentTypes::ASSIGN, expectedSuchThat);
			Query resultQuery = QueryParser().parseQuery(selectParentT);

			Assert::IsTrue(expectedOutput.compare(resultQuery));
		}
		TEST_METHOD(selectFollows_VALID) {
			QuerySuchThatClause expectedSuchThat = QuerySuchThatClause(RelRef::FOLLOWS, "s", "10", ArgumentTypes::STMT, ArgumentTypes::INT);
			Query expectedOutput = Query("s", ArgumentTypes::STMT, expectedSuchThat);
			Query resultQuery = QueryParser().parseQuery(selectFollows);

			Assert::IsTrue(expectedOutput.compare(resultQuery));
		}
		TEST_METHOD(selectFollowsT_VALID) {
			QuerySuchThatClause expectedSuchThat = QuerySuchThatClause(RelRef::FOLLOWS_T, "w", "s", ArgumentTypes::WHILE, ArgumentTypes::STMT);
			Query expectedOutput = Query("w", ArgumentTypes::WHILE, expectedSuchThat);
;			Query resultQuery = QueryParser().parseQuery(selectFollowsT);

			Assert::IsTrue(expectedOutput.compare(resultQuery));
		}
		TEST_METHOD(selectNext_VALID) {
			QuerySuchThatClause expectedSuchThat = QuerySuchThatClause(RelRef::NEXT, "4", "c", ArgumentTypes::INT, ArgumentTypes::CALL);
			Query expectedOutput = Query("c", ArgumentTypes::CALL, expectedSuchThat);
			Query resultQuery = QueryParser().parseQuery(selectNext);

			Assert::IsTrue(expectedOutput.compare(resultQuery));
		}
		TEST_METHOD(selectNextT_VALID) {
			QuerySuchThatClause expectedSuchThat = QuerySuchThatClause(RelRef::NEXT_T, "pn", "4", ArgumentTypes::PRINT, ArgumentTypes::INT);
			Query expectedOutput = Query("pn", ArgumentTypes::PRINT, expectedSuchThat);
			Query resultQuery = QueryParser().parseQuery(selectNextT);

			Assert::IsTrue(expectedOutput.compare(resultQuery));
		}
		TEST_METHOD(selectCalls_VALID) {
			QuerySuchThatClause expectedSuchThat = QuerySuchThatClause(RelRef::CALLS, "p", "q", ArgumentTypes::PROCEDURE, ArgumentTypes::STRING);
			Query expectedOutput = Query("p", ArgumentTypes::PROCEDURE, expectedSuchThat);
			Query resultQuery = QueryParser().parseQuery(selectCalls);

			Assert::IsTrue(expectedOutput.compare(resultQuery));
		}
		TEST_METHOD(selectCallsT_VALID) {
			QuerySuchThatClause expectedSuchThat = QuerySuchThatClause(RelRef::CALLS_T, "Example", "p", ArgumentTypes::STRING, ArgumentTypes::PROCEDURE);
			Query expectedOutput = Query("p", ArgumentTypes::PROCEDURE, expectedSuchThat);
			Query resultQuery = QueryParser().parseQuery(selectCallsT);

			Assert::IsTrue(expectedOutput.compare(resultQuery));
		}
		TEST_METHOD(selectAffects_VALID) {
			QuerySuchThatClause expectedSuchThat = QuerySuchThatClause(RelRef::AFFECTS, "a", "n", ArgumentTypes::ASSIGN, ArgumentTypes::PROG_LINE);
			Query expectedOutput = Query("a", ArgumentTypes::ASSIGN, expectedSuchThat);
			Query resultQuery = QueryParser().parseQuery(selectAffects);

			Assert::IsTrue(expectedOutput.compare(resultQuery));
		}
		TEST_METHOD(selectAffectsT_VALID) {
			QuerySuchThatClause expectedSuchThat = QuerySuchThatClause(RelRef::AFFECTS_T, "a", "n", ArgumentTypes::ASSIGN, ArgumentTypes::PROG_LINE);
			Query expectedOutput = Query("a", ArgumentTypes::ASSIGN, expectedSuchThat);
			Query resultQuery = QueryParser().parseQuery(selectAffectsT);

			Assert::IsTrue(expectedOutput.compare(resultQuery));
		}
		TEST_METHOD(selectSuchThatPattern_VALID) {
			QuerySuchThatClause expectedSuchThat = QuerySuchThatClause(RelRef::FOLLOWS, "s1", "2", ArgumentTypes::STMT, ArgumentTypes::INT);
			QueryPatternClause expectedPattern = QueryPatternClause("a", "x", "_", ArgumentTypes::ASSIGN, ArgumentTypes::STRING, ArgumentTypes::ANY);
			Query expectedOutput = Query("s", ArgumentTypes::STMT, expectedSuchThat, expectedPattern);
			Query resultQuery = QueryParser().parseQuery(selectSuchThatPattern);

			Assert::IsTrue(expectedOutput.compare(resultQuery));
		}
		TEST_METHOD(multipleDeclarationsSameType_VALID) {
			Query expectedOutput = Query("s4", ArgumentTypes::STMT);
			Query resultQuery = QueryParser().parseQuery(multipleDeclarationsSameType);

			Assert::IsTrue(expectedOutput.compare(resultQuery));
		}
		TEST_METHOD(selectPatternOnly_VALID) {
			QueryPatternClause expectedPattern = QueryPatternClause("a", "v", "_", ArgumentTypes::ASSIGN, ArgumentTypes::VARIABLE, ArgumentTypes::ANY);
			Query expectedOutput = Query("v", ArgumentTypes::VARIABLE, expectedPattern);
			Query resultQuery = QueryParser().parseQuery(selectPatternOnly);

			Assert::IsTrue(expectedOutput.compare(resultQuery));
		}
		TEST_METHOD(modifiesRead_VALID) {
			QuerySuchThatClause expectedSuchThat = QuerySuchThatClause(RelRef::MODIFIES_S, "r", "v", ArgumentTypes::READ, ArgumentTypes::VARIABLE);
			Query expectedOutput = Query("r", ArgumentTypes::READ, expectedSuchThat);
			Query resultQuery = QueryParser().parseQuery(modifiesRead);

			Assert::IsTrue(expectedOutput.compare(resultQuery));
		}
		TEST_METHOD(usesPrint_VALID) {
			QuerySuchThatClause expectedSuchThat = QuerySuchThatClause(RelRef::USES_S, "pn", "v", ArgumentTypes::PRINT, ArgumentTypes::VARIABLE);
			Query expectedOutput = Query("pn", ArgumentTypes::PRINT, expectedSuchThat);
			Query resultQuery = QueryParser().parseQuery(usesPrint);

			Assert::IsTrue(expectedOutput.compare(resultQuery));
		}
		TEST_METHOD(patternUnderscore_VALID) {
			QueryPatternClause expectedPattern = QueryPatternClause("a", "_", "answer", ArgumentTypes::ASSIGN, ArgumentTypes::ANY, ArgumentTypes::STRING);
			Query expectedOutput = Query("a", ArgumentTypes::ASSIGN, expectedPattern);
			Query resultQuery = QueryParser().parseQuery(patternUnderscore);

			Assert::IsTrue(expectedOutput.compare(resultQuery));
		}
		TEST_METHOD(patternUnderscoreWhitespace_VALID) {
			QueryPatternClause expectedPattern = QueryPatternClause("a", "_", "answer", ArgumentTypes::ASSIGN, ArgumentTypes::ANY, ArgumentTypes::STRING);
			Query expectedOutput = Query("a", ArgumentTypes::ASSIGN, expectedPattern);
			Query resultQuery = QueryParser().parseQuery(patternUnderscoreWhitespace);

			Assert::IsTrue(expectedOutput.compare(resultQuery));
		}
		TEST_METHOD(multipleDeclarationsCapitalize_VALID) {
			Query expectedOutput = Query("S", ArgumentTypes::STMT);
			Query resultQuery = QueryParser().parseQuery(multipleDeclarationsCapitalize);

			Assert::IsTrue(expectedOutput.compare(resultQuery));
		}
		TEST_METHOD(selectUsesInt_VALID) {
			QuerySuchThatClause expectedSuchThat = QuerySuchThatClause(RelRef::USES_S, "5", "v", ArgumentTypes::INT, ArgumentTypes::VARIABLE);
			Query expectedOutput = Query("s", ArgumentTypes::STMT, expectedSuchThat);
			Query resultQuery = QueryParser().parseQuery(selectUsesInt);

			Assert::IsTrue(expectedOutput.compare(resultQuery));
		}
		TEST_METHOD(suchThatPatternWhitespace_VALID) {
			QuerySuchThatClause expectedSuchThat = QuerySuchThatClause(RelRef::PARENT, "_", "_", ArgumentTypes::ANY, ArgumentTypes::ANY);
			QueryPatternClause expectedPattern = QueryPatternClause("Parent", "_", "200", ArgumentTypes::ASSIGN, ArgumentTypes::ANY, ArgumentTypes::STRING);
			Query expectedOutput = Query("Pattern", ArgumentTypes::ASSIGN, expectedSuchThat, expectedPattern);
			Query resultQuery = QueryParser().parseQuery(suchThatPatternWhitespace);

			Assert::IsTrue(expectedOutput.compare(resultQuery));
		}
		TEST_METHOD(patternZero_VALID) {
			QuerySuchThatClause expectedSuchThat = QuerySuchThatClause(RelRef::PARENT_T, "40", "a", ArgumentTypes::INT, ArgumentTypes::ASSIGN);
			QueryPatternClause expectedPattern = QueryPatternClause("a", "v", "0", ArgumentTypes::ASSIGN, ArgumentTypes::VARIABLE, ArgumentTypes::INT);
			Query expectedOutput = Query("v", ArgumentTypes::VARIABLE, expectedSuchThat, expectedPattern);
			Query resultQuery = QueryParser().parseQuery(patternZero);

			Assert::IsTrue(expectedOutput.compare(resultQuery));
		}

		TEST_METHOD(withPrintProcName_VALID) {
			bool isBooleanResult = false;
			std::vector<QuerySuchThatClause> suchThatClauses = {};
			std::vector<QueryPatternClause> patternClauses = {};
			std::vector<QueryWithClause> withClauses = {};

			QueryWithClause expectedWithClause = QueryWithClause("pn", "a", ArgumentTypes::PRINT, ArgumentTypes::STRING, AttrName::VAR_NAME, AttrName::NONE);
			withClauses.push_back(expectedWithClause);

			std::vector<std::string> resultSynonyms = { "pn" };
			std::vector<ArgumentTypes> resultTypes = { ArgumentTypes::PRINT };
			std::vector<AttrName> resultAttrNames = { AttrName::NONE };
			Query expectedOutput = Query(resultSynonyms, resultTypes, resultAttrNames,
				suchThatClauses, patternClauses, withClauses, isBooleanResult);
			Query resultQuery = QueryParser().parseQuery(withPrintProcName);

			Assert::IsTrue(expectedOutput.compare(resultQuery));
		}

		TEST_METHOD(withReadProcName_VALID) {
			bool isBooleanResult = false;
			std::vector<QuerySuchThatClause> suchThatClauses = {};
			std::vector<QueryPatternClause> patternClauses = {};
			std::vector<QueryWithClause> withClauses = {};

			QueryWithClause expectedWithClause = QueryWithClause("r", "v", ArgumentTypes::READ, ArgumentTypes::VARIABLE, AttrName::VAR_NAME, AttrName::VAR_NAME);
			withClauses.push_back(expectedWithClause);

			std::vector<std::string> resultSynonyms = { "v" };
			std::vector<ArgumentTypes> resultTypes = { ArgumentTypes::VARIABLE };
			std::vector<AttrName> resultAttrNames = { AttrName::NONE };
			Query expectedOutput = Query(resultSynonyms, resultTypes, resultAttrNames,
				suchThatClauses, patternClauses, withClauses, isBooleanResult);
			Query resultQuery = QueryParser().parseQuery(withReadProcName);

			Assert::IsTrue(expectedOutput.compare(resultQuery));
		}

		TEST_METHOD(withStmtNumber_VALID) {
			bool isBooleanResult = false;
			std::vector<QuerySuchThatClause> suchThatClauses = {};
			std::vector<QueryPatternClause> patternClauses = {};
			std::vector<QueryWithClause> withClauses = {};

			QueryWithClause expectedWithClause = QueryWithClause("s", "20", ArgumentTypes::STMT, ArgumentTypes::INT, AttrName::STMT_NUM, AttrName::NONE);
			withClauses.push_back(expectedWithClause);

			std::vector<std::string> resultSynonyms = { "BOOLEAN" };
			std::vector<ArgumentTypes> resultTypes = { ArgumentTypes::BOOLEAN };
			std::vector<AttrName> resultAttrNames = { AttrName::NONE };
			Query expectedOutput = Query(resultSynonyms, resultTypes, resultAttrNames,
				suchThatClauses, patternClauses, withClauses, isBooleanResult);
			Query resultQuery = QueryParser().parseQuery(withStmtNumber);

			Assert::IsTrue(expectedOutput.compare(resultQuery));
		}

		TEST_METHOD(withConstantValue_VALID) {
			bool isBooleanResult = false;
			std::vector<QuerySuchThatClause> suchThatClauses = {};
			std::vector<QueryPatternClause> patternClauses = {};
			std::vector<QueryWithClause> withClauses = {};

			QueryWithClause expectedWithClause = QueryWithClause("c", "5", ArgumentTypes::CONSTANT, ArgumentTypes::INT, AttrName::VALUE, AttrName::NONE);
			withClauses.push_back(expectedWithClause);

			std::vector<std::string> resultSynonyms = { "s" };
			std::vector<ArgumentTypes> resultTypes = { ArgumentTypes::STMT };
			std::vector<AttrName> resultAttrNames = { AttrName::NONE };
			Query expectedOutput = Query(resultSynonyms, resultTypes, resultAttrNames,
				suchThatClauses, patternClauses, withClauses, isBooleanResult);
			Query resultQuery = QueryParser().parseQuery(withConstantValue);

			Assert::IsTrue(expectedOutput.compare(resultQuery));
		}

		TEST_METHOD(withVarProcName_VALID) {
			bool isBooleanResult = false;
			std::vector<QuerySuchThatClause> suchThatClauses = {};
			std::vector<QueryPatternClause> patternClauses = {};
			std::vector<QueryWithClause> withClauses = {};

			std::vector<std::string> resultSynonyms = { "v" };
			std::vector<ArgumentTypes> resultTypes = { ArgumentTypes::VARIABLE };
			std::vector<AttrName> resultAttrNames = { AttrName::VAR_NAME };
			Query expectedOutput = Query(resultSynonyms, resultTypes, resultAttrNames,
				suchThatClauses, patternClauses, withClauses, isBooleanResult);
			Query resultQuery = QueryParser().parseQuery(withVarProcName);

			Assert::IsTrue(expectedOutput.compare(resultQuery));
		}
		TEST_METHOD(withCallProcName_VALID) {
			bool isBooleanResult = false;
			std::vector<QuerySuchThatClause> suchThatClauses = {};
			std::vector<QueryPatternClause> patternClauses = {};
			std::vector<QueryWithClause> withClauses = {};

			QueryWithClause expectedWithClause = QueryWithClause("p", "c", ArgumentTypes::PROCEDURE, ArgumentTypes::CALL, AttrName::PROC_NAME, AttrName::PROC_NAME);
			withClauses.push_back(expectedWithClause);

			std::vector<std::string> resultSynonyms = {"p"};
			std::vector<ArgumentTypes> resultTypes = {ArgumentTypes::PROCEDURE};
			std::vector<AttrName> resultAttrNames = {AttrName::NONE};
			Query expectedOutput = Query(resultSynonyms, resultTypes, resultAttrNames,
				suchThatClauses, patternClauses, withClauses, isBooleanResult);
			Query resultQuery = QueryParser().parseQuery(withCallProcName);

			Assert::IsTrue(expectedOutput.compare(resultQuery));
		}

		TEST_METHOD(withProcProcName_VALID) {
			bool isBooleanResult = false;
			std::vector<QuerySuchThatClause> suchThatClauses = {};
			std::vector<QueryPatternClause> patternClauses = {};
			std::vector<QueryWithClause> withClauses = {};

			QueryWithClause expectedWithClause = QueryWithClause("p", "procedure", ArgumentTypes::PROCEDURE, ArgumentTypes::STRING, AttrName::PROC_NAME, AttrName::NONE);
			withClauses.push_back(expectedWithClause);

			std::vector<std::string> resultSynonyms = { "p" };
			std::vector<ArgumentTypes> resultTypes = { ArgumentTypes::PROCEDURE };
			std::vector<AttrName> resultAttrNames = { AttrName::NONE };
			Query expectedOutput = Query(resultSynonyms, resultTypes, resultAttrNames,
				suchThatClauses, patternClauses, withClauses, isBooleanResult);
			Query resultQuery = QueryParser().parseQuery(withProcProcName);

			Assert::IsTrue(expectedOutput.compare(resultQuery));
		}

		TEST_METHOD(withVar_VALID) {
			bool isBooleanResult = false;
			std::vector<QuerySuchThatClause> suchThatClauses = {};
			std::vector<QueryPatternClause> patternClauses = {};
			std::vector<QueryWithClause> withClauses = {};

			QueryWithClause expectedWithClause = QueryWithClause("v", "variable", ArgumentTypes::VARIABLE, ArgumentTypes::STRING, AttrName::VAR_NAME, AttrName::NONE);
			withClauses.push_back(expectedWithClause);

			QuerySuchThatClause expectedSuchThatClause = QuerySuchThatClause(RelRef::USES_P, "p", "v", ArgumentTypes::PROCEDURE, ArgumentTypes::VARIABLE);
			suchThatClauses.push_back(expectedSuchThatClause);

			std::vector<std::string> resultSynonyms = { "p" };
			std::vector<ArgumentTypes> resultTypes = { ArgumentTypes::PROCEDURE };
			std::vector<AttrName> resultAttrNames = { AttrName::NONE };
			Query expectedOutput = Query(resultSynonyms, resultTypes, resultAttrNames,
				suchThatClauses, patternClauses, withClauses, isBooleanResult);
			Query resultQuery = QueryParser().parseQuery(withVar);

			Assert::IsTrue(expectedOutput.compare(resultQuery));
		}

		TEST_METHOD(withModifies_VALID) {
			bool isBooleanResult = false;
			std::vector<QuerySuchThatClause> suchThatClauses = {};
			std::vector<QueryPatternClause> patternClauses = {};
			std::vector<QueryWithClause> withClauses = {};

			QueryWithClause expectedWithClause = QueryWithClause("v", "x", ArgumentTypes::VARIABLE, ArgumentTypes::STRING, AttrName::VAR_NAME, AttrName::NONE);
			withClauses.push_back(expectedWithClause);

			QuerySuchThatClause expectedSuchThatClause = QuerySuchThatClause(RelRef::MODIFIES_S, "r", "v", ArgumentTypes::READ, ArgumentTypes::VARIABLE);
			suchThatClauses.push_back(expectedSuchThatClause);

			std::vector<std::string> resultSynonyms = { "r" };
			std::vector<ArgumentTypes> resultTypes = { ArgumentTypes::READ };
			std::vector<AttrName> resultAttrNames = { AttrName::STMT_NUM };
			Query expectedOutput = Query(resultSynonyms, resultTypes, resultAttrNames,
				suchThatClauses, patternClauses, withClauses, isBooleanResult);
			Query resultQuery = QueryParser().parseQuery(withModifies);

			Assert::IsTrue(expectedOutput.compare(resultQuery));
		}

		TEST_METHOD(selectProcName_VALID) {
			bool isBooleanResult = false;
			std::vector<QuerySuchThatClause> suchThatClauses = {};
			std::vector<QueryPatternClause> patternClauses = {};
			std::vector<QueryWithClause> withClauses = {};

			QuerySuchThatClause expectedSuchThatClause = QuerySuchThatClause(RelRef::CALLS, "p", "q", ArgumentTypes::PROCEDURE, ArgumentTypes::PROCEDURE);
			suchThatClauses.push_back(expectedSuchThatClause);

			std::vector<std::string> resultSynonyms = { "p" };
			std::vector<ArgumentTypes> resultTypes = { ArgumentTypes::PROCEDURE };
			std::vector<AttrName> resultAttrNames = { AttrName::PROC_NAME };
			Query expectedOutput = Query(resultSynonyms, resultTypes, resultAttrNames,
				suchThatClauses, patternClauses, withClauses, isBooleanResult);
			Query resultQuery = QueryParser().parseQuery(selectProcName);

			Assert::IsTrue(expectedOutput.compare(resultQuery));
		}

		TEST_METHOD(andSuchThat_VALID) {
			bool isBooleanResult = false;
			std::vector<QuerySuchThatClause> suchThatClauses = {};
			std::vector<QueryPatternClause> patternClauses = {};
			std::vector<QueryWithClause> withClauses = {};

			QuerySuchThatClause expectedSuchThatClause = QuerySuchThatClause(RelRef::MODIFIES_S, "a", "x", ArgumentTypes::ASSIGN, ArgumentTypes::STRING);
			suchThatClauses.push_back(expectedSuchThatClause);

			QuerySuchThatClause expectedSuchThatClause1 = QuerySuchThatClause(RelRef::PARENT_T, "w", "a", ArgumentTypes::WHILE, ArgumentTypes::ASSIGN);
			suchThatClauses.push_back(expectedSuchThatClause1);

			QuerySuchThatClause expectedSuchThatClause2 = QuerySuchThatClause(RelRef::NEXT_T, "1", "a", ArgumentTypes::INT, ArgumentTypes::ASSIGN);
			suchThatClauses.push_back(expectedSuchThatClause2);

			std::vector<std::string> resultSynonyms = { "a" };
			std::vector<ArgumentTypes> resultTypes = { ArgumentTypes::ASSIGN };
			std::vector<AttrName> resultAttrNames = { AttrName::NONE };
			Query expectedOutput = Query(resultSynonyms, resultTypes, resultAttrNames,
				suchThatClauses, patternClauses, withClauses, isBooleanResult);
			Query resultQuery = QueryParser().parseQuery(andSuchThat);

			Assert::IsTrue(expectedOutput.compare(resultQuery));
		}

		TEST_METHOD(andPattern_VALID) {
			bool isBooleanResult = false;
			std::vector<QuerySuchThatClause> suchThatClauses = {};
			std::vector<QueryPatternClause> patternClauses = {};
			std::vector<QueryWithClause> withClauses = {};

			QueryPatternClause expectedPatternClause1 = QueryPatternClause("ifs", "v", "_", ArgumentTypes::IF, ArgumentTypes::VARIABLE, ArgumentTypes::ANY, true);
			patternClauses.push_back(expectedPatternClause1);
			QueryPatternClause expectedPatternClause2 = QueryPatternClause("a", "_", "v", ArgumentTypes::ASSIGN, ArgumentTypes::ANY, ArgumentTypes::VARIABLE, true);
			patternClauses.push_back(expectedPatternClause2);
			QueryPatternClause expectedPatternClause3 = QueryPatternClause("w", "v", "_", ArgumentTypes::WHILE, ArgumentTypes::VARIABLE, ArgumentTypes::ANY, true);
			patternClauses.push_back(expectedPatternClause3);

			std::vector<std::string> resultSynonyms = { "v" };
			std::vector<ArgumentTypes> resultTypes = { ArgumentTypes::VARIABLE };
			std::vector<AttrName> resultAttrNames = { AttrName::NONE };
			Query expectedOutput = Query(resultSynonyms, resultTypes, resultAttrNames,
				suchThatClauses, patternClauses, withClauses, isBooleanResult);
			Query resultQuery = QueryParser().parseQuery(andPattern);

			Assert::IsTrue(expectedOutput.compare(resultQuery));
		}

		TEST_METHOD(andWith_VALID) {
			bool isBooleanResult = false;
			std::vector<QuerySuchThatClause> suchThatClauses = {};
			std::vector<QueryPatternClause> patternClauses = {};
			std::vector<QueryWithClause> withClauses = {};

			QuerySuchThatClause expectedSuchThatClause = QuerySuchThatClause(RelRef::NEXT_T, "c1", "n", ArgumentTypes::CONSTANT, ArgumentTypes::PROG_LINE);
			suchThatClauses.push_back(expectedSuchThatClause);

			QuerySuchThatClause expectedSuchThatClause1 = QuerySuchThatClause(RelRef::NEXT_T, "n", "c2", ArgumentTypes::PROG_LINE, ArgumentTypes::CONSTANT);
			suchThatClauses.push_back(expectedSuchThatClause1);

			QueryWithClause expectedWithClause = QueryWithClause("c1", "5", ArgumentTypes::CONSTANT, ArgumentTypes::INT, AttrName::VALUE, AttrName::NONE);
			withClauses.push_back(expectedWithClause);

			QueryWithClause expectedWithClause1 = QueryWithClause("c2", "12", ArgumentTypes::CONSTANT, ArgumentTypes::INT, AttrName::VALUE, AttrName::NONE);
			withClauses.push_back(expectedWithClause1);

			std::vector<std::string> resultSynonyms = { "n" };
			std::vector<ArgumentTypes> resultTypes = { ArgumentTypes::PROG_LINE };
			std::vector<AttrName> resultAttrNames = { AttrName::NONE };
			Query expectedOutput = Query(resultSynonyms, resultTypes, resultAttrNames,
				suchThatClauses, patternClauses, withClauses, isBooleanResult);
			Query resultQuery = QueryParser().parseQuery(andWith);

			Assert::IsTrue(expectedOutput.compare(resultQuery));
		}

		TEST_METHOD(combi1_VALID) {
			bool isBooleanResult = false;
			std::vector<QuerySuchThatClause> suchThatClauses = {};
			std::vector<QueryPatternClause> patternClauses = {};
			std::vector<QueryWithClause> withClauses = {};

			QuerySuchThatClause expectedSuchThatClause = QuerySuchThatClause(RelRef::CALLS, "Second", "p", ArgumentTypes::STRING, ArgumentTypes::PROCEDURE);
			suchThatClauses.push_back(expectedSuchThatClause);

			QuerySuchThatClause expectedSuchThatClause1 = QuerySuchThatClause(RelRef::PARENT, "w", "c", ArgumentTypes::WHILE, ArgumentTypes::CALL);
			suchThatClauses.push_back(expectedSuchThatClause1);

			QueryWithClause expectedWithClause = QueryWithClause("c", "p", ArgumentTypes::CALL, ArgumentTypes::PROCEDURE, AttrName::PROC_NAME, AttrName::PROC_NAME);
			withClauses.push_back(expectedWithClause);

			QueryWithClause expectedWithClause1 = QueryWithClause("c2", "12", ArgumentTypes::CONSTANT, ArgumentTypes::INT, AttrName::VALUE, AttrName::NONE);
			withClauses.push_back(expectedWithClause1);

			std::vector<std::string> resultSynonyms = { "p" };
			std::vector<ArgumentTypes> resultTypes = { ArgumentTypes::PROCEDURE };
			std::vector<AttrName> resultAttrNames = { AttrName::NONE };
			Query expectedOutput = Query(resultSynonyms, resultTypes, resultAttrNames,
				suchThatClauses, patternClauses, withClauses, isBooleanResult);
			Query resultQuery = QueryParser().parseQuery(combi1);

			Assert::IsTrue(expectedOutput.compare(resultQuery));
		}

		TEST_METHOD(combi2_VALID) {
			bool isBooleanResult = false;
			std::vector<QuerySuchThatClause> suchThatClauses = {};
			std::vector<QueryPatternClause> patternClauses = {};
			std::vector<QueryWithClause> withClauses = {};

			QuerySuchThatClause expectedSuchThatClause = QuerySuchThatClause(RelRef::FOLLOWS_T, "s", "s1", ArgumentTypes::STMT, ArgumentTypes::STMT);
			suchThatClauses.push_back(expectedSuchThatClause);

			QueryWithClause expectedWithClause = QueryWithClause("s1", "n", ArgumentTypes::STMT, ArgumentTypes::PROG_LINE, AttrName::STMT_NUM, AttrName::NONE);
			withClauses.push_back(expectedWithClause);

			QueryWithClause expectedWithClause1 = QueryWithClause("n", "10", ArgumentTypes::PROG_LINE, ArgumentTypes::INT, AttrName::NONE, AttrName::NONE);
			withClauses.push_back(expectedWithClause1);

			std::vector<std::string> resultSynonyms = { "s" };
			std::vector<ArgumentTypes> resultTypes = { ArgumentTypes::STMT };
			std::vector<AttrName> resultAttrNames = { AttrName::STMT_NUM };
			Query expectedOutput = Query(resultSynonyms, resultTypes, resultAttrNames,
				suchThatClauses, patternClauses, withClauses, isBooleanResult);
			Query resultQuery = QueryParser().parseQuery(combi2);

			Assert::IsTrue(expectedOutput.compare(resultQuery));
		}

		TEST_METHOD(combi3_VALID) {
			bool isBooleanResult = false;
			std::vector<QuerySuchThatClause> suchThatClauses = {};
			std::vector<QueryPatternClause> patternClauses = {};
			std::vector<QueryWithClause> withClauses = {};

			QuerySuchThatClause expectedSuchThatClause = QuerySuchThatClause(RelRef::PARENT_T, "w1", "w2", ArgumentTypes::WHILE, ArgumentTypes::WHILE);
			suchThatClauses.push_back(expectedSuchThatClause);

			QuerySuchThatClause expectedSuchThatClause1 = QuerySuchThatClause(RelRef::PARENT_T, "w2", "w3", ArgumentTypes::WHILE, ArgumentTypes::WHILE);
			suchThatClauses.push_back(expectedSuchThatClause1);

			std::vector<std::string> resultSynonyms = { "w1", "w2", "w3" };
			std::vector<ArgumentTypes> resultTypes = { ArgumentTypes::WHILE, ArgumentTypes::WHILE, ArgumentTypes::WHILE };
			std::vector<AttrName> resultAttrNames = { AttrName::NONE, AttrName::NONE, AttrName::NONE };
			Query expectedOutput = Query(resultSynonyms, resultTypes, resultAttrNames,
				suchThatClauses, patternClauses, withClauses, isBooleanResult);
			Query resultQuery = QueryParser().parseQuery(combi3);

			Assert::IsTrue(expectedOutput.compare(resultQuery));
		}

		TEST_METHOD(combi4_VALID) {
			bool isBooleanResult = false;
			std::vector<QuerySuchThatClause> suchThatClauses = {};
			std::vector<QueryPatternClause> patternClauses = {};
			std::vector<QueryWithClause> withClauses = {};

			QuerySuchThatClause expectedSuchThatClause = QuerySuchThatClause(RelRef::PARENT_T, "w", "a", ArgumentTypes::WHILE, ArgumentTypes::ASSIGN);
			suchThatClauses.push_back(expectedSuchThatClause);

			QuerySuchThatClause expectedSuchThatClause1 = QuerySuchThatClause(RelRef::NEXT_T, "60", "n", ArgumentTypes::INT, ArgumentTypes::PROG_LINE);
			suchThatClauses.push_back(expectedSuchThatClause1);

			QueryPatternClause expectedPatternClause = QueryPatternClause("a", "x", "_", ArgumentTypes::ASSIGN, ArgumentTypes::STRING, ArgumentTypes::ANY, true);
			patternClauses.push_back(expectedPatternClause);

			QueryWithClause expectedWithClause = QueryWithClause("a", "n", ArgumentTypes::ASSIGN, ArgumentTypes::PROG_LINE, AttrName::STMT_NUM, AttrName::NONE);
			withClauses.push_back(expectedWithClause);

			std::vector<std::string> resultSynonyms = { "a" };
			std::vector<ArgumentTypes> resultTypes = { ArgumentTypes::ASSIGN };
			std::vector<AttrName> resultAttrNames = { AttrName::NONE };
			Query expectedOutput = Query(resultSynonyms, resultTypes, resultAttrNames,
				suchThatClauses, patternClauses, withClauses, isBooleanResult);
			Query resultQuery = QueryParser().parseQuery(combi4);

			Assert::IsTrue(expectedOutput.compare(resultQuery));
		}

		TEST_METHOD(boolean1_VALID) {
			bool isBooleanResult = true;
			std::vector<QuerySuchThatClause> suchThatClauses = {};
			std::vector<QueryPatternClause> patternClauses = {};
			std::vector<QueryWithClause> withClauses = {};

			QuerySuchThatClause expectedSuchThatClause = QuerySuchThatClause(RelRef::NEXT_T, "2", "9", ArgumentTypes::INT, ArgumentTypes::INT);
			suchThatClauses.push_back(expectedSuchThatClause);

			std::vector<std::string> resultSynonyms = { "BOOLEAN" };
			std::vector<ArgumentTypes> resultTypes = { ArgumentTypes::BOOLEAN };
			std::vector<AttrName> resultAttrNames = { AttrName::NONE };
			Query expectedOutput = Query(resultSynonyms, resultTypes, resultAttrNames,
				suchThatClauses, patternClauses, withClauses, isBooleanResult);
			Query resultQuery = QueryParser().parseQuery(boolean1);

			Assert::IsTrue(expectedOutput.compare(resultQuery));
		}

		TEST_METHOD(boolean2_VALID) {
			bool isBooleanResult = true;
			std::vector<QuerySuchThatClause> suchThatClauses = {};
			std::vector<QueryPatternClause> patternClauses = {};
			std::vector<QueryWithClause> withClauses = {};

			QueryWithClause expectedWithClause = QueryWithClause("12", "12", ArgumentTypes::INT, ArgumentTypes::INT, AttrName::NONE, AttrName::NONE);
			withClauses.push_back(expectedWithClause);

			std::vector<std::string> resultSynonyms = { "BOOLEAN" };
			std::vector<ArgumentTypes> resultTypes = { ArgumentTypes::BOOLEAN };
			std::vector<AttrName> resultAttrNames = { AttrName::NONE };
			Query expectedOutput = Query(resultSynonyms, resultTypes, resultAttrNames,
				suchThatClauses, patternClauses, withClauses, isBooleanResult);
			Query resultQuery = QueryParser().parseQuery(boolean2);

			Assert::IsTrue(expectedOutput.compare(resultQuery));
		}

		TEST_METHOD(boolean3_VALID) {
			bool isBooleanResult = true;
			std::vector<QuerySuchThatClause> suchThatClauses = {};
			std::vector<QueryPatternClause> patternClauses = {};
			std::vector<QueryWithClause> withClauses = {};

			QueryWithClause expectedWithClause = QueryWithClause("a", "12", ArgumentTypes::ASSIGN, ArgumentTypes::INT, AttrName::STMT_NUM, AttrName::NONE);
			withClauses.push_back(expectedWithClause);

			std::vector<std::string> resultSynonyms = { "BOOLEAN" };
			std::vector<ArgumentTypes> resultTypes = { ArgumentTypes::BOOLEAN };
			std::vector<AttrName> resultAttrNames = { AttrName::NONE };
			Query expectedOutput = Query(resultSynonyms, resultTypes, resultAttrNames,
				suchThatClauses, patternClauses, withClauses, isBooleanResult);
			Query resultQuery = QueryParser().parseQuery(boolean3);

			Assert::IsTrue(expectedOutput.compare(resultQuery));
		}

		TEST_METHOD(tuple1_VALID) {
			bool isBooleanResult = false;
			std::vector<QuerySuchThatClause> suchThatClauses = {};
			std::vector<QueryPatternClause> patternClauses = {};
			std::vector<QueryWithClause> withClauses = {};

			QuerySuchThatClause expectedSuchThatClause = QuerySuchThatClause(RelRef::CALLS, "p", "q", ArgumentTypes::PROCEDURE, ArgumentTypes::PROCEDURE);
			suchThatClauses.push_back(expectedSuchThatClause);

			std::vector<std::string> resultSynonyms = { "p", "q" };
			std::vector<ArgumentTypes> resultTypes = { ArgumentTypes::PROCEDURE, ArgumentTypes::PROCEDURE };
			std::vector<AttrName> resultAttrNames = { AttrName::NONE, AttrName::NONE };
			Query expectedOutput = Query(resultSynonyms, resultTypes, resultAttrNames,
				suchThatClauses, patternClauses, withClauses, isBooleanResult);
			Query resultQuery = QueryParser().parseQuery(tuple1);

			Assert::IsTrue(expectedOutput.compare(resultQuery));
		}

		// invalid
		TEST_METHOD(synonymNameDoesNotExist_INVALID) {
			Query expectedOutput = Query();
			Query resultQuery = QueryParser().parseQuery(synonymNameDoesNotExist);

			Assert::IsTrue(expectedOutput.compare(resultQuery));
		}
		TEST_METHOD(invalidDesignEntity_INVALID) {
			Query expectedOutput = Query();
			Query resultQuery = QueryParser().parseQuery(invalidDesignEntity);

			Assert::IsTrue(expectedOutput.compare(resultQuery));
		}
		TEST_METHOD(integerStartingZero_INVALID) {
			Query expectedOutput = Query();
			Query resultQuery = QueryParser().parseQuery(integerStartingZero);

			Assert::IsTrue(expectedOutput.compare(resultQuery));
		}
		TEST_METHOD(patternSpecialChars_INVALID) {
			Query expectedOutput = Query();
			Query resultQuery = QueryParser().parseQuery(patternSpecialChars);

			Assert::IsTrue(expectedOutput.compare(resultQuery));
		}
		TEST_METHOD(patternEmptyArgs_INVALID) {
			Query expectedOutput = Query();
			Query resultQuery = QueryParser().parseQuery(patternEmptyArgs);

			Assert::IsTrue(expectedOutput.compare(resultQuery));
		}
		TEST_METHOD(patternStartZeroNumber_INVALID) {
			Query expectedOutput = Query();
			Query resultQuery = QueryParser().parseQuery(patternStartZeroNumber);

			Assert::IsTrue(expectedOutput.compare(resultQuery));
		}
		TEST_METHOD(patternStartZeroName_INVALID) {
			Query expectedOutput = Query();
			Query resultQuery = QueryParser().parseQuery(patternStartZeroName);

			Assert::IsTrue(expectedOutput.compare(resultQuery));
		}
		TEST_METHOD(repeatedNames_INVALID) {
			Query expectedOutput = Query(false);
			Query resultQuery = QueryParser().parseQuery(repeatedNames);

			Assert::IsTrue(expectedOutput.compare(resultQuery));
		}
		TEST_METHOD(extraSemicolonDeclaration_INVALID) {
			Query expectedOutput = Query();
			Query resultQuery = QueryParser().parseQuery(extraSemicolonDeclaration);

			Assert::IsTrue(expectedOutput.compare(resultQuery));
		}
		TEST_METHOD(extraSemicolonSelection_INVALID) {
			Query expectedOutput = Query();
			Query resultQuery = QueryParser().parseQuery(extraSemicolonSelection);

			Assert::IsTrue(expectedOutput.compare(resultQuery));
		}
		TEST_METHOD(noSelectClause_INVALID) {
			Query expectedOutput = Query();
			Query resultQuery = QueryParser().parseQuery(noSelectClause);

			Assert::IsTrue(expectedOutput.compare(resultQuery));
		}
		TEST_METHOD(selectWithNoEntity_INVALID) {
			Query expectedOutput = Query();
			Query resultQuery = QueryParser().parseQuery(selectWithNoEntity);

			Assert::IsTrue(expectedOutput.compare(resultQuery));
		}
		TEST_METHOD(noDeclarations_INVALID) {
			Query expectedOutput = Query();
			Query resultQuery = QueryParser().parseQuery(noDeclarations);

			Assert::IsTrue(expectedOutput.compare(resultQuery));
		}
		TEST_METHOD(invalidParentT_INVALID) {
			Query expectedOutput = Query();
			Query resultQuery = QueryParser().parseQuery(invalidParentT);

			Assert::IsTrue(expectedOutput.compare(resultQuery));
		}
		TEST_METHOD(selectPatternNotVariable_INVALID) {
			Query expectedOutput = Query(false);
			Query resultQuery = QueryParser().parseQuery(selectPatternNotVariable);

			Assert::IsTrue(expectedOutput.compare(resultQuery));
		}
		TEST_METHOD(modifiesPrint_INVALID) {
			Query expectedOutput = Query(false);
			Query resultQuery = QueryParser().parseQuery(modifiesPrint);

			Assert::IsTrue(expectedOutput.compare(resultQuery));
		}
		TEST_METHOD(usesRead_INVALID) {
			Query expectedOutput = Query(false);
			Query resultQuery = QueryParser().parseQuery(usesRead);

			Assert::IsTrue(expectedOutput.compare(resultQuery));
		}
		TEST_METHOD(parentAssign_INVALID) {
			Query expectedOutput = Query(false);
			Query resultQuery = QueryParser().parseQuery(parentAssign);

			Assert::IsTrue(expectedOutput.compare(resultQuery));
		}
		TEST_METHOD(parentTAssign_INVALID) {
			Query expectedOutput = Query(false);
			Query resultQuery = QueryParser().parseQuery(parentTAssign);

			Assert::IsTrue(expectedOutput.compare(resultQuery));
		}
		TEST_METHOD(followsVariable_INVALID) {
			Query expectedOutput = Query(false);
			Query resultQuery = QueryParser().parseQuery(followsVariable);

			Assert::IsTrue(expectedOutput.compare(resultQuery));
		}
		TEST_METHOD(followsTVariable_INVALID) {
			Query expectedOutput = Query(false);
			Query resultQuery = QueryParser().parseQuery(followsTVariable);

			Assert::IsTrue(expectedOutput.compare(resultQuery));
		}
		TEST_METHOD(followsIfConstant_INVALID) {
			Query expectedOutput = Query(false);
			Query resultQuery = QueryParser().parseQuery(followsIfConstant);

			Assert::IsTrue(expectedOutput.compare(resultQuery));
		}
		TEST_METHOD(followsTAssignProcedure_INVALID) {
			Query expectedOutput = Query(false);
			Query resultQuery = QueryParser().parseQuery(followsTAssignProcedure);

			Assert::IsTrue(expectedOutput.compare(resultQuery));
		}
		TEST_METHOD(followsVariableProcedure_INVALID) {
			Query expectedOutput = Query(false);
			Query resultQuery = QueryParser().parseQuery(followsVariableProcedure);

			Assert::IsTrue(expectedOutput.compare(resultQuery));
		}
		TEST_METHOD(followsTConstantVariable_INVALID) {
			Query expectedOutput = Query(false);
			Query resultQuery = QueryParser().parseQuery(followsTConstantVariable);

			Assert::IsTrue(expectedOutput.compare(resultQuery));
		}
		TEST_METHOD(suchThatWhiteSpace_INVALID) {
			Query expectedOutput = Query();
			Query resultQuery = QueryParser().parseQuery(suchThatWhiteSpace);

			Assert::IsTrue(expectedOutput.compare(resultQuery));
		}
		TEST_METHOD(patternRightUnderscore_INVALID) {
			Query expectedOutput = Query();
			Query resultQuery = QueryParser().parseQuery(patternRightUnderscore);

			Assert::IsTrue(expectedOutput.compare(resultQuery));
		}
		TEST_METHOD(patternLeftUnderscore_INVALID) {
			Query expectedOutput = Query();
			Query resultQuery = QueryParser().parseQuery(patternLeftUnderscore);

			Assert::IsTrue(expectedOutput.compare(resultQuery));
		}
		TEST_METHOD(patternIdentExpr_INVALID) {
			Query expectedOutput = Query();
			Query resultQuery = QueryParser().parseQuery(patternIdentExpr);

			Assert::IsTrue(expectedOutput.compare(resultQuery));
		}
		TEST_METHOD(modifiesIfConstant_INVALID) {
			Query expectedOutput = Query(false);
			Query resultQuery = QueryParser().parseQuery(modifiesIfConstant);

			Assert::IsTrue(expectedOutput.compare(resultQuery));
		}
		TEST_METHOD(usesWhileConstant_INVALID) {
			Query expectedOutput = Query(false);
			Query resultQuery = QueryParser().parseQuery(usesWhileConstant);

			Assert::IsTrue(expectedOutput.compare(resultQuery));
		}
		TEST_METHOD(modifiesVariablePrint_INVALID) {
			Query expectedOutput = Query(false);
			Query resultQuery = QueryParser().parseQuery(modifiesVariablePrint);

			Assert::IsTrue(expectedOutput.compare(resultQuery));
		}
		TEST_METHOD(usesReadWhile_INVALID) {
			Query expectedOutput = Query(false);
			Query resultQuery = QueryParser().parseQuery(usesReadWhile);

			Assert::IsTrue(expectedOutput.compare(resultQuery));
		}
		TEST_METHOD(parentIfConstant_INVALID) {
			Query expectedOutput = Query(false);
			Query resultQuery = QueryParser().parseQuery(parentIfConstant);

			Assert::IsTrue(expectedOutput.compare(resultQuery));
		}
		TEST_METHOD(parentTWhileVariable_INVALID) {
			Query expectedOutput = Query(false);
			Query resultQuery = QueryParser().parseQuery(parentTWhileVariable);

			Assert::IsTrue(expectedOutput.compare(resultQuery));
		}
		TEST_METHOD(parentAssignVariable_INVALID) {
			Query expectedOutput = Query(false);
			Query resultQuery = QueryParser().parseQuery(parentAssignVariable);

			Assert::IsTrue(expectedOutput.compare(resultQuery));
		}
		TEST_METHOD(parentTReadConstant_INVALID) {
			Query expectedOutput = Query(false);
			Query resultQuery = QueryParser().parseQuery(parentTReadConstant);

			Assert::IsTrue(expectedOutput.compare(resultQuery));
		}
		TEST_METHOD(noSynonymName_INVALID) {
			Query expectedOutput = Query();
			Query resultQuery = QueryParser().parseQuery(noSynonymName);

			Assert::IsTrue(expectedOutput.compare(resultQuery));
		}
		TEST_METHOD(noSynonymNameNoWhitespace_INVALID) {
			Query expectedOutput = Query();
			Query resultQuery = QueryParser().parseQuery(noSynonymNameNoWhitespace);

			Assert::IsTrue(expectedOutput.compare(resultQuery));
		}
		TEST_METHOD(noSynonymNameWhitespace_INVALID) {
			Query expectedOutput = Query();
			Query resultQuery = QueryParser().parseQuery(noSynonymNameWhitespace);

			Assert::IsTrue(expectedOutput.compare(resultQuery));
		}
		TEST_METHOD(patternMissingBracket_INVALID) {
			Query expectedOutput = Query();
			Query resultQuery = QueryParser().parseQuery(patternMissingBracket);

			Assert::IsTrue(expectedOutput.compare(resultQuery));
		}
		TEST_METHOD(selectModifiesInvalid_INVALID) {
			Query expectedOutput = Query(false);
			Query resultQuery = QueryParser().parseQuery(selectModifiesInvalid);

			Assert::IsTrue(expectedOutput.compare(resultQuery));
		}
		TEST_METHOD(selectUsesInvalid_INVALID) {
			Query expectedOutput = Query(false);
			Query resultQuery = QueryParser().parseQuery(selectUsesInvalid);

			Assert::IsTrue(expectedOutput.compare(resultQuery));
		}

		TEST_METHOD(modifiesInteger_INVALID) {
			Query expectedOutput = Query(false);
			Query resultQuery = QueryParser().parseQuery(modifiesInteger);

			Assert::IsTrue(expectedOutput.compare(resultQuery));
		}
		TEST_METHOD(usesWhile_INVALID) {
			Query expectedOutput = Query(false);
			Query resultQuery = QueryParser().parseQuery(usesWhile);

			Assert::IsTrue(expectedOutput.compare(resultQuery));
		}
		TEST_METHOD(modifiesCall_INVALID) {
			Query expectedOutput = Query(false);
			Query resultQuery = QueryParser().parseQuery(modifiesCall);

			Assert::IsTrue(expectedOutput.compare(resultQuery));
		}
		TEST_METHOD(usesProc_INVALID) {
			Query expectedOutput = Query(false);
			Query resultQuery = QueryParser().parseQuery(usesProc);

			Assert::IsTrue(expectedOutput.compare(resultQuery));
		}
		TEST_METHOD(modifiesVarInt_INVALID) {
			Query expectedOutput = Query(false);
			Query resultQuery = QueryParser().parseQuery(modifiesVarInt);

			Assert::IsTrue(expectedOutput.compare(resultQuery));
		}
		TEST_METHOD(usesProgPrint_INVALID) {
			Query expectedOutput = Query(false);
			Query resultQuery = QueryParser().parseQuery(usesProgPrint);

			Assert::IsTrue(expectedOutput.compare(resultQuery));
		}

		TEST_METHOD(selectVar_INVALID) {
			Query expectedOutput = Query(false);
			Query resultQuery = QueryParser().parseQuery(nextVar);

			Assert::IsTrue(expectedOutput.compare(resultQuery));
		}
		TEST_METHOD(selectVarProc_INVALID) {
			Query expectedOutput = Query(false);
			Query resultQuery = QueryParser().parseQuery(nextVarProc);

			Assert::IsTrue(expectedOutput.compare(resultQuery));
		}
		TEST_METHOD(selectProc_INVALID) {
			Query expectedOutput = Query(false);
			Query resultQuery = QueryParser().parseQuery(nextProc);

			Assert::IsTrue(expectedOutput.compare(resultQuery));
		}

		TEST_METHOD(callsAssign_INVALID) {
			Query expectedOutput = Query(false);
			Query resultQuery = QueryParser().parseQuery(callsAssign);

			Assert::IsTrue(expectedOutput.compare(resultQuery));
		}
		TEST_METHOD(callsProgLine_INVALID) {
			Query expectedOutput = Query(false);
			Query resultQuery = QueryParser().parseQuery(callsProgLine);

			Assert::IsTrue(expectedOutput.compare(resultQuery));
		}
		TEST_METHOD(callsIntStmt_INVALID) {
			Query expectedOutput = Query(false);
			Query resultQuery = QueryParser().parseQuery(callsIntStmt);

			Assert::IsTrue(expectedOutput.compare(resultQuery));
		}

		TEST_METHOD(affectsIf_INVALID) {
			Query expectedOutput = Query(false);
			Query resultQuery = QueryParser().parseQuery(affectsIf);

			Assert::IsTrue(expectedOutput.compare(resultQuery));
		}
		TEST_METHOD(affectsIdent_INVALID) {
			Query expectedOutput = Query(false);
			Query resultQuery = QueryParser().parseQuery(affectsIdent);

			Assert::IsTrue(expectedOutput.compare(resultQuery));
		}
		TEST_METHOD(affectsCallWhile_INVALID) {
			Query expectedOutput = Query(false);
			Query resultQuery = QueryParser().parseQuery(affectsCallWhile);

			Assert::IsTrue(expectedOutput.compare(resultQuery));
		}

		TEST_METHOD(withInvalid1_INVALID) {
			Query expectedOutput = Query(false);
			Query resultQuery = QueryParser().parseQuery(withInvalid1);

			Assert::IsTrue(expectedOutput.compare(resultQuery));
		}
		TEST_METHOD(withInvalid2_INVALID) {
			Query expectedOutput = Query(false);
			Query resultQuery = QueryParser().parseQuery(withInvalid2);

			Assert::IsTrue(expectedOutput.compare(resultQuery));
		}
		TEST_METHOD(withInvalid3_INVALID) {
			Query expectedOutput = Query(false);
			Query resultQuery = QueryParser().parseQuery(withInvalid3);

			Assert::IsTrue(expectedOutput.compare(resultQuery));
		}

		TEST_METHOD(diffClause1_INVALID) {
			Query expectedOutput = Query(); // syntax error
			Query resultQuery = QueryParser().parseQuery(diffClause1);

			Assert::IsTrue(expectedOutput.compare(resultQuery));
		}
		TEST_METHOD(diffClause2_INVALID) {
			Query expectedOutput = Query(); // syntax error
			Query resultQuery = QueryParser().parseQuery(diffClause2);

			Assert::IsTrue(expectedOutput.compare(resultQuery));
		}
		TEST_METHOD(diffClause3_INVALID) {
			Query expectedOutput = Query(); // syntax error
			Query resultQuery = QueryParser().parseQuery(diffClause3);

			Assert::IsTrue(expectedOutput.compare(resultQuery));
		}
		TEST_METHOD(andInvalid_INVALID) {
			Query expectedOutput = Query(); // syntax error
			Query resultQuery = QueryParser().parseQuery(andInvalid);

			Assert::IsTrue(expectedOutput.compare(resultQuery));
		}

	};
}