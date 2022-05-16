#pragma once
#include "QueryEvaluator.h"

/*
procedure testM {
	1 p = 4;
	2 a = p * 2;
	3 if (some condition) {
	4	pattern = p * a * a + ( p * w + p * ifs);
	  }
	5 while(some cond);
	}
	6 read r;
	7 if(some cond) {
	8	while(some cond) {
	9		a = a - 1;
	10		read b;
	11		ifs = ifs - 1;
		 }
		}
	12 a = 10;
	13 print a;
	14 if(some cond) {
	15	q = p - a;
	16	a = 10;
	17 call testM2;
    }
}

procedure testM2 {
	18 a = 2; 
	19 call testM3;
}

procedure testM3 {
	20 print p;
}
*/

std::unordered_set<std::string> allProgStmt = { "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "20"};
std::unordered_set<std::string> modifyingStmtList = { "1", "2", "3", "4", "6", "7", "8", "9", "10", "11", "12", "14", "15", "16", "17", "18" };
std::unordered_set<std::string> allModifiedVarList = { "p", "a", "pattern", "r", "b", "ifs", "q" };
std::unordered_set<std::string> ifModifyingList = { "3", "7", "14" };
std::unordered_set<std::string> whileModifyingList = { "8" };
std::unordered_set<std::string> readModifyingList = { "6", "10" };
std::unordered_set<std::string> callModifyingStmtList = { "17" };

//Any, any
//select s such that Modifies(_,_) -> returns all stmtNo as long as there is some modifies relationship
Query ModifiesAnyAny = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::MODIFIES_S, "_", "_", ArgumentTypes::ANY, ArgumentTypes::ANY));

//stmt, any
//select s such that Modifies(s, _) -> return all modifying stmt
Query ModifiesStmtAnyInClause = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::MODIFIES_S, "s", "_", ArgumentTypes::STMT, ArgumentTypes::ANY));
//select s such that Modifies(s1, _) -> return all stmts
Query ModifiesStmtAnyNotInClause = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::MODIFIES_S, "s1", "_", ArgumentTypes::STMT, ArgumentTypes::ANY));

//any, var
//select v such that Modifies(_, v) -> returns all modified variables 
Query ModifiesAnyVarsInClause = Query("v", ArgumentTypes::VARIABLE, QuerySuchThatClause(RelRef::MODIFIES_S, "_", "v", ArgumentTypes::ANY, ArgumentTypes::VARIABLE));

//select s such that Modifies(_, v1)
Query ModifiesAnyVarsNotInClause = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::MODIFIES_S, "_", "v", ArgumentTypes::ANY, ArgumentTypes::VARIABLE));

//stmt, var
//select s such that Modifies(s, v) -> returns all stmt that modifies any variable 
Query ModifiesStmtVarInClause = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::MODIFIES_S, "s", "v", ArgumentTypes::STMT, ArgumentTypes::VARIABLE));

//select v such that Modifies(s1, v) -> returns all var that are modified by any stmt 
Query ModifiesStmtVarNotInClause = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::MODIFIES_S, "s1", "v", ArgumentTypes::STMT, ArgumentTypes::VARIABLE));

//stmt, string
//select s such that Modifies(s, "a") -> return all stmtNo that modifies var "a"
Query ModifiesStmtStrInClause = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::MODIFIES_S, "s", "a", ArgumentTypes::STMT, ArgumentTypes::STRING));

//select s such that Modifies(s1, "a") 
Query ModifiesStmtStrNotInClause = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::MODIFIES_S, "s1", "a", ArgumentTypes::STMT, ArgumentTypes::STRING));


//any, string
//select s such that Modifies(_, "a") -> return all stmtNo as long as "a" is modified by any stmt
//evaluate to true
Query ModifiesAnyStrTrue = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::MODIFIES_S, "_", "a", ArgumentTypes::ANY, ArgumentTypes::STRING));

//select s such that Modifies(_, "c") -> return all stmtNo as long as "v" is modified by any stmt
//evaluate to false -> no stmt returned
Query ModifiesAnyStrFalse = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::MODIFIES_S, "_", "c", ArgumentTypes::ANY, ArgumentTypes::STRING));

//int, any
//select s such that Modifies(1, _) -> returns prog stmtList if true
Query ModifiesIntAny_True = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::MODIFIES_S, "1", "_", ArgumentTypes::INT, ArgumentTypes::ANY));

