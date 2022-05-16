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
6			sum = sum - subtract;
	} else {
7			call main2;
	}
8	if (sum < subtract) then {
9		subtract = subtract + sum;
10		print subtract;
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

procedure main2 {
20 c = 10;
}
*/

std::unordered_set<std::string> allStmts = { "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "20" };
std::unordered_set<std::string> allChildren = { "2", "3", "6", "7", "9", "10", "11", "13", "14", "15", "16", "18", "19" };
std::unordered_set<std::string> allParents = { "1", "2", "5", "8", "12", "14", "17", "18" };
std::unordered_set<std::string> parentsOfAssign = { "5", "8", "12" };
std::unordered_set<std::string> parentsOfPrint = { "8", "14", "18" };
std::unordered_set<std::string> parentsOfRead = { "2", "14" };
std::unordered_set<std::string> parentsOfIf = { "12", "17" };
std::unordered_set<std::string> parentsOfWhile = { "1" };
std::unordered_set<std::string> parentsOfCall = { "5" };
std::unordered_set<std::string> childrenOfIf = { "6", "7", "9", "10", "11", "15", "16", "18", "19" };
std::unordered_set<std::string> childrenOfWhile = { "2", "3", "13", "14" };

// Select s such that Parent(_, _) - if a container stmt exists, returns all stmts
Query ParentAnyAny = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::PARENT, "_", "_", ArgumentTypes::ANY, ArgumentTypes::ANY));

// Select s such that Parent(_, s) - if any, return all children stmts
Query ParentAnyStmt = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::PARENT, "_", "s", ArgumentTypes::ANY, ArgumentTypes::STMT));
// Select s such that Parent(_, s1) - if a container stmt exists, returns all stmts
Query argNotInSelectParentAnyStmt = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::PARENT, "_", "s1", ArgumentTypes::ANY, ArgumentTypes::STMT));

// Select s such that Parent(s, _) - if any, return all container (parent) stmts
Query ParentStmtAny = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::PARENT, "s", "_", ArgumentTypes::STMT, ArgumentTypes::ANY));
// Select s such that Parent(s1, _) - if a container stmt exists, returns all stmts
Query argNotInSelectParentStmtAny = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::PARENT, "s1", "_", ArgumentTypes::STMT, ArgumentTypes::ANY));

// Select s such that Parent(s, s2) - if any, return all container (parent) stmts
Query ParentStmtStmt = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::PARENT, "s", "s2", ArgumentTypes::STMT, ArgumentTypes::STMT));
// Select s such that Parent(s, s) - return empty set
Query edgeCaseParentSameStmt = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::PARENT, "s", "s", ArgumentTypes::STMT, ArgumentTypes::STMT));
// Select s such that Parent(s1, s2) - same as Parent(_, _), return all stmts
Query argNotInSelectParentStmtStmt = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::PARENT, "s1", "s2", ArgumentTypes::STMT, ArgumentTypes::STMT));

/* ===== Select s such that Parent(*, INT) ===== */

// Select s such that Parent(_, 9) - if stmt 9 has parent, return all stmts
Query ParentAnyInt = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::PARENT, "_", "9", ArgumentTypes::ANY, ArgumentTypes::INT));
// stmt 8 has no parent - return empty set
Query argNotInSelectParentAnyInt = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::PARENT, "_", "8", ArgumentTypes::ANY, ArgumentTypes::INT));
Query invalidStmtNoParentAnyInt1 = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::PARENT, "_", "50", ArgumentTypes::ANY, ArgumentTypes::INT));
Query invalidStmtNoParentAnyInt2 = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::PARENT, "_", "0", ArgumentTypes::ANY, ArgumentTypes::INT));

// Select s such that Parent(s, 16) - return 14
Query ParentStmtInt = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::PARENT, "s", "16", ArgumentTypes::STMT, ArgumentTypes::INT));
// Select s such that Parent(s1, 11) - 11 has parent, return all stmts
Query argNotInSelectParentStmtInt = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::PARENT, "s1", "11", ArgumentTypes::STMT, ArgumentTypes::INT));

