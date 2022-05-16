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
8						print a;
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
*/

// Select s pattern a(_, _) - if true, returns all stmtNo
Query AssignAnyAnyS = Query("s", ArgumentTypes::STMT, QueryPatternClause("a", "_", "_", ArgumentTypes::ASSIGN, ArgumentTypes::ANY, ArgumentTypes::ANY));
// Select a pattern a(_, _) - if true, returns all assign stmtNo
Query AssignAnyAnyA = Query("a", ArgumentTypes::ASSIGN, QueryPatternClause("a", "_", "_", ArgumentTypes::ASSIGN, ArgumentTypes::ANY, ArgumentTypes::ANY));

// Select s pattern a(v, _) - if there is a variable modified, returns all stmtNo
Query AssignVariableAnyS = Query("s", ArgumentTypes::STMT, QueryPatternClause("a", "v", "_", ArgumentTypes::ASSIGN, ArgumentTypes::VARIABLE, ArgumentTypes::ANY));
// Select a pattern a(v, _) - if there is a variable modified, return all assign stmtNo
Query AssignVariableAnyA = Query("a", ArgumentTypes::ASSIGN, QueryPatternClause("a", "v", "_", ArgumentTypes::ASSIGN, ArgumentTypes::VARIABLE, ArgumentTypes::ANY));
// Select v a(v, _) - if true, returns all variables modified in assign statement
Query AssignVariableAnyV = Query("v", ArgumentTypes::VARIABLE, QueryPatternClause("a", "v", "_", ArgumentTypes::ASSIGN, ArgumentTypes::VARIABLE, ArgumentTypes::ANY));

// Select s a(_, _"ifs"_) - if true, returns all stmtNo
Query AssignAnyString = Query("s", ArgumentTypes::STMT, QueryPatternClause("a", "_", "ifs", ArgumentTypes::ASSIGN, ArgumentTypes::ANY, ArgumentTypes::STRING));
Query InvalidAssignAnyString = Query("s", ArgumentTypes::STMT, QueryPatternClause("a", "_", "pattern", ArgumentTypes::ASSIGN, ArgumentTypes::ANY, ArgumentTypes::STRING));

// Select a a(_, _"ifs"_) - if true, returns all assign stmtNo with ifs in expression
Query AssignAnyStringA1 = Query("a", ArgumentTypes::ASSIGN, QueryPatternClause("a", "_", "ifs", ArgumentTypes::ASSIGN, ArgumentTypes::ANY, ArgumentTypes::STRING));
// Select a a(_, _"10"_) - if true, returns all assign stmtNo with 10 in expression
Query AssignAnyStringA2 = Query("a", ArgumentTypes::ASSIGN, QueryPatternClause("a", "_", "10", ArgumentTypes::ASSIGN, ArgumentTypes::ANY, ArgumentTypes::STRING));

// Select s a(v, _"10"_) - if true, returns all stmtNo
Query AssignVariableString = Query("s", ArgumentTypes::STMT, QueryPatternClause("a", "v", "10", ArgumentTypes::ASSIGN, ArgumentTypes::VARIABLE, ArgumentTypes::STRING));
Query InvalidAssignVariableString = Query("s", ArgumentTypes::STMT, QueryPatternClause("a", "v", "pattern", ArgumentTypes::ASSIGN, ArgumentTypes::VARIABLE, ArgumentTypes::STRING));

// Select a a(v, _"ifs"_) - if true, returns all assign stmtNo with ifs in expression
Query AssignVariableStringA = Query("a", ArgumentTypes::ASSIGN, QueryPatternClause("a", "v", "ifs", ArgumentTypes::ASSIGN, ArgumentTypes::VARIABLE, ArgumentTypes::STRING));
// Select v a(v, _"ifs"_) - if true, returns all variable in the assign stmt matched
Query AssignVariableStringV = Query("v", ArgumentTypes::VARIABLE, QueryPatternClause("a", "v", "ifs", ArgumentTypes::ASSIGN, ArgumentTypes::VARIABLE, ArgumentTypes::STRING));

// Select s a("pattern", _) - if true, returns all assign stmtNo with ifs in expression
Query AssignStringAny = Query("s", ArgumentTypes::STMT, QueryPatternClause("a", "pattern", "_", ArgumentTypes::ASSIGN, ArgumentTypes::STRING, ArgumentTypes::ANY));
Query InvalidAssignStringAny = Query("s", ArgumentTypes::STMT, QueryPatternClause("a", "w", "_", ArgumentTypes::ASSIGN, ArgumentTypes::STRING, ArgumentTypes::ANY));

// Select a a("pattern", _) - if true, returns all assign stmtNo that uses pattern
Query AssignStringAnyA = Query("a", ArgumentTypes::ASSIGN, QueryPatternClause("a", "a", "_", ArgumentTypes::ASSIGN, ArgumentTypes::STRING, ArgumentTypes::ANY));

// Select s a("pattern", "ifs") - if true, return all stmtNo
Query AssignStringString = Query("s", ArgumentTypes::STMT, QueryPatternClause("a", "pattern", "ifs", ArgumentTypes::ASSIGN, ArgumentTypes::STRING, ArgumentTypes::STRING));
Query InvalidAssignStringString = Query("s", ArgumentTypes::STMT, QueryPatternClause("a", "pattern", "hadouken", ArgumentTypes::ASSIGN, ArgumentTypes::STRING, ArgumentTypes::STRING));

// Select a a("pattern", "ifs") - if true, returns all assign stmtNo that modifies "pattern" and uses "ifs"
Query AssignStringStringA = Query("a", ArgumentTypes::ASSIGN, QueryPatternClause("a", "pattern", "ifs", ArgumentTypes::ASSIGN, ArgumentTypes::STRING, ArgumentTypes::STRING));
Query InvalidAssignStringStringA = Query("s", ArgumentTypes::STMT, QueryPatternClause("a", "pattern", "hadouken", ArgumentTypes::ASSIGN, ArgumentTypes::STRING, ArgumentTypes::STRING));
