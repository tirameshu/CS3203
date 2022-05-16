#pragma once

#include "QueryEvaluator.h"

/*
procedure main {
1	while (a != b ) {
2		while (sum != subtract) {
3			read sum;
		}
	}
4	subtract = a - b;
5	if (subtract > b) then {
6		if (sum < subtract) then {
7			subtract = subtract + sum;
8			print subtract;
		} else {
9			read a;
10			print b;
		}
	} else {
11		subtract = subtract - sum;
	}
12	while (subtract < sum) {
13    mod13 = use13 + 2;
14    if (use14 == 4) {
15       print use15;
		} else {
16       read mod16;
		}
	}
17	if (subtract == sum) {
18		if (a == b) {
19			print a;
		}
	}
}
*/

std::unordered_set<std::string> allStmtsParentStar = { "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15", "16", "17", "18", "19" };
std::unordered_set<std::string> allDescendants = { "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "13", "14", "15", "16", "18", "19" };
std::unordered_set<std::string> allAncestors = { "1", "2", "4", "5", "12", "14", "17", "18" };

std::unordered_set<std::string> ancestorsOfAssign = { "1", "4", "5", "12"};
std::unordered_set<std::string> ancestorsOfPrint = { "1", "4", "5", "12", "14", "17", "18" };
std::unordered_set<std::string> ancestorsOfRead = { "1", "2", "4", "5", "12", "14" };
std::unordered_set<std::string> ancestorsOfIf = { "1", "12", "17" };
std::unordered_set<std::string> ancestorsOfWhile = { "1", "4" };

std::unordered_set<std::string> descendantsOfIf = { "5", "6", "7", "8", "9", "10", "11", "15", "16", "18", "19" };
std::unordered_set<std::string> descendantsOfWhile = { "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "13", "14", "15", "16" };

// Select s such that ParentStar(_, _) - if a container stmt exists, returns all stmts
Query ParentStarAnyAny = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::PARENT_T, "_", "_", ArgumentTypes::ANY, ArgumentTypes::ANY));

// Select s such that ParentStar(_, s) - if any, return all descendents stmts
Query ParentStarAnyStmt = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::PARENT_T, "_", "s", ArgumentTypes::ANY, ArgumentTypes::STMT));
// Select s such that ParentStar(_, s1) - if a container stmt exists, returns all stmts
Query argNotInSelectParentStarAnyStmt = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::PARENT_T, "_", "s1", ArgumentTypes::ANY, ArgumentTypes::STMT));

// Select s such that ParentStar(s, _) - if any, return all container (parent) stmts
Query ParentStarStmtAny = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::PARENT_T, "s", "_", ArgumentTypes::STMT, ArgumentTypes::ANY));
// Select s such that ParentStar(s1, _) - if a container stmt exists, returns all stmts
Query argNotInSelectParentStarStmtAny = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::PARENT_T, "s1", "_", ArgumentTypes::STMT, ArgumentTypes::ANY));

// Select s such that ParentStar(s, s2) - if any, return all container (parent) stmts
Query ParentStarStmtStmt = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::PARENT_T, "s", "s2", ArgumentTypes::STMT, ArgumentTypes::STMT));
// Select s such that ParentStar(s, s) - return empty set
Query edgeCaseParentStarSameStmt = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::PARENT_T, "s", "s", ArgumentTypes::STMT, ArgumentTypes::STMT));
// Select s such that ParentStar(s1, s2) - same as ParentStar(_, _), return all stmts
Query argNotInSelectParentStarStmtStmt = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::PARENT_T, "s1", "s2", ArgumentTypes::STMT, ArgumentTypes::STMT));

/* ===== Select s such that ParentStar(*, INT) ===== */

// Select s such that ParentStar(_, 9) - if stmt 9 has parent, return all stmts
Query ParentStarAnyInt = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::PARENT_T, "_", "9", ArgumentTypes::ANY, ArgumentTypes::INT));
// stmt 12 has no parent - return empty set
Query argNotInSelectParentStarAnyInt = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::PARENT_T, "_", "12", ArgumentTypes::ANY, ArgumentTypes::INT));

