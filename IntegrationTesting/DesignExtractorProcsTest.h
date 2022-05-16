#pragma once

#include <stdio.h>
#include <string>
#include <unordered_map>
#include <unordered_set>

std::unordered_set<int> emptyIntSet = {};
std::unordered_set<std::string> emptyStrSet = {};
std::unordered_set<std::string> allCallees = { "B", "C", "D", "E", "F" };
std::unordered_set<std::string> allCallers = { "A", "B", "C", "D", "G" };
std::unordered_set<std::string> allVarsUsed = { "usesB1", "usesB2", "usesB3", "usesF1", "modF1", "usesC1", "usesC2", "usesC3", "usesE1", "modE1" };
std::unordered_set<std::string> allVarsModified = { "modA", "modB1", "modF1", "modC1", "modC2", "modE1" };