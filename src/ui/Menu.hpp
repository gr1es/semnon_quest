#pragma once

#include "Display.hpp"
#include <string>
#include <vector>

enum class Choice
{
	Continue,
	NewGame,
	Settings,
	Credits,
	Exit,
	Quit
};

class Menu
{
		// TODO: add savegame handling
		// right now, menu can only start a game or continue a running game
	public:
		Menu(Display &display, bool is_ingame);
		void renderEntries() const;
		Choice getChoice() const;
		void showCredits() const;
		void showSettings() const;

	private:
		Display &_display;
		std::vector<std::string> _entries;
		bool _isIngame;
};