// Select s such that ParentStar(s, 16) - return 12, 14
Query ParentStarStmtInt = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::PARENT_T, "s", "16", ArgumentTypes::STMT, ArgumentTypes::INT));
// Select s such that ParentStar(s1, 11) - 11 has parent, return all stmts
Query argNotInSelectParentStarStmtInt = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::PARENT_T, "s1", "11", ArgumentTypes::STMT, ArgumentTypes::INT));

/* ===== Select s such that ParentStar(s, *) ===== */

// Select s such that ParentStar(_, a) - true, return all stmtNo 
Query ParentStarAnyAssign = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::PARENT_T, "_", "a", ArgumentTypes::ANY, ArgumentTypes::ASSIGN));

// Select s such that ParentStar(_, p) - true, return all stmtNo
Query ParentStarAnyPrint = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::PARENT_T, "_", "p", ArgumentTypes::ANY, ArgumentTypes::PRINT));

// Select s such that ParentStar(_, r) - true, return all stmtNo
Query ParentStarAnyRead = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::PARENT_T, "_", "r", ArgumentTypes::ANY, ArgumentTypes::READ));

// Select s such that ParentStar(_, i) - true, return all stmtNo 
Query ParentStarAnyIf = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::PARENT_T, "_", "i", ArgumentTypes::ANY, ArgumentTypes::IF));

// Select s such that ParentStar(_, w) - true, return all stmts
Query ParentStarAnyWhile = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::PARENT_T, "_", "w", ArgumentTypes::ANY, ArgumentTypes::WHILE));

/* ===== Select s such that ParentStar(s, *) ===== */

// Select s such that ParentStar(s, a) - if any, return all ancestors of assign stmts
Query ParentStarStmtAssign = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::PARENT_T, "s", "a", ArgumentTypes::STMT, ArgumentTypes::ASSIGN));
// Select s such that ParentStar(s1, a) - if an assign stmt has parent, return all stmtNo 
Query argNotInSelectParentStarStmtAssign = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::PARENT_T, "s1", "a", ArgumentTypes::STMT, ArgumentTypes::ASSIGN));

// Select s such that ParentStar(s, p) - if any, return all ancestors of print stmts
Query ParentStarStmtPrint = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::PARENT_T, "s", "p", ArgumentTypes::STMT, ArgumentTypes::PRINT));
// Select s such that ParentStar(s1, p) - if a print stmt has parent, return all stmts
Query argNotInSelectParentStarStmtPrint = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::PARENT_T, "s1", "p", ArgumentTypes::STMT, ArgumentTypes::PRINT));

// Select s such that ParentStar(s, r) - if any, return all ancestors of read stmts
Query ParentStarStmtRead = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::PARENT_T, "s", "r", ArgumentTypes::STMT, ArgumentTypes::READ));
// Select s such that ParentStar(s1, r) - if a read stmt has parent, return all stmts
Query argNotInSelectParentStarStmtRead = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::PARENT_T, "s1", "r", ArgumentTypes::STMT, ArgumentTypes::READ));

// Select s such that ParentStar(s, i) - if any, return all ancestors of if stmts
Query ParentStarStmtIf = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::PARENT_T, "s", "i", ArgumentTypes::STMT, ArgumentTypes::IF));
// Select s such that ParentStar(s1, i) - if an if stmt has parent, return all stmts
Query argNotInSelectParentStarStmtIf = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::PARENT_T, "s1", "i", ArgumentTypes::STMT, ArgumentTypes::IF));

// Select s such that ParentStar(s, w) - if any, return all ancestors of while stmts
Query ParentStarStmtWhile = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::PARENT_T, "s", "w", ArgumentTypes::STMT, ArgumentTypes::WHILE));
// Select s such that ParentStar(s1, w) - if a while stmt has parent, return all stmts
Query argNotInSelectParentStarStmtWhile = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::PARENT_T, "s1", "w", ArgumentTypes::STMT, ArgumentTypes::WHILE));

/* ===== Select s such that ParentStar(INT, *) ===== */

