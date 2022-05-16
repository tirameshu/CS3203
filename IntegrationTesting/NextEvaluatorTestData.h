#pragma once

#include "QueryEvaluator.h"

/*
procedure testA {
1    p = 4;
2    a = p * 2;
3    if (c == k)
		then {
4			pattern = p * a * a + p * ifs + p * w;
5			while(w > 0) {
6				read p;
7				if (a > 10) {
					then {
8						call testB;
9						a = a - 1;
					}
					else {
10						read p;
					}
				}
11				ifs = ifs -1;
			}
12			a = 10;
13			read p;
14			print a;
		}
	  else {
15		  print p;
16		  a = 10;
	  }
}
*/

std::unordered_set<std::string> nextAllStmt = { "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15", "16"};
std::unordered_set<std::string> nextPrevList = { "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "15" };
std::unordered_set<std::string> nextNextList = { "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15", "16" };

/*
std::unordered_map<int, nextStruct> expectedStmtNextMap = {
	{1, {{}, {2}, {}, {2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16}}},
	{2, {{1}, {3}, {1}, {3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16}}},
	{3, {{2}, {4, 15}, {1, 2}, {4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16}}},
	{4, {{3}, {5}, {1, 2, 3}, {5, 6, 7, 8, 9, 10, 11, 12, 13, 14}}},
	{5, {{4}, {6}, {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11}, {5, 6, 7, 8, 9, 10, 11, 12, 13, 14}}},
	{6, {{5}, {7}, {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11}, {5, 6, 7, 8, 9, 10, 11, 12, 13, 14}}},
	{7, {{6}, {8, 10}, {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11}, {5, 6, 7, 8, 9, 10, 11, 12, 13, 14}}},
	{8, {{7}, {9}, {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11}, {5, 6, 7, 8, 9, 10, 11, 12, 13, 14}}},
	{9, {{8}, {11}, {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11}, {5, 6, 7, 8, 9, 10, 11, 12, 13, 14}}},
	{10, {{7}, {11}, {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11}, {5, 6, 7, 8, 9, 10, 11, 12, 13, 14}}},
	{11, {{9, 10}, {5, 12}, {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11}, {5, 6, 7, 8, 9, 10, 11, 12, 13, 14}}},
	{12, {{11}, {13}, {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11}, {13, 14}}},
	{13, {{12}, {14}, {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12}, {14}}},
	{14, {{13}, {}, {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13}, {}}},
	{15, {{3}, {16}, {1, 2, 3}, {16}}},
	{16, {{15}, {}, {1, 2, 3, 15}, {}}},
};

std::unordered_set<std::pair<int, int>, IntPairHasher> expectedNextPairList = {
	{1, 2}, {2, 3}, {3, 4}, {3, 15}, {4, 5}, {5, 6}, {6, 7}, {7, 8}, {7, 10}, {8, 9},
	{9, 11}, {10, 11}, {11, 5}, {11, 12}, {12, 13}, {13, 14}, {15, 16}
};

std::unordered_set<std::pair<int, int>, IntPairHasher> expectedNextStarPairList = {
	{1, 2}, {1, 3}, {1, 4}, {1, 5}, {1, 6}, {1, 7}, {1, 8}, {1, 9}, {1, 10}, {1, 11}, {1, 12}, {1, 13}, {1, 14}, {1, 15}, {1, 16},
	{2, 3}, {2, 4}, {2, 5}, {2, 6}, {2, 7}, {2, 8}, {2, 9}, {2, 10}, {2, 11}, {2, 12}, {2, 13}, {2, 14}, {2, 15}, {2, 16},
    {3, 4}, {3, 5}, {3, 6}, {3, 7}, {3, 8}, {3, 9}, {3, 10}, {3, 11}, {3, 12}, {3, 13}, {3, 14}, {3, 15}, {3, 16},
	{4, 5}, {4, 6}, {4, 7}, {4, 8}, {4, 9}, {4, 10}, {4, 11}, {4, 12}, {4, 13}, {4, 14},
	{5, 5}, {5, 6}, {5, 7}, {5, 8}, {5, 9}, {5, 10}, {5, 11}, {5, 12}, {5, 13}, {5, 14},
	{6, 5}, {6, 6}, {6, 7}, {6, 8}, {6, 9}, {6, 10}, {6, 11}, {6, 12}, {6, 13}, {6, 14},
	{7, 5}, {7, 6}, {7, 7}, {7, 8}, {7, 9}, {7, 10}, {7, 11}, {7, 12}, {7, 13}, {7, 14},
	{8, 5}, {8, 6}, {8, 7}, {8, 8}, {8, 9}, {8, 10}, {8, 11}, {8, 12}, {8, 13}, {8, 14},
	{9, 5}, {9, 6}, {9, 7}, {9, 8}, {9, 9}, {9, 10}, {9, 11}, {9, 12}, {9, 13}, {9, 14},
	{10, 5}, {10, 6}, {10, 7}, {10, 8}, {10, 9}, {10, 10}, {10, 11}, {10, 12}, {10, 13}, {10, 14},
	{11, 5}, {11, 6}, {11, 7}, {11, 8}, {11, 9}, {11, 10}, {11, 11}, {11, 12}, {11, 13}, {11, 14},
	{12, 13}, {12, 14},
	{13, 14},
	{15, 16}
};
*/



// STMT STMT
// select s such that Next(s, s1), return all prev stmts
Query NextStmtStmtInClause = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::NEXT, "s", "s1", ArgumentTypes::STMT, ArgumentTypes::STMT));