/* ===== Select s such that Parent(s, *) ===== */

// Select s such that Parent(_, a) - true, return all stmtNo 
Query ParentAnyAssign = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::PARENT, "_", "a", ArgumentTypes::ANY, ArgumentTypes::ASSIGN));

// Select s such that Parent(_, p) - true, return all stmtNo
Query ParentAnyPrint = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::PARENT, "_", "p", ArgumentTypes::ANY, ArgumentTypes::PRINT));

// Select s such that Parent(_, r) - true, return all stmtNo
Query ParentAnyRead = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::PARENT, "_", "r", ArgumentTypes::ANY, ArgumentTypes::READ));

// Select s such that Parent(_, i) - true, return all stmtNo 
Query ParentAnyIf = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::PARENT, "_", "i", ArgumentTypes::ANY, ArgumentTypes::IF));

// Select s such that Parent(_, w) - true, return all stmts
Query ParentAnyWhile = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::PARENT, "_", "w", ArgumentTypes::ANY, ArgumentTypes::WHILE));

/* ===== Select s such that Parent(s, *) ===== */

// Select s such that Parent(s, a) - if any, return all parents of assign stmts
Query ParentStmtAssign = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::PARENT, "s", "a", ArgumentTypes::STMT, ArgumentTypes::ASSIGN));
// Select s such that Parent(s1, a) - if an assign stmt has parent, return all stmtNo 
Query argNotInSelectParentStmtAssign = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::PARENT, "s1", "a", ArgumentTypes::STMT, ArgumentTypes::ASSIGN));

// Select s such that Parent(s, p) - if any, return all parents of print stmts
Query ParentStmtPrint = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::PARENT, "s", "p", ArgumentTypes::STMT, ArgumentTypes::PRINT));
// Select s such that Parent(s1, p) - if a print stmt has parent, return all stmts
Query argNotInSelectParentStmtPrint = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::PARENT, "s1", "p", ArgumentTypes::STMT, ArgumentTypes::PRINT));

// Select s such that Parent(s, r) - if any, return all parents of read stmts
Query ParentStmtRead = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::PARENT, "s", "r", ArgumentTypes::STMT, ArgumentTypes::READ));
// Select s such that Parent(s1, r) - if a read stmt has parent, return all stmts
Query argNotInSelectParentStmtRead = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::PARENT, "s1", "r", ArgumentTypes::STMT, ArgumentTypes::READ));

// Select s such that Parent(s, i) - if any, return all parents of if stmts
Query ParentStmtIf = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::PARENT, "s", "i", ArgumentTypes::STMT, ArgumentTypes::IF));
// Select s such that Parent(s1, i) - if an if stmt has parent, return all stmts
Query argNotInSelectParentStmtIf = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::PARENT, "s1", "i", ArgumentTypes::STMT, ArgumentTypes::IF));

// Select s such that Parent(s, w) - if any, return all parents of while stmts
Query ParentStmtWhile = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::PARENT, "s", "w", ArgumentTypes::STMT, ArgumentTypes::WHILE));
// Select s such that Parent(s1, w) - if a while stmt has parent, return all stmts
Query argNotInSelectParentStmtWhile = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::PARENT, "s1", "w", ArgumentTypes::STMT, ArgumentTypes::WHILE));

/* ===== Select s such that Parent(INT, *) ===== */

// Select s such that Parent(5, _) - 5 is parent, return all stmts
Query ParentIntAnyHasResult = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::PARENT, "5", "_", ArgumentTypes::INT, ArgumentTypes::ANY));
// Select s such that Parent(4, _) - 4 not parent, return empty set
Query ParentIntAnyNoResult = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::PARENT, "4", "_", ArgumentTypes::INT, ArgumentTypes::ANY));
Query ParentIntAnyInvalidStmtNo1 = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::PARENT, "50", "_", ArgumentTypes::INT, ArgumentTypes::ANY));
Query ParentIntAnyInvalidStmtNo2 = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::PARENT, "0", "_", ArgumentTypes::INT, ArgumentTypes::ANY));

