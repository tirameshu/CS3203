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

procedure testB {
17	  a = 10;
}
*/

std::unordered_set<std::string> followsAllStmt = { "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15", "16", "17" };
std::unordered_set<std::string> followsAllFollowed = { "1", "2", "4", "6", "8", "7", "5", "12", "13", "15" };
std::unordered_set<std::string> followsAllFollower = { "2", "3", "5", "7", "9", "11", "12", "13", "14", "16" };

// Follows(_, _) - if true, returns all stmtNo
Query FollowsAnyAny = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::FOLLOWS, "_", "_", ArgumentTypes::ANY, ArgumentTypes::ANY));

// Follows(_, s) - if true, return all followers stmtNo
Query FollowsAnyStmt = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::FOLLOWS, "_", "s", ArgumentTypes::ANY, ArgumentTypes::STMT));
Query InvalidFollowsAnyStmt = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::FOLLOWS, "_", "s1", ArgumentTypes::ANY, ArgumentTypes::STMT));

// Follows(s, _) - if true, return all stmtNo that are followed
Query FollowsStmtAny = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::FOLLOWS, "s", "_", ArgumentTypes::STMT, ArgumentTypes::ANY));
Query InvalidFollowsStmtAny = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::FOLLOWS, "s1", "_", ArgumentTypes::STMT, ArgumentTypes::ANY));

// Follows(s, s2) - if true, return all stmtNo that are followed
Query FollowsStmtStmt = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::FOLLOWS, "s", "s2", ArgumentTypes::STMT, ArgumentTypes::STMT));
// Follows(s, s) - return empty set
Query InvalidFollowsSameStmt = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::FOLLOWS, "s", "s", ArgumentTypes::STMT, ArgumentTypes::STMT));
Query InvalidFollowsStmtStmt = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::FOLLOWS, "s1", "s2", ArgumentTypes::STMT, ArgumentTypes::STMT));

// Follows(_, 9) - if true, return all stmtNo
Query FollowsAnyInt = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::FOLLOWS, "_", "9", ArgumentTypes::ANY, ArgumentTypes::INT));
Query InvalidFollowsAnyInt = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::FOLLOWS, "_", "10", ArgumentTypes::ANY, ArgumentTypes::INT));

// Follows(s, 11) - if true, return stmtNo of 7
Query FollowsStmtInt = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::FOLLOWS, "s", "11", ArgumentTypes::STMT, ArgumentTypes::INT));
Query InvalidFollowsStmtInt = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::FOLLOWS, "s1", "11", ArgumentTypes::STMT, ArgumentTypes::INT));

// Follows(_, a) - if true, return all stmtNo 
Query FollowsAnyAssign = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::FOLLOWS, "_", "a", ArgumentTypes::ANY, ArgumentTypes::ASSIGN));

// Follows(_, p) - if true, return all stmtNo
Query FollowsAnyPrint = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::FOLLOWS, "_", "p", ArgumentTypes::ANY, ArgumentTypes::PRINT));

// Follows(_, r) - if true, return all stmtNo
Query FollowsAnyRead = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::FOLLOWS, "_", "r", ArgumentTypes::ANY, ArgumentTypes::READ));

// Follows(_, i) - if true, return all stmtNo 
Query FollowsAnyIf = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::FOLLOWS, "_", "i", ArgumentTypes::ANY, ArgumentTypes::IF));

// Follows(_, w) - if true, return all stmtNo 
Query FollowsAnyWhile = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::FOLLOWS, "_", "w", ArgumentTypes::ANY, ArgumentTypes::WHILE));

// Follows(s, a) - if true, return all stmtNo that are followed of assign statements
Query FollowsStmtAssign = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::FOLLOWS, "s", "a", ArgumentTypes::STMT, ArgumentTypes::ASSIGN));
Query InvalidFollowsStmtAssign = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::FOLLOWS, "s1", "a", ArgumentTypes::STMT, ArgumentTypes::ASSIGN));

