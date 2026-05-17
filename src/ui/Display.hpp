#pragma once

#include <string>
#include <vector>

// abstract base class defining the rendering interface
// will be compatible with Terminal AND Ncurses
// game code depends on Display, not on any concrete class
// --> swapping between Ncurses and terminal requires no changes to game logic

// TerminalDisplay: phase A backend (std::cout), kept as debug/fallback even in phase B
// NcursesDisplay: phase B backend — arrow keys, color, proper layout

class Display
{
	public:
		virtual ~Display() { };

		/// clears the terminal between scenes
		virtual void clearScreen() = 0;
		/// tells player first glance where he/she is
		virtual void renderSceneName(const std::string &scene_name) = 0;
		/// renders optional ASCII art above description
		virtual void renderArt(const std::string &art) = 0;
		/// renders the main scene description text
		virtual void renderDescription(const std::string &text) = 0;
		/// renders the numbered list of player choices (1–0, max 10)
		/// handles numbering internally — caller passes plain option strings
		virtual void renderOptions(const std::vector<std::string> &options) = 0;
		/// renders the status bar at the bottom of the screen
		/// player_info: pre-formatted string, like "[Aldric | Lv2]"
		/// location_name: the location node, like "Tavern"
		/// scene_name: the current scene within the location, like "Common Room"
		/// implementation formats them as "Tavern – Common Room"
		/// shortcuts (C/J/I/M for Character/Journal/Inventory/Menu) are always highlighted
		virtual void renderStatusBar(const std::string &player_info, const std::string &location_name, const std::string &scene_name) = 0;
		/// reads a single keypress from the player and returns it as a char
		/// valid inputs: '1' to'0' for options, C/J/I/M for menus
		virtual char getInput() = 0;
};
