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
6				while(some cond) {}
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
15			print p;
16			a = 10;
		}
}

procedure testB {
17	  a = 10;
}
*/

std::unordered_set<std::string> allProgStmts = { "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15", "16", "17" };
//set of all stmts that follow some stmt
std::unordered_set<std::string> allFollowerStmt = { "2", "3", "5", "7", "9", "11", "12", "13", "14", "16" };
//set of all stmts followed by some stmt
std::unordered_set<std::string> allFollowedStmt = { "1", "2", "4", "5", "6", "7", "8", "12", "13", "15" };

//Any, any
// FollowsT(_, _) - if true, returns all stmtNo
Query FollowsTAnyAny = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::FOLLOWS_T, "_", "_", ArgumentTypes::ANY, ArgumentTypes::ANY));

//any, stmt
// select s such that followsT(_, s) - if true, returns all followers stmtNo
Query FollowsTAnyStmtInClause = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::FOLLOWS_T, "_", "s", ArgumentTypes::ANY, ArgumentTypes::STMT));

//select s such that followsT(_, s1) - if true, returns all stmtNo
Query FollowsTAnyStmtNotInClause = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::FOLLOWS_T, "_", "s1", ArgumentTypes::ANY, ArgumentTypes::STMT));

//stmt, any
//select s such that followsT(s, _) - if true, return all followed stmtNo
Query FollowsTStmtAnyInClause = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::FOLLOWS_T, "s", "_", ArgumentTypes::STMT, ArgumentTypes::ANY));

//select s such that followT(s1, _) - if true, return all stmtNo
Query FollowsTStmtAnyNotInClause = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::FOLLOWS_T, "s1", "_", ArgumentTypes::STMT, ArgumentTypes::ANY));

//stmt, stmt
//select s such that followsT(s, s1) -> if true, returns all s that has some follower
Query FollowsTStmtStmtFollowed = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::FOLLOWS_T, "s", "s1", ArgumentTypes::STMT, ArgumentTypes::STMT)); 

//select s such that followsT(s1, s) -> if true, return all stmt that is a follower
Query FollowsTStmtStmtFollower = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::FOLLOWS_T, "s1", "s", ArgumentTypes::STMT, ArgumentTypes::STMT));

//select s such that followsT(s1, s2) -> if true, return all stmtNo
Query FollowsTStmtStmtNotInClause = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::FOLLOWS_T, "s1", "s2", ArgumentTypes::STMT, ArgumentTypes::STMT)); 

//any, int
//select s such that followsT(_, 16)  -> if true, return all stmtNo
Query FollowsTAnyInt_True = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::FOLLOWS_T, "_", "16", ArgumentTypes::ANY, ArgumentTypes::INT));

//select s such that FollowsT(_, 1)
Query FollowsTAnyInt_False = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::FOLLOWS_T, "_", "1", ArgumentTypes::ANY, ArgumentTypes::INT));

//stmt, int
//select s such that followT(s, 3) -> returns all stmt that 3 follows transitively
Query FollowsTStmtIntInClause = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::FOLLOWS_T, "s", "3", ArgumentTypes::STMT, ArgumentTypes::INT));

//select s such that followsT(s1, 16) -> return all stmts if 16 is following any stmt
Query FollowsTStmtIntNotInClause = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::FOLLOWS_T, "s1", "16", ArgumentTypes::STMT, ArgumentTypes::INT));

//select s such that followT(s, 1) -> always false, empty set
Query FollowsTStmtInt_False = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::FOLLOWS_T, "s", "1", ArgumentTypes::STMT, ArgumentTypes::INT));

//any, assign
//select a such that FollowsT(_, a) -> if true, return all assign statements followers
Query FollowsTAnyAssignInClause = Query("a", ArgumentTypes::ASSIGN, QuerySuchThatClause(RelRef::FOLLOWS_T, "_", "a", ArgumentTypes::ANY, ArgumentTypes::ASSIGN));

