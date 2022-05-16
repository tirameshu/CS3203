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
17			call testB;
		}
}

procedure testB {
18	  print a;
19	  call testC;
}

procedure testC {
20	  read p;
}
*/
std::unordered_set<std::string> expected = { "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "20" };
std::unordered_set<std::string> usesStmt = { "2", "3", "4", "5", "7", "8", "9", "11", "14", "15", "17", "18" };
std::unordered_set<std::string> usedVar = { "p", "a", "ifs", "w", "c", "k" };
std::unordered_set<std::string> allVar = { "p", "a", "ifs", "w", "pattern", "c", "k" };

// Select s such that Uses(_, _) - if true, returns all stmtNo
Query UsesAnyAny = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::USES_S, "_", "_", ArgumentTypes::ANY, ArgumentTypes::ANY));

// Select s such that Uses(_, v) - if there exist a variable used, returns all stmtNo
Query UsesAnyVariable = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::USES_S, "_", "v", ArgumentTypes::ANY, ArgumentTypes::VARIABLE));

// Select s such that Uses(s, _) - if true, return all uses stmtNo that uses
Query UsesStmtAny1 = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::USES_S, "s", "_", ArgumentTypes::STMT, ArgumentTypes::ANY));

// Select s such that Uses(s1, _) - if true, return all stmtNo
Query UsesStmtAny2 = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::USES_S, "s1", "_", ArgumentTypes::STMT, ArgumentTypes::ANY));

// Select s such that Uses(s, v) - if true, return all uses stmtNo that uses
Query UsesStmtVariableS1 = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::USES_S, "s", "v", ArgumentTypes::STMT, ArgumentTypes::VARIABLE));

// Select s such that Uses(s1, v) - if true, return all stmtNo
Query UsesStmtVariableS2 = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::USES_S, "s1", "v", ArgumentTypes::STMT, ArgumentTypes::VARIABLE));

// Select v such that Uses(s, v) - if true, return all variable used
Query UsesStmtVariableV1 = Query("v", ArgumentTypes::VARIABLE, QuerySuchThatClause(RelRef::USES_S, "s", "v", ArgumentTypes::STMT, ArgumentTypes::VARIABLE));

// Select v such that Uses(s, v1) - if true, return all variables
Query UsesStmtVariableV2 = Query("v", ArgumentTypes::VARIABLE, QuerySuchThatClause(RelRef::USES_S, "s", "v1", ArgumentTypes::STMT, ArgumentTypes::VARIABLE));

// Select s such that Uses(_, "w") - if true, returns all stmtNo
Query UsesAnyString = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::USES_S, "_", "w", ArgumentTypes::ANY, ArgumentTypes::STRING));
Query InvalidUsesAnyString = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::USES_S, "_", "coolio", ArgumentTypes::ANY, ArgumentTypes::STRING));

// Select s such that Uses(s, "w") - if true, returns all stmtNo that uses w	
Query UsesStmtStringS = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::USES_S, "s", "ifs", ArgumentTypes::STMT, ArgumentTypes::STRING));
Query InvalidUsesStmtStringS = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::USES_S, "s1", "w", ArgumentTypes::STMT, ArgumentTypes::STRING)); // result synonym not in clause

// Select s such that Uses(s, "coolio") - if true, returns all stmtNo that uses coolio
Query UsesStmtStringV = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::USES_S, "s", "coolio", ArgumentTypes::STMT, ArgumentTypes::STRING));
Query InvalidUsesStmtStringV = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::USES_S, "s1", "coolio", ArgumentTypes::STMT, ArgumentTypes::STRING)); // result synonym not in clause

// Select s such that Uses(14, _) - if true, returns all stmtNo
Query UsesIntAny = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::USES_S, "14", "_", ArgumentTypes::INT, ArgumentTypes::ANY));
Query InvalidUsesIntAny = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::USES_S, "13", "_", ArgumentTypes::INT, ArgumentTypes::ANY));

