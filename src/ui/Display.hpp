#pragma once

#include <string>
#include <vector>

// abstract base class for definition of display interface
// concrete implementation classes(TerminalDisplay, NcursesDisplay) handle the actual output
// the game loop talks only to Display
class Display
{
	public:
		virtual ~Display() { };

		/// clears the terminal between scenes
		virtual void clearScreen() = 0;
		/// tells player first glance where he/she is
		virtual void renderSceneName(const std::string &sceneName) = 0;
		/// renders optional ASCII art above description
		virtual void renderArt(const std::string &art) = 0;
		/// renders the main scene description text
		virtual void renderDescription(const std::string &text) = 0;
		/// renders the numbered list of player choices (1–0, max 10)
		/// handles numbering internally — caller passes plain option strings
		virtual void renderOptions(const std::vector<std::string> &options) = 0;
		/// renders the status bar at the bottom of the screen
		/// playerInfo: pre-formatted string, like "[Aldric | Ranger | Lv2]"
		/// locationName: the location node, like "Tavern"
		/// sceneName: the current scene within the location, like "Common Room"
		/// implementation formats them as "Tavern – Common Room"
		/// shortcuts (C/Q/I/M for Character/Quests/Inventory/Menu) are always highlighted
		virtual void renderStatusBar(const std::string &playerInfo, const std::string &locationName, const std::string &sceneName) = 0;
		/// reads a single keypress from the player and returns it as a char
		/// valid inputs: '1' to'0' for options, C/Q/I/M for menus
		virtual char getInput() = 0;
};
