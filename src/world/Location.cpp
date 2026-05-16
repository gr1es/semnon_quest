#include "Location.hpp"
#include <stdexcept>

// constructor
Location::Location(
	const std::string &location_id,
	const std::string &location_name,
	const std::map<std::string, Scene> &scenes,
	const std::string &default_scene_id)
	: _id(location_id),
	  _name(location_name),
	  _scenes(scenes),
	  _defaultSceneId(default_scene_id)
{
}

// getters
const std::string &Location::id() const
{
	return (_id);
}

const std::string &Location::name() const
{
	return (_name);
}

const std::string &Location::defaultSceneId() const
{
	return (_defaultSceneId);
}

// returns the scene matching scene_id; throws if not found
const Scene &Location::getScene(const std::string &scene_id) const
{
	auto it = _scenes.find(scene_id);
	if (it != _scenes.end())
		return (it->second);
	throw std::runtime_error("Location " + _id + " has no scene " + scene_id);
}
