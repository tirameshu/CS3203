#pragma once

#include "QueryParser.h"

// VALID QUERIES
// only select-cl
std::string onlySelect = "stmt s; Select s";
std::string onlySelect_multipleDeclarations = "stmt s; stmt s1; Select s1";
std::string onlySelect_multipleDeclarationTypes = "stmt s; assign a; print pn; if ifs; read re; Select pn";
std::string selectProcedure = "procedure p; Select p";
std::string selectConstant = "constant c; Select c";
std::string selectCall = "call c; Select c";
std::string selectProgLine = "prog_line n; Select n";
std::string extraWhitespace = "stmt   abc   ;    Select    abc  ";
std::string clauseNamedSelect = "stmt Select; Select Select";
std::string declarationWhitespace = "stmt a,b; while c,   d; Select b";
std::string multipleDeclarationsSameType = "stmt s1, s2; stmt s3, s4, s5; Select s4";
std::string multipleDeclarationsCapitalize = "stmt s, S; Select S";

// only one pattern-cl
std::string patternExtraWhitespace = "assign a; constant c; Select c pattern a  (_, _\"   answer  \"_)";
std::string patternLeftWhitespace = "assign a; constant c; Select c pattern a  (   _   , _\"answer\"_)";
std::string selectPatternOnly = "variable v; assign a; Select v pattern a(v, _)";
std::string patternUnderscore = "assign a; Select a pattern a(_, _\"answer\"_)";
std::string patternUnderscoreWhitespace = "assign a; Select a pattern a(_, _   \"   answer  \"   _)";
std::string patternIdentExprExact = "assign a; Select a pattern a(\"x\", \"answer\")";
std::string patternPostfixExact = "assign a; Select a pattern a(_, \"bc + ad\")";
std::string patternPostfix = "assign a; Select a pattern a(_, _\"bc + ad\"_)";

// only one suchthat-cl
std::string selectModifies = "variable v; Select v such that Modifies(  3, v)";
std::string selectUses = "if ifs; Select ifs such that Uses(ifs, \"x\")";
std::string selectUsesInt = "stmt s; variable v; Select s such that Uses(5, v)";
std::string selectParent = "if ifs; stmt s; Select s such that Parent(ifs, s)";
std::string selectParentT = "assign a; Select a such that Parent*(5, a)";
std::string selectFollows = "stmt s; Select s such that Follows(s, 10)";
std::string selectFollowsT = "while w; stmt s; Select w such that Follows*(w, s)";
std::string selectNext = "call c; Select c such that Next(4, c)";
std::string selectNextT = "print pn; Select pn such that Next*(pn, 4)";
std::string selectCalls = "procedure p; Select p such that Calls(p, \"q\")";
std::string selectCallsT = "procedure p; Select p such that Calls*(\"Example\", p)";
std::string selectAffects = "assign a; prog_line n; Select a such that Affects(a, n)";
std::string selectAffectsT = "assign a; prog_line n; Select a such that Affects*(a, n)";
std::string modifiesRead = "read r; variable v; Select r such that Modifies(r, v)";
std::string usesPrint = "print pn; variable v; Select pn such that Uses(pn, v)";

// with-cl
std::string withProcProcName = "procedure p; Select p with p.procName = \"procedure\"";
std::string withCallProcName = "procedure p; call c; Select p with p.procName = c.procName";
std::string withVarProcName = "variable v; Select v.varName";
std::string withConstantValue = "constant c; stmt s; Select s with c.value = 5";
std::string withStmtNumber = "stmt s; Select BOOLEAN with s.stmt# = 20";
std::string withReadProcName = "read r; variable v; Select v with r.varName = v.varName";
std::string withPrintProcName = "print pn; Select pn with pn.varName = \"a\"";

std::string withVar = "variable v; procedure p; Select p such that Uses(p, v) with v.varName = \"variable\"";
std::string withModifies = "read r; variable v; Select r.stmt# such that Modifies(r, v) with v.varName = \"x\""; 

// select attrName
std::string selectProcName = "procedure p, q; Select p.procName such that Calls (p, q)";