//select s such that FollowsT(_, a) -> if true, return all statements
Query FollowsTAnyAssignNotInClause = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::FOLLOWS_T, "_", "a", ArgumentTypes::ANY, ArgumentTypes::ASSIGN)); 

//any, print
//select s such that FollowsT(_, p) -> if true, return all statements
Query FollowsTAnyPrintNotInClause = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::FOLLOWS_T, "_", "p", ArgumentTypes::ANY, ArgumentTypes::PRINT));

//any, read
//select s such that FollowsT(_, r) -> if true, return all statements
Query FollowsTAnyReadNotInClause = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::FOLLOWS_T, "_", "r", ArgumentTypes::ANY, ArgumentTypes::READ));

//any, if -> returns all if stmt that are followers
//select ifs such that FollowsT(_, ifs)
Query FollowsTAnyIfInClause = Query("ifs", ArgumentTypes::IF, QuerySuchThatClause(RelRef::FOLLOWS_T, "_", "ifs", ArgumentTypes::ANY, ArgumentTypes::IF)); 

//select s such that FollowsT(_, ifs) -> if true, return all statements
Query FollowsTAnyIfNotInClause = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::FOLLOWS_T, "_", "ifs", ArgumentTypes::ANY, ArgumentTypes::IF));

//any, while
//select w such that FollowsT(_, w)
Query FollowsTAnyWhileInClause = Query("w", ArgumentTypes::WHILE, QuerySuchThatClause(RelRef::FOLLOWS_T, "_", "w", ArgumentTypes::ANY, ArgumentTypes::WHILE));

//select s such that Follows(_, w)
Query FollowsTAnyWhileNotInClause = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::FOLLOWS_T, "_", "w", ArgumentTypes::ANY, ArgumentTypes::WHILE));

//stmt, if
//select s such that FollowsT(s, ifs)
Query FollowsTStmtIfInClause = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::FOLLOWS_T, "s", "ifs", ArgumentTypes::STMT, ArgumentTypes::IF));

//select s such that FollowsT(s1, ifs)
Query FollowsTStmtIfNotInClause = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::FOLLOWS_T, "s1", "ifs", ArgumentTypes::STMT, ArgumentTypes::IF));

//stmt, while
//select s such that FollowsT(s, w)
Query FollowsTStmtWhileInClause = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::FOLLOWS_T, "s", "w", ArgumentTypes::STMT, ArgumentTypes::WHILE));

//select s such that FollowsT(s1, w)
Query FollowsTStmtWhileNotInClause = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::FOLLOWS_T, "s1", "w", ArgumentTypes::STMT, ArgumentTypes::WHILE)); 

//int, any
//select s such that followsT(1, _) -> if true, return all stmtNo
Query FollowsTIntAny_True = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::FOLLOWS_T, "1", "_", ArgumentTypes::INT, ArgumentTypes::ANY)); 

//select s such that followsT(16, _)
Query FollowsTIntAny_False = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::FOLLOWS_T, "16", "_", ArgumentTypes::INT, ArgumentTypes::ANY));

//int, stmt
//select s such that FollowsT(4, s) -> return all stmts following 4
Query FollowsTIntStmtInClause = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::FOLLOWS_T, "4", "s", ArgumentTypes::INT, ArgumentTypes::STMT)); 

//select s such that FollowsT(1, s1) -> return all stmts
Query FollowsTIntStmtNotInClause = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::FOLLOWS_T, "1", "s1", ArgumentTypes::INT, ArgumentTypes::STMT));

//select s such that FollowsT(16, s) -> always false, empty set
Query FollowsTIntStmt_False = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::FOLLOWS_T, "16", "s", ArgumentTypes::INT, ArgumentTypes::STMT)); 

//int, int
//select s such that followsT(1, 2) -> returns all stmts if evaluate true
Query FollowsTIntInt_True = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::FOLLOWS_T, "1", "2", ArgumentTypes::INT, ArgumentTypes::INT)); 

