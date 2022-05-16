#pragma once

#include <utility>

#include "Statement.h"

// Hasher function for pair<int, int>
class IntPairHasher {
public:
	template<typename X, typename Y>

	size_t operator ()(const std::pair<X, Y> &p) const {
		return p.first * p.second + p.first; // to have minimal collision
	}
};

// Hasher function for pair<string, string>
class StringPairHasher {
public:
	template<typename X, typename Y>

	size_t operator ()(const std::pair<X, Y>& p) const {
		auto hash1 = std::hash<X>{}(p.first);
		auto hash2 = std::hash<Y>{}(p.second);
		return hash1 ^ hash2; 
	}
};

// Hasher function for Statement class
class StatementHasher {
public:
	size_t operator ()(Statement stmt) const {
		int stmtNo = stmt.getStmtNo();
		int stmtType = stmt.getStmtType();
		return stmtNo * stmtType + stmtNo;
	}
};
