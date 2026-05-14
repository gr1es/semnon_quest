#include "GameState.hpp"
#include <iostream>

// constructor
GameState::GameState(const std::string &player_name, const std::string &player_gender,
	const std::string &player_race, const std::string &player_background)
	: _playerName(player_name), _playerGender(player_gender), _playerRace(player_race),
	  _playerBackground(player_background), _level(1), _experience(0)
{
}

// immutable getters
const std::string &GameState::name() const
{
	return (_playerName);
}

const std::string &GameState::gender() const
{
	return (_playerGender);
}

const std::string &GameState::race() const
{
	return (_playerRace);
}

const std::string &GameState::background() const
{
	return (_playerBackground);
}

// location methods
const std::string &GameState::currentLocation() const
{
	return (_currentLocation);
}

void GameState::setCurrentLocation(const std::string &locationId)
{
	_currentLocation = locationId;
}

bool GameState::hasDiscovered(const std::string &locationId) const
{
	return (_discoveredLocations.count(locationId) > 0);
}

void GameState::discoverLocation(const std::string &locationId)
{
	_discoveredLocations.insert(locationId);
}

const std::set<std::string> &GameState::discoveredLocations() const
{
	return (_discoveredLocations);
}

// progression methods
int GameState::level() const
{
	return (_level);
}

int GameState::experience() const
{
	return (_experience);
}

void GameState::addExperience(int amount)
{
	_experience += amount;
	if (_experience < 0)
		_experience = 0;
	// TODO: add level up mechanics here later
	// if (_experience >= XXX)
	//	_level++;
}

// flag methods
bool GameState::getFlag(const std::string &flag) const
{
	auto it = _flags.find(flag);
	if (it != _flags.end())
		return (it->second);
	return (false);
}

void GameState::setFlag(const std::string &flag, bool state)
{
	_flags[flag] = state;
}

// skill methods
int GameState::getSkill(const std::string &skill) const
{
	auto it = _skills.find(skill);
	if (it != _skills.end())
		return (it->second);
	std::cout << "\n!!!\nDEBUG: skill " << skill << " requested but not found.\n!!!\n" << std::endl;
	return (-1);
}

void GameState::modifySkill(const std::string &skill, int delta)
{
	auto it = _skills.find(skill);
	if (it != _skills.end())
	{
		it->second += delta;
		// TODO: implement skill maximum here, if ever so decided
		if (it->second < 0)
			it->second = 0;
	}
	else
		std::cout << "\n!!!\nDEBUG: skill " << skill << " requested but not found.\n!!!\n" << std::endl;
}

// counter methods
int GameState::getCounter(const std::string &key) const
{
	auto it = _counters.find(key);
	if (it != _counters.end())
		return (it->second);
	return (0);
}

void GameState::modifyCounter(const std::string &key, int delta)
{
	_counters[key] += delta;
}

// feat methods
bool GameState::hasFeat(const std::string &feat) const
{
	return (_feats.count(feat) > 0);
}

void GameState::addFeat(const std::string &feat)
{
	_feats[feat] = true;
}

// inventory methods
int GameState::getItem(const std::string &itemId) const
{
	auto it = _inventory.find(itemId);
	if (it != _inventory.end())
		return (it->second);
	return (0);
}

void GameState::modifyInventory(const std::string &itemId, int delta)
{
	auto it = _inventory.find(itemId);
	if (it != _inventory.end())
	{
		it->second += delta;
		if (it->second < 0)
			it->second = 0;
	}
	else if (delta > 0)
		_inventory.insert({ itemId, delta });
}

// faction methods
int GameState::factionStanding(const std::string &factionId) const
{
	auto it = _factionStanding.find(factionId);
	if (it != _factionStanding.end())
		return (it->second);
	return (0);
}

void GameState::modifyFactionStanding(const std::string &factionId, int delta)
{
	_factionStanding[factionId] += delta;
}