// Select s such that ParentStar(5, _) - 5 is ancestor, return all stmts
Query ParentStarIntAnyHasResult = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::PARENT_T, "5", "_", ArgumentTypes::INT, ArgumentTypes::ANY));
// Select s such that ParentStar(6, _) - 6 not parent, return empty set
Query ParentStarIntAnyNoResult = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::PARENT_T, "6", "_", ArgumentTypes::INT, ArgumentTypes::ANY));

// Select s such that ParentStar(1, s) - 1 is ancestor, return stmts 2, 3, 4, 5, 6, 7, 8, 9, 10, 11
Query ParentStarIntStmtHasResult = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::PARENT_T, "1", "s", ArgumentTypes::INT, ArgumentTypes::STMT));
// Select s such that ParentStar(13, s) - 13 not parent, return empty set
Query ParentStarIntStmtNoResult = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::PARENT_T, "13", "s", ArgumentTypes::INT, ArgumentTypes::STMT));
// Select s such that ParentStar(12, s1) - 12 is parent, return all stmts
Query argNotInSelectParentStarIntStmt = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::PARENT_T, "12", "s1", ArgumentTypes::INT, ArgumentTypes::STMT));

/* ===== Select s such that ParentStar(INT, INT) ===== */

// Select s such that ParentStar(12, 13) - true, return all stmts
Query ParentStarIntIntTrue = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::PARENT_T, "12", "13", ArgumentTypes::INT, ArgumentTypes::INT));
// Select s such that ParentStar(5, 12) - false, return empty set
Query ParentStarIntIntFalse = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::PARENT_T, "5", "12", ArgumentTypes::INT, ArgumentTypes::INT));
// Select s such that ParentStar(12, 15) - false (not direct parent), return empty set
Query ParentStarIntIntNotDirectParentStar = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::PARENT_T, "5", "12", ArgumentTypes::INT, ArgumentTypes::INT));
// Select s such that ParentStar(12, 5) - false, return empty set
Query edgeCaseParentStarIntIntBiggerSmaller = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::PARENT_T, "12", "5", ArgumentTypes::INT, ArgumentTypes::INT)); // LHS > RHS
// Select s such that ParentStar(5, 5) - false, return empty set
Query edgeCaseParentStarIntIntSame = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::PARENT_T, "5", "5", ArgumentTypes::INT, ArgumentTypes::INT)); // LHS = RHS

/* ===== Select s such that ParentStar(INT, *) ===== */

// Select s such that ParentStar(5, a) - 5 is parent of assign stmt, return all stmts
Query ParentStarIntAssignHasResult = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::PARENT_T, "5", "a", ArgumentTypes::INT, ArgumentTypes::ASSIGN));
// Select s such that ParentStar(14, a) - 14 not a parent of assign stmt, return empty set
Query ParentStarIntAssignNoResult = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::PARENT_T, "14", "a", ArgumentTypes::INT, ArgumentTypes::ASSIGN));

// Select s such that ParentStar(17, p) - 17 is ancestor of print stmt, return all stmts
Query ParentStarIntPrintHasResult = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::PARENT_T, "17", "p", ArgumentTypes::INT, ArgumentTypes::PRINT));
// Select s such that ParentStar(19, p) - 19 not ancestor, return empty set
Query ParentStarIntPrintNoResult = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::PARENT_T, "19", "p", ArgumentTypes::INT, ArgumentTypes::PRINT));

// Select s such that ParentStar(14, r) - 14 is parent of read stmt, return all stmts
Query ParentStarIntReadHasResult = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::PARENT_T, "14", "r", ArgumentTypes::INT, ArgumentTypes::READ));
// Select s such that ParentStar(17, r) - 8 not ancestor of read stmt, return empty set
Query ParentStarIntReadNoResult = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::PARENT_T, "17", "r", ArgumentTypes::INT, ArgumentTypes::READ));

// Select s such that ParentStar(12, i) - 12 is parent of if stmt, return all stmts
Query ParentStarIntIfHasResult = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::PARENT_T, "12", "i", ArgumentTypes::INT, ArgumentTypes::IF));
// Select s such that ParentStar(5, i) - 5 not parent of if stmt, return empty set
Query ParentStarIntIfNoResult = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::PARENT_T, "5", "i", ArgumentTypes::INT, ArgumentTypes::IF));

