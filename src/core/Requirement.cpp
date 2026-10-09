#include "Requirement.hpp"
#include <iostream>

// checks if a single requirement is given or not according to current GameState
bool requirementMet(const Requirement &req, const GameState &gs)
{
	switch (req.type)
	{
		// FLAG
		case (StateType::Flag):
			return (gs.getFlag(req.key) == req.expected);

		// SKILL
		case (StateType::Skill):
		{
			// guard is strictly speaking not necessary here
			// since < comparison with empty optional<int> defaults to false
			if (req.min.has_value() && gs.getSkill(req.key) < req.min)
				return (false);
			// guard IS necessary here!
			// since > comparison with empty optional<int> defaults to true
			// --> return(false) would trigger!
			if (req.max.has_value() && gs.getSkill(req.key) > req.max)
				return (false);
			return (true);
		}

		// COUNTER
		case (StateType::Counter):
		{
			if (req.min.has_value() && gs.getCounter(req.key) < req.min)
				return (false);
			if (req.max.has_value() && gs.getCounter(req.key) > req.max)
				return (false);
			return (true);
		}

		// FEAT
		case (StateType::Feat):
			return (gs.hasFeat(req.key) == req.expected);

		// ITEM
		case (StateType::Item):
		{
			if (req.min.has_value() && gs.getItem(req.key) < req.min)
				return (false);
			if (req.max.has_value() && gs.getItem(req.key) > req.max)
				return (false);
			return (true);
		}

		// FACTION STANDING
		case (StateType::Standing):
		{
			if (req.min.has_value() && gs.factionStanding(req.key) < req.min)
				return (false);
			if (req.max.has_value() && gs.factionStanding(req.key) > req.max)
				return (false);
			return (true);
		}

		default:
		{
			std::cerr << "WARNING: Requirement: requirementMet has no case for StateType " << enumToString(req.type) << ".\n";
			return (false);
		}
	}
}

// check for multiple requirements
bool requirementsMet(const std::vector<Requirement> &reqs, const GameState &gs)
{
	for (const Requirement &req : reqs)
	{
		if (!requirementMet(req, gs))
			return (false);
	}
	return (true);
}