// Select s such that Parent(8, s) - 8 is parent, return stmts 9, 10, 11
Query ParentIntStmtHasResult = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::PARENT, "8", "s", ArgumentTypes::INT, ArgumentTypes::STMT));
// Select s such that Parent(14, s) - 14 is parent, return 15, 16
Query ParentIntStmtNoResult = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::PARENT, "14", "s", ArgumentTypes::INT, ArgumentTypes::STMT));
// Select s such that Parent(12, s1) - 12 is parent, return all stmts
Query argNotInSelectParentIntStmt = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::PARENT, "12", "s1", ArgumentTypes::INT, ArgumentTypes::STMT));

/* ===== Select s such that Parent(INT, INT) ===== */

// Select s such that Parent(12, 13) - true, return all stmts
Query ParentIntIntTrue = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::PARENT, "12", "13", ArgumentTypes::INT, ArgumentTypes::INT));
// Select s such that Parent(5, 12) - false, return empty set
Query ParentIntIntFalse = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::PARENT, "5", "12", ArgumentTypes::INT, ArgumentTypes::INT));
// Select s such that Parent(12, 15) - false (not direct parent), return empty set
Query ParentIntIntNotDirectParent = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::PARENT, "5", "12", ArgumentTypes::INT, ArgumentTypes::INT));
// Select s such that Parent(12, 5) - false, return empty set
Query edgeCaseParentIntIntBiggerSmaller = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::PARENT, "12", "5", ArgumentTypes::INT, ArgumentTypes::INT)); // LHS > RHS
// Select s such that Parent(5, 5) - false, return empty set
Query edgeCaseParentIntIntSame = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::PARENT, "5", "5", ArgumentTypes::INT, ArgumentTypes::INT)); // LHS = RHS

/* ===== Select s such that Parent(INT, *) ===== */

// Select s such that Parent(5, a) - 5 is parent of assign stmt, return all stmts
Query ParentIntAssignHasResult = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::PARENT, "5", "a", ArgumentTypes::INT, ArgumentTypes::ASSIGN));
// Select s such that Parent(14, a) - 14 not a parent of assign stmt, return empty set
Query ParentIntAssignNoResult = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::PARENT, "14", "a", ArgumentTypes::INT, ArgumentTypes::ASSIGN));

// Select s such that Parent(8, p) - 8 is parent of print stmt, return all stmts
Query ParentIntPrintHasResult = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::PARENT, "8", "p", ArgumentTypes::INT, ArgumentTypes::PRINT));
// Select s such that Parent(12, p) - 12 not direct parent of print stmt, return empty set
Query ParentIntPrintNoResult = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::PARENT, "12", "p", ArgumentTypes::INT, ArgumentTypes::PRINT));

// Select s such that Parent(14, r) - 14 is parent of read stmt, return all stmts
Query ParentIntReadHasResult = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::PARENT, "14", "r", ArgumentTypes::INT, ArgumentTypes::READ));
// Select s such that Parent(12, r) - 12 not direct parent of read stmt, return empty set
Query ParentIntReadNoResult = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::PARENT, "12", "r", ArgumentTypes::INT, ArgumentTypes::READ));

// Select s such that Parent(12, i) - 12 is parent of if stmt, return all stmts
Query ParentIntIfHasResult = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::PARENT, "12", "i", ArgumentTypes::INT, ArgumentTypes::IF));
// Select s such that Parent(5, i) - 5 not parent of if stmt, return empty set
Query ParentIntIfNoResult = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::PARENT, "5", "i", ArgumentTypes::INT, ArgumentTypes::IF));

// Select s such that Parent(1, w) - 1 is parent of while stmt, return all stmts
Query ParentIntWhileHasResult = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::PARENT, "1", "w", ArgumentTypes::INT, ArgumentTypes::WHILE));
// Select s such that Parent(2, w) - 2 not parent of while stmt, return empty set
Query ParentIntWhileNoResult = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::PARENT, "2", "w", ArgumentTypes::INT, ArgumentTypes::WHILE));

/* ===== Select s such that Parent(*, _) ===== */

