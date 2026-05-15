#pragma once

#include "Display.hpp"

class TerminalDisplay : public Display
{
	public:
		// functions
		// see Display.hpp for comments
		void clearScreen() override;
		void renderSceneName(const std::string &scene_name) override;
		void renderArt(const std::string &art) override;
		void renderDescription(const std::string &text) override;
		void renderOptions(const std::vector<std::string> &options) override;
		void renderStatusBar(const std::string &player_info, const std::string &location_name, const std::string &scene_name) override;
		char getInput() override;
};