// select s such that Next(s1, s2), return all stmts
Query NextStmtStmtNotInClause = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::NEXT, "s1", "s2", ArgumentTypes::STMT, ArgumentTypes::STMT));

// STMT PROG_LINE/ PROG_LINE STMT
// select s such that Next(s, p), return all prev stmts
Query NextStmtProgLineInClause = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::NEXT, "s", "p", ArgumentTypes::STMT, ArgumentTypes::PROG_LINE));

// select s such that Next(p, s), return all next stmts
Query NextProgLineStmtInClause = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::NEXT, "p", "s", ArgumentTypes::PROG_LINE, ArgumentTypes::STMT));

// STMT SYN/ PROG_LINE SYN
// select s such that Next(s, a), return all prev stmts of assign
Query NextStmtAssign = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::NEXT, "s", "a", ArgumentTypes::STMT, ArgumentTypes::ASSIGN));

// select s such that Next(s, pr), return all prev stmts of print
Query NextStmtPrint = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::NEXT, "s", "pr", ArgumentTypes::STMT, ArgumentTypes::PRINT));

// select s such that Next(s, r), return all prev stmts of read
Query NextStmtRead = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::NEXT, "s", "r", ArgumentTypes::STMT, ArgumentTypes::READ));

// select p such that Next(p, ifs), return all prev stmts of if
Query NextProgLineIf = Query("p", ArgumentTypes::PROG_LINE, QuerySuchThatClause(RelRef::NEXT, "p", "ifs", ArgumentTypes::PROG_LINE, ArgumentTypes::IF));

// select p such that Next(p, w), return all prev stmts of w
Query NextProgLineWhile = Query("p", ArgumentTypes::PROG_LINE, QuerySuchThatClause(RelRef::NEXT, "p", "w", ArgumentTypes::PROG_LINE, ArgumentTypes::WHILE));

// select p such that Next(p, c), return all prev stmts of call
Query NextProgLineCall = Query("p", ArgumentTypes::PROG_LINE, QuerySuchThatClause(RelRef::NEXT, "p", "c", ArgumentTypes::PROG_LINE, ArgumentTypes::CALL));