// Select s such that Parent(a, _) - always false, return empty set
Query ParentAssignAny = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::PARENT, "a", "_", ArgumentTypes::ASSIGN, ArgumentTypes::ANY));

// Select s such that Parent(p, _) - always false, return empty set
Query ParentPrintAny = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::PARENT, "p", "_", ArgumentTypes::PRINT, ArgumentTypes::ANY));

// Select s such that Parent(r, _) - always false, return empty set
Query ParentReadAny = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::PARENT, "r", "_", ArgumentTypes::READ, ArgumentTypes::ANY));

// Select s such that Parent(i, _) - always true, return all stmts
Query ParentIfAny = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::PARENT, "i", "_", ArgumentTypes::IF, ArgumentTypes::ANY));

// Select s such that Parent(w, _) - always true, return all stmts
Query ParentWhileAny = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::PARENT, "w", "_", ArgumentTypes::WHILE, ArgumentTypes::ANY));

/* ===== Select s such that Parent(*, s) ===== */

// Select s such that Parent(a, s) - always false, return empty set
Query ParentAssignStmt = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::PARENT, "a", "s", ArgumentTypes::ASSIGN, ArgumentTypes::STMT));
// Select s such that Parent(a, s1) - always false, return empty set
Query argNotInSelectParentAssignStmt = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::PARENT, "a", "s1", ArgumentTypes::ASSIGN, ArgumentTypes::STMT));

// Select s such that Parent(p, s) - always false, return empty set
Query ParentPrintStmt = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::PARENT, "p", "s", ArgumentTypes::PRINT, ArgumentTypes::STMT));
// Select s such that Parent(a, s1) - always false, return empty set
Query argNotInSelectParentPrintStmt = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::PARENT, "p", "s1", ArgumentTypes::PRINT, ArgumentTypes::STMT));

// Select s such that Parent(r, s) - always false, return empty set
Query ParentReadStmt = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::PARENT, "r", "s", ArgumentTypes::READ, ArgumentTypes::STMT));
// Select s such that Parent(a, s1) - always false, return empty set
Query argNotInSelectParentReadStmt = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::PARENT, "r", "s1", ArgumentTypes::READ, ArgumentTypes::STMT));

// Select s such that Parent(i, s) - return children of if stmts
Query ParentIfStmt = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::PARENT, "i", "s", ArgumentTypes::IF, ArgumentTypes::STMT));
// Select s such that Parent(i, s1) - always true, return all stmts
Query argNotInSelectParentIfStmt = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::PARENT, "i", "s1", ArgumentTypes::IF, ArgumentTypes::STMT));

// Select s such that Parent(w, s) - return children of while stmts
Query ParentWhileStmt = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::PARENT, "w", "s", ArgumentTypes::WHILE, ArgumentTypes::STMT));
// Select s such that Parent(w, s1) - always true, return all stmts
Query argNotInSelectParentWhileStmt = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::PARENT, "w", "s1", ArgumentTypes::WHILE, ArgumentTypes::STMT));

/* ===== Select s such that Parent(*, INT) ===== */

// Select s such that Parent(a, 3) - always false, return empty set
Query ParentAssignInt = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::PARENT, "a", "3", ArgumentTypes::ASSIGN, ArgumentTypes::INT));

// Select s such that Parent(p, 9) - always false, return empty set
Query ParentPrintInt = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::PARENT, "p", "9", ArgumentTypes::PRINT, ArgumentTypes::INT));

// Select s such that Parent(r, 7) - always false, return empty set 
Query ParentReadInt = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::PARENT, "r", "7", ArgumentTypes::READ, ArgumentTypes::INT));

// Select s such that Parent(i, 11) - parent of 11 is 8 which is an if stmt, return all stmts
Query ParentIfIntHasResult = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::PARENT, "i", "11", ArgumentTypes::IF, ArgumentTypes::INT));
// Select s such that Parent(i, 3) - parent of 3 is 2 which is NOT an if stmt, return empty set
Query ParentIfIntNoResult = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::PARENT, "i", "3", ArgumentTypes::IF, ArgumentTypes::INT));

