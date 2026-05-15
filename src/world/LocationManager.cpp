#include "LocationManager.hpp"
#include <stdexcept>

const Location &LocationManager::getLocation(const std::string &location_id) const
{
	auto it = _locations.find(location_id);
	if (it != _locations.end())
		return (it->second);
	throw std::runtime_error("LocationManager: location '" + location_id + "' not found.");
}

void LocationManager::addLocation(const Location &location)
{
	// does nothing if key already exists
	_locations.insert({ location.id(), location });
}

bool LocationManager::hasLocation(const std::string &location_id) const
{
	return (_locations.count(location_id) > 0);
}