// Select s such that Uses(14, v) - if true, returns all stmtNo
Query UsesIntVariableS = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::USES_S, "14", "v", ArgumentTypes::INT, ArgumentTypes::VARIABLE));
Query InvalidUsesIntVariableS = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::USES_S, "13", "v", ArgumentTypes::INT, ArgumentTypes::VARIABLE));

// Select v such that Uses(3, v) - if true, returns all variable used by stmt 3
Query UsesIntVariableV = Query("v", ArgumentTypes::VARIABLE, QuerySuchThatClause(RelRef::USES_S, "3", "v", ArgumentTypes::INT, ArgumentTypes::VARIABLE));
Query InvalidUsesIntVariableV1 = Query("v", ArgumentTypes::VARIABLE, QuerySuchThatClause(RelRef::USES_S, "3", "v1", ArgumentTypes::INT, ArgumentTypes::VARIABLE)); // result synonym not in clause
Query InvalidUsesIntVariableV2 = Query("v", ArgumentTypes::VARIABLE, QuerySuchThatClause(RelRef::USES_S, "13", "v", ArgumentTypes::INT, ArgumentTypes::VARIABLE)); // clause not true

// Select s such that Uses(3, "w") - if true, returns all stmtNo
Query UsesIntString = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::USES_S, "3", "w", ArgumentTypes::INT, ArgumentTypes::STRING));
Query InvalidUsesIntString = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::USES_S, "3", "coolio", ArgumentTypes::INT, ArgumentTypes::STRING)); // clause not true
Query InvalidUsesIntString1 = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::USES_S, "20", "coolio", ArgumentTypes::INT, ArgumentTypes::STRING));
Query InvalidUsesIntString2 = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::USES_S, "0", "coolio", ArgumentTypes::INT, ArgumentTypes::STRING));

// Select s such that Uses(a, _) - if true, returns all stmtNo
Query UsesAssignAny = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::USES_S, "a", "_", ArgumentTypes::ASSIGN, ArgumentTypes::ANY));

// Select s such that Uses(p, _) - if true, returns all stmtNo
Query UsesPrintAny = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::USES_S, "p", "_", ArgumentTypes::PRINT, ArgumentTypes::ANY));

// Select s such that Uses(i, _) - if true, returns all stmtNo
Query UsesIfAny = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::USES_S, "i", "_", ArgumentTypes::IF, ArgumentTypes::ANY));

// Select s such that Uses(w, _) - if true, returns all stmtNo
Query UsesWhileAny = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::USES_S, "w", "_", ArgumentTypes::WHILE, ArgumentTypes::ANY));

// Select s such that Uses(a, v) - if true, returns all stmtNo
Query UsesAssignVariableS = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::USES_S, "a", "v", ArgumentTypes::ASSIGN, ArgumentTypes::VARIABLE));

// Select v such that Uses(a, v) - if true, return variables used in assign stmt
Query UsesAssignVariableV = Query("v", ArgumentTypes::VARIABLE, QuerySuchThatClause(RelRef::USES_S, "a", "v", ArgumentTypes::ASSIGN, ArgumentTypes::VARIABLE));

// Select s such that Uses(p, v) - if true, returns all stmtNo
Query UsesPrintVariableS = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::USES_S, "p", "v", ArgumentTypes::PRINT, ArgumentTypes::VARIABLE));

// Select s such that Uses(p, v) - if true, return variables used in print stmt
Query UsesPrintVariableV = Query("v", ArgumentTypes::VARIABLE, QuerySuchThatClause(RelRef::USES_S, "p", "v", ArgumentTypes::PRINT, ArgumentTypes::VARIABLE));

// Select s such that Uses(i, v) - if true, returns all stmtNo
Query UsesIfVariableS = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::USES_S, "i", "v", ArgumentTypes::IF, ArgumentTypes::VARIABLE));

