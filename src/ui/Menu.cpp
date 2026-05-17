#include "Menu.hpp"
#include <iostream>
#include <limits>

Menu::Menu(Display &display, bool is_ingame)
	: _display(display), _isIngame(is_ingame)
{
	// build entry list based on context
	if (_isIngame)
		_entries.push_back("CONTINUE");
	_entries.push_back("NEW GAME");
	_entries.push_back("SETTINGS");
	_entries.push_back("CREDITS");
	_entries.push_back("EXIT");
}

void Menu::renderEntries() const
{
	_display.renderOptions(_entries);
}

Choice Menu::getChoice() const
{
	char input = _display.getInput();
	if (input == EOF)
		return (Choice::Quit);
	if (!isdigit(input) || !(input >= '1' && (input - '0') <= _entries.size()))
		return (getChoice());
	else
	{
		int choice = input - '0';
		// accessing menu ingame means that there is a CONTINUE button
		// input 1 then means enum slot 0 (CONTINUE)
		// --> input 1 has to be substracted from the return value
		if (_isIngame)
			return (static_cast<Choice>(choice - 1));
		// not ingame = no CONTINUE button
		// --> input 1 means enum slot 1 (NEW GAME), no change necessary
		else
			return (static_cast<Choice>(choice));
		// TODO: all of this has to be revised when implementing savegames
	}
}
