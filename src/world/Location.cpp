#include "Location.hpp"
#include <stdexcept>

// constructor
Location::Location(const std::string &location_id,
	const std::string &location_name,
	const std::map<std::string, Scene> &scenes,
	const std::vector<Connection> &connections)
	: _id(location_id), _name(location_name), _scenes(scenes), _connections(connections)
{
}

// getters
const std::string &Location::id() const
{
	return(_id);
}

const std::string &Location::name() const
{
	return (_name);
}

const std::vector<Connection> &Location::connections() const
{

}

// returns the scene matching sceneId; throws if not found
const Scene &Location::getScene(const std::string &sceneId) const
{
	auto it = _scenes.find(sceneId);
	

}
