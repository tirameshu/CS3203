#pragma once

#include "ModifiesStore.h"

/*
procedure testA
1    read p;
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
 
std::unordered_map<int, std::unordered_set<std::string>> expectedStmtModifiesMap = { {1, {"p"} },
																						{2, {"a"} },
																						{3, {"pattern", "w", "ifs", "a", "p", "x"} },
																						{4, {"pattern"} },
																						{5, {"w"}},
																						{6, {"w"}},
																						{7, {"ifs", "a", "p", "x"}},
																						{8, {"a", "p", "x"}},
																						{9, {"ifs"}},
																						{10, {"a", "p", "x"}}, 
																						{11, {"a", "p", "x"}},
																						{12, {"a"}},
																						{13, {"p", "x"}},
																						{14, {"p"}}, 
																						{15, {"x"}}  };


std::unordered_map<std::string, std::unordered_set<int>> expectedVarModifiedMap = { {"p", {1, 3, 7, 8, 10, 11, 13, 14}}, {"a", {2, 3, 7, 8, 10, 11, 12}}, {"pattern", {3, 4}}, {"w", {3, 5, 6}}, {"ifs", {3, 7, 9}}, {"x", {3, 7, 8, 10, 11, 13, 15}} };
std::unordered_set<std::string> expectedAllModifiedVarList = { "p", "a", "pattern", "w", "ifs", "x" };
std::unordered_set<std::string> expectedAllReadModifiedVarList = {};
std::unordered_set<int> expectedModifyingStmtList = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
std::unordered_set<int> expectedReadStmtList = {1};
std::unordered_set<int> expectedModifyingIfStmtList = {3};
std::unordered_set<int> expectedModifyingWhileStmtList = { 5, 10 };

//ModifiesP
std::unordered_map<std::string, std::unordered_set<std::string>> expectedProcModifiesMap = { {"testA", {"p", "a", "pattern", "w", "ifs", "x"}}, {"testB", {"ifs", "a", "p", "x"}},{"testC", {"a", "p", "x"}}, {"testD", {"p", "x"}} };
std::unordered_map<std::string, std::unordered_set<std::string>> expectedVarModifiedProcMap = { {"p", {"testA", "testB", "testC", "testD"}}, {"a", {"testA", "testB", "testC"}}, {"pattern", {"testA"}}, {"w", {"testA"}}, {"ifs", {"testA", "testB"}}, {"x", {"testA", "testB", "testC", "testD"}} };
std::unordered_set<std::string> expectedAllModifiyingProcs = { "testA", "testB", "testC", "testD" };

std::unordered_set<int> expectedModifyingCallStmtList = { 7, 8, 11, 13 };

std::unordered_map<int, std::string> expectedReadStmtToVarMap = { {1, "p"} };
std::unordered_map<std::string, std::unordered_set<int>> expectedVarToReadStmtMap = { {"p", {1}} };