// Select v such that Uses(i, v) - if true, returns variables used in if stmt
Query UsesIfVariableV = Query("v", ArgumentTypes::VARIABLE, QuerySuchThatClause(RelRef::USES_S, "i", "v", ArgumentTypes::IF, ArgumentTypes::VARIABLE));

// Select s such that Uses(w, v) - if true, returns all stmtNo
Query UsesWhileVariableS = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::USES_S, "w", "v", ArgumentTypes::WHILE, ArgumentTypes::VARIABLE));

// Select v such that Uses(w, v) - if true, returns variables used in while stmt
Query UsesWhileVariableV = Query("v", ArgumentTypes::VARIABLE, QuerySuchThatClause(RelRef::USES_S, "w", "v", ArgumentTypes::WHILE, ArgumentTypes::VARIABLE));

// Select s such that Uses(a, "ifs") - if true, returns all stmtNo
Query UsesAssignString = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::USES_S, "a", "ifs", ArgumentTypes::ASSIGN, ArgumentTypes::STRING));
Query InvalidUsesAssignString = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::USES_S, "a", "coolio", ArgumentTypes::ASSIGN, ArgumentTypes::STRING));

// Select s such that Uses(p, "p") - if true, returns all stmtNo
Query UsesPrintString = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::USES_S, "p", "p", ArgumentTypes::PRINT, ArgumentTypes::STRING));
Query InvalidUsesPrintString = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::USES_S, "p", "coolio", ArgumentTypes::PRINT, ArgumentTypes::STRING));

// Select s such that Uses(i, "c") - if true, returns all stmtNo
Query UsesIfString = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::USES_S, "i", "c", ArgumentTypes::IF, ArgumentTypes::STRING));
Query InvalidUsesIfString = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::USES_S, "i", "coolio", ArgumentTypes::IF, ArgumentTypes::STRING));

// Select s such that Uses(w, "w") - if true, returns all stmtNo
Query UsesWhileString = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::USES_S, "w", "w", ArgumentTypes::WHILE, ArgumentTypes::STRING));
Query InvalidUsesWhileString = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::USES_S, "w", "coolio", ArgumentTypes::WHILE, ArgumentTypes::STRING));

/*
UsesS for call and prog_line
*/
// select c such that Uses(c, v), return specific call statement
Query UsesCallSynonymInClause = Query("c", ArgumentTypes::CALL, QuerySuchThatClause(RelRef::USES_S, "c", "v", ArgumentTypes::CALL, ArgumentTypes::VARIABLE));

// select c such that Uses(a, v), return all call statement
Query UsesCallSynonymNotInClause = Query("c", ArgumentTypes::CALL, QuerySuchThatClause(RelRef::USES_S, "a", "v", ArgumentTypes::ASSIGN, ArgumentTypes::VARIABLE));

// select c such that Uses(c, "a"), return call statements that uses variable "a"
Query UsesCallSynonymStringInClause = Query("c", ArgumentTypes::CALL, QuerySuchThatClause(RelRef::USES_S, "c", "a", ArgumentTypes::CALL, ArgumentTypes::STRING));

// select c such that Uses(c, "p"), return nothing
Query UsesCallSynonymStringInClause2 = Query("c", ArgumentTypes::CALL, QuerySuchThatClause(RelRef::USES_S, "c", "p", ArgumentTypes::CALL, ArgumentTypes::STRING));

// select c such that Uses(c, _), return call statements that uses something
Query UsesCallSynonymAnyInClause = Query("c", ArgumentTypes::CALL, QuerySuchThatClause(RelRef::USES_S, "c", "_", ArgumentTypes::CALL, ArgumentTypes::ANY));

// select c such that Uses(a, "_"), return all call statements 
Query UsesCallSynonymAnyNotInClause = Query("c", ArgumentTypes::CALL, QuerySuchThatClause(RelRef::USES_S, "a", "_", ArgumentTypes::ASSIGN, ArgumentTypes::ANY));