// Follows(s, p) - if true, return all stmtNo that are followed of print statements
Query FollowsStmtPrint = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::FOLLOWS, "s", "p", ArgumentTypes::STMT, ArgumentTypes::PRINT));
Query InvalidFollowsStmtPrint = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::FOLLOWS, "s1", "p", ArgumentTypes::STMT, ArgumentTypes::PRINT));

// Follows(s, r) - if true, return all stmtNo that are followed of read statements
Query FollowsStmtRead = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::FOLLOWS, "s", "r", ArgumentTypes::STMT, ArgumentTypes::READ));
Query InvalidFollowsStmtRead = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::FOLLOWS, "s1", "r", ArgumentTypes::STMT, ArgumentTypes::READ));

// Follows(s, i) - if true, return all stmtNo that are followed of if statements
Query FollowsStmtIf = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::FOLLOWS, "s", "i", ArgumentTypes::STMT, ArgumentTypes::IF));
Query InvalidFollowsStmtIf = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::FOLLOWS, "s1", "i", ArgumentTypes::STMT, ArgumentTypes::IF));

// Follows(s, w) - if true, return all stmtNo that are followed of while statements
Query FollowsStmtWhile = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::FOLLOWS, "s", "w", ArgumentTypes::STMT, ArgumentTypes::WHILE));
Query InvalidFollowsStmtWhile = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::FOLLOWS, "s1", "w", ArgumentTypes::STMT, ArgumentTypes::WHILE));

// Follows(4, _) - if true, return all stmtNo
Query FollowsIntAny = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::FOLLOWS, "4", "_", ArgumentTypes::INT, ArgumentTypes::ANY));
Query InvalidFollowsIntAny = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::FOLLOWS, "16", "_", ArgumentTypes::INT, ArgumentTypes::ANY));

// Follows(12, s) - if true, return stmtNo 13
Query FollowsIntStmt = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::FOLLOWS, "12", "s", ArgumentTypes::INT, ArgumentTypes::STMT));
Query InvalidFollowsIntStmt1 = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::FOLLOWS, "14", "s", ArgumentTypes::INT, ArgumentTypes::STMT));
Query InvalidFollowsIntStmt2 = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::FOLLOWS, "12", "s1", ArgumentTypes::INT, ArgumentTypes::STMT));
Query InvalidFollowsIntStmt3 = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::FOLLOWS, "50", "s1", ArgumentTypes::INT, ArgumentTypes::STMT));
Query InvalidFollowsIntStmt4 = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::FOLLOWS, "0", "s1", ArgumentTypes::INT, ArgumentTypes::STMT));

//Int, int(1)
Query FollowsIntInt = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::FOLLOWS, "5", "12", ArgumentTypes::INT, ArgumentTypes::INT));
Query InvalidFollowsIntInt1 = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::FOLLOWS, "12", "5", ArgumentTypes::INT, ArgumentTypes::INT)); // LHS > RHS
Query InvalidFollowsIntInt2 = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::FOLLOWS, "5", "5", ArgumentTypes::INT, ArgumentTypes::INT)); // LHS = RHS

// Follows(1, a) - if true, return all stmtNo
Query FollowsIntAssign = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::FOLLOWS, "1", "a", ArgumentTypes::INT, ArgumentTypes::ASSIGN));
Query InvalidFollowsIntAssign = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::FOLLOWS, "2", "a", ArgumentTypes::INT, ArgumentTypes::ASSIGN));

// Follows(13, p) - if true, return all stmtNo
Query FollowsIntPrint = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::FOLLOWS, "13", "p", ArgumentTypes::INT, ArgumentTypes::PRINT));
Query InvalidFollowsIntPrint = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::FOLLOWS, "15", "p", ArgumentTypes::INT, ArgumentTypes::PRINT));

// Follows(12, r) - if true, return all stmtNo
Query FollowsIntRead = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::FOLLOWS, "12", "r", ArgumentTypes::INT, ArgumentTypes::READ));
Query InvalidFollowsIntRead = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::FOLLOWS, "15", "r", ArgumentTypes::INT, ArgumentTypes::READ));

// Follows(2, i) - if true, return all stmtNo
Query FollowsIntIf = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::FOLLOWS, "6", "i", ArgumentTypes::INT, ArgumentTypes::IF));
Query InvalidFollowsIntIf = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::FOLLOWS, "9", "i", ArgumentTypes::INT, ArgumentTypes::IF));