//select s such that followsT(16, 2)
Query FollowsTIntInt_False = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::FOLLOWS_T, "16", "2", ArgumentTypes::INT, ArgumentTypes::INT));

//int, if
//select ifs such that FollowsT(1, ifs)
Query FollowsTIntIfInClause = Query("ifs", ArgumentTypes::IF, QuerySuchThatClause(RelRef::FOLLOWS_T, "1", "ifs", ArgumentTypes::INT, ArgumentTypes::IF));

//select s such that FollowsT(1, ifs)
Query FollowsTIntIfNotInClause = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::FOLLOWS_T, "1", "ifs", ArgumentTypes::INT, ArgumentTypes::IF));

//select ifs such that FollowsT(12, ifs)
Query FollowsTIntIf_False = Query("ifs", ArgumentTypes::IF, QuerySuchThatClause(RelRef::FOLLOWS_T, "12", "ifs", ArgumentTypes::INT, ArgumentTypes::IF));

//int, while
//select w such that FollowsT(4, w)
Query FollowsTIntWhileInClause = Query("w", ArgumentTypes::WHILE, QuerySuchThatClause(RelRef::FOLLOWS_T, "4", "w", ArgumentTypes::INT, ArgumentTypes::WHILE));

//select s such that Follows(4, w)
Query FollowsTIntWhileNotInClause = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::FOLLOWS_T, "4", "w", ArgumentTypes::INT, ArgumentTypes::WHILE));

//select w such that FollowsT(12, w)
Query FollowsTIntWhile_False = Query("w", ArgumentTypes::WHILE, QuerySuchThatClause(RelRef::FOLLOWS_T, "12", "w", ArgumentTypes::INT, ArgumentTypes::WHILE));

//if, any
//select ifs such that FollowsT(ifs, _)
Query FollowsTIfAnyInClause = Query("ifs", ArgumentTypes::IF, QuerySuchThatClause(RelRef::FOLLOWS_T, "ifs", "_", ArgumentTypes::IF, ArgumentTypes::ANY));

//select s such that FollowsT(ifs, _)
Query FollowsTIfAnyNotInClause = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::FOLLOWS_T, "ifs", "_", ArgumentTypes::IF, ArgumentTypes::ANY));

//while, any
//select w such that Follows(w, _)
Query FollowsTWhileAnyInClause = Query("w", ArgumentTypes::WHILE, QuerySuchThatClause(RelRef::FOLLOWS_T, "w", "_", ArgumentTypes::WHILE, ArgumentTypes::ANY));

//select s such that FollowsT(w, _)
Query FollowsTWhileAnyNotInClause = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::FOLLOWS_T, "w", "_", ArgumentTypes::WHILE, ArgumentTypes::ANY));

//if, stmt
//select ifs such that followsT(ifs, s) -> return all if stmts followed by some stmt
Query FollowsTIfStmtInClause = Query("ifs", ArgumentTypes::IF, QuerySuchThatClause(RelRef::FOLLOWS_T, "ifs", "s", ArgumentTypes::IF, ArgumentTypes::STMT));

//select s such that followsT(ifs, s1) -> return all stmts if some if is followed by some stmt
Query FollowsTIfStmtNotInClause = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::FOLLOWS_T, "ifs", "s1", ArgumentTypes::IF, ArgumentTypes::STMT));

//while, stmt
//select w such that followsT(w, s) -> return all while followed by some stmt
Query FollowsTWhileStmtInClause = Query("w", ArgumentTypes::WHILE, QuerySuchThatClause(RelRef::FOLLOWS_T, "w", "s", ArgumentTypes::WHILE, ArgumentTypes::STMT));

//select s such that followsT(w, s1) -> return allprogstmts if there is some while followed by some stmt
Query FollowsTWhileStmtNotInClause = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::FOLLOWS_T, "w", "s1", ArgumentTypes::WHILE, ArgumentTypes::STMT));

