#include "Dialogue.hpp"
#include <stdexcept>

Dialogue::Dialogue(const std::string &dialogue_id, 
			const std::string &dialogue_start_node, 
			const std::map<std::string, DialogueNode> &dialogue_nodes)
			: _id(dialogue_id), _startNode(dialogue_start_node), _nodes(dialogue_nodes)
{
	// check for missing starting node to fail early if missing
	if (_startNode.empty())
		throw std::runtime_error("ERROR: Dialogue: " + _id + " has no start node.");
	if (!_nodes.contains(_startNode))
		throw std::runtime_error("ERROR: Dialogue: " + _id + " has no start node " + _startNode + ".");
	// check if dialogue chain is complete (target nodes in Dialogue Responses exist)
	for (const auto &[node_id, node] : _nodes)
		for (const DialogueResponse &response : node.responses)
			if (!response.target_node.empty() && !_nodes.contains(response.target_node))
				throw std::runtime_error("ERROR: Dialogue: " + _id + " has no node " + response.target_node + ", targeted by response \"" + response.label + "\" in node " + node_id + ".");
}

// getters
const std::string &Dialogue::id() const
{
	return (_id);
}

const std::string &Dialogue::startNode() const
{
	return (_startNode);
}

const std::map<std::string, DialogueNode> &Dialogue::nodes() const
{
	return (_nodes);
}

const DialogueNode &Dialogue::getNode(const std::string &node_id) const
{
	auto it = _nodes.find(node_id);
	if (it != _nodes.end())
		return (it->second);
	throw std::runtime_error("ERROR: Dialogue: " + _id + " has no node " + node_id + ".");
}
