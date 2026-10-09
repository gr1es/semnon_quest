#include "StateType.hpp"
#include <stdexcept>
#include <string>

const std::string enumToString(StateType s)
{
	switch (s)
	{
		case (StateType::Flag):
		{
			return ("Flag");
		}
		case (StateType::Skill):
		{
			return ("Skill");
		}
		case (StateType::Counter):
		{
			return ("Counter");
		}
		case (StateType::Feat):
		{
			return ("Feat");
		}
		case (StateType::Item):
		{
			return ("Item");
		}
		case (StateType::Standing):
		{
			return ("Standing");
		}
	}
	// no default case above, so the compiler can warn about unhandled enumerators
	throw std::runtime_error("ERROR: StateType: unknown StateType " + std::to_string(static_cast<int>(s)) + ".");
}

StateType stringToStateType(const std::string &str)
{
	// reminder: switch doesn't work with std::strings
	if (str == "Flag")
		return (StateType::Flag);
	if (str == "Skill")
		return (StateType::Skill);
	if (str == "Counter")
		return (StateType::Counter);
	if (str == "Feat")
		return (StateType::Feat);
	if (str == "Item")
		return (StateType::Item);
	if (str == "Standing")
		return (StateType::Standing);
	throw std::runtime_error("ERROR: StateType: unknown StateType " + str + ".");
}
