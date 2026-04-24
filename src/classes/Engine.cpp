#include "Engine.hpp"

Engine::Engine() : _window(sf::VideoMode(800, 600), "Semnon Quest"), _isRunning(true)
{
}

Engine::~Engine()
{
}

void Engine::run()
{
    while (_isRunning)
    {
        processEvents();
        updateLogic();
        renderScreen();
    }
}

void Engine::processEvents()
{
    sf::Event event;
    while (_window.pollEvent(event))
    {
        // Handle window close button
        if (event.type == sf::Event::Closed)
        {
            _isRunning = false;
            _window.close();
        }
        
        // Handle keyboard input for global engine shortcuts
        if (event.type == sf::Event::KeyPressed)
        {
            // Allow Esc to instantly quit the game
            if (event.key.code == sf::Keyboard::Escape)
            {
                _isRunning = false;
                _window.close();
            }
        }
    }
}

void Engine::updateLogic()
{
    // Empty for now.
    // Eventually, this will call: _currentState->update();
}

void Engine::renderScreen()
{
    // 1. Clear the previous frame
    _window.clear(sf::Color::Black);

    // 2. Draw the current state (Empty for now)
    // Eventually, this will call: _currentState->draw(_window);

    // 3. Display the new frame
    _window.display();
}