// Follows(4, w) - if true, return all stmtNo
Query FollowsIntWhile = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::FOLLOWS, "4", "w", ArgumentTypes::INT, ArgumentTypes::WHILE));
Query InvalidFollowsIntWhile = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::FOLLOWS, "5", "w", ArgumentTypes::INT, ArgumentTypes::WHILE));

// Follows(a, _) - if true, return all stmtNo
Query FollowsAssignAny = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::FOLLOWS, "a", "_", ArgumentTypes::ASSIGN, ArgumentTypes::ANY));

// Follows(p, _) - if true, return all stmtNo
Query FollowsPrintAny = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::FOLLOWS, "p", "_", ArgumentTypes::PRINT, ArgumentTypes::ANY));

// Follows(r, _) - if true, return all stmtNo
Query FollowsReadAny = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::FOLLOWS, "r", "_", ArgumentTypes::READ, ArgumentTypes::ANY));

// Follows(i, _) - if true, return all stmtNo
Query FollowsIfAny = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::FOLLOWS, "i", "_", ArgumentTypes::IF, ArgumentTypes::ANY));

// Follows(w, _) - if true, return all stmtNo
Query FollowsWhileAny = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::FOLLOWS, "w", "_", ArgumentTypes::WHILE, ArgumentTypes::ANY));

// Follows(a, s) - if true, return all stmtNo that follows assign statement
Query FollowsAssignStmt = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::FOLLOWS, "a", "s", ArgumentTypes::ASSIGN, ArgumentTypes::STMT));
Query InvalidFollowsAssignStmt = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::FOLLOWS, "a", "s1", ArgumentTypes::ASSIGN, ArgumentTypes::STMT));

// Follows(p, s) - if true, return all stmtNo that follows print statement
Query FollowsPrintStmt = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::FOLLOWS, "p", "s", ArgumentTypes::PRINT, ArgumentTypes::STMT));
Query InvalidFollowsPrintStmt = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::FOLLOWS, "p", "s1", ArgumentTypes::PRINT, ArgumentTypes::STMT));

// Follows(r, s) - if true, return all stmtNo that follows read statement
Query FollowsReadStmt = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::FOLLOWS, "r", "s", ArgumentTypes::READ, ArgumentTypes::STMT));
Query InvalidFollowsReadStmt = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::FOLLOWS, "r", "s1", ArgumentTypes::READ, ArgumentTypes::STMT));

// Follows(i, s) - if true, return all stmtNo that follows if statement
Query FollowsIfStmt = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::FOLLOWS, "i", "s", ArgumentTypes::IF, ArgumentTypes::STMT));
Query InvalidFollowsIfStmt = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::FOLLOWS, "i", "s1", ArgumentTypes::IF, ArgumentTypes::STMT));

// Follows(w, s) - if true, return all stmtNo that follows while statement
Query FollowsWhileStmt = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::FOLLOWS, "w", "s", ArgumentTypes::WHILE, ArgumentTypes::STMT));
Query InvalidFollowsWhileStmt = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::FOLLOWS, "w", "s1", ArgumentTypes::WHILE, ArgumentTypes::STMT));

// Follows(a, 3) - if true, return all stmtNo 
Query FollowsAssignInt = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::FOLLOWS, "a", "3", ArgumentTypes::ASSIGN, ArgumentTypes::INT));
Query InvalidFollowsAssignInt = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::FOLLOWS, "a", "16", ArgumentTypes::ASSIGN, ArgumentTypes::INT));

// Follows(p, 9) - if true, return all stmtNo 
Query FollowsPrintInt = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::FOLLOWS, "p", "9", ArgumentTypes::PRINT, ArgumentTypes::INT));
Query InvalidFollowsPrintInt = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::FOLLOWS, "p", "10", ArgumentTypes::PRINT, ArgumentTypes::INT));