// Select s such that Parent(w, 2) - parent of 2 is 1 which is a while stmt, return all stmts
Query ParentWhileIntHasResult = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::PARENT, "w", "2", ArgumentTypes::WHILE, ArgumentTypes::INT));
// Select s such that Parent(w, 15) - parent of 15 is 14 which is NOT a while stmt, return empty set
Query ParentWhileIntNoResult = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::PARENT, "w", "15", ArgumentTypes::WHILE, ArgumentTypes::INT));

/* ===== Select s such that Parent(a, *) ===== */

// Select s such that Parent(a1, a2) - always false, return empty set
Query ParentAssignAssign = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::PARENT, "a1", "a2", ArgumentTypes::ASSIGN, ArgumentTypes::ASSIGN));
Query edgeCaseParentAssignAssignSame = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::PARENT, "a", "a", ArgumentTypes::PRINT, ArgumentTypes::PRINT)); // repeated synonym

// Select s such that Parent(a, p) - always false, return empty set
Query ParentAssignPrint = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::PARENT, "a", "p", ArgumentTypes::ASSIGN, ArgumentTypes::PRINT));

// Select s such that Parent(a, r) - always false, return empty set
Query ParentAssignRead = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::PARENT, "a", "r", ArgumentTypes::ASSIGN, ArgumentTypes::READ));

// Select s such that Parent(a, i) - always false, return empty set
Query ParentAssignIf = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::PARENT, "a", "i", ArgumentTypes::ASSIGN, ArgumentTypes::IF));

// Select s such that Parent(a, w) - always false, return empty set
Query ParentAssignWhile = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::PARENT, "a", "w", ArgumentTypes::ASSIGN, ArgumentTypes::WHILE));

/* ===== Select s such that Parent(p, *) ===== */

// Select s such that Parent(p1, p2) - always false, return empty set
Query ParentPrintPrint = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::PARENT, "p1", "p2", ArgumentTypes::PRINT, ArgumentTypes::PRINT));
Query edgeCaseParentPrintPrintSame = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::PARENT, "p", "p", ArgumentTypes::PRINT, ArgumentTypes::PRINT)); // repeated synonym

// Select s such that Parent(p, a) - always false, return empty set
Query ParentPrintAssign = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::PARENT, "p", "a", ArgumentTypes::PRINT, ArgumentTypes::ASSIGN));

// Select s such that Parent(p, r) - always false, return empty set
Query ParentPrintRead = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::PARENT, "p", "r", ArgumentTypes::PRINT, ArgumentTypes::READ));

// Select s such that Parent(p, i) - always false, return empty set
Query ParentPrintIf = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::PARENT, "p", "i", ArgumentTypes::PRINT, ArgumentTypes::IF));

// Select s such that Parent(p, w) - always false, return empty set
Query ParentPrintWhile = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::PARENT, "p", "w", ArgumentTypes::PRINT, ArgumentTypes::WHILE));

/* ===== Select s such that Parent(r, *) ===== */

// Select s such that Parent(r1, r2) - always false, return empty set
Query ParentReadRead = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::PARENT, "r1", "r2", ArgumentTypes::READ, ArgumentTypes::READ));
// Select s such that Parent(r, r) - always false, return empty set
Query edgeCaseParentReadReadSame = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::PARENT, "r", "r", ArgumentTypes::READ, ArgumentTypes::READ)); // repeated synonym

// Select s such that Parent(r, a) - always false, return empty set
Query ParentReadAssign = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::PARENT, "r", "a", ArgumentTypes::READ, ArgumentTypes::ASSIGN));

// Select s such that Parent(r, p) - always false, return empty set
Query ParentReadPrint = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::PARENT, "r", "p", ArgumentTypes::READ, ArgumentTypes::PRINT));

// Select s such that Parent(r, i) - always false, return empty set
Query ParentReadIf = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::PARENT, "r", "i", ArgumentTypes::READ, ArgumentTypes::IF));

// Select s such that Parent(r, w) - always false, return empty set
Query ParentReadWhile = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::PARENT, "r", "w", ArgumentTypes::READ, ArgumentTypes::WHILE));

/* ===== Select s such that Parent(i, *) ===== */

