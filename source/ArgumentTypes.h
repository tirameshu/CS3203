#pragma once

/*
	ArgumentTypes is an enum class that provides an easy representation
	of each argument type identified in the query parsed
*/

enum class	ArgumentTypes {
	STMT,
	READ,
	PRINT,
	WHILE,
	IF,
	ASSIGN,
	VARIABLE,
	CONSTANT,
	PROCEDURE,
	CALL,
	PROG_LINE,
	INT,
	STRING,
	BOOLEAN,
	ANY,
	NONE
};