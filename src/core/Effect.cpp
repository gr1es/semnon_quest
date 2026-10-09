#include <iostream>

#include "Effect.hpp"

void applyEffect(const Effect &eff, GameState &gs)
{
	switch (eff.type)
	{
		// FLAG
		case (StateType::Flag):
		{
			gs.setFlag(eff.key, eff.flagValue);
			break;
		}

		// SKILL
		case (StateType::Skill):
		{
			gs.modifySkill(eff.key, eff.delta);
			break;
		}

		// COUNTER
		case (StateType::Counter):
		{
			gs.modifyCounter(eff.key, eff.delta);
			break;
		}

		// FEAT
		case (StateType::Feat):
		{
			gs.addFeat(eff.key);
			break;
		}

		// ITEM
		case (StateType::Item):
		{
			gs.modifyInventory(eff.key, eff.delta);
			break;
		}

		// FACTION STANDING
		case (StateType::Standing):
		{
			gs.modifyFactionStanding(eff.key, eff.delta);
			break;
		}

		default:
			std::cerr << "WARNING: Effect: applyEffect has no case for StateType " << enumToString(eff.type) << ".\n";
	}
}

void applyEffects(const std::vector<Effect> &effs, GameState &gs)
{
	for (const Effect &eff : effs)
		applyEffect(eff, gs);
}