// STMT INT
// select s such that Next(s, 11), return all prev stmts of stmt 11
Query NextStmtIntTrue = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::NEXT, "s", "11", ArgumentTypes::STMT, ArgumentTypes::INT));

// select p such that Next(p, 1), return nth
Query NextProgLineIntFalse = Query("p", ArgumentTypes::PROG_LINE, QuerySuchThatClause(RelRef::NEXT, "p", "1", ArgumentTypes::PROG_LINE, ArgumentTypes::INT));

// STMT ANY/ PROG_LINE ANY
// select s such that Next(s, _), return all prev stmts
Query NextStmtAny = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::NEXT, "s", "_", ArgumentTypes::STMT, ArgumentTypes::ANY));

// select p such that Next(p, _), return all prev stmts
Query NextProgLineAny = Query("p", ArgumentTypes::PROG_LINE, QuerySuchThatClause(RelRef::NEXT, "p", "_", ArgumentTypes::PROG_LINE, ArgumentTypes::ANY));

// SYN STMT/ SYN PROG_LINE
// select p such that Next(a, p), return all next stmts of assign
Query NextAssignProgLine = Query("p", ArgumentTypes::PROG_LINE, QuerySuchThatClause(RelRef::NEXT, "a", "p", ArgumentTypes::ASSIGN, ArgumentTypes::PROG_LINE));

// select p such that Next(pr, p), return all next stmts of print
Query NextPrintProgLine = Query("p", ArgumentTypes::PROG_LINE, QuerySuchThatClause(RelRef::NEXT, "pr", "p", ArgumentTypes::PRINT, ArgumentTypes::PROG_LINE));

// select p such that Next(r, p), return all next stmts of read
Query NextReadProgLine = Query("p", ArgumentTypes::PROG_LINE, QuerySuchThatClause(RelRef::NEXT, "r", "p", ArgumentTypes::READ, ArgumentTypes::PROG_LINE));

// select s such that Next(ifs, s), return all next stmts of if
Query NextIfStmt = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::NEXT, "ifs", "s", ArgumentTypes::IF, ArgumentTypes::STMT));

// select s such that Next(w, s), return all next stmts of while
Query NextWhileStmt = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::NEXT, "ifs", "s", ArgumentTypes::WHILE, ArgumentTypes::STMT));

// select s such that Next(c, s), return all next stmts of call
Query NextCallStmt = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::NEXT, "c", "s", ArgumentTypes::CALL, ArgumentTypes::STMT));

// SYN SYN
// ASSIGN
// select a such that Next(a, a1), return prev stmts of assign that are assign
Query NextAssignAssign = Query("a", ArgumentTypes::ASSIGN, QuerySuchThatClause(RelRef::NEXT, "a", "a1", ArgumentTypes::ASSIGN, ArgumentTypes::ASSIGN));

// select a such that Next(a, pr), return prev stmts of print that are assign, nothing
Query NextAssignPrint = Query("a", ArgumentTypes::ASSIGN, QuerySuchThatClause(RelRef::NEXT, "a", "pr", ArgumentTypes::ASSIGN, ArgumentTypes::PRINT));

// select a such that Next(a, r), return prev stmts of read that are assign
Query NextAssignRead = Query("a", ArgumentTypes::ASSIGN, QuerySuchThatClause(RelRef::NEXT, "a", "r", ArgumentTypes::ASSIGN, ArgumentTypes::READ));

// select a such that Next(a, ifs), return prev stmts of if that are assign
Query NextAssignIf = Query("a", ArgumentTypes::ASSIGN, QuerySuchThatClause(RelRef::NEXT, "a", "ifs", ArgumentTypes::ASSIGN, ArgumentTypes::IF));

// select a such that Next(a, w), return prev stmts of while that are assign
Query NextAssignWhile = Query("a", ArgumentTypes::ASSIGN, QuerySuchThatClause(RelRef::NEXT, "a", "w", ArgumentTypes::ASSIGN, ArgumentTypes::WHILE));

