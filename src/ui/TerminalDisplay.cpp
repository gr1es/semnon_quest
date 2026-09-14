#include "TerminalDisplay.hpp"
#include <fstream>
#include <iostream>
#include <limits>
#include <cstdio>

void TerminalDisplay::clearScreen()
{
	std::cout << "\033[2J\033[H";
}

void TerminalDisplay::renderSceneName(const std::string &scene_name)
{
	std::cout << scene_name << "\n";
}

void TerminalDisplay::renderArt(const std::string &art_path)
{
	std::string line;
	std::ifstream file;

	file.open(art_path);
	if (!file)
		std::cerr << "WARNING: art path \"" << art_path << "\"couldn't be opened.\n";
	while (std::getline(file, line))
		std::cout << line << "\n";
	file.close();
}

void TerminalDisplay::renderDescription(const std::string &text)
{
	std::cout << text << "\n";
}

void TerminalDisplay::renderMessage(const std::string &text)
{
	std::cout << text << "\n";
}

void TerminalDisplay::renderOptions(const std::vector<std::string> &options)
{
	// size_t i because .size() returns a size_t
	for (size_t i = 0; i < options.size(); i++)
		std::cout << i + 1 << ". " << options[i] << "\n";
}

void TerminalDisplay::renderStatusBar(const std::string &player_info, const std::string &location_name, const std::string &scene_name)
{
	// TODO test whether abbreviation of CJIM elements is necessary
	// REMINDER: this function owns the brackets! don't feed bracketed strings!
	std::cout << "[" << player_info << "]    [" << location_name << " | " << scene_name << "]\n[C]haracter    [J]ournal    [I]nventory    [M]enu\n";
}

char TerminalDisplay::getInput()
{
	char input;
	// on end-of-input (e.g. CTRL+D) the input extraction fails
	// so if the piping into input fails, EOF is returned so calling functions can react can stop
	// without this, input keeps a garbage value and the input loop spins endlessly
	if (!(std::cin >> input))
		return (EOF);
	// .peek() returns the next character in the input sequence, without extracting it
	// this guards against input of >1 characters
	if (std::cin.peek() != '\n' && std::cin.peek() != EOF)
	{
		// this discards everything up to and including the newline
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		return (getInput()); // reprompt by recursing
	}
	return (input);
}