// Follows(r, 7) - if true, return all stmtNo 
Query FollowsReadInt = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::FOLLOWS, "r", "7", ArgumentTypes::READ, ArgumentTypes::INT));
Query InvalidFollowsReadInt = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::FOLLOWS, "r", "10", ArgumentTypes::READ, ArgumentTypes::INT));

// Follows(i, 11) - if true, return all stmtNo 
Query FollowsIfInt = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::FOLLOWS, "i", "11", ArgumentTypes::IF, ArgumentTypes::INT));
Query InvalidFollowsIfInt = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::FOLLOWS, "i", "10", ArgumentTypes::IF, ArgumentTypes::INT));

// Follows(w, 12) - if true, return all stmtNo 
Query FollowsWhileInt = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::FOLLOWS, "w", "12", ArgumentTypes::WHILE, ArgumentTypes::INT));
Query InvalidFollowsWhileInt = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::FOLLOWS, "w", "10", ArgumentTypes::WHILE, ArgumentTypes::INT));

// Follows(a1, a2) - if true, return all stmtNo 
Query FollowsAssignAssign = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::FOLLOWS, "a1", "a2", ArgumentTypes::ASSIGN, ArgumentTypes::ASSIGN));
Query InvalidFollowsAssignAssign = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::FOLLOWS, "a", "a", ArgumentTypes::ASSIGN, ArgumentTypes::ASSIGN)); // repeated synonym

// Follows(a, p) - if true, return all stmtNo 
Query FollowsAssignPrint = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::FOLLOWS, "a", "p", ArgumentTypes::ASSIGN, ArgumentTypes::PRINT));

// Follows(a, r) - if true, return all stmtNo 
Query FollowsAssignRead = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::FOLLOWS, "a", "r", ArgumentTypes::ASSIGN, ArgumentTypes::READ));

// Follows(a, i) - if true, return all stmtNo 
Query FollowsAssignIf = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::FOLLOWS, "a", "i", ArgumentTypes::ASSIGN, ArgumentTypes::IF));

// Follows(a, w) - if true, return all stmtNo 
Query FollowsAssignWhile = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::FOLLOWS, "a", "w", ArgumentTypes::ASSIGN, ArgumentTypes::WHILE));

// Follows(p1, p2) - if true, return all stmtNo 
Query FollowsPrintPrint = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::FOLLOWS, "p1", "p2", ArgumentTypes::PRINT, ArgumentTypes::PRINT));
Query InvalidFollowsPrintPrint = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::FOLLOWS, "p", "p", ArgumentTypes::PRINT, ArgumentTypes::PRINT)); // repeated synonym

// Follows(p, a) - if true, return all stmtNo
Query FollowsPrintAssign = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::FOLLOWS, "p", "a", ArgumentTypes::PRINT, ArgumentTypes::ASSIGN));

// Follows(p, r) - if true, return all stmtNo
Query FollowsPrintRead = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::FOLLOWS, "p", "r", ArgumentTypes::PRINT, ArgumentTypes::READ));

// Follows(p, i) - if true, return all stmtNo
Query FollowsPrintIf = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::FOLLOWS, "p", "i", ArgumentTypes::PRINT, ArgumentTypes::IF));

// Follows(p, w) - if true, return all stmtNo
Query FollowsPrintWhile = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::FOLLOWS, "p", "w", ArgumentTypes::PRINT, ArgumentTypes::WHILE));

// Follows(r1, r2) - if true, return all stmtNo 
Query FollowsReadRead = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::FOLLOWS, "r1", "r2", ArgumentTypes::READ, ArgumentTypes::READ));
Query InvalidFollowsReadRead = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::FOLLOWS, "r", "r", ArgumentTypes::READ, ArgumentTypes::READ)); // repeated synonym

// Follows(r, a) - if true, return all stmtNo
Query FollowsReadAssign = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::FOLLOWS, "r", "a", ArgumentTypes::READ, ArgumentTypes::ASSIGN));

// Follows(r, p) - if true, return all stmtNo
Query FollowsReadPrint = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::FOLLOWS, "r", "p", ArgumentTypes::READ, ArgumentTypes::PRINT));

// Follows(r, i) - if true, return all stmtNo
Query FollowsReadIf = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::FOLLOWS, "r", "i", ArgumentTypes::READ, ArgumentTypes::IF));

