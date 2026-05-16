#pragma once

#include "Connection.hpp"
#include "Scene.hpp"
#include <map>
#include <string>
#include <vector>

class Location
{
	public:
		Location(const std::string &location_id,
			const std::string &location_name,
			const std::map<std::string, Scene> &scenes,
			const std::string &default_scene_id,
			const std::vector<Connection> &connections);
		// location ID
		const std::string &id() const;
		// location name
		const std::string &name() const;
		const std::vector<Connection> &connections() const;
		// returns the ID of the scene to be presented when none is specified
		const std::string &defaultSceneId() const;
		const Scene &getScene(const std::string &scene_id) const;

	private:
		/// internal ID, "tavern"
		const std::string _id;
		/// player-facing name: "The Prancing Pony"
		const std::string _name;
		// map of ID + Scene object
		const std::map<std::string, Scene> _scenes;
		// ID of standard scene seen when arriving at location
		const std::string _defaultSceneId;
		// vector of connected locations
		const std::vector<Connection> _connections;
};