// select a such that Next(a, c), return prev stmts of call that are assign
Query NextAssignCall = Query("a", ArgumentTypes::ASSIGN, QuerySuchThatClause(RelRef::NEXT, "a", "c", ArgumentTypes::ASSIGN, ArgumentTypes::CALL));

// PRINT
// select pr such that Next(pr, a), return prev stmts of assign that are print
Query NextPrintAssign = Query("pr", ArgumentTypes::PRINT, QuerySuchThatClause(RelRef::NEXT, "pr", "a", ArgumentTypes::PRINT, ArgumentTypes::ASSIGN));

// select pr such that Next(pr, pr1), return prev stmts of print that are print
Query NextPrintPrint = Query("pr", ArgumentTypes::PRINT, QuerySuchThatClause(RelRef::NEXT, "pr", "pr1", ArgumentTypes::PRINT, ArgumentTypes::PRINT));

// select pr such that Next(pr, r), return prev stmts of read that are print
Query NextPrintRead = Query("pr", ArgumentTypes::PRINT, QuerySuchThatClause(RelRef::NEXT, "pr", "r", ArgumentTypes::PRINT, ArgumentTypes::READ));

// select pr such that Next(pr, ifs), return prev stmts of if that are print
Query NextPrintIf = Query("pr", ArgumentTypes::PRINT, QuerySuchThatClause(RelRef::NEXT, "pr", "ifs", ArgumentTypes::PRINT, ArgumentTypes::IF));

// select pr such that Next(pr, w), return prev stmts of while that are print
Query NextPrintWhile = Query("pr", ArgumentTypes::PRINT, QuerySuchThatClause(RelRef::NEXT, "pr", "w", ArgumentTypes::PRINT, ArgumentTypes::WHILE));

// select pr such that Next(pr, c), return prev stmts of call that are print
Query NextPrintCall = Query("pr", ArgumentTypes::PRINT, QuerySuchThatClause(RelRef::NEXT, "pr", "c", ArgumentTypes::PRINT, ArgumentTypes::CALL));

// READ
// select r such that Next(r, a), return prev stmts of assign that are read
Query NextReadAssign = Query("r", ArgumentTypes::READ, QuerySuchThatClause(RelRef::NEXT, "r", "a", ArgumentTypes::READ, ArgumentTypes::ASSIGN));

// select r such that Next(r, pr1), return prev stmts of print that are read
Query NextReadPrint = Query("r", ArgumentTypes::READ, QuerySuchThatClause(RelRef::NEXT, "r", "pr", ArgumentTypes::READ, ArgumentTypes::PRINT));

// select r such that Next(r, r1), return prev stmts of read that are read
Query NextReadRead = Query("r", ArgumentTypes::READ, QuerySuchThatClause(RelRef::NEXT, "r", "r1", ArgumentTypes::READ, ArgumentTypes::READ));

// select r such that Next(r, ifs), return prev stmts of if that are read
Query NextReadIf = Query("r", ArgumentTypes::READ, QuerySuchThatClause(RelRef::NEXT, "r", "ifs", ArgumentTypes::READ, ArgumentTypes::IF));

// select r such that Next(r, w), return prev stmts of while that are read
Query NextReadWhile = Query("r", ArgumentTypes::READ, QuerySuchThatClause(RelRef::NEXT, "r", "w", ArgumentTypes::READ, ArgumentTypes::WHILE));

// select r such that Next(r, c), return prev stmts of call that are read
Query NextReadCall = Query("r", ArgumentTypes::READ, QuerySuchThatClause(RelRef::NEXT, "r", "c", ArgumentTypes::READ, ArgumentTypes::CALL));

// IF
// select ifs such that Next(ifs, a), return prev stmts of assign that are if
Query NextIfAssign = Query("ifs", ArgumentTypes::IF, QuerySuchThatClause(RelRef::NEXT, "ifs", "a", ArgumentTypes::IF, ArgumentTypes::ASSIGN));

