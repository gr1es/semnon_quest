#include "OptionType.hpp"
#include <stdexcept>
#include <string>

const std::string enumToString(OptionType o)
{
	switch (o)
	{
		case (OptionType::Dialogue):
		{
			return ("Dialogue");
		}
		case (OptionType::Action):
		{
			return ("Action");
		}
		case (OptionType::Move):
		{
			return ("Move");
		}
	}
	throw std::runtime_error("ERROR: OptionType: unknown OptionType " + std::to_string(static_cast<int>(o)) + ".");
}

OptionType stringToOptionType(const std::string &s)
{
	// reminder: switch doesn't work with std::strings
	if (s == "Dialogue")
		return (OptionType::Dialogue);
	if (s == "Action")
		return (OptionType::Action);
	if (s == "Move")
		return (OptionType::Move);
	throw std::runtime_error("ERROR: OptionType: unknown OptionType " + s + ".");
}
