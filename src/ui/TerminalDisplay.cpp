#include "TerminalDisplay.hpp"
#include <iostream>

void TerminalDisplay::clearScreen()
{
	std::cout << "\033[2J\033[H";
}

void TerminalDisplay::renderSceneName(const std::string &sceneName)
{
	std::cout << sceneName << "\n";
}

void TerminalDisplay::renderArt(const std::string &art)
{
	std::cout << art << "\n";
}

void TerminalDisplay::renderDescription(const std::string &text)
{
	std::cout << text << "\n";
}

void TerminalDisplay::renderOptions(const std::vector<std::string> &options)
{
	// size_t i because .size() returns a size_t
	for (size_t i = 0; i < options.size(); i++)
		std::cout << i + 1 << ". " << options[i] << "\n";
}

void TerminalDisplay::renderStatusBar(const std::string &playerInfo, const std::string &locationName, const std::string &sceneName)
{
	// TODO test whether abbreviation of CJIM elements is necessary
	// REMINDER: this function owns the brackets! don't feed bracketed strings!
	std::cout << "[ " << playerInfo << " ]\t\t" << "[ " << locationName << " | " << sceneName << " ]\t\t" << "[C]haracter\t[J]ournal\t[I]nventory\t[M]enu\n";
}

char TerminalDisplay::getInput()
{
	char input;
	std::cin >> input;
	return (input);
}
