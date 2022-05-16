#pragma once

/*
	StatementTypeEnum is an enum class that provides an easy representation
	of each statement type in the program
*/

enum StatementTypeEnum {
	// 1: assign, 2: print, 3: read, 4: if, 5: while, 6: else (for parser usage), 7: call
	ASSIGN = 1,
	PRINT = 2,
	READ = 3,
	IF = 4,
	WHILE = 5,
	ELSE = 6,
	CALL = 7
};
