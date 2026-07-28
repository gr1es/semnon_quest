#include "Scene.hpp"
#include "Requirement.hpp"
#include <stdexcept>

// constructor
Scene::Scene(const std::string &scene_id,
	const std::string &scene_name,
	const std::vector<SceneVariant> &scene_variants,
	const std::vector<Option> &options,
	const std::vector<Connection> &connections) : _id(scene_id), _name(scene_name), _variants(scene_variants), _options(options), _connections(connections)
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

const std::string &Scene::getDescription(const GameState &state) const
{
	const std::string *desc = nullptr;
	for (size_t i = 0; i < _variants.size(); i++)
	{
		// assign default description as fallback text
		if (_variants[i].requirements.empty())
			desc = &_variants[i].description;
		// if there are requirements contained and they are fulfilled in the GameState
		// --> return that!
		else if (requirementsMet(_variants[i].requirements, state))
			return (_variants[i].description);
	}
	// if the whole vector of descriptions was searched and there is no default text as fallback, something is wrong
	if (desc == nullptr)
		throw std::runtime_error("Scene " + _id + " has no default description.");
	return (*desc);
}

// mirrors logic of getter above
const std::string &Scene::getArtPath(const GameState &state) const
{
	const std::string *art_path = nullptr;
	static const std::string empty = "";
	for (size_t i = 0; i < _variants.size(); i++)
	{
		if (_variants[i].requirements.empty())
			art_path = &_variants[i].art;
		else if (requirementsMet(_variants[i].requirements, state))
			return (_variants[i].art);
	}
	if (art_path == nullptr)
		return (empty);
	return (*art_path);
}

const std::vector<Option> &Scene::options() const
{
	return (_options);
}

const std::vector<Connection> &Scene::connections() const
{
	return (_connections);
}
