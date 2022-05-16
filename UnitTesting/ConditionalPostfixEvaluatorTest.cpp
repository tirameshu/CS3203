#include <queue>
#include <vector>

#include "stdafx.h"
#include "CppUnitTest.h"
#include "ConditionalPostfixEvaluator.h"
#include "ExpressionTokenizer.h"
#include "ParserException.h"
#include "ParserShuntingConverter.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{
	TEST_CLASS(ConditonalPostfixEvaluatorTest)
	{
	public:

		TEST_METHOD(evaluatePostfix_valid) {
			std::string VALID_CONDITIONAL_EXPRESSION_1 = "((a < b) && (v < d)) && (d > e)";
			std::string VALID_CONDITIONAL_EXPRESSION_2 = "x<0";
			std::string VALID_CONDITIONAL_EXPRESSION_3 = "a == baca + cool";
			std::string VALID_CONDITIONAL_EXPRESSION_4 = "a + b -c + adsd + sdsda - csacsa * sadyusa < super + auyasyas";
			std::string VALID_CONDITIONAL_EXPRESSION_5 = "(not <= this) && (when * super > cool)";
			std::vector<std::string> validTokens = ExpressionTokenizer::tokenize(VALID_CONDITIONAL_EXPRESSION_1);
			std::vector<std::string> validTokens2 = ExpressionTokenizer::tokenize(VALID_CONDITIONAL_EXPRESSION_2);
			std::vector<std::string> validTokens3 = ExpressionTokenizer::tokenize(VALID_CONDITIONAL_EXPRESSION_3);
			std::vector<std::string> validTokens4 = ExpressionTokenizer::tokenize(VALID_CONDITIONAL_EXPRESSION_4);
			std::vector<std::string> validTokens5 = ExpressionTokenizer::tokenize(VALID_CONDITIONAL_EXPRESSION_5);

			std::queue<std::string> validQueue = ParserShuntingConverter::convert(validTokens, VALID_CONDITIONAL_EXPRESSION_1, true);
			std::queue<std::string> validQueue2 = ParserShuntingConverter::convert(validTokens2, VALID_CONDITIONAL_EXPRESSION_2, true);
			std::queue<std::string> validQueue3 = ParserShuntingConverter::convert(validTokens3, VALID_CONDITIONAL_EXPRESSION_3, true);
			std::queue<std::string> validQueue4 = ParserShuntingConverter::convert(validTokens4, VALID_CONDITIONAL_EXPRESSION_4, true);
			std::queue<std::string> validQueue5 = ParserShuntingConverter::convert(validTokens5, VALID_CONDITIONAL_EXPRESSION_5, true);

			Assert::IsTrue(ConditionalPostfixEvaluator::evaluateConditionalPostfix(validQueue));
			Assert::IsTrue(ConditionalPostfixEvaluator::evaluateConditionalPostfix(validQueue2));
			Assert::IsTrue(ConditionalPostfixEvaluator::evaluateConditionalPostfix(validQueue3));
			Assert::IsTrue(ConditionalPostfixEvaluator::evaluateConditionalPostfix(validQueue4));
			Assert::IsTrue(ConditionalPostfixEvaluator::evaluateConditionalPostfix(validQueue5));
		}

		TEST_METHOD(evaluatePostfix_invalid) {
			std::string INVALID_CONDITIONAL_EXPRESSION_1 = "a < b + super > 2";
			std::string INVALID_CONDITIONAL_EXPRESSION_2 = "a < b < c < d";
			std::string INVALID_CONDITIONAL_EXPRESSION_3 = "A * deasd && (b + c)";
			std::string INVALID_CONDITIONAL_EXPRESSION_4 = "(a < b) || (v > dds) && (ds - sdsd)";
			std::string INVALID_CONDITIONAL_EXPRESSION_5 = "(a > b) && ((super > d) + (not < epic))";
			std::string INVALID_CONDITIONAL_EXPRESSION_6 = "(a < b) >= ((this > d) && (super > epic))";
			std::string INVALID_CONDITIONAL_EXPRESSION_7 = "(likethis + thenhow) || (means - what)";

			auto invalidFunction = [&] {
				std::vector<std::string> invalidTokens = ExpressionTokenizer::tokenize(INVALID_CONDITIONAL_EXPRESSION_1);
				std::queue<std::string> invalidQueue = ParserShuntingConverter::convert(invalidTokens, INVALID_CONDITIONAL_EXPRESSION_1, true);
				bool isSuccess = ConditionalPostfixEvaluator::evaluateConditionalPostfix(invalidQueue);
			};

			auto invalidFunction2 = [&] {
				std::vector<std::string> invalidTokens2 = ExpressionTokenizer::tokenize(INVALID_CONDITIONAL_EXPRESSION_2);
				std::queue<std::string> invalidQueue2 = ParserShuntingConverter::convert(invalidTokens2, INVALID_CONDITIONAL_EXPRESSION_2, true);
				bool isSuccess2 = ConditionalPostfixEvaluator::evaluateConditionalPostfix(invalidQueue2);
			};

			auto invalidFunction3 = [&] {
				std::vector<std::string> invalidTokens3 = ExpressionTokenizer::tokenize(INVALID_CONDITIONAL_EXPRESSION_3);
				std::queue<std::string> invalidQueue3 = ParserShuntingConverter::convert(invalidTokens3, INVALID_CONDITIONAL_EXPRESSION_3, true);
				bool isSuccess3 = ConditionalPostfixEvaluator::evaluateConditionalPostfix(invalidQueue3);
			};

			auto invalidFunction4 = [&] {
				std::vector<std::string> invalidTokens4 = ExpressionTokenizer::tokenize(INVALID_CONDITIONAL_EXPRESSION_4);
				std::queue<std::string> invalidQueue4 = ParserShuntingConverter::convert(invalidTokens4, INVALID_CONDITIONAL_EXPRESSION_4, true);
				bool isSuccess4 = ConditionalPostfixEvaluator::evaluateConditionalPostfix(invalidQueue4);
			};

			auto invalidFunction5 = [&] {
				std::vector<std::string> invalidTokens5 = ExpressionTokenizer::tokenize(INVALID_CONDITIONAL_EXPRESSION_5);
				std::queue<std::string> invalidQueue5 = ParserShuntingConverter::convert(invalidTokens5, INVALID_CONDITIONAL_EXPRESSION_5, true);
				bool isSuccess5 = ConditionalPostfixEvaluator::evaluateConditionalPostfix(invalidQueue5);
			};

			auto invalidFunction6 = [&] {
				std::vector<std::string> invalidTokens6 = ExpressionTokenizer::tokenize(INVALID_CONDITIONAL_EXPRESSION_6);
				std::queue<std::string> invalidQueue6 = ParserShuntingConverter::convert(invalidTokens6, INVALID_CONDITIONAL_EXPRESSION_6, true);
				bool isSuccess6 = ConditionalPostfixEvaluator::evaluateConditionalPostfix(invalidQueue6);
			};

			auto invalidFunction7 = [&] {
				std::vector<std::string> invalidTokens7 = ExpressionTokenizer::tokenize(INVALID_CONDITIONAL_EXPRESSION_7);
				std::queue<std::string> invalidQueue7 = ParserShuntingConverter::convert(invalidTokens7, INVALID_CONDITIONAL_EXPRESSION_7, true);
				bool isSuccess7 = ConditionalPostfixEvaluator::evaluateConditionalPostfix(invalidQueue7);
			};

			Assert::ExpectException<ParserException>(invalidFunction);
			Assert::ExpectException<ParserException>(invalidFunction2);
			Assert::ExpectException<ParserException>(invalidFunction3);
			Assert::ExpectException<ParserException>(invalidFunction4);
			Assert::ExpectException<ParserException>(invalidFunction5);
			Assert::ExpectException<ParserException>(invalidFunction6);
			Assert::ExpectException<ParserException>(invalidFunction7);
		}
	};
}

