#pragma once

#include "Display.hpp"

class TerminalDisplay : public Display
{
	public:
		// functions
		// see Display.hpp for comments
		void clearScreen() override;
		void renderSceneName(const std::string &sceneName) override;
		void renderArt(const std::string &art) override;
		void renderDescription(const std::string &text) override;
		void renderOptions(const std::vector<std::string> &options) override;
		void renderStatusBar(const std::string &playerInfo, const std::string &locationName, const std::string &sceneName) override;
		char getInput() override;
};
