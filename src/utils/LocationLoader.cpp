#include "LocationLoader.hpp"
#include "ReactivityParser.hpp"
#include <filesystem>
#include <fstream>
#include <iostream> // for error msg
#include <map>
#include <string>
#include <vector>
#include <nlohmann/json.hpp>

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
					// parseRequirements gets fed the nlohmann::json for the key "requirements", turned into a vector of Requirement objects
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
						// streaming a json string directly would print it with its JSON quotes -> extract the plain string
						std::cerr << "WARNING: LocationLoader: unknown OptionType " << (opt["type"].is_string() ? opt["type"].get<std::string>() : opt["type"].dump()) << ".\n";
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
