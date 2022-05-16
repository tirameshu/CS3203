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

std::unordered_set<std::string> nextTAllStmt = { "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15", "16" };
std::unordered_set<std::string> nextTPrevList = { "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "15" };
std::unordered_set<std::string> nextTNextList = { "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15", "16" };

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
// select s such that NextT(s, s1), return all prev stmts
Query NextTStmtStmtInClause = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::NEXT_T, "s", "s1", ArgumentTypes::STMT, ArgumentTypes::STMT));

// select s such that NextT(s1, s2), return all stmts
Query NextTStmtStmtNotInClause = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::NEXT_T, "s1", "s2", ArgumentTypes::STMT, ArgumentTypes::STMT));

// STMT PROG_LINE/ PROG_LINE STMT
// select s such that NextT(s, p), return all prev stmts
Query NextTStmtProgLineInClause = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::NEXT_T, "s", "p", ArgumentTypes::STMT, ArgumentTypes::PROG_LINE));

// select s such that NextT(p, s), return all next stmts
Query NextTProgLineStmtInClause = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::NEXT_T, "p", "s", ArgumentTypes::PROG_LINE, ArgumentTypes::STMT));

// STMT SYN/ PROG_LINE SYN
// select s such that NextT(s, a), return all prevT stmts of all assign stmts
Query NextTStmtAssign = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::NEXT_T, "s", "a", ArgumentTypes::STMT, ArgumentTypes::ASSIGN));

// select s such that NextT(s, pr), return all prevT stmts of all print stmts
Query NextTStmtPrint = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::NEXT_T, "s", "pr", ArgumentTypes::STMT, ArgumentTypes::PRINT));

// select s such that NextT(s, r), return all prevT stmts of all read stmts
Query NextTStmtRead = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::NEXT_T, "s", "r", ArgumentTypes::STMT, ArgumentTypes::READ));

// select p such that NextT(p, ifs), return all prevT stmts of all if stmts
Query NextTProgLineIf = Query("p", ArgumentTypes::PROG_LINE, QuerySuchThatClause(RelRef::NEXT_T, "p", "ifs", ArgumentTypes::PROG_LINE, ArgumentTypes::IF));

// select p such that NextT(p, w), return all prevT stmts of all while stmts
Query NextTProgLineWhile = Query("p", ArgumentTypes::PROG_LINE, QuerySuchThatClause(RelRef::NEXT_T, "p", "w", ArgumentTypes::PROG_LINE, ArgumentTypes::WHILE));

// select p such that NextT(p, c), return all prevT stmts of all call stmts
Query NextTProgLineCall = Query("p", ArgumentTypes::PROG_LINE, QuerySuchThatClause(RelRef::NEXT_T, "p", "c", ArgumentTypes::PROG_LINE, ArgumentTypes::CALL));

// STMT INT
// select s such that NextT(s, 11), return all prevT stmts of stmt 11
Query NextTStmtIntTrue = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::NEXT_T, "s", "11", ArgumentTypes::STMT, ArgumentTypes::INT));

// select p such that NextT(p, 1), return nth
Query NextTProgLineIntFalse = Query("p", ArgumentTypes::PROG_LINE, QuerySuchThatClause(RelRef::NEXT_T, "p", "1", ArgumentTypes::PROG_LINE, ArgumentTypes::INT));

// STMT ANY/ PROG_LINE ANY
// select s such that NextT(s, _), return all prev stmts
Query NextTStmtAny = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::NEXT_T, "s", "_", ArgumentTypes::STMT, ArgumentTypes::ANY));

// select p such that NextT(p, _), return all prev stmts
Query NextTProgLineAny = Query("p", ArgumentTypes::PROG_LINE, QuerySuchThatClause(RelRef::NEXT_T, "p", "_", ArgumentTypes::PROG_LINE, ArgumentTypes::ANY));

// SYN STMT/ SYN PROG_LINE
// select p such that NextT(a, p), return all nextT stmts of assign
Query NextTAssignProgLine = Query("p", ArgumentTypes::PROG_LINE, QuerySuchThatClause(RelRef::NEXT_T, "a", "p", ArgumentTypes::ASSIGN, ArgumentTypes::PROG_LINE));

// select p such that NextT(pr, p), return all next stmts of print
Query NextTPrintProgLine = Query("p", ArgumentTypes::PROG_LINE, QuerySuchThatClause(RelRef::NEXT_T, "pr", "p", ArgumentTypes::PRINT, ArgumentTypes::PROG_LINE));

