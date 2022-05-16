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

std::unordered_set<std::string> callsAllStmts = { "1", "2", "3", "4", "5", "6", "7", "8", "9"};
std::unordered_set<std::string> callsAllProc = { "testA", "testB", "testC" };
std::unordered_set<std::string> callsAllCallStmt = { "2", "5", "7" };

// PROC PROC
// select p such that Calls(p, p2), return all caller procedures
Query CallsProcProcInClause = Query("p", ArgumentTypes::PROCEDURE, QuerySuchThatClause(RelRef::CALLS, "p", "p2", ArgumentTypes::PROCEDURE, ArgumentTypes::PROCEDURE));

// select p such that Calls(p1, p2), return all procedures
Query CallsProcProcNotInClause = Query("p", ArgumentTypes::PROCEDURE, QuerySuchThatClause(RelRef::CALLS, "p1", "p2", ArgumentTypes::PROCEDURE, ArgumentTypes::PROCEDURE));

// PROC STRING
// select p such that Calls(p, "testC"), return all caller of testC
Query CallsProcStringInClauseTrue = Query("p", ArgumentTypes::PROCEDURE, QuerySuchThatClause(RelRef::CALLS, "p", "testC", ArgumentTypes::PROCEDURE, ArgumentTypes::STRING));

// select p such that Calls(p, "testD"), return nothing
Query CallsProcStringInClauseFalse = Query("p", ArgumentTypes::PROCEDURE, QuerySuchThatClause(RelRef::CALLS, "p", "testD", ArgumentTypes::PROCEDURE, ArgumentTypes::STRING));

// select p such that Calls(p1, "testC"), return all procedures
Query CallsProcStringNotInClauseTrue = Query("p", ArgumentTypes::PROCEDURE, QuerySuchThatClause(RelRef::CALLS, "p1", "testC", ArgumentTypes::PROCEDURE, ArgumentTypes::STRING));

// select p such that Calls(p1, "testD"), return nothing
Query CallsProcStringNotInClauseFalse = Query("p", ArgumentTypes::PROCEDURE, QuerySuchThatClause(RelRef::CALLS, "p1", "testD", ArgumentTypes::PROCEDURE, ArgumentTypes::STRING));

// PROC ANY
// select p such that Calls(p, _), return all caller procedures
Query CallsProcAnyInClause = Query("p", ArgumentTypes::PROCEDURE, QuerySuchThatClause(RelRef::CALLS, "p", "_", ArgumentTypes::PROCEDURE, ArgumentTypes::ANY));

// select p such that Calls(p1, _), return all procedures
Query CallsProcAnyNotInClause = Query("p", ArgumentTypes::PROCEDURE, QuerySuchThatClause(RelRef::CALLS, "p1", "_", ArgumentTypes::PROCEDURE, ArgumentTypes::ANY));

// STRING PROC
// select p such that Calls("testA", p), return all callee procedure of testA
Query CallsStringProcInClauseTrue = Query("p", ArgumentTypes::PROCEDURE, QuerySuchThatClause(RelRef::CALLS, "testA", "p", ArgumentTypes::STRING, ArgumentTypes::PROCEDURE));

// select p such that Calls("testD", ), return nothing
Query CallsStringProcInClauseFalse = Query("p", ArgumentTypes::PROCEDURE, QuerySuchThatClause(RelRef::CALLS, "testD", "p", ArgumentTypes::STRING, ArgumentTypes::PROCEDURE));

// select p such that Calls("testA", p1), return all procedures
Query CallsStringProcNotInClauseTrue = Query("p", ArgumentTypes::PROCEDURE, QuerySuchThatClause(RelRef::CALLS, "testA", "p1", ArgumentTypes::STRING, ArgumentTypes::PROCEDURE));

// select p such that Calls("testD", p1), return nothing
Query CallsStringProcNotInClauseFalse = Query("p", ArgumentTypes::PROCEDURE, QuerySuchThatClause(RelRef::CALLS, "testD", "p1", ArgumentTypes::STRING, ArgumentTypes::PROCEDURE));

// STRING STRING
// select p such that Calls("testA", "testB"), return all procedures
Query CallsStringStringTrue = Query("p", ArgumentTypes::PROCEDURE, QuerySuchThatClause(RelRef::CALLS, "testA", "testB", ArgumentTypes::STRING, ArgumentTypes::STRING));

// select p such that Calls("testA", "testC"), return nothing
Query CallsStringStringFalse = Query("p", ArgumentTypes::PROCEDURE, QuerySuchThatClause(RelRef::CALLS, "testA", "testC", ArgumentTypes::STRING, ArgumentTypes::STRING));

// STRING ANY
// select p such that Calls("testA", _), return all procedures
Query CallsStringAnyTrue = Query("p", ArgumentTypes::PROCEDURE, QuerySuchThatClause(RelRef::CALLS, "testA", "_", ArgumentTypes::STRING, ArgumentTypes::ANY));

// select p such that Calls("testC", _), return nothing
Query CallsStringAnyFalse = Query("p", ArgumentTypes::PROCEDURE, QuerySuchThatClause(RelRef::CALLS, "testC", "_", ArgumentTypes::STRING, ArgumentTypes::ANY));

// ANY PROC
// select p such that Calls(_, p), return all callee procedures
Query CallsAnyProcInClause = Query("p", ArgumentTypes::PROCEDURE, QuerySuchThatClause(RelRef::CALLS, "_", "p", ArgumentTypes::ANY, ArgumentTypes::PROCEDURE));

// select p such that Calls(_, p1), return all procedures
Query CallsAnyProcNotInClause = Query("p", ArgumentTypes::PROCEDURE, QuerySuchThatClause(RelRef::CALLS, "_", "p1", ArgumentTypes::ANY, ArgumentTypes::PROCEDURE));

// ANY STRING
// select p such that Calls(_, "testB"), return all procedures
Query CallsAnyStringTrue = Query("p", ArgumentTypes::PROCEDURE, QuerySuchThatClause(RelRef::CALLS, "_", "testB", ArgumentTypes::ANY, ArgumentTypes::STRING));

// select p such that Calls(_, "testA"), return nothing
Query CallsAnyStringFalse = Query("p", ArgumentTypes::PROCEDURE, QuerySuchThatClause(RelRef::CALLS, "_", "testA", ArgumentTypes::ANY, ArgumentTypes::STRING));

// ANY ANY
// select p such that Calls(_, _), return all procedures
Query CallsAnyAnyTrue = Query("p", ArgumentTypes::PROCEDURE, QuerySuchThatClause(RelRef::CALLS, "_", "_", ArgumentTypes::ANY, ArgumentTypes::ANY));

// Edge Cases
// select p such that Calls("testA", "testA"), return nothing
Query CallsEdgeCase1 = Query("p", ArgumentTypes::PROCEDURE, QuerySuchThatClause(RelRef::CALLS, "testA", "testA", ArgumentTypes::STRING, ArgumentTypes::STRING));

// select p such that Calls(p, p), return nothing
Query CallsEdgeCase2 = Query("p", ArgumentTypes::PROCEDURE, QuerySuchThatClause(RelRef::CALLS, "p", "p", ArgumentTypes::PROCEDURE, ArgumentTypes::PROCEDURE));