// select n such that Uses(n, "a"), return statements/prog_line that uses variable a
Query UsesProgLineInClause = Query("n", ArgumentTypes::PROG_LINE, QuerySuchThatClause(RelRef::USES_S, "n", "a", ArgumentTypes::PROG_LINE, ArgumentTypes::STRING));

// select n such that Uses("a", "a"), return statements/prog_line 
Query UsesProgLineNotInClause = Query("n", ArgumentTypes::PROG_LINE, QuerySuchThatClause(RelRef::USES_S, "a", "a", ArgumentTypes::ASSIGN, ArgumentTypes::VARIABLE));

/*
ModifiesP
*/
// select p such that Uses(p, v), return procedures that uses variable
Query UsesProcVarInClause = Query("p", ArgumentTypes::PROCEDURE, QuerySuchThatClause(RelRef::USES_P, "p", "v", ArgumentTypes::PROCEDURE, ArgumentTypes::VARIABLE));

// select p such that Uses(a, v), return procedures that uses variable
Query UsesProcVarNotInClause = Query("p", ArgumentTypes::PROCEDURE, QuerySuchThatClause(RelRef::USES_S, "a", "v", ArgumentTypes::ASSIGN, ArgumentTypes::VARIABLE));

// select p such that Uses(p, "a"), return all procedures that uses variable "a"
Query UsesProcStrInClause = Query("p", ArgumentTypes::PROCEDURE, QuerySuchThatClause(RelRef::USES_P, "p", "a", ArgumentTypes::PROCEDURE, ArgumentTypes::STRING));

// select p such that Uses(a, "a"), return all procedures 
Query UsesProcStrNotInClause = Query("p", ArgumentTypes::PROCEDURE, QuerySuchThatClause(RelRef::USES_S, "a", "a", ArgumentTypes::ASSIGN, ArgumentTypes::STRING));

// select p such that Uses(p, _), return all procedures using
Query UsesProcAnyInClause = Query("p", ArgumentTypes::PROCEDURE, QuerySuchThatClause(RelRef::USES_P, "p", "_", ArgumentTypes::PROCEDURE, ArgumentTypes::ANY));

// select p such that Uses(a, _), return all procedures
Query UsesProcAnyNotInClause = Query("p", ArgumentTypes::PROCEDURE, QuerySuchThatClause(RelRef::USES_S, "a", "a", ArgumentTypes::ASSIGN, ArgumentTypes::ANY));

// select p such that Uses("testA", v)
Query UsesStrVarInClauseTrue = Query("p", ArgumentTypes::PROCEDURE, QuerySuchThatClause(RelRef::USES_P, "testA", "v", ArgumentTypes::STRING, ArgumentTypes::VARIABLE));

// select p such that Uses("testC", v)
Query UsesStrVarInClauseFalse = Query("p", ArgumentTypes::PROCEDURE, QuerySuchThatClause(RelRef::USES_P, "testC", "v", ArgumentTypes::STRING, ArgumentTypes::VARIABLE));

// select p such that Uses("testB", "a")
Query UsesStrStrInClauseTrue = Query("p", ArgumentTypes::PROCEDURE, QuerySuchThatClause(RelRef::USES_P, "testB", "a", ArgumentTypes::STRING, ArgumentTypes::STRING));

// select p such that Uses("testC", "a")
Query UsesStrStrInClauseFalse = Query("p", ArgumentTypes::PROCEDURE, QuerySuchThatClause(RelRef::USES_P, "testC", "a", ArgumentTypes::STRING, ArgumentTypes::STRING));

// select p such that Uses("testA", _)
Query UsesStrAnyInClauseTrue = Query("p", ArgumentTypes::PROCEDURE, QuerySuchThatClause(RelRef::USES_P, "testA", "_", ArgumentTypes::STRING, ArgumentTypes::ANY));

// select p such that Uses("testC", _)
Query UsesStrAnyInClauseFalse = Query("p", ArgumentTypes::PROCEDURE, QuerySuchThatClause(RelRef::USES_P, "testC", "_", ArgumentTypes::STRING, ArgumentTypes::ANY));
