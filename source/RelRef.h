#pragma once

/*
	RelRef is an enum class that provides an easy representation
	of each type of relationship identified in the suchthat clause 
	contained in the query parsed
*/

enum class	RelRef {
	MODIFIES_S,
	MODIFIES_P,
	USES_S,
	USES_P,
	PARENT,
	PARENT_T,
	FOLLOWS,
	FOLLOWS_T,
	CALLS,
	CALLS_T,
	AFFECTS,
	AFFECTS_T,
	AFFECTS_BIP,
	AFFECTS_BIP_T,
	NEXT,
	NEXT_T,
	NEXT_BIP,
	NEXT_BIP_T,
	NONE
};