//select s such that Modifies(5, _)
Query ModifiesIntAny_False = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::MODIFIES_S, "5", "_", ArgumentTypes::INT, ArgumentTypes::ANY));

//int, var
//select v such that Modifies(1, v) -> return all vars modified in stmt 1
Query ModifiesIntVarInClause = Query("v", ArgumentTypes::VARIABLE, QuerySuchThatClause(RelRef::MODIFIES_S, "1", "v", ArgumentTypes::INT, ArgumentTypes::VARIABLE));

//select s such that Modifies(1, v) -> all stmts as long as stmt 1 modifies some var
Query ModifiesIntVarNotInClause = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::MODIFIES_S, "1", "v", ArgumentTypes::INT, ArgumentTypes::VARIABLE));

//int, string
//select s such that Modifies(1, "p")  returns all prog stmts if modifies evaluate true
Query ModifiesIntString_True = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::MODIFIES_S, "1", "p", ArgumentTypes::INT, ArgumentTypes::STRING));

//select s such that Modifies(1, "a") 
Query ModifiesIntString_False = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::MODIFIES_S, "1", "a", ArgumentTypes::INT, ArgumentTypes::STRING));
Query ModifiesIntString_False1 = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::MODIFIES_S, "20", "a", ArgumentTypes::INT, ArgumentTypes::STRING));
Query ModifiesIntString_False2 = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::MODIFIES_S, "0", "a", ArgumentTypes::INT, ArgumentTypes::STRING));

//syn, any
//select ifs such that Modifies(ifs,_)
Query ModifiesIfAnyInClause = Query("ifs", ArgumentTypes::IF, QuerySuchThatClause(RelRef::MODIFIES_S, "ifs", "_", ArgumentTypes::IF, ArgumentTypes::ANY));

//select s such that Modifies(ifs, _)
Query ModifiesIfAnyNotInClause = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::MODIFIES_S, "ifs", "_", ArgumentTypes::IF, ArgumentTypes::ANY));

//select w such that Modifies(w, _)
Query ModifiesWhileAnyInClause = Query("w", ArgumentTypes::WHILE, QuerySuchThatClause(RelRef::MODIFIES_S, "w", "_", ArgumentTypes::WHILE, ArgumentTypes::ANY));

//select s such that Modifies(w, )
Query ModifiesWhileAnyNotInClause = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::MODIFIES_S, "w", "_", ArgumentTypes::WHILE, ArgumentTypes::ANY));

//select r such that Modifies(r,_)
Query ModifiesReadAnyInClause = Query("r", ArgumentTypes::READ, QuerySuchThatClause(RelRef::MODIFIES_S, "r", "_", ArgumentTypes::READ, ArgumentTypes::ANY));

//select s such that Modifies(r,_)
Query ModifiesReadAnyNotInClause = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::MODIFIES_S, "r", "_", ArgumentTypes::READ, ArgumentTypes::ANY));


//syn, var
//select ifs such that Modifies(ifs, v)
Query ModifiesIfVarInClause = Query("ifs", ArgumentTypes::IF, QuerySuchThatClause(RelRef::MODIFIES_S, "ifs", "v", ArgumentTypes::IF, ArgumentTypes::VARIABLE));

//select s such that Modifies(ifs, v)
Query ModifiesIfVarNotInClause = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::MODIFIES_S, "ifs", "v", ArgumentTypes::IF, ArgumentTypes::VARIABLE));

//select w such that Modifies(w, v)
Query ModifiesWhileVarInClause = Query("w", ArgumentTypes::WHILE, QuerySuchThatClause(RelRef::MODIFIES_S, "w", "v", ArgumentTypes::WHILE, ArgumentTypes::VARIABLE));

//select s such that Modifies(w, v)
Query ModifiesWhileVarNotInClause = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::MODIFIES_S, "w", "v", ArgumentTypes::WHILE, ArgumentTypes::VARIABLE));

//select r such that Modifies(r,v)
Query ModifiesReadVarInClause = Query("r", ArgumentTypes::READ, QuerySuchThatClause(RelRef::MODIFIES_S, "r", "v", ArgumentTypes::READ, ArgumentTypes::VARIABLE));

//select s such that Modifies(r,v)
Query ModifiesReadVarNotInClause = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::MODIFIES_S, "r", "v", ArgumentTypes::READ, ArgumentTypes::VARIABLE));