// select ifs such that Next(ifs, pr1), return prev stmts of print that are if
Query NextIfPrint = Query("ifs", ArgumentTypes::IF, QuerySuchThatClause(RelRef::NEXT, "ifs", "pr", ArgumentTypes::IF, ArgumentTypes::PRINT));

// select ifs such that Next(ifs, r), return prev stmts of read that are if
Query NextIfRead = Query("ifs", ArgumentTypes::IF, QuerySuchThatClause(RelRef::NEXT, "ifs", "r", ArgumentTypes::IF, ArgumentTypes::READ));

// select ifs such that Next(ifs, ifs1), return prev stmts of if that are if
Query NextIfIf = Query("ifs", ArgumentTypes::IF, QuerySuchThatClause(RelRef::NEXT, "ifs", "ifs1", ArgumentTypes::IF, ArgumentTypes::IF));

// select ifs such that Next(ifs, w), return prev stmts of while that are if
Query NextIfWhile = Query("ifs", ArgumentTypes::IF, QuerySuchThatClause(RelRef::NEXT, "ifs", "w", ArgumentTypes::IF, ArgumentTypes::WHILE));

// select ifs such that Next(ifs, c), return prev stmts of call that are if
Query NextIfCall = Query("ifs", ArgumentTypes::IF, QuerySuchThatClause(RelRef::NEXT, "ifs", "c", ArgumentTypes::IF, ArgumentTypes::CALL));

// WHILE
// select w such that Next(w, a), return prev stmts of assign that are while
Query NextWhileAssign = Query("w", ArgumentTypes::WHILE, QuerySuchThatClause(RelRef::NEXT, "w", "a", ArgumentTypes::WHILE, ArgumentTypes::ASSIGN));

// select w such that Next(w, pr1), return prev stmts of print that are while
Query NextWhilePrint = Query("w", ArgumentTypes::WHILE, QuerySuchThatClause(RelRef::NEXT, "w", "pr1", ArgumentTypes::WHILE, ArgumentTypes::PRINT));

// select w such that Next(w, r), return prev stmts of read that are while
Query NextWhileRead = Query("w", ArgumentTypes::WHILE, QuerySuchThatClause(RelRef::NEXT, "w", "r", ArgumentTypes::WHILE, ArgumentTypes::READ));

// select w such that Next(w, ifs1), return prev stmts of if that are while
Query NextWhileIf = Query("w", ArgumentTypes::WHILE, QuerySuchThatClause(RelRef::NEXT, "w", "ifs", ArgumentTypes::WHILE, ArgumentTypes::IF));

// select w such that Next(w, w1), return prev stmts of while that are while
Query NextWhileWhile = Query("w", ArgumentTypes::WHILE, QuerySuchThatClause(RelRef::NEXT, "w", "w1", ArgumentTypes::WHILE, ArgumentTypes::WHILE));

// select w such that Next(w, c), return prev stmts of call that are while
Query NextWhileCall = Query("w", ArgumentTypes::WHILE, QuerySuchThatClause(RelRef::NEXT, "w", "c", ArgumentTypes::WHILE, ArgumentTypes::CALL));

// CALL
// select c such that Next(c, a), return prev stmts of assign that are call
Query NextCallAssign = Query("c", ArgumentTypes::CALL, QuerySuchThatClause(RelRef::NEXT, "c", "a", ArgumentTypes::CALL, ArgumentTypes::ASSIGN));

// select c such that Next(c, pr1), return prev stmts of print that are call
Query NextCallPrint = Query("c", ArgumentTypes::CALL, QuerySuchThatClause(RelRef::NEXT, "c", "pr1", ArgumentTypes::CALL, ArgumentTypes::PRINT));

// select c such that Next(c, r), return prev stmts of read that are call
Query NextCallRead = Query("c", ArgumentTypes::CALL, QuerySuchThatClause(RelRef::NEXT, "c", "r", ArgumentTypes::CALL, ArgumentTypes::READ));

