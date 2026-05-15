#pragma once

#include "Scene.hpp"
#include <map>
#include <string>
#include <vector>

// TODO: replace with include later
class Connection;

class Location
{
	public:
		Location(const std::string &location_id,
			const std::string &location_name,
			const std::map<std::string, Scene> &scenes,
			const std::vector<Connection> &connections);
		// location ID
		const std::string &id() const;
		// location name
		const std::string &name() const;
		const std::vector<Connection> &connections() const;
		const Scene &getScene(const std::string &scene_id) const;

	private:
		/// internal ID, "tavern"
		const std::string _id;
		/// player-facing name: "The Prancing Pony"
		const std::string _name;
		const std::vector<Connection> _connections;
		const std::map<std::string, Scene> _scenes;
};