//syn string
//select ifs such that Modifies(ifs, "a")
Query ModifiesIfStrInClause = Query("ifs", ArgumentTypes::IF, QuerySuchThatClause(RelRef::MODIFIES_S, "ifs", "a", ArgumentTypes::IF, ArgumentTypes::STRING));

//select s such that Modifies(ifs, "a")
Query ModifiesIfStrNotInClause = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::MODIFIES_S, "ifs", "a", ArgumentTypes::IF, ArgumentTypes::STRING));

//select w such that Modifies(w, "a")
Query ModifiesWhileStrInClause = Query("w", ArgumentTypes::WHILE, QuerySuchThatClause(RelRef::MODIFIES_S, "w", "a", ArgumentTypes::WHILE, ArgumentTypes::STRING));

//select s such that Modifies(w, "a")
Query ModifiesWhileStrNotInClause = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::MODIFIES_S, "w", "a", ArgumentTypes::WHILE, ArgumentTypes::STRING));

//select r such that Modifies(r, "r")
Query ModifiesReadStrInClause = Query("r", ArgumentTypes::READ, QuerySuchThatClause(RelRef::MODIFIES_S, "r", "r", ArgumentTypes::READ, ArgumentTypes::STRING));

//select s such that Modifies(r,"r")
Query ModifiesReadStrNotInClause = Query("s", ArgumentTypes::STMT, QuerySuchThatClause(RelRef::MODIFIES_S, "r", "r", ArgumentTypes::READ, ArgumentTypes::STRING));

// ITERATION 1 BUG REPORT ADDITIONS
//select w such that Modifies(a, "p"), return all while statements
Query WhileSynonymNotInClause = Query("w", ArgumentTypes::WHILE, QuerySuchThatClause(RelRef::MODIFIES_S, "a", "pattern", ArgumentTypes::ASSIGN, ArgumentTypes::STRING));

Query WhileSynonymNotInClauseVarNameDoesNotExist = Query("w", ArgumentTypes::WHILE, QuerySuchThatClause(RelRef::MODIFIES_S, "a", "cs3203", ArgumentTypes::ASSIGN, ArgumentTypes::STRING));

//select a such that Modifies(a, "p"), return specific assign statements
Query AssignSynonymInClause = Query("a", ArgumentTypes::ASSIGN, QuerySuchThatClause(RelRef::MODIFIES_S, "a", "a", ArgumentTypes::ASSIGN, ArgumentTypes::STRING));

Query AssignSynonymInClauseVarNameDoesNotExist = Query("a", ArgumentTypes::ASSIGN, QuerySuchThatClause(RelRef::MODIFIES_S, "a", "SPA", ArgumentTypes::ASSIGN, ArgumentTypes::STRING));

//ModifiesS testing call and prog_line
// select c such that Modifies(c, v), return specific call statement
Query CallSynonymInClause = Query("c", ArgumentTypes::CALL, QuerySuchThatClause(RelRef::MODIFIES_S, "c", "v", ArgumentTypes::CALL, ArgumentTypes::VARIABLE));

// select c such that Modifies(a, v), return all call statement
Query CallSynonymNotInClause = Query("c", ArgumentTypes::CALL, QuerySuchThatClause(RelRef::MODIFIES_S, "a", "v", ArgumentTypes::ASSIGN, ArgumentTypes::VARIABLE));

// select c such that Modifies(c, "a"), return call statements that modify variable "a"
Query CallSynonymStringInClause = Query("c", ArgumentTypes::CALL, QuerySuchThatClause(RelRef::MODIFIES_S, "c", "a", ArgumentTypes::CALL, ArgumentTypes::STRING));

// select c such that Modifies(c, "p"), return nothing
Query CallSynonymStringInClause2 = Query("c", ArgumentTypes::CALL, QuerySuchThatClause(RelRef::MODIFIES_S, "c", "p", ArgumentTypes::CALL, ArgumentTypes::STRING));

// select c such that Modifies(c, _), return call statements that modifies something
Query CallSynonymAnyInClause = Query("c", ArgumentTypes::CALL, QuerySuchThatClause(RelRef::MODIFIES_S, "c", "_", ArgumentTypes::CALL, ArgumentTypes::ANY));

// select c such that Modifies(a, "_"), return all call statements 
Query CallSynonymAnyNotInClause = Query("c", ArgumentTypes::CALL, QuerySuchThatClause(RelRef::MODIFIES_S, "a", "_", ArgumentTypes::ASSIGN, ArgumentTypes::ANY));