// Follows(r, w) - if true, return all stmtNo
Query FollowsReadWhile = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::FOLLOWS, "r", "w", ArgumentTypes::READ, ArgumentTypes::WHILE));

// Follows(i1, i2) - if true, return all stmtNo 
Query FollowsIfIf = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::FOLLOWS, "i1", "i2", ArgumentTypes::IF, ArgumentTypes::IF));
Query InvalidFollowsIfIf = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::FOLLOWS, "i", "i", ArgumentTypes::IF, ArgumentTypes::IF)); // repeated synonym

// Follows(i, a) - if true, return all stmtNo
Query FollowsIfAssign = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::FOLLOWS, "i", "a", ArgumentTypes::IF, ArgumentTypes::ASSIGN));

// Follows(i, p) - if true, return all stmtNo
Query FollowsIfPrint = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::FOLLOWS, "i", "p", ArgumentTypes::IF, ArgumentTypes::PRINT));

// Follows(i, r) - if true, return all stmtNo
Query FollowsIfRead = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::FOLLOWS, "i", "r", ArgumentTypes::IF, ArgumentTypes::READ));

// Follows(i, a) - if true, return all stmtNo
Query FollowsIfWhile = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::FOLLOWS, "i", "w", ArgumentTypes::IF, ArgumentTypes::WHILE));

// Follows(w1, w2) - if true, return all stmtNo 
Query FollowsWhileWhile = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::FOLLOWS, "w1", "w2", ArgumentTypes::WHILE, ArgumentTypes::WHILE));
Query InvalidFollowsWhileWhile = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::FOLLOWS, "w", "w", ArgumentTypes::WHILE, ArgumentTypes::WHILE));  // repeated synonym

// Follows(w, a) - if true, return all stmtNo
Query FollowsWhileAssign = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::FOLLOWS, "w", "a", ArgumentTypes::WHILE, ArgumentTypes::ASSIGN));

// Follows(w, p) - if true, return all stmtNo
Query FollowsWhilePrint = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::FOLLOWS, "w", "p", ArgumentTypes::WHILE, ArgumentTypes::PRINT));

// Follows(w, r) - if true, return all stmtNo
Query FollowsWhileRead = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::FOLLOWS, "w", "r", ArgumentTypes::WHILE, ArgumentTypes::READ));

// Follows(w, i) - if true, return all stmtNo
Query FollowsWhileIf = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::FOLLOWS, "w", "i", ArgumentTypes::WHILE, ArgumentTypes::IF));

/*
Prog_line and call stmt
*/
// Select n such that Follows(n, s) - true, return all stmts that are followed
Query FollowsProgLineStmtInClause = Query("n", ArgumentTypes::PROG_LINE, QuerySuchThatClause(RelRef::FOLLOWS, "n", "s", ArgumentTypes::PROG_LINE, ArgumentTypes::STMT));

// Select n such that Follows(s1, s2) - true, return all prog_line
Query FollowsProgLineStmtNotInClause = Query("n", ArgumentTypes::PROG_LINE, QuerySuchThatClause(RelRef::FOLLOWS, "s1", "s2", ArgumentTypes::STMT, ArgumentTypes::STMT));

// Select s such that Follows(s, c) - true, return all followed of call stmts
Query FollowsStmtCallInClause = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::FOLLOWS, "s", "c", ArgumentTypes::STMT, ArgumentTypes::CALL));

// Select s such that Follows(n, c) - false, return nothing
Query FollowsStmtCallNotInClause = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::FOLLOWS, "n", "c", ArgumentTypes::PROG_LINE, ArgumentTypes::CALL));

// Select s such that Follows(s, n) - true, return all followed stmts
Query FollowsStmtProgLineInClause = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::FOLLOWS, "s", "n", ArgumentTypes::STMT, ArgumentTypes::PROG_LINE));

// Select s such that Follows(n, c) - false, return nothing
Query FollowsStmtProgLineNotInClause = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::FOLLOWS, "n", "c", ArgumentTypes::PROG_LINE, ArgumentTypes::CALL));
