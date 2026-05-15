#pragma once // a shortform of the usual header guard ("#ifndef ...")

#include <map>
#include <set>
#include <string>

class GameState
{
	public:
		// constructor
		GameState(const std::string &player_name, const std::string &player_gender,
			const std::string &player_race, const std::string &player_background);
		// immutable getters
		const std::string &name() const;
		const std::string &gender() const;
		const std::string &race() const;
		const std::string &background() const;
		// location and scene methods
		const std::string &currentLocation() const;
		void setCurrentLocation(const std::string &locationId);
		const std::string &currentScene() const;
		void setCurrentScene(const std::string &sceneId);
		bool hasDiscovered(const std::string &locationId) const;
		void discoverLocation(const std::string &locationId);
		const std::set<std::string> &discoveredLocations() const;
		// progression methods
		int level() const;
		int experience() const;
		void addExperience(int amount);
		// flag methods
		bool getFlag(const std::string &flag) const;
		void setFlag(const std::string &flag, bool state);
		// skill methods
		int getSkill(const std::string &skill) const;
		void modifySkill(const std::string &skill, int delta);
		// counter methods
		int getCounter(const std::string &key) const;
		void modifyCounter(const std::string &key, int delta);
		// feat methods
		bool hasFeat(const std::string &feat) const;
		void addFeat(const std::string &feat);
		// inventory methods
		int getItem(const std::string &itemId) const;
		void modifyInventory(const std::string &itemId, int delta);
		// faction methods
		int factionStanding(const std::string &factionId) const;
		void modifyFactionStanding(const std::string &factionId, int delta);

	private:
		const std::string _playerName;
		const std::string _playerGender;
		const std::string _playerRace;
		const std::string _playerBackground;
		std::string _currentLocation;
		std::string _currentScene; ///< scene within the current location (e.g. "common_room" inside "tavern")
		std::set<std::string> _discoveredLocations;
		int _level;
		int _experience; ///<  cannot go negative
		std::map<std::string, bool> _flags;
		std::map<std::string, int> _skills; ///< cannot go negative
		std::map<std::string, int> _counters; ///< can go negative
		std::map<std::string, bool> _feats;
		std::map<std::string, int> _inventory; ///< cannot go negative
		std::map<std::string, int> _factionStanding; ///< can go negative
};
