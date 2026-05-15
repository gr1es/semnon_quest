#pragma once

#include "../core/GameState.hpp"
#include <string>
#include <tuple>
#include <vector>

class Scene
{
	public:
		Scene(const std::string &scene_id,
			const std::string &scene_name,
			const std::vector<std::tuple<std::string, std::string, std::string>> &scene_descriptions);
		// scene ID
		const std::string &id() const;
		// scene name
		const std::string &name() const;
		// returns description text matching the first active flag in GameState; empty flag = default
		const std::string &getDescription(const GameState &state) const;
		// returns art path matching the first active flag in GameState; may be empty string
		const std::string &getArtPath(const GameState &state) const;

	private:
		/// internal ID: "common_room"
		const std::string _id;
		/// player-facing name: "Common Room"
		const std::string _name;
		// this is a vector (iterable list, vector[i]) of tuples (fixed sized bundle, accessable with std::get<COLUMN NBR (0-3)>(TUPLE))
		/// each entry: (flag_name, description_text, art_path)
		/// flag_name == "" → default entry, shown when no other flag matches
		/// art_path == "" → no art for this state
		const std::vector<std::tuple<std::string, std::string, std::string>> _descriptions;
};
