#pragma once

#include "ParentStore.h"

/*
procedure testA {
1    p = 4;
2    a = p * 2;
3    if (c == k) then {
4        pattern = p * a * a + p * ifs + p * w;
5        while(w > 0) {
6            w = w - 1;
		 }
7        ifs = ifs -1;
	 }
	else {
8        p = 5;
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
}
*/

std::unordered_map<int, parentStruct> expectedStmtParentMap = { {3, {0, {4, 5, 7, 8}, {}, {4, 5, 6, 7, 8}}},
	{4, {3, {}, {3}, {}}}, {5, {3, {6}, {3}, {6}}}, {6, {5, {}, {3, 5}, {}}}, {7, {3, {}, {3}, {}}}, 
	{8, {3, {}, {3}, {}}}, {10, {0, {11}, {}, {11}}}, {11, {10, {}, {10}, {}}}
};

std::unordered_set<std::pair<int, int>, IntPairHasher> expectedParentPairList = {
	std::make_pair(3, 4), std::make_pair(3, 5), std::make_pair(3, 7), std::make_pair(3, 8),
	std::make_pair(5, 6), std::make_pair(10, 11)
};

std::unordered_set<std::pair<int, int>, IntPairHasher> expectedParentStarPairList = {
	std::make_pair(3, 4), std::make_pair(3, 5), std::make_pair(3, 6), std::make_pair(3, 7), 
	std::make_pair(3, 8), std::make_pair(5, 6), std::make_pair(10, 11)
};

std::unordered_set<int> expectedParentList = { 3, 5, 10 };
std::unordered_set<int> expectedChildList = { 4, 5, 6, 7, 8, 11 };