// and-cl
std::string andSuchThat = "assign a; while w; Select a such that Modifies (a, \"x\") and Parent* (w, a) and Next* (1, a)";
std::string andPattern = "variable v; if ifs; assign a; while w; Select v pattern ifs (v, _, _) and a (_, v) and w (v, _)";
std::string andWith = "prog_line n; constant c1, c2; Select n such that Next* (c1, n) and Next* (n, c2) with c1.value = 5 and c2.value = 12";

// combination
std::string combi1 = "procedure p; call c; while w; Select p such that Calls(\"Second\", p) and Parent(w, c) with c.procName = p.procName";
std::string combi2 = "prog_line n; stmt s, s1; Select s.stmt# such that Follows* (s, s1) with s1.stmt#=n and n=10";
std::string combi3 = "while w1, w2, w3; Select <w1, w2, w3> such that Parent* (w1, w2) and Parent* (w2, w3)";
std::string combi4 = "assign a; while w; prog_line n; Select a such that Parent* (w, a) and Next* (60, n) pattern a(\"x\", _) with a.stmt#  = n";

// both suchthat-cl and pattern-cl
std::string selectSuchThatPattern = "assign a; stmt s, s1; Select s such that Follows(s1, 2) pattern a(\"x\", _)";
std::string suchThatPatternWhitespace = "assign Parent, Pattern; Select Pattern such that     Parent(_, _) pattern    Parent(_, _\"200\"_)";
std::string patternZero = "assign a; variable v; Select v such that Parent* (40, a) pattern a(v, _\"0\"_)";

// select BOOLEAN
std::string boolean1 = "Select BOOLEAN such that Next* (2, 9)";
std::string boolean2 = "Select BOOLEAN with 12 = 12";
std::string boolean3 = "Select BOOLEAN with a.stmt# = 12";

// select <tuple>
std::string tuple1 = "procedure p, q; Select <p, q > such that Calls(p, q)";

// INVALID QUERIES
// invalid syntax
std::string invalidDesignEntity = "stm s; Select s";
std::string integerStartingZero = "variable v; Select v such that Modifies(001, v)";
std::string patternSpecialChars = "assign a; Select a pattern a(_, \"%x\")";
std::string patternEmptyArgs = "assign a; Select a pattern()";
std::string patternStartZeroNumber = "assign a; Select a pattern(_, _\"0123\"_)";
std::string patternStartZeroName = "assign a; Select a pattern(_, _\"5xyz\"_)";
std::string repeatedNames = "stmt s; constant s; Select s";
std::string extraSemicolonDeclaration = "print pn;; Select pn";
std::string extraSemicolonSelection = "print pn; Select pn;";
std::string noSelectClause = "assign a; if ifs; constant c;";
std::string selectWithNoEntity = "stmt s; Select";
std::string noDeclarations = "Select s";
std::string invalidParentT = "while w; stmt s; Select s such that Parent * (w, s)";
std::string suchThatWhiteSpace = "variable v; Select v such   that Modifies(3, v)";

// invalid semantics
std::string synonymNameDoesNotExist = "stmt s; assign a; Select w";
std::string noSynonymName = "assign ; stmt s; constant c1; Select c1";
std::string noSynonymNameNoWhitespace = "assign; stmt s; constant c1; Select c1";
std::string noSynonymNameWhitespace = "assign      ; stmt s; constant c1; Select c1";
std::string selectModifiesInvalid = "variable v; Select v such that Modifies(_, v)";
std::string selectUsesInvalid = "if ifs; Select ifs such that Uses(_, \"x\")";

// TEST PATTERN
std::string patternRightUnderscore = "assign a; Select a pattern a(_, \"answer\"_)";
std::string patternLeftUnderscore = "assign a; Select a pattern a(_, _\"answer\")";
// invalid, valid
std::string selectPatternNotVariable = "stmt s; assign a; Select s pattern a(s, _)";
// valid, invalid
std::string patternIdentExpr = "assign a; Select a pattern a(\"x\", \"0answer\")";
// invalid syntax
std::string patternMissingBracket = "assign a; stmt s; Select a pattern a(s, \"answer\"";

// TEST WITH
std::string withInvalid1 = "variable v; Select v.value";
std::string withInvalid2 = "procedure p; Select p with p.stmt# = 3";
std::string withInvalid3 = "procedure p; stmt s; Select s with p.procName = 10";

