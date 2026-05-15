#pragma once

#include "GameState.hpp"
#include "LocationManager.hpp"
#include "TerminalDisplay.hpp"

class Game
{
	public:
		Game();
		void run();

	private:
		GameState _gameState;
		TerminalDisplay _terminalDisplay;
		LocationManager _locationManager;
};
