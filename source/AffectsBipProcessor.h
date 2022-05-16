#pragma once
#include <unordered_set>
#include <unordered_map>
#include <stack>

/*
	AffectsBipExtractor is responsible for extracting AffectsBip design abstraction relationship
	information between design entities of the program, and populate the PKB's AffectsBipStore
	with information extracted.
*/

class AffectsBipProcessor {
public:
	static void calculateAffectsBip();

private:
	static void calculateAffect(int stmt, std::unordered_map<std::string, std::unordered_set<int>>& LMT);
	static void handleCall(int stmt, std::unordered_map<std::string, std::unordered_set<int>>& LMT);
	static void handleWhile(int stmt, std::unordered_map<std::string, std::unordered_set<int>>& LMT);
	static void handleIf(int stmt, std::unordered_map<std::string, std::unordered_set<int>>& LMT);
	static void handleAssign(int stmt, std::unordered_map<std::string, std::unordered_set<int>>& LMT);
	static void handleOther(int stmt, std::unordered_map<std::string, std::unordered_set<int>>& LMT);
};