// Select s such that ParentStar(1, w) - 1 is parent of while stmt, return all stmts
Query ParentStarIntWhileHasResult = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::PARENT_T, "1", "w", ArgumentTypes::INT, ArgumentTypes::WHILE));
// Select s such that ParentStar(2, w) - 2 not parent of while stmt, return empty set
Query ParentStarIntWhileNoResult = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::PARENT_T, "2", "w", ArgumentTypes::INT, ArgumentTypes::WHILE));

/* ===== Select s such that ParentStar(*, _) ===== */

// Select s such that ParentStar(a, _) - always false, return empty set
Query ParentStarAssignAny = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::PARENT_T, "a", "_", ArgumentTypes::ASSIGN, ArgumentTypes::ANY));

// Select s such that ParentStar(p, _) - always false, return empty set
Query ParentStarPrintAny = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::PARENT_T, "p", "_", ArgumentTypes::PRINT, ArgumentTypes::ANY));

// Select s such that ParentStar(r, _) - always false, return empty set
Query ParentStarReadAny = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::PARENT_T, "r", "_", ArgumentTypes::READ, ArgumentTypes::ANY));

// Select s such that ParentStar(i, _) - always true, return all stmts
Query ParentStarIfAny = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::PARENT_T, "i", "_", ArgumentTypes::IF, ArgumentTypes::ANY));

// Select s such that ParentStar(w, _) - always true, return all stmts
Query ParentStarWhileAny = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::PARENT_T, "w", "_", ArgumentTypes::WHILE, ArgumentTypes::ANY));

/* ===== Select s such that ParentStar(*, s) ===== */

// Select s such that ParentStar(a, s) - always false, return empty set
Query ParentStarAssignStmt = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::PARENT_T, "a", "s", ArgumentTypes::ASSIGN, ArgumentTypes::STMT));
// Select s such that ParentStar(a, s1) - always false, return empty set
Query argNotInSelectParentStarAssignStmt = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::PARENT_T, "a", "s1", ArgumentTypes::ASSIGN, ArgumentTypes::STMT));

// Select s such that ParentStar(p, s) - always false, return empty set
Query ParentStarPrintStmt = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::PARENT_T, "p", "s", ArgumentTypes::PRINT, ArgumentTypes::STMT));
// Select s such that ParentStar(a, s1) - always false, return empty set
Query argNotInSelectParentStarPrintStmt = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::PARENT_T, "p", "s1", ArgumentTypes::PRINT, ArgumentTypes::STMT));

// Select s such that ParentStar(r, s) - always false, return empty set
Query ParentStarReadStmt = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::PARENT_T, "r", "s", ArgumentTypes::READ, ArgumentTypes::STMT));
// Select s such that ParentStar(a, s1) - always false, return empty set
Query argNotInSelectParentStarReadStmt = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::PARENT_T, "r", "s1", ArgumentTypes::READ, ArgumentTypes::STMT));

// Select s such that ParentStar(i, s) - return descendents of if stmts
Query ParentStarIfStmt = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::PARENT_T, "i", "s", ArgumentTypes::IF, ArgumentTypes::STMT));
// Select s such that ParentStar(i, s1) - always true, return all stmts
Query argNotInSelectParentStarIfStmt = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::PARENT_T, "i", "s1", ArgumentTypes::IF, ArgumentTypes::STMT));

// Select s such that ParentStar(w, s) - return descendents of while stmts
Query ParentStarWhileStmt = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::PARENT_T, "w", "s", ArgumentTypes::WHILE, ArgumentTypes::STMT));
// Select s such that ParentStar(w, s1) - always true, return all stmts
Query argNotInSelectParentStarWhileStmt = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::PARENT_T, "w", "s1", ArgumentTypes::WHILE, ArgumentTypes::STMT));

/* ===== Select s such that ParentStar(*, INT) ===== */

// Select s such that ParentStar(a, 3) - always false, return empty set
Query ParentStarAssignInt = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::PARENT_T, "a", "3", ArgumentTypes::ASSIGN, ArgumentTypes::INT));

