#pragma once

#include "PKB.h"
#include "StatementTypeEnum.h"

/*
procedure testA {
1    p = 4;
2    a = p * 2;
3    if (c == k) then {
4        pattern = p * a * a + p * ifs + p * w;
5        while(w > 0) {
6            read p;
		 }
7        ifs = ifs -1;
	 }
	else {
8        print p;
	 }
9	 call testB
}

procedure testB {
10	ifs = ifs - 1;
11	while (a < 5) {
12		print ifs;
	}
13  call testC
}

procedure testC {
14	pattern = p * a * a + p * ifs + p * w;
}
*/

std::unordered_map<int, Statement> expectedProgramStmtMap = {
	{1, {ASSIGN, 1}}, {2, {ASSIGN, 2}}, {3, {IF, 3}}, {4, {ASSIGN, 4}}, {5, {WHILE, 5}}, {6, {READ, 6}},
	{7, {ASSIGN, 7}}, {8, {PRINT, 8}}, {9, {CALL, 9}}, {10, {ASSIGN, 10}}, {11, {WHILE, 11}}, {12, {PRINT, 12}},
	{13, {CALL, 13}}, {14, {ASSIGN, 14}}
};

std::unordered_map<std::string, std::vector<int>> expectedProcedureStmtMap = {
	{"testA", {1, 2, 3, 4, 5, 6, 7, 8, 9}}, {"testB", {10, 11, 12, 13}}, {"testC", {14}},
};

std::unordered_map<int, std::string> expectedStmtProcedureMap = {
	{1, "testA"}, {2, "testA"}, {3, "testA"}, {4, "testA"}, {5, "testA"}, {6, "testA"},
	{7, "testA"}, {8, "testA"}, {9, "testA"}, {10, "testB"}, {11, "testB"}, {12, "testB"},
	{13, "testB"}, {14, "testC"}
};

std::unordered_map<int, std::vector<int>> expectedIfStmtMap = {
	{3, {4, 5, 7, 8}}
};

std::unordered_map<int, std::vector<int>> expectedWhileStmtMap = {
	{5, {6}}, {11, {12}}
};

std::unordered_map<std::string, std::unordered_set<int>> expectedConstantStmtMap = {
	{"4", {1}}, {"2", {2}}, {"0", {5}}, {"1", {7, 10}}, {"5", {11}}
};

std::unordered_map<int, std::unordered_set<std::string>> expectedStmtConstantMap = {
	{1, {"4"}}, {2, {"2"}}, {5, {"0"}}, {7, {"1"}}, {10, {"1"}}, {11, {"5"}}
};

std::unordered_map<int, std::string> expectedCallProcedureMap = {
	{9, "testB"}, {13, "testC"}
};

std::unordered_set<int> expectedStatementList = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14};
std::unordered_set<std::string> expectedProcedureList = { "testA", "testB", "testC" };
std::unordered_set<std::string> expectedVariableList = { "p", "a", "c", "k", "pattern", "ifs", "w" };
std::unordered_set<std::string> expectedConstantList = { "4", "2", "0", "1", "5" };

std::unordered_set<int> expectedPKBAssignStmtList = { 1, 2, 4, 7, 10, 14 };
std::unordered_set<int> expectedPKBPrintStmtList = { 8, 12};
std::unordered_set<int> expectedPKBReadStmtList = { 6 };
std::unordered_set<int> expectedIfStmtList = { 3 };
std::unordered_set<int> expectedWhileStmtList = { 5, 11 };
std::unordered_set<int> expectedCallStmtList = { 9, 13 };