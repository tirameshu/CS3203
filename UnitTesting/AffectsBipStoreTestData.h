#pragma once

#include "AffectsBipStore.h"

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

std::unordered_map<int, affectsBipStruct> expectedStmtAffectsBipMap = {
	{1, {{}, {2, 4, 10}, {}, {2, 4, 10, 13}}}, 
	{2, {{1}, {4, 10, 13}, {1}, {4, 10, 13, 8}}}, 
	{4, {{1, 2, 6, 8, 13}, {}, {1, 2, 6, 8, 10, 13}, {}}}, 
	{6, {{}, {4}, {}, {4}}}, 
	{8, {{8, 10, 13}, {4}, {1, 2, 10, 8, 13}, {4}}}, 
	{10, {{1, 2}, {8}, {1, 2}, {4, 8}}}, 
	{13, {{2, 13}, {4, 10}, {2, 13, 1}, {4, 10, 8}}}
};

std::unordered_set<std::pair<int, int>, IntPairHasher> expectedAffectsBipPairList = {
	{1, 2}, 
	{1, 4}, 
	{1, 10}, 
	{2, 4}, 
	{2, 10}, 
	{2, 13}, 
	{6, 4}, 
	{8, 4},  
	{10, 8}, 
	{13, 4}, 
	{13, 10}
};
std::unordered_set<std::pair<int, int>, IntPairHasher> expectedAffectsBipStarPairList = {
	{1, 2}, 
	{1, 4}, 
	{1, 10}, 
	{1, 13},
	{2, 4},
	{2, 10}, 
	{2, 13}, 
	{2, 8}, 
	{6, 4}, 
	{8, 4},
	{10, 8}, 
	{10, 4}, 
	{13, 4}, 
	{13, 10}, 
	{13, 8}
};
std::unordered_set<int> expectedAffectedBipList = {2, 4, 8, 10, 13};
std::unordered_set<int> expectedAffectingBipList{ 1, 2, 6, 8, 10, 13 };

