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
		std::cout << e.what();
	}

	return (0);
}
