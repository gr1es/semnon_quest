#include "LocationLoader.hpp"
#include "OptionType.hpp"
#include "ReactivityParser.hpp"
#include <filesystem>
#include <fstream>
#include <map>
#include <stdexcept>
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
			// mainly nlohmann's exceptions are caught here: broken syntax / unreadable file, missing required key, value of the wrong type
			// own logic that might throw:
			// stringToOptionType() and stringToStateType() (through parseRequirements/parseEffects) if string does not correspond with an enum
			// if a Move enum is about to become part of a scene's option (belongs to Connection!)
			// if there is an error constructing the Location object (no default scene set or found)
			try
			{
				std::ifstream file_stream(file.path());
				/// this creates a json-object with all the keys and values from the file at the entry's path
				/// nesting/hierarchies are properly copied from file
				nlohmann::json parsed = nlohmann::json::parse(file_stream);

				// getting single values from top level (not accessing '"scenes":', for example)
				std::string id = parsed.at("id");
				std::string name = parsed.at("name");
				std::string default_scene = parsed.at("default_scene");

				// constructing temp map of scenes
				std::map<std::string, Scene> scenes;
				// = for the "scene" segment of each JSON file:
				for (const auto &scene : parsed.at("scenes"))
				{
					// ID + name
					std::string scene_id = scene.at("id");
					std::string scene_name = scene.at("name");
					// getting the SceneVariant data
					std::vector<SceneVariant> scene_variants;
					for (const auto &var : scene.at("variants"))
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
					for (const auto &opt : scene.at("options"))
					{
						// converting string from JSON to OptionType enum and saving it
						OptionType type = stringToOptionType(opt.at("type"));
						// .get() is needed because a JSON value can't be directly added to a string via "+"
						if (type == OptionType::Move)
							throw std::runtime_error("ERROR: LocationLoader: option \"" + opt.at("label").get<std::string>() + "\" in scene " + scene_id + " can't have OptionType Move, as movement belongs in connections.");
						std::vector<Requirement> opt_requirements = parseRequirements(opt.value("requirements", nlohmann::json::array()));
						std::vector<Effect> opt_effects = parseEffects(opt.value("effects", nlohmann::json::array()));

						// "" are unused destination fields
						// they are used in separate movement related Option objects and generated during runtime
						scene_options.push_back({ opt.at("label"), type, opt.at("target_id"), "", "", opt_requirements, opt_effects });
					}
					// connections
					std::vector<Connection> scene_connections;
					for (const auto &conn : scene.at("connections"))
					{
						// prepping temp values to later push into scene_connections
						std::vector<Requirement> conn_requirements = parseRequirements(conn.value("requirements", nlohmann::json::array()));
						std::vector<Effect> conn_effects = parseEffects(conn.value("effects", nlohmann::json::array()));

						scene_connections.push_back({ conn.at("label"), conn.at("destination_location"), conn.at("destination_scene"), conn_requirements, conn_effects });
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
			catch (const std::exception &e)
			{
				std::string msg = e.what();
				// add usual prefix to error message if not already there
				if (!msg.starts_with("ERROR: "))
					msg = "ERROR: LocationLoader: " + msg;
				// remove ending period since more error message info is appended
				if (msg.ends_with('.'))
					msg.pop_back();
				// add info concerning file that contains flawed content
				throw std::runtime_error(msg + ", in file " + file.path().filename().string() + ".");
			}
		}
	}
	// populated LocationManager gets returned
	// contains parsed data of every JSON file in ./data/locations/ !
	return (manager);
}
