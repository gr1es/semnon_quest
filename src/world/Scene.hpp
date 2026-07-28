#pragma once

#include "SceneVariant.hpp"
#include "Connection.hpp"
#include "GameState.hpp"
#include "Option.hpp"
#include <string>
#include <vector>

class Scene
{
	public:
		Scene(const std::string &scene_id,
			const std::string &scene_name,
			const std::vector<SceneVariant> &scene_variants,
			const std::vector<Option> &options,
			const std::vector<Connection> &connections);

		// scene ID
		const std::string &id() const;
		// scene name
		const std::string &name() const;
		// returns description of the first variant whose requirements are met; empty requirements = default (throws if no default)
		const std::string &getDescription(const GameState &state) const;
		// returns art of the first variant whose requirements are met; empty requirements = default; may be empty string
		const std::string &getArtPath(const GameState &state) const;
		const std::vector<Option> &options() const;
		const std::vector<Connection> &connections() const;

	private:
		/// internal ID, "common_room"
		const std::string _id;
		/// player-facing name: "Common Room"
		const std::string _name;
		const std::vector<SceneVariant> _variants;
		/// options will not really change, just become (in)visible depending on flags, hence "const"
		const std::vector<Option> _options;
		const std::vector<Connection> _connections;
};
