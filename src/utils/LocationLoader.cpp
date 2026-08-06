#include "LocationLoader.hpp"
#include <filesystem>
#include <fstream>
#include <iostream> // for error msg
#include <stdexcept>

static StateType stringToStateType(const std::string &s)
{
	if (s == "Flag")
		return (StateType::Flag);
	if (s == "Skill")
		return (StateType::Skill);
	if (s == "Counter")
		return (StateType::Counter);
	if (s == "Feat")
		return (StateType::Feat);
	if (s == "Item")
		return (StateType::Item);
	if (s == "Standing")
		return (StateType::Standing);
	throw(std::invalid_argument("ERROR: LocationManager found no match for StateType."));
}

static std::vector<Requirement> parseRequirements(const nlohmann::json &reqs)
{
	std::vector<Requirement> res;

	for (const nlohmann::json &req : reqs)
	{
		Requirement temp;
		// .at is a nlohmann method that returns the value to the entered key
		// throws if no match for key is found! the throw is desired to ring alarm about broken JSON
		temp.type = stringToStateType(req.at("type"));
		temp.key = req.at("key");
		// min and max below rely on nlohmann's optional support; should work fine on newest versions
		// these fields in temp are only populated when they exist in req
		// alternative that always works:
		// req.at("min").get<int>()
		if (req.contains("min"))
			temp.min = req.at("min");
		if (req.contains("max"))
			temp.max = req.at("max");
		if (req.contains("expected"))
			temp.expected = req.at("expected");
		res.push_back(temp);
	}
	return (res);
}

static std::vector<Effect> parseEffects(const nlohmann::json &effs)
{
	std::vector<Effect> res;

	for (const nlohmann::json &eff : effs)
	{
		Effect temp;

		// same as above: use of .at to generate a throw if required type or key fields do not exist (malformed JSON!)
		temp.type = stringToStateType(eff.at("type"));
		temp.key = eff.at("key");
		// value: reads the value to the entered key and returns it OR, if the key is not there, returns the second argument (in this case 0)
		// this is used here as fallback to default values should the JSON be missing these fields (perfectly fine possibility, hereby handled)
		temp.delta = eff.value("delta", 0);
		temp.flagValue = eff.value("flagValue", true);
		res.push_back(temp);
	}
	return (res);
}


// load() builds entire world with principle: one *.json file = one Location
// file -> scenes -> (variants | options | connections) -> requirements | effects
// each Location object is built bottom up from its children
LocationManager LocationLoader::load(const std::string &directory)
{
	// to be returned object
	LocationManager manager;

	// this looks at all files in the directory ...
	for (const auto &file : std::filesystem::directory_iterator(directory))
	{
		// ... and acts on those ending in ".json"
		if (file.path().extension() == ".json")
		{
			std::ifstream file_stream(file.path());
			/// this creates a json-object with all the keys and values from the file at the entry's path
			/// nesting/hierarchies are properly copied from file
			nlohmann::json parsed = nlohmann::json::parse(file_stream);

			// getting single values from top level (not accessing '"scenes":', for example)
			std::string id = parsed["id"];
			std::string name = parsed["name"];
			std::string default_scene = parsed["default_scene"];

			// constructing temp map of scenes
			std::map<std::string, Scene> scenes;
			// = for the "scene" segment of each JSON file:
			for (const auto &scene : parsed["scenes"])
			{
				// ID + name
				std::string scene_id = scene["id"];
				std::string scene_name = scene["name"];
				// getting the SceneVariant data
				std::vector<SceneVariant> scene_variants;
				for (const auto &var : scene["variants"])
				{
					std::string art = var.at("art");
					// prepend art file name with standard path for ascii artworks
					if (!art.empty())
						art = "./data/ascii/" + art;
					// the three elements of a SceneVariant are populated: requirements, description, art-path
					// the requirements utilize the helper function written above
					// it gets fed the nlohmann::json for the key "requirements", turned into a vector of Requirement objects
					scene_variants.push_back({ parseRequirements(var.value("requirements", nlohmann::json::array())),
						var.at("description"),
						art });
				}
				// options
				std::vector<Option> scene_options;
				for (const auto &opt : scene["options"])
				{
					// can't pass string from JSON's "type"
					// because Option class expects an enum
					// --> if/else to pass enum Dialogue/Action
					OptionType type;
					if (opt["type"] == "Dialogue")
						type = OptionType::Dialogue;
					else if (opt["type"] == "Action")
						type = OptionType::Action;
					else
					{
						std::cerr << "WARNING: unknown option type \"" << opt["type"] << "\"\n";
						continue;
					}
					std::vector<Requirement> opt_requirements = parseRequirements(opt.value("requirements", nlohmann::json::array()));
					std::vector<Effect> opt_effects = parseEffects(opt.value("effects", nlohmann::json::array()));

					// "" are unused destination fields
					// they are used in separate movement related Option objects and generated during runtime
					scene_options.push_back({ opt["label"], type, opt["target_id"], "", "", opt_requirements, opt_effects });
				}
				// connections
				std::vector<Connection> scene_connections;
				for (const auto &conn : scene["connections"])
				{
					// prepping temp values to later push into scene_connections
					std::vector<Requirement> conn_requirements = parseRequirements(conn.value("requirements", nlohmann::json::array()));
					std::vector<Effect> conn_effects = parseEffects(conn.value("effects", nlohmann::json::array()));

					scene_connections.push_back({ conn["label"], conn["destination_location"], conn["destination_scene"], conn_requirements, conn_effects });
				}
				// --> go into temp Scene
				Scene s(scene_id, scene_name, scene_variants, scene_options, scene_connections);
				// --> go into temp Scenes map
				scenes.insert({ scene_id, s });
			}
			// --> scenes go into temp Location
			Location loc(id, name, scenes, default_scene);
			// --> go into manager
			manager.addLocation(loc);
		}
	}
	// populated LocationManager gets returned
	// contains parsed data of every JSON file in ./data/locations/ !
	return (manager);
}
