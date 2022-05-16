#include "stdafx.h"
#include "CppUnitTest.h"
#include "ExpressionTokenizer.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{
	TEST_CLASS(ExpressionTokenizerTest)
	{
	public:
		TEST_METHOD(tokenizeExpression_valid_letterVarnameWithSpace) {
			std::string EXPRESSION_LETTER_VARNAME_SPACE = "( a > 1 ) || ! ( a >= c )";
			std::vector<std::string> output = ExpressionTokenizer::tokenize(EXPRESSION_LETTER_VARNAME_SPACE);

			Assert::IsTrue(
				output[0].compare("(") == 0
				&& output[1].compare("a") == 0
				&& output[2].compare(">") == 0
				&& output[3].compare("1") == 0
				&& output[4].compare(")") == 0
				&& output[5].compare("||") == 0
				&& output[6].compare("!") == 0
				&& output[7].compare("(") == 0
				&& output[8].compare("a") == 0
				&& output[9].compare(">=") == 0
				&& output[10].compare("c") == 0
				&& output[11].compare(")") == 0
			);
		}

		TEST_METHOD(tokenizeExpression_valid_letterVarnameNoSpace) {
			std::string EXPRESSION_LETTER_VARNAME_NOSPACE = "!(!(2>=c) && (b>d))";
			std::vector<std::string> output = ExpressionTokenizer::tokenize(EXPRESSION_LETTER_VARNAME_NOSPACE);

			Assert::IsTrue(
				output[0].compare("!") == 0
				&& output[1].compare("(") == 0
				&& output[2].compare("!") == 0
				&& output[3].compare("(") == 0
				&& output[4].compare("2") == 0
				&& output[5].compare(">=") == 0
				&& output[6].compare("c") == 0
				&& output[7].compare(")") == 0
				&& output[8].compare("&&") == 0
				&& output[9].compare("(") == 0
				&& output[10].compare("b") == 0
				&& output[11].compare(">") == 0
				&& output[12].compare("d") == 0
				&& output[13].compare(")") == 0
				&& output[14].compare(")") == 0
			);
		}

		TEST_METHOD(tokenizeExpression_valid_wordVarnameWithSpace) {
			std::string EXPRESSION_WORD_VARNAME_SPACE = "( awesome <= ! ( cool + epic ) )";
			std::vector<std::string> output = ExpressionTokenizer::tokenize(EXPRESSION_WORD_VARNAME_SPACE);
			Assert::IsTrue(
				output[0].compare("(") == 0
				&& output[1].compare("awesome") == 0
				&& output[2].compare("<=") == 0
				&& output[3].compare("!") == 0
				&& output[4].compare("(") == 0
				&& output[5].compare("cool") == 0
				&& output[6].compare("+") == 0
				&& output[7].compare("epic") == 0
				&& output[8].compare(")") == 0
				&& output[9].compare(")") == 0
			);
		}

		TEST_METHOD(tokenizeExpression_valid_wordVarnameNoSpace) {
			std::string EXPRESSION_WORD_VARNAME_NOSPACE = "!(nowthis > 223)";
			std::vector<std::string> output = ExpressionTokenizer::tokenize(EXPRESSION_WORD_VARNAME_NOSPACE);
			Assert::IsTrue(
				output[0].compare("!") == 0
				&& output[1].compare("(") == 0
				&& output[2].compare("nowthis") == 0
				&& output[3].compare(">") == 0
				&& output[4].compare("223") == 0
				&& output[5].compare(")") == 0
			);
		}
		
	};
}