// select c such that Next(c, ifs1), return prev stmts of if that are call
Query NextCallIf = Query("c", ArgumentTypes::CALL, QuerySuchThatClause(RelRef::NEXT, "c", "ifs", ArgumentTypes::CALL, ArgumentTypes::IF));

// select c such that Next(c, w1), return prev stmts of while that are call
Query NextCallWhile = Query("c", ArgumentTypes::CALL, QuerySuchThatClause(RelRef::NEXT, "c", "w1", ArgumentTypes::CALL, ArgumentTypes::WHILE));

// select c such that Next(c, c1), return prev stmts of call that are call
Query NextCallCall = Query("c", ArgumentTypes::CALL, QuerySuchThatClause(RelRef::NEXT, "c", "c1", ArgumentTypes::CALL, ArgumentTypes::CALL));

// INT STMT/ INT PROG_LINE
// select s such that Next("7", s), return next stmts of stmt 7
Query NextIntStmtTrue = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::NEXT, "7", "s", ArgumentTypes::INT, ArgumentTypes::STMT));

// select p such that Next("16", p), return next stmts of stmt 16, returns nothing
Query NextIntProgLineFalse = Query("p", ArgumentTypes::PROG_LINE, QuerySuchThatClause(RelRef::NEXT, "16", "p", ArgumentTypes::INT, ArgumentTypes::PROG_LINE));

// INT SYN
// select s such that Next("1", a), return all stmts
Query NextIntAssignTrue = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::NEXT, "1", "a", ArgumentTypes::INT, ArgumentTypes::ASSIGN));

// select s such that Next("7", a), return nothing
Query NextIntAssignFalse = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::NEXT, "7", "a", ArgumentTypes::INT, ArgumentTypes::ASSIGN));

// select s such that Next("3", pr), return all stmts
Query NextIntPrintTrue = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::NEXT, "3", "pr", ArgumentTypes::INT, ArgumentTypes::PRINT));

// select s such that Next("1", pr), return nothing
Query NextIntPrintFalse = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::NEXT, "1", "pr", ArgumentTypes::INT, ArgumentTypes::PRINT));

// select s such that Next("5", r), return all stmts
Query NextIntReadTrue = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::NEXT, "5", "r", ArgumentTypes::INT, ArgumentTypes::READ));

// select s such that Next("1", r), return nothing
Query NextIntReadFalse = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::NEXT, "1", "r", ArgumentTypes::INT, ArgumentTypes::READ));

// select p such that Next("2", ifs), return all stmts
Query NextIntIfTrue = Query("p", ArgumentTypes::PROG_LINE, QuerySuchThatClause(RelRef::NEXT, "2", "ifs", ArgumentTypes::INT, ArgumentTypes::IF));

// select p such that Next("14", ifs), return nothing
Query NextIntIfFalse = Query("p", ArgumentTypes::PROG_LINE, QuerySuchThatClause(RelRef::NEXT, "14", "ifs", ArgumentTypes::INT, ArgumentTypes::IF));

// select p such that Next("4", w), return all stmts
Query NextIntWhileTrue = Query("p", ArgumentTypes::PROG_LINE, QuerySuchThatClause(RelRef::NEXT, "4", "w", ArgumentTypes::INT, ArgumentTypes::WHILE));

// select p such that Next("14", w), return nothing
Query NextIntWhileFalse = Query("p", ArgumentTypes::PROG_LINE, QuerySuchThatClause(RelRef::NEXT, "14", "w", ArgumentTypes::INT, ArgumentTypes::WHILE));

// select p such that Next("7", c), return all stmts
Query NextIntCallTrue = Query("p", ArgumentTypes::PROG_LINE, QuerySuchThatClause(RelRef::NEXT, "7", "c", ArgumentTypes::INT, ArgumentTypes::CALL));

