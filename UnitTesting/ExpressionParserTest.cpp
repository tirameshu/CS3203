#include "stdafx.h"
#include "CppUnitTest.h"
#include "ExpressionParser.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{
	TEST_CLASS(ExpressionParserTest)
	{
	public: 
		TEST_METHOD(getPostfixFromInfix_OneVariable)
		{
			std::string testExpression = "cS32oE";
			std::string expectedOutput = "cS32oE";

			std::string postfixExpression = ExpressionParser::getPostfixFromInfix(testExpression);

			Assert::AreEqual(expectedOutput, postfixExpression);
		}

		TEST_METHOD(getPostfixFromInfix_OneConstant)
		{
			std::string testExpression = "3203";
			std::string expectedOutput = "3203";

			std::string postfixExpression = ExpressionParser::getPostfixFromInfix(testExpression);

			Assert::AreEqual(expectedOutput, postfixExpression);
		}

		TEST_METHOD(getPostfixFromInfix_ExpressionWithPlus)
		{
			std::string testExpression = "cS32oE+3203";
			std::string expectedOutput = "cS32oE3203+";

			std::string postfixExpression = ExpressionParser::getPostfixFromInfix(testExpression);

			Assert::AreEqual(expectedOutput, postfixExpression);
		}

		TEST_METHOD(getPostfixFromInfix_ExpressionWithMinus)
		{
			std::string testExpression = "3203-cS32oE";
			std::string expectedOutput = "3203cS32oE-";

			std::string postfixExpression = ExpressionParser::getPostfixFromInfix(testExpression);

			Assert::AreEqual(expectedOutput, postfixExpression);
		}

		TEST_METHOD(getPostfixFromInfix_ExpressionWithTimes)
		{
			std::string testExpression = "3203*cS32oE";
			std::string expectedOutput = "3203cS32oE*";

			std::string postfixExpression = ExpressionParser::getPostfixFromInfix(testExpression);

			Assert::AreEqual(expectedOutput, postfixExpression);
		}

		TEST_METHOD(getPostfixFromInfix_ExpressionWithDivide)
		{
			std::string testExpression = "3203/cS32oE";
			std::string expectedOutput = "3203cS32oE/";

			std::string postfixExpression = ExpressionParser::getPostfixFromInfix(testExpression);

			Assert::AreEqual(expectedOutput, postfixExpression);
		}

		TEST_METHOD(getPostfixFromInfix_ExpressionWithMod)
		{
			std::string testExpression = "cS32oE%3203";
			std::string expectedOutput = "cS32oE3203%";

			std::string postfixExpression = ExpressionParser::getPostfixFromInfix(testExpression);

			Assert::AreEqual(expectedOutput, postfixExpression);
		}

		TEST_METHOD(getPostfixFromInfix_ExpressionWithMinusThenPlus)
		{
			std::string testExpression = "cS32oE-3203+tEam2o";
			std::string expectedOutput = "cS32oE3203-tEam2o+";

			std::string postfixExpression = ExpressionParser::getPostfixFromInfix(testExpression);

			Assert::AreEqual(expectedOutput, postfixExpression);
		}

		TEST_METHOD(getPostfixFromInfix_ExpressionWithPlusThenTimes)
		{
			std::string testExpression = "cS32oE+3203*tEam2o";
			std::string expectedOutput = "cS32oE3203tEam2o*+";

			std::string postfixExpression = ExpressionParser::getPostfixFromInfix(testExpression);

			Assert::AreEqual(expectedOutput, postfixExpression);
		}

		TEST_METHOD(getPostfixFromInfix_ExpressionWithMinusThenMod)
		{
			std::string testExpression = "cS32oE-3203%tEam2o";
			std::string expectedOutput = "cS32oE3203tEam2o%-";

			std::string postfixExpression = ExpressionParser::getPostfixFromInfix(testExpression);

			Assert::AreEqual(expectedOutput, postfixExpression);
		}

		TEST_METHOD(getPostfixFromInfix_ExpressionWithModThenMinus)
		{
			std::string testExpression = "cS32oE%3203-tEam2o";
			std::string expectedOutput = "cS32oE3203%tEam2o-";

			std::string postfixExpression = ExpressionParser::getPostfixFromInfix(testExpression);

			Assert::AreEqual(expectedOutput, postfixExpression);
		}

		TEST_METHOD(getPostfixFromInfix_ExpressionWithBracketedPlusThenDivide)
		{
			std::string testExpression = "(cS32oE+3203)/tEam2o";
			std::string expectedOutput = "cS32oE3203+tEam2o/";

			std::string postfixExpression = ExpressionParser::getPostfixFromInfix(testExpression);

			Assert::AreEqual(expectedOutput, postfixExpression);
		}

		TEST_METHOD(getPostfixFromInfix_ExpressionWithTimesThenMinusThenDivide)
		{
			std::string testExpression = "cS32oE*3203-tEam2o/2021";
			std::string expectedOutput = "cS32oE3203*tEam2o2021/-";

			std::string postfixExpression = ExpressionParser::getPostfixFromInfix(testExpression);

			Assert::AreEqual(expectedOutput, postfixExpression);
		}

		TEST_METHOD(getPostfixFromInfix_ExpressionWithComplexExpression1)
		{
			std::string testExpression = "a*b/(c-a)/b/c-z";
			std::string expectedOutput = "ab*ca-/b/c/z-";

			std::string postfixExpression = ExpressionParser::getPostfixFromInfix(testExpression);

			Assert::AreEqual(expectedOutput, postfixExpression);
		}

		TEST_METHOD(getPostfixFromInfix_ExpressionWithComplexExpression2)
		{
			std::string testExpression = "a*b/c-x/(b/c)-z";
			std::string expectedOutput = "ab*c/xbc//-z-";

			std::string postfixExpression = ExpressionParser::getPostfixFromInfix(testExpression);

			Assert::AreEqual(expectedOutput, postfixExpression);
		}

		TEST_METHOD(getPostfixFromInfix_ExpressionWithComplexExpression3)
		{
			std::string testExpression = "a*b/c-a/b/(c-z)";
			std::string expectedOutput = "ab*c/ab/cz-/-";

			std::string postfixExpression = ExpressionParser::getPostfixFromInfix(testExpression);

			Assert::AreEqual(expectedOutput, postfixExpression);
		}

		TEST_METHOD(getPostfixFromInfix_ExpressionWithComplexExpression4)
		{
			std::string testExpression = "a*(b/c-a%b)/c-z";
			std::string expectedOutput = "abc/ab%-*c/z-";

			std::string postfixExpression = ExpressionParser::getPostfixFromInfix(testExpression);

			Assert::AreEqual(expectedOutput, postfixExpression);
		}

		TEST_METHOD(getPostfixFromInfix_ExpressionWithComplexExpression5)
		{
			std::string testExpression = "(((3/(((x-5))))-(4/(x+5)))*(x+5)*(x-5))/(((7/((x+5)*(x-5)))-(4/(x-5)))*(x+5)*(x-5))";
			std::string expectedOutput = "3x5-/4x5+/-x5+*x5-*7x5+x5-*/4x5-/-x5+*x5-*/";

			std::string postfixExpression = ExpressionParser::getPostfixFromInfix(testExpression);

			Assert::AreEqual(expectedOutput, postfixExpression);
		}
	};
}
