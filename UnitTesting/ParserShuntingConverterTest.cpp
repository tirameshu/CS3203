#include <queue>
#include <vector>

#include "stdafx.h"
#include "CppUnitTest.h"
#include "ParserShuntingConverter.h"
#include "ExpressionTokenizer.h"
#include "ParserException.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{
	TEST_CLASS(ParserShuntingConverterTest)
	{
	public:

		TEST_METHOD(shuntingConvert_valid_conditional) {
			std::string CONDITIONAL_INFIX_EXPRESSION_1 = "!(((a > b) || (1 <= 2)) && (!(3 > a)  || (c <= 5)))";
			std::string CONDITIONAL_POSTFIX_EXPRESSION_1 = "a b > 1 2 <= || 3 a > c 5 <= || ! && !";
			std::string CONDITIONAL_INFIX_EXPRESSION_2 = "((bee >= movie)&&(scripts==epic))";
			std::string CONDITIONAL_POSTFIX_EXPRESSION_2 = "bee movie >= scripts epic == &&";
			std::vector<std::string> vectorOfTokens1 = ExpressionTokenizer::tokenize(CONDITIONAL_INFIX_EXPRESSION_1);
			std::vector<std::string> vectorOfTokens2 = ExpressionTokenizer::tokenize(CONDITIONAL_INFIX_EXPRESSION_2);

			std::queue<std::string> queueOfOutputs1 = ParserShuntingConverter::convert(vectorOfTokens1, CONDITIONAL_INFIX_EXPRESSION_1, true);
			std::queue<std::string> queueOfOutputs2 = ParserShuntingConverter::convert(vectorOfTokens1, CONDITIONAL_INFIX_EXPRESSION_2, true);

			std::string outputString1;
			std::string outputString2;

			while (!queueOfOutputs1.empty()) {
				outputString1 += queueOfOutputs1.front() + " ";
				queueOfOutputs1.pop();
			}

			while (!queueOfOutputs2.empty()) {
				outputString2 += queueOfOutputs2.front() + " ";
				queueOfOutputs2.pop();
			}

			Assert::IsTrue(outputString1.compare(CONDITIONAL_POSTFIX_EXPRESSION_1));
			Assert::IsTrue(outputString2.compare(CONDITIONAL_POSTFIX_EXPRESSION_2));

		}
		TEST_METHOD(shuntingConvert_invalid_conditional) {
			std::string STANDARD_INFIX_EXPRESSION_1 = "a*b + c - 3 * (2 + d)";
			std::string STANDARD_POSTFIX_EXPRESSION_1 = "a b * c + 3 2 d + * -";
			std::vector<std::string> vectorOfTokens = ExpressionTokenizer::tokenize(STANDARD_INFIX_EXPRESSION_1);

			auto exceptionFunction = [&] {
				std::queue<std::string> queueOfOutputs = ParserShuntingConverter::convert(vectorOfTokens, STANDARD_INFIX_EXPRESSION_1, true);
			};
			
			Assert::ExpectException<ParserException>(exceptionFunction);
		}

		TEST_METHOD(shuntingConvert_valid_standard) {
			std::string STANDARD_INFIX_EXPRESSION_2 = "(((3/(((x-5))))-(4/(x+5)))*(x+5)*(x-5))/(((7/((x+5)*(x-5)))-(4/(x-5)))*(x+5)*(x-5))";
			std::string STANDARD_POSTFIX_EXPRESSION_2 = "3x5-/4x5+/-x5+*x5-*7x5+x5-*/4x5-/-x5+*x5-*/";
			std::vector<std::string> vectorOfTokens = ExpressionTokenizer::tokenize(STANDARD_INFIX_EXPRESSION_2);
			std::queue<std::string> queueOfOutputs = ParserShuntingConverter::convert(vectorOfTokens, STANDARD_INFIX_EXPRESSION_2, false);
			std::string outputString;

			while (!queueOfOutputs.empty()) {
				outputString += queueOfOutputs.front();
				queueOfOutputs.pop();
			}

			Assert::IsTrue(outputString.compare(STANDARD_POSTFIX_EXPRESSION_2) == 0);
		}

		TEST_METHOD(shuntingConvert_invalid_standard) {
			std::string CONDITIONAL_INFIX_EXPRESSION_1 = "!(((a > b) || (1 <= 2)) && (!(3 > a)  || (c <= 5)))";
			std::vector<std::string> vectorOfTokens = ExpressionTokenizer::tokenize(CONDITIONAL_INFIX_EXPRESSION_1);

			auto exceptionFunction = [&] {
				std::queue<std::string> queueOfOutputs = ParserShuntingConverter::convert(vectorOfTokens, CONDITIONAL_INFIX_EXPRESSION_1, false);
			};

			Assert::ExpectException<ParserException>(exceptionFunction);
		}

		TEST_METHOD(shuntingConvert_invalid_wrongConditionalExpressionFormat) {
			std::string INVALID_CONDITIONAL_EXPRESSION_1 = "(a < super) && !(not == epic)";
			std::string INVALID_CONDITIONAL_EXPRESSION_2 = "!a < c";
			std::string INVALID_CONDITIONAL_EXPRESSION_3 = "saoiufhasu << not";
			std::string INVALID_CONDITIONAL_EXPRESSION_4 = "a";
			std::string INVALID_CONDITIONAL_EXPRESSION_5 = "this = e+c+d+d";
			std::string INVALID_CONDITIONAL_EXPRESSION_6 = "A < 0123";
			std::string INVALID_CONDITIONAL_EXPRESSION_7 = "!(a || b)";
			std::string INVALID_CONDITIONAL_EXPRESSION_8 = "wiurd + super";
			std::string INVALID_CONDITIONAL_EXPRESSION_9 = "(a < b) && (v < d) && (d > e)";
			std::string INVALID_CONDITIONAL_EXPRESSION_10 = "";
			std::string INVALID_CONDITIONAL_EXPRESSION_11 = "A B";
			std::string INVALID_CONDITIONAL_EXPRESSION_12 = "thenhow || (means <= how)";
			std::vector<std::string> vectorOfTokens1 = ExpressionTokenizer::tokenize(INVALID_CONDITIONAL_EXPRESSION_1);
			std::vector<std::string> vectorOfTokens2 = ExpressionTokenizer::tokenize(INVALID_CONDITIONAL_EXPRESSION_2);
			std::vector<std::string> vectorOfTokens3 = ExpressionTokenizer::tokenize(INVALID_CONDITIONAL_EXPRESSION_3);
			std::vector<std::string> vectorOfTokens4 = ExpressionTokenizer::tokenize(INVALID_CONDITIONAL_EXPRESSION_4);
			std::vector<std::string> vectorOfTokens5 = ExpressionTokenizer::tokenize(INVALID_CONDITIONAL_EXPRESSION_5);
			std::vector<std::string> vectorOfTokens6 = ExpressionTokenizer::tokenize(INVALID_CONDITIONAL_EXPRESSION_6);
			std::vector<std::string> vectorOfTokens7 = ExpressionTokenizer::tokenize(INVALID_CONDITIONAL_EXPRESSION_7);
			std::vector<std::string> vectorOfTokens8 = ExpressionTokenizer::tokenize(INVALID_CONDITIONAL_EXPRESSION_8);
			std::vector<std::string> vectorOfTokens9 = ExpressionTokenizer::tokenize(INVALID_CONDITIONAL_EXPRESSION_9);
			std::vector<std::string> vectorOfTokens10 = ExpressionTokenizer::tokenize(INVALID_CONDITIONAL_EXPRESSION_10);
			std::vector<std::string> vectorOfTokens11 = ExpressionTokenizer::tokenize(INVALID_CONDITIONAL_EXPRESSION_11);
			std::vector<std::string> vectorOfTokens12 = ExpressionTokenizer::tokenize(INVALID_CONDITIONAL_EXPRESSION_12);

			auto exceptionFunction1 = [&] {
				std::queue<std::string> queueOfOutputs = ParserShuntingConverter::convert(vectorOfTokens1, INVALID_CONDITIONAL_EXPRESSION_1, true);
			};

			auto exceptionFunction2 = [&] {
				std::queue<std::string> queueOfOutputs2 = ParserShuntingConverter::convert(vectorOfTokens2, INVALID_CONDITIONAL_EXPRESSION_2, true);
			};

			auto exceptionFunction3 = [&] {
				std::queue<std::string> queueOfOutputs3 = ParserShuntingConverter::convert(vectorOfTokens3, INVALID_CONDITIONAL_EXPRESSION_3, true);
			};

			auto exceptionFunction4 = [&] {
				std::queue<std::string> queueOfOutputs4 = ParserShuntingConverter::convert(vectorOfTokens4, INVALID_CONDITIONAL_EXPRESSION_4, true);
			};

			auto exceptionFunction5 = [&] {
				std::queue<std::string> queueOfOutputs5 = ParserShuntingConverter::convert(vectorOfTokens5, INVALID_CONDITIONAL_EXPRESSION_5, true);
			};

			auto exceptionFunction6 = [&] {
				std::queue<std::string> queueOfOutputs6 = ParserShuntingConverter::convert(vectorOfTokens6, INVALID_CONDITIONAL_EXPRESSION_6, true);
			};

			auto exceptionFunction7 = [&] {
				std::queue<std::string> queueOfOutputs7 = ParserShuntingConverter::convert(vectorOfTokens7, INVALID_CONDITIONAL_EXPRESSION_7, true);
			};

			auto exceptionFunction8 = [&] {
				std::queue<std::string> queueOfOutputs8 = ParserShuntingConverter::convert(vectorOfTokens8, INVALID_CONDITIONAL_EXPRESSION_8, true);
			};

			auto exceptionFunction9 = [&] {
				std::queue<std::string> queueOfOutputs9 = ParserShuntingConverter::convert(vectorOfTokens9, INVALID_CONDITIONAL_EXPRESSION_9, true);
			};

			auto exceptionFunction10 = [&] {
				std::queue<std::string> queueOfOutputs10 = ParserShuntingConverter::convert(vectorOfTokens10, INVALID_CONDITIONAL_EXPRESSION_10, true);
			};

			auto exceptionFunction11 = [&] {
				std::queue<std::string> queueOfOutputs11 = ParserShuntingConverter::convert(vectorOfTokens11, INVALID_CONDITIONAL_EXPRESSION_11, true);
			};

			auto exceptionFunction12 = [&] {
				std::queue<std::string> queueOfOutputs12 = ParserShuntingConverter::convert(vectorOfTokens12, INVALID_CONDITIONAL_EXPRESSION_12, true);
			};

			Assert::ExpectException<ParserException>(exceptionFunction1);
			Assert::ExpectException<ParserException>(exceptionFunction2);
			Assert::ExpectException<ParserException>(exceptionFunction3);
			Assert::ExpectException<ParserException>(exceptionFunction4);
			Assert::ExpectException<ParserException>(exceptionFunction5);
			Assert::ExpectException<ParserException>(exceptionFunction6);
			Assert::ExpectException<ParserException>(exceptionFunction7);
			Assert::ExpectException<ParserException>(exceptionFunction8);
			Assert::ExpectException<ParserException>(exceptionFunction9);
			Assert::ExpectException<ParserException>(exceptionFunction10);
			Assert::ExpectException<ParserException>(exceptionFunction11);
			Assert::ExpectException<ParserException>(exceptionFunction12);
		}
	};
}
