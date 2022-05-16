#include "Statement.h"

// For general statements 
Statement::Statement(int type, int number) {
	stmtType = type;
	stmtNo = number;
}

int Statement::getStmtType() {
	return stmtType;
}

int Statement::getStmtNo() {
	return stmtNo;
}

bool Statement::operator==(const Statement& otherStmt) const {
	return (stmtNo == otherStmt.stmtNo);
}
