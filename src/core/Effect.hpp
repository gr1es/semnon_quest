#pragma once

#include <string>
#include <vector>

#include "GameState.hpp"
#include "StateType.hpp"

struct Effect
{
	StateType type;
	std::string key;		// state ID
	int delta = 0;			// for Modify methods
	bool flagValue = true;	// so set a Flag to true/false
};

void applyEffect(const Effect &eff, GameState &gs);
void applyEffects(const std::vector<Effect> &effs, GameState &gs);