// TEST AND
std::string diffClause1 = "assign a; while w; Select a such that Parent* (w, a) pattern a (\"x\", _) and Next* (1, a)";
std::string diffClause2 = "assign a; while w; Select a such that Parent* (w, a) and pattern a (\"x\", _) such that Next* (1, a)";
std::string diffClause3 = "assign a; while w; Select a such that Parent* (w, a) and Modifies (a, \"x\") and such that Next* (1, a)";
std::string andInvalid = "stmt s; Select s and Parent* (w, a)";


// TEST MODIFIES_P AND USES_P
// invalid, valid
std::string modifiesInteger = "stmt s; variable v; Select s such that Modifies(_ , v)";
std::string usesWhile = "while w; Select w such that Uses(_, _)";
// valid, invalid
std::string modifiesCall = "call c; Select c such that Modifies* (\"proc\", c)";
std::string usesProc = "procedure p; Select p such that Uses* (\"proc\", p)";
// invalid, invalid
std::string modifiesVarInt = "variable v; Select v such that Modifies(v, v)";
std::string usesProgPrint = "prog_line pl; print pn; stmt s; Select s such that Uses (pl, pn)";

// TEST NEXT AND NEXT_T
// invalid, valid
std::string nextVar = "variable v; Select v such that Next(v, 5)";
// valid, invalid
std::string nextProc = "procedure p; stmt s; Select s such that Next* (_, p)";
// invalid, invalid
std::string nextVarProc = "procedure p; variable v; Select v such that Next  (p, v)";

// TEST CALLS AND CALLS_T
// invalid, valid
std::string callsProgLine = "prog_line p; Select p such that Calls(p, \"proc\")";
// valid, invalid
std::string callsAssign = "assign a; procedure p; Select a such that Calls(p, a)";
// invalid, invalid
std::string callsIntStmt = "stmt s; Select s such that Calls* (4, s)";

// TEST AFFECTS AND AFFECTS_T
// invalid, valid
std::string affectsIf = "if ifs; assign a; Select a such that Affects* (ifs, a)";
// valid, invalid
std::string affectsIdent = "prog_line n; stmt s; Select n such that Affects (n, \"s\")";
// invalid, invalid
std::string affectsCallWhile = "call c; while w; stmt s; Select s such that Affects*   (c, w)";


// TEST MODIFIES_S AND USES_S
// invalid, valid
std::string modifiesPrint = "print pn; Select pn such that Modifies(pn, _)";
std::string usesRead = "read r; Select r such that Uses(r, _)";
// valid, invalid
std::string modifiesIfConstant = "if ifs; constant c; Select ifs such that Modifies(ifs, c)";
std::string usesWhileConstant = "while w; constant c; Select w such that Uses(w, c)";
// invalid, invalid
std::string modifiesVariablePrint = "variable v; print pn; Select v such that Modifies(v, pn)";
std::string usesReadWhile = "read r; while w; Select w such that Uses(r, w)";

// TEST PARENT AND PARENT_T
// invalid, valid
std::string parentAssign = "assign a; read r; Select a such that Parent(a, r)";
std::string parentTAssign = "assign a; read r; Select a such that Parent*(a, r)";
// valid, invalid
std::string parentIfConstant = "if ifs; procedure p; Select ifs such that Parent(ifs, p)";
std::string parentTWhileVariable = "while w; variable v; Select w such that Parent*(w, v)";
// invalid, invalid
std::string parentAssignVariable = "assign a; variable v; Select a such that Parent(a, v)";
std::string parentTReadConstant = "read r; procedure p; Select r such that Parent*(r, p)";

// TEST FOLLOWS AND FOLLOWS_T
// invalid, valid
std::string followsVariable = "variable v; stmt s; Select s such that Follows(v, 10)";
std::string followsTVariable = "variable v; stmt s; Select s such that Follows*(v, 10)";
// valid, invalid
std::string followsIfConstant = "if ifs; variable v; Select ifs such that Follows(ifs, v)";
std::string followsTAssignProcedure = "procedure p; assign a; Select a such that Follows*(a, p)";
// invalid, invalid
std::string followsVariableProcedure = "procedure p; variable v; Select v such that Follows(v, p)";
std::string followsTConstantVariable = "constant c; variable v; Select v such that Follows*(c, v)";