// Select s such that ParentStar(p, 9) - always false, return empty set
Query ParentStarPrintInt = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::PARENT_T, "p", "9", ArgumentTypes::PRINT, ArgumentTypes::INT));

// Select s such that ParentStar(r, 7) - always false, return empty set 
Query ParentStarReadInt = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::PARENT_T, "r", "7", ArgumentTypes::READ, ArgumentTypes::INT));

// Select s such that ParentStar(i, 11) - parent of 11 is 8 which is an if stmt, return all stmts
Query ParentStarIfIntHasResult = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::PARENT_T, "i", "11", ArgumentTypes::IF, ArgumentTypes::INT));
// Select s such that ParentStar(i, 3) - parent of 3 is 2 which is NOT an if stmt, return empty set
Query ParentStarIfIntNoResult = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::PARENT_T, "i", "3", ArgumentTypes::IF, ArgumentTypes::INT));

// Select s such that ParentStar(w, 2) - parent of 2 is 1 which is a while stmt, return all stmts
Query ParentStarWhileIntHasResult = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::PARENT_T, "w", "2", ArgumentTypes::WHILE, ArgumentTypes::INT));
// Select s such that ParentStar(w, 19) - ancestors of 19 are 17, 18 which is NOT a while stmt, return empty set
Query ParentStarWhileIntNoResult = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::PARENT_T, "w", "19", ArgumentTypes::WHILE, ArgumentTypes::INT));

/* ===== Select s such that ParentStar(a, *) ===== */

// Select s such that ParentStar(a1, a2) - always false, return empty set
Query ParentStarAssignAssign = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::PARENT_T, "a1", "a2", ArgumentTypes::ASSIGN, ArgumentTypes::ASSIGN));
Query edgeCaseParentStarAssignAssignSame = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::PARENT_T, "a", "a", ArgumentTypes::PRINT, ArgumentTypes::PRINT)); // repeated synonym

// Select s such that ParentStar(a, p) - always false, return empty set
Query ParentStarAssignPrint = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::PARENT_T, "a", "p", ArgumentTypes::ASSIGN, ArgumentTypes::PRINT));

// Select s such that ParentStar(a, r) - always false, return empty set
Query ParentStarAssignRead = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::PARENT_T, "a", "r", ArgumentTypes::ASSIGN, ArgumentTypes::READ));

// Select s such that ParentStar(a, i) - always false, return empty set
Query ParentStarAssignIf = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::PARENT_T, "a", "i", ArgumentTypes::ASSIGN, ArgumentTypes::IF));

// Select s such that ParentStar(a, w) - always false, return empty set
Query ParentStarAssignWhile = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::PARENT_T, "a", "w", ArgumentTypes::ASSIGN, ArgumentTypes::WHILE));

/* ===== Select s such that ParentStar(p, *) ===== */

// Select s such that ParentStar(p1, p2) - always false, return empty set
Query ParentStarPrintPrint = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::PARENT_T, "p1", "p2", ArgumentTypes::PRINT, ArgumentTypes::PRINT));
Query edgeCaseParentStarPrintPrintSame = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::PARENT_T, "p", "p", ArgumentTypes::PRINT, ArgumentTypes::PRINT)); // repeated synonym

// Select s such that ParentStar(p, a) - always false, return empty set
Query ParentStarPrintAssign = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::PARENT_T, "p", "a", ArgumentTypes::PRINT, ArgumentTypes::ASSIGN));

// Select s such that ParentStar(p, r) - always false, return empty set
Query ParentStarPrintRead = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::PARENT_T, "p", "r", ArgumentTypes::PRINT, ArgumentTypes::READ));

// Select s such that ParentStar(p, i) - always false, return empty set
Query ParentStarPrintIf = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::PARENT_T, "p", "i", ArgumentTypes::PRINT, ArgumentTypes::IF));

// Select s such that ParentStar(p, w) - always false, return empty set
Query ParentStarPrintWhile = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::PARENT_T, "p", "w", ArgumentTypes::PRINT, ArgumentTypes::WHILE));

