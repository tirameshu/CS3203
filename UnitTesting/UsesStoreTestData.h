#pragma once

#include "UsesStore.h"
/*
procedure testA
1    print p;
2    a = p * 2;
3    if (c == k) then {
4        pattern = p * a * a + p * ifs + p * w;
5        while(w > 0) {
6            w = w - 1;
		 }
7        call testB;
	 }
	else {
8        call testC;
	 }
}

procedure testB {
9	ifs = p + a;
10	while (a < 5) {
11		call testC;
	}
}

procedure testC {
12	a = a - 1;
13 call testD;
}

procedure testD {
14 p = 1;
15 x = p + 3;
}

*/

std::unordered_map<int, std::unordered_set<std::string>> expectedStmtUsesMap = { {1, {"p"}},
																					{2, {"p"}},
																					{3, {"c", "k", "a", "p", "ifs", "w"}},
																					{4, {"a", "p", "ifs", "w"}},
																					{5, {"w"}},
																					{6, {"w"}},
																					{7, {"p", "a"}},
																					{8, {"a", "p"}},
																					{9, {"a", "p"}},
																					{10, {"a", "p"}},
																					{11, {"a", "p"}},
																					{12, {"a"}},
																					{13, {"p"}},
																					{15, {"p"}} };
std::unordered_map<std::string, std::unordered_set<int>> expectedVarUsedMap = { {"a", {3, 4, 7, 8, 9, 10, 11, 12}},
																		{"c", {3}},
																		{"k", {3}},
																		{"p", {1, 2, 3, 7, 8, 9, 10, 11, 13, 15}},
																		{"w", {3, 4, 5, 6}},
																		{"ifs", {3, 4}} };


std::unordered_set<std::string> expectedAllUsedVarList = { "a", "c", "k", "p", "w", "ifs" };
std::unordered_set<std::string> exepctedAllPrintUsedVarList = { "p" };
std::unordered_set<int> expectedAllUsesAssignStmts = {2, 4, 6, 9, 12, 15};
std::unordered_set<int> expectedPrintStmtList = { 1 };
std::unordered_set<int> expectedAllUsesIfStmts = { 3 };
std::unordered_set<int> expectedAllUsesWhileStmts = { 5, 10 };

//UsesP
std::unordered_map<std::string, std::unordered_set<std::string>> expectedProcUsesMap = { {"testA", {"a", "c", "p", "k", "ifs", "w"}}, 
																								{"testB", {"a", "p"}}, 
																								{"testC", {"a", "p"}}, 
																								{"testD", {"p"}} };
std::unordered_map<std::string, std::unordered_set<std::string>> expectedVarUsesPMap = { {"a", {"testA", "testB", "testC"}},
																								{"c", {"testA"}}, 
																								{"k", {"testA"}},
																								{"p", {"testA", "testB", "testC", "testD"}},
																								{"w", {"testA"}},
																								{"ifs", {"testA"}} };
std::unordered_set<std::string> expectedAllUsesProc = {"testA", "testB", "testC", "testD"};

std::unordered_set<int> expectedAllUsesCallStmts = {7, 8, 11, 13};

std::unordered_map<int, std::string> expectedPrintStmtToVarMap = { {1, "p"} };
std::unordered_map<std::string, std::unordered_set<int>> expectedVarToPrintStmtMap = { {"p", {1}} };