#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

class Engine
{
  public:
	Engine();
	~Engine();

	void run();

  private:
	void processEvents();
	void updateLogic();
	void renderScreen();

	sf::RenderWindow _window;
	bool _isRunning;
};
