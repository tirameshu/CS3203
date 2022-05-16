#pragma once

#include "AssignStore.h"

/*
procedure testA {
1    p = 4;
2    a = p * 2;
3    if (c == k) then {
4        pattern = p * a * a + p * ifs + p * w;
5        while(w > 0) {
6            p = p - 1;
		 }
7        ifs = ifs -1;
	 }
	else {
8        p = 5;
	 }
}

procedure testB {
9	ifs = ifs -1;
10	while (a < 5) {
11		call testC;
	}
}

procedure testC {
12	pattern = p * a * a + p * ifs + p * w;
}
*/

std::unordered_set<int> expectedAssignStmtList = {1, 2, 4, 6, 7, 8, 9, 12};

std::unordered_map<int, assignStruct> expectedStmtAssignMap = {
	{1, {"p", "4"}}, {2, {"a", "p2*"}}, {4, {"pattern", "pa*a*pifs*+pw*+"}}, {6, {"p", "p1-"}},
	{7, {"ifs", "ifs1-"}}, {8, {"p", "5"}}, {9, {"ifs", "ifs1-"}} , {12, {"pattern", "pa*a*pifs*+pw*+"}}
};

std::unordered_map<std::pair<std::string, std::string>, std::unordered_set<int>, StringPairHasher> expectedVarExpStmtList = {
	{std::make_pair("p", "4"), {1}}, {std::make_pair("a", "p2*"), {2}}, 
	{std::make_pair("pattern", "pa*a*pifs*+pw*+"), {4, 12}}, {std::make_pair("p", "p1-"), {6}}, 
	{std::make_pair("ifs", "ifs1-"), {7, 9}}, {std::make_pair("p", "5"), {8}}
};

std::unordered_map <std::string, std::unordered_set<std::string>> expectedVarToExpMap = {
	{"p", {"4", "p1-", "5"}}, {"a", {"p2*"}}, {"pattern", {"pa*a*pifs*+pw*+"}}, {"ifs", {"ifs1-"}}
};

std::unordered_map<std::string, std::unordered_set<int>> expectedVarAssignStmtList = {
	{"p", {1, 6, 8}}, {"a", {2}}, {"pattern", {4, 12}}, {"ifs", {7, 9}}
};

std::unordered_map<std::string, std::unordered_set<int>> expectedExpAssignStmtList = {
	{"4", {1}}, {"p2*", {2}}, {"pa*a*pifs*+pw*+", {4, 12}}, {"p1-", {6}}, {"ifs1-", {7, 9}}, {"5", {8}}
};