// select p such that NextT(r, p), return all next stmts of read
Query NextTReadProgLine = Query("p", ArgumentTypes::PROG_LINE, QuerySuchThatClause(RelRef::NEXT_T, "r", "p", ArgumentTypes::READ, ArgumentTypes::PROG_LINE));

// select s such that NextT(ifs, s), return all next stmts of if
Query NextTIfStmt = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::NEXT_T, "ifs", "s", ArgumentTypes::IF, ArgumentTypes::STMT));

// select s such that NextT(w, s), return all next stmts of while
Query NextTWhileStmt = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::NEXT_T, "ifs", "s", ArgumentTypes::WHILE, ArgumentTypes::STMT));

// select s such that NextT(c, s), return all next stmts of call
Query NextTCallStmt = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::NEXT_T, "c", "s", ArgumentTypes::CALL, ArgumentTypes::STMT));

// SYN SYN
// ASSIGN
// select a such that NextT(a, a1), return prevT stmts of assign that are assign
Query NextTAssignAssign = Query("a", ArgumentTypes::ASSIGN, QuerySuchThatClause(RelRef::NEXT_T, "a", "a1", ArgumentTypes::ASSIGN, ArgumentTypes::ASSIGN));

// select a such that NextT(a, pr), return prevT stmts of print that are assign, nothing
Query NextTAssignPrint = Query("a", ArgumentTypes::ASSIGN, QuerySuchThatClause(RelRef::NEXT_T, "a", "pr", ArgumentTypes::ASSIGN, ArgumentTypes::PRINT));

// select a such that NextT(a, r), return prevT stmts of read that are assign
Query NextTAssignRead = Query("a", ArgumentTypes::ASSIGN, QuerySuchThatClause(RelRef::NEXT_T, "a", "r", ArgumentTypes::ASSIGN, ArgumentTypes::READ));

// select a such that NextT(a, ifs), return prevT stmts of if that are assign
Query NextTAssignIf = Query("a", ArgumentTypes::ASSIGN, QuerySuchThatClause(RelRef::NEXT_T, "a", "ifs", ArgumentTypes::ASSIGN, ArgumentTypes::IF));

// select a such that NextT(a, w), return prevT stmts of while that are assign
Query NextTAssignWhile = Query("a", ArgumentTypes::ASSIGN, QuerySuchThatClause(RelRef::NEXT_T, "a", "w", ArgumentTypes::ASSIGN, ArgumentTypes::WHILE));

// select a such that NextT(a, c), return prevT stmts of call that are assign
Query NextTAssignCall = Query("a", ArgumentTypes::ASSIGN, QuerySuchThatClause(RelRef::NEXT_T, "a", "c", ArgumentTypes::ASSIGN, ArgumentTypes::CALL));

// PRINT
// select pr such that NextT(pr, a), return prevT stmts of assign that are print
Query NextTPrintAssign = Query("pr", ArgumentTypes::PRINT, QuerySuchThatClause(RelRef::NEXT_T, "pr", "a", ArgumentTypes::PRINT, ArgumentTypes::ASSIGN));

// select pr such that NextT(pr, pr1), return prevT stmts of print that are print
Query NextTPrintPrint = Query("pr", ArgumentTypes::PRINT, QuerySuchThatClause(RelRef::NEXT_T, "pr", "pr1", ArgumentTypes::PRINT, ArgumentTypes::PRINT));

// select pr such that NextT(pr, r), return prevT stmts of read that are print
Query NextTPrintRead = Query("pr", ArgumentTypes::PRINT, QuerySuchThatClause(RelRef::NEXT_T, "pr", "r", ArgumentTypes::PRINT, ArgumentTypes::READ));

// select pr such that NextT(pr, ifs), return prevT stmts of if that are print
Query NextTPrintIf = Query("pr", ArgumentTypes::PRINT, QuerySuchThatClause(RelRef::NEXT_T, "pr", "ifs", ArgumentTypes::PRINT, ArgumentTypes::IF));

// select pr such that NextT(pr, w), return prevT stmts of while that are print
Query NextTPrintWhile = Query("pr", ArgumentTypes::PRINT, QuerySuchThatClause(RelRef::NEXT_T, "pr", "w", ArgumentTypes::PRINT, ArgumentTypes::WHILE));

// select pr such that NextT(pr, c), return prevT stmts of call that are print
Query NextTPrintCall = Query("pr", ArgumentTypes::PRINT, QuerySuchThatClause(RelRef::NEXT_T, "pr", "c", ArgumentTypes::PRINT, ArgumentTypes::CALL));

