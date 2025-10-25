#pragma once
#include "toolbar/inventory.hpp"
#include "world.hpp"
#include "states/stateManager.hpp"

#include <SFML/Graphics.hpp>

class Game {
public:
	Game();
	void run();
	StateManager stateManager;

	sf::RenderWindow window;
	sf::View uiView;
	sf::View view;
private:

	sf::Font font;
	sf::Text fps;
	void handleInput();
};