#pragma once

#include "CallStore.h"


/*
procedure testA
1    p = 4;
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


callStruct testAStruct = callStruct{ {}, {"testB", "testC"}, {}, {"testB", "testC", "testD"} };
callStruct testBStruct = callStruct{ {"testA"}, {"testC"}, {"testA"}, {"testC", "testD"} };
callStruct testCStruct = callStruct{ {"testA", "testB"}, {"testD"}, {"testA", "testB"}, {"testD"} };
callStruct testDStruct = callStruct{ {"testC"}, {}, {"testA", "testB", "testC"}, {} };


std::unordered_map<std::string, callStruct> expectedProcCallsMap = { {"testA", testAStruct},
																		{"testB", testBStruct},
																		{"testC", testCStruct},
																		{"testD", testDStruct} };

std::unordered_set<std::string> expectedListOfRootProcs = { "testA" };
std::unordered_set<std::string> expectedListOfLeafProcs = { "testD" };
std::unordered_set<std::string> expectedListOfAllCallers = { "testA", "testB", "testC" };
std::unordered_set<std::string> expectedListOfAllCallees = { "testB", "testC", "testD" };
std::unordered_map<int, std::string> expectedCallerStmtToProcMap = { {7, "testB"}, {8, "testC"}, {11, "testC"}, {13, "testD"} };
std::unordered_map<std::string, std::unordered_set<int>> expectedProcToCallerStmtsMap = { {"testB", {7}}, {"testC", {8, 11}}, {"testD", {13}} };
std::unordered_map<std::string, std::unordered_set<int>> expectedProcedureToCallStmtMap = { {"testA", {7, 8}}, {"testB", {11}}, {"testC", {13}} };
std::unordered_map<int, std::string> expectedCallStmtToProcedureMap = { {7, "testA"}, {8, "testA"}, {11, "testB"}, {13, "testC"} };



