#include "DialogueManager.hpp"
#include <stdexcept>

const Dialogue &DialogueManager::getDialogue(const std::string &dialogue_id) const
{
	auto it = _dialogues.find(dialogue_id);
	if (it != _dialogues.end())
		return (it->second);
	throw std::runtime_error("ERROR: DialogueManager: has no dialogue " + dialogue_id + ".");
}

void DialogueManager::addDialogue(const Dialogue &dialogue)
{
	// does nothing if key already exists
	_dialogues.insert({ dialogue.id(), dialogue });
}

bool DialogueManager::hasDialogue(const std::string &dialogue_id) const
{
	return (_dialogues.count(dialogue_id) > 0);
}
