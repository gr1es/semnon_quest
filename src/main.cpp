#include "Game.hpp"
#include <iostream>
#include <stdexcept>

int main()
{
	Game game;

	try
	{
		game.run();
	}
	catch (const std::exception &e)
	{
		std::cerr << e.what() << "\n";
		return (1);
	}

	return (0);
}