// Select s such that Parent(i1, i2) - Parent(17, 18) true, return all stmts
Query ParentIfIf = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::PARENT, "i1", "i2", ArgumentTypes::IF, ArgumentTypes::IF));

// Select s such that Parent(i, i) - always false, return empty set
Query edgeCaseParentIfIfSame = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::PARENT, "i", "i", ArgumentTypes::IF, ArgumentTypes::IF)); // repeated synonym

// Select s such that Parent(i, a) - Parent(14, 15) true, return all stmts
Query ParentIfAssign = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::PARENT, "i", "a", ArgumentTypes::IF, ArgumentTypes::ASSIGN));

// Select s such that Parent(i, p) - Parent(18, 19) true, return all stmts
Query ParentIfPrint = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::PARENT, "i", "p", ArgumentTypes::IF, ArgumentTypes::PRINT));

// Select s such that Parent(i, r) - Parent(14, 16) true, return all stmts
Query ParentIfRead = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::PARENT, "i", "r", ArgumentTypes::IF, ArgumentTypes::READ));

// Select s such that Parent(i, w) - false for this program, return empty set
Query ParentIfWhile = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::PARENT, "i", "w", ArgumentTypes::IF, ArgumentTypes::WHILE));

/* ===== Select s such that Parent(w, *) ===== */

// Select s such that Parent(w1, w2) - Parent(1, 2) true, return all stmts 
Query ParentWhileWhile = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::PARENT, "w1", "w2", ArgumentTypes::WHILE, ArgumentTypes::WHILE));

// Select s such that Parent(w, w) - always false, return empty set
Query edgeCaseParentWhileWhileSame = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::PARENT, "w", "w", ArgumentTypes::WHILE, ArgumentTypes::WHILE));  // repeated synonym

// Select s such that Parent(w, a) - Parent(12, 13) true, return all stmts
Query ParentWhileAssign = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::PARENT, "w", "a", ArgumentTypes::WHILE, ArgumentTypes::ASSIGN));

// Select s such that Parent(w, p) - false for this program, return empty set
Query ParentWhilePrint = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::PARENT, "w", "p", ArgumentTypes::WHILE, ArgumentTypes::PRINT));

// Select s such that Parent(w, r) - Parent(2, 3) true, return all stmts
Query ParentWhileRead = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::PARENT, "w", "r", ArgumentTypes::WHILE, ArgumentTypes::READ));

// Select s such that Parent(w, i) - Parent(12, 14) true, return all stmts
Query ParentWhileIf = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::PARENT, "w", "i", ArgumentTypes::WHILE, ArgumentTypes::IF));

/*
Prog_line and call stmt
*/
// Select n such that Parent(n, s) - true, return all stmts that are parents
Query ParentProgLineStmtInClause = Query("n", ArgumentTypes::PROG_LINE, QuerySuchThatClause(RelRef::PARENT, "n", "s", ArgumentTypes::PROG_LINE, ArgumentTypes::STMT));

// Select n such that Parent(s1, s2) - true, return all prog_line
Query ParentProgLineStmtNotInClause = Query("n", ArgumentTypes::PROG_LINE, QuerySuchThatClause(RelRef::PARENT, "s1", "s2", ArgumentTypes::STMT, ArgumentTypes::STMT));

// Select s such that Parent(s, c) - true, return all parents of call stmts
Query ParentStmtCallInClause = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::PARENT, "s", "c", ArgumentTypes::STMT, ArgumentTypes::CALL));

// Select s such that Parent(n, c) - true, return all stmts
Query ParentStmtCallNotInClause = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::PARENT, "n", "c", ArgumentTypes::PROG_LINE, ArgumentTypes::CALL));

// Select s such that Parent(s, n) - true, return all parent stmts
Query ParentStmtProgLineInClause = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::PARENT, "s", "n", ArgumentTypes::STMT, ArgumentTypes::PROG_LINE));

// Select s such that Parent(n, c) - true, return all stmts
Query ParentStmtProgLineNotInClause = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::PARENT, "n", "c", ArgumentTypes::PROG_LINE, ArgumentTypes::CALL));
