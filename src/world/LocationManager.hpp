#pragma once

#include "Location.hpp"
#include <map>
#include <string>

class LocationManager
{
	public:
		const Location &getLocation(const std::string &location_id) const;
		void addLocation(const Location &location);
		// this looks up if a location with the entered id is existent before trying to do something with it --> graceful fail instead of undefined behavior if it returns false
		bool hasLocation(const std::string &location_id) const;

	private:
		// map with location IDs and location objects
		std::map<std::string, Location> _locations;
};