// READ
// select r such that NextT(r, a), return prevT stmts of assign that are read
Query NextTReadAssign = Query("r", ArgumentTypes::READ, QuerySuchThatClause(RelRef::NEXT_T, "r", "a", ArgumentTypes::READ, ArgumentTypes::ASSIGN));

// select r such that NextT(r, pr1), return prevT stmts of print that are read
Query NextTReadPrint = Query("r", ArgumentTypes::READ, QuerySuchThatClause(RelRef::NEXT_T, "r", "pr", ArgumentTypes::READ, ArgumentTypes::PRINT));

// select r such that NextT(r, r1), return prevT stmts of read that are read
Query NextTReadRead = Query("r", ArgumentTypes::READ, QuerySuchThatClause(RelRef::NEXT_T, "r", "r1", ArgumentTypes::READ, ArgumentTypes::READ));

// select r such that NextT(r, ifs), return prevT stmts of if that are read
Query NextTReadIf = Query("r", ArgumentTypes::READ, QuerySuchThatClause(RelRef::NEXT_T, "r", "ifs", ArgumentTypes::READ, ArgumentTypes::IF));

// select r such that NextT(r, w), return prevT stmts of while that are read
Query NextTReadWhile = Query("r", ArgumentTypes::READ, QuerySuchThatClause(RelRef::NEXT_T, "r", "w", ArgumentTypes::READ, ArgumentTypes::WHILE));

// select r such that NextT(r, c), return prevT stmts of call that are read
Query NextTReadCall = Query("r", ArgumentTypes::READ, QuerySuchThatClause(RelRef::NEXT_T, "r", "c", ArgumentTypes::READ, ArgumentTypes::CALL));

// IF
// select ifs such that NextT(ifs, a), return prevT stmts of assign that are if
Query NextTIfAssign = Query("ifs", ArgumentTypes::IF, QuerySuchThatClause(RelRef::NEXT_T, "ifs", "a", ArgumentTypes::IF, ArgumentTypes::ASSIGN));

// select ifs such that NextT(ifs, pr1), return prevT stmts of print that are if
Query NextTIfPrint = Query("ifs", ArgumentTypes::IF, QuerySuchThatClause(RelRef::NEXT_T, "ifs", "pr", ArgumentTypes::IF, ArgumentTypes::PRINT));

// select ifs such that NextT(ifs, r), return prevT stmts of read that are if
Query NextTIfRead = Query("ifs", ArgumentTypes::IF, QuerySuchThatClause(RelRef::NEXT_T, "ifs", "r", ArgumentTypes::IF, ArgumentTypes::READ));

// select ifs such that NextT(ifs, ifs1), return prevT stmts of if that are if
Query NextTIfIf = Query("ifs", ArgumentTypes::IF, QuerySuchThatClause(RelRef::NEXT_T, "ifs", "ifs1", ArgumentTypes::IF, ArgumentTypes::IF));

// select ifs such that NextT(ifs, w), return prevT stmts of while that are if
Query NextTIfWhile = Query("ifs", ArgumentTypes::IF, QuerySuchThatClause(RelRef::NEXT_T, "ifs", "w", ArgumentTypes::IF, ArgumentTypes::WHILE));

// select ifs such that NextT(ifs, c), return prevT stmts of call that are if
Query NextTIfCall = Query("ifs", ArgumentTypes::IF, QuerySuchThatClause(RelRef::NEXT_T, "ifs", "c", ArgumentTypes::IF, ArgumentTypes::CALL));

// WHILE
// select w such that NextT(w, a), return prevT stmts of assign that are while
Query NextTWhileAssign = Query("w", ArgumentTypes::WHILE, QuerySuchThatClause(RelRef::NEXT_T, "w", "a", ArgumentTypes::WHILE, ArgumentTypes::ASSIGN));

// select w such that NextT(w, pr1), return prevT stmts of print that are while
Query NextTWhilePrint = Query("w", ArgumentTypes::WHILE, QuerySuchThatClause(RelRef::NEXT_T, "w", "pr1", ArgumentTypes::WHILE, ArgumentTypes::PRINT));

// select w such that NextT(w, r), return prevT stmts of read that are while
Query NextTWhileRead = Query("w", ArgumentTypes::WHILE, QuerySuchThatClause(RelRef::NEXT_T, "w", "r", ArgumentTypes::WHILE, ArgumentTypes::READ));

// select w such that NextT(w, ifs1), return prevT stmts of if that are while
Query NextTWhileIf = Query("w", ArgumentTypes::WHILE, QuerySuchThatClause(RelRef::NEXT_T, "w", "ifs", ArgumentTypes::WHILE, ArgumentTypes::IF));

