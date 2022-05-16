#pragma once

#include "FollowsStore.h"
#include "StatementTypeEnum.h"

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

std::unordered_map<int, followsStruct> expectedStmtFollowsMap = { {1, {0, 2, {}, {2, 3}}},
	{2, {1, 3, {1}, {3}}}, {3, {2, 0, {1, 2}, {}}}, {4, {0, 5, {}, {5, 7}}},
	{5, {4, 7, {4}, {7}}}, {7, {5, 0, {4, 5}, {}}}, {9, {0, 10, {}, {10}}}, {10, {9, 0, {9}, {}}} };

std::unordered_set<std::pair<int, int>, IntPairHasher> expectedFollowsPairList = {
	std::make_pair(1, 2), std::make_pair(2, 3), std::make_pair(4, 5), std::make_pair(5, 7), std::make_pair(9, 10)
};

std::unordered_set<std::pair<int, int>, IntPairHasher> expectedFollowsStarPairList = {
	std::make_pair(1, 2), std::make_pair(1, 3), std::make_pair(2, 3), std::make_pair(4, 5), std::make_pair(4, 7),
	std::make_pair(5, 7), std::make_pair(9, 10)
};

std::unordered_set<int> expectedFollowedList = { 1, 2, 4, 5, 9 };

std::unordered_set<int> expectedFollowerList = { 2, 3, 5, 7, 10 };