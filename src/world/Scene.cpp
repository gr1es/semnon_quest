#include "Scene.hpp"
#include <stdexcept>

// constructor
Scene::Scene(const std::string &scene_id,
	const std::string &scene_name,
	const std::vector<std::tuple<std::string, std::string, std::string>> &scene_descriptions,
	const std::vector<Option> &options)
	: _id(scene_id), _name(scene_name), _descriptions(scene_descriptions), _options(options)
{
}

// getters
const std::string &Scene::id() const
{
	return (_id);
}

const std::string &Scene::name() const
{
	return (_name);
}

// std::get<0>(vector entry) for flag name
// std::get<1>(vector entry) for description
// std::get<2>(vector entry) for art path
// NOTE: returns the art path assigned to the  flag with the first match in GameState OR default description
const std::string &Scene::getDescription(const GameState &state) const
{
	const std::string *fallback = nullptr;
	for (size_t i = 0; i < _descriptions.size(); i++)
	{
		// assign default description as fallback
		if (std::get<0>(_descriptions[i]) == "")
			fallback = &std::get<1>(_descriptions[i]);
		// if the flag in _descriptions is contained and set to true in GameState's flags
		// --> return that!
		else if (state.getFlag(std::get<0>(_descriptions[i])))
			return (std::get<1>(_descriptions[i]));
	}
	// if no corresponding tuple flag entry was found or it was set to false(getFlag returned false)
	// -> return fallback (default description)
	if (fallback == nullptr)
		throw std::runtime_error("Scene " + _id + " has no default description.");
	return (*fallback);
}

// identical to above
// NOTE: returns the art path assigned to the flag with the first match in GameState
const std::string &Scene::getArtPath(const GameState &state) const
{
	const std::string *fallback = nullptr;
	static const std::string empty = "";
	for (size_t i = 0; i < _descriptions.size(); i++)
	{
		if (std::get<0>(_descriptions[i]) == "")
			fallback = &std::get<2>(_descriptions[i]);
		else if (state.getFlag(std::get<0>(_descriptions[i])))
			return (std::get<2>(_descriptions[i]));
	}
	if (fallback == nullptr)
		return (empty);
	return (*fallback);
}

const std::vector<Option> &Scene::options() const
{
	return (_options);
}
