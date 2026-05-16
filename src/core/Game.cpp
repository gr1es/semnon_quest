#include "Game.hpp"
#include <iostream>

// initializes GameState with placeholder player data
// TODO: remove hardcoded parameters
Game::Game() : _gameState("Aldric", "male", "human", "ranger")
{
}

// unlike _terminalDisplay, _locationManager holds data (TODO later!) and needs to be constructed
static LocationManager buildLocations()
{
	LocationManager manager;
	return (manager);
}

void Game::run()
{
	_locationManager = buildLocations();
	// TODO: current location and scene need to be defined by data from savegame
	_gameState.setCurrentLocation("pracing_pony");
	_gameState.setCurrentScene("common_room");

	while (true)
	{
		const Location &loc = _locationManager.getLocation(_gameState.currentLocation());
		const Scene &scene = loc.getScene(_gameState.currentScene());
		// local Option vector to collect actually available options for player
		const std::vector<Option> options = buildOptions(scene);
		renderScene(loc, scene, options);
		if (!handleInput(options))
			break;
	}
}

std::vector<Option> Game::buildOptions(const Scene &scene) const
{
	std::vector<Option> options;

	// add dialogue options, only those with required flags
	for (const Option &opt : scene.options())
		if (opt.type == OptionType::Dialogue && (_gameState.getFlag(opt.required_flag) || opt.required_flag.empty()))
			options.push_back(opt);
	// add action options, only those with required flags
	for (const Option &opt : scene.options())
		if (opt.type == OptionType::Action && (_gameState.getFlag(opt.required_flag) || opt.required_flag.empty()))
			options.push_back(opt);
	// add movement options
	for (const Connection &conn : scene.connections())
		options.push_back({ conn.label, OptionType::Move, "", conn.destination_location, conn.destination_scene, "" });
	// TODO: add filtering for invisible options as soon as requires + effects are added to Connections

	// check if more than 10 options -> options 11+ are un-selectable by player
	if (options.size() > 9)
		std::cerr << "WARNING: scene \"" << scene.id() << "\" has more than 9 options -- excess options unreachable\n";

	return (options);
}

void Game::renderScene(const Location &loc, const Scene &scene, const std::vector<Option> &options)
{
	_terminalDisplay.clearScreen();
	_terminalDisplay.renderSceneName(scene.name());
	if (!scene.getArtPath(_gameState).empty())
		_terminalDisplay.renderArt(scene.getArtPath(_gameState));
	_terminalDisplay.renderDescription(scene.getDescription(_gameState));

	// get labels from local collection of available options
	std::vector<std::string> labels;
	for (const Option &opt : options)
		labels.push_back(opt.label);
	_terminalDisplay.renderOptions(labels);

	// assemble player info here so it stays up-to-date
	std::string player_info = _gameState.name() + " | " + _gameState.race() + " | Lv. " + std::to_string(_gameState.level());
	_terminalDisplay.renderStatusBar(player_info, loc.name(), scene.name());
}

// returns false on EOF to signal the outer loop to exit
bool Game::handleInput(const std::vector<Option> &options)
{
	while (true)
	{
		char input = _terminalDisplay.getInput();
		if (input == EOF)
			return (false);
		if (!isdigit(input) && input != 'C' && input != 'J' && input != 'I' && input != 'M')
			continue;
		// TODO: logic for CJIM input

		size_t choice = input - '1'; // the same as: (input - '0') - 1; stoi and substraction
		// reprompt if choice is outside options range
		if (choice >= options.size())
			continue;

		switch (options[choice].type)
		{
		case OptionType::Dialogue:
			// TODO: add execution
			break;
		case OptionType::Action:
			// TODO: add execution
			break;
		case OptionType::Move:
		{
			const Option &chosen = options[choice];
			if (chosen.destination_location.empty() && chosen.destination_scene.empty())
			{
				std::cerr << "WARNING: Move option \"" << chosen.label << "\" has no destination\n";
				break;
			}
			// we need to move to a new location, location and scene changes
			if (!chosen.destination_location.empty())
			{
				_gameState.setCurrentLocation(chosen.destination_location);
				_gameState.discoverLocation(chosen.destination_location);
				// new Location variable for readability's sake
				const Location &new_loc = _locationManager.getLocation(chosen.destination_location);
				// ternary: if destination_scene is empty, use defaultSceneId(), otherwise use destination_scene.
				_gameState.setCurrentScene(chosen.destination_scene.empty() ? new_loc.defaultSceneId() : chosen.destination_scene);
			}
			// we stay at the same location, just scene changes
			else
				_gameState.setCurrentScene(chosen.destination_scene);
			break;
		}
		}
		return (true);
	}
}