// select n such that Modifies(n, "a"), return statements/prog_line that modifies variable a
Query ProgLineInClause = Query("n", ArgumentTypes::PROG_LINE, QuerySuchThatClause(RelRef::MODIFIES_S, "n", "a", ArgumentTypes::PROG_LINE, ArgumentTypes::STRING));

// select n such that Modifies("a", "a"), return statements/prog_line 
Query ProgLineNotInClause = Query("n", ArgumentTypes::PROG_LINE, QuerySuchThatClause(RelRef::MODIFIES_S, "a", "a", ArgumentTypes::ASSIGN, ArgumentTypes::VARIABLE));

/*
ModifiesP
*/

// select p such that Modifies(p, v), return procedures that modifies variable
Query ProcVarInClause = Query("p", ArgumentTypes::PROCEDURE, QuerySuchThatClause(RelRef::MODIFIES_P, "p", "v", ArgumentTypes::PROCEDURE, ArgumentTypes::VARIABLE));

// select p such that Modifies(a, v), return procedures that modifies variable
Query ProcVarNotInClause = Query("p", ArgumentTypes::PROCEDURE, QuerySuchThatClause(RelRef::MODIFIES_S, "a", "v", ArgumentTypes::ASSIGN, ArgumentTypes::VARIABLE));

// select p such that Modifies(p, "a"), return all procedures that modfies variable "a"
Query ProcStrInClause = Query("p", ArgumentTypes::PROCEDURE, QuerySuchThatClause(RelRef::MODIFIES_P, "p", "a", ArgumentTypes::PROCEDURE, ArgumentTypes::STRING));

// select p such that Modifies(a, "a"), return all procedures 
Query ProcStrNotInClause = Query("p", ArgumentTypes::PROCEDURE, QuerySuchThatClause(RelRef::MODIFIES_S, "a", "a", ArgumentTypes::ASSIGN, ArgumentTypes::STRING));

// select p such that Modifies(p, _), return all procedures modifying
Query ProcAnyInClause = Query("p", ArgumentTypes::PROCEDURE, QuerySuchThatClause(RelRef::MODIFIES_P, "p", "_", ArgumentTypes::PROCEDURE, ArgumentTypes::ANY));

// select p such that Modifies(a, _), return all procedures
Query ProcAnyNotInClause = Query("p", ArgumentTypes::PROCEDURE, QuerySuchThatClause(RelRef::MODIFIES_S, "p", "a", ArgumentTypes::ASSIGN, ArgumentTypes::ANY));

// select p such that Modifies("testM", v)
Query StrVarInClauseTrue = Query("p", ArgumentTypes::PROCEDURE, QuerySuchThatClause(RelRef::MODIFIES_P, "testM", "v", ArgumentTypes::STRING, ArgumentTypes::VARIABLE));

// select p such that Modifies("testM3", v)
Query StrVarInClauseFalse = Query("p", ArgumentTypes::PROCEDURE, QuerySuchThatClause(RelRef::MODIFIES_P, "testM3", "v", ArgumentTypes::STRING, ArgumentTypes::VARIABLE));

// select p such that Modifies("testM", "ifs")
Query StrStrInClauseTrue = Query("p", ArgumentTypes::PROCEDURE, QuerySuchThatClause(RelRef::MODIFIES_P, "testM", "ifs", ArgumentTypes::STRING, ArgumentTypes::STRING));

// select p such that Modifies("testM3", "ifs")
Query StrStrInClauseFalse = Query("p", ArgumentTypes::PROCEDURE, QuerySuchThatClause(RelRef::MODIFIES_P, "testM3", "ifs", ArgumentTypes::STRING, ArgumentTypes::STRING));

// select p such that Modifies("testM", _)
Query StrAnyInClauseTrue = Query("p", ArgumentTypes::PROCEDURE, QuerySuchThatClause(RelRef::MODIFIES_P, "testM", "_", ArgumentTypes::STRING, ArgumentTypes::ANY));

// select p such that Modifies("testM3", _)
Query StrAnyInClauseFalse = Query("p", ArgumentTypes::PROCEDURE, QuerySuchThatClause(RelRef::MODIFIES_P, "testM3", "_", ArgumentTypes::STRING, ArgumentTypes::ANY));
