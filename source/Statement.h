#pragma once

#include<string>
#include<vector>

/*
	Statement is a class to contain key information regarding a generic statement 
	in the program. A generic statement will always have the following information
	
	1. stmtType
	- The type of statement (assign, print, read, if, while, call)
	2. stmtNo
	- The statement number of the statement
*/

class Statement {
private:
	int stmtType;
	int stmtNo;

public:
	// constructor
	Statement(int type, int number); // for general stmts

	// getters
	int getStmtType();
	int getStmtNo();	

	bool operator==(const Statement& otherStmt) const;
};
