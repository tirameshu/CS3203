#pragma once

#include "AffectsStore.h"

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


std::unordered_map<int, affectsStruct> expectedStmtAffectsMap = {
	{1, {{}, {2}, {}, {2, 4}}},
	{2, {{1}, {4}, {1}, {4}} },
	{4, {{2, 6, 8}, {}, {1, 2, 6, 8}, {}} }, 
	{6, {{}, {4}, {}, {4}}}, 
	{8, {{}, {4}, {}, {4}}}, 
};

std::unordered_set<std::pair<int, int>, IntPairHasher>  expectedAffectsPairList = {
	{1, 2},
	{2, 4}, 
	{6, 4}, 
	{8, 4}
};

std::unordered_set<std::pair<int, int>, IntPairHasher>  expectedAffectsStarPairList = {
	{1, 2}, 
	{1, 4}, 
	{2, 4}, 
	{6, 4}, 
	{8, 4}
};

std::unordered_set<int>  expectedAffectedList = { 2, 4 };
std::unordered_set<int>  expectedAffectingList = {1, 2, 6, 8};
