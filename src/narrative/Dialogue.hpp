#pragma once

#include <string>
#include <map>
#include "DialogueNode.hpp"

class Dialogue
{
	public:
		Dialogue(const std::string &dialogue_id, 
			const std::string &dialogue_start_node, 
			const std::map<std::string, DialogueNode> &dialogue_nodes);
		const std::string &id() const;
		const std::string &startNode() const;
		const std::map<std::string, DialogueNode> &nodes() const;
		const DialogueNode &getNode(const std::string &node_id) const;

	private:
		const std::string _id;	// identifier
		const std::string _startNode; // id of the standard first node
		const std::map<std::string, DialogueNode> _nodes; // associated nodes, paired with their id, mirroring Location's scenes map
};

