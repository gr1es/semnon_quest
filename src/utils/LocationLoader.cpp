#include "LocationLoader.hpp"
#include <filesystem>
#include <fstream>
#include <iostream> // for error msg

LocationManager LocationLoader::load(const std::string &directory)
{
	// to be returned object
	LocationManager manager;

	// this looks at all files in the directory ...
	for (const auto &entry : std::filesystem::directory_iterator(directory))
	{
		// ... and acts on those ending in ".json"
		if (entry.path().extension() == ".json")
		{

			std::ifstream file(entry.path());
			/// this creates a json-object with all the keys and values from the file at the entry's path
			/// nesting/hierarchies are properly copied from file
			nlohmann::json parsed = nlohmann::json::parse(file);

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
				// extracting description tuple-vectors
				std::vector<std::tuple<std::string, std::string, std::string>> scene_descriptions;
				for (const auto &desc : scene["descriptions"])
				{
					// art gets its own local variable to enable just having to type the file name into the art section of each location JSON
					std::string art = desc["art"];
					if (!art.empty())
						art = "./data/ascii/" + art;
					scene_descriptions.push_back({ desc["required_flag"], desc["text"], art });
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
					scene_options.push_back({ opt["label"], type, opt["target_id"], "", "", opt["required_flag"] });
				}
				// connections
				std::vector<Connection> scene_connections;
				for (const auto &conn : scene["connections"])
				{
					scene_connections.push_back({ conn["label"], conn["destination_location"], conn["destination_scene"] });
				}
				// --> go into temp Scene
				Scene s(scene_id, scene_name, scene_descriptions, scene_options, scene_connections);
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
