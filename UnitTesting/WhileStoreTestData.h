#pragma once

#include "WhileStore.h"

/*
procedure testA {
1    p = 4;
2    a = p * 2;
3    while (w > 0) {
4        pattern = p * a * a + p * ifs + p * w;
5        if (c == k) {
6            w = w - 1;
		 }
		 else {
7			call testB;
		 }
8        ifs = ifs -1;
	 }
9	read p;
}

procedure testB {
10	ifs = p + a;
11	while (a < 5) {
12		call testC;
	}
}

procedure testC {
13 while (a < b) {
14		a = a + 1;
	}
}
*/

std::unordered_map<int, std::unordered_set < std::string >> expectedWhileStmtUsesMap = {
	{3, {"w"}}, {11, {"a"}}, {13, {"a", "b"}}
};

std::unordered_map<std::string, std::unordered_set<int>> expectedVarToWhileStmtMap = {
	{"w", {3}}, {"a", {11, 13}}, {"b", {13}}
};

std::unordered_set<std::string> expectedWhileUsedVarList = {
	"w", "a", "b"
};

std::unordered_set<int> expectedWhileUsesStmtList = {
	3, 11, 13
};
