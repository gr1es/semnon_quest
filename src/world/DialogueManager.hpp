#pragma once

#include "Dialogue.hpp"
#include <map>
#include <string>

class DialogueManager
{
	public:
		const Dialogue &getDialogue(const std::string &dialogue_id) const;
		void addDialogue(const Dialogue &dialogue);
		// this looks up if a dialogue with the entered id is existent before trying to do something with it --> graceful fail instead of undefined behavior if it returns false
		bool hasDialogue(const std::string &dialogue_id) const;

	private:
		std::map<std::string, Dialogue> _dialogues;
};