/* ===== Select s such that ParentStar(r, *) ===== */

// Select s such that ParentStar(r1, r2) - always false, return empty set
Query ParentStarReadRead = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::PARENT_T, "r1", "r2", ArgumentTypes::READ, ArgumentTypes::READ));
Query edgeCaseParentStarReadReadSame = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::PARENT_T, "r", "r", ArgumentTypes::READ, ArgumentTypes::READ)); // repeated synonym

// Select s such that ParentStar(r, a) - always false, return empty set
Query ParentStarReadAssign = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::PARENT_T, "r", "a", ArgumentTypes::READ, ArgumentTypes::ASSIGN));

// Select s such that ParentStar(r, p) - always false, return empty set
Query ParentStarReadPrint = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::PARENT_T, "r", "p", ArgumentTypes::READ, ArgumentTypes::PRINT));

// Select s such that ParentStar(r, i) - always false, return empty set
Query ParentStarReadIf = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::PARENT_T, "r", "i", ArgumentTypes::READ, ArgumentTypes::IF));

// Select s such that ParentStar(r, w) - always false, return empty set
Query ParentStarReadWhile = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::PARENT_T, "r", "w", ArgumentTypes::READ, ArgumentTypes::WHILE));

/* ===== Select s such that ParentStar(i, *) ===== */

// Select s such that ParentStar(i1, i2) - ParentStar(17, 18) true, return all stmts
Query ParentStarIfIf = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::PARENT_T, "i1", "i2", ArgumentTypes::IF, ArgumentTypes::IF));
// Select s such that ParentStar(i, i) - always false, return empty set
Query edgeCaseParentStarIfIfSame = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::PARENT_T, "i", "i", ArgumentTypes::IF, ArgumentTypes::IF)); // repeated synonym

// Select s such that ParentStar(i, a) - ParentStar(14, 15) true, return all stmts
Query ParentStarIfAssign = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::PARENT_T, "i", "a", ArgumentTypes::IF, ArgumentTypes::ASSIGN));

// Select s such that ParentStar(i, p) - ParentStar(18, 19) true, return all stmts
Query ParentStarIfPrint = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::PARENT_T, "i", "p", ArgumentTypes::IF, ArgumentTypes::PRINT));

// Select s such that ParentStar(i, r) - ParentStar(14, 16) true, return all stmts
Query ParentStarIfRead = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::PARENT_T, "i", "r", ArgumentTypes::IF, ArgumentTypes::READ));

// Select s such that ParentStar(i, w) - false for this program, return empty set
Query ParentStarIfWhile = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::PARENT_T, "i", "w", ArgumentTypes::IF, ArgumentTypes::WHILE));

/* ===== Select s such that ParentStar(w, *) ===== */

// Select s such that ParentStar(w1, w2) - ParentStar(1, 2) true, return all stmts 
Query ParentStarWhileWhile = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::PARENT_T, "w1", "w2", ArgumentTypes::WHILE, ArgumentTypes::WHILE));
Query edgeCaseParentStarWhileWhileSame = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::PARENT_T, "w", "w", ArgumentTypes::WHILE, ArgumentTypes::WHILE));  // repeated synonym

// Select s such that ParentStar(w, a) - ParentStar(12, 13) true, return all stmts
Query ParentStarWhileAssign = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::PARENT_T, "w", "a", ArgumentTypes::WHILE, ArgumentTypes::ASSIGN));

// Select s such that ParentStar(w, p) - true for this program, return all stmts
Query ParentStarWhilePrint = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::PARENT_T, "w", "p", ArgumentTypes::WHILE, ArgumentTypes::PRINT));

// Select s such that ParentStar(w, r) - ParentStar(2, 3) true, return all stmts
Query ParentStarWhileRead = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::PARENT_T, "w", "r", ArgumentTypes::WHILE, ArgumentTypes::READ));

// Select s such that ParentStar(w, i) - ParentStar(12, 14) true, return all stmts
Query ParentStarWhileIf = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::PARENT_T, "w", "i", ArgumentTypes::WHILE, ArgumentTypes::IF));