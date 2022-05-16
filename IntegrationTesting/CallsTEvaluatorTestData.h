#pragma once

#include "QueryEvaluator.h"

/*
procedure testA {
1    p = 4;
2    call testB;
}

procedure testB {
3	  a = 10;
4	  if (a > 5)
	  then {
5		call testC;
6		print f;
	  }
	  else {
7	    call testC;
8		read q;
	  }
}

procedure testC {
9	  a = 5;
}
*/

std::unordered_set<std::string> callsTAllStmts = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };
std::unordered_set<std::string> callsTAllProc = { "testA", "testB", "testC" };
std::unordered_set<std::string> callsTAllCallStmt = { "2", "5", "7" };

// PROC PROC
// select p such that CallsT(p, p2), return all caller procedures
Query CallsTProcProcInClause = Query("p", ArgumentTypes::PROCEDURE, QuerySuchThatClause(RelRef::CALLS_T, "p", "p2", ArgumentTypes::PROCEDURE, ArgumentTypes::PROCEDURE));

// select p such that CallsT(p1, p2), return all procedures
Query CallsTProcProcNotInClause = Query("p", ArgumentTypes::PROCEDURE, QuerySuchThatClause(RelRef::CALLS_T, "p1", "p2", ArgumentTypes::PROCEDURE, ArgumentTypes::PROCEDURE));

// PROC STRING
// select p such that CallsT(p, "testC"), return all callerT of testC
Query CallsTProcStringInClauseTrue = Query("p", ArgumentTypes::PROCEDURE, QuerySuchThatClause(RelRef::CALLS_T, "p", "testC", ArgumentTypes::PROCEDURE, ArgumentTypes::STRING));

// select p such that CallsT(p, "testD"), return nothing
Query CallsTProcStringInClauseFalse = Query("p", ArgumentTypes::PROCEDURE, QuerySuchThatClause(RelRef::CALLS_T, "p", "testD", ArgumentTypes::PROCEDURE, ArgumentTypes::STRING));

// select p such that CallsT(p1, "testC"), return all procedures
Query CallsTProcStringNotInClauseTrue = Query("p", ArgumentTypes::PROCEDURE, QuerySuchThatClause(RelRef::CALLS_T, "p1", "testC", ArgumentTypes::PROCEDURE, ArgumentTypes::STRING));

// select p such that CallsT(p1, "testD"), return nothing
Query CallsTProcStringNotInClauseFalse = Query("p", ArgumentTypes::PROCEDURE, QuerySuchThatClause(RelRef::CALLS_T, "p1", "testD", ArgumentTypes::PROCEDURE, ArgumentTypes::STRING));

// PROC ANY
// select p such that CallsT(p, _), return all caller procedures
Query CallsTProcAnyInClause = Query("p", ArgumentTypes::PROCEDURE, QuerySuchThatClause(RelRef::CALLS_T, "p", "_", ArgumentTypes::PROCEDURE, ArgumentTypes::ANY));

// select p such that CallsT(p1, _), return all procedures
Query CallsTProcAnyNotInClause = Query("p", ArgumentTypes::PROCEDURE, QuerySuchThatClause(RelRef::CALLS_T, "p1", "_", ArgumentTypes::PROCEDURE, ArgumentTypes::ANY));

// STRING PROC
// select p such that CallsT("testA", p), return all calleeT procedure of testA
Query CallsTStringProcInClauseTrue = Query("p", ArgumentTypes::PROCEDURE, QuerySuchThatClause(RelRef::CALLS_T, "testA", "p", ArgumentTypes::STRING, ArgumentTypes::PROCEDURE));

// select p such that CallsT("testD", ), return nothing
Query CallsTStringProcInClauseFalse = Query("p", ArgumentTypes::PROCEDURE, QuerySuchThatClause(RelRef::CALLS_T, "testD", "p", ArgumentTypes::STRING, ArgumentTypes::PROCEDURE));