//if, int
//select ifs such that FollowsT(ifs, 11) -> return all if statement that is followed by 11
Query FollowsTIfIntInClause = Query("ifs", ArgumentTypes::IF, QuerySuchThatClause(RelRef::FOLLOWS_T, "ifs", "11", ArgumentTypes::IF, ArgumentTypes::INT));

//select s such that FollowsT(ifs, 11) -> return all stmts if there is some if that is followed by 11
Query FollowsTIfIntNotInClause = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::FOLLOWS_T, "ifs", "11", ArgumentTypes::IF, ArgumentTypes::INT));

//select s such that FollowsT(ifs, 16) -> eval to false, returns empty set
Query FollowsTIfIntNotInClause_False = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::FOLLOWS_T, "ifs", "16", ArgumentTypes::IF, ArgumentTypes::INT));

//while, int
//select w such that Follows(w, 12) -> return all while follwed by 12
Query FollowsTWhileIntInClause = Query("w", ArgumentTypes::WHILE, QuerySuchThatClause(RelRef::FOLLOWS_T, "w", "12", ArgumentTypes::WHILE, ArgumentTypes::INT));

//select s such that FollowsT(w, 12) -> returns allprogstmts if true
Query FollowsTWhileIntNotInClause = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::FOLLOWS_T, "w", "12", ArgumentTypes::WHILE, ArgumentTypes::INT));

//select s such that FollowsT(w, 16) -> evaluates to false, returns empty set
Query FollowsTWhileIntNotInClause_False = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::FOLLOWS_T, "w", "16", ArgumentTypes::WHILE, ArgumentTypes::INT));

//syn, syn
//select ifs such that Follows(w, ifs) -> return if stmts following while (stmt 7)
Query FollowsTWhileIfInClause = Query("ifs", ArgumentTypes::IF, QuerySuchThatClause(RelRef::FOLLOWS_T, "w", "ifs", ArgumentTypes::WHILE, ArgumentTypes::IF));

//select s such that Follows(w, ifs) -> return all stmts if there is some if stmt following while
Query FollowsTWhileIfNotInClause = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::FOLLOWS_T, "w", "ifs", ArgumentTypes::WHILE, ArgumentTypes::IF));

/*
Prog_line and call stmt
*/
// Select n such that FollowsT(n, s) - true, return all stmts that are followed
Query FollowsTProgLineStmtInClause = Query("n", ArgumentTypes::PROG_LINE, QuerySuchThatClause(RelRef::FOLLOWS_T, "n", "s", ArgumentTypes::PROG_LINE, ArgumentTypes::STMT));

// Select n such that FollowsT(s1, s2) - true, return all prog_line
Query FollowsTProgLineStmtNotInClause = Query("n", ArgumentTypes::PROG_LINE, QuerySuchThatClause(RelRef::FOLLOWS_T, "s1", "s2", ArgumentTypes::STMT, ArgumentTypes::STMT));

// Select s such that FollowsT(s, c) - true, return all followedT of call stmts
Query FollowsTStmtCallInClause = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::FOLLOWS_T, "s", "c", ArgumentTypes::STMT, ArgumentTypes::CALL));

// Select s such that FollowsT(n, c) - true, return all stmts
Query FollowsTStmtCallNotInClause = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::FOLLOWS_T, "n", "c", ArgumentTypes::PROG_LINE, ArgumentTypes::CALL));

// Select s such that FollowsT(s, n) - true, return all followed stmts
Query FollowsTStmtProgLineInClause = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::FOLLOWS_T, "s", "n", ArgumentTypes::STMT, ArgumentTypes::PROG_LINE));

// Select s such that FollowsT(n, c) - true, return all stmts
Query FollowsTStmtProgLineNotInClause = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::FOLLOWS_T, "n", "c", ArgumentTypes::PROG_LINE, ArgumentTypes::CALL));
