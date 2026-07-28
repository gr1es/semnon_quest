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
		// throws if no match for key is found!
		temp.type = stringToStateType(req.at("type"));
		temp.key = req.at("key");
		// min and max below rely on nlohmann's optional support; should work fine on newest versions
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
					if (!art.empty())
						art = "./data/ascii/" + art;
					// the three elements of a SceneVariant are populated: requirements, description, art-path
					// the requirements utilize the helper function written above
					// it gets fed the nlohmann::json for the key "requirements", turned into a vector of Requirement objects
					scene_variants.push_back({
						parseRequirements(var.value("requirements", nlohmann::json::array())),
						var.at("description"),
						art
					});
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
						std::cerr << "WARNING: unknown option type \"" << opt["type"] << "\"\n";
					std::vector<Requirement> opt_requirements;
					std::string opt_flag = opt["required_flag"];
					if (!opt_flag.empty())
						opt_requirements.push_back({ StateType::Flag, opt_flag, std::nullopt, std::nullopt, true });
					scene_options.push_back({ opt["label"], type, opt["target_id"], "", "", opt_requirements, { } });
				}
				// connections
				std::vector<Connection> scene_connections;
				for (const auto &conn : scene["connections"])
				{
					scene_connections.push_back({ conn["label"], conn["destination_location"], conn["destination_scene"], { }, { } });
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
