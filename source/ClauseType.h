#pragma once

/*
	ClauseType is an enum class that provides an easy representation
	of each type of clause identified in the query parsed
*/

enum class ClauseType {
	SUCH_THAT,
	PATTERN,
	WITH
};