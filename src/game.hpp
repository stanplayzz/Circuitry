#pragma once
#include "toolbar/inventory.hpp"
#include "world.hpp"

#include <SFML/Graphics.hpp>

class Game {
public:
	Game();

	std::unique_ptr<World> world;


	sf::RenderWindow window;
	sf::View uiView;
	sf::View view;

	float currentZoom = 1.f;
private:
	sf::Font font;
	sf::Text fps;

	sf::Vector2i prevMouse;
	bool panning = false;
	sf::Vector2f originalViewSize;

	void run();
	void handleInput();
};