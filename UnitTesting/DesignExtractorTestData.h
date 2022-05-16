#pragma once

#include <stdio.h>
#include <string>
#include <unordered_map>
#include <unordered_set>


class DesignExtractorTestClass {
public:
	static std::unordered_set<std::string> allCallees;
	static std::unordered_set<std::string> allCallers;

	static std::unordered_map<std::string, std::unordered_set<std::string>> procToCallers;
	static std::unordered_map<std::string, std::unordered_set<std::string>> procToCallees;
	static std::unordered_map<std::string, std::unordered_set<std::string>> procToAllCallees;
	static std::unordered_map<std::string, std::unordered_set<std::string>> procToAllCallers;

	static void computeAllCallees(std::string);
	static void computeAllCallers(std::string);
};
