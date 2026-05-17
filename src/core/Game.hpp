#pragma once

#include "GameState.hpp"
#include "LocationManager.hpp"
#include "Menu.hpp"
#include "Option.hpp"
#include "TerminalDisplay.hpp"
#include <vector>

class Game
{
	public:
		Game();
		void run();

	private:
		GameState _gameState;
		TerminalDisplay _terminalDisplay;
		LocationManager _locationManager;

		/// helper functions for run()
		Choice showMenu(Display &display, bool is_ingame);
		void startNewGame();
		std::vector<Option> buildOptions(const Scene &scene) const;
		void renderScene(const Location &loc, const Scene &scene, const std::vector<Option> &options);
		bool handleInput(const std::vector<Option> &options);
};