// select w such that NextT(w, w1), return prevT stmts of while that are while
Query NextTWhileWhile = Query("w", ArgumentTypes::WHILE, QuerySuchThatClause(RelRef::NEXT_T, "w", "w1", ArgumentTypes::WHILE, ArgumentTypes::WHILE));

// select w such that NextT(w, c), return prevT stmts of call that are while
Query NextTWhileCall = Query("w", ArgumentTypes::WHILE, QuerySuchThatClause(RelRef::NEXT_T, "w", "c", ArgumentTypes::WHILE, ArgumentTypes::CALL));

// CALL
// select c such that NextT(c, a), return prevT stmts of assign that are call
Query NextTCallAssign = Query("c", ArgumentTypes::CALL, QuerySuchThatClause(RelRef::NEXT_T, "c", "a", ArgumentTypes::CALL, ArgumentTypes::ASSIGN));

// select c such that NextT(c, pr1), return prevT stmts of print that are call
Query NextTCallPrint = Query("c", ArgumentTypes::CALL, QuerySuchThatClause(RelRef::NEXT_T, "c", "pr1", ArgumentTypes::CALL, ArgumentTypes::PRINT));

// select c such that NextT(c, r), return prevT stmts of read that are call
Query NextTCallRead = Query("c", ArgumentTypes::CALL, QuerySuchThatClause(RelRef::NEXT_T, "c", "r", ArgumentTypes::CALL, ArgumentTypes::READ));

// select c such that NextT(c, ifs1), return prevT stmts of if that are call
Query NextTCallIf = Query("c", ArgumentTypes::CALL, QuerySuchThatClause(RelRef::NEXT_T, "c", "ifs", ArgumentTypes::CALL, ArgumentTypes::IF));

// select c such that NextT(c, w1), return prevT stmts of while that are call
Query NextTCallWhile = Query("c", ArgumentTypes::CALL, QuerySuchThatClause(RelRef::NEXT_T, "c", "w1", ArgumentTypes::CALL, ArgumentTypes::WHILE));

// select c such that NextT(c, c1), return prevT stmts of call that are call
Query NextTCallCall = Query("c", ArgumentTypes::CALL, QuerySuchThatClause(RelRef::NEXT_T, "c", "c1", ArgumentTypes::CALL, ArgumentTypes::CALL));

// INT STMT/ INT PROG_LINE
// select s such that NextT("7", s), return next stmts of stmt 7
Query NextTIntStmtTrue = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::NEXT_T, "7", "s", ArgumentTypes::INT, ArgumentTypes::STMT));

// select p such that NextT("16", p), return next stmts of stmt 16, returns nothing
Query NextTIntProgLineFalse = Query("p", ArgumentTypes::PROG_LINE, QuerySuchThatClause(RelRef::NEXT_T, "16", "p", ArgumentTypes::INT, ArgumentTypes::PROG_LINE));

// INT SYN
// select s such that NextT("1", a), return all stmts
Query NextTIntAssignTrue = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::NEXT_T, "1", "a", ArgumentTypes::INT, ArgumentTypes::ASSIGN));

// select s such that NextT("3", pr), return all stmts
Query NextTIntPrintTrue = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::NEXT_T, "3", "pr", ArgumentTypes::INT, ArgumentTypes::PRINT));

// select s such that NextT("5", r), return all stmts
Query NextTIntReadTrue = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::NEXT_T, "5", "r", ArgumentTypes::INT, ArgumentTypes::READ));

// select p such that NextT("2", ifs), return all stmts
Query NextTIntIfTrue = Query("p", ArgumentTypes::PROG_LINE, QuerySuchThatClause(RelRef::NEXT_T, "2", "ifs", ArgumentTypes::INT, ArgumentTypes::IF));

// select p such that NextT("14", ifs), return nothing
Query NextTIntIfFalse = Query("p", ArgumentTypes::PROG_LINE, QuerySuchThatClause(RelRef::NEXT_T, "14", "ifs", ArgumentTypes::INT, ArgumentTypes::IF));

// select p such that NextT("4", w), return all stmts
Query NextTIntWhileTrue = Query("p", ArgumentTypes::PROG_LINE, QuerySuchThatClause(RelRef::NEXT_T, "4", "w", ArgumentTypes::INT, ArgumentTypes::WHILE));

// select p such that NextT("14", w), return nothing
Query NextTIntWhileFalse = Query("p", ArgumentTypes::PROG_LINE, QuerySuchThatClause(RelRef::NEXT_T, "14", "w", ArgumentTypes::INT, ArgumentTypes::WHILE));

