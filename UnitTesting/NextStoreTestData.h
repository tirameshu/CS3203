#pragma once

#include "NextStore.h"

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
13	a = a - 1;
}
*/

std::unordered_map<int, nextStruct> expectedStmtNextMap = {
	{1, {{}, {2}, {}, {2, 3, 4, 5, 6, 7, 8, 9}}},
	{2, {{1}, {3}, {1}, {3, 4, 5, 6, 7, 8, 9}}},
	{3, {{2}, {4}, {1, 2, 3, 4, 5, 6, 7, 8}, {3, 4, 5, 6, 7, 8, 9}}},
	{4, {{3}, {5}, {1, 2, 3, 4, 5, 6, 7, 8}, {3, 4, 5, 6, 7, 8, 9}}},
	{5, {{4}, {6, 7}, {1, 2, 3, 4, 5, 6, 7, 8}, {3, 4, 5, 6, 7, 8, 9}}},
	{6, {{5}, {8}, {1, 2, 3, 4, 5, 6, 7, 8}, {3, 4, 5, 6, 7, 8, 9}}},
	{7, {{5}, {8}, {1, 2, 3, 4, 5, 6, 7, 8}, {3, 4, 5, 6, 7, 8, 9}}},
	{8, {{6, 7}, {3}, {1, 2, 3, 4, 5, 6, 7, 8}, {3, 4, 5, 6, 7, 8, 9}}},
	{9, {{3}, {}, {1, 2, 3, 4, 5, 6, 7, 8}, {}}},
	{10, {{}, {11}, {}, {11, 12}}},
	{11, {{10}, {12}, {10, 11, 12}, {11, 12}}},
	{12, {{11}, {11}, {10, 11, 12}, {11, 12}}},
};

std::unordered_set<std::pair<int, int>, IntPairHasher> expectedNextPairList = {
	{1, 2}, {2, 3}, {3, 4}, {4, 5}, {5, 6}, {5, 7}, {6, 8}, {7, 8}, {8, 3}, {3, 9},
	{10, 11}, {11, 12}, {12, 11}
};

std::unordered_set<std::pair<int, int>, IntPairHasher> expectedNextStarPairList = {
	{1, 2}, {1, 3}, {1, 4}, {1, 5}, {1, 6}, {1, 7}, {1, 8}, {1, 9},
	{2, 3}, {2, 4}, {2, 5}, {2, 6}, {2, 7}, {2, 8}, {2, 9},
	{3, 3}, {3, 4}, {3, 5}, {3, 6}, {3, 7}, {3, 8}, {3, 9},
	{4, 3}, {4, 4}, {4, 5}, {4, 6}, {4, 7}, {4, 8}, {4, 9},
	{5, 3}, {5, 4}, {5, 5}, {5, 6}, {5, 7}, {5, 8}, {5, 9},
	{6, 3}, {6, 4}, {6, 5}, {6, 6}, {6, 7}, {6, 8}, {6, 9},
	{7, 3}, {7, 4}, {7, 5}, {7, 6}, {7, 7}, {7, 8}, {7, 9},
	{8, 3}, {8, 4}, {8, 5}, {8, 6}, {8, 7}, {8, 8}, {8, 9},
	{10, 11}, {10, 12}, {11, 11}, {11, 12}, {12, 11}, {12, 12}
};

std::unordered_set<int> expectedPrevList = { 1, 2, 3, 4, 5, 6, 7, 8, 10, 11, 12 };

std::unordered_set<int> expectedNextList = { 2, 3, 4, 5, 6, 7, 8, 9, 11, 12 };
