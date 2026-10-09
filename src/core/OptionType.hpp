#pragma once
#include <string>

enum class OptionType
{
	Dialogue,
	Action,
	Move
};

// other enum-to-string helper functions have the same names (thanks overload functionality!)
const std::string enumToString(OptionType o);
// this can't be overloaded since only the return types differ which is not enough of a distinction for the compiler to be able to decide which function to use
// -> this helper needs a unique name
OptionType stringToOptionType(const std::string &str);