// select p such that CallsT("testA", p1), return all procedures
Query CallsTStringProcNotInClauseTrue = Query("p", ArgumentTypes::PROCEDURE, QuerySuchThatClause(RelRef::CALLS_T, "testA", "p1", ArgumentTypes::STRING, ArgumentTypes::PROCEDURE));

// select p such that CallsT("testD", p1), return nothing
Query CallsTStringProcNotInClauseFalse = Query("p", ArgumentTypes::PROCEDURE, QuerySuchThatClause(RelRef::CALLS_T, "testD", "p1", ArgumentTypes::STRING, ArgumentTypes::PROCEDURE));

// STRING STRING
// select p such that CallsT("testA", "testC"), return all procedures
Query CallsTStringStringTrue = Query("p", ArgumentTypes::PROCEDURE, QuerySuchThatClause(RelRef::CALLS_T, "testA", "testC", ArgumentTypes::STRING, ArgumentTypes::STRING));

// select p such that CallsT("testA", "testD"), return nothing
Query CallsTStringStringFalse = Query("p", ArgumentTypes::PROCEDURE, QuerySuchThatClause(RelRef::CALLS_T, "testA", "testD", ArgumentTypes::STRING, ArgumentTypes::STRING));

// STRING ANY
// select p such that CallsT("testA", _), return all procedures
Query CallsTStringAnyTrue = Query("p", ArgumentTypes::PROCEDURE, QuerySuchThatClause(RelRef::CALLS_T, "testA", "_", ArgumentTypes::STRING, ArgumentTypes::ANY));

// select p such that CallsT("testC", _), return nothing
Query CallsTStringAnyFalse = Query("p", ArgumentTypes::PROCEDURE, QuerySuchThatClause(RelRef::CALLS_T, "testC", "_", ArgumentTypes::STRING, ArgumentTypes::ANY));

// ANY PROC
// select p such that CallsT(_, p), return all callee procedures
Query CallsTAnyProcInClause = Query("p", ArgumentTypes::PROCEDURE, QuerySuchThatClause(RelRef::CALLS_T, "_", "p", ArgumentTypes::ANY, ArgumentTypes::PROCEDURE));

// select p such that CallsT(_, p1), return all procedures
Query CallsTAnyProcNotInClause = Query("p", ArgumentTypes::PROCEDURE, QuerySuchThatClause(RelRef::CALLS_T, "_", "p1", ArgumentTypes::ANY, ArgumentTypes::PROCEDURE));

// ANY STRING
// select p such that CallsT(_, "testB"), return all procedures
Query CallsTAnyStringTrue = Query("p", ArgumentTypes::PROCEDURE, QuerySuchThatClause(RelRef::CALLS_T, "_", "testB", ArgumentTypes::ANY, ArgumentTypes::STRING));

// select p such that CallsT(_, "testA"), return nothing
Query CallsTAnyStringFalse = Query("p", ArgumentTypes::PROCEDURE, QuerySuchThatClause(RelRef::CALLS_T, "_", "testA", ArgumentTypes::ANY, ArgumentTypes::STRING));

// ANY ANY
// select p such that CallsT(_, _), return all procedures
Query CallsTAnyAnyTrue = Query("p", ArgumentTypes::PROCEDURE, QuerySuchThatClause(RelRef::CALLS_T, "_", "_", ArgumentTypes::ANY, ArgumentTypes::ANY));

// Edge Cases
// select p such that CallsT("testA", "testA"), return nothing
Query CallsTEdgeCase1 = Query("p", ArgumentTypes::PROCEDURE, QuerySuchThatClause(RelRef::CALLS_T, "testA", "testA", ArgumentTypes::STRING, ArgumentTypes::STRING));

// select p such that CallsT(p, p), return nothing
Query CallsTEdgeCase2 = Query("p", ArgumentTypes::PROCEDURE, QuerySuchThatClause(RelRef::CALLS_T, "p", "p", ArgumentTypes::PROCEDURE, ArgumentTypes::PROCEDURE));
