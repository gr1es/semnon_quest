#pragma once

#include "Dialogue.hpp"
#include <string>
#include "DialogueManager.hpp"

class DialogueLoader
{
	public:
		static DialogueManager load(const std::string &directory);
};
