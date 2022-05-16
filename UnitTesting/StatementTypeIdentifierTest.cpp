#include "stdafx.h"
#include "CppUnitTest.h"
#include "StatementTypeIdentifier.h"
#include "StatementTypeEnum.h"
#include "ParserException.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{
	TEST_CLASS(StatementTypeIdentifierTest)
	{
	public:
		TEST_METHOD(identifyStatement_valid_read) {
			std::string VALID_READ_STATEMENT_1 = "read validReadVar1";
			std::string VALID_READ_STATEMENT_2 = "read validReadVar2";
			int statementType1 = StatementTypeIdentifier::identifyStatement(VALID_READ_STATEMENT_1);
			int statementType2 = StatementTypeIdentifier::identifyStatement(VALID_READ_STATEMENT_2);
			
			Assert::IsTrue(statementType1 == StatementTypeEnum::READ);
			Assert::IsTrue(statementType2 == StatementTypeEnum::READ);
		}

		TEST_METHOD(identifyStatement_valid_print) {
			std::string VALID_PRINT_STATEMENT_1 = "print validPrintVar1";
			std::string VALID_PRINT_STATEMENT_2 = "print validPrintStmt2";
			int statementType1 = StatementTypeIdentifier::identifyStatement(VALID_PRINT_STATEMENT_1);
			int statementType2 = StatementTypeIdentifier::identifyStatement(VALID_PRINT_STATEMENT_2);

			Assert::IsTrue(statementType1 == StatementTypeEnum::PRINT);
			Assert::IsTrue(statementType2 == StatementTypeEnum::PRINT);
		}

		TEST_METHOD(identifyStatement_valid_assign) {
			std::string VALID_ASSIGNMENT_STATEMENT_1 = "a=b+c+d";
			std::string VALID_ASSIGNMENT_STATEMENT_2 = "read = read + 1";
			int statementType1 = StatementTypeIdentifier::identifyStatement(VALID_ASSIGNMENT_STATEMENT_1);
			int statementType2 = StatementTypeIdentifier::identifyStatement(VALID_ASSIGNMENT_STATEMENT_2);

			Assert::IsTrue(statementType1 == StatementTypeEnum::ASSIGN);
			Assert::IsTrue(statementType2 == StatementTypeEnum::ASSIGN);
		}

		TEST_METHOD(identifyStatement_valid_if) {
			std::string VALID_IF_STATEMENT_1 = "if (a < b)";
			std::string VALID_IF_STATEMENT_2 = "if(a>b)";
			int statementType1 = StatementTypeIdentifier::identifyStatement(VALID_IF_STATEMENT_1);
			int statementType2 = StatementTypeIdentifier::identifyStatement(VALID_IF_STATEMENT_2);

			Assert::IsTrue(statementType1 == StatementTypeEnum::IF);
			Assert::IsTrue(statementType2 == StatementTypeEnum::IF);
		}

		TEST_METHOD(identifyStatement_valid_while) {
			std::string VALID_WHILE_STATEMENT_1 = "while(a > b)";
			std::string VALID_WHILE_STATEMENT_2 = "while(b ==c)";
			int statementType1 = StatementTypeIdentifier::identifyStatement(VALID_WHILE_STATEMENT_1);
			int statementType2 = StatementTypeIdentifier::identifyStatement(VALID_WHILE_STATEMENT_2);

			Assert::IsTrue(statementType1 == StatementTypeEnum::WHILE);
			Assert::IsTrue(statementType2 == StatementTypeEnum::WHILE);
		}

		TEST_METHOD(identifyStatement_invalid_wrongFormat) {
			std::string INVALID_STATEMENT_INVALID_KEYWORD = "prant invalidVar";
			std::string INVALID_STATEMENT_UNIDENTIFIED_TYPE = "uvuvwevwevwe onyetenyevwe ugwemubwem ossas";
			std::string INVALID_STATEMENT_EXPRESSION = "a+b+c+d";
			auto exceptionFunction1 = [&] {
				int statementType1 = StatementTypeIdentifier::identifyStatement(INVALID_STATEMENT_INVALID_KEYWORD);
			};

			auto exceptionFunction2 = [&] {
				int statementType2 = StatementTypeIdentifier::identifyStatement(INVALID_STATEMENT_UNIDENTIFIED_TYPE);
			};

			auto exceptionFunction3 = [&] {
				int statementType3 = StatementTypeIdentifier::identifyStatement(INVALID_STATEMENT_EXPRESSION);
			};

			Assert::ExpectException<ParserException>(exceptionFunction1);
			Assert::ExpectException<ParserException>(exceptionFunction2);
			Assert::ExpectException<ParserException>(exceptionFunction3);
		}

		TEST_METHOD(identifyStatement_invalid_empty) {
			std::string INVALID_STATEMENT_EMPTY = "";
			auto exceptionFunction4 = [&] {
				int statementType4 = StatementTypeIdentifier::identifyStatement(INVALID_STATEMENT_EMPTY);
			};

			Assert::ExpectException<ParserException>(exceptionFunction4);
		}
	};
}