// select p such that Next("14", c), return nothing
Query NextIntCallFalse = Query("p", ArgumentTypes::PROG_LINE, QuerySuchThatClause(RelRef::NEXT, "14", "c", ArgumentTypes::INT, ArgumentTypes::CALL));

// INT INT
// select p such that Next("11", "5"), return all stmts
Query NextIntIntTrue = Query("p", ArgumentTypes::PROG_LINE, QuerySuchThatClause(RelRef::NEXT, "11", "5", ArgumentTypes::INT, ArgumentTypes::INT));

// select p such that Next("14", "6"), return all stmts
Query NextIntIntFalse = Query("p", ArgumentTypes::PROG_LINE, QuerySuchThatClause(RelRef::NEXT, "14", "6", ArgumentTypes::INT, ArgumentTypes::INT));

// INT ANY
// select p such that Next("11", "_"), return all stmts
Query NextIntAnyTrue = Query("p", ArgumentTypes::PROG_LINE, QuerySuchThatClause(RelRef::NEXT, "11", "_", ArgumentTypes::INT, ArgumentTypes::ANY));

// select p such that Next("14", "_"), return all stmts
Query NextIntAnyFalse = Query("p", ArgumentTypes::PROG_LINE, QuerySuchThatClause(RelRef::NEXT, "14", "_", ArgumentTypes::INT, ArgumentTypes::ANY));

// ANY STMT/ ANY PROG_LINE
// select p such that Next("_", p), return all next stmts
Query NextAnyProgLineTrue = Query("p", ArgumentTypes::PROG_LINE, QuerySuchThatClause(RelRef::NEXT, "_", "p", ArgumentTypes::ANY, ArgumentTypes::PROG_LINE));

// select s such that Next("_", s), return all next stmts
Query NextAnyStmtTrue = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::NEXT, "_", "s", ArgumentTypes::ANY, ArgumentTypes::STMT));

// ANY SYN
// select s such that Next("_", a), return all stmts
Query NextAnyAssign = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::NEXT, "_", "a", ArgumentTypes::ANY, ArgumentTypes::ASSIGN));

// select s such that Next("_", pr), return all stmts
Query NextAnyPrint = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::NEXT, "_", "pr", ArgumentTypes::ANY, ArgumentTypes::PRINT));

// select s such that Next("_", r), return all stmts
Query NextAnyRead = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::NEXT, "_", "r", ArgumentTypes::ANY, ArgumentTypes::READ));

// select s such that Next("_", ifs), return all stmts
Query NextAnyIf = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::NEXT, "_", "ifs", ArgumentTypes::ANY, ArgumentTypes::IF));

// select s such that Next("_", w), return all stmts
Query NextAnyWhile = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::NEXT, "_", "w", ArgumentTypes::ANY, ArgumentTypes::WHILE));

// select s such that Next("_", c), return all stmts
Query NextAnyCall = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::NEXT, "_", "c", ArgumentTypes::ANY, ArgumentTypes::CALL));

// ANY INT
// select s such that Next("_", "2"), return all stmts
Query NextAnyIntTrue = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::NEXT, "_", "2", ArgumentTypes::ANY, ArgumentTypes::INT));

// select s such that Next("_", "1"), return nothing
Query NextAnyIntFalse = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::NEXT, "_", "1", ArgumentTypes::ANY, ArgumentTypes::INT));

// ANY ANY
// select s such that Next("_", "_"), return all stmts
Query NextAnyAny = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::NEXT, "_", "_", ArgumentTypes::ANY, ArgumentTypes::ANY));

// EDGE CASE
// select s such that Next(s, s), return nothing
Query NextEdgeCase1 = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::NEXT, "s", "s", ArgumentTypes::STMT, ArgumentTypes::ANY));

// select s such that Next(5, 5), return nothing
Query NextEdgeCase2 = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::NEXT, "5", "5", ArgumentTypes::INT, ArgumentTypes::INT));
