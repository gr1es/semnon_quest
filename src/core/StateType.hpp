#pragma once
#include <string>

enum class StateType
{
	Flag,
	Skill,
	Counter,
	Feat,
	Item,
	Standing
};

// other enum-to-string helper functions have the same names (thanks overload functionality!)
const std::string enumToString(StateType s);
// this can't be overloaded since only the return types differ which is not enough of a distinction for the compiler to be able to decide which function to use
// -> this helper needs a unique name
StateType stringToStateType(const std::string &str);
