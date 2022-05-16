#pragma once

#include "IfStore.h"

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
11	if (a < 5) {
12		call testC;
	}
}

procedure testC {
13  if (a < b) {
14		a = a + 1;
	}
}
*/

std::unordered_map<int, std::unordered_set < std::string >> expectedIfStmtUsesMap = {
	{5, {"c", "k"}}, {11, {"a"}}, {13, {"a", "b"}}
};

std::unordered_map<std::string, std::unordered_set<int>> expectedVarToIfStmtMap = {
	{"c", {5}}, {"k", {5}}, {"a", {11, 13}}, {"b", {13}}
};

std::unordered_set<std::string> expectedIfUsedVarList = {
	"c", "k", "a", "b"
};

std::unordered_set<int> expectedIfUsesStmtList = {
	5, 11, 13
};