// select p such that NextT("7", c), return all stmts
Query NextTIntCallTrue = Query("p", ArgumentTypes::PROG_LINE, QuerySuchThatClause(RelRef::NEXT_T, "7", "c", ArgumentTypes::INT, ArgumentTypes::CALL));

// select p such that NextT("14", c), return nothing
Query NextTIntCallFalse = Query("p", ArgumentTypes::PROG_LINE, QuerySuchThatClause(RelRef::NEXT_T, "14", "c", ArgumentTypes::INT, ArgumentTypes::CALL));

// INT INT
// select p such that NextT("11", "5"), return all stmts
Query NextTIntIntTrue = Query("p", ArgumentTypes::PROG_LINE, QuerySuchThatClause(RelRef::NEXT_T, "11", "5", ArgumentTypes::INT, ArgumentTypes::INT));

// select p such that NextT("14", "6"), return all stmts
Query NextTIntIntFalse = Query("p", ArgumentTypes::PROG_LINE, QuerySuchThatClause(RelRef::NEXT_T, "14", "6", ArgumentTypes::INT, ArgumentTypes::INT));

// INT ANY
// select p such that NextT("11", "_"), return all stmts
Query NextTIntAnyTrue = Query("p", ArgumentTypes::PROG_LINE, QuerySuchThatClause(RelRef::NEXT_T, "11", "_", ArgumentTypes::INT, ArgumentTypes::ANY));

// select p such that NextT("14", "_"), return all stmts
Query NextTIntAnyFalse = Query("p", ArgumentTypes::PROG_LINE, QuerySuchThatClause(RelRef::NEXT_T, "14", "_", ArgumentTypes::INT, ArgumentTypes::ANY));

// ANY STMT/ ANY PROG_LINE
// select p such that NextT("_", p), return all next stmts
Query NextTAnyProgLineTrue = Query("p", ArgumentTypes::PROG_LINE, QuerySuchThatClause(RelRef::NEXT_T, "_", "p", ArgumentTypes::ANY, ArgumentTypes::PROG_LINE));

// select s such that NextT("_", s), return all next stmts
Query NextTAnyStmtTrue = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::NEXT_T, "_", "s", ArgumentTypes::ANY, ArgumentTypes::STMT));

// ANY SYN
// select s such that NextT("_", a), return all stmts
Query NextTAnyAssign = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::NEXT_T, "_", "a", ArgumentTypes::ANY, ArgumentTypes::ASSIGN));

// select s such that NextT("_", pr), return all stmts
Query NextTAnyPrint = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::NEXT_T, "_", "pr", ArgumentTypes::ANY, ArgumentTypes::PRINT));

// select s such that NextT("_", r), return all stmts
Query NextTAnyRead = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::NEXT_T, "_", "r", ArgumentTypes::ANY, ArgumentTypes::READ));

// select s such that NextT("_", ifs), return all stmts
Query NextTAnyIf = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::NEXT_T, "_", "ifs", ArgumentTypes::ANY, ArgumentTypes::IF));

// select s such that NextT("_", w), return all stmts
Query NextTAnyWhile = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::NEXT_T, "_", "w", ArgumentTypes::ANY, ArgumentTypes::WHILE));

// select s such that NextT("_", c), return all stmts
Query NextTAnyCall = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::NEXT_T, "_", "c", ArgumentTypes::ANY, ArgumentTypes::CALL));

// ANY INT
// select s such that NextT("_", "2"), return all stmts
Query NextTAnyIntTrue = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::NEXT_T, "_", "2", ArgumentTypes::ANY, ArgumentTypes::INT));

// select s such that NextT("_", "1"), return nothing
Query NextTAnyIntFalse = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::NEXT_T, "_", "1", ArgumentTypes::ANY, ArgumentTypes::INT));

// ANY ANY
// select s such that NextT("_", "_"), return all stmts
Query NextTAnyAny = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::NEXT_T, "_", "_", ArgumentTypes::ANY, ArgumentTypes::ANY));

// EDGE CASE
// select s such that NextT(s, s), return nothing
Query NextTEdgeCase1 = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::NEXT_T, "s", "s", ArgumentTypes::STMT, ArgumentTypes::ANY));

// select s such that NextT(5, 5), SHOULD BE TRUE NOW, return all statements
Query NextTEdgeCase2 = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::NEXT_T, "5", "5", ArgumentTypes::INT, ArgumentTypes